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

struct Disk1 {
	uint16 _field0;
	uint16 _field2;
	uint16 _field4;
	byte _gfxMode = 3;
	byte _field7;
	byte _timeHours;
	byte _timeMinutes;
	uint16 _fieldA;
	uint16 _fieldC;
	uint32 _cash;
	byte _maps[30];
	byte _mapIndex;
	byte _partyCount;
	byte _partyIndexes[PARTY_COUNT];
	byte _timeWeekday;
	uint16 _field38;
	Party _party;
	byte _unknown4[250];
	byte _field7B0[6];
	uint16 _field7B6;
	byte _unknown5[1800];

	void synchronize(Common::Serializer &s);

	/**
	 * Loads the data for disk 1
	 */
	bool load(bool &hasParty);

	void resetRoster();
};

} // namespace Data
} // namespace FOD
} // namespace Wasteland

#endif
