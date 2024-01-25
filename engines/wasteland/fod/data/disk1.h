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

#ifndef WASTELAND_FOD_DATA_DISK1_H
#define WASTELAND_FOD_DATA_DISK1_H

#include "wasteland/fod/data/party.h"

namespace Wasteland {
namespace FOD {
namespace Data {

struct Disk1Table {
	struct Entry {
		byte _mapNum = 0;
		byte _x = 0;
		byte _y = 0;
		byte _flags = 0;
		uint16 _field4 = 0;
		byte _field6 = 0;
		byte _field7 = 0;
		uint16 _field8 = 0;
		uint16 _fieldA = 0;

		void load(Common::Serializer &s);
	};

	uint16 _count;
	Entry _entries[150];

	void load(Common::Serializer &s);
};

struct Disk1 {
	uint32 _saveCtr;
	uint16 _field4;
	byte _gfxMode = 3;
	byte _field7;
	byte _timeHours;
	byte _timeMinutes;
	uint16 &_mapPosX;
	uint16 &_mapPosY;
	uint32 _cash;
	byte _maps[10];
	byte _mapsX[10];
	byte _mapsY[10];
	byte _mapIndex;
	byte _partyCount;
	byte _partyIndexes[PARTY_COUNT];
	byte _timeWeekday;
	uint16 _field38;
	Party _party;
	byte _unknown4[250];
	byte _unknown5[6];
	Disk1Table _table;

	int _mapVal1 = 0;
	int _mapVal2 = 0;

	Disk1(uint16 &mapX, uint16 &mapY);

	void synchronize(Common::Serializer &s);

	/**
	 * Loads the data for disk 1
	 */
	bool load(bool &hasParty);

	void resetParty();

	/**
	 * Returns true if someone in the party is alive
	 */
	bool isPartyAlive() const;

	void save();

	void moveTo(int newX, int newY);
};

} // namespace Data
} // namespace FOD
} // namespace Wasteland

#endif
