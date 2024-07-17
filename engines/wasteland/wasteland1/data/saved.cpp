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

#include "wasteland/wasteland1/data/saved.h"
#include "wasteland/core/file.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Data {

Saved::Saved() {
	_parties.resize(4);
	_roster.resize(7);
}

void Saved::synchronize(Serializer &s) {
	for (uint i = 0; i < _parties.size(); ++i)
		_parties[i].synchronize(s);

	s.skip(64);		// TODO: Figure out unknown bytes
	_saved2.synchronize(s);
	s.skip(74);		// TODO: Figure out unknown bytes

	char locName[13];
	Common::strcpy_s(locName, _saveLocationName.c_str());
	s.syncBytes((byte *)locName, 12);
	locName[12] = '\0';
	if (s.isLoading())
		_saveLocationName = Common::String(locName);

	s.skip(0x24);	// TODO: Figure out unknown bytes

	for (uint i = 1; i <= _roster.size(); ++i)
		_roster[i].synchronize(s);
}

void Saved::load() {
	File f("savegame");	// Loads savegame block from game1
	Serializer s(&f);
	synchronize(s);
}


void Saved2::synchronize(Serializer &s) {
	s.syncAsByte(_field0);
	s.syncAsByte(_field1);
	s.syncAsByte(_field2);
	s.syncAsByte(_field3);
	s.syncAsByte(_field4);
	s.syncAsByte(_membersInGroup);
	s.syncAsByte(_activePartyGroup);
	s.syncAsByte(_field7);
	s.syncAsByte(_field8);
	s.syncAsByte(_totalPartyGroups);
	s.syncAsByte(_fieldA);
	s.syncAsByte(_fieldB);
	s.syncAsByte(_fieldC);
	s.syncAsByte(_fieldD);
}

void Saved::setCurrentCharacter(int partyNum) {
	auto &party = _parties[_saved2._activePartyGroup];
	_currentCharacter = &_roster[party._members[partyNum]];
}

} // namespace Data
} // namespace Wasteland1
} // namespace Wasteland
