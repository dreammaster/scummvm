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

#ifndef ULTIMA5_LOGIC_DUNGEON_ITEMS_H
#define ULTIMA5_LOGIC_DUNGEON_ITEMS_H

#include "ultima/ultima5/views/message_log.h"

namespace Ultima {
namespace Ultima5 {

/**
 * What the party does with the chests, the fountains and the walls of a dungeon:
 * looking at them, searching them, opening them and taking what's in them
 */
class DungeonItems {
private:
	Views::MessageLog &_log;

	/**
	 * Gives the cell the party is in, or one next to it
	 */
	static byte &cellAt(int x, int y);

public:
	DungeonItems(Views::MessageLog &log) : _log(log) {}

	/**
	 * Works out where a direction relative to the way the party faces leads
	 * @param key	The arrow key: ahead, here, right or left
	 */
	static void target(int key, int &x, int &y);

	/**
	 * Tells how good a member of the party is at finding things on the level
	 */
	static int skill(int who);

	/**
	 * Says what a search finds in a cell
	 */
	void search(int who, int x, int y);

	/**
	 * Says what is in a cell, and what drinking from it does
	 * @returns		True if there's a fountain that a drink is wanted from
	 */
	bool look(int x, int y);

	/**
	 * Drinks from the fountain
	 */
	void drink(int who, int x, int y);

	/**
	 * Opens the chest the party is in, which may be trapped
	 */
	void open(int who);

	/**
	 * Tries to pick the lock of the chest the party is in
	 */
	void jimmy(int who);

	/**
	 * Takes the contents of an open chest
	 */
	void take();

	/**
	 * What a trapped chest does to a member of the party
	 */
	void chestTrap(int who);

	/**
	 * Adds something found to what the party has, and says what it is
	 * @param tile		What kind of thing it is, as the objects are numbered
	 * @param amount	How many, or which
	 */
	void award(int tile, int amount);
};

} // namespace Ultima5
} // namespace Ultima

#endif
