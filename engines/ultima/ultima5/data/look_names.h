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

#ifndef ULTIMA5_DATA_LOOK_NAMES_H
#define ULTIMA5_DATA_LOOK_NAMES_H

#include "common/array.h"
#include "common/str.h"

namespace Ultima {
namespace Ultima5 {
namespace Data {

/**
 * The names of what can be seen on the map, from look2.dat: one for each
 * map tile, and one for each object. Terrain that needs special words
 * to describe it is named "*"
 */
class LookNames {
private:
	Common::Array<byte> _data;

	Common::String nameAt(int index) const;

public:
	/**
	 * Loads the names
	 * @returns		False if they couldn't be loaded
	 */
	bool load();

	Common::String tileName(byte tile) const {
		return nameAt(tile);
	}

	Common::String objectName(byte tile) const {
		return nameAt(256 + tile);
	}
};

} // namespace Data
} // namespace Ultima5
} // namespace Ultima

#endif
