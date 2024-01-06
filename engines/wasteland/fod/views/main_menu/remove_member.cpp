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

#include "wasteland/fod/views/main_menu/remove_member.h"
#include "wasteland/fod/fod.h"

namespace Wasteland {
namespace FOD {
namespace Views {

RemoveMember::RemoveMember() : UIElement("RemoveMember") {
	setBounds(Gfx::Window(0, 14, 39, 19));
}

bool RemoveMember::msgGame(const GameMessage &msg) {
	if (msg._name == "REMOVE_MEMBER") {
		_selectedPartyMember = msg._value;
		addView();
		return true;
	}

	return false;
}

bool RemoveMember::msgKeypress(const KeypressMessage &msg) {
	if (msg.keycode == Common::KEYCODE_y) {
		for (int i = _selectedPartyMember; i < (g_engine->_disk1._partyCount - 1); ++i)
			g_engine->_disk1._party[i] = g_engine->_disk1._party[i + 1];

		--g_engine->_disk1._partyCount;
	}

	close();
	return true;
}


void RemoveMember::draw() {
	Surface s = getSurface();
	s.clear();

	s.writeCenteredString("Really remove this member?", 1);
	s.writeCenteredString("Y)es N)o", 3);
}

} // namespace Views
} // namespace FOD
} // namespace Wasteland
