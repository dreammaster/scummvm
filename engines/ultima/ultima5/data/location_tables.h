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

#ifndef ULTIMA5_DATA_LOCATION_TABLES_H
#define ULTIMA5_DATA_LOCATION_TABLES_H

#include "common/scummsys.h"

namespace Ultima {
namespace Ultima5 {
namespace Data {

constexpr int LOCATION_COUNT = 40;

// Towns, dwellings, castles and keeps, which have their maps in four files
// of 8 locations each
constexpr int LOCATION_FILE_LOCATIONS = 8;

extern const byte LOCATION_FIRST_BLOCK[LOCATION_FILE_LOCATIONS * 4];
extern const byte LOCATION_ENTRANCE_X[LOCATION_COUNT];
extern const byte LOCATION_ENTRANCE_Y[LOCATION_COUNT];

// The names of the locations, or null for the ones that aren't announced
extern const char *const LOCATION_NAMES[LOCATION_COUNT];

} // namespace Data
} // namespace Ultima5
} // namespace Ultima

#endif
