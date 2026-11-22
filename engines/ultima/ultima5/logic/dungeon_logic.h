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

#ifndef ULTIMA5_LOGIC_DUNGEON_LOGIC_H
#define ULTIMA5_LOGIC_DUNGEON_LOGIC_H

#include "ultima/ultima5/data/savegame.h"
#include "ultima/ultima5/logic/combat_logic.h"
#include "ultima/ultima5/views/message_log.h"

namespace Ultima {
namespace Ultima5 {

constexpr int DUNGEON_SIZE = 8;
constexpr int DUNGEON_LEVELS = 8;
constexpr int DUNGEON_LEVEL_BYTES = DUNGEON_SIZE * DUNGEON_SIZE;
constexpr int DUNGEON_BYTES = DUNGEON_LEVEL_BYTES * DUNGEON_LEVELS;

/**
 * The dungeons, caves and mines, walked through a cell at a time and seen from
 * inside. The first of the maps of this kind is the first after the places in towns
 */
class DungeonLogic {
public:
	// What a key did when the party was walking about
	enum Move {
		MOVE_NONE,		// Nothing was done
		MOVE_TURNED,	// The party only turned
		MOVE_WALKED		// The party went, or tried to
	};

	// The kinds of thing in a cell, in its upper four bits
	enum Cell {
		CELL_OPEN = 0x00,
		CELL_LADDER_UP = 0x10,
		CELL_LADDER_DOWN = 0x20,
		CELL_LADDER_BOTH = 0x30,
		CELL_CHEST = 0x40,
		CELL_FOUNTAIN = 0x50,
		CELL_TRAP_A = 0x60,
		CELL_FIELD = 0x80,
		CELL_FIRST_WALL = 0xA0,
		CELL_SIGN = 0xB0,
		CELL_DOOR_WALL = 0xC0,
		CELL_SECRET_DOOR = 0xD0,
		CELL_DOORWAY = 0xE0,
		CELL_ROOM = 0xF0
	};

	/**
	 * What came of arriving in a cell, which may be a fight
	 */
	struct Event {
		enum Kind {
			EVENT_NONE,
			EVENT_ATTACK,	// A monster has come to the party
			EVENT_ROOM,		// The party is in a room
			EVENT_ENDGAME	// The party has gone through the mirror at the end of the game
		};

		Kind _kind = EVENT_NONE;
		int _cell = 0;			// What the cell is, for a room
		int _savedX = 0;		// Where the party was
		int _savedY = 0;
		bool _bothLadders = false;
	};

private:
	Views::MessageLog &_log;

	// Which way the party faces, and how it got here
	static int _facing;
	static int _arrival;

	static void markRoomCleared(int room);
	static bool roomCleared(int room);
	static bool placeMonster();
	bool moveMonster();
	static void buildFightMap(byte *map, bool &bothLadders);
	static void fillSides(byte *map, byte floor, byte wall);
	void electricField(int sign);
	void pitTrap();
	void sleepSpell();
	void poison();

public:
	DungeonLogic(Views::MessageLog &log) : _log(log) {}

	static bool isDungeon(byte mapId) {
		return mapId > 0x20;
	}

	static int facing() {
		return _facing;
	}

	/**
	 * Gives which of the three looks of walls the dungeon has
	 */
	static int style();

	/**
	 * Returns what's in a cell of a level, with the wrap-around of the map
	 */
	static byte cell(int x, int y, int level);

	/**
	 * Returns what the party sees of a cell: the same, without the mark that it's been walked on
	 */
	static byte seen(int x, int y);

	/**
	 * Goes into the dungeon, cave or mine that the party is at the mouth of
	 * @param kind	The word for it
	 * @returns		False if the party can't, which has been said
	 */
	bool enter(const char *kind);

	/**
	 * Goes where a key says, or turns
	 */
	Move command(int key);

	/**
	 * Does what's in the cell the party has come to, and the turn's upkeep
	 * @param timePassed	Whether the monster has its turn as well
	 */
	Event arrive(bool timePassed);

	/**
	 * Works out what's in the cell the party is in, which can be a room
	 */
	Event checkCell();

	/**
	 * Puts a new wandering monster somewhere, or takes it away
	 */
	static void spawnMonster();
	static void clearMonster();

	/**
	 * Sets up a fight with a monster that has come to the party, or with what is in a room
	 */
	bool prepareFight(CombatLogic &fight, Event &event);

	/**
	 * Does what follows from the fight, which can lead to another
	 * @returns		What the party has come to next
	 */
	Event finishFight(const CombatLogic &fight, const Event &event);

	/**
	 * Goes the way the party left a fight by
	 */
	void applyExit(int code);

	/**
	 * Marks the rooms that have been fought in as empty
	 */
	static void hideClearedRooms();

	/**
	 * Climbs a ladder, up if negative
	 * @returns		True if that is out of the dungeon
	 */
	bool climb(int direction);

	/**
	 * Gives which ways there are to climb from where the party is
	 */
	static void climbOptions(bool &up, bool &down, bool &hook);

	/**
	 * Leaves for the world the party came from
	 */
	void leave();

	/**
	 * Keeps the facing for what may be a different dungeon
	 */
	static void setFacing(int facing) {
		_facing = facing;
	}
};

} // namespace Ultima5
} // namespace Ultima

#endif
