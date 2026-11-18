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

#ifndef ULTIMA5_LOGIC_GAME_RANDOM_H
#define ULTIMA5_LOGIC_GAME_RANDOM_H

#include "common/scummsys.h"

namespace Ultima {
namespace Ultima5 {

/**
 * The game's own random numbers. Parts of the game seed it from the date
 * and expect to get the same numbers each time, such as which of
 * the trees in a town have been cut down
 */
class GameRandom {
private:
	uint16 _seed = 0;

public:
	void seed(uint16 value) {
		_seed = value;
	}

	/**
	 * Returns a number from min to max, inclusive
	 */
	int range(int min, int max) {
		uint16 value = _seed + 0x9248;
		value = (uint16)((value >> 3) | (value << 13));
		value ^= 0x9248;
		value += 0x11;
		_seed = value;

		return (value & 0x7FFF) % (uint16)(max - min + 1) + min;
	}
};

} // namespace Ultima5
} // namespace Ultima

#endif
