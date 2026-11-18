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

#include "ultima/ultima5/views/view.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {

constexpr byte GLYPH_CAP_RIGHT = 1;
constexpr byte GLYPH_CAP_LEFT = 2;
constexpr byte GLYPH_CURSOR = 5;

void View::drawCell(int col, int row, byte ch, byte fg, byte bg) {
	auto s = getSurface();
	s.setColor(fg, bg);
	s.writeChar(Common::Point(col, row), ch);
}

void View::print(int col, int row, const Common::String &text, byte fg, byte bg) {
	auto s = getSurface();
	s.setColor(fg, bg);
	s.writeString(Common::Point(col, row), text);
}

void View::drawCursor(int col, int row, int cursorFrame) {
	drawCell(col, row, GLYPH_CURSOR + (cursorFrame & 3), COLOR_WHITE, COLOR_BLACK);
}

void View::drawCaps(int x, int y, bool left) {
	drawCell(x / 8, y / 8, left ? GLYPH_CAP_LEFT : GLYPH_CAP_RIGHT, COLOR_BLUE, COLOR_BLACK);

	auto s = getSurface();
	if (left) {
		s.drawLine(x, y, x + 5, y + 3, COLOR_WHITE);
		s.drawLine(x + 5, y + 4, x, y + 7, COLOR_WHITE);
	} else {
		s.drawLine(x + 7, y, x + 2, y + 3, COLOR_WHITE);
		s.drawLine(x + 2, y + 4, x + 7, y + 7, COLOR_WHITE);
	}
}

} // namespace Views
} // namespace Ultima5
} // namespace Ultima
