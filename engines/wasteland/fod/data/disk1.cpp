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

void Disk1::synchronize(Common::Serializer &s) {
	s.syncAsUint16LE(_field0);
	s.syncAsUint16LE(_field2);
	s.syncAsUint16LE(_field4);
	s.syncAsByte(_gfxMode);
	s.syncAsByte(_field7);
	s.syncAsByte(_timeHours);
	s.syncAsByte(_timeMinutes);
	s.syncAsUint16LE(_fieldA);
	s.syncAsUint16LE(_fieldC);
	s.syncAsUint32LE(_cash);
	s.syncBytes(_maps, 30);
	s.syncAsByte(_mapIndex);

	s.syncAsByte(_partyCount);
	s.syncBytes(_partyIndexes, PARTY_COUNT);
	s.syncAsByte(_timeWeekday);
	s.syncAsUint16LE(_field38);
	_party.synchronize(s);

	s.syncBytes(_unknown4, 250);
	s.syncBytes(_unknown5, 6);
	_table.load(s);
}

bool Disk1::load(bool &hasParty) {
	Common::File f;

	if (!f.open("DISK1"))
		return false;

	Common::Serializer s(&f, nullptr);
	synchronize(s);

	hasParty = _field2 || _field0;

	if (!hasParty) {
		resetRoster();

		// Reset game state to beginning
		_cash = 100;
		_timeHours = 9;
		_timeMinutes = 0;
		_timeWeekday = 1;
		_field7 = 0;
		_field38 = 0;
		_fieldA = 10;
		_fieldC = 6;
		_mapIndex = 0;
		_maps[0] = 28;
		_maps[10] = 10;
		_maps[20] = 6;
		_table._count = 0;
		_partyCount = 0;
	}

	return true;
}

void Disk1::resetRoster() {
	for (int memberNum = 0; memberNum < PARTY_COUNT; ++memberNum) {
		PartyMember &member = _party[memberNum];
		_partyIndexes[memberNum] = memberNum;
		member.reset();

		_unknown5[memberNum] = 0;
	}
}

} // namespace Data
} // namespace FOD
} // namespace Wasteland
