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
	s.syncAsUint16LE(_fieldE);
	s.syncAsUint16LE(_field10);
	s.syncAsByte(_field12);
	s.syncBytes(_unknown1, 9);
	s.syncAsByte(_field1C);
	s.syncBytes(_unknown2, 9);
	s.syncAsByte(_field26);
	s.syncBytes(_unknown3, 9);
	s.syncAsByte(_field30);

	s.syncAsByte(_partyCount);
	s.syncBytes(_partyIndexes, PARTY_COUNT);
	s.syncAsByte(_timeWeekday);
	s.syncAsUint16LE(_field38);
	_party.synchronize(s);

	s.syncBytes(_unknown4, 250);
	s.syncBytes(_field7B0, 6);
	s.syncAsUint16LE(_field7B6);
	s.syncBytes(_unknown5, 1800);
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

		_fieldE = 100;
		_field10 = 0;
		_timeHours = 9;
		_timeMinutes = 0;
		_timeWeekday = 1;
		_field7 = 0;
		_field38 = 0;
		_fieldA = 10;
		_fieldC = 6;
		_field30 = 0;
		_field12 = 28;
		_field1C = 10;
		_field26 = 6;
		_field7B6 = 0;
		_partyCount = 0;
	}

	return true;
}

void Disk1::resetRoster() {
	for (int memberNum = 0; memberNum < PARTY_COUNT; ++memberNum) {
		PartyMember &member = _party[memberNum];
		_partyIndexes[memberNum] = memberNum;
		member.reset();

		_field7B0[memberNum] = 0;
	}
}

} // namespace Data
} // namespace FOD
} // namespace Wasteland
