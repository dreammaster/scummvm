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
#include "wasteland/fod/views/main_menu.h"
#include "wasteland/engine.h"

namespace Wasteland {
namespace FOD {
namespace Views {

bool MainMenu::msgKeypress(const KeypressMessage &msg) {
	// Any keypress to close the view
	close();
	return true;
}

void MainMenu::draw() {
	Surface s = getSurface();
	s.clear();
	s.drawBorders();

	Surface portrait = getSurface(Gfx::Window(1, 0, 12, 11));
	portrait.writeCenteredString("Welcome", 11);

	Surface main = getSurface(Gfx::Window(0, 14, 39, 19));
	main.writeCenteredString("Choose a function:", 1);
	main.writeString("A)dd member      R)emove member", 5, 3);
	main.writeString("E)dit member     P)lay the game", 5, 4);
}

} // namespace Views
} // namespace FOD
} // namespace Wasteland
