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

#include "wasteland/wasteland1/views/character/delete.h"
#include "wasteland/wasteland1/wasteland1.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Views {
namespace Character {

Delete::Delete() : Pane("CharacterDelete") {
}

void Delete::draw() {
	Pane::draw();

	Surface s = getSurface();
	s.writeString("Which player do you want to delete?");

	if (_selectedCharacter) {
		Common::String msg = Common::String::format(
			"Are you sure you want to delete %s\n\nYes\nNo",
			g_engine->_party[_selectedCharacter]->_name.c_str());
		s.writeString(msg, 0, 3);
	}
}

bool Delete::msgKeypress(const KeypressMessage &msg) {
	if (_selectedCharacter == 0) {
		if (msg.keycode >= Common::KEYCODE_1 && msg.keycode <=
				(Common::KeyCode)(Common::KEYCODE_0 + g_engine->_party.size())) {
			_selectedCharacter = msg.keycode - Common::KEYCODE_0;
			redraw();
			return true;
		}
	} else if (msg.keycode == Common::KEYCODE_y) {
		deleteCharacter();
		return true;
	} else if (msg.keycode == Common::KEYCODE_n) {
		close();
		return true;
	}

	return false;
}

bool Delete::msgGame(const GameMessage &msg) {
	if (msg._name == "DeleteCharacter") {
		if (g_engine->_party.size() > 0) {
			_selectedCharacter = 0;
			addView();
		}

		return true;
	}

	return Pane::msgGame(msg);
}

void Delete::deleteCharacter() {
	g_engine->_currentChar = nullptr;	// Just in case it's the deleted one

	if (g_engine->_party.size() > 1) {
		// Distribute their money across remaining party
		Data::PartyMember *c = g_engine->_party[_selectedCharacter];
		uint money = c->_money / g_engine->_party.size();

		for (uint i = 1; i <= g_engine->_party.size(); ++i) {
			g_engine->_party[i]->_money += money;

		// Remove party member
		// TODO
		}
	}
}

} // namespace Character
} // namespace Views
} // namespace Wasteland1
} // namespace Wasteland
