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
#include "ultima/ultima5/ultima5.h"

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

constexpr int WINDOW_COLUMN = 24;
constexpr int WINDOW_WIDTH = 15;
constexpr int LABEL_CENTER = 30;
constexpr byte GLYPH_FRAME_TOP_LEFT = 0x10;
constexpr byte GLYPH_FRAME_TOP = 0x11;
constexpr byte GLYPH_FRAME_TOP_RIGHT = 0x13;
constexpr byte GLYPH_FRAME_BOTTOM_LEFT = 0x14;
constexpr byte GLYPH_FRAME_BOTTOM = 0x15;
constexpr byte GLYPH_FRAME_BOTTOM_RIGHT = 0x16;
constexpr byte GLYPH_FRAME_SIDE = 0x17;
constexpr byte GLYPH_ARROW_UP = 0x18;
constexpr byte GLYPH_ARROW_DOWN = 0x19;
constexpr byte GLYPH_ARROWS = 0x12;

void View::drawRune(int col, int row, byte ch, byte fg, byte bg) {
	auto s = getSurface();
	s.fillRect(Common::Rect(col * 8, row * 8, col * 8 + 8, row * 8 + 8), bg);
	g_engine->_runes->drawChar(s.surfacePtr(), ch, col * 8, row * 8, fg);
}

void View::drawWindowFrame(int top, int bottom) {
	const int right = WINDOW_COLUMN + WINDOW_WIDTH - 1;

	drawCell(WINDOW_COLUMN, top, GLYPH_FRAME_TOP_LEFT, COLOR_WHITE, COLOR_BLACK);
	drawCell(WINDOW_COLUMN, bottom, GLYPH_FRAME_BOTTOM_LEFT, COLOR_WHITE, COLOR_BLACK);
	drawCell(right, top, GLYPH_FRAME_TOP_RIGHT, COLOR_WHITE, COLOR_BLACK);
	drawCell(right, bottom, GLYPH_FRAME_BOTTOM_RIGHT, COLOR_WHITE, COLOR_BLACK);

	for (int x = WINDOW_COLUMN + 1; x < right; ++x) {
		drawCell(x, top, GLYPH_FRAME_TOP, COLOR_WHITE, COLOR_BLACK);
		drawCell(x, bottom, GLYPH_FRAME_BOTTOM, COLOR_WHITE, COLOR_BLACK);

		for (int y = top + 1; y < bottom; ++y)
			drawCell(x, y, ' ', COLOR_WHITE, COLOR_BLACK);
	}

	for (int y = top + 1; y < bottom; ++y) {
		drawCell(WINDOW_COLUMN, y, GLYPH_FRAME_SIDE, COLOR_WHITE, COLOR_BLACK);
		drawCell(right, y, GLYPH_FRAME_SIDE, COLOR_WHITE, COLOR_BLACK);
	}
}

void View::drawTextGrid(const TextGrid &grid, int top) {
	for (int y = 0; y < TextGrid::ROWS; ++y) {
		for (int x = 0; x < TextGrid::COLUMNS; ++x) {
			const TextGrid::Cell &c = grid.cell(x, y);
			const byte fg = c._inverse ? COLOR_BLACK : COLOR_WHITE;
			const byte bg = c._inverse ? COLOR_WHITE : COLOR_BLACK;

			if (c._rune)
				drawRune(WINDOW_COLUMN + x, top + y, c._ch, fg, bg);
			else
				drawCell(WINDOW_COLUMN + x, top + y, c._ch, fg, bg);
		}
	}
}

void View::drawLabel(const Common::String &text) {
	const int column = LABEL_CENTER - (int)text.size() / 2;

	drawCaps(column * 8, 0, true);
	print(column + 1, 0, text, COLOR_WHITE, COLOR_BLACK);
	drawCaps((column + 1 + (int)text.size()) * 8, 0, false);
}

void View::drawScrollArrows(int row, int arrows) {
	if (arrows == 0)
		return;

	drawCaps(LABEL_CENTER * 8, row * 8, true);
	drawCell(LABEL_CENTER + 1, row, arrows == 1 ? GLYPH_ARROW_DOWN : arrows == 2 ? GLYPH_ARROW_UP : GLYPH_ARROWS,
		COLOR_WHITE, COLOR_BLACK);
	drawCaps((LABEL_CENTER + 2) * 8, row * 8, false);
}

} // namespace Views
} // namespace Ultima5
} // namespace Ultima
