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

#include "common/str.h"
#include "ultima/ultima5/logic/text_grid.h"

namespace Ultima {
namespace Ultima5 {

constexpr byte GLYPH_FRAME_TOP_LEFT = 0x10;
constexpr byte GLYPH_FRAME_TOP = 0x11;
constexpr byte GLYPH_FRAME_TOP_RIGHT = 0x13;
constexpr byte GLYPH_FRAME_BOTTOM_LEFT = 0x14;
constexpr byte GLYPH_FRAME_BOTTOM = 0x15;
constexpr byte GLYPH_FRAME_BOTTOM_RIGHT = 0x16;
constexpr byte GLYPH_FRAME_SIDE = 0x17;
constexpr int FRAME_WIDTH = 15;

void TextGrid::clear() {
	for (int y = 0; y < ROWS; ++y) {
		for (int x = 0; x < COLUMNS; ++x)
			_cells[y][x] = Cell();
	}

	_x = _y = 0;
}

void TextGrid::printChar(char ch) {
	if (ch == '\n' || ch == '\r') {
		_x = 0;
		++_y;
		return;
	}

	if (_y < ROWS && _x < COLUMNS) {
		Cell &c = _cells[_y][_x];
		c._ch = (byte)ch;
		c._rune = _rune;
		c._inverse = _inverse;
		c._set = true;
	}

	if (++_x >= COLUMNS) {
		_x = 0;
		++_y;
	}
}

void TextGrid::print(const char *text) {
	printString(text, COLUMNS - 1);
}

void TextGrid::printNumber(int value, int width, char filler) {
	Common::String text = Common::String::format("%d", value);

	while ((int)text.size() < width)
		text = Common::String(filler) + text;

	print(text.c_str());
}

void TextGrid::drawFrame(int bottom) {
	setPosition(0, 0);
	printChar(GLYPH_FRAME_TOP_LEFT);

	for (int i = 0; i < FRAME_WIDTH - 2; ++i)
		printChar(GLYPH_FRAME_TOP);

	printChar(GLYPH_FRAME_TOP_RIGHT);

	for (int y = 1; y != bottom; ++y) {
		setPosition(0, y);
		printChar(GLYPH_FRAME_SIDE);
		setPosition(FRAME_WIDTH - 1, y);
		printChar(GLYPH_FRAME_SIDE);
	}

	printChar('\n');
	printChar(GLYPH_FRAME_BOTTOM_LEFT);

	for (int i = 0; i < FRAME_WIDTH - 2; ++i)
		printChar(GLYPH_FRAME_BOTTOM);

	printChar(GLYPH_FRAME_BOTTOM_RIGHT);
}

} // namespace Ultima5
} // namespace Ultima
