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

#include "ultima/ultima5/logic/world_logic.h"
#include "ultima/ultima5/logic/time_logic.h"
#include "ultima/ultima5/data/world_map.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {

constexpr int MINUTES_PER_SLOW_STEP = 2;
constexpr int MINUTES_PER_VERY_SLOW_STEP = 4;

// What can move over what. Each terrain tile is a bit, which is set if it blocks
static const byte BLOCKING_TERRAIN[32] = {
	0x70, 0x0C, 0x00, 0x28, 0x01, 0xF3, 0x00, 0xBD,
	0x72, 0x3F, 0xFF, 0xFF, 0xFF, 0xCF, 0xFF, 0xFF,
	0xFC, 0xF6, 0x0F, 0xFF, 0xFF, 0xC7, 0xFF, 0xF7,
	0xF0, 0x3F, 0xFF, 0xF3, 0xFF, 0xFF, 0xFF, 0xBE
};

enum MovementKind {
	MOVE_NORMAL = 0, MOVE_SEA_MONSTER = 1, MOVE_FLYING = 2, MOVE_HORSE = 3, MOVE_GHOST = 4,
	MOVE_SKIFF = 5, MOVE_FRIGATE = 6, MOVE_ROTWORM = 7, MOVE_CORPSER = 8, MOVE_WHIRLPOOL = 9,
	MOVE_SAND_TRAP = 10, MOVE_NONE = 0xFF
};

// The kind of movement of each group of 4 actor tiles
static const byte MOVEMENT_KINDS[64] = {
	0x00, 0x00, 0x00, 0x00, 0x03, 0x02, 0x00, 0x00,
	0x06, 0x06, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x02,
	0x01, 0x01, 0x01, 0x01, 0x00, 0x02, 0x00, 0x04,
	0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02,
	0x0A, 0x00, 0xFF, 0x09, 0x02, 0x08, 0x07, 0x04
};

// Which ways a boat can go through each of the river and shore tiles, a bit per
// direction
static const byte BOAT_RIVER_DIRECTIONS[16] = {
	0x0A, 0x05, 0x03, 0x09, 0x0C, 0x06, 0x0B, 0x0D, 0x0E, 0x07, 0x0A, 0x05, 0x08, 0x04, 0x02, 0x01
};
static const byte BOAT_CHANNEL_DIRECTIONS[4] = { 0x06, 0x03, 0x09, 0x0C };

constexpr byte TILE_FIRST_RIVER = 0x34;
constexpr byte TILE_FIRST_SHORE = 0x60;
constexpr byte TILE_LAVA = 0x8F;
constexpr byte TILE_CHAIR_FIRST = 0x90;
constexpr byte TILE_VERY_SLOW_FIRST = 9;
constexpr byte TILE_SLOW_A = 0x1E;
constexpr byte TILE_SLOW_B = 0x1F;

constexpr byte ACTOR_FIRST_FRIGATE = 0x20;
constexpr byte ACTOR_SECOND_FRIGATE = 0x24;
constexpr byte ACTOR_SKIFF = 0x28;
constexpr byte ACTOR_PIRATE = 0x2C;
constexpr byte ACTOR_FIRST_LAND = 0x30;
constexpr byte ACTOR_HORSE = 0x10;
constexpr byte ACTOR_CARPET = 0x1B;
constexpr byte ACTOR_WIZARD_GROUP = 0x40;

static bool isOpenWater(byte tile) {
	return tile < 4 || (tile & 0xF0) == TILE_FIRST_SHORE;
}

// Land creatures, which can't sit on furniture meant for people
static bool isWalkableByLand(byte actorTile, byte terrain) {
	bool open = (BLOCKING_TERRAIN[terrain >> 3] & (0x80 >> (terrain & 7))) == 0;

	if ((actorTile & 0xFE) != TILE_AVATAR && (actorTile & 0xF0) != ACTOR_WIZARD_GROUP &&
			(terrain & 0xFC) == TILE_CHAIR_FIRST)
		open = false;

	return open;
}

bool WorldLogic::isWalkable(byte actorTile, byte terrain) {
	switch (MOVEMENT_KINDS[actorTile >> 2]) {
	case MOVE_NORMAL:
		return isWalkableByLand(actorTile, terrain);

	case MOVE_SEA_MONSTER:
		return isOpenWater(terrain);

	case MOVE_FLYING:
		return (terrain & 0xF0) == TILE_FIRST_SHORE || isOpenWater(terrain) || isWalkableByLand(actorTile, terrain);

	case MOVE_HORSE:
		return isWalkableByLand(actorTile, terrain) && terrain != TILE_LAVA && terrain != Data::TILE_POISON;

	case MOVE_GHOST:
		return !isOpenWater(terrain);

	case MOVE_SKIFF:
		if ((terrain & 0xFC) == TILE_FIRST_RIVER)
			return ((8 >> (actorTile & 3)) & BOAT_CHANNEL_DIRECTIONS[terrain - TILE_FIRST_RIVER]) != 0;
		if (!isOpenWater(terrain))
			return false;
		if (terrain < TILE_FIRST_SHORE)
			return true;
		return (BOAT_RIVER_DIRECTIONS[terrain - TILE_FIRST_SHORE] & (8 >> (actorTile & 3))) != 0;

	case MOVE_FRIGATE:
		return terrain <= 2;

	case MOVE_ROTWORM:
		return terrain == Data::TILE_POISON;

	case MOVE_CORPSER:
		return terrain == Data::TILE_GRASS;

	case MOVE_WHIRLPOOL:
		return terrain == Data::TILE_WATER;

	case MOVE_SAND_TRAP:
		return terrain == 7;

	default:
		return false;
	}
}

void WorldLogic::syncPartyObject(Data::Savegame &sg) {
	Data::ActorRecord party = sg.getActor(0);
	party._x = sg._mapX;
	party._y = sg._mapY;
	party._z = sg._mapLevel;
	party._tile = party._animTile = sg._partyTile;
	sg.setActor(0, party);
}

// Returns the tile of the object at a place, other than the party, or zero
static byte findObjectTile(const Data::Savegame &sg, int x, int y) {
	for (int i = 1; i < Data::ACTOR_COUNT; ++i) {
		const Data::ActorRecord actor = sg.getActor(i);
		if (actor._x == x && actor._y == y && actor._z == sg._mapLevel)
			return actor._tile;
	}

	return 0;
}

bool WorldLogic::walk(Direction direction) {
	static const char *const NAMES[] = { "North\n", "East\n", "South\n", "West\n" };
	static const int8 DX[] = { 0, 1, 0, -1 };
	static const int8 DY[] = { -1, 0, 1, 0 };

	Data::Savegame &sg = _G(savegame);
	_log.print(NAMES[direction]);

	const byte x = sg._mapX + DX[direction];
	const byte y = sg._mapY + DY[direction];
	const byte party = sg._partyTile;

	// Other things in the way, unless they're something to get on
	bool free = true;
	const byte other = findObjectTile(sg, x, y);
	if (other != 0) {
		free = false;

		if (party >= ACTOR_FIRST_LAND || party < ACTOR_FIRST_FRIGATE) {
			if ((other >= ACTOR_SECOND_FRIGATE && other < ACTOR_PIRATE) || other == ACTOR_CARPET ||
					(other & 0xFE) == ACTOR_HORSE)
				free = true;
		} else if (party >= ACTOR_SKIFF && other >= ACTOR_SECOND_FRIGATE && other < ACTOR_SKIFF) {
			free = true;
		}
	}

	if (!free || !isWalkable(party, _G(world).getTile(x, y))) {
		_log.print("Blocked!\n");
		return false;
	}

	sg._mapX = x;
	sg._mapY = y;

	// Rough ground slows the party down
	const byte ground = _G(world).getTile(x, y);
	int slowness = 0;
	if (ground == Data::TILE_GRASS) {
		slowness = 0;
	} else if (ground == TILE_SLOW_A || ground == TILE_SLOW_B) {
		slowness = 1;
	} else if (ground < Data::TILE_POISON || ground >= Data::TILE_HUT) {
		slowness = 0;
	} else if (ground >= TILE_VERY_SLOW_FIRST) {
		slowness = 2;
	} else {
		slowness = 1;
	}

	if (slowness == 1) {
		_log.print("Slow progress!\n");
		TimeLogic::advance(sg, MINUTES_PER_SLOW_STEP);
	} else if (slowness == 2) {
		_log.print("Very slow!\n");
		TimeLogic::advance(sg, MINUTES_PER_VERY_SLOW_STEP);
	}

	return true;
}

void WorldLogic::pass() {
	_log.print("Pass\n");
}

} // namespace Ultima5
} // namespace Ultima
