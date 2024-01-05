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

#ifndef WASTELAND_FOD_DATA_PARTY_H
#define WASTELAND_FOD_DATA_PARTY_H

#include "wasteland/engine.h"
#include "wasteland/fod/views/views.h"

namespace Wasteland {
namespace FOD {
namespace Data {

#define ROSTER_COUNT 5
#define ATTRIBUTES_COUNT 7
#define SKILLS_COUNT 16

enum Sex {
	SEX_OTHER = 0,
	SEX_MALE = 1,
	SEX_FEMALE = 2
};

struct MemberArray1Entry {
	byte _field0 = 0;
	byte _field1 = 0;
	byte _field2 = 0;
	byte _field3 = 0;
	byte _field4 = 0;
	byte _field5 = 0;

	void synchronize(Common::Serializer &s);
};

struct PartyMember {
	Common::String _name;
	byte _attributes[ATTRIBUTES_COUNT];
	uint32 _unknown1;
	byte _field23;
	byte _activeSkills[SKILLS_COUNT];
	byte _passiveSkills[SKILLS_COUNT];
	uint16 _con;
	uint16 _conBase;
	byte _profession;
	byte _field49;
	byte _field4A;
	byte _array2[3];
	byte _field4E;
	byte _field4F;
	byte _sex;
	byte _field51;
	byte _field52;
	byte _field53;
	byte _field54;
	byte _field55;
	byte _field56;
	byte _field57;
	byte _field58;
	byte _field59;
	uint16 _field5A;
	uint16 _field5C;
	uint16 _field5E;
	uint16 _field60;
	MemberArray1Entry _array1[32];
	uint16 _conTemp;
	byte _activeSkills2[SKILLS_COUNT];
	byte _passiveSkills2[SKILLS_COUNT];
	uint32 _field144;
	uint32 _field148;

	void synchronize(Common::Serializer &s);

	void reset();
};

class Roster {
private:
	PartyMember _party[5];

public:
	PartyMember &operator[](uint idx) {
		assert(idx < 5);
		return _party[idx];
	}
	const PartyMember &operator[](uint idx) const {
		assert(idx < 5);
		return _party[idx];
	}

	void synchronize(Common::Serializer &s);
};

struct Disk1 {
	uint16 _field0;
	uint16 _field2;
	uint16 _field4;
	byte _gfxMode = 3;
	byte _field7;
	byte _field8;
	byte _field9;
	uint16 _fieldA;
	uint16 _fieldC;
	uint16 _fieldE;
	uint16 _field10;
	byte _field12;
	byte _unknown1[9];
	byte _field1C;
	byte _unknown2[9];
	byte _field26;
	byte _unknown3[9];
	byte _field30;
	byte _partyCount;
	byte _partyIndexes[ROSTER_COUNT];
	byte _field37;
	uint16 _field38;
	Roster _party;
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

#endif // WASTELAND_H
