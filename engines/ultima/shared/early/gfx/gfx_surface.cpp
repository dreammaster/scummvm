/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "ultima/shared/early/gfx/gfx_surface.h"
#include "ultima/shared/early/gfx/dos_font.h"
#include "ultima/shared/early/core/events.h"

namespace Ultima {
namespace Shared {
namespace Gfx {

GfxSurface::GfxSurface(Graphics::Font *font) : Graphics::ManagedSurface(),
	_font(font ? font : new DosFont()) {
}

GfxSurface::GfxSurface(Graphics::ManagedSurface &surf, const Common::Rect &bounds, Graphics::Font *font) :
	Graphics::ManagedSurface(surf, bounds), _font(font ? font : new DosFont()) {
}

void GfxSurface::writeString(const Common::Point &pt, const Common::String &str,
		Graphics::TextAlign align) {
	_textPos = pt;
	writeString(str, align);
}

void GfxSurface::writeString(const Common::String &str, Graphics::TextAlign align) {
	size_t strSize = 0;
	for (const char *p = str.c_str(); *p; ++p)
		strSize += Common::isPrint(*p) ? 1 : 0;

	switch (align) {
	case Graphics::kTextAlignCenter:
		_textPos.x -= strSize / 2;
		break;
	case Graphics::kTextAlignRight:
		_textPos.x -= strSize;
		break;
	case Graphics::kTextAlignLeft:
	default:
		break;
	}

	for (const char *p = str.c_str(); *p; ++p) {
		if (*p == '\n') {
			assert(align == Graphics::kTextAlignLeft);
			newLine();
		} else {
			writeChar(*p);
		}
	}
}

void GfxSurface::writeChar(uint32 chr) {
	if (chr < 32) {
		setColor((byte)chr);
		return;
	}

	const int fw = _font->getMaxCharWidth();
	const int fh = _font->getFontHeight();
	Common::Rect r(fw, fh);
	r.translate(_textPos.x * fw, _textPos.y * fh);

	fillRect(r, _bgColor);
	_font->drawChar(this, chr, r.left, r.top, _fgColor);
	++_textPos.x;

	if ((_textPos.x * _font->getMaxCharWidth()) >= this->w)
		newLine();
}

void GfxSurface::newLine() {
	const int LINE_H = _font->getFontHeight();
	_textPos.x = 0;
	_textPos.y++;

	if ((_textPos.y * LINE_H) >= this->h) {
		_textPos.y = (this->h / LINE_H) - 1;

		if (_scrollable) {
			// Scroll the screen contents up
			blitFrom(*this, Common::Rect(0, LINE_H, this->w, this->h),
				Common::Point(0, 0));
			fillRect(Common::Rect(0, this->h - LINE_H, this->w, this->h), 0);
		}
	}
}

void GfxSurface::setTextPos(const Common::Point &pt) {
	_textPos = pt;
}

byte GfxSurface::setColor(byte color) {
	byte oldColor = _fgColor;
	_fgColor = color;
	return oldColor;
}

byte GfxSurface::setColor(byte r, byte g, byte b) {
	byte color = Core::g_events->_palette.findBestColor(r, g, b);
	return setColor(color);
}

void GfxSurface::setColor(byte fgColor, byte bgColor) {
	setColor(fgColor);
	setBgColor(bgColor);
}

byte GfxSurface::setBgColor(byte color) {
	byte oldColor = _bgColor;
	_bgColor = color;
	return oldColor;
}

byte GfxSurface::setBgColor(byte r, byte g, byte b) {
	byte color = Core::g_events->_palette.findBestColor(r, g, b);
	return setBgColor(color);
}

void GfxSurface::reverseColor() {
	SWAP(_fgColor, _bgColor);
}

} // namespace Gfx
} // namespace Shared
} // namespace Ultima
