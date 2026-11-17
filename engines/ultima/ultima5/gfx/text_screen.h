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

#ifndef ULTIMA5_GFX_TEXT_SCREEN_H
#define ULTIMA5_GFX_TEXT_SCREEN_H

#include "common/ptr.h"
#include "graphics/managed_surface.h"
#include "ultima/ultima5/gfx/charset.h"

namespace Ultima {
namespace Ultima5 {
namespace Gfx {

constexpr byte TEXT_BLACK = 0;
constexpr byte TEXT_BLUE = 1;
constexpr byte TEXT_WHITE = 15;

/**
 * Base for the classes that draw whole screens of the game, which are laid out
 * in 8x8 cells of text mixed with graphics
 */
class TextScreen {
private:
	Common::ScopedPtr<CharSet> _font;

protected:
	/**
	 * Loads the font, if it hasn't been already
	 */
	void loadFont();

public:
	virtual ~TextScreen() {}

	/**
	 * Draws a character in one of the 8x8 cells of the screen, on a background
	 */
	void drawCell(Graphics::ManagedSurface &dst, int col, int row, byte ch, byte fg, byte bg) const;

	/**
	 * Draws text from a cell onwards
	 */
	void print(Graphics::ManagedSurface &dst, int col, int row, const char *text, byte fg, byte bg) const;

	/**
	 * Draws the animated text cursor in a cell
	 */
	void drawCursor(Graphics::ManagedSurface &dst, int col, int row, int cursorFrame) const;

	/**
	 * Draws the cap that starts or ends a label set in a border, in a cell
	 */
	void drawCaps(Graphics::ManagedSurface &dst, int x, int y, bool left) const;
};

} // namespace Gfx
} // namespace Ultima5
} // namespace Ultima

#endif
