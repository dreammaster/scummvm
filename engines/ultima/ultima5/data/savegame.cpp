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
#include "ultima/ultima5/data/savegame.h"

namespace Ultima {
namespace Ultima5 {
namespace Data {

void Character::synchronize(Common::Serializer &s) {
	s.syncBytes((byte *)_name, NAME_SIZE);
	s.syncAsByte(_gender);
	s.syncAsByte(_class);
	s.syncAsByte(_status);
	s.syncAsByte(_str);
	s.syncAsByte(_dex);
	s.syncAsByte(_int);
	s.syncAsByte(_mag);
	s.syncAsSint16LE(_hp);
	s.syncAsSint16LE(_maxHp);
	s.syncAsSint16LE(_exp);
	s.syncAsByte(_level);
	s.syncAsByte(_unk17);
	s.syncAsByte(_unk18);
	s.syncBytes(_equipment, sizeof(_equipment));
	s.syncAsByte(_mapId);
}

void Savegame::synchronize(Common::Serializer &s) {
	s.syncAsUint16LE(_unk0000);

	for (int i = 0; i < PARTY_MAX; ++i)
		_party[i].synchronize(s);

	s.syncAsSint16LE(_food);
	s.syncAsSint16LE(_gold);
	s.syncAsByte(_keys);
	s.syncAsByte(_gems);
	s.syncAsByte(_torches);
	s.syncAsByte(_grapple);
	s.syncAsByte(_carpets);
	s.syncAsByte(_skullKeys);
	s.syncAsByte(_skullKeysDay);
	s.syncAsByte(_amulet);
	s.syncAsByte(_crown);
	s.syncAsByte(_sceptre);
	s.syncBytes(_shards, 4);
	s.syncAsByte(_spyGlass);
	s.syncAsByte(_capePlans);
	s.syncAsByte(_sextant);
	s.syncAsByte(_pocketWatch);
	s.syncAsByte(_blackBadge);
	s.syncAsByte(_sandalwoodBox);
	s.syncBytes(_equipment, 48);
	s.syncBytes(_mixtures, 48);
	s.syncBytes(_scrolls, 8);
	s.syncBytes(_potions, 8);
	s.syncBytes(_moonstoneX, 8);
	s.syncBytes(_moonstoneY, 8);
	s.syncBytes(_moonstoneState, 8);
	s.syncBytes(_moonstoneZ, 8);
	s.syncBytes(_reagents, 8);
	s.syncBytes(_harvestDays, 3);
	s.syncAsByte(_partySize);
	s.syncBytes(_unk02B6, 24);
	s.syncAsUint16LE(_year);
	s.syncAsSint16LE(_unk02D0);
	s.syncAsSint16LE(_unk02D2);
	s.syncAsByte(_icon);
	s.syncAsByte(_activeChar);
	s.syncAsByte(_partyTile);
	s.syncAsByte(_month);
	s.syncAsByte(_day);
	s.syncAsByte(_hour);
	s.syncAsByte(_hourCopy);
	s.syncAsByte(_minute);
	s.syncAsByte(_unk02DC);
	s.syncAsByte(_unk02DD);
	s.syncAsByte(_unk02DE);
	s.syncAsByte(_trammelPhase);
	s.syncAsByte(_feluccaPhase);
	s.syncAsByte(_moongateHeight);
	s.syncAsByte(_karma);
	s.syncAsByte(_unk02E3);
	s.syncAsByte(_unk02E4);
	s.syncAsByte(_turnCount);
	s.syncAsByte(_unk02E6);
	s.syncAsByte(_unk02E7);
	s.syncAsByte(_protection);
	s.syncAsByte(_unk02E9);
	s.syncAsByte(_unk02EA);
	s.syncAsByte(_animate);
	s.syncAsByte(_windDirection);
	s.syncAsByte(_mapId);
	s.syncAsByte(_combatMapId);
	s.syncAsByte(_mapLevel);
	s.syncAsByte(_mapX);
	s.syncAsByte(_mapY);
	s.syncAsByte(_crosshairShown);
	s.syncAsByte(_crosshairX);
	s.syncAsByte(_crosshairY);
	s.syncAsByte(_chunkX);
	s.syncAsByte(_chunkY);
	s.syncAsByte(_attackerWeapon);
	s.syncAsByte(_unk02F8);
	s.syncAsByte(_focusShown);
	s.syncAsByte(_combatExitDirection);
	s.syncAsByte(_combatFlags);
	s.syncAsByte(_unk02FC);
	s.syncAsByte(_battleWon);
	s.syncAsByte(_updateMap);
	s.syncAsByte(_light);
	s.syncAsByte(_unk0300);
	s.syncAsByte(_unk0301);
	s.syncBytes(_unk0302, 32);
	s.syncBytes(_unk0322, 3);
	s.syncAsByte(_unk0325);
	s.syncAsUint16LE(_shrineActive);
	s.syncAsUint16LE(_shrineDone);
	s.syncBytes(_unk032A, 8);
	s.syncBytes(_unk0332, 8);
	s.syncBytes(_unk033A, 14);
	s.syncBytes(_spellCounts, 32);
	s.syncBytes(_unk0368, 32);
	s.syncBytes(_unk0388, 32);
	s.syncAsByte(_unk03A8);
	s.syncAsByte(_doorOpen);
	s.syncAsByte(_doorX);
	s.syncAsByte(_doorY);
	s.syncAsByte(_doorTurns);
	s.syncAsByte(_shipX);
	s.syncAsByte(_shipY);
	s.syncAsByte(_unk03AF);
	s.syncAsByte(_unk03B0);
	s.syncAsByte(_drunkTurns);
	s.syncAsByte(_unk03B2);
	s.syncAsByte(_unk03B3);
	s.syncBytes(_mapData, 512);
	s.syncBytes(_npcKilled, 128);
	s.syncBytes(_npcMet, 128);
	s.syncBytes(_objects, 256);
	s.syncAsUint16LE(_unk07B4);
	s.syncAsUint16LE(_unk07B6);
	s.syncBytes(_unk07B8, 512);
	s.syncBytes(_unk09B8, 512);
	s.syncBytes(_unk0BB8, 1024);
	s.syncBytes(_unk0FB8, 64);
	s.syncBytes(_unk0FF8, 32);
	s.syncAsByte(_unk1018);
	s.syncAsByte(_unk1019);
	s.syncAsUint16LE(_unk101A);
	s.syncBytes(_unk101C, 64);
	s.syncAsByte(_unk105C);
	s.syncAsByte(_unk105D);
	s.syncAsByte(_unk105E);
	s.syncAsByte(_unk105F);
}

bool Savegame::load(const char *filename) {
	Common::File f;
	if (!f.open(filename) || f.size() != SAVEGAME_SIZE)
		return false;

	Common::Serializer s(&f, nullptr);
	synchronize(s);
	return !f.err();
}

} // namespace Data
} // namespace Ultima5
} // namespace Ultima
