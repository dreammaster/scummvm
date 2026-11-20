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

#ifndef ULTIMA5_LOGIC_NPC_LOGIC_H
#define ULTIMA5_LOGIC_NPC_LOGIC_H

#include "ultima/ultima5/data/savegame.h"

namespace Ultima {
namespace Ultima5 {

/**
 * The people of the towns, castles, keeps and dwellings: where they
 * are at the time of day, and putting them on the map
 */
class NpcLogic {
private:
	/**
	 * Reads the people of the location the party is in from the game folder
	 * @returns		False if they couldn't be loaded
	 */
	static bool loadFile(Data::Savegame &savegame);

	/**
	 * Works out where each person is, for the time of day
	 */
	static void initialize(Data::Savegame &savegame);

public:
	/**
	 * Finds a spare object for a person, or returns zero if there isn't one
	 */
	static int findFreeObject(const Data::Savegame &savegame);

	/**
	 * Puts a person at a place, on the map if that's the level the party is on
	 */
	static void placeNpc(Data::Savegame &savegame, int npc, int x, int y, int z);

	/**
	 * Returns which of a person's three schedules is the one for an hour
	 */
	static int scheduleSlot(const Data::NpcSchedule &schedule, byte hour);

	/**
	 * Puts the people on the map as the day has them, on the level the party is on
	 */
	static void place(Data::Savegame &savegame);

	/**
	 * Takes a person off the map, and out of the game
	 */
	static void despawn(Data::Savegame &savegame, int npc);

	/**
	 * Sets up the people of the location the party has just come to
	 * @returns		False if they couldn't be loaded
	 */
	static bool arrive(Data::Savegame &savegame);
};

} // namespace Ultima5
} // namespace Ultima

#endif
