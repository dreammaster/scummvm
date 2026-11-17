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

#ifndef ULTIMA5_GFX_INTRO_SCREEN_H
#define ULTIMA5_GFX_INTRO_SCREEN_H

#include "common/ptr.h"
#include "graphics/managed_surface.h"
#include "ultima/ultima5/gfx/charset.h"
#include "ultima/ultima5/gfx/image_file.h"

namespace Ultima {
namespace Ultima5 {
namespace Gfx {

constexpr int MENU_ITEM_COUNT = 6;

/**
 * Draws the parts of the title screen: the logo, the shimmering subtitle,
 * the blue bordered menu at the bottom and its text
 */
class IntroScreen {
private:
	Common::ScopedPtr<CharSet> _font;
	ImageFile _logo;

	void drawCaps(Graphics::ManagedSurface &dst, int x, int y, bool left) const;

public:
	static constexpr int WD_FRAME_COUNT = 4;

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
	 * Loads the graphics, once the render mode is known
	 */
	bool load();

	/**
	 * Draws the black screen with the logo, a frame of the subtitle,
	 * the menu border and the copyright line
	 */
	void drawTitle(Graphics::ManagedSurface &dst, int wdFrame) const;

	/**
	 * Draws the subtitle on its own
	 */
	void drawSubtitle(Graphics::ManagedSurface &dst, int wdFrame) const;

	/**
	 * Draws the menu border and its white inner outline
	 */
	void drawMenuFrame(Graphics::ManagedSurface &dst) const;

	/**
	 * Draws the menu choices, with one highlighted
	 */
	void drawMenu(Graphics::ManagedSurface &dst, int selected) const;

	/**
	 * Draws the "Select:" prompt on the top border, with its animated cursor
	 */
	void drawPrompt(Graphics::ManagedSurface &dst, int cursorFrame) const;

	/**
	 * Draws the title screen with the inside of the menu frame emptied, as the
	 * background for entering a name
	 */
	void drawEmptyMenu(Graphics::ManagedSurface &dst, int wdFrame) const;

	/**
	 * Draws up to three lines of text in the menu frame, in place of the menu
	 */
	void drawMessage(Graphics::ManagedSurface &dst, const char *line1, const char *line2 = nullptr,
		const char *line3 = nullptr) const;
};

} // namespace Gfx
} // namespace Ultima5
} // namespace Ultima

#endif
