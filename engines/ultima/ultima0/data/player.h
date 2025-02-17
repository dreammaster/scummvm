
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

#ifndef ULTIMA_ULTIMA0_DATA_PLAYER_H
#define ULTIMA_ULTIMA0_DATA_PLAYER_H

#include "common/rect.h"
#include "ultima/ultima0/data/defines.h"

namespace Ultima {
namespace Ultima0 {

struct COORD {
	int x = 0, y = 0;
};

struct PLAYER {
	char  Name[MAX_NAME + 1];	// Player Name
	COORD World;				// World map position
	COORD Dungeon;				// Dungeon map position
	COORD DungDir;				// Dungeon direction facing
	char  Class = '?';			// Player class (F or M)
	int   HPGain = 0;			// HPs gained in dungeon
	int	  Level = 0;			// Dungeon level, 0 = world map
	int	  Skill = 0;			// Skill level
	int	  Task = 0;				// Task set (-1 = none)
	int	  TaskCompleted = 0;	// Task completed
	int	  LuckyNumber = 42;		// Value used for seeding
	int	  Attributes = 6;		// Number of attributes
	int	  Objects = 6;			// Number of objects
	int   Attr[MAX_ATTR];		// Attribute values
	double Object[MAX_OBJ];		// Object counts

	PLAYER();

	/**
	 * Sets enhanced mode, which allows for a larger inventory
	 */
	void setIsEnhanced(bool isEnh);

	/**
	 * Sets up a demo character
	 */
	void setDemo();

	/**
	 * Sets up a debug character
	 */
	void setDebug();
};

} // namespace Ultima0
} // namespace Ultima

#endif
