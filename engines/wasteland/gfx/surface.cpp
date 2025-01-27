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

#include "common/system.h"
#include "graphics/palette.h"
#include "graphics/paletteman.h"
#include "wasteland/gfx/surface.h"
#include "wasteland/engine.h"

namespace Wasteland {
namespace Shared {
namespace Gfx {

#define FONT_COLOR 15

Surface::Surface() : Graphics::ManagedSurface() {
	_currentFont = g_engine->_fonts[0];
}

Surface::Surface(ManagedSurface &surf, const Common::Rect &bounds) :
	Graphics::ManagedSurface(surf, bounds) {
	_currentFont = g_engine->_fonts[0];
}

void Surface::setupPalette() {
	Graphics::Palette ega = Graphics::Palette::createEGAPalette();
	g_system->getPaletteManager()->setPalette(ega);

	uint32 white = 0xffffffff;
	g_system->getPaletteManager()->setPalette((const byte *)&white, 255, 1);
}

void Surface::setFont(int fontNum) {
	_currentFont = g_engine->_fonts[fontNum];
}

void Surface::writeString(const Common::String &str) {
	StringArray lines;
	lines.split(str, '\n', this->w / FONT_W);

	for (uint lineNum = 0; lineNum < lines.size(); ++lineNum) {
		if (lineNum > 0) {
			_textX = 0;
			++_textY;
		}

		// Iterate over the characters
		int x = _textX * FONT_W;
		int y = _textY * FONT_H;
		const Common::String &line = lines[lineNum];

		for (uint i = 0; i < line.size(); ++i) {
			unsigned char c = line[i];

			if (c == 1) {
				// Toggle inverse color
				setInverseColor(!_inverseColor);
			} else if (x < this->w) {
				_currentFont->drawChar(this, c, x, y,
					_inverseColor ? -FONT_COLOR : FONT_COLOR);
				x += FONT_W;
			}
		}

		_textX += lines[lineNum].size();
	}
}

void Surface::writeString(const Common::String &str, int x, int y) {
	_textX = x;
	_textY = y;
	writeString(str);
}

void Surface::writeString(const unsigned char *str, int x, int y) {
	_textX = x;
	_textY = y;
	Common::String s((const char *)str);
	writeString(s);
}

void Surface::writeCenteredString(const Common::String &str, int y) {
	int x = ((this->w / FONT_W) - str.size()) / 2;
	writeString(str, x, y);
}

void Surface::writeChar(unsigned char c) {
	_currentFont->drawChar(this, c, _textX * FONT_W, _textY * FONT_H,
		_inverseColor ? -FONT_COLOR : FONT_COLOR);
	++_textX;
}

void Surface::writeChar(unsigned char c, int x, int y) {
	setTextPos(x, y);
	writeChar(c);
}

void Surface::setTextPos(int x, int y) {
	_textX = x;
	_textY = y;
}

} // namespace Gfx
} // namespace Shared
} // namespace Wasteland
