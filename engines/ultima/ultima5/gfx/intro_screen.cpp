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

#include "ultima/ultima5/gfx/intro_screen.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {
namespace Gfx {

constexpr byte BLACK = 0;
constexpr byte BLUE = 1;
constexpr byte WHITE = 15;

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
constexpr byte GLYPH_CAP_RIGHT = 1;
constexpr byte GLYPH_CAP_LEFT = 2;
constexpr byte GLYPH_CURSOR = 5;

static const char *const MENU_ITEMS[MENU_ITEM_COUNT] = {
	"Journey Onward", "Create New Character", "Transfer from Ultima IV",
	"Ultima V Introduction", "Acknowledgements", "Return to the View"
};
static const byte MENU_ITEM_COLUMNS[MENU_ITEM_COUNT] = { 12, 9, 8, 9, 11, 10 };

static const char *const COPYRIGHT = "Copyright 1988 Lord British";

bool IntroScreen::load() {
	if (!_font)
		_font.reset(new CharSet());

	return _logo.load("ultima", _G(renderMode)) && _logo.size() > WD_FRAME_COUNT;
}

void IntroScreen::drawCell(Graphics::ManagedSurface &dst, int col, int row, byte ch, byte fg, byte bg) const {
	dst.fillRect(Common::Rect(col * 8, row * 8, col * 8 + 8, row * 8 + 8), bg);
	_font->drawChar(dst.surfacePtr(), ch, col * 8, row * 8, fg);
}

void IntroScreen::print(Graphics::ManagedSurface &dst, int col, int row, const char *text, byte fg, byte bg) const {
	for (; *text; ++text, ++col)
		drawCell(dst, col, row, *text, fg, bg);
}

void IntroScreen::drawCaps(Graphics::ManagedSurface &dst, int x, int y, bool left) const {
	dst.fillRect(Common::Rect(x, y, x + 8, y + 8), BLACK);
	_font->drawChar(dst.surfacePtr(), left ? GLYPH_CAP_LEFT : GLYPH_CAP_RIGHT, x, y, BLUE);

	if (left) {
		dst.drawLine(x, y, x + 5, y + 3, WHITE);
		dst.drawLine(x + 5, y + 4, x, y + 7, WHITE);
	} else {
		dst.drawLine(x + 7, y, x + 2, y + 3, WHITE);
		dst.drawLine(x + 2, y + 4, x + 7, y + 7, WHITE);
	}
}

void IntroScreen::drawSubtitle(Graphics::ManagedSurface &dst, int wdFrame) const {
	// The pictures can be a row taller than what's shown
	Graphics::ManagedSurface clip(dst, Common::Rect(0, 0, dst.w, SUBTITLE_Y + SUBTITLE_HEIGHT));
	_logo.draw(clip, 1 + (wdFrame % WD_FRAME_COUNT), SUBTITLE_X, SUBTITLE_Y);
}

void IntroScreen::drawMenuFrame(Graphics::ManagedSurface &dst) const {
	for (int col = 1; col < MENU_LAST_COL; ++col) {
		drawCell(dst, col, MENU_TOP_ROW, GLYPH_EDGE, BLUE, BLACK);
		drawCell(dst, col, MENU_BOTTOM_ROW, GLYPH_EDGE, BLUE, BLACK);
	}

	for (int row = MENU_TOP_ROW + 1; row < MENU_BOTTOM_ROW; ++row) {
		drawCell(dst, 0, row, GLYPH_EDGE, BLUE, BLACK);
		drawCell(dst, MENU_LAST_COL, row, GLYPH_EDGE, BLUE, BLACK);
	}

	drawCell(dst, 0, MENU_TOP_ROW, GLYPH_CORNER_TOP_LEFT, BLUE, BLACK);
	drawCell(dst, MENU_LAST_COL, MENU_TOP_ROW, GLYPH_CORNER_TOP_RIGHT, BLUE, BLACK);
	drawCell(dst, 0, MENU_BOTTOM_ROW, GLYPH_CORNER_BOTTOM_LEFT, BLUE, BLACK);
	drawCell(dst, MENU_LAST_COL, MENU_BOTTOM_ROW, GLYPH_CORNER_BOTTOM_RIGHT, BLUE, BLACK);

	dst.drawLine(7, 127, 312, 127, WHITE);
	dst.drawLine(312, 127, 312, 192, WHITE);
	dst.drawLine(312, 192, 7, 192, WHITE);
	dst.drawLine(7, 192, 7, 127, WHITE);
}

void IntroScreen::drawMenu(Graphics::ManagedSurface &dst, int selected) const {
	for (int i = 0; i < MENU_ITEM_COUNT; ++i) {
		const Common::String text = Common::String(" ") + MENU_ITEMS[i] + " ";
		const bool highlight = i == selected;

		print(dst, MENU_ITEM_COLUMNS[i], MENU_FIRST_ITEM_ROW + i, text.c_str(),
			highlight ? BLACK : WHITE, highlight ? WHITE : BLACK);
	}
}

void IntroScreen::drawPrompt(Graphics::ManagedSurface &dst, int cursorFrame) const {
	drawCaps(dst, 15 * 8, MENU_TOP_ROW * 8, true);
	print(dst, 16, MENU_TOP_ROW, "Select: ", WHITE, BLACK);
	drawCaps(dst, 24 * 8, MENU_TOP_ROW * 8, false);
	drawCursor(dst, 23, MENU_TOP_ROW, cursorFrame);
}

void IntroScreen::drawCursor(Graphics::ManagedSurface &dst, int col, int row, int cursorFrame) const {
	drawCell(dst, col, row, GLYPH_CURSOR + (cursorFrame & 3), WHITE, BLACK);
}

void IntroScreen::drawEmptyMenu(Graphics::ManagedSurface &dst, int wdFrame) const {
	drawTitle(dst, wdFrame);
	dst.fillRect(Common::Rect(8, 128, 312, 192), BLACK);
	dst.fillRect(Common::Rect(0x78, 0x78, 0xC9, 0x7F), BLUE);
	dst.drawLine(0x78, 0x7F, 0xC8, 0x7F, WHITE);
}

void IntroScreen::drawMessage(Graphics::ManagedSurface &dst, const char *line1, const char *line2,
		const char *line3) const {
	dst.fillRect(Common::Rect(8, 128, 312, 192), BLACK);

	const char *const lines[] = { line1, line2, line3 };
	for (int i = 0; i < 3; ++i) {
		if (lines[i])
			print(dst, 2, 18 + i, lines[i], WHITE, BLACK);
	}
}

void IntroScreen::drawTitle(Graphics::ManagedSurface &dst, int wdFrame) const {
	dst.clear();
	_logo.draw(dst, 0, 0, 0);
	drawSubtitle(dst, wdFrame);
	drawMenuFrame(dst);

	// The copyright line sits in a gap in the bottom border, between two caps
	const int len = strlen(COPYRIGHT);
	const int start = 18 - len / 2;
	const int end = start + len + 2;
	dst.fillRect(Common::Rect(8, 193, start * 8 + 1, 200), BLUE);
	dst.fillRect(Common::Rect(end * 8, 193, 312, 200), BLUE);
	dst.drawLine(8, 192, start * 8, 192, WHITE);
	dst.drawLine(end * 8, 192, 311, 192, WHITE);

	drawCaps(dst, start * 8, MENU_BOTTOM_ROW * 8, true);
	print(dst, start + 1, MENU_BOTTOM_ROW, COPYRIGHT, WHITE, BLACK);
	drawCaps(dst, (start + 1 + len) * 8, MENU_BOTTOM_ROW * 8, false);
}

} // namespace Gfx
} // namespace Ultima5
} // namespace Ultima
