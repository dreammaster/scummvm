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

#include "ultima/ultima5/logic/location_logic.h"
#include "ultima/ultima5/logic/game_random.h"
#include "ultima/ultima5/logic/npc_logic.h"
#include "ultima/ultima5/logic/time_logic.h"
#include "ultima/ultima5/data/location_tables.h"
#include "ultima/ultima5/ultima5.h"

namespace Ultima {
namespace Ultima5 {

// Where the party starts in a location, at the bottom in the middle
constexpr byte ENTRANCE_X = 0x0F;
constexpr byte ENTRANCE_Y = 0x1E;

// The maps that are entered from the map of Britannia, from the dwellings to the
// castles; the shrines and dungeons are different
constexpr int LOCATION_NAMED_COUNT = 0x20;
constexpr int FIRST_UNNAMED_LOCATION = 0x0D;
constexpr int LAST_UNNAMED_LOCATION = 0x11;
constexpr int LOG_WIDTH = 16;

// The map of the shrine of the Codex, which is left for the underworld
constexpr byte CODEX_MAP = 0x19;
constexpr byte UNDERWORLD_LEVEL = 0xFF;

constexpr byte TILE_HUT = 0x10;
constexpr byte TILE_KEEP = 0x12;
constexpr byte TILE_VILLAGE = 0x13;
constexpr byte TILE_TOWNE = 0x14;
constexpr byte TILE_CASTLE = 0x15;
constexpr byte TILE_LIGHTHOUSE = 0x1B;
constexpr byte TILE_PALACE_OF_BLACKTHORN = 0x39;
constexpr byte TILE_CASTLE_OF_LORD_BRITISH = 0x3E;
constexpr byte TILE_STAIRS = 0xC4;

constexpr byte TILE_WINDOW = 0x87;
constexpr byte TILE_WINDOW_LIT = 0xDD;
constexpr byte TILE_BRIDGE_FIRST = 0x48;
constexpr byte TILE_BRIDGE_RAISED = 3;
constexpr byte TILE_TREE = 0x2E;
constexpr byte TILE_STUMP = 0x2B;
constexpr byte TILE_CROPS = 0x2D;
constexpr byte TILE_CROPS_PICKED = 0x2C;

constexpr int FIRST_NIGHT_HOUR = 20;
constexpr int FIRST_DAY_HOUR = 5;
constexpr int LAST_DAY_HOUR = 19;

constexpr byte ACTOR_SLEEPING = 0x1E;
constexpr byte ACTOR_DEAD = 0x1F;

// The ground changes from day to day, as trees are cut and crops picked, except
// on the day the shadowlords come
constexpr byte NO_CHANGES_FLAG = 0xFF;
constexpr int CHANGE_ODDS = 7;
constexpr int SHADOWLORD_PLACES = 3;

static void changeGround(Data::LocationMap &map, const Data::Savegame &sg) {
	if (sg._unk03B2 == NO_CHANGES_FLAG)
		return;

	GameRandom random;
	random.seed(sg._day);

	for (int y = 0; y < Data::LOCATION_SIZE; ++y) {
		for (int x = 0; x < Data::LOCATION_SIZE; ++x) {
			const byte tile = map.getTile(x, y);

			if (tile == TILE_TREE && random.range(0, CHANGE_ODDS) != 0)
				map.setTile(x, y, TILE_STUMP);
			else if (tile == TILE_CROPS && random.range(0, CHANGE_ODDS) != 0)
				map.setTile(x, y, TILE_CROPS_PICKED);
		}
	}
}

bool LocationLogic::load() {
	Data::Savegame &sg = _G(savegame);
	if (_G(location).isLoaded(sg._mapId, sg._mapLevel))
		return true;

	if (!_G(location).load(sg._mapId, sg._mapLevel))
		return false;

	if (sg._hour < FIRST_DAY_HOUR || sg._hour > LAST_DAY_HOUR)
		updateForHour();

	changeGround(_G(location), sg);
	_G(visibility).invalidate();
	TimeLogic::advance(sg, 0);
	return true;
}

void LocationLogic::updateForHour() {
	const Data::Savegame &sg = _G(savegame);
	Data::LocationMap &map = _G(location);

	for (int y = 0; y < Data::LOCATION_SIZE - 1; ++y) {
		for (int x = 0; x < Data::LOCATION_SIZE; ++x) {
			if (map.getTile(x, y) == TILE_WINDOW)
				map.setTile(x, y + 1, map.getTile(x, y + 1) ^ TILE_WINDOW_LIT);
		}
	}

	// The drawbridges go up at night, unless the party is on one
	if ((map.getTile(sg._mapX, sg._mapY) & 0xFE) != TILE_BRIDGE_FIRST) {
		for (int i = 0; i < map.getBridgeCount(); ++i)
			map.setTile(map.getBridgeX(i), map.getBridgeY(i),
				sg._hour == FIRST_DAY_HOUR ? map.getBridgeTile(i) : TILE_BRIDGE_RAISED);
	}

	_G(visibility).invalidate();
}

bool LocationLogic::arrive() {
	Data::Savegame &sg = _G(savegame);

	// The shadowlords are in three of the locations each day, and spoil the ground there
	sg._unk03B2 = NO_CHANGES_FLAG;
	for (int i = 0; i < SHADOWLORD_PLACES; ++i) {
		if (sg._unk0322[i] == sg._mapId) {
			sg._unk03B2 = i;
			break;
		}
	}

	_G(gameRandom).seed(g_engine->getRandomNumber(0xFFFF));

	// Locations are drawn from their top left
	sg._chunkX = sg._chunkY = 0;

	_G(location).unload();
	_G(visibility).invalidate();
	return NpcLogic::arrive(sg);
}

bool LocationLogic::enter() {
	const Data::Savegame &sg = _G(savegame);

	switch (_G(world).getTile(sg._mapX, sg._mapY)) {
	case TILE_HUT:
		return goIn("hut");
	case TILE_KEEP:
		return goIn("keep");
	case TILE_VILLAGE:
		return goIn("village");
	case TILE_TOWNE:
		return goIn("towne");
	case TILE_CASTLE:
		return goIn("castle");
	case TILE_LIGHTHOUSE:
		return goIn("lighthouse");
	case TILE_PALACE_OF_BLACKTHORN:
		return goIn("the palace of Blackthorn!");
	case TILE_CASTLE_OF_LORD_BRITISH:
		return goIn("the Castle of Lord British!");
	default:
		_log.print("\n");
		return false;
	}
}

bool LocationLogic::goIn(const char *kind) {
	Data::Savegame &sg = _G(savegame);
	_log.print(kind);

	int location = 0;
	while (location < LOCATION_NAMED_COUNT && (Data::LOCATION_ENTRANCE_X[location] != sg._mapX ||
			Data::LOCATION_ENTRANCE_Y[location] != sg._mapY))
		++location;

	if (location == LOCATION_NAMED_COUNT) {
		_log.print("\nWhat town?\n");
		return true;
	}

	if (location < FIRST_UNNAMED_LOCATION || location > LAST_UNNAMED_LOCATION) {
		// The name is centered in the window
		Common::String line("\n\n");
		const Common::String name = Data::LOCATION_NAMES[location];
		for (uint i = 0; i < (LOG_WIDTH - name.size()) / 2; ++i)
			line += ' ';

		_log.print(line + name);
	}
	_log.print("\n");

	sg.storeWorldObjects(sg._mapLevel != 0);
	sg.clearObjects();

	sg._mapId = location + 1;
	sg._mapLevel = 0;
	sg._mapX = ENTRANCE_X;
	sg._mapY = ENTRANCE_Y;
	arrive();
	return false;
}

LocationLogic::WalkResult LocationLogic::walk(Direction direction) {
	static const char *const NAMES[] = { "North\n", "East\n", "South\n", "West\n" };
	static const int8 DX[] = { 0, 1, 0, -1 };
	static const int8 DY[] = { -1, 0, 1, 0 };

	Data::Savegame &sg = _G(savegame);
	_log.print(NAMES[direction]);

	const int x = sg._mapX + DX[direction];
	const int y = sg._mapY + DY[direction];
	const bool leaving = x < 0 || y < 0 || x >= Data::LOCATION_SIZE || y >= Data::LOCATION_SIZE;
	const byte tile = _G(location).getTile(x, y);
	const byte party = sg._partyTile;

	// Other things in the way, unless they're something to get on or step over
	bool free = true;
	const byte other = WorldLogic::objectTileAt(sg, x, y);
	if (other != 0) {
		free = false;

		if (party >= 0x30 || party < 0x20) {
			if ((other >= 0x24 && other < 0x2C) || other == 0x1B || (other & 0xFE) == 0x10 ||
					other == ACTOR_SLEEPING || other == ACTOR_DEAD)
				free = true;
		} else if (party >= 0x28 && other >= 0x24 && other < 0x28) {
			free = true;
		}
	}

	if (!free || !WorldLogic::isWalkable(party, tile)) {
		_log.print("Blocked!\n");
		return WALK_BLOCKED;
	}

	if (leaving)
		return WALK_AT_EDGE;

	sg._mapX = x;
	sg._mapY = y;

	// Stairs lead up in the direction they're facing, and down the other way
	if ((tile & 0xFC) == TILE_STAIRS) {
		const int facing = tile - TILE_STAIRS;

		if (facing == direction) {
			++sg._mapLevel;
			_log.print("Up!\n");
			NpcLogic::place(sg);
		} else if (facing == (direction ^ 2)) {
			--sg._mapLevel;
			_log.print("Down!\n");
			NpcLogic::place(sg);
		}
	}

	return WALK_MOVED;
}

void LocationLogic::leave() {
	Data::Savegame &sg = _G(savegame);
	_log.print("Yes\n\nExit to\n");

	if (sg._mapId == CODEX_MAP) {
		_log.print("Underworld!\n");
		sg._mapLevel = UNDERWORLD_LEVEL;
	} else {
		_log.print("Britannia!\n");
		sg._mapLevel = 0;
	}

	sg._doorOpen = 0;
	sg._mapX = Data::LOCATION_ENTRANCE_X[sg._mapId - 1];
	sg._mapY = Data::LOCATION_ENTRANCE_Y[sg._mapId - 1];
	sg._mapId = 0;
	sg.restoreWorldObjects(sg._mapLevel != 0);
	WorldLogic::centerWindow(sg);
	_G(visibility).invalidate();
}

} // namespace Ultima5
} // namespace Ultima
