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

#ifndef WIZARDRY_DATA_SCENARIO_H
#define WIZARDRY_DATA_SCENARIO_H

#include "common/scummsys.h"

namespace Wizardry {

struct TSCNTOC {
	char _title[40] = {};	// "$PROVING GROUNDS..." string
	uint16 _numLevels = 0;	// usually 10
	uint16 _monsters = 0;	// block index of monster table
	uint16 _items = 0;		// block index of item table
	uint16 _spells = 0;		// block index of spell table
	uint16 _maze[10] = {};	// block indices for each level's maze data
	uint16 _messages = 0;	// block for in-game messages
	uint16 _specials = 0;	// block for special events/traps
	uint16 _rewards = 0;		// block for reward/treasure tables
	uint8 _reserved[512 - 0x5E] = {};  // fill remainder of block (optional)

	void load(const byte *src);
};

} // namespace Wizardry

#endif
