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

#include "ultima/ultima5/logic/attack_logic.h"
#include "ultima/ultima5/data/combat_data.h"
#include "ultima/ultima5/logic/combat_logic.h"
#include "ultima/ultima5/logic/npc_logic.h"
#include "ultima/ultima5/logic/world_logic.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {

constexpr byte OUTDOOR_MAP = 0;
constexpr byte TILE_WATER_LAST = 3;
constexpr byte TILE_FIRST_ENEMY = 0x40;
constexpr byte TILE_REMAINS = 0xE8;
constexpr byte TILE_AFTER_REMAINS = 0xF0;
constexpr byte TILE_SHARD = 0xB4;
constexpr byte TILE_DAEMON = 0xD8;
constexpr byte TILE_BLACKTHORN = 0x78;
constexpr byte TILE_SHADOWLORD = 0xFC;
constexpr byte TILE_GUARD = 0x70;
constexpr byte TILE_PIRATE = 0x2C;
constexpr byte TILE_SKIFF = 0x28;
constexpr byte TILE_CARPET = 0x14;
constexpr byte TILE_PEASANTS_END = 0x80;
constexpr byte TILE_LAST_NPC = 0x74;
constexpr byte TILE_MIRROR = 0x9D;
constexpr byte TILE_MIRROR_BROKEN = 0x9F;
constexpr byte TILE_BED = 0xAB;
constexpr byte TILE_SLEEPING_A = 0x84;
constexpr byte TILE_SLEEPING_B = 0x85;
constexpr int NPC_ANGRY = 0xFD;
constexpr int NPC_CALM = 0xFE;
constexpr int KARMA_LOSS = 5;
constexpr int HOSTILE_SCHEDULE = 3;
constexpr int CALL_FOR_GUARDS_SCHEDULE = 6;
constexpr int CALL_FOR_GUARDS_FIRST_SPECIAL = 7;
constexpr int FIRST_NPC_SPECIAL = 0x2F;

static Data::Savegame &save() {
	return _G(savegame);
}

bool AttackLogic::canAttack() {
	const Data::Savegame &sg = save();
	const byte ground = g_engine->getMapTile(sg._mapX, sg._mapY);
	bool onFoot;

	if (sg._mapId == OUTDOOR_MAP)
		onFoot = ground <= TILE_WATER_LAST && ((sg._partyTile & 0xFC) == TILE_SKIFF || (sg._partyTile & 0xFE) == TILE_CARPET);
	else
		onFoot = ground <= TILE_WATER_LAST && sg._partyTile != TILE_AVATAR;

	if (onFoot)
		_log.print("On foot!\n");

	return !onFoot;
}

int AttackLogic::findObject(int x, int y) {
	const Data::Savegame &sg = save();

	for (int i = 1; i < Data::ACTOR_COUNT; ++i) {
		const Data::ActorRecord actor = sg.getActor(i);
		if (actor._x == x && actor._y == y && actor._z == sg._mapLevel && actor._tile != 0)
			return i;
	}

	return 0;
}

int AttackLogic::findNpc(int object) {
	const Data::Savegame &sg = save();

	for (int i = 0; i < Data::NPC_COUNT; ++i) {
		const Data::NpcState state = sg.getNpcState(i);
		if (state._object == object && sg._npcTypes[i] != 0 && state._present != 0)
			return i;
	}

	return -1;
}

void AttackLogic::lowerKarma() {
	Data::Savegame &sg = save();
	sg._karma = sg._karma > KARMA_LOSS ? sg._karma - KARMA_LOSS : 0;
}

void AttackLogic::callGuard(int npc) {
	Data::Savegame &sg = save();
	Data::NpcSchedule schedule = sg.getSchedule(npc);
	const byte type = sg._npcTypes[npc] >= FIRST_NPC_SPECIAL ? CALL_FOR_GUARDS_FIRST_SPECIAL : CALL_FOR_GUARDS_SCHEDULE;

	memset(schedule._time, 0, sizeof(schedule._time));
	for (int i = 0; i < Data::NPC_SCHEDULE_SLOTS; ++i)
		schedule._type[i] = type;

	sg.setSchedule(npc, schedule);
}

void AttackLogic::provoke(int npc) {
	Data::Savegame &sg = save();
	Data::NpcSchedule schedule = sg.getSchedule(npc);
	Data::NpcState state = sg.getNpcState(npc);
	bool scheduled = false;

	for (int i = 0; i < Data::NPC_TIME_SLOTS; ++i) {
		if (schedule._time[i] != 0)
			scheduled = true;
	}

	const byte kind = sg._npcTypes[npc];
	if (kind < TILE_LAST_NPC && kind >= TILE_FIRST_ENEMY && (state._dialog == NPC_CALM || scheduled)) {
		state._dialog = NPC_ANGRY;
		sg.setNpcState(npc, state);

		for (int i = 0; i < Data::NPC_SCHEDULE_SLOTS; ++i)
			schedule._type[i] = HOSTILE_SCHEDULE;
		sg.setSchedule(npc, schedule);
	}
}

void AttackLogic::callGuards() {
	Data::Savegame &sg = save();

	for (int i = 0; i < Data::NPC_COUNT; ++i) {
		if (sg.getNpcState(i)._present == 0)
			continue;

		const byte kind = sg._npcTypes[i];
		if (kind == TILE_SHADOWLORD || kind == TILE_DAEMON || kind == TILE_GUARD)
			callGuard(i);
		else if (_G(gameRandom).range(0, 0xFF) < 0x80)
			provoke(i);
	}
}

bool AttackLogic::townAttack(int dx, int dy, Fight &fight) {
	Data::Savegame &sg = save();
	const int x = sg._mapX + dx;
	const int y = sg._mapY + dy;
	const byte ground = g_engine->getMapTile(x, y);

	if (ground == TILE_MIRROR) {
		_G(location).setTile(x, y, TILE_MIRROR_BROKEN);
		_log.print("Broken!\n");
		return true;
	}

	const int object = findObject(x, y);
	const byte tile = object != 0 ? sg.getActor(object)._tile : 0;
	int npc = -1;
	bool nothing = true;

	if (tile != 0) {
		npc = findNpc(object);

		if (tile >= TILE_FIRST_ENEMY && (tile < TILE_REMAINS || tile >= TILE_AFTER_REMAINS) && (tile & 0xFC) != TILE_SHARD)
			nothing = false;
	}

	if (nothing) {
		_log.print("Nothing to attack!\n");
		return true;
	}

	if (tile < TILE_PEASANTS_END) {
		lowerKarma();
		callGuards();
	} else if ((tile & 0xFC) == TILE_DAEMON) {
		callGuards();
	}

	switch (ground) {
	case TILE_SLEEPING_A:
	case TILE_SLEEPING_B:
	case TILE_MIRROR_BROKEN:
	case TILE_BED:
		// Whoever is asleep can't fight back
		if (tile == TILE_BLACKTHORN) {
			_log.print("Missed!\n");
		} else {
			_log.print("Murdered!\n");
			lowerKarma();

			if (npc >= 0) {
				sg.setNpcKilled(npc);
				NpcLogic::despawn(sg, npc);
			}
		}
		break;

	default:
		if (npc >= 0) {
			sg.setNpcKilled(npc);
			fight._started = true;
			fight._actor = object;
			fight._npc = npc;
		}
		break;
	}

	return true;
}

void AttackLogic::outdoorAttack(int dx, int dy, Fight &fight) {
	const Data::Savegame &sg = save();
	const int object = findObject(sg._mapX + dx, sg._mapY + dy);
	const int group = (object != 0 ? sg.getActor(object)._tile : 0) & 0xFC;

	if (group == TILE_PIRATE || (group != TILE_SHARD && group != TILE_REMAINS && group >= TILE_FIRST_ENEMY)) {
		fight._started = true;
		fight._actor = object;
	} else {
		_log.print("Nothing to attack!\n");
	}
}

bool AttackLogic::attack(int dx, int dy, Fight &fight) {
	if (save()._mapId == OUTDOOR_MAP) {
		outdoorAttack(dx, dy, fight);
		return false;
	}

	return townAttack(dx, dy, fight);
}

void AttackLogic::prepare(Fight &fight) {
	Data::Savegame &sg = save();
	const Data::ActorRecord actor = sg.getActor(fight._actor);
	const int group = actor._tile & 0xFC;

	// The name of what is met is centered
	const Common::String name = group < TILE_FIRST_ENEMY ? "PIRATES" : Data::ENEMY_GROUP_NAMES[(group - TILE_FIRST_ENEMY) / 4];
	Common::String line = "\n";
	for (int i = (Views::LOG_COLUMNS - 1 - (int)name.size()) / 2; i > 0; --i)
		line += ' ';

	_log.print(line + name + "\n\n");

	const byte ground = g_engine->getMapTile(actor._x, actor._y);
	fight._map = CombatLogic::mapFor(ground, sg._partyTile, group, sg._mapId);
	fight._kind = CombatLogic::enemyKind(actor._tile);
	fight._lone = actor._hull <= 0x7F;

	if (group == TILE_SHADOWLORD && sg._sceptre != 0) {
		_log.print("The Sceptre is reclaimed!\n");
		sg._sceptre = 0;
	}
}

void AttackLogic::afterFight(const Fight &fight, bool won) {
	Data::Savegame &sg = save();
	Data::ActorRecord actor = sg.getActor(fight._actor);

	if (won && (actor._tile & 0xFC) == TILE_PIRATE) {
		// What is left of a pirate ship
		actor._tile -= 8;
		actor._animTile -= 8;
		actor._hull = 99;
		actor._skiffs = 2;
	} else {
		actor._tile = actor._animTile = actor._x = actor._y = actor._z = 0;
	}

	sg.setActor(fight._actor, actor);

	if (fight._npc >= 0)
		NpcLogic::despawn(sg, fight._npc);
}

} // namespace Ultima5
} // namespace Ultima
