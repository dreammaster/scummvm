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

#ifndef ULTIMA5_LOGIC_WORLD_LOGIC_H
#define ULTIMA5_LOGIC_WORLD_LOGIC_H

#include "common/scummsys.h"
#include "ultima/ultima5/data/savegame.h"

namespace Ultima {
namespace Ultima5 {

enum Direction {
	DIR_NORTH = 0, DIR_EAST = 1, DIR_SOUTH = 2, DIR_WEST = 3
};

// The tile of the Avatar on foot
constexpr byte TILE_AVATAR = 0x1C;

/**
 * The rules for moving around Britannia and the underworld
 */
class WorldLogic {
public:
	/**
	 * Returns true if something of a kind can go onto a kind of terrain
	 * @param actorTile		The tile of what's moving
	 * @param terrainTile	The tile of what it's moving onto
	 */
	static bool isWalkable(byte actorTile, byte terrainTile);

	/**
	 * Makes sure that the first object, which is the party, is where
	 * the party is
	 */
	static void syncPartyObject(Data::Savegame &savegame);

	/**
	 * Moves the party a step, or reports why it can't
	 * @returns		True if the party moved
	 */
	bool walk(Direction direction);

	/**
	 * The party waits a while
	 */
	void pass();
};

} // namespace Ultima5
} // namespace Ultima

#endif
