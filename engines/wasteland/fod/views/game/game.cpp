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

	int mapNum = g_engine->_disk1._maps[g_engine->_disk1._mapIndex];

	g_engine->_disk.loadMap(mapNum);

	return true;
}

void Game::draw() {
	drawBorders(0);

	writeParty();
	writeTime();
	writeInfo();
}

bool Game::msgKeypress(const KeypressMessage &msg) {
	assert(_mode == MODE_NORMAL);

	switch (msg.keycode) {
	case Common::KEYCODE_a:
		addView("AllCharacterInfo");
		break;

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

bool Game::msgMouseDown(const MouseDownMessage &msg) {
	if (BaseView::msgMouseDown(msg))
		return true;

	for (uint partyNum = 0; partyNum < g_engine->_disk1._partyCount; ++partyNum) {
		const Common::Rect r(4 + partyNum * 28, 0, 52 + partyNum * 56, 24);
		if (r.contains(msg._pos)) {
			send("CharacterInfo", GameMessage("INFO", partyNum));
			return true;
		}
	}

	if (Common::Rect(280, 0, 320, 24).contains(msg._pos)) {
		addView("AllCharacterInfo");
		return true;
	}

	return false;
}

void Game::setMode(Mode newMode) {
	_mode = newMode;
	_infoText.clear();
}

void Game::writeParty() {
	for (uint partyNum = 0; partyNum < g_engine->_disk1._partyCount; ++partyNum) {
		const Data::PartyMember &member = g_engine->_disk1._party[partyNum];

		Gfx::Window win(1, 1, 6, 1);
		win.translate(7 * partyNum, 0);
		Surface s = getSurface(win);

		if (member._afflicted) {
			s.clear(15);
			s.setInverseColor(true);
		}

		Common::String tmp = (member._name.size() <= 6) ? member._name :
			Common::String(member._name.c_str(), 6);
		s.writeCenteredString(tmp, 0);
	}
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
