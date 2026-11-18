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

#include "ultima/ultima5/views/title_base.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {

constexpr int SUBTITLE_X = 0x10;
constexpr int SUBTITLE_Y = 0x41;
constexpr int SUBTITLE_HEIGHT = 0x31;

constexpr int MENU_TOP_ROW = 15;
constexpr int MENU_BOTTOM_ROW = 24;
constexpr int MENU_LAST_COL = 39;
constexpr int MENU_FIRST_ITEM_ROW = 17;

constexpr byte GLYPH_CORNER_TOP_LEFT = 0x7B;
constexpr byte GLYPH_CORNER_TOP_RIGHT = 0x7C;
constexpr byte GLYPH_CORNER_BOTTOM_LEFT = 0x7D;
constexpr byte GLYPH_CORNER_BOTTOM_RIGHT = 0x7E;
constexpr byte GLYPH_EDGE = 0x7F;

static const char *const MENU_ITEMS[MENU_ITEM_COUNT] = {
	"Journey Onward", "Create New Character", "Transfer from Ultima IV",
	"Ultima V Introduction", "Acknowledgements", "Return to the View"
};
static const byte MENU_ITEM_COLUMNS[MENU_ITEM_COUNT] = { 12, 9, 8, 9, 11, 10 };

static const char *const COPYRIGHT = "Copyright 1988 Lord British";

bool TitleBase::loadTitleGraphics() {
	return _logo.load("ultima", _G(renderMode)) && _logo.size() > SUBTITLE_FRAME_COUNT;
}

void TitleBase::drawSubtitle(int subtitleFrame) {
	auto s = getSurface();

	// The pictures can be a row taller than what's shown
	Graphics::ManagedSurface clip(s, Common::Rect(0, 0, s.w, SUBTITLE_Y + SUBTITLE_HEIGHT));
	_logo.draw(clip, 1 + (subtitleFrame % SUBTITLE_FRAME_COUNT), SUBTITLE_X, SUBTITLE_Y);
}

void TitleBase::drawMenuFrame() {
	for (int col = 1; col < MENU_LAST_COL; ++col) {
		drawCell(col, MENU_TOP_ROW, GLYPH_EDGE, COLOR_BLUE, COLOR_BLACK);
		drawCell(col, MENU_BOTTOM_ROW, GLYPH_EDGE, COLOR_BLUE, COLOR_BLACK);
	}

	for (int row = MENU_TOP_ROW + 1; row < MENU_BOTTOM_ROW; ++row) {
		drawCell(0, row, GLYPH_EDGE, COLOR_BLUE, COLOR_BLACK);
		drawCell(MENU_LAST_COL, row, GLYPH_EDGE, COLOR_BLUE, COLOR_BLACK);
	}

	drawCell(0, MENU_TOP_ROW, GLYPH_CORNER_TOP_LEFT, COLOR_BLUE, COLOR_BLACK);
	drawCell(MENU_LAST_COL, MENU_TOP_ROW, GLYPH_CORNER_TOP_RIGHT, COLOR_BLUE, COLOR_BLACK);
	drawCell(0, MENU_BOTTOM_ROW, GLYPH_CORNER_BOTTOM_LEFT, COLOR_BLUE, COLOR_BLACK);
	drawCell(MENU_LAST_COL, MENU_BOTTOM_ROW, GLYPH_CORNER_BOTTOM_RIGHT, COLOR_BLUE, COLOR_BLACK);

	auto s = getSurface();
	s.drawLine(7, 127, 312, 127, COLOR_WHITE);
	s.drawLine(312, 127, 312, 192, COLOR_WHITE);
	s.drawLine(312, 192, 7, 192, COLOR_WHITE);
	s.drawLine(7, 192, 7, 127, COLOR_WHITE);
}

void TitleBase::drawMenu(int selected) {
	for (int i = 0; i < MENU_ITEM_COUNT; ++i) {
		const bool highlight = i == selected;

		print(MENU_ITEM_COLUMNS[i], MENU_FIRST_ITEM_ROW + i, Common::String(" ") + MENU_ITEMS[i] + " ",
			highlight ? COLOR_BLACK : COLOR_WHITE, highlight ? COLOR_WHITE : COLOR_BLACK);
	}
}

void TitleBase::drawPrompt(int cursorFrame) {
	drawCaps(15 * 8, MENU_TOP_ROW * 8, true);
	print(16, MENU_TOP_ROW, "Select: ", COLOR_WHITE, COLOR_BLACK);
	drawCaps(24 * 8, MENU_TOP_ROW * 8, false);
	drawCursor(23, MENU_TOP_ROW, cursorFrame);
}

void TitleBase::drawEmptyMenu(int subtitleFrame) {
	drawTitle(subtitleFrame);

	auto s = getSurface();
	s.fillRect(Common::Rect(8, 128, 312, 192), COLOR_BLACK);
	s.fillRect(Common::Rect(0x78, 0x78, 0xC9, 0x7F), COLOR_BLUE);
	s.drawLine(0x78, 0x7F, 0xC8, 0x7F, COLOR_WHITE);
}

void TitleBase::drawMessage(const char *line1, const char *line2, const char *line3) {
	auto s = getSurface();
	s.fillRect(Common::Rect(8, 128, 312, 192), COLOR_BLACK);

	const char *const lines[] = { line1, line2, line3 };
	for (int i = 0; i < 3; ++i) {
		if (lines[i])
			print(2, 18 + i, lines[i], COLOR_WHITE, COLOR_BLACK);
	}
}

void TitleBase::drawTitle(int subtitleFrame) {
	auto s = getSurface();
	s.clear();
	_logo.draw(s, 0, 0, 0);
	drawSubtitle(subtitleFrame);
	drawMenuFrame();

	// The copyright line sits in a gap in the bottom border, between two caps
	const int len = strlen(COPYRIGHT);
	const int start = 18 - len / 2;
	const int end = start + len + 2;
	s.fillRect(Common::Rect(8, 193, start * 8 + 1, 200), COLOR_BLUE);
	s.fillRect(Common::Rect(end * 8, 193, 312, 200), COLOR_BLUE);
	s.drawLine(8, 192, start * 8, 192, COLOR_WHITE);
	s.drawLine(end * 8, 192, 311, 192, COLOR_WHITE);

	drawCaps(start * 8, MENU_BOTTOM_ROW * 8, true);
	print(start + 1, MENU_BOTTOM_ROW, COPYRIGHT, COLOR_WHITE, COLOR_BLACK);
	drawCaps((start + 1 + len) * 8, MENU_BOTTOM_ROW * 8, false);
}

} // namespace Views
} // namespace Ultima5
} // namespace Ultima
