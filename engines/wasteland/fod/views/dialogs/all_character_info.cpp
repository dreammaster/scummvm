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

#include "wasteland/fod/views/dialogs/all_character_info.h"
#include "wasteland/fod/data/strings.h"
#include "wasteland/fod/fod.h"

namespace Wasteland {
namespace FOD {
namespace Views {
namespace Dialogs {

void AllCharacterInfo::draw() {
	drawBorders(2);

	for (uint partyNum = 0; partyNum < g_engine->_disk1._partyCount; ++partyNum)
		writeMember(partyNum);

	writeOptions();
}

void AllCharacterInfo::writeMember(int partyNum) {
	const Data::PartyMember &member = g_engine->_disk1._party[partyNum];
	Surface s = getSurface(Gfx::Window(1, 1 + partyNum * 4, 38, 3 + partyNum * 4));
	int ac = member.getArmorClass();

	s.writeString(Common::String::format("F%1d>%-19.19s",
		partyNum + 1, member._name.c_str()), 0, 0);
	s.writeString(Common::String::format("(%s)",
		member._afflicted ? "Afflicted" : "Unafflicted"), 25, 0);

	auto status = member.getStatus();
	if (status == Data::STATUS_OK) {
		s.writeString(Common::String::format(" Con:%3u /%3u",
			member._con, member._conBase), 0, 1);
	} else {
		s.writeString(Common::String::format("Con: %3s /%3u",
			Data::STATUSES[status], member._conBase), 0, 1);
	}

	s.writeString(Common::String::format("AC: %2d", ac), 16, 1);

	s.writeString(Common::String::format("Weapon: %-19.19s   Ammo: %2d",
		member._equippedWeapon == 0xff ? "Hands" :
			g_engine->_globals._items[member._array1[member._equippedWeapon]._id]._name.c_str(),
		member._equippedWeapon == 0xff ? 0 : member._array1[member._equippedWeapon]._ammo),
		0, 2);	
}

void AllCharacterInfo::writeOptions() {
	Surface s = getSurface(Gfx::Window(1, 21, 38, 23));
	s.writeString(Common::String::format("Cash: $%lu", g_engine->_disk1._cash), 22, 0);
	s.writeString("(R)eorder roster", 19, 1);
	s.writeString("(B)anish   (L)eave", 19, 2);
}

bool AllCharacterInfo::msgKeypress(const KeypressMessage &msg) {
	close();
	return true;
}

} // namespace Dialogs
} // namespace Views
} // namespace FOD
} // namespace Wasteland
