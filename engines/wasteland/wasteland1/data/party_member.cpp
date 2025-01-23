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

#include "common/hashmap.h"
#include "wasteland/wasteland1/data/party_member.h"
#include "wasteland/wasteland1/vars.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Data {

void PartyMember::synchronize(Serializer &s) {
	s.syncAsString(_name, 14);
	s.syncAsByte(_strength);
	s.syncAsByte(_iq);
	s.syncAsByte(_luck);
	s.syncAsByte(_speed);
	s.syncAsByte(_agility);
	s.syncAsByte(_dexterity);
	s.syncAsByte(_charisma);
	s.syncAsInt3(_money);
	s.syncAsByte(_gender);
	s.syncAsByte(_nationality);
	s.syncAsByte(_ac);
	s.syncAsUint16LE(_maxCon);
	s.syncAsUint16LE(_con);
	s.syncAsByte(_weapon);
	s.syncAsByte(_skillPoints);
	s.syncAsInt3(_experience);
	s.syncAsByte(_level);
	s.syncAsByte(_armor);
	s.syncAsUint16LE(_lastCon);
	s.syncAsByte(_afflictions);
	s.syncAsByte(_npc);
	s.syncAsByte(_unknown2A);
	s.syncAsByte(_itemRefuse);
	s.syncAsByte(_skillRefuse);
	s.syncAsByte(_attribRefuse);
	s.syncAsByte(_tradeRefuse);
	s.syncAsByte(_unknown2F);
	s.syncAsByte(_joinString);
	s.syncAsByte(_willingness);
	s.syncAsString(_rank, 25);

	s.skip(53);		// Unknown bytes that are always zero

	// Serialize skills and items
	_skills.synchronize(s);
	s.skip(1);
	_items.synchronize(s);

	s.skip(7);
}

int PartyMember::getConditionIndex() const {
	static const int8 CONDITION_THRESHOLDS[] = {
		-1, -11, -20, -30, -40
	};

	if (!_con)
		return 0;

	for (int idx = 4; idx >= 0; --idx) {
		if (_con <= CONDITION_THRESHOLDS[idx])
			return idx;
	}

	return -1;
}

const ItemDetails *PartyMember::getEquippedWeaponDetails() const {
	return !_weapon ? &g_vars->_itemDetails[0] :
		_items[_weapon].getItemDetails();
}

} // namespace Data
} // namespace Wasteland1
} // namespace Wasteland
