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

#ifndef ULTIMA5_VIEWS_ACKNOWLEDGEMENTS_H
#define ULTIMA5_VIEWS_ACKNOWLEDGEMENTS_H

#include "graphics/managed_surface.h"
#include "ultima/shared/engine/view.h"
#include "ultima/ultima5/gfx/image_file.h"
#include "ultima/ultima5/gfx/intro_screen.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {

using namespace Graphics::Views;

/**
 * The credits. A rolled up scroll rise over the title screen and unfurls
 * to show them, then close again on a key press to show the menu once more
 */
class Acknowledgements : public Shared::View {
private:
	enum Phase { PHASE_RISE, PHASE_OPEN, PHASE_WAIT, PHASE_CLOSE, PHASE_DROP };

	Gfx::IntroScreen _intro;
	Gfx::ImageFile _scroll;

	// What's on the screen, what the scroll reveals when it unfurls,
	// and what it reveals when it closes again
	Graphics::ManagedSurface _screen;
	Graphics::ManagedSurface _credits;
	Graphics::ManagedSurface _menu;

	Phase _phase = PHASE_RISE;
	int _pos = 0;

	/**
	 * Draws both scroll edges, with the left one at a position
	 */
	void drawScrollEdges(int leftX, int rightX, int y);

	/**
	 * Restores an area of the screen from a surface
	 */
	void restore(const Graphics::ManagedSurface &src, int x1, int y1, int x2, int y2);

	void step();

public:
	Acknowledgements() : Shared::View("Acknowledgements") {}
	~Acknowledgements() override {}

	bool msgFocus(const FocusMessage &msg) override;
	void draw() override;
	void timeout() override;
	bool msgKeypress(const KeypressMessage &msg) override;
};

} // namespace Views
} // namespace Ultima5
} // namespace Ultima

#endif
