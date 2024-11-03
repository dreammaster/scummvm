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

#ifndef ULTIMA_ULTIMA0_STRUCT_H
#define ULTIMA_ULTIMA0_STRUCT_H

#include "ultima/ultima0/data/defines.h"
#include "ultima/ultima0/data/player.h"

namespace Ultima {
namespace Ultima0 {

struct RECT {
	int left, top, right, bottom;		// Rectangle coords
};

struct WORLDMAP {
	int MapSize;								// Size of map
	byte Map[WORLD_MAP_SIZE][WORLD_MAP_SIZE];	// Info
};

struct MONSTER {
	COORD Loc;							// Position
	int	  Type;							// Monster type
	int	  Strength;						// Strength
	int	  Alive;						// Alive flag
};

struct DUNGEONMAP {
	int MapSize;							// Size of Map
	byte Map[DUNGEON_MAP_SIZE][DUNGEON_MAP_SIZE];	// Map information
	int	MonstCount;							// Number of Monsters
	MONSTER Monster[MAX_MONSTERS];			// Monster records
};

} // namespace Ultima0
} // namespace Ultima

#endif
