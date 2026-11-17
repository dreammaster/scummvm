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

#include "ultima/ultima5/gfx/text_screen.h"

namespace Ultima {
namespace Ultima5 {
namespace Gfx {

constexpr byte GLYPH_CAP_RIGHT = 1;
constexpr byte GLYPH_CAP_LEFT = 2;
constexpr byte GLYPH_CURSOR = 5;

void TextScreen::loadFont() {
	if (!_font)
		_font.reset(new CharSet());
}

void TextScreen::drawCell(Graphics::ManagedSurface &dst, int col, int row, byte ch, byte fg, byte bg) const {
	dst.fillRect(Common::Rect(col * 8, row * 8, col * 8 + 8, row * 8 + 8), bg);
	_font->drawChar(dst.surfacePtr(), ch, col * 8, row * 8, fg);
}

void TextScreen::print(Graphics::ManagedSurface &dst, int col, int row, const char *text, byte fg, byte bg) const {
	for (; *text; ++text, ++col)
		drawCell(dst, col, row, *text, fg, bg);
}

void TextScreen::drawCursor(Graphics::ManagedSurface &dst, int col, int row, int cursorFrame) const {
	drawCell(dst, col, row, GLYPH_CURSOR + (cursorFrame & 3), TEXT_WHITE, TEXT_BLACK);
}

void TextScreen::drawCaps(Graphics::ManagedSurface &dst, int x, int y, bool left) const {
	dst.fillRect(Common::Rect(x, y, x + 8, y + 8), TEXT_BLACK);
	_font->drawChar(dst.surfacePtr(), left ? GLYPH_CAP_LEFT : GLYPH_CAP_RIGHT, x, y, TEXT_BLUE);

	if (left) {
		dst.drawLine(x, y, x + 5, y + 3, TEXT_WHITE);
		dst.drawLine(x + 5, y + 4, x, y + 7, TEXT_WHITE);
	} else {
		dst.drawLine(x + 7, y, x + 2, y + 3, TEXT_WHITE);
		dst.drawLine(x + 2, y + 4, x + 7, y + 7, TEXT_WHITE);
	}
}

} // namespace Gfx
} // namespace Ultima5
} // namespace Ultima
