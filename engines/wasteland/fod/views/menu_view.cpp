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

#include "common/system.h"
#include "graphics/palette.h"
#include "wasteland/fod/views/menu_view.h"
#include "wasteland/fod/fod.h"

namespace Wasteland {
namespace FOD {
namespace Views {

void MenuView::draw() {
	Surface s = getSurface();
	s.clear();
	drawBorders();

	// Draw the welcome portrait
	Surface portrait = getSurface(Gfx::Window(1, 1, 12, 11));
	portrait.blitFrom(*g_engine->_pics._welcome.getSurface());
}

void MenuView::drawBorders() {
	Surface s = getSurface();
	const byte *borders = g_engine->_pics._borders;

	for (int y = 0; y < TEXT_H; ++y) {
		for (int x = 0; x < TEXT_W; ++x) {
			s.setTextPos(x, y);
			s.writeChar(*borders++);
		}
	}
}

} // namespace Views
} // namespace FOD
} // namespace Wasteland
