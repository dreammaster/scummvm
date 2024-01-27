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

#include "common/file.h"
#include "wasteland/fod/data/scripts.h"
#include "wasteland/fod/data/disk1.h"
#include "wasteland/fod/fod.h"

namespace Wasteland {
namespace FOD {
namespace Data {

void Disk1Table::load(Common::Serializer &s) {
	s.syncAsUint16LE(_count);

	for (int i = 0; i < 150; ++i)
		_entries[i].load(s);
}

int Disk1Table::indexOf(int mapNum, int mapX, int mapY) {
	for (int i = 0; i < _count; ++i) {
		const auto &item = _entries[i];
		if (item._mapNum == mapNum && item._x == mapX && item._y == mapY)
			return i;
	}

	return -1;
}

bool Disk1Table::contains(int mapNum, int mapX, int mapY) {
	return indexOf(mapNum, mapX, mapY) != -1;
}

Disk1Table::Entry *Disk1Table::add(int mapNum, int mapX, int mapY) {
	// Make sure the table hasn't run out
	if (_count >= DISK1_TABLE_SIZE)
		error("Flushing Needed!!");

	// Ensure there aren't duplicates
	int idx = indexOf(mapNum, mapX, mapY);
	if (idx != -1)
		return &_entries[idx];

	// Add the new entry
	Disk1Table::Entry &entry = _entries[_count++];
	entry._mapNum = mapNum;
	entry._x = mapX;
	entry._y = mapY;
	entry._flags = 0;
	entry._field6 = 0;
	entry._field4 = 0;
	entry._field7 = 0;
	entry._field8 = 0;

	return &entry;
}

void Disk1Table::Entry::load(Common::Serializer &s) {
	s.syncAsByte(_mapNum);
	s.syncAsByte(_x);
	s.syncAsByte(_y);
	s.syncAsByte(_flags);
	s.syncAsUint16LE(_field4);
	s.syncAsByte(_field6);
	s.syncAsByte(_field7);
	s.syncAsUint16LE(_field8);
	s.syncAsUint16LE(_fieldA);
}

Disk1::Disk1(uint16 &mapX, uint16 &mapY) : _mapPosX(mapX), _mapPosY(mapY),
		_party(_partyCount) {
}


void Disk1::synchronize(Common::Serializer &s) {
	s.syncAsUint32LE(_saveCtr);
	s.syncAsUint16LE(_field4);
	s.syncAsByte(_gfxMode);
	s.syncAsByte(_field7);
	s.syncAsByte(_timeHours);
	s.syncAsByte(_timeMinutes);
	s.syncAsUint16LE(_mapPosX);
	s.syncAsUint16LE(_mapPosY);
	s.syncAsUint32LE(_cash);
	s.syncBytes(_maps, 10);
	s.syncBytes(_mapsX, 10);
	s.syncBytes(_mapsY, 10);
	s.syncAsByte(_mapIndex);

	s.syncAsByte(_partyCount);
	s.syncBytes(_partyIndexes, PARTY_COUNT);
	s.syncAsByte(_timeWeekday);
	s.syncAsUint16LE(_field38);
	_party.synchronize(s);

	s.syncBytes(_partyFlags, 250);
	s.syncBytes(_unknown5, 6);
	_table.load(s);
}

bool Disk1::load(bool &hasParty) {
	Common::File f;

	if (!f.open("DISK1"))
		return false;

	Common::Serializer s(&f, nullptr);
	synchronize(s);

	hasParty = _saveCtr != 0;

	if (!hasParty) {
		resetParty();

		// Reset game state to beginning
		_cash = 100;
		_timeHours = 9;
		_timeMinutes = 0;
		_timeWeekday = 1;
		_field7 = 0;
		_field38 = 0;
		_mapPosX = 10;
		_mapPosY = 6;
		_mapIndex = 0;
		_maps[0] = 28;
		_mapsX[0] = 10;
		_mapsY[0] = 6;

		_table.clear();
		_partyCount = 0;
	}

	return true;
}

void Disk1::resetParty() {
	for (int memberNum = 0; memberNum < PARTY_COUNT; ++memberNum) {
		PartyMember &member = _party[memberNum];
		_partyIndexes[memberNum] = memberNum;
		member.reset();

		_unknown5[memberNum] = 0;
	}
}

bool Disk1::isPartyAlive() const {
	for (uint i = 0; i < _partyCount; ++i) {
		if (_party[i]._con > 0 && _party[i]._field57)
			return true;
	}

	return false;
}

void Disk1::save() {
	// TODO
}

void Disk1::moveTo(int newX, int newY) {
	_mapPosX = newX;
	_mapPosY = newY;

	g_engine->_moveMessage = nullptr;
	g_engine->_mapVal2 = 0;
}

void Disk1::tableAdd(int opcode, int mapNum, int mapX, int mapY, int arg5, int arg6) {
	Map::MapTile &mapTile = g_engine->_disk._map._tiles[mapX][mapY];

	if (_maps[_mapIndex] == mapNum) {
		mapTile.proc2(opcode, arg5, arg6);

	} else {
		// Add the entry
		Disk1Table::Entry *entry = _table.add(mapNum, mapX, mapY);
	
		switch (opcode) {
		case Logic::kOpcode28:
			entry->_flags |= 0x80;
			entry->_field4 = arg5 & 7;
			break;

		case Logic::kOpcode29:
			entry->_flags |= 0x40;
			break;
		}
	}
	// TODO: Finish
}

} // namespace Data
} // namespace FOD
} // namespace Wasteland
