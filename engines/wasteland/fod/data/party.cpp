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

#include "wasteland/fod/data/party.h"
#include "common/file.h"

namespace Wasteland {
namespace FOD {
namespace Data {

void MemberArray1Entry::synchronize(Common::Serializer &s) {
	s.syncAsByte(_field0);
	s.syncAsByte(_field1);
	s.syncAsByte(_field2);
	s.syncAsByte(_field3);
	s.syncAsByte(_field4);
	s.syncAsByte(_field5);
}

void PartyMember::synchronize(Common::Serializer &s) {
	char nameBuff[24];
	if (s.isSaving()) {
		Common::fill(nameBuff, nameBuff + 24, 0);
		Common::strcpy_s(nameBuff, _name.c_str());
		s.syncBytes((byte *)nameBuff, 24);
	} else {
		s.syncBytes((byte *)nameBuff, 24);
		nameBuff[23] = '\0';
		_name = Common::String(nameBuff);
	}

	s.syncBytes(_attributes, ATTRIBUTES_COUNT);
	s.syncAsUint32LE(_unknown1);
	s.syncAsByte(_field23);
	s.syncBytes(_array5, 16);
	s.syncBytes(_array6, 16);
	s.syncAsUint16LE(_con);
	s.syncAsUint16LE(_field46);
	s.syncAsByte(_profession);
	s.syncAsByte(_field49);
	s.syncAsByte(_field4A);
	s.syncBytes(_array2, 3);
	s.syncAsByte(_field4E);
	s.syncAsByte(_field4F);
	s.syncAsByte(_sex);
	s.syncAsByte(_field51);
	s.syncAsByte(_field52);
	s.syncAsByte(_field53);
	s.syncAsByte(_field54);
	s.syncAsByte(_field55);
	s.syncAsByte(_field56);
	s.syncAsByte(_field57);
	s.syncAsByte(_field58);
	s.syncAsByte(_field59);
	s.syncAsUint16LE(_field5A);
	s.syncAsUint16LE(_field5C);
	s.syncAsUint16LE(_field5E);
	s.syncAsUint16LE(_field60);
	for (int i = 0; i < 32; ++i)
		_array1[i].synchronize(s);
	s.syncAsUint16LE(_field122);
	s.syncBytes(_array3, 16);
	s.syncBytes(_array4, 16);
	s.syncAsUint32LE(_field144);
	s.syncAsUint32LE(_field148);
}

void PartyMember::reset() {
	_name.clear();
	_field52 = 0;

	for (int i = 0; i < 32; ++i)
		_array1[i] = MemberArray1Entry();
	_array1[0]._field0 = 2;
	_array1[0]._field2 = 9;
	_array1[1]._field0 = 12;
	_array1[2]._field0 = 12;
	_array1[3]._field0 = 32;
	_array1[4]._field0 = 41;
	_array1[5]._field0 = 55;

	_field4E = 1;
	_field5E = 1500;
	_field60 = 0;
	_field4A = 0xff;

	Common::fill(&_array2[0], &_array2[3], 0xff);

	_field5C = 0;
	_field5A = 0;
	_field4F = 0;
	_sex = SEX_MALE;
	_field53 = 0;
	_field54 = 0;
	_field55 = 0;
	_field56 = 0;
	_field57 = 0;
	_field49 = 0;
}

void Roster::synchronize(Common::Serializer &s) {
	for (int i = 0; i < 5; ++i)
		_party[i].synchronize(s);
}

void Disk1::synchronize(Common::Serializer &s) {
	s.syncAsUint16LE(_field0);
	s.syncAsUint16LE(_field2);
	s.syncAsUint16LE(_field4);
	s.syncAsByte(_gfxMode);
	s.syncAsByte(_field7);
	s.syncAsByte(_field8);
	s.syncAsByte(_field9);
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
	s.syncBytes(_partyIndexes, ROSTER_COUNT);
	s.syncAsByte(_field37);
	s.syncAsUint16LE(_field38);
	_roster.synchronize(s);

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

	if (!hasParty)
		resetRoster();

	return true;
}

void Disk1::resetRoster() {
	for (int memberNum = 0; memberNum < ROSTER_COUNT; ++memberNum) {
		PartyMember &member = _roster[memberNum];
		_partyIndexes[memberNum] = memberNum;
		member.reset();

		_field7B0[memberNum] = 0;
	}
}

} // namespace Data
} // namespace FOD
} // namespace Wasteland
