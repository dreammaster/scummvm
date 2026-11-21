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
#include "ultima/ultima5/logic/combat_logic.h"
#include "ultima/ultima5/data/combat_data.h"
#include "ultima/ultima5/data/dungeon_data.h"
#include "ultima/ultima5/logic/time_logic.h"
#include "ultima/ultima5/logic/world_logic.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {

using namespace Data;

// The kinds of enemy that the game treats differently
constexpr int ENEMY_GUARD = 12;
constexpr int ENEMY_MIMIC = 0x1A;
constexpr int ENEMY_REAPER = 0x1B;
constexpr int ENEMY_GAZER = 0x1C;
constexpr int ENEMY_GARGOYLE = 0x1E;
constexpr int ENEMY_SWARM = 0x1F;
constexpr int ENEMY_SEA_SERPENT = 0x2D;
constexpr int ENEMY_DAEMON = 0x26;
constexpr int ENEMY_SHADOWLORD = 0x2F;
constexpr int ENEMY_SEA_FIRST = 8;

// The tiles of the objects in a fight
constexpr byte TILE_CHEST = 0x01;
constexpr byte TILE_CIRCLE = 0x16;
constexpr byte TILE_HIDDEN_PARTY = 0x1D;
constexpr byte TILE_SLEEPING = 0x1E;
constexpr byte TILE_DEAD = 0x1F;
constexpr byte TILE_WIZARD = 0x40;
constexpr byte TILE_BARD = 0x44;
constexpr byte TILE_FIGHTER = 0x48;
constexpr byte TILE_COMBAT_AVATAR = 0x4C;
constexpr byte TILE_DAEMON = 0xD8;
constexpr byte TILE_FIRE = 0xE8;
constexpr byte TILE_SLEEP_FIELD = 0xE9;
constexpr byte TILE_ENERGY = 0xEA;
constexpr byte TILE_BARRIER = 0xEB;
constexpr byte TILE_SHIP_FIRST = 0x20;
constexpr byte TILE_SHOT = 0xF4;
constexpr byte TILE_PIRATE = 0x2C;

// The tiles of the map
constexpr byte MAP_SWAMP = 0x04;
constexpr byte MAP_FIRE_A = 0x8F;
constexpr byte MAP_FIRE_B = 0xBC;
constexpr byte MAP_COVER = 0x87;
constexpr byte MAP_GRAVE = 0x4C;
constexpr byte MAP_HIDDEN = 0xFF;
constexpr byte MAP_CHEST_FLOOR = 0xDC;

// Items with their own way of working
constexpr int ITEM_BOOMERANG = 0x13;
constexpr int ITEM_SPECIAL_FIRST = 0x23;
constexpr int ITEM_BARE_HANDS = 0xFF;
constexpr int ITEM_CHARM_RING = 0x23;
constexpr int ITEM_INVISIBLE_RING = 0x2A;
constexpr int ITEM_REGEN_RING = 0x2C;
constexpr int ITEM_SHATTERING_SWORD = 0x27;
constexpr int ITEM_MIRROR_SHIELD = 0x28;
constexpr int ITEM_TURNING_AMULET = 0x2D;
constexpr int ITEM_ARROWS = 0x1B;
constexpr int ITEM_QUARRELS = 0x1D;

constexpr int ICON_TIME_STOP = 'T';
constexpr int ICON_QUICK = 'Q';
constexpr int ICON_NEGATE = 'N';
constexpr int ICON_CHARM = 'C';
constexpr int ICON_PROTECTION = 'P';
constexpr int ICON_SHADOW = 0x1C;

constexpr int OUTCOME_OFF_MAP = 0x10;
constexpr int OUTCOME_GRAZED = 0x20;
constexpr int OUTCOME_POISONED = 0x08;
constexpr int OUTCOME_SLEPT = 0x04;
constexpr int OUTCOME_VANISHED = 0x02;
constexpr int OUTCOME_KILLED = 0x01;

constexpr int PATH_MAX_POINTS = 0x148;
constexpr int PATH_BUFFER = 0x200;
constexpr int PATH_LIMIT = 0x100;
constexpr int SPARK_STEPS = 0x30;

static int rnd(int low, int high) {
	return _G(gameRandom).range(low, high);
}

// A number from 0 to a limit
static int rnd0(int limit) {
	return rnd(0, limit);
}

// The roll an attack must beat, from 1 to 30
static int quickRoll() {
	const int value = rnd0(0x3C) / 2;
	return value == 0 ? 1 : value;
}

static Savegame &save() {
	return _G(savegame);
}

CombatLogic::CombatLogic(Views::MessageLog &log) : _log(log) {
	for (int i = 0; i < COMBAT_ENTITIES; ++i)
		_interferer[i] = 0;
}

void CombatLogic::say(const char *text) {
	_log.print(text);
}

void CombatLogic::newline() {
	_log.printChar('\n');
}

void CombatLogic::sayName(int idx) {
	if (_entities[idx]._flags & FLAG_PLAYER)
		say(save()._party[_entities[idx]._entity]._name);
	else
		say(ENEMY_NAMES[_entities[idx]._entity]);
}

bool CombatLogic::isEnemySide(int idx) const {
	const byte flags = _entities[idx]._flags;
	if (flags & FLAG_DEAD)
		return false;

	if (flags & FLAG_PLAYER) {
		const int who = _entities[idx]._entity;
		if (who != 0 && save()._party[who]._name[4] == 'j')
			return true;

		return (flags & FLAG_CHARMED) != 0;
	}

	return (flags & FLAG_CHARMED) == 0;
}

int CombatLogic::stat(int idx, int which) {
	const Entity &e = _entities[idx];
	const bool monster = (e._flags & FLAG_MONSTER) != 0;

	if (monster) {
		if (which == 0)
			which = (ENEMY_FLAGS[e._entity] & ENEMY_FLAG_STRONG) ? STAT_STR : STAT_DEX;
	} else if (which > 0) {
		if (which == ITEM_BARE_HANDS)
			which = STAT_DEX;
		else
			which = WEAPON_KIND[which] == 8 ? STAT_STR : STAT_DEX;
	}

	switch (which) {
	case STAT_INT:
		return monster ? ENEMY_STATS[e._entity]._int : save()._party[e._entity]._int;

	case STAT_DEX:
		if (save()._icon == ICON_TIME_STOP && monster)
			return 1;
		if (e._entity == ENEMY_MIMIC)
			return 1;
		if (e._flags & FLAG_ASLEEP)
			return 1;
		return e._dex;

	case STAT_STR:
		return monster ? ENEMY_STATS[e._entity]._str : save()._party[e._entity]._str;

	case STAT_DEF:
		return monster ? ENEMY_STATS[e._entity]._def : armour(e._entity);

	default:
		return 0;
	}
}

int CombatLogic::armour(int who) const {
	const Savegame &sg = save();
	const Character &c = sg._party[who];

	if (_origin == 40 && c._equipment[1] != 0x0F)
		return 0;

	int total = 0;
	for (int i = 0; i < 6; ++i) {
		if (c._equipment[i] != 0xFF)
			total += EQUIPMENT_DEFENSE[c._equipment[i]];
	}

	if (sg._icon == ICON_PROTECTION)
		total += 3;

	return total;
}

bool CombatLogic::canStand(int tile, int x, int y) {
	if (x > 10 || y > 10 || x < 0 || y < 0)
		return true;

	const byte ground = _map[y * COMBAT_MAP_STRIDE + x];
	if (!WorldLogic::isWalkable(tile, ground) || ground == MAP_HIDDEN)
		return false;

	for (int i = 0; i < COMBAT_ENTITIES; ++i) {
		if (_actors[i]._x == x && _actors[i]._y == y) {
			const byte t = _actors[i]._tile;
			if (t == TILE_BARRIER)
				return false;

			if ((t & 0xFC) == TILE_FIRE || t == TILE_SLEEPING || t == TILE_DEAD)
				continue;

			if (t != 0 && _actors[i]._animTile != 0)
				return false;
		} else if (_entities[i]._x == x && _entities[i]._y == y) {
			const byte flags = _entities[i]._flags;
			if ((flags & (FLAG_DEAD | FLAG_SWALLOWED)) == 0 && flags == 0)
				return false;

			if (_actors[i]._animTile != 0 && flags == FLAG_ASLEEP)
				return false;
		}
	}

	return true;
}

// Whether a fighter is barred from a place. Past the edge of the map only those that are fleeing may go
bool CombatLogic::isBlocked(int x, int y, int idx) {
	if (x > -1 && x < COMBAT_SIZE && y > -1 && y < COMBAT_SIZE)
		return !canStand(_actors[_entities[idx]._actor]._tile, x, y);

	return (_entities[idx]._flags & FLAG_FLEEING) == 0;
}

bool CombatLogic::boxedIn(int idx) {
	int count = 0;
	int x = _entities[idx]._x;
	int y = _entities[idx]._y;

	for (int i = 0; i < 4; ++i) {
		switch (i) {
		case 0:
			++y;
			break;
		case 2:
			x -= 2;
			break;
		default:
			--y;
			++x;
			break;
		}

		if (isBlocked(x, y, idx))
			++count;
	}

	return count == 4;
}

int CombatLogic::distancePoints(int ax, int ay, int bx, int by) {
	// The whole number part of the distance, found as the original does
	int squares = (ax - bx) * (ax - bx) + (ay - by) * (ay - by);
	int step = 1;
	int count = 0;

	while (squares >= step) {
		squares -= step;
		step += 2;
		++count;
	}

	return count;
}

int CombatLogic::distance(int a, int b) {
	return distancePoints(_entities[a]._x, _entities[a]._y, _entities[b]._x, _entities[b]._y);
}

int CombatLogic::findFighter(int x, int y) {
	int found = -1;

	for (int i = 0; i < COMBAT_ENTITIES; ++i) {
		if (_entities[i]._x == x && _entities[i]._y == y) {
			const byte flags = _entities[i]._flags;

			if (_actors[_entities[i]._actor]._animTile != TILE_SHOT) {
				if ((flags & (FLAG_PLAYER | FLAG_MONSTER)) != 0 && (flags & FLAG_DEAD) == 0 && (flags & FLAG_SWALLOWED) == 0) {
					found = i;
					break;
				}
			}

			found = -1;
		}
	}

	return found;
}

void CombatLogic::countSides(int &enemies, int &party) {
	enemies = party = 0;

	for (int i = 0; i < COMBAT_ENTITIES; ++i) {
		if (_entities[i]._flags != 0 && (_entities[i]._flags & FLAG_DEAD) == 0) {
			if (isEnemySide(i))
				++enemies;
			else
				++party;
		}
	}
}

// Frees one of the party that has been charmed. Gives its place, or -1 if there was none
int CombatLogic::uncharm() {
	int i;
	for (i = 0; i < 6; ++i) {
		if ((_entities[i]._flags & FLAG_CHARMED) && (_entities[i]._flags & FLAG_PLAYER)) {
			_entities[i]._flags &= ~FLAG_CHARMED;

			say(save()._party[_entities[i]._entity]._name);
			say(" passes out!");
			unequip(_entities[i]._entity, ITEM_CHARM_RING);
			fallAsleep(i);

			return i == 6 ? -1 : i;
		}
	}

	return i == 6 ? -1 : i;
}

void CombatLogic::clearFighters() {
	for (int i = 0; i < COMBAT_ENTITIES; ++i) {
		_actors[i] = ActorRecord();
		_entities[i] = Entity();
	}
}

// Puts a member of the party, an enemy or an object on the map. Gives back what it was put in
int CombatLogic::place(int what, int type, int x, int y) {
	int first = 0;
	int idx = 0;
	int placed = -1;
	Entity *e = nullptr;
	Savegame &sg = save();

	if (type != 2) {
		if (type == 0)
			first = 6;

		for (idx = first; idx < COMBAT_ENTITIES; ++idx) {
			if (_entities[idx]._flags != 0)
				continue;

			e = &_entities[idx];
			placed = idx;

			if (type == 1 && sg._party[what]._status != STATUS_DEAD) {
				e->_dex = sg._party[what]._dex;
				e->_timer = 36 - e->_dex;
				e->_flags = FLAG_PLAYER;
				if (sg._party[what]._status != STATUS_GOOD && sg._party[what]._status != STATUS_POISONED)
					e->_flags |= FLAG_ASLEEP;
			}

			if (type == 0) {
				e->_hp = ENEMY_STATS[what]._maxHp;
				e->_dex = rnd0(7) - 4 + ENEMY_STATS[what]._dex;
				if (e->_dex > 0x1E)
					e->_dex = ENEMY_STATS[what]._dex;

				e->_timer = 36 - e->_dex;
				e->_flags = FLAG_MONSTER;
				if (what == ENEMY_SEA_FIRST || what == ENEMY_SEA_FIRST + 1)
					e->_flags = FLAG_DEAD;
			}

			e->_entity = what;
			e->_x = x;
			e->_y = y;
			break;
		}
	}

	if (placed > -1 || type == 2) {
		int slot;
		for (slot = 0; slot < COMBAT_ENTITIES; ++slot) {
			if (_actors[slot]._tile != 0)
				continue;

			if (type == 0) {
				e->_actor = slot;
				_actors[slot]._animTile = _actors[slot]._tile = what * 4 + TILE_WIZARD;
			}

			if (type == 1)
				e->_actor = slot;

			if (type == 2) {
				_actors[slot]._animTile = _actors[slot]._tile = what;
				placed = slot;
			}

			_actors[slot]._x = x;
			_actors[slot]._y = y;
			_actors[slot]._z = 0;
			_actors[slot]._skiffs = 0xFF;
			_actors[slot]._hull = type == 0 ? e->_hp : what;

			if (type != 2)
				e->_actor = slot;
			break;
		}

		if (type != 2 && slot == COMBAT_ENTITIES && placed > -1)
			placed = -1;
	} else {
		placed = -1;
	}

	if (placed >= 0) {
		for (int i = 0; i < COMBAT_ENTITIES; ++i) {
			if (_actors[_entities[i]._actor]._skiffs == placed)
				_actors[_entities[i]._actor]._skiffs = 0xFF;
		}
	}

	return placed;
}

// Puts on the effects of the rings of a member of the party that are there for the fight
void CombatLogic::hideIfInvisible(int idx) {
	Entity &e = _entities[idx];
	Savegame &sg = save();

	if ((e._flags & FLAG_PLAYER) && (e._flags & (FLAG_DEAD | FLAG_ASLEEP)) == 0) {
		if (sg._party[e._entity]._equipment[4] == ITEM_INVISIBLE_RING) {
			_actors[e._actor]._animTile = TILE_HIDDEN_PARTY;
			e._flags |= FLAG_INVISIBLE;
		} else if (sg._party[e._entity]._equipment[4] == ITEM_REGEN_RING) {
			regenerate();
		}
	}
}

// The rings of regeneration heal now and then
void CombatLogic::regenerate() {
	Savegame &sg = save();

	for (int i = 0; i < sg._partySize; ++i) {
		if (sg._party[i]._status != STATUS_DEAD && sg._party[i]._equipment[4] == ITEM_REGEN_RING) {
			if (rnd(0, 7) == 7) {
				if (sg._party[i]._hp + 1 < sg._party[i]._maxHp)
					sg._party[i]._hp += 1;
				else
					sg._party[i]._hp = sg._party[i]._maxHp;
			}
		}
	}
}

void CombatLogic::wakeUp(int idx) {
	Entity &e = _entities[idx];

	if (e._flags & FLAG_ASLEEP) {
		if (e._flags & FLAG_PLAYER) {
			save()._party[e._entity]._status = STATUS_GOOD;
			if (e._flags & FLAG_INVISIBLE)
				_actors[e._actor]._animTile = TILE_HIDDEN_PARTY;
			else
				_actors[e._actor]._animTile = _actors[e._actor]._tile;
		} else {
			_actors[e._actor]._unk6 = 0;
		}

		e._flags &= ~FLAG_ASLEEP;
	}
}

void CombatLogic::fallAsleep(int idx) {
	Entity &e = _entities[idx];
	Savegame &sg = save();

	if (e._flags & FLAG_PLAYER) {
		if (sg._party[e._entity]._status != STATUS_DEAD) {
			sg._party[e._entity]._status = STATUS_SLEEP;
			e._flags |= FLAG_ASLEEP;
			_actors[e._actor]._animTile = TILE_SLEEPING;

			if (idx == sg._activeChar)
				sg._activeChar = NO_ACTIVE_CHARACTER;

			_outcome = OUTCOME_SLEPT;
		}
	} else {
		e._flags |= FLAG_ASLEEP;
		_outcome = OUTCOME_SLEPT;
		_actors[e._actor]._unk6 = 0xFF;
	}
}

bool CombatLogic::unequip(int who, int item) {
	Character &c = save()._party[who];
	bool found = true;

	if (item == c._equipment[0]) {
		c._equipment[0] = 0xFF;
	} else if (item == c._equipment[1]) {
		c._equipment[1] = 0xFF;
	} else if (item == c._equipment[2]) {
		c._equipment[2] = 0xFF;
	} else if (item == c._equipment[3]) {
		c._equipment[3] = 0xFF;
	} else if (item == c._equipment[4]) {
		c._equipment[4] = 0xFF;
		if (item == ITEM_INVISIBLE_RING && _current < COMBAT_ENTITIES)
			_entities[_current]._flags &= ~FLAG_INVISIBLE;
	} else if (item == c._equipment[5]) {
		c._equipment[5] = 0xFF;
	} else {
		found = false;
	}

	return found;
}

// Takes something off everyone in the party. Gives how many had it on
int CombatLogic::unequipAll(int item) {
	int count = 0;
	for (int i = 0; i < save()._partySize; ++i) {
		if (unequip(i, item))
			++count;
	}

	return count;
}

// Takes a fighter out of the fight, or an object off the map. A fighter is given as -index - 1
void CombatLogic::removeFighter(int idx) {
	int actor;

	if (idx < 0) {
		idx = -idx - 1;
		Entity &e = _entities[idx];
		actor = e._actor;
		const byte kept = e._entity;
		e = Entity();
		e._entity = kept;
	} else {
		actor = idx - 1;
	}

	// What the original leaves behind is the last two bytes
	ActorRecord &a = _actors[actor];
	a._tile = a._animTile = a._x = a._y = a._z = a._hull = 0;
}

void CombatLogic::placeParty() {
	Savegame &sg = save();
	clearFighters();

	for (int i = 0; i < sg._partySize; ++i) {
		if (sg._party[i]._status == STATUS_DEAD)
			continue;

		int ring = 0;
		if (sg._party[i]._equipment[4] == ITEM_INVISIBLE_RING)
			ring = ITEM_INVISIBLE_RING;
		if (sg._party[i]._equipment[4] == ITEM_REGEN_RING)
			ring = ITEM_REGEN_RING;

		if (ring != 0 && rnd(0, 0xF) == 0xB) {
			say("A ring has vanished!\n");
			unequip(i, ring);
		}

		const int idx = place(i, 1, _partyX[i], _partyY[i]);
		_actors[_entities[idx]._actor]._skiffs = 0xFF;

		byte tile = 0;
		switch (sg._party[i]._class) {
		case 'A':
			tile = TILE_COMBAT_AVATAR;
			break;
		case 'F':
		case 'P':
		case 'R':
			tile = TILE_FIGHTER;
			break;
		case 'D':
		case 'M':
			tile = TILE_WIZARD;
			break;
		case 'B':
		case 'S':
		case 'T':
			tile = TILE_BARD;
			break;
		}

		_actors[_entities[idx]._actor]._tile = tile;
		_actors[_entities[idx]._actor]._animTile = tile;

		if (sg._party[i]._status == STATUS_SLEEP)
			fallAsleep(idx);
		else
			hideIfInvisible(idx);
	}
}

void CombatLogic::placeEnemies(int kind, bool lone, bool shuffle) {
	// Who goes where is mixed up when the party is taken by surprise
	int order[16];
	for (int i = 0; i < 16; ++i)
		order[i] = i;

	if (shuffle) {
		for (int i = 0; i < 15; ++i) {
			const int other = rnd(0, 0xF);
			const int kept = order[i];
			order[i] = order[other];
			order[other] = kept;
		}
	}

	say("*** CONFLICT ***\n");

	int count;
	if (_origin != 0 && _origin < 0x21 && kind != ENEMY_GUARD && lone)
		count = 1;
	else
		count = ENEMY_STATS[kind]._maxGroup;

	if (count != 8 && count != 0x10 && count != 1) {
		count = rnd(1, count);
		if (count > 0x1F - 6)
			count = 0x1A;
	}

	place(kind, 0, _monsterX[order[0]], _monsterY[order[0]]);

	const int mixed = count / 4 + 1;
	for (int i = 1; i < count; ++i) {
		int what = kind;
		if (i < mixed && rnd0(8) == 0)
			what = ENEMY_SPLIT[kind];

		place(what, 0, _monsterX[order[i]], _monsterY[order[i]]);
	}
}

bool CombatLogic::loadMap(const char *filename, int offset) {
	Common::File f;
	if (!f.open(filename))
		return false;

	memset(_map, 0, sizeof(_map));
	f.seek(offset);
	f.read(_map, COMBAT_MAP_BYTES);

	// The places the party and the enemies start at are listed beside the map
	for (int i = 0; i < 6; ++i) {
		_partyX[i] = _map[3 * COMBAT_MAP_STRIDE + 11 + i];
		_partyY[i] = _map[3 * COMBAT_MAP_STRIDE + 17 + i];
	}

	for (int i = 0; i < 16; ++i) {
		_monsterX[i] = _map[6 * COMBAT_MAP_STRIDE + 11 + i];
		_monsterY[i] = _map[7 * COMBAT_MAP_STRIDE + 11 + i];
	}

	return true;
}

int CombatLogic::mapFor(byte ground, byte partyTile, byte enemyTile, byte mapId) {
	const int group = enemyTile & 0xFC;
	bool atSea = ground < 4 || ((ground & 0xFE) != 0x6A && (ground & 0xF0) == 0x60);

	if (group == 0xFC)
		return 10;

	if ((group & 0xF0) == 0x80)
		atSea = true;

	if ((partyTile & 0xF8) == TILE_SHIP_FIRST) {
		if (group == TILE_PIRATE)
			return 14;
		return atSea ? 11 : 13;
	}

	if (group == TILE_PIRATE)
		return 12;
	if (atSea)
		return 15;

	switch (ground) {
	case 0x04:
		return 1;
	case 0x05:
		return 2;
	case 0x06:
	case 0x08:
		return 3;
	case 0x07:
	case 0x1E:
	case 0x1F:
		return 4;
	case 0x09:
	case 0x0A:
		return 5;
	case 0x0B:
	case 0x0C:
	case 0x0D:
	case 0x0E:
	case 0x0F:
		return 6;
	case 0x1D:
	case 0x48:
	case 0x49:
	case 0x6A:
	case 0x6B:
		return 7;
	case 0x44:
		return 8;
	default:
		return mapId != 0 ? 8 : 2;
	}
}

int CombatLogic::enemyKind(byte tile) {
	return (tile & 0xFC) == TILE_PIRATE ? 1 : (tile - TILE_WIZARD) >> 2;
}

void CombatLogic::begin(int kind, bool lone, int flags, byte origin) {
	Savegame &sg = save();

	_origin = origin;
	_flags = flags;
	_exitCode = 0;
	_won = false;
	_result = 1;
	_current = 0xFF;
	_outcome = 0;
	_savedActive = sg._activeChar;
	sg._activeChar = NO_ACTIVE_CHARACTER;

	placeParty();

	if (_map[5 * COMBAT_MAP_STRIDE + 5] == MAP_CHEST_FLOOR) {
		// A chest waits in the middle of the room
		const int chest = place(TILE_CHEST, 2, 5, 5);
		_actors[chest]._hull = 7;
	}

	placeEnemies(kind, lone);

	int enemies, party;
	countSides(enemies, party);
	_won = enemies == 0;
	_stage = STAGE_SCAN;
	_current = 0;
}

// Works out which way a fighter would go to get to the fighter it's after, and gives who that is
int CombatLogic::pickTarget(int idx) {
	int target = -1;
	int closest = 99;
	int partySeen = -1;
	Entity &e = _entities[idx];
	// With no target, the original goes by whatever the last search left behind
	int tx = _lastTargetX;
	int ty = _lastTargetY;

	int side = isEnemySide(idx);
	if (save()._icon == ICON_CHARM) {
		if (stat(idx, STAT_INT) < quickRoll())
			side = 0;
	}

	for (int i = COMBAT_ENTITIES - 1; i > -1; --i) {
		const Entity &o = _entities[i];

		if (i != idx && o._flags != 0 && (o._flags & FLAG_DEAD) == 0) {
			const int otherSide = isEnemySide(i);

			if (side != otherSide) {
				if ((_origin == 40 || e._entity == ENEMY_SHADOWLORD || (o._flags & FLAG_INVISIBLE) == 0) &&
						(o._flags & FLAG_SWALLOWED) == 0) {
					if (i < 5)
						++partySeen;

					const int d = distancePoints(e._x, e._y, o._x, o._y);
					if (d < closest) {
						closest = d;
						target = i;
						tx = o._x;
						ty = o._y;
					}
				}
			}
		}
	}

	if (partySeen == -1 && target == -1)
		target = uncharm();

	if (target == -1) {
		// With nobody left to fight the enemies run
		for (int i = COMBAT_ENTITIES - 1; i > 5; --i) {
			if (_entities[i]._flags & FLAG_MONSTER) {
				_entities[i]._hp = 1;
				_entities[i]._flags |= FLAG_FLEEING;
			}
		}

		tx = 5;
		ty = 5;
	}

	_lastTargetX = tx;
	_lastTargetY = ty;
	_scratchX = _scratchY = 0;

	if (e._x > tx)
		_scratchX = -1;
	if (e._x < tx)
		_scratchX = 1;
	if (e._y > ty)
		_scratchY = -1;
	if (e._y < ty)
		_scratchY = 1;

	if (e._flags & FLAG_FLEEING) {
		_scratchX = -_scratchX;
		_scratchY = -_scratchY;
	}

	return target == idx ? -1 : target;
}

bool CombatLogic::randomSpot() {
	_scratchX = rnd0(0xF);
	_scratchY = rnd0(0xF);
	return !(_scratchX > 10 || _scratchY > 10);
}

void CombatLogic::spreadNear(int x, int y) {
	do {
		_scratchX = rnd(1, 3) + x - 2;
		_scratchY = rnd(1, 3) + y - 2;
	} while (_scratchX < 0 || _scratchX > 10 || _scratchY < 0 || _scratchY > 10);
}

// A fighter moves one step, or teleports. Gives true if it did
bool CombatLogic::monsterMove(int idx) {
	int moved = 0;
	int tried = 0;
	Entity &e = _entities[idx];

	if (e._entity == ENEMY_REAPER || e._entity == ENEMY_MIMIC)
		return false;

	if ((e._flags & FLAG_PLAYER) == 0 && (ENEMY_FLAGS[e._entity] & ENEMY_FLAG_TELEPORT) && save()._icon != ICON_NEGATE &&
			save()._icon != ICON_SHADOW && (boxedIn(idx) || rnd0(3) != 3) && randomSpot()) {
		if (canStand(_actors[e._actor]._tile, _scratchX, _scratchY)) {
			_actors[e._actor]._x = e._x = _scratchX;
			_actors[e._actor]._y = e._y = _scratchY;

			say(ENEMY_NAMES[e._entity]);
			say(" teleports!\n");
			++moved;
		}
	}

	if (moved == 0 && !boxedIn(idx)) {
		pickTarget(idx);

		if (rnd0(0xFF) > 0x7F && !isBlocked(e._x + _scratchX, e._y, idx)) {
			_scratchY = 0;
			tried = 999;
		} else if (!isBlocked(e._x, e._y + _scratchY, idx)) {
			_scratchX = 0;
			tried = 998;
		}

		if (tried < 990) {
			tried = 0;

			for (moved = 4; moved > 0; --moved) {
				tried = rnd0(3);

				switch (tried) {
				case 0:
					_scratchY = 1;
					_scratchX = 0;
					break;
				case 1:
					_scratchX = 1;
					_scratchY = 0;
					break;
				case 2:
					_scratchY = -1;
					_scratchX = 0;
					break;
				default:
					_scratchX = -1;
					_scratchY = 0;
					break;
				}

				if (!isBlocked(e._x + _scratchX, e._y + _scratchY, idx))
					tried = 991;

				if (tried > 990)
					break;

				_scratchX = _scratchY = 0;
			}
		}

		if (tried != 0) {
			e._x += _scratchX;
			_actors[e._actor]._x = e._x;

			e._y += _scratchY;
			_actors[e._actor]._y = e._y;

			moved = 1;

			if (!inside(e._x, e._y))
				_outcome = OUTCOME_OFF_MAP;
		}
	}

	return moved != 0;
}

// Whether a fighter takes the place of one the party has to be talked into leaving
bool CombatLogic::monsterSpecial(int idx) {
	Entity &e = _entities[idx];
	const int kind = e._entity;
	const int actor = e._actor;
	Savegame &sg = save();

	if (sg._icon == ICON_SHADOW || sg._icon == ICON_NEGATE || (e._flags & FLAG_PLAYER))
		return false;

	if (ENEMY_FLAGS[kind] & ENEMY_FLAG_CHARM) {
		const int victim = rnd(0, 0x1F);
		const byte flags = _entities[victim]._flags;

		if ((flags & FLAG_PLAYER) && (flags & (FLAG_DEAD | FLAG_INVISIBLE | FLAG_ASLEEP | FLAG_SWALLOWED | FLAG_CHARMED)) == 0) {
			// The player's intelligence against the enemy's
			const int mine = stat(idx, STAT_INT);
			const int theirs = stat(victim, STAT_INT);
			const bool resisted = quickRoll() < (theirs + 0x1E - mine) / 2;

			if (!resisted) {
				newline();

				_entities[victim]._flags |= FLAG_CHARMED;
				if (_entities[victim]._entity == sg._activeChar)
					sg._activeChar = NO_ACTIVE_CHARACTER;

				sayName(victim);
				say(" possessed!\n");

				if (kind == ENEMY_DAEMON)
					removeFighter(-idx - 1);
			}

			return true;
		}
	}

	if ((ENEMY_FLAGS[kind] & ENEMY_FLAG_DISAPPEAR) && rnd(0, 0xFF) < 0x20) {
		newline();
		sayName(idx);

		if (_actors[actor]._animTile == 0) {
			say(" reappears!");
			e._flags &= ~FLAG_INVISIBLE;
			_actors[actor]._animTile = _actors[actor]._tile;
		} else {
			say(" disappears!");
			e._flags |= FLAG_INVISIBLE;
			_actors[actor]._animTile = 0;
		}

		return true;
	}

	if ((ENEMY_FLAGS[kind] & ENEMY_FLAG_SUMMON) && rnd(0, 0xFF) < 0x20) {
		if (randomSpot() && canStand(TILE_DAEMON, _scratchX, _scratchY)) {
			const int daemon = place(ENEMY_DAEMON, 0, _scratchX, _scratchY);

			if (daemon != -1) {
				newline();
				sayName(idx);
				say(" gates in a daemon!\n");

				_actors[_entities[daemon]._actor]._tile = _actors[_entities[daemon]._actor]._animTile = TILE_DAEMON;
				return true;
			}
		}
	}

	return false;
}

void CombatLogic::giveExperience(int idx, int amount) {
	Character &c = save()._party[_entities[idx]._entity];
	if (c._exp + amount < 9999)
		c._exp += amount;
	else
		c._exp = 9999;
}

// Works out whether a blow hits, from how the two fighters compare
bool CombatLogic::hits(int target, int attacker, int item) {
	bool hit = false;
	int mine = STAT_INT;
	int theirs = STAT_INT;

	if (_spell) {
		if ((item > 0x29 && item < 0x32) || item == 0x33)
			hit = true;
		else
			theirs = mine = STAT_INT;
	} else {
		if (item == ITEM_SHATTERING_SWORD || item == ITEM_CHARM_RING || item == ITEM_MIRROR_SHIELD) {
			hit = true;
		} else {
			mine = STAT_DEX;
			theirs = item;
		}
	}

	if (!hit) {
		mine = stat(target, mine);
		theirs = stat(attacker, theirs);

		if (((mine - theirs) + 0x1E) / 2 <= quickRoll())
			hit = true;
	}

	return hit;
}

// How much damage a blow does, from the weapon and the armour
int CombatLogic::blowDamage(int attacker, int target) {
	int damage;

	if (_entities[attacker]._flags & FLAG_MONSTER) {
		damage = ENEMY_STATS[_entities[attacker]._entity]._atk;
	} else {
		damage = _item;

		switch (damage) {
		case ITEM_SHATTERING_SWORD:
			say("Thy sword hath shattered!\n");
			unequip(attacker, damage);
			damage = 99;
			break;

		case ITEM_BARE_HANDS:
			damage = 1;
			break;

		case ITEM_MIRROR_SHIELD:
			damage = 0;
			break;

		default:
			damage = WEAPON_DAMAGE[damage];
			if (damage > 1 && damage != 99)
				damage = rnd(1, damage);
			break;
		}
	}

	if (damage == 99)
		return 99;

	int defence;
	if (_entities[target]._flags & FLAG_MONSTER)
		defence = ENEMY_STATS[_entities[target]._entity]._def;
	else
		defence = save()._party[_entities[target]._entity]._unk18;

	if (defence != 0)
		damage -= rnd(1, defence);

	return damage;
}

// Wounds a fighter, and kills it if that's it. Gives the experience for a kill
int CombatLogic::takeDamage(int idx, int amount) {
	int experience = 0;
	Entity &e = _entities[idx];
	Savegame &sg = save();

	if (amount < 1) {
		_outcome = OUTCOME_GRAZED;
		amount = 0;
	}

	if (e._flags & FLAG_PLAYER) {
		Character &c = sg._party[e._entity];
		c._hp -= amount;

		if (c._hp < 1 || amount == 99) {
			c._hp = 0;
			e._flags |= FLAG_DEAD;
			c._status = STATUS_DEAD;

			_actors[e._actor]._tile = _actors[e._actor]._animTile = TILE_SLEEPING;
			if (e._entity == sg._activeChar)
				sg._activeChar = NO_ACTIVE_CHARACTER;
		}

		return 0;
	}

	if ((ENEMY_FLAGS[e._entity] & ENEMY_FLAG_UNDEAD) && !_special)
		amount /= 2;

	if (ENEMY_FLAGS[e._entity] & ENEMY_FLAG_NO_DAMAGE)
		amount = 0;

	if (e._hp < amount)
		e._hp = 0;
	else
		e._hp -= amount;

	if (e._hp == 0 || amount == 99) {
		experience = (ENEMY_STATS[e._entity]._maxHp >> 2) + 1;
		const int treasure = ENEMY_STATS[e._entity]._treasure;

		e._flags = FLAG_DEAD;
		e._hp = 0;

		const int x = e._x;
		const int y = e._y;
		const byte ground = _map[y * COMBAT_MAP_STRIDE + x];
		const uint16 flags = ENEMY_FLAGS[e._entity];
		ActorRecord &actor = _actors[e._actor];

		if ((flags & (ENEMY_FLAG_VANISH | ENEMY_FLAG_NO_CORPSE)) == 0) {
			if (e._entity == ENEMY_GAZER) {
				actor._tile = actor._animTile = TILE_DEAD;
				place(ENEMY_SWARM, 0, x, y);
			} else if (e._entity == ENEMY_GARGOYLE) {
				_map[y * COMBAT_MAP_STRIDE + x] = MAP_GRAVE;
				removeFighter(-idx - 1);
			} else if (ground == MAP_COVER || ground < 4) {
				removeFighter(-idx - 1);
			} else if (treasure >= quickRoll()) {
				actor._tile = actor._animTile = TILE_CHEST;
				actor._hull = treasure;

				if (treasure > quickRoll())
					actor._hull |= 0x80;
			} else {
				actor._tile = actor._animTile = TILE_DEAD;
			}
		} else if (flags & ENEMY_FLAG_VANISH) {
			say(ENEMY_NAMES[e._entity]);
			say(" vanishes!");
			_outcome = OUTCOME_VANISHED;
			actor._tile = actor._animTile = TILE_CIRCLE;
			removeFighter(-idx - 1);
			uncharm();
		} else {
			removeFighter(-idx - 1);
		}
	} else if (ENEMY_FLAGS[e._entity] & ENEMY_FLAG_DIVIDE) {
		for (int i = 0; i < 8; ++i) {
			spreadNear(e._x, e._y);

			const int x = _scratchX;
			const int y = _scratchY;
			if (canStand(_actors[e._actor]._tile, x, y)) {
				const int copy = place((_actors[e._actor]._tile - TILE_WIZARD) >> 2, 0, x, y);

				if (copy > -1) {
					_entities[copy]._hp = e._hp;
					say(ENEMY_NAMES[e._entity]);
					say(" divides!\n");
					break;
				}
			}
		}
	}

	return experience;
}

void CombatLogic::poison(int target, int attacker) {
	Savegame &sg = save();
	Entity &t = _entities[target];

	if ((t._flags & FLAG_PLAYER) && sg._party[t._entity]._status == STATUS_GOOD) {
		sg._party[t._entity]._status = STATUS_POISONED;
		say(sg._party[t._entity]._name);
		say(" is poisoned!\n");
		_outcome = OUTCOME_POISONED;
	} else {
		const int gained = takeDamage(target, rnd0(0x14));

		if (attacker > -1 && (_entities[attacker]._flags & FLAG_PLAYER))
			giveExperience(attacker, gained);
	}
}

// One fighter hits another
void CombatLogic::strike(int target, int attacker) {
	const bool byEnemy = (_entities[attacker]._flags & FLAG_PLAYER) == 0;
	const Entity &by = _entities[attacker];

	if (byEnemy && (ENEMY_FLAGS[by._entity] & (ENEMY_FLAG_POISON_BITE | ENEMY_FLAG_POISON_STING)) && rnd(0, 3) != 0) {
		poison(target, attacker);
		return;
	}

	if (byEnemy && by._entity == ENEMY_GAZER && (_entities[target]._flags & FLAG_ASLEEP) == 0) {
		fallAsleep(target);
		return;
	}

	if (_special && _item == 52) {
		fallAsleep(target);
		return;
	}

	if (!_special || _item != 51) {
		int damage = blowDamage(attacker, target);

		if (damage < 0 && (_entities[target]._flags & FLAG_PLAYER)) {
			_outcome = OUTCOME_GRAZED;
			return;
		}

		damage = takeDamage(target, damage);
		if (_entities[attacker]._flags & FLAG_PLAYER)
			giveExperience(attacker, damage);
	} else {
		poison(target, attacker);
	}
}

// How badly an enemy is hurt, and whether that makes it run
int CombatLogic::woundLevel(int idx) {
	Entity &e = _entities[idx];
	int level = 0;
	bool flee = false;

	if (e._flags & FLAG_PLAYER)
		return 0;

	int quarter = ENEMY_STATS[e._entity]._maxHp / 4;
	if (e._hp < quarter) {
		level = 1;
		flee = true;
	} else {
		quarter *= 2;

		if (e._hp < quarter) {
			level = 2;
			if (rnd0(0x100) > 0xFB)
				flee = true;
		} else {
			quarter = (quarter / 2) * 3;
			level = e._hp < quarter ? 3 : 4;
		}
	}

	if (flee)
		e._flags |= FLAG_FLEEING;
	else
		e._flags &= ~FLAG_FLEEING;

	return level;
}

// Says what a blow did
void CombatLogic::report(int idx, int attacker) {
	const byte flags = _entities[idx]._flags;

	_outcome &= ~OUTCOME_KILLED;
	if (_outcome & OUTCOME_GRAZED) {
		sayName(idx);
		say(" grazed!\n");
	}

	if ((_outcome & (OUTCOME_GRAZED | OUTCOME_VANISHED)) == 0) {
		if (flags == 0 || (flags & FLAG_DEAD)) {
			sayName(idx);
			say(" killed!\n");
			_outcome |= OUTCOME_KILLED;
		} else if (_outcome & OUTCOME_SLEPT) {
			sayName(idx);
			say(" slept!\n");
		} else if ((_outcome & OUTCOME_POISONED) == 0) {
			sayName(idx);

			if (flags & FLAG_PLAYER) {
				if (attacker != 0xFF && _entities[attacker]._entity == ENEMY_SEA_SERPENT) {
					say(" dragged under!\n");
					_entities[idx]._flags |= FLAG_SWALLOWED;
					_actors[_entities[idx]._actor]._animTile = 0;
				} else {
					say(" hit!\n");
				}
			} else {
				switch (woundLevel(idx)) {
				case 4:
					say(" barely wounded!\n");
					break;
				case 3:
					say(" lightly wounded!\n");
					break;
				case 2:
					say(" heavily wounded!\n");
					break;
				case 1:
					say(" critical!\n");
					break;
				}
			}
		}

		_outcome &= 0xF3;
	} else {
		_outcome &= 0xDD;
	}
}

void CombatLogic::missed(int idx) {
	if (_spell) {
		say("Failed!\n");
	} else {
		sayName(idx);
		say(" missed!\n");
	}
}

// Works out where a thrown or fired thing goes from where it's aimed, and who it hits
int CombatLogic::project(int attacker, int tx, int ty, bool landed, int kind) {
	int lx = tx;
	int ly = ty;
	const int ax = _entities[attacker]._x;
	const int ay = _entities[attacker]._y;
	bool reached = false;

	if (!landed) {
		do {
			spreadNear(lx, ly);
			if (ax != _scratchX)
				break;
		} while (ay == _scratchY);

		lx = _scratchX;
		ly = _scratchY;
	}

	if (_item == 25 || _item == 34) {
		if (landed) {
			lx = tx;
			ly = ty;
			reached = true;
		}
	} else {
		reached = pathClear(ax, ay, lx, ly, kind);

		if (reached)
			triggers(lx, ly);
		else {
			lx = _scratchX;
			ly = _scratchY;
		}
	}

	if (reached) {
		const int found = findFighter(lx, ly);

		switch (_item) {
		case 0x13:
		case 53:
			place(TILE_ENERGY, 2, lx, ly);
			break;
		case 51:
			place(TILE_FIRE, 2, lx, ly);
			break;
		case 52:
			place(TILE_SLEEP_FIELD, 2, lx, ly);
			break;
		case 54:
			place(TILE_BARRIER, 2, lx, ly);
			break;
		}

		if ((!_spell || landed) && found >= 0 && found != _current) {
			_scratchX = lx;
			_scratchY = ly;
			return found;
		}
	}

	_scratchX = lx;
	_scratchY = ly;
	return -1;
}

// Follows a thing from one place to another over tiles 32 to a row. Gives false if something is in the way
bool CombatLogic::traceProjectile(int fromX, int fromY, int toX, int toY, int kind, const byte *tiles, int &lastX, int &lastY) {
	byte px[PATH_BUFFER];
	byte py[PATH_BUFFER];
	memset(px, 0xFF, sizeof(px));
	memset(py, 0xFF, sizeof(py));

	// The path from pixel to pixel, as the original draws it
	int x1 = fromX * 0x10 + 0x10;
	int y1 = fromY * 0x10 + 0x10;
	const int x2 = toX * 0x10 + 0x10;
	const int y2 = toY * 0x10 + 0x10;

	{
		int stepX = 1;
		int stepY = 1;
		int limit = PATH_MAX_POINTS;
		int slope;

		if (x2 != x1)
			slope = ((y2 - y1) * 100) / (x2 - x1);
		else
			slope = 0x4B00;

		if (slope < 0)
			slope = -slope;
		if (x2 < x1)
			stepX = -1;
		if (y2 < y1)
			stepY = -1;

		int remainX = x2 - x1;
		if (remainX < 0)
			remainX = -remainX;

		int remainY = y2 - y1;
		if (remainY < 0)
			remainY = -remainY;

		int error = slope;
		byte *outX = px;
		byte *outY = py;
		int cx = x1;
		int cy = y1;

		*outX++ = cx;
		*outY++ = cy;

		while (remainX > 0 || remainY > 0) {
			while (error > 0 && remainY > 0) {
				error -= 100;
				cy += stepY;

				*outY++ = cy;
				--remainY;

				*outX++ = cx;
				--limit;

				if (limit == 0)
					break;
			}

			if (limit == 0)
				break;

			--remainX;
			error += slope;

			cx += stepX;

			*outY = cy;
			*outX = cx;

			++outY;
			++outX;
			--limit;
		}

		*outX = 0xFF;
		*outY = 0xFF;
	}

	int step = 0xD;
	if (kind == 7)
		step = 8;

	int pos = 0;
	int phase = 0;
	if (kind == 0)
		pos = 4;

	while (px[pos] != 0xFF) {
		phase = (phase & 3) + 1;

		// Where in the map that is, or off it
		if (px[pos] < 8 || px[pos] > 0xB7 || py[pos] < 8 || py[pos] > 0xB7) {
			lastX = lastY = -1;
			break;
		}

		lastX = (px[pos] - 8) / 0x10;
		lastY = (py[pos] - 8) / 0x10;

		// Fire, energy and the like are made of sparks, which are random
		if (kind >= 3 && kind <= 6) {
			for (int i = 0; i < SPARK_STEPS; ++i) {
				rnd(0, 0xF);
				rnd(0, 0xF);
				rnd(0, 1);
				rnd(0, 1);
			}
		}

		pos += step;

		if (pos > PATH_LIMIT)
			break;

		if (px[pos] == 0xFF)
			break;

		const int ground = tiles[lastY * COMBAT_MAP_STRIDE + lastX];
		if (((0x80 >> (ground & 7)) & PROJECTILE_PASS[ground >> 3]) == 0 && (fromX != lastX || fromY != lastY))
			return false;
	}

	return true;
}

// Follows a thing from one place to another on the map of the fight
bool CombatLogic::pathClear(int fromX, int fromY, int toX, int toY, int kind) {
	int lastX = _scratchX;
	int lastY = _scratchY;
	const bool clear = traceProjectile(fromX, fromY, toX, toY, kind, _map, lastX, lastY);

	_scratchX = lastX;
	_scratchY = lastY;
	return clear;
}

// Changes the map where the party has trodden or a thing has landed, in the rooms that do that
bool CombatLogic::triggers(int x, int y) {
	bool done = false;

	if (!(_flags & 0x82))
		return false;

	for (int i = 0; i < 8; ++i) {
		byte &triggerX = _map[8 * COMBAT_MAP_STRIDE + 11 + i];
		byte &triggerY = _map[8 * COMBAT_MAP_STRIDE + 19 + i];
		const byte newTile = _map[0 * COMBAT_MAP_STRIDE + 11 + i];
		const byte x1 = _map[9 * COMBAT_MAP_STRIDE + 11 + i];
		const byte y1 = _map[9 * COMBAT_MAP_STRIDE + 19 + i];
		const byte x2 = _map[10 * COMBAT_MAP_STRIDE + 11 + i];
		const byte y2 = _map[10 * COMBAT_MAP_STRIDE + 19 + i];

		if (triggerX == x && triggerY == y) {
			triggerX = triggerY = 0xFF;

			if (x1 < 0xB && y1 < 0xB)
				_map[y1 * COMBAT_MAP_STRIDE + x1] = newTile;
			if (x2 < 0xB && y2 < 0xB)
				_map[y2 * COMBAT_MAP_STRIDE + x2] = newTile;

			done = true;
		}
	}

	return done;
}

// A melee blow of one fighter to another
void CombatLogic::meleeBlow(int attacker, int target, int item) {
	if (hits(target, attacker, item)) {
		strike(target, attacker);
		report(target, attacker);
	} else {
		missed(target);
	}
}

// Uses up ammunition, or the weapon itself if it's thrown
void CombatLogic::useAmmunition(int item, int range) {
	Savegame &sg = save();
	if (_spell)
		return;

	switch (item) {
	case 0x1A:
	case 0x24:
		if (--sg._equipment[ITEM_ARROWS] == 0)
			sg._equipment[item] += unequipAll(item);
		break;

	case 0x1C:
		if (--sg._equipment[ITEM_QUARRELS] == 0)
			sg._equipment[item] += unequipAll(item);
		break;

	case 0x10:
	case 0x15:
	case 0x16:
		if (range <= 1)
			break;

		if (sg._equipment[item] != 0)
			--sg._equipment[item];
		else
			unequip(_entities[_current]._entity, item);
		break;
	}
}

// What an enemy does about the fighter it's after: an attack, a shot or nothing. Gives true if it did something
int CombatLogic::monsterAttack(int idx) {
	Entity &e = _entities[idx];
	const int kind = e._entity;
	const int actor = e._actor;
	Savegame &sg = save();

	const int target = pickTarget(idx);
	if (target < 0)
		return 0;

	const int targetKind = _entities[target]._entity;

	if (e._flags & FLAG_CHARMED) {
		if (distance(idx, target) == 1) {
			_item = 0x21;
			newline();
			if (hits(target, idx, 0x21)) {
				strike(target, idx);
				report(target, idx);
			} else {
				missed(target);
			}
			return 1;
		}

		return 0;
	}

	bool protect = false;
	if ((_entities[target]._flags & FLAG_PLAYER) && sg._party[targetKind]._equipment[5] == ITEM_TURNING_AMULET &&
			(ENEMY_FLAGS[kind] & ENEMY_FLAG_MAGIC))
		protect = rnd(0, 0xFF) < 0x80;

	if (distance(idx, target) <= ENEMY_RANGE[kind]) {
		if (kind == ENEMY_MIMIC)
			_actors[actor]._unk6 = 0x20;

		if (distance(idx, target) == 1) {
			if (kind == ENEMY_SEA_SERPENT)
				_actors[actor]._unk6 = 0x20;

			_interferer[target] = idx;
			if (!hits(target, idx, 0))
				return 1;

			if ((ENEMY_FLAGS[kind] & ENEMY_FLAG_STEAL) && rnd(0, 3) != 0 && sg._food != 0) {
				say("\nA ");
				sayName(idx);
				say(" stole some food!\n");
				sg._food = sg._food > 5 ? sg._food - 5 : 0;
				return 1;
			}

			newline();
			strike(target, idx);
			report(target, idx);
			return 1;
		}

		return monsterShoot(idx, target, protect);
	}

	return 0;
}

// An enemy shoots or casts at a fighter some way off
int CombatLogic::monsterShoot(int idx, int target, bool protect) {
	const int kind = _entities[idx]._entity;
	const int shot = ENEMY_SHOT[kind];

	if ((kind != ENEMY_MIMIC && rnd(0, 0xFF) >= 0x80) ||
			((ENEMY_FLAGS[kind] & ENEMY_FLAG_MAGIC) && (save()._icon == ICON_NEGATE || save()._icon == ICON_SHADOW)))
		return 0;

	bool landed = false;
	if (!protect)
		landed = hits(target, idx, 0);

	const int hit = project(idx, _entities[target]._x, _entities[target]._y, landed, shot);
	if (hit >= 0) {
		newline();
		strike(hit, idx);
		report(hit, idx);
	}

	return 1;
}

void CombatLogic::regurgitate(int idx) {
	if (quickRoll() < _entities[idx]._dex) {
		sayName(idx);
		say(" regurgitated!\n");

		_entities[idx]._flags &= ~FLAG_SWALLOWED;
		_actors[_entities[idx]._actor]._animTile = _actors[_entities[idx]._actor]._tile;
	}
}

// A turn of an enemy, or of a charmed member of the party
void CombatLogic::monsterTurn() {
	Entity &e = _entities[_current];
	const int actor = e._actor;
	Savegame &sg = save();

	_item = 0;

	if (sg._icon != ICON_TIME_STOP && (sg._icon != ICON_QUICK || rnd(0, 1) != 0) && (e._flags & FLAG_SWALLOWED) == 0) {
		if (e._flags & FLAG_ASLEEP) {
			if (rnd(0, 0x10) == 0x10)
				wakeUp(_current);
		} else {
			if (e._entity == ENEMY_SEA_SERPENT)
				_actors[actor]._unk6 = 0;

			if (e._flags & FLAG_FLEEING) {
				if ((e._flags & FLAG_PLAYER) == 0 && rnd(0, 3) == 3)
					++e._hp;

				woundLevel(_current);
			} else {
				if (monsterSpecial(_current))
					return;

				if (monsterAttack(_current))
					return;
			}

			if (monsterMove(_current)) {
				if (_outcome & OUTCOME_OFF_MAP) {
					newline();
					sayName(_current);
					say(" escapes!\n");
					removeFighter(-_current - 1);

					if ((e._flags & FLAG_PLAYER) == 0 && e._entity == ENEMY_SHADOWLORD)
						uncharm();
				}
			} else if (e._flags & FLAG_FLEEING) {
				monsterAttack(_current);
			}
		}
	}
}

// What the ground and the fields do to a fighter at the end of its turn
void CombatLogic::hazards(int idx) {
	Entity &e = _entities[idx];
	int kind = 0;
	const int ground = _map[e._y * COMBAT_MAP_STRIDE + e._x];

	if (ground == MAP_FIRE_A || ground == MAP_FIRE_B)
		kind = 100;
	if (ground == MAP_SWAMP)
		kind = 50;

	if (kind == 0) {
		for (int i = 0; i < COMBAT_ENTITIES; ++i) {
			if (i != e._actor && e._x == _actors[i]._x && e._y == _actors[i]._y) {
				if (_actors[i]._tile == TILE_ENERGY)
					kind = 100;
				if (_actors[i]._tile == TILE_FIRE)
					kind = 50;
				if (_actors[i]._tile == TILE_SLEEP_FIELD)
					kind = 150;

				if (kind != 0)
					break;
			}
		}
	}

	switch (kind) {
	case 150:
		fallAsleep(idx);
		break;

	case 100:
		takeDamage(idx, rnd0(10));
		report(idx, 0xFF);
		break;

	case 50:
		if (_actors[e._actor]._tile < 0x80)
			poison(idx, -1);
		break;
	}
}

// The things that happen as a fighter finishes a turn
void CombatLogic::upkeep() {
	hideIfInvisible(_current);
	burnOut();

	Savegame &sg = save();
	if (sg._protection != 0 && sg._protection != 0xFF) {
		if (--sg._protection == 0)
			sg._icon = 0;
	}
}

// Fires and fields go out now and then
void CombatLogic::burnOut() {
	for (int i = 0; i < COMBAT_ENTITIES; ++i) {
		if ((_actors[i]._tile & 0xFC) == TILE_FIRE) {
			if (rnd(0, 0xFF) < 0x10)
				removeFighter(i + 1);
		}
	}
}

// Works out the line that says what's in a member of the party's hands, and how many weapons that is
int CombatLogic::armedWith(int item, bool first, Common::String &list) {
	if (item != 0xFF && WEAPON_DAMAGE[item] != 0) {
		if (!first)
			list += ", ";

		list += EQUIPMENT_NAMES[item];
		return 1;
	}

	return 0;
}

bool CombatLogic::prompt(bool header) {
	const Entity &e = _entities[_current];

	if (header) {
		newline();
		sayName(_current);

		Common::String list;
		_armed = 0;

		if (e._flags & FLAG_PLAYER) {
			say(", armed with ");
			const Character &c = save()._party[e._entity];

			_armed = armedWith(c._equipment[0], true, list);
			_armed += armedWith(c._equipment[2], _armed == 0, list);
			_armed += armedWith(c._equipment[3], _armed == 0, list);

			if (_armed == 0)
				list += "bare hands";
		}

		list += ":";
		say(list.c_str());
	}

	newline();

	if (e._flags & FLAG_SWALLOWED) {
		say("ARGH!\n");
		regurgitate(_current);
		endTurn(0xFF);
		return false;
	}

	if (e._flags & FLAG_ASLEEP) {
		if (rnd(0, 0xFF) < 0x10)
			wakeUp(_current);

		say("Zzzzz...\n");
		endTurn(0xFF);
		return false;
	}

	_stage = STAGE_COMMAND;
	return true;
}

// Begins the turn of a fighter that isn't an enemy. Gives true if the player is to be asked what to do
bool CombatLogic::beginTurn() {
	Entity &e = _entities[_current];
	Savegame &sg = save();

	if (sg._activeChar != NO_ACTIVE_CHARACTER && ((e._flags & FLAG_PLAYER) == 0 || sg._activeChar != e._entity)) {
		upkeep();
		_stage = STAGE_AFTER;
		return false;
	}

	if ((e._flags & FLAG_PLAYER) && (sg._party[e._entity]._equipment[2] == ITEM_CHARM_RING || sg._party[e._entity]._equipment[3] == ITEM_CHARM_RING)) {
		e._flags |= FLAG_CHARMED;
		sg._activeChar = NO_ACTIVE_CHARACTER;
		monsterTurn();
		endTurn(0xFF);
		return false;
	}

	return prompt(true);
}

// Finishes the turn of a member of the party
void CombatLogic::endTurn(int key) {
	if (key < '0' || key > '6')
		upkeep();

	_stage = STAGE_AFTER;
}

CombatLogic::State CombatLogic::advance() {
	Savegame &sg = save();

	for (;;) {
		if (_stage == STAGE_COMMAND)
			return STATE_COMMAND;

		if (_stage == STAGE_DONE)
			return STATE_FINISHED;

		if (_stage == STAGE_AFTER) {
			_interferer[_current] = 0xFF;
			hazards(_current);

			int enemies, party;
			countSides(enemies, party);

			if (party == 0) {
				if (enemies == 0) {
					_result = 0;
					_stage = STAGE_DONE;
					return STATE_FINISHED;
				}

				if (uncharm() == -1) {
					if (!_won) {
						say("\nBATTLE IS LOST!");
						_result = 1;
					}

					_stage = STAGE_DONE;
					return STATE_FINISHED;
				}
			} else if (enemies == 0 && !_won) {
				say("\nVICTORY!\n");
				_won = true;
			}

			_stage = STAGE_SCAN;
			++_current;
		}

		if (_current >= COMBAT_ENTITIES)
			_current = 0;

		Entity &e = _entities[_current];
		const byte flags = e._flags;

		if ((flags & (FLAG_PLAYER | FLAG_MONSTER)) && (flags & FLAG_DEAD) == 0) {
			if ((flags & FLAG_PLAYER) && sg._party[e._entity]._status == STATUS_DEAD) {
				e._flags |= FLAG_DEAD;
				takeDamage(_current, 99);
			} else if ((_map[e._y * COMBAT_MAP_STRIDE + e._x] & 0xFE) != 0x84) {
				if (--e._timer == 0) {
					e._timer = 36 - e._dex;
					_item = 0;
					_special = _spell = false;
					_outcome = 0;

					if (++sg._unk02DC == 10) {
						sg._unk02DC = 0;
						TimeLogic::advance(sg, 1);
					}

					if (isEnemySide(_current)) {
						monsterTurn();
						_stage = STAGE_AFTER;
					} else if (beginTurn()) {
						return STATE_COMMAND;
					}

					continue;
				}
			}
		}

		++_current;
	}
}

void CombatLogic::finish() {
	Savegame &sg = save();

	if (_savedActive < sg._partySize && sg._party[_savedActive]._status != STATUS_DEAD && sg._party[_savedActive]._status != STATUS_SLEEP)
		sg._activeChar = _savedActive;
	else
		sg._activeChar = NO_ACTIVE_CHARACTER;
}

bool CombatLogic::setActive(int who) {
	bool ok = false;
	int i;

	say("Set active plr:\n");

	for (i = 0; i < COMBAT_ENTITIES; ++i) {
		const byte flags = _entities[i]._flags;

		if ((flags & FLAG_PLAYER) && !isEnemySide(i) && _entities[i]._entity == who) {
			ok = (flags & (FLAG_DEAD | FLAG_ASLEEP | FLAG_SWALLOWED)) == 0;
			break;
		}
	}

	if (ok) {
		save()._activeChar = who;
		sayName(i);
		newline();
	} else {
		say("Invalid!\n");
	}

	return ok;
}

// Leaves the fight by an edge of the map
bool CombatLogic::leaveBattle(int idx, int code) {
	Savegame &sg = save();

	if ((sg._partyTile & 0xF8) == TILE_SHIP_FIRST) {
		say("\nStay with ship!\n");
		return false;
	}

	if (_entities[idx]._flags & FLAG_PLAYER) {
		if (_exitCode == 0) {
			_exitCode = code;
		} else if (code != _exitCode && (_flags & 0x80)) {
			say("\nAll must use the same exit!\n");
			return false;
		}
	}

	int enemies, party;
	countSides(enemies, party);
	say(enemies == 0 ? "Leave!\n" : "Escape!\n");

	sg._activeChar = NO_ACTIVE_CHARACTER;
	removeFighter(-idx - 1);
	return true;
}

CombatLogic::Outcome CombatLogic::movePlayer(int key) {
	Entity &e = _entities[_current];
	int dx = 0;
	int dy = 0;

	switch (key) {
	case KEY_UP:
		dy = -1;
		say("North\n");
		break;
	case KEY_DOWN:
		dy = 1;
		say("South\n");
		break;
	case KEY_RIGHT:
		dx = 1;
		say("East\n");
		break;
	case KEY_LEFT:
		dx = -1;
		say("West\n");
		break;
	}

	const int x = e._x + dx;
	const int y = e._y + dy;

	if (x > 10 || y > 10 || x < 0 || y < 0)
		return leaveBattle(_current, key) ? OUTCOME_DONE : OUTCOME_INVALID;

	if (canStand(_actors[e._actor]._tile, x, y)) {
		_actors[e._actor]._x = e._x = x;
		_actors[e._actor]._y = e._y = y;

		if (_flags & 0x82)
			triggers(x, y);

		return OUTCOME_DONE;
	}

	say("Blocked!\n");
	return OUTCOME_INVALID;
}

void CombatLogic::aimStart(int attacker, int range) {
	int target = _actors[_entities[attacker]._actor]._skiffs;

	_aimRange = range;
	_aimDistance = 0;

	if (target > 0x1F || (_entities[target]._flags & (FLAG_DEAD | FLAG_INVISIBLE)) != 0 || _entities[target]._flags == 0 ||
			_actors[_entities[target]._actor]._tile == 0 || distance(attacker, target) > range)
		target = attacker;

	_aimX = _entities[target]._x;
	_aimY = _entities[target]._y;
}

void CombatLogic::aimMove(int dx, int dy) {
	const int x = _aimX + dx;
	const int y = _aimY + dy;
	const Entity &e = _entities[_current];

	if (distancePoints(x, y, e._x, e._y) <= _aimRange && x >= 0 && x < COMBAT_SIZE && y >= 0 && y < COMBAT_SIZE) {
		_aimX = x;
		_aimY = y;
	}
}

bool CombatLogic::aimOnSelf() const {
	return _aimX == _entities[_current]._x && _aimY == _entities[_current]._y;
}

bool CombatLogic::aimAccept() {
	if (aimOnSelf())
		return false;

	newline();
	_aimDistance = distancePoints(_aimX, _aimY, _entities[_current]._x, _entities[_current]._y);
	return true;
}

CombatLogic::Outcome CombatLogic::aimCancel() {
	newline();
	_aimDistance = 0;
	return aimFire();
}

// Starts the attack of a member of the party with what's in their hands
CombatLogic::Outcome CombatLogic::attack() {
	const Entity &e = _entities[_current];

	_queueSize = 0;
	_queuePos = 0;

	if ((e._flags & FLAG_PLAYER) && _armed > 1)
		say(" ");

	if (!(e._flags & FLAG_PLAYER) || _armed == 0) {
		_queue[_queueSize++] = ITEM_BARE_HANDS;
	} else {
		const Character &c = save()._party[e._entity];
		const byte slots[3] = { c._equipment[0], c._equipment[2], c._equipment[3] };

		for (int i = 0; i < 3; ++i) {
			if (slots[i] != 0xFF && WEAPON_DAMAGE[slots[i]] != 0)
				_queue[_queueSize++] = slots[i];
		}
	}

	return nextWeapon();
}

// Carries on to the next weapon, which needs a target. Ends the turn when there are no more
CombatLogic::Outcome CombatLogic::nextWeapon() {
	Savegame &sg = save();
	const Entity &e = _entities[_current];

	while (_queuePos < _queueSize) {
		const int item = _queue[_queuePos++];

		if (_armed > 1) {
			_special = _spell = false;
			newline();
			say(EQUIPMENT_NAMES[item]);
			say(":\n");
		}

		_item = item;
		say("Attack-");

		if (item >= ITEM_SPECIAL_FIRST)
			_special = true;

		int range;
		if ((e._flags & FLAG_PLAYER) == 0) {
			range = ENEMY_RANGE[e._entity];
			if (range == 1)
				range = 0;
		} else {
			range = item != ITEM_BARE_HANDS ? WEAPON_RANGE[item] : 0;
		}

		_queueItem = item;
		if (range == 0) {
			say("Aim! ");
			aimStart(_current, 1);
			return OUTCOME_AIM;
		}

		// Some weapons can't be used with an enemy next to the one using them
		if (item == 0x1A || item == 0x1C || item == 0x24 || item == ITEM_BOOMERANG || item == 0x11) {
			const int u = _interferer[_current];

			if (u != 0xFF && _entities[u]._flags != 0 && isEnemySide(u) && (_entities[u]._flags & (FLAG_ASLEEP | FLAG_SWALLOWED)) == 0 &&
					sg._icon != ICON_TIME_STOP && distance(_current, u) == 1) {
				newline();
				sayName(u);
				say(" interferes!\n");
				continue;
			}
		}

		say("Aim! ");
		aimStart(_current, range);
		return OUTCOME_AIM;
	}

	endTurn('A');
	return OUTCOME_DONE;
}

// Carries out an attack at what's aimed at, then goes on to the next weapon
CombatLogic::Outcome CombatLogic::aimFire() {
	const int item = _queueItem;
	const Entity &e = _entities[_current];
	const int attacker = _current;
	const bool melee = item == ITEM_BARE_HANDS || WEAPON_RANGE[item] == 0;

	if (melee) {
		const int target = findFighter(_aimX, _aimY);

		if (_aimDistance == 0 || target == -1) {
			say("Nothing!\n");
		} else {
			_actors[e._actor]._skiffs = target;

			newline();
			meleeBlow(attacker, target, item);
		}
	} else if (_aimDistance != 0) {
		Savegame &sg = save();
		const int kind = (e._flags & FLAG_PLAYER) ? WEAPON_KIND[item] : ENEMY_SHOT[e._entity];

		if (item == ITEM_BOOMERANG) {
			if (sg._equipment[item] != 0)
				--sg._equipment[item];
			else
				unequip(e._entity, item);
		}

		ActorRecord &me = _actors[e._actor];
		me._skiffs = 0xFF;

		const int found = findFighter(_aimX, _aimY);
		int hit;

		if (found > -1) {
			me._skiffs = found;
			useAmmunition(item, _aimDistance);
			const bool landed = hits(found, attacker, item);
			hit = project(attacker, _aimX, _aimY, landed, kind);
		} else {
			hit = project(attacker, _aimX, _aimY, true, kind);
		}

		const int lx = _scratchX;
		const int ly = _scratchY;

		if (hit > -1) {
			newline();
			strike(hit, attacker);
			report(hit, attacker);
		} else if (found > -1) {
			newline();
			missed(found);
		}

		if (item == 0x26)
			pathClear(lx, ly, _entities[attacker]._x, _entities[attacker]._y, kind);
	}

	return nextWeapon();
}

CombatLogic::Outcome CombatLogic::command(int key) {
	Outcome outcome = OUTCOME_DONE;

	switch (key) {
	case 'A':
		return attack();

	case 'K':
		return klimb();

	case 'R':
		say("Ready...\n\n");
		return OUTCOME_SUBCOMMAND;

	case 'U':
		say("Use item\n\n");
		return OUTCOME_SUBCOMMAND;

	case 'Z':
		say("Z-stats...\n");
		return OUTCOME_SUBCOMMAND;

	case 27: {
		// Escape: everyone goes, if they have won
		say("Escape");

		bool partyLeft = false;
		for (int i = 0; i < COMBAT_ENTITIES; ++i) {
			if ((_entities[i]._flags & (FLAG_PLAYER | FLAG_DEAD)) == FLAG_PLAYER) {
				partyLeft = true;
				break;
			}
		}

		bool refused = false;
		if (partyLeft) {
			if (_flags & 0x80) {
				say("-Not here!\n");
				refused = true;
			} else if (!_won) {
				say("-Not yet!\n");
				refused = true;
			}
		}

		if (refused) {
			prompt(true);
			return OUTCOME_AGAIN;
		}

		_log.printChar('!');
		for (int i = 0; i < COMBAT_ENTITIES; ++i) {
			if (_entities[i]._flags != 0)
				removeFighter(-i - 1);
		}

		for (int i = 0; i < COMBAT_ENTITIES; ++i) {
			if (_actors[i]._tile != 0)
				removeFighter(i + 1);
		}
		break;
	}

	case ' ':
		say("Pass\n");
		break;

	case '0':
		save()._activeChar = NO_ACTIVE_CHARACTER;
		say("Set active plr:\nNone!\n");
		break;

	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
		if (!setActive(key - '1')) {
			prompt(true);
			return OUTCOME_AGAIN;
		}
		break;

	case KEY_LEFT:
	case KEY_RIGHT:
	case KEY_UP:
	case KEY_DOWN:
		outcome = movePlayer(key);
		if (outcome == OUTCOME_INVALID)
			newline();
		break;

	case 'B':
	case 'X':
		say(key == 'B' ? "Board" : "X-it");
		say(" what?");
		newline();
		prompt(true);
		return OUTCOME_AGAIN;

	case 'E':
	case 'F':
	case 'H':
	case 'I':
	case 'L':
	case 'M':
	case 'N':
	case 'Q':
	case 'V':
		say(key == 'E' ? "Enter" : key == 'F' ? "Fire" : key == 'H' ? "Hole up" : key == 'I' ? "Ignite torch" :
			key == 'L' ? "Look" : key == 'M' ? "Mix" : key == 'N' ? "New order" : key == 'Q' ? "Quit" : "View");
		say("-Not here");
		newline();
		prompt(true);
		return OUTCOME_AGAIN;

	case 'T':
		say("Talk-Funny, no response!");
		newline();
		prompt(true);
		return OUTCOME_AGAIN;

	case 'D':
	case 'W':
		say(key == 'D' ? "D-What?\n" : "W-What?\n");
		prompt(true);
		return OUTCOME_AGAIN;

	default:
		say("What?\n");
		newline();
		return OUTCOME_INVALID;
	}

	if (outcome == OUTCOME_DONE)
		endTurn(key);

	return outcome;
}

void CombatLogic::commandDone(int key) {
	endTurn(key);
}

constexpr byte MAP_LADDER_UP = 0xC8;
constexpr byte MAP_LADDER_DOWN = 0xC9;
constexpr byte MAP_ROOM_EXIT = 0x86;
constexpr int ROOM_FLAG = 2;

// The tile of the last object at a place, which is zero if there's none
int CombatLogic::objectTileAt(int x, int y) const {
	for (int i = COMBAT_ENTITIES - 1; i >= 0; --i) {
		if (_actors[i]._x == x && _actors[i]._y == y)
			return _actors[i]._tile;
	}

	return 0;
}

// Climbing in a room of a dungeon, which goes up or down a ladder
CombatLogic::Outcome CombatLogic::klimb() {
	const Entity &e = _entities[_current];
	const byte tile = _map[e._y * COMBAT_MAP_STRIDE + e._x];

	say("Klimb-");

	if ((_flags & ROOM_FLAG) && tile == MAP_LADDER_UP && _bothLadders) {
		say("U/D-");
		return OUTCOME_CHOICE;
	}

	return klimbAt(tile);
}

CombatLogic::Outcome CombatLogic::klimbChosen(bool down) {
	return klimbAt(down ? MAP_LADDER_DOWN : MAP_LADDER_UP);
}

CombatLogic::Outcome CombatLogic::klimbAt(int tile) {
	if (tile == MAP_LADDER_UP) {
		say("Up!\n");

		if (leaveBattle(_current, EXIT_UP)) {
			endTurn('K');
			return OUTCOME_DONE;
		}

		newline();
		return OUTCOME_INVALID;
	}

	if ((tile == MAP_ROOM_EXIT && (_flags & 0x80)) || tile == MAP_LADDER_DOWN) {
		say("Down!\n");

		if (leaveBattle(_current, EXIT_DOWN)) {
			endTurn('K');
			return OUTCOME_DONE;
		}

		newline();
		return OUTCOME_INVALID;
	}

	return OUTCOME_DIRECTION;
}

CombatLogic::Outcome CombatLogic::klimbDirection(int dx, int dy, bool cancelled) {
	Entity &e = _entities[_current];

	if (!cancelled) {
		const int x = e._x + dx;
		const int y = e._y + dy;

		if (inside(x, y) && _map[y * COMBAT_MAP_STRIDE + x] == MAP_GRAVE && objectTileAt(x, y) == 0) {
			_actors[e._actor]._x = e._x = x;
			_actors[e._actor]._y = e._y = y;
			triggers(x, y);
		} else {
			say("What?\n");
			newline();
			return OUTCOME_INVALID;
		}
	}

	endTurn('K');
	return OUTCOME_DONE;
}

constexpr int DUNGEON_MONSTER_ROW = 5;
constexpr int DUNGEON_MONSTER_X_ROW = 6;
constexpr int DUNGEON_MONSTER_Y_ROW = 7;
constexpr int DUNGEON_SPECIAL_COLUMN = 11;
constexpr int DUNGEON_SPECIAL_Y = 6;
constexpr int DUNGEON_KIND_CHEST = 1;
constexpr int DUNGEON_KIND_GOLD = 2;
constexpr int DUNGEON_KIND_OTHER_LIMIT = 0x10;
constexpr int DUNGEON_RANDOM_FIRST = 0xEC;

void CombatLogic::beginDungeon(byte origin, int flags, int facing, int cell, int level, bool withEnemies, bool ladders) {
	Savegame &sg = save();

	_origin = origin;
	_flags = flags;
	_exitCode = 0;
	_won = false;
	_result = 1;
	_current = 0xFF;
	_outcome = 0;
	_bothLadders = ladders;
	_savedActive = sg._activeChar;
	sg._activeChar = NO_ACTIVE_CHARACTER;

	// Where the party starts depends on the way it came in
	int row = 4;
	switch (facing) {
	case 0:
	case 5:
		row = 3;
		break;
	case 1:
		row = 2;
		break;
	case 3:
		row = 1;
		break;
	}

	for (int i = 0; i < 6; ++i) {
		_partyX[i] = _map[row * COMBAT_MAP_STRIDE + DUNGEON_SPECIAL_COLUMN + i];
		_partyY[i] = _map[row * COMBAT_MAP_STRIDE + DUNGEON_SPECIAL_COLUMN + DUNGEON_SPECIAL_Y + i];
	}

	placeParty();

	for (int i = 0; i < sg._partySize; ++i) {
		_entities[i]._x = _actors[i]._x = _partyX[i];
		_entities[i]._y = _actors[i]._y = _partyY[i];
	}

	if (withEnemies) {
		byte roll[4];
		for (int i = 0; i < 4; ++i)
			roll[i] = ROOM_ENEMY_KIND[rnd(0, 7)];

		for (int i = 0; i < 16; ++i) {
			const byte tile = _map[DUNGEON_MONSTER_ROW * COMBAT_MAP_STRIDE + DUNGEON_SPECIAL_COLUMN + i];
			if (tile == 0)
				continue;

			int type;
			int kind;
			if (tile < 0x40 || (tile & 0xFC) == 0xB4 || (tile & 0xFC) == 0xE8) {
				type = 2;
				kind = tile;
			} else {
				type = 0;
				kind = (tile - 0x40) / 4;
			}

			if ((tile & 0xFC) == DUNGEON_RANDOM_FIRST)
				kind = roll[tile & 3];

			const int placed = place(kind, type, _map[DUNGEON_MONSTER_X_ROW * COMBAT_MAP_STRIDE + DUNGEON_SPECIAL_COLUMN + i],
				_map[DUNGEON_MONSTER_Y_ROW * COMBAT_MAP_STRIDE + DUNGEON_SPECIAL_COLUMN + i]);

			if (type == 2 && placed >= 0) {
				if (kind == DUNGEON_KIND_CHEST) {
					_actors[placed]._hull = level * 3 + 7;
				} else if (kind == DUNGEON_KIND_GOLD) {
					_actors[placed]._hull = rnd(1, level * 10 + 10);
				} else if (kind < DUNGEON_KIND_OTHER_LIMIT && kind >= 3) {
					_actors[placed]._hull = TREASURE_BASE[kind - 3] + rnd(0, TREASURE_RANGE[kind - 3] - 1);
				}
			}
		}
	}

	int enemies, party;
	countSides(enemies, party);
	_won = enemies == 0;
	_stage = STAGE_SCAN;
	_current = 0;
}

// The kinds of enemy that can come on the party as it sleeps
static const byte AMBUSHERS[8] = { 0x29, 0x14, 0x15, 0x18, 0x16, 0x19, 0x24, 0x14 };

constexpr int CAMP_STEP_MINUTES = 5;
constexpr int CAMP_AMBUSH_ODDS = 0x3F;
constexpr int CAMP_REST_HOURS = 5;
constexpr int CAMP_REST_DELAY = 0xE;
constexpr int CAMP_APPARITION_ODDS = 0x18;
constexpr int CAMP_HEAL_MAX = 0x3F;

CombatLogic::CampResult CombatLogic::camp(int guard, int hours, byte origin, bool &apparition) {
	Savegame &sg = save();
	int poisoned[8] = {};
	int lastHour = sg._hour;

	apparition = false;
	_origin = origin;
	_flags = KIND_CAMP;
	_exitCode = 0;
	_won = false;
	_result = 1;
	_current = 0xFF;
	_outcome = 0;

	sg._icon = 0;
	sg._protection = 0;
	_savedActive = sg._activeChar;
	sg._activeChar = NO_ACTIVE_CHARACTER;

	placeParty();

	int target = hours + sg._hour;
	if (target > 0x17)
		target -= 0x18;

	if (guard > 5 || guard < -1)
		guard = -1;

	int slot = 0;
	for (int member = 0; member < sg._partySize; ++member) {
		for (slot = 0; slot < COMBAT_ENTITIES; ++slot) {
			if (member == _entities[slot]._entity)
				break;
		}

		switch (sg._party[member]._status) {
		case STATUS_POISONED:
			++poisoned[member];
			// Fall through

		case STATUS_GOOD:
		case STATUS_SLEEP:
			// Those that are poisoned stay awake
			if (member != guard && slot < COMBAT_ENTITIES &&
					((_entities[slot]._flags & FLAG_PLAYER) == 0 || sg._party[_entities[slot]._entity]._status != STATUS_POISONED))
				fallAsleep(slot);
			break;
		}
	}

	say("Zzzzzz...\n\n");

	int guardSlot = 0;
	byte guardTile = 0;
	if (guard != -1) {
		for (guardSlot = 0; guardSlot < COMBAT_ENTITIES; ++guardSlot) {
			if (_entities[guardSlot]._entity == guard)
				break;
		}

		guardSlot = MIN(guardSlot, COMBAT_ENTITIES - 1);
		guardTile = _actors[_entities[guardSlot]._actor]._tile;
	}

	while (target != sg._hour) {
		if (sg._hour >= 0x18)
			sg._hour -= 0x18;

		regenerate();

		if (lastHour != sg._hour) {
			if (rnd(0, CAMP_AMBUSH_ODDS) == 0) {
				// Whatever comes on the party comes at a time that can't be told beforehand
				_G(gameRandom).seed(g_system ? g_system->getMillis() : 0);
				const int kind = AMBUSHERS[rnd(0, 7)];
				say("Ambushed!\n\n");

				if (guard > -1) {
					for (int i = 0; i < 6; ++i) {
						const int member = _entities[i]._entity;

						if (member < sg._partySize) {
							wakeUp(i);
							sg._party[member]._status = poisoned[member] != 0 ? STATUS_POISONED : STATUS_GOOD;
						}
					}
				}

				placeEnemies(kind, true, true);

				int enemies, party;
				countSides(enemies, party);
				_won = enemies == 0;
				_stage = STAGE_SCAN;
				_current = 0;
				return CAMP_AMBUSHED;
			}
		}

		lastHour = sg._hour;
		TimeLogic::advance(sg, CAMP_STEP_MINUTES);

		if (guard != -1 && rnd(0, 3) == 2) {
			Entity &g = _entities[guardSlot];
			int x = g._x;
			int y = g._y;

			switch (rnd(0, 3)) {
			case 0:
				--y;
				break;
			case 1:
				++y;
				break;
			case 2:
				++x;
				break;
			default:
				--x;
				break;
			}

			if (inside(x, y) && canStand(guardTile, x, y)) {
				_actors[g._actor]._x = g._x = x;
				_actors[g._actor]._y = g._y = y;
			}
		}
	}

	CampResult result = CAMP_NO_EFFECT;

	if (sg._unk02E6 < 1 && hours > CAMP_REST_HOURS) {
		say("Party rested!\n");
		result = CAMP_RESTED;

		for (int member = 0; member < sg._partySize; ++member) {
			Character &c = sg._party[member];

			if (poisoned[member] == 0 && sg._unk02E6 < 1 && hours > CAMP_REST_HOURS && c._status != STATUS_DEAD && member != guard) {
				c._hp += rnd(1, CAMP_HEAL_MAX);
				if (c._hp > c._maxHp)
					c._hp = c._maxHp;

				switch (c._class) {
				case 'A':
				case 'M':
					c._mag = c._int;
					break;
				case 'B':
					c._mag = c._int >> 1;
					break;
				}
			}
		}

		if ((_flags & 0x82) == 0 && rnd(0, 99) <= CAMP_APPARITION_ODDS) {
			sg._unk02E7 = sg._month;
			apparition = true;
		}

		sg._unk02E6 = CAMP_REST_DELAY;
	} else {
		say("No effect...\n");
	}

	for (int member = 0; member < sg._partySize; ++member) {
		if (sg._party[member]._status == STATUS_SLEEP)
			sg._party[member]._status = STATUS_GOOD;
	}

	sg._activeChar = _savedActive;
	return result;
}

} // namespace Ultima5
} // namespace Ultima
