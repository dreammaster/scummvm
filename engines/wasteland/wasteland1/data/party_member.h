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

#ifndef WASTELAND_WASTELAND1_DATA_PARTY_MEMBER_H
#define WASTELAND_WASTELAND1_DATA_PARTY_MEMBER_H

#include "wasteland/core/serializer.h"
#include "wasteland/wasteland1/data/items.h"
#include "wasteland/wasteland1/data/skills.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Data {

struct PartyMember {
	Common::String _name;
	int _strength = 0;
	int _iq = 0;
	int _luck = 0;
	int _speed = 0;
	int _agility = 0;
	int _dexterity = 0;
	int _charisma = 0;
	int _money = 0;
	int _gender = 0;
	int _nationality = 0;
	int _ac = 0;
	int _maxCon = 0;
	int _con = 0;
	int _weapon = 0;
	int _skillPoints = 0;
	int _experience = 0;
	int _level = 0;
	int _armor = 0;
	int _lastCon = 0;
	int _afflictions = 0;
	bool _npc = false;
	int _unknown2A = 0;
	int _itemRefuse = 0;
	int _skillRefuse = 0;
	int _attribRefuse = 0;
	int _tradeRefuse = 0;
	int _unknown2F = 0;
	int _joinString = 0;
	int _willingness = 0;
	Common::String _rank;
	Skills _skills;
	InventoryItems _items;

	void synchronize(Serializer &s);

	int getConditionIndex() const;

	const ItemDetails *getEquippedWeaponDetails() const;
};

} // namespace Data
} // namespace Wasteland1
} // namespace Wasteland

#endif
