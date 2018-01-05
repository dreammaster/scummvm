/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "legend/gfx/visual_surface.h"
#include "legend/legend.h"

namespace Legend {
namespace Gfx {

void FontDetails::reset() {
	_fontNumber = 1;
	_horizSpacings = -1;
	_allowsPendingText = false;
}

/*-------------------------------------------------------------------*/

VisualSurface::VisualSurface(const Graphics::ManagedSurface &src,
		FontDetails &fontDetails, const Common::Rect &bounds) :
		Graphics::ManagedSurface(src), _font(fontDetails), _bounds(bounds) {
}

Font *VisualSurface::loadFont(int fontNumber) {
	_font.reset();
	Font *font = Font::loadFont(fontNumber);
	assert(font);

	_font._fontNumber = font->_fontNumber;
	_font._fontCenter = Common::Point(font->_xCenter, font->_yCenter);
	return font;
}

Font *VisualSurface::getFont() {
	if (!Font::_activeFont || Font::_activeFont->_fontNumber != _font._fontNumber) {
		assert(_font._fontNumber > 0);
		loadFont(_font._fontNumber);
	}

	return Font::_activeFont;
}

void VisualSurface::setFontColor(int fgColor, int bgColor) {
	_font._fgColor = fgColor;
	_font._bgColor = bgColor;
}

int VisualSurface::writeString(const String &text) {
	// Ensure the correct font is active
	Font *font = getFont();

	// Set the color
	font->setColor(_font._fgColor, _font._bgColor);

	// Prefix any pending text
	Common::String msg = !_font._allowsPendingText ? text.c_str() : (_font._pendingText + text).c_str();

	// Main loop for writing text
	int result = 0;
	const char *startP = msg.c_str(), *endP = msg.c_str();
	for (;;) {
		endP = startP;

		if (*endP) {
			// Scan for end of next word
			for (; *endP; ++endP) {
				if (*endP == ' ' || *endP == '\t' || *endP == '\n' || *endP == '-')
					break;
			}
		}

		if (!*startP || (!*endP && _font._allowsPendingText)) {
			// Finished string
			if (endP != startP) {
				if (_font._allowsPendingText && (endP - startP) < 16) {
					_font._pendingText = Common::String(startP, endP);
				} else {
					font->writeString(*this, _font._writePos, Common::String(startP));
					result += endP - startP;
				}
			}

			// Check for end of line
			if (_font._writePos.x >= (int)(this->w - font->_xCenter))
				newLine();

			break;
		}

		if (*endP == '-')
			++endP;

		uint wordWidth = font->stringWidth(Common::String(startP, endP));
		uint width = this->w - font->_xCenter;
		if ((width - font->_xCenter) >= wordWidth) {
			width -= _font._writePos.x;

			if (wordWidth < width) {
				result += width / font->_maxCharWidth;
				if (newLine())
					break;
			}
		} else {
			width -= _font._writePos.x;
			width = 0;
			endP = startP;

			// Figure out allowable word width to fit in surface
			for (; *endP && (font->charWidth(*endP) + wordWidth) <= width;
					wordWidth += font->charWidth(*endP), ++endP) ;
		}

		// Display the word
		Common::String word(startP, endP);
		font->writeString(*this, _font._writePos, word);

		_font._writePos.x += wordWidth;
		result += word.size();

		if (*endP == '\t') {
			// Word ended by tab character
			++endP;
			startP = endP;

			// Ensure there's enough remaining width for a tab
			width = this->w - _font._writePos.x - font->_xCenter;
			if (width >= font->_tabWidth) {
				int xPos = ((_font._writePos.x - font->_xCenter) / font->_tabWidth + 1) * font->_tabWidth;
				_font._writePos.x = xPos + font->_xCenter;
				result += xPos / font->_maxCharWidth;
			}
		} else if (*endP == '\n') {
			// Newline character
			startP = ++endP;
			if (newLine())
				break;
		} else if (*endP == ' ') {
			// Space character
			endP = startP;
		} else {
			// All other characters
			if (*endP)
				++endP;
			startP = endP;

			wordWidth = font->charWidth(' ');
			width = this->w - _font._writePos.x - font->_xCenter;
			if (wordWidth <= width) {
				font->writeString(*this, _font._writePos, " ");
				++result;
			} else {
				if (newLine())
					break;
			}
		}
	}

	// Return number of characters written
	return result;
}

void VisualSurface::writeString(const Common::Point &pt, const String &msg) {
	setTextPos(pt);
	writeString(msg);
}

int VisualSurface::stringWidth(const String &msg) {
	Font *font = Font::_activeFont;
	return font->stringWidth(msg);
}

bool VisualSurface::newLine() {
	// TODO
	return false;
}

void VisualSurface::blitFrom(const Graphics::Surface &src, const Region &r) {
	Common::Point pt(r.left - _bounds.left, r.top - _bounds.top);
	blitFrom(src, pt);
}

VisualSurface VisualSurface::getSubArea(const Common::Rect &r) {
	Common::Rect newBounds(r.left + _bounds.left, r.top + _bounds.top,
		r.right + _bounds.left, r.bottom + _bounds.top);
	Graphics::ManagedSurface src(Screen::get(), newBounds);
	return VisualSurface(src, _font, newBounds);
}

} // End of namespace Gfx
} // End of namespace Legend
