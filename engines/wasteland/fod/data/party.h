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
#include "wasteland/fod/data/archetypes.h"

namespace Wasteland {
namespace FOD {
namespace Data {

#define PARTY_COUNT 5
#define ATTRIBUTES_COUNT 7
#define SKILLS_COUNT 16
#define AFFLICTED_80 0x80
#define INVENTORY_COUNT 32

enum Sex {
	SEX_OTHER = 0,
	SEX_MALE = 1,
	SEX_FEMALE = 2
};

enum MemberStatus {
	STATUS_OK = 0,
	STATUS_UNCONSCIOUS = 1,
	STATUS_SERIOUS = 2,
	STATUS_CRITICAL = 3,
	STATUS_COMATOSE = 4,
	STATUS_DEAD = 5
};

struct InventoryItem {
	byte _id = 0;
	byte _field1 = 0;
	byte _ammo = 0;
	byte _field3 = 0;
	byte _field4 = 0;
	byte _field5 = 0;

	void synchronize(Serializer &s);

	void updateItem(int itemId);
};

struct PartyMember {
	Common::String _name;
	byte _attributes[ATTRIBUTES_COUNT];
	uint32 _unknown1;
	byte _attributesRemaining;
	byte _activeSkills[SKILLS_COUNT];
	byte _passiveSkills[SKILLS_COUNT];
	uint16 _con;
	uint16 _conBase;
	byte _profession;
	byte _field49;
	byte _equippedWeapon;
	byte _array2[3];
	byte _rank;
	byte _field4F;
	byte _sex;
	byte _field51;
	byte _inventoryCount;
	byte _field53;
	byte _field54;
	byte _field55;
	byte _field56;
	byte _field57;
	byte _afflicted;
	byte _field59;
	uint16 _field5A;
	uint16 _field5C;
	uint16 _field5E;
	uint16 _field60;
	InventoryItem _inventory[INVENTORY_COUNT];
	uint16 _conTemp;
	byte _activeSkills2[SKILLS_COUNT];
	byte _passiveSkills2[SKILLS_COUNT];
	uint32 _field144;
	uint32 _field148;

	void synchronize(Serializer &s);

	void reset();

	const Profession &getProfession() const;
	MemberStatus getStatus() const;
	int getArmorClass() const;
	void updateCon(int amount);
};

class Party {
private:
	PartyMember _party[5];

public:
	byte &_count;
	bool _ignoreMemberAC = false;
	int _conMin = 0;
	int _conMax = 0;
	bool _conDamaging = false;

public:
	Party(byte &count) : _count(count) {
	}

	PartyMember &operator[](uint idx) {
		assert(idx < 5);
		return _party[idx];
	}
	const PartyMember &operator[](uint idx) const {
		assert(idx < 5);
		return _party[idx];
	}

	void synchronize(Serializer &s);

	int getMemberByStatus(int status) const;

	void healOrDamageParty();
	void damageParty(int, int min, int max, const char *message);
	int damageMember(int partyNum, int min, int max);
	int healMember(int partyNum, int min, int max);
};

} // namespace Data
} // namespace FOD
} // namespace Wasteland

#endif
