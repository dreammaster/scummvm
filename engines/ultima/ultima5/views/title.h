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

#ifndef ULTIMA5_VIEWS_TITLE_H
#define ULTIMA5_VIEWS_TITLE_H

#include "ultima/shared/engine/view.h"
#include "ultima/ultima5/gfx/intro_screen.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {

using namespace Graphics::Views;

/**
 * The title screen, with the logo, the shimmering subtitle and the main menu
 */
class Title : public Shared::View {
private:
	Gfx::IntroScreen _screen;
	int _selected = 0;
	int _animFrame = 0;
	bool _showNotice = false;

	/**
	 * Moves the highlight to a choice, and carries it out if it's one that's
	 * chosen directly by its letter
	 */
	void choose(int index, bool activate);

	void activate(int index);

public:
	Title() : Shared::View("Title") {}
	~Title() override {}

	bool msgFocus(const FocusMessage &msg) override;
	void draw() override;
	void timeout() override;
	bool msgKeypress(const KeypressMessage &msg) override;
};

} // namespace Views
} // namespace Ultima5
} // namespace Ultima

#endif
