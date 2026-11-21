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

	return true;
}

void DungeonLogic::electricField(int sign) {
	Data::Savegame &sg = save();

	_log.print("Ouch!\nElectric field!\n");

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
	}
}

void DungeonLogic::arrive() {
	Data::Savegame &sg = save();

	for (int i = 0; i < sg._partySize; ++i) {
		if (sg._party[i]._status == Data::STATUS_SLEEP && _G(gameRandom).range(0, 0x3F) < SLEEP_WAKE_ODDS + 1)
			sg._party[i]._status = Data::STATUS_GOOD;
	}

	const byte tile = cell(sg._mapX, sg._mapY, sg._mapLevel);

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
