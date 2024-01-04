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
#include "wasteland/fod/fod.h"

namespace Wasteland {
namespace FOD {
namespace Views {

MainMenu::MainMenu() : MenuView("MainMenu"),
	_addMember("AddMember", this, 5, 17, "A)dd member", Common::KEYCODE_a),
	_editMember("EditMember", this, 5, 18, "E)dit member", Common::KEYCODE_e),
	_removeMember("RemoveMember", this, 22, 17, "R)emove member", Common::KEYCODE_r),
	_playGame("PlayGame", this, 22, 18, "P)lay the game", Common::KEYCODE_p) {
}

bool MainMenu::msgKeypress(const KeypressMessage &msg) {
	switch (msg.keycode) {
	case Common::KEYCODE_a:
		addMember();
		break;
	case Common::KEYCODE_e:
		editMember();
		break;
	case Common::KEYCODE_r:
		removeMember();
		break;
	case Common::KEYCODE_p:
		playGame();
		break;
	default:
		break;
	}


	return true;
}

void MainMenu::draw() {
	MenuView::draw();

	Surface portrait = getSurface(Gfx::Window(1, 0, 12, 11));
	portrait.writeCenteredString("Welcome", 11);

	Surface main = getSurface(Gfx::Window(0, 14, 39, 19));
	main.writeCenteredString("Choose a function:", 1);

	UIElement::draw();
}

void MainMenu::addMember() {

}

void MainMenu::editMember() {
}

void MainMenu::removeMember() {
}

void MainMenu::playGame() {
	if (g_engine->_disk1._partyCount == 0) {
		Surface main = getSurface(Gfx::Window(0, 14, 39, 19));
		main.clear();
		main.writeCenteredString("It's tough out there!", 1);
		main.writeCenteredString("You should take somebody with you.", 3);

		delaySeconds(3);

	} else {
		// Start the game
		replaceView("Game");
	}
}

} // namespace Views
} // namespace FOD
} // namespace Wasteland
