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
#include "wasteland/fod/data/party.h"
#include "wasteland/fod/fod.h"

namespace Wasteland {
namespace FOD {
namespace Data {

void MemberArray1Entry::synchronize(Common::Serializer &s) {
	s.syncAsByte(_id);
	s.syncAsByte(_field1);
	s.syncAsByte(_ammo);
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
	s.syncAsByte(_attributesRemaining);
	s.syncBytes(_activeSkills, SKILLS_COUNT);
	s.syncBytes(_passiveSkills, SKILLS_COUNT);
	s.syncAsUint16LE(_con);
	s.syncAsUint16LE(_conBase);
	s.syncAsByte(_profession);
	s.syncAsByte(_field49);
	s.syncAsByte(_equippedWeapon);
	s.syncBytes(_array2, 3);
	s.syncAsByte(_rank);
	s.syncAsByte(_field4F);
	s.syncAsByte(_sex);
	s.syncAsByte(_field51);
	s.syncAsByte(_field52);
	s.syncAsByte(_field53);
	s.syncAsByte(_field54);
	s.syncAsByte(_field55);
	s.syncAsByte(_field56);
	s.syncAsByte(_field57);
	s.syncAsByte(_afflicted);
	s.syncAsByte(_field59);
	s.syncAsUint16LE(_field5A);
	s.syncAsUint16LE(_field5C);
	s.syncAsUint16LE(_field5E);
	s.syncAsUint16LE(_field60);
	for (int i = 0; i < 32; ++i)
		_array1[i].synchronize(s);
	s.syncAsUint16LE(_conTemp);
	s.syncBytes(_activeSkills2, SKILLS_COUNT);
	s.syncBytes(_passiveSkills2, SKILLS_COUNT);
	s.syncAsUint32LE(_field144);
	s.syncAsUint32LE(_field148);
}

void PartyMember::reset() {
	_name.clear();
	_field52 = 0;

	for (int i = 0; i < 32; ++i)
		_array1[i] = MemberArray1Entry();
	_array1[0]._id = 2;
	_array1[0]._ammo = 9;
	_array1[1]._id = 12;
	_array1[2]._id = 12;
	_array1[3]._id = 32;
	_array1[4]._id = 41;
	_array1[5]._id = 55;

	_rank = 1;
	_field5E = 1500;
	_field60 = 0;
	_equippedWeapon = 0xff;

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

void Party::synchronize(Common::Serializer &s) {
	for (int i = 0; i < 5; ++i)
		_party[i].synchronize(s);
}

int Party::getMemberByStatus(int status) const {
	for (uint i = 0; i < _count; ++i) {
		if (_party[i].getStatus() < status)
			return i;
	}

	return _count;
}

void Party::healOrDamageParty() {
	if (_conDamaging) {
		damageParty(0, _conMin, _conMax, g_engine->_moveMessage);

	} else {
		for (uint partyNum = 0; partyNum < _count; ++partyNum) {
			auto &member = _party[partyNum];
			if (!(member._afflicted & ~AFFLICTED_80))
				healMember(partyNum, _conMin, _conMax);
		}
	}
}

void Party::damageParty(int, int min, int max, const char *message) {
	// TODO
}

int Party::damageMember(int partyNum, int min, int max) {
	PartyMember &member = _party[partyNum];
	int amount = g_engine->getRandomNumber(min, max);
	int ac = _ignoreMemberAC ? 0 : member.getArmorClass();

	if (ac) {
		ac = g_engine->getRandomNumber(g_engine->_globals._field0 * ac,
			g_engine->_globals._field1 * ac);
	}

	if (amount > ac) {
		amount -= ac;
		member.updateCon(amount);
		return amount;
	} else {
		return 0;
	}
}

int Party::healMember(int partyNum, int min, int max) {
	PartyMember &member = _party[partyNum];
	int amount = g_engine->getRandomNumber(min, max);

	member._con = MIN((int)member._conBase, (int)member._con + amount);
	return member._con;
}

const Profession &PartyMember::getProfession() const {
	return g_engine->_archetypes._professions[_profession];
}

MemberStatus PartyMember::getStatus() const {
	if (_con >= 1)
		return STATUS_OK;
	else if (_con > -60)
		return (MemberStatus)(ABS(_con) / 15 + 1);
	else
		return STATUS_DEAD;
}

int PartyMember::getArmorClass() const {
	int total = 0;

	for (int i = 0; i < 3; ++i) {
		int index = _array2[i];

		if (index != 0xff) {
			int itemIndex = _array1[index]._id;
			const GlobalItem &gi = g_engine->_globals._items[itemIndex];
			total += gi._ac;
		}
	}

	return total ? total : getProfession()._ac;
}

void PartyMember::updateCon(int amount) {
	auto &disk1 = g_engine->_disk1;

	if (amount > 0) {
		if (_con > 0) {
			int con = _con - amount;

			if (con < 1) {
				int threshold = 100 * _con / _conBase;
				_conTemp = (threshold > 50) ? (_conBase / 4) + 1 : _con;					
			}
		}

		if (!(disk1._partyFlags[_field49] & PARTYFLAG_DEAD))
			_con -= amount;

		if (_con <= -60)
			disk1._partyFlags[_field49] |= PARTYFLAG_DEAD;
	}
}

} // namespace Data
} // namespace FOD
} // namespace Wasteland
