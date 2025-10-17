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

#include "wizardry/gfx/surface.h"
#include "wizardry/data/globals.h"

namespace Wizardry {

constexpr int CHAR_SIZE = 8;

void Surface::PRINTCHR(char c) {
	Common::String str(c);
	PRINTSTR(str);
}

void Surface::PRINTSTR(const Common::String &str) {
	_G(font)->drawString(this, str, _textPos.x * CHAR_SIZE, _textPos.y * CHAR_SIZE,
		320 - (_textPos.x * CHAR_SIZE), _textColor);
	_textPos.x += str.size();
}

void Surface::CLRRECT(int x1, int y1, int x2, int y2) {
	fillRect(Common::Rect(x1 * CHAR_SIZE, y1 * CHAR_SIZE, x2 * CHAR_SIZE, y2 * CHAR_SIZE), 0);
}

void Surface::setGraphicFont(bool gfxMode) {
	_G(font)->setGfxMode(gfxMode);
}

} // namespace Wizardry
