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

#ifndef ULTIMA5_VIEWS_TITLE_BASE_H
#define ULTIMA5_VIEWS_TITLE_BASE_H

#include "ultima/ultima5/gfx/image_file.h"
#include "ultima/ultima5/views/view.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {

constexpr int MENU_ITEM_COUNT = 6;

/**
 * Base for the views that are shown over the title screen: the logo, the
 * shimmering subtitle and the blue bordered menu at the bottom
 */
class TitleBase : public View {
private:
	Gfx::ImageFile _logo;

protected:
	static constexpr int SUBTITLE_FRAME_COUNT = 4;

	TitleBase(const Common::String &name) : View(name) {}

	/**
	 * Loads the graphics, once the render mode is known
	 */
	bool loadTitleGraphics();

	/**
	 * Draws the black screen with the logo, a frame of the subtitle,
	 * the menu border and the copyright line
	 */
	void drawTitle(int subtitleFrame);

	/**
	 * Draws the logo alone on a black screen
	 */
	void drawLogo();

	/**
	 * Draws the subtitle on its own
	 */
	void drawSubtitle(int subtitleFrame);

	/**
	 * Draws the menu border and its white inner outline
	 */
	void drawMenuFrame();

	/**
	 * Draws the menu choices, with one highlighted
	 */
	void drawMenu(int selected);

	/**
	 * Draws the "Select:" prompt on the top border, with its animated cursor
	 */
	void drawPrompt(int cursorFrame);

	/**
	 * Draws the title screen with the inside of the menu frame emptied, as the
	 * background for entering a name
	 */
	void drawEmptyMenu(int subtitleFrame);

	/**
	 * Draws up to three lines of text in the menu frame, in place of the menu
	 */
	void drawMessage(const char *line1, const char *line2 = nullptr, const char *line3 = nullptr);

public:
	~TitleBase() override {}
};

} // namespace Views
} // namespace Ultima5
} // namespace Ultima

#endif
