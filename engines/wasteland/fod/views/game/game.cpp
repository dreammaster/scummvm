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

#include "wasteland/fod/views/game/game.h"
#include "wasteland/fod/fod.h"

namespace Wasteland {
namespace FOD {
namespace Views {

static const char *WEEKDAYS[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

Game::Game() : BaseView("Game") {
}

bool Game::msgFocus(const FocusMessage &msg) {
	setMode(MODE_NORMAL);

	if (msg._priorView->getName() == "Title")
		_infoText = "Welcome to the beautiful island of Florida!\n";

	return true;
}

void Game::draw() {
	drawBorders(0);

	writeTime();
	writeInfo();
}

bool Game::msgKeypress(const KeypressMessage &msg) {
	assert(_mode == MODE_NORMAL);

	switch (msg.keycode) {
	case Common::KEYCODE_F1:
	case Common::KEYCODE_F2:
	case Common::KEYCODE_F3:
	case Common::KEYCODE_F4:
	case Common::KEYCODE_F5: {
		int partyNum = msg.keycode - Common::KEYCODE_F1;
		if (partyNum < g_engine->_disk1._partyCount)
			send("CharacterInfo", GameMessage("INFO", partyNum));
		break;
	}

	case Common::KEYCODE_q:
		addView("QuitDialog");
		break;

	default:
		break;
	}

	return true;
}

void Game::setMode(Mode newMode) {
	_mode = newMode;
	_infoText.clear();
}

void Game::writeTime() {
	Surface s = getSurface(Gfx::Window(1, 22, 5, 23));
	s.writeString(WEEKDAYS[g_engine->_disk1._timeWeekday], 1, 0);
	s.writeString(Common::String::format("%2.2d:%.2d", g_engine->_disk1._timeHours, g_engine->_disk1._timeMinutes),
		0, 1);
}

void Game::writeInfo() {
	Surface s = getSurface(Gfx::Window(7, 22, 39, 24));
	s.writeString(_infoText);
}

} // namespace Views
} // namespace FOD
} // namespace Wasteland
