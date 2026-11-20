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

#ifndef ULTIMA5_VIEWS_VIEW_H
#define ULTIMA5_VIEWS_VIEW_H

#include "common/str.h"
#include "ultima/ultima5/logic/text_grid.h"
#include "ultima/shared/engine/view.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {

using namespace Graphics::Views;

constexpr byte COLOR_BLACK = 0;
constexpr byte COLOR_BLUE = 1;
constexpr byte COLOR_WHITE = 15;

/**
 * Base class for the views of the game. The screen is laid out in cells of 8x8
 * pixels, holding text or the pieces of the blue borders, and this
 * adds drawing the things that go in them
 */
class View : public Shared::View {
public:
	/**
	 * Draws a character in one of the cells of the screen, on a background
	 */
	void drawCell(int col, int row, byte ch, byte fg, byte bg);

	/**
	 * Draws text from a cell onwards
	 */
	void print(int col, int row, const Common::String &text, byte fg, byte bg);

	/**
	 * Draws the animated text cursor in a cell
	 */
	void drawCursor(int col, int row, int cursorFrame);

	/**
	 * Draws the cap that starts or ends a label set in a border, in the cell
	 * at a position given in pixels
	 */
	void drawCaps(int x, int y, bool left);

	/**
	 * Draws a character of the rune font, which has the small pictures too, in a cell
	 */
	void drawRune(int col, int row, byte ch, byte fg, byte bg);

	/**
	 * Draws the box that the windows over the party have, from one row to another
	 */
	void drawWindowFrame(int top, int bottom);

	/**
	 * Draws the window of text that goes over the party, from a row of the screen
	 */
	void drawTextGrid(const TextGrid &grid, int top);

	/**
	 * Sets a label centered in the top border
	 */
	void drawLabel(const Common::String &text);

	/**
	 * Shows in a border row which ways a list goes on: 0 for neither, 1 for down,
	 * 2 for up and 3 for both
	 */
	void drawScrollArrows(int row, int arrows);

	View(const Common::String &name) : Shared::View(name) {}
	~View() override {}
};

} // namespace Views
} // namespace Ultima5
} // namespace Ultima

#endif
