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

#include "common/endian.h"
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
	s.syncBytes(_npcSchedules, 512);
	s.syncBytes(_npcStates, 512);
	s.syncBytes(_npcMovement, 1024);
	s.syncBytes(_npcTalk, 64);
	s.syncBytes(_npcTypes, 32);
	s.syncAsByte(_unk1018);
	s.syncAsByte(_unk1019);
	s.syncAsUint16LE(_unk101A);
	s.syncBytes(_unk101C, 64);
	s.syncAsByte(_unk105C);
	s.syncAsByte(_unk105D);
	s.syncAsByte(_unk105E);
	s.syncAsByte(_unk105F);
}

ActorRecord Savegame::getActor(int idx) const {
	assert(idx >= 0 && idx < ACTOR_COUNT);
	const byte *p = &_objects[idx * ACTOR_SIZE];

	ActorRecord actor;
	actor._tile = p[0];
	actor._animTile = p[1];
	actor._x = p[2];
	actor._y = p[3];
	actor._z = p[4];
	actor._hull = p[5];
	actor._unk6 = p[6];
	actor._unk7 = p[7];
	return actor;
}

void Savegame::setActor(int idx, const ActorRecord &actor) {
	assert(idx >= 0 && idx < ACTOR_COUNT);
	byte *p = &_objects[idx * ACTOR_SIZE];

	p[0] = actor._tile;
	p[1] = actor._animTile;
	p[2] = actor._x;
	p[3] = actor._y;
	p[4] = actor._z;
	p[5] = actor._hull;
	p[6] = actor._unk6;
	p[7] = actor._unk7;
}

NpcSchedule Savegame::getSchedule(int npc) const {
	assert(npc >= 0 && npc < NPC_COUNT);
	const byte *p = &_npcSchedules[npc * NPC_SCHEDULE_SIZE];

	NpcSchedule schedule;
	memcpy(schedule._type, p, NPC_SCHEDULE_SLOTS);
	memcpy(schedule._x, p + 3, NPC_SCHEDULE_SLOTS);
	memcpy(schedule._y, p + 6, NPC_SCHEDULE_SLOTS);
	memcpy(schedule._z, p + 9, NPC_SCHEDULE_SLOTS);
	memcpy(schedule._time, p + 12, NPC_TIME_SLOTS);
	return schedule;
}

void Savegame::setSchedule(int npc, const NpcSchedule &schedule) {
	assert(npc >= 0 && npc < NPC_COUNT);
	byte *p = &_npcSchedules[npc * NPC_SCHEDULE_SIZE];

	memcpy(p, schedule._type, NPC_SCHEDULE_SLOTS);
	memcpy(p + 3, schedule._x, NPC_SCHEDULE_SLOTS);
	memcpy(p + 6, schedule._y, NPC_SCHEDULE_SLOTS);
	memcpy(p + 9, schedule._z, NPC_SCHEDULE_SLOTS);
	memcpy(p + 12, schedule._time, NPC_TIME_SLOTS);
}

NpcState Savegame::getNpcState(int npc) const {
	assert(npc >= 0 && npc < NPC_COUNT);
	const byte *p = &_npcStates[npc * NPC_STATE_SIZE];

	NpcState state;
	state._present = READ_LE_UINT16(p);
	state._x = READ_LE_INT16(p + 2);
	state._y = READ_LE_INT16(p + 4);
	state._z = READ_LE_UINT16(p + 6);
	state._tile = READ_LE_UINT16(p + 8);
	state._dialog = READ_LE_UINT16(p + 10);
	state._object = READ_LE_UINT16(p + 12);
	state._slot = READ_LE_UINT16(p + 14);
	return state;
}

void Savegame::setNpcState(int npc, const NpcState &state) {
	assert(npc >= 0 && npc < NPC_COUNT);
	byte *p = &_npcStates[npc * NPC_STATE_SIZE];

	WRITE_LE_UINT16(p, state._present);
	WRITE_LE_INT16(p + 2, state._x);
	WRITE_LE_INT16(p + 4, state._y);
	WRITE_LE_UINT16(p + 6, state._z);
	WRITE_LE_UINT16(p + 8, state._tile);
	WRITE_LE_UINT16(p + 10, state._dialog);
	WRITE_LE_UINT16(p + 12, state._object);
	WRITE_LE_UINT16(p + 14, state._slot);
}

bool Savegame::isNpcKilled(int npc) const {
	// Only some kinds of people stay dead: those that are chests, and the wizards upwards
	const byte type = _npcTypes[npc];
	if (type != NPC_KIND_BOX && type < NPC_KIND_FIRST_PERSON)
		return false;

	return (READ_LE_UINT32(&_npcKilled[(_mapId - 1) * 4]) & (1U << npc)) != 0;
}

void Savegame::synchronizeWorldObjects(Common::Serializer &s) {
	s.syncBytes(_worldObjects[0], sizeof(_worldObjects[0]));
	s.syncBytes(_worldObjects[1], sizeof(_worldObjects[1]));
}

bool Savegame::loadWorldObjects(const char *filename) {
	Common::File f;
	memset(_worldObjects, 0, sizeof(_worldObjects));

	return f.open(filename) && f.size() == sizeof(_worldObjects[1]) &&
		f.read(_worldObjects[1], sizeof(_worldObjects[1])) == sizeof(_worldObjects[1]);
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
