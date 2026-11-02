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

#ifndef NUVIE_SCRIPT_COMMON_SCRIPT_RANDOM_H
#define NUVIE_SCRIPT_COMMON_SCRIPT_RANDOM_H

#include "ultima/ultima6/core/nuvie_defs.h"
#include "ultima/ultima6/core/game.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {

/** Random number from low to high inclusive */
inline int randomRange(int low, int high) {
	return low + NUVIE_RAND() % (high - low + 1);
}

} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima

#endif
