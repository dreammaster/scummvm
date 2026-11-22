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
#include "ultima/ultima5/logic/dungeon_logic.h"
#include "ultima/ultima5/data/combat_data.h"
#include "ultima/ultima5/data/dungeon_data.h"
#include "ultima/ultima5/data/location_tables.h"
#include "ultima/ultima5/logic/party_logic.h"
#include "ultima/ultima5/logic/transport_logic.h"
#include "ultima/ultima5/logic/world_logic.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {

constexpr int FIRST_DUNGEON_LOCATION = 0x20;
constexpr int DUNGEON_COUNT = 8;
constexpr int DOOM_LOCATION = 0x27;
constexpr int LOG_WIDTH = 16;
constexpr byte TILE_AVATAR_ON_FOOT = 0x1C;
constexpr byte VISITED = 8;
constexpr int ENTRANCE_START = 1;
constexpr int BOTTOM_START = 7;
constexpr int SLEEP_WAKE_ODDS = 3;
constexpr byte ELECTRIC_FIELD = 0x83;
constexpr int KEY_LEFT = 1;
constexpr int KEY_RIGHT = 2;
constexpr int KEY_UP = 3;
constexpr int KEY_DOWN = 4;
constexpr byte UNDERWORLD_LEVEL = 0xFF;

static const int8 DX[4] = { 0, 1, 0, -1 };
static const int8 DY[4] = { -1, 0, 1, 0 };

int DungeonLogic::_facing = 0;
int DungeonLogic::_arrival = 0;

static Data::Savegame &save() {
	return _G(savegame);
}

int DungeonLogic::style() {
	const int place = save()._mapId - 0x20;

	if (place == 1 || place == 4 || place == 5)
		return 3;

	return (place == 6 || place == 7) ? 2 : 1;
}

byte DungeonLogic::cell(int x, int y, int level) {
	return save()._mapData[level * DUNGEON_LEVEL_BYTES + (y & 7) * DUNGEON_SIZE + (x & 7)];
}

byte DungeonLogic::seen(int x, int y) {
	byte c = cell(x, y, save()._mapLevel);

	if (c < 0x90)
		c &= 0xF7;

	return c;
}

bool DungeonLogic::enter(const char *kind) {
	Data::Savegame &sg = save();
	int location;

	_log.print(kind);

	for (location = FIRST_DUNGEON_LOCATION; location < FIRST_DUNGEON_LOCATION + DUNGEON_COUNT; ++location) {
		if (Data::LOCATION_ENTRANCE_X[location] == sg._mapX && Data::LOCATION_ENTRANCE_Y[location] == sg._mapY)
			break;
	}

	if (location == FIRST_DUNGEON_LOCATION + DUNGEON_COUNT) {
		_log.print("\nWhat dungeon?\n");
		return false;
	}

	if (sg._partyTile != TILE_AVATAR_ON_FOOT) {
		_log.print("\nOn foot!\n");
		return false;
	}

	if (location == DOOM_LOCATION) {
		if ((sg._unk0322[0] & sg._unk0322[1] & sg._unk0322[2]) < 0x80) {
			// The way in is guarded until the three places that change are fixed
			_log.print("\nAttacked at entrance!\n");
			return false;
		}
	} else {
		// The name is centered in the window
		Common::String line("\n\n");
		const Common::String name = Data::LOCATION_NAMES[location];
		for (uint i = 0; i < (LOG_WIDTH - name.size()) / 2; ++i)
			line += ' ';

		_log.print(line + name);
	}

	_log.print("\n");

	Common::File f;
	if (!f.open("dungeon.dat") || !f.seek((location - FIRST_DUNGEON_LOCATION) * DUNGEON_BYTES))
		return false;

	f.read(sg._mapData, DUNGEON_BYTES);

	sg.storeWorldObjects(sg._mapLevel != 0);
	sg.clearObjects();

	const bool fromBelow = sg._mapLevel != 0 && location + 1 != DOOM_LOCATION + 1;
	sg._mapId = location + 1;
	hideClearedRooms();

	if (fromBelow) {
		sg._mapLevel = BOTTOM_START;
		sg._mapX = sg._mapY = BOTTOM_START;
		_facing = 3;
		_arrival = 4;
	} else {
		sg._mapLevel = 0;
		sg._mapX = sg._mapY = ENTRANCE_START;
		_facing = 1;
		_arrival = 5;
	}

	clearMonster();
	spawnMonster();
	return true;
}

void DungeonLogic::electricField(int sign) {
	Data::Savegame &sg = save();

	_log.print("Ouch!\nElectric field!\n");
	g_engine->sound().noise(1, 500, 20000);

	if (sign > 0) {
		sg._mapX -= DX[_facing];
		sg._mapY -= DY[_facing];
	} else {
		sg._mapX += DX[_facing];
		sg._mapY += DY[_facing];
	}

	PartyLogic::hurtParty();
}

DungeonLogic::Move DungeonLogic::command(int key) {
	Data::Savegame &sg = save();
	const byte here = cell(sg._mapX, sg._mapY, sg._mapLevel);
	int direction = 0;

	switch (key) {
	case KEY_UP:
		_log.print("Advance\n");
		direction = 1;
		break;

	case KEY_RIGHT:
		if ((here & 0xF0) == CELL_DOORWAY)
			_log.print("Not in doorway!\n");
		else {
			_log.print("Turn right\n");
			_facing = (_facing + 1) & 3;
		}
		break;

	case KEY_DOWN:
		_log.print("Back up\n");
		direction = -1;
		break;

	case KEY_LEFT:
		if ((here & 0xF0) == CELL_DOORWAY)
			_log.print("Not in doorway!\n");
		else {
			_log.print("Turn left\n");
			_facing = (_facing + 3) & 3;
		}
		break;

	default:
		_log.print("Turn around.\n");
		_facing = (_facing + 2) & 3;
		break;
	}

	_arrival = _facing;

	if (direction == 0)
		return MOVE_TURNED;

	int x = DX[_facing] * direction + sg._mapX;
	if (x < 0)
		x = 7;
	if (x > 7)
		x = 0;

	int y = DY[_facing] * direction + sg._mapY;
	if (y < 0)
		y = 7;
	if (y > 7)
		y = 0;

	byte next = cell(x, y, sg._mapLevel);
	if (next == ELECTRIC_FIELD) {
		sg._mapX = x;
		sg._mapY = y;
		electricField(direction);
		x = sg._mapX;
		y = sg._mapY;
	}

	next &= 0xF0;
	if ((next > CELL_FIRST_WALL && next < CELL_DOORWAY) || (direction == -1 && (next == CELL_FIRST_WALL || next == CELL_ROOM)))
		_log.print("Blocked!\n");
	else {
		sg._mapX = x & 7;
		sg._mapY = y & 7;
	}

	return MOVE_WALKED;
}

void DungeonLogic::sleepSpell() {
	Data::Savegame &sg = save();

	_log.print("Sleep spell!\n");

	for (int i = 0; i < sg._partySize; ++i) {
		if (sg._party[i]._dex <= _G(gameRandom).range(1, 0x1E) && sg._party[i]._status != Data::STATUS_DEAD)
			sg._party[i]._status = Data::STATUS_SLEEP;
	}

	sg._mapData[sg._mapLevel * DUNGEON_LEVEL_BYTES + sg._mapY * DUNGEON_SIZE + sg._mapX] &= VISITED;
}

void DungeonLogic::poison() {
	Data::Savegame &sg = save();

	_log.print("Poison!\n");

	for (int i = 0; i < sg._partySize; ++i) {
		if (sg._party[i]._dex <= _G(gameRandom).range(1, 0x1E) && sg._party[i]._status != Data::STATUS_DEAD)
			sg._party[i]._status = Data::STATUS_POISONED;
	}
}

void DungeonLogic::pitTrap() {
	Data::Savegame &sg = save();
	byte tile = cell(sg._mapX, sg._mapY, sg._mapLevel);

	_arrival = 5;

	while ((tile == 0x61 || tile == 0x69) && sg._mapLevel < DUNGEON_LEVELS) {
		_log.print("Pit Trap!\nFalling...\n");

		sg._mapData[sg._mapLevel * DUNGEON_LEVEL_BYTES + sg._mapY * DUNGEON_SIZE + sg._mapX] &= 0xF8;
		++sg._mapLevel;

		if (sg._mapLevel < DUNGEON_LEVELS) {
			byte &below = sg._mapData[sg._mapLevel * DUNGEON_LEVEL_BYTES + sg._mapY * DUNGEON_SIZE + sg._mapX];
			tile = below;
			if (tile < 0x90) {
				tile |= VISITED;
				below = tile;
			}
		}

		_log.print("      ...splat!\n");
		PartyLogic::hurtParty();
	}

	// Past the lowest level is out of the dungeon
	if (sg._mapLevel == DUNGEON_LEVELS) {
		sg._mapLevel = DUNGEON_LEVELS - 1;
		leave();
	} else {
		spawnMonster();
	}
}

// The place a dungeon's monster is on the level, and where it was
DungeonLogic::Event DungeonLogic::arrive(bool timePassed) {
	Data::Savegame &sg = save();

	for (int i = 0; i < sg._partySize; ++i) {
		if (sg._party[i]._status == Data::STATUS_SLEEP && _G(gameRandom).range(0, 0x3F) < SLEEP_WAKE_ODDS + 1)
			sg._party[i]._status = Data::STATUS_GOOD;
	}

	if (timePassed && moveMonster()) {
		static const char *const SIDES[] = { "north", "east", "south", "west" };
		const Data::ActorRecord from = sg.getActor(2);

		_log.print("Attacked");

		if (((from._x - 1) & 7) == sg._mapX)
			_arrival = 1;
		else if (((from._x + 1) & 7) == sg._mapX)
			_arrival = 3;
		else if (((from._y - 1) & 7) == sg._mapY)
			_arrival = 2;
		else
			_arrival = 0;

		if (_arrival != _facing) {
			_log.print(" from the ");
			_log.print(SIDES[_arrival]);
			_facing = _arrival;
		}

		_log.print("!\n");

		Event event;
		event._kind = Event::EVENT_ATTACK;
		return event;
	}

	return checkCell();
}

DungeonLogic::Event DungeonLogic::checkCell() {
	Data::Savegame &sg = save();
	const byte tile = cell(sg._mapX, sg._mapY, sg._mapLevel);
	const int type = tile & 0xF0;
	Event event;

	if (type == CELL_ROOM || type == CELL_FIRST_WALL) {
		event._kind = Event::EVENT_ROOM;
		event._cell = tile;
		return event;
	}

	switch (tile) {
	case 0x80:
	case 0x88:
		sleepSpell();
		break;

	case 0x81:
	case 0x89:
		poison();
		break;

	case 0x82:
	case 0x8A:
		_log.print("Fire!!\n");
		PartyLogic::hurtParty();
		break;

	case 0x62:
	case 0x6A:
		_log.print("Bomb Trap!\nKABOOM!!\n");
		sg._mapData[sg._mapLevel * DUNGEON_LEVEL_BYTES + sg._mapY * DUNGEON_SIZE + sg._mapX] &= VISITED;
		PartyLogic::hurtParty();
		break;

	case 0x61:
	case 0x69:
		pitTrap();
		break;
	}

	if (isDungeon(sg._mapId))
		PartyLogic(_log).turnUpkeep();

	return event;
}

// Marks a room as one that has been fought in, if it is one that stays done with
void DungeonLogic::markRoomCleared(int room) {
	Data::Savegame &sg = save();

	for (int i = 0; i < 6; ++i) {
		if (Data::CLEARED_ROOM_SKIP[i] == (sg._mapId & 0xF) * 0x10 + room)
			return;
	}

	int dungeon = sg._mapId - 0x21;
	if (dungeon >= 1)
		--dungeon;

	const int bit = room + dungeon * 0x10;
	sg._unk033A[bit / 8] |= 1 << (bit & 7);
}

bool DungeonLogic::roomCleared(int room) {
	const Data::Savegame &sg = save();
	int dungeon = sg._mapId - 0x21;
	if (dungeon >= 1)
		--dungeon;

	const int bit = dungeon * 0x10 + room;
	return (sg._unk033A[bit / 8] & (1 << (bit & 7))) != 0;
}

void DungeonLogic::hideClearedRooms() {
	Data::Savegame &sg = save();

	for (int i = 0; i < DUNGEON_BYTES; ++i) {
		const byte c = sg._mapData[i];
		if ((c & 0xF0) == CELL_ROOM && roomCleared(c & 0xF))
			sg._mapData[i] &= 0xAF;
	}
}

void DungeonLogic::clearMonster() {
	Data::ActorRecord monster = save().getActor(1);

	monster._tile = monster._animTile = 0;
	monster._hull = monster._x = monster._y = 0xFF;
	save().setActor(1, monster);
}

// Finds a place for the wandering monster, away from the party. Gives false if there isn't one
bool DungeonLogic::placeMonster() {
	Data::Savegame &sg = save();
	Data::ActorRecord monster = sg.getActor(1);
	Data::ActorRecord before = sg.getActor(2);
	int tries;

	for (tries = 0; tries < 8; ++tries) {
		const int place = _G(gameRandom).range(0, 0x3F);
		const int type = sg._mapData[sg._mapLevel * DUNGEON_LEVEL_BYTES + place] & 0xF0;

		if (type < 0x60 || type == 0x70) {
			const int y = place / 8;
			const int x = place % 8;

			if (x != sg._mapX && y != sg._mapY) {
				monster._x = before._x = x;
				monster._y = before._y = y;

				if ((monster._hull == 0x16 || monster._hull == 0x18) && _G(gameRandom).range(0, 99) > 0x30)
					monster._skiffs = 0xFF;

				break;
			}
		}
	}

	if (tries == 8) {
		monster._x = monster._y = before._x = before._y = 0xFF;
		sg.setActor(1, monster);
		sg.setActor(2, before);
		return false;
	}

	sg.setActor(1, monster);
	sg.setActor(2, before);
	return true;
}

void DungeonLogic::spawnMonster() {
	Data::Savegame &sg = save();
	Data::ActorRecord monster = sg.getActor(1);
	const int kind = _G(gameRandom).range(0, 7);

	monster._tile = monster._animTile = kind;
	monster._skiffs = 0;
	monster._unk6 = Data::WANDERER_MODE[kind];
	monster._hull = Data::WANDERER_KIND[kind];
	monster._z = sg._mapLevel;
	sg.setActor(1, monster);

	if (!placeMonster()) {
		monster = sg.getActor(1);
		monster._tile = monster._animTile = 0;
		monster._hull = 0xFF;
		sg.setActor(1, monster);
	}
}

// The monster takes a step, unless it is of a kind that stays still. Gives true if it reached the party
bool DungeonLogic::moveMonster() {
	Data::Savegame &sg = save();
	Data::ActorRecord monster = sg.getActor(1);
	Data::ActorRecord before = sg.getActor(2);
	bool reached = false;

	if (monster._hull == 0xFF)
		return false;

	if (monster._hull != 0x1B) {
		int x = 0;
		int y = 0;
		bool moved = false;

		for (int tries = 0; tries < 8; ++tries) {
			const int way = _G(gameRandom).range(0, 3);

			x = DX[way] + monster._x;
			if (x > 7)
				x = 0;
			if (x < 0)
				x = 7;

			y = DY[way] + monster._y;
			if (y > 7)
				y = 0;
			if (y < 0)
				y = 7;

			const int type = cell(x, y, sg._mapLevel) & 0xF0;
			if (type != 0x60 && type != 0x80 && type < 0xA0) {
				if (x != before._x || y != before._y || _G(gameRandom).range(0, 7) == 1) {
					moved = true;
					break;
				}
			}
		}

		if (moved) {
			before._x = monster._x;
			before._y = monster._y;
			monster._x = x;
			monster._y = y;
		}
	}

	if (monster._x == sg._mapX && monster._y == sg._mapY) {
		reached = true;
		monster._x = before._x;
		monster._y = before._y;
	}

	sg.setActor(1, monster);
	sg.setActor(2, before);
	return reached;
}

// Fills the edges of a map of a fight with the way into the next cell
void DungeonLogic::fillSides(byte *map, byte floor, byte wall) {
	Data::Savegame &sg = save();
	const byte here = cell(sg._mapX, sg._mapY, sg._mapLevel) & 0xF0;

	for (int dir = 0; dir < 4; ++dir) {
		int x = sg._mapX;
		int y = sg._mapY;

		switch (dir) {
		case 0:
			--y;
			break;
		case 1:
			++x;
			break;
		case 2:
			++y;
			break;
		default:
			--x;
			break;
		}

		const int type = cell(x & 7, y & 7, sg._mapLevel) & 0xF0;

		if (type < 0xA0) {
			// A passage of seven cells
			int px = 2, py = 1, dx = 1, dy = 0;
			if (dir == 1) {
				px = 9;
				py = 2;
				dx = 0;
				dy = 1;
			} else if (dir == 2) {
				px = 2;
				py = 9;
			} else if (dir == 3) {
				px = 1;
				py = 2;
				dx = 0;
				dy = 1;
			}

			for (int i = 0; i < 7; ++i, px += dx, py += dy)
				map[py * COMBAT_MAP_STRIDE + px] = floor;
		}

		if (type == 0xB0 || type == 0xC0 || type == 0xD0) {
			// A wall with a door in it: the edge is solid
			int px = 0, py = 0, dx = 0, dy = 0;
			switch (dir) {
			case 0:
				dx = 1;
				break;
			case 1:
				dy = 1;
				px = 10;
				break;
			case 2:
				dx = 1;
				py = 10;
				break;
			default:
				dy = 1;
				break;
			}

			for (int i = 0; i < 11; ++i, px += dx, py += dy)
				map[py * COMBAT_MAP_STRIDE + px] = 0xFF;

			if (dir == 0 && here == CELL_DOORWAY)
				map[2 * COMBAT_MAP_STRIDE + 5] = map[8 * COMBAT_MAP_STRIDE + 5] = wall;
			if (dir == 3 && here == CELL_DOORWAY)
				map[5 * COMBAT_MAP_STRIDE + 2] = map[5 * COMBAT_MAP_STRIDE + 8] = wall;
		} else {
			// An opening of five cells
			int px = 3, py = 3, dx = 0, dy = 0;
			switch (dir) {
			case 0:
				dx = 1;
				py = 1;
				break;
			case 1:
				dy = 1;
				px = 9;
				break;
			case 2:
				dx = 1;
				py = 9;
				break;
			default:
				dy = 1;
				px = 1;
				break;
			}

			for (int i = 0; i < 5; ++i, px += dx, py += dy)
				map[py * COMBAT_MAP_STRIDE + px] = floor;
		}
	}
}

// Makes the map for a fight with a monster, from the walls and doors around the party
void DungeonLogic::buildFightMap(byte *map, bool &bothLadders, bool withEnemies) {
	Data::Savegame &sg = save();
	const bool light = style() == 3;
	const byte wall = light ? 0x4F : 0x4D;
	const byte floor = light ? 0x45 : 5;

	for (int y = 0; y < 11; ++y)
		memset(map + y * COMBAT_MAP_STRIDE, floor, 11);

	// A ring of wall with the cells of the sides beyond it
	memset(map + 1 * COMBAT_MAP_STRIDE, wall, 11);
	memset(map + 9 * COMBAT_MAP_STRIDE, wall, 11);
	for (int y = 0; y < 11; ++y)
		map[y * COMBAT_MAP_STRIDE + 1] = map[y * COMBAT_MAP_STRIDE + 9] = wall;

	map[0] = map[10] = map[10 * COMBAT_MAP_STRIDE] = map[10 * COMBAT_MAP_STRIDE + 10] = 0xFF;

	const int type = cell(sg._mapX, sg._mapY, sg._mapLevel) & 0xF0;
	bothLadders = false;
	if (type != 0 && type < 0x80) {
		const int kind = type >> 4;
		if (Data::ROOM_CENTRE[kind] != 0)
			map[5 * COMBAT_MAP_STRIDE + 5] = Data::ROOM_CENTRE[kind];

		bothLadders = kind == 3;
	}

	fillSides(map, floor, wall);

	for (int i = 0; i < 8; ++i)
		map[8 * COMBAT_MAP_STRIDE + 11 + i] = map[8 * COMBAT_MAP_STRIDE + 19 + i] = 0xFF;

	// Where the party and the enemies start
	for (int i = 0; i < 6; ++i) {
		map[3 * COMBAT_MAP_STRIDE + 11 + i] = Data::ROOM_SPOTS_D[i];
		map[2 * COMBAT_MAP_STRIDE + 11 + i] = Data::ROOM_SPOTS_C[i];
		map[4 * COMBAT_MAP_STRIDE + 11 + i] = Data::ROOM_SPOTS_A[i];
		map[1 * COMBAT_MAP_STRIDE + 11 + i] = Data::ROOM_SPOTS_B[i];
	}

	for (int i = 0; i < 6; ++i) {
		map[3 * COMBAT_MAP_STRIDE + 17 + i] = Data::ROOM_SPOTS_B[i];
		map[2 * COMBAT_MAP_STRIDE + 17 + i] = Data::ROOM_SPOTS_A[i];
		map[4 * COMBAT_MAP_STRIDE + 17 + i] = Data::ROOM_SPOTS_C[i];
		map[1 * COMBAT_MAP_STRIDE + 17 + i] = Data::ROOM_SPOTS_D[i];
	}

	for (int i = 0; i < 16; ++i) {
		byte x, y;
		switch (_facing) {
		case 0:
			x = Data::ENEMY_SPOTS_A[i];
			y = Data::ENEMY_SPOTS_C[i];
			break;
		case 1:
			x = Data::ENEMY_SPOTS_B[i];
			y = Data::ENEMY_SPOTS_D[i];
			break;
		case 2:
			x = Data::ENEMY_SPOTS_D[i];
			y = Data::ENEMY_SPOTS_B[i];
			break;
		default:
			x = Data::ENEMY_SPOTS_C[i];
			y = Data::ENEMY_SPOTS_A[i];
			break;
		}

		map[6 * COMBAT_MAP_STRIDE + 11 + i] = x;
		map[7 * COMBAT_MAP_STRIDE + 11 + i] = y;
	}

	if (!withEnemies)
		return;

	int order[16];
	for (int i = 0; i < 16; ++i)
		order[i] = i;

	for (int i = 0; i < 16; ++i) {
		const int other = _G(gameRandom).range(0, 0xF);
		const int kept = order[i];
		order[i] = order[other];
		order[other] = kept;

		map[5 * COMBAT_MAP_STRIDE + 11 + i] = 0;
	}

	const int kind = sg.getActor(1)._hull;
	int count = _G(gameRandom).range(1, Data::ENEMY_STATS[kind]._maxGroup);
	if (Data::ENEMY_STATS[kind]._maxGroup == 8 || Data::ENEMY_STATS[kind]._maxGroup == 0x10)
		count = Data::ENEMY_STATS[kind]._maxGroup;

	for (int i = 0; i < count; ++i)
		map[5 * COMBAT_MAP_STRIDE + 11 + order[i]] = kind * 4 + 0x40;
}

bool DungeonLogic::prepareFight(CombatLogic &fight, Event &event) {
	Data::Savegame &sg = save();

	if (event._kind == Event::EVENT_ATTACK) {
		bool ladders;
		memset(fight.mapData(), 0, 0x400);
		buildFightMap(fight.mapData(), ladders);
		event._bothLadders = ladders;
		event._savedX = sg._mapX;
		event._savedY = sg._mapY;

		fight.beginDungeon(sg._mapId, 2, _facing, 0, sg._mapLevel, true, ladders);
		return true;
	}

	_log.print("Entering room...\n");
	_facing = Data::ARRIVAL_FACING[_arrival];

	int dungeon = sg._mapId - 0x21;
	if (dungeon >= 1)
		--dungeon;

	event._savedX = sg._mapX;
	event._savedY = sg._mapY;

	if (!fight.loadMap("dungeon.cbt", 0x1600 * dungeon + (event._cell & 0xF) * COMBAT_MAP_BYTES))
		return false;

	fight.beginDungeon(sg._mapId, 0x82, _facing, event._cell, sg._mapLevel, event._cell > 0xEF, false);
	return true;
}

void DungeonLogic::applyExit(int code) {
	Data::Savegame &sg = save();

	switch (code) {
	case 3:
		--sg._mapY;
		_arrival = _facing = 0;
		if (sg._mapY > 7)
			sg._mapY = 7;
		break;

	case 2:
		++sg._mapX;
		_arrival = _facing = 1;
		if (sg._mapX > 7)
			sg._mapX = 0;
		break;

	case 4:
		++sg._mapY;
		_arrival = _facing = 2;
		if (sg._mapY > 7)
			sg._mapY = 0;
		break;

	case 1:
		--sg._mapX;
		_arrival = _facing = 3;
		if (sg._mapX > 7)
			sg._mapX = 7;
		break;

	case 5:
		if (sg._mapLevel == 0) {
			leave();
		} else {
			--sg._mapLevel;
			_arrival = 4;
		}
		break;

	case 6:
		if (sg._mapLevel == 7) {
			leave();
		} else {
			++sg._mapLevel;
			_arrival = 5;
		}
		break;
	}
}

DungeonLogic::Event DungeonLogic::finishFight(const CombatLogic &fight, const Event &event) {
	Data::Savegame &sg = save();
	Event next;

	if (event._kind == Event::EVENT_ATTACK) {
		applyExit(fight.exitCode());

		clearMonster();
		if (isDungeon(sg._mapId)) {
			spawnMonster();
			return checkCell();
		}

		return next;
	}

	if (fight.result() == 0 && fight.exitCode() == CombatLogic::EXIT_ENDGAME) {
		next._kind = Event::EVENT_ENDGAME;
		return next;
	}

	if (fight.result() == 0) {
		markRoomCleared(event._cell & 0xF);
		sg._mapData[sg._mapLevel * DUNGEON_LEVEL_BYTES + event._savedY * DUNGEON_SIZE + event._savedX] &= 0xAF;
	}

	sg._mapX = event._savedX;
	sg._mapY = event._savedY;
	applyExit(fight.exitCode());

	if (!isDungeon(sg._mapId))
		return next;

	const byte tile = cell(sg._mapX, sg._mapY, sg._mapLevel);
	if (((tile & 0xF0) == CELL_ROOM || (tile & 0xF0) == CELL_FIRST_WALL) && sg._party[0]._status != Data::STATUS_DEAD) {
		next._kind = Event::EVENT_ROOM;
		next._cell = tile;
		return next;
	}

	clearMonster();
	PartyLogic(_log).turnUpkeep();
	return next;
}

// Where the party and the enemies stand in the place that is made to camp in
static const byte CAMP_PARTY_X[6] = { 5, 6, 5, 4, 4, 6 };
static const byte CAMP_PARTY_Y[6] = { 4, 5, 6, 5, 4, 6 };
static const byte CAMP_ENEMY_X[16] = { 2, 2, 2, 3, 3, 3, 4, 5, 5, 6, 7, 7, 7, 8, 8, 8 };
static const byte CAMP_ENEMY_Y[16] = { 4, 5, 7, 2, 3, 8, 8, 3, 8, 3, 4, 7, 8, 3, 6, 7 };
constexpr byte CAMP_FIRE_TILE = 0xB3;
constexpr int CAMP_MAP_BUFFER = 0x400;

void DungeonLogic::buildCampMap(byte *map) {
	const byte floor = style() == 3 ? 0x45 : 5;
	bool bothLadders;

	memset(map, 0, CAMP_MAP_BUFFER);
	buildFightMap(map, bothLadders, false);

	// A fire is in the middle, unless the cell has something of its own there
	if (map[5 * COMBAT_MAP_STRIDE + 5] == floor)
		map[5 * COMBAT_MAP_STRIDE + 5] = CAMP_FIRE_TILE;

	for (int i = 0; i < 6; ++i) {
		for (int row = 1; row <= 4; ++row) {
			map[row * COMBAT_MAP_STRIDE + 11 + i] = CAMP_PARTY_X[i];
			map[row * COMBAT_MAP_STRIDE + 17 + i] = CAMP_PARTY_Y[i];
		}
	}

	for (int i = 0; i < 16; ++i) {
		map[6 * COMBAT_MAP_STRIDE + 11 + i] = CAMP_ENEMY_X[i];
		map[7 * COMBAT_MAP_STRIDE + 11 + i] = CAMP_ENEMY_Y[i];
	}
}

void DungeonLogic::climbOptions(bool &up, bool &down, bool &hook) {
	const Data::Savegame &sg = save();
	const int type = seen(sg._mapX, sg._mapY) & 0xF0;
	const bool marked = (cell(sg._mapX, sg._mapY, sg._mapLevel) & VISITED) != 0;

	up = type == CELL_LADDER_UP || type == CELL_LADDER_BOTH || (marked && sg._grapple != 0);
	down = type == CELL_LADDER_DOWN || type == CELL_LADDER_BOTH || type == CELL_TRAP_A;
	hook = marked;
}

bool DungeonLogic::climb(int direction) {
	Data::Savegame &sg = save();

	_log.print(direction > 0 ? "Down!\n" : "Up!\n");

	if ((direction > 0 && sg._mapLevel == DUNGEON_LEVELS - 1) || (direction < 0 && sg._mapLevel == 0))
		return true;

	sg._mapLevel += direction;
	_arrival = direction > 0 ? 5 : 4;
	spawnMonster();
	return false;
}

void DungeonLogic::leave() {
	Data::Savegame &sg = save();

	sg._mapX = Data::LOCATION_ENTRANCE_X[sg._mapId - 1];
	sg._mapY = Data::LOCATION_ENTRANCE_Y[sg._mapId - 1];

	_log.print("\nExit to ");
	if (sg._mapLevel != 0) {
		sg._mapLevel = UNDERWORLD_LEVEL;
		_log.print("Underworld!\n\n");
	} else {
		sg._mapLevel = 0;
		_log.print("Britannia!\n\n");
	}

	sg._mapId = 0;
	sg.restoreWorldObjects(sg._mapLevel != 0);
	TransportLogic::placePurchasedShip();
	WorldLogic::centerWindow(sg);
	_G(visibility).invalidate();
}

} // namespace Ultima5
} // namespace Ultima
