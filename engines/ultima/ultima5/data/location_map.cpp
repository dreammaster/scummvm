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

#include "common/file.h"
#include "ultima/ultima5/data/location_map.h"
#include "ultima/ultima5/data/location_tables.h"

namespace Ultima {
namespace Ultima5 {
namespace Data {

constexpr int BLOCK_SIZE = LOCATION_SIZE * LOCATION_SIZE;
constexpr byte FIRST_BASEMENT_LEVEL = 0x80;
constexpr byte TILE_BRIDGE_FIRST = 0x48;
constexpr int MAX_LOCATION_ID = LOCATION_FILE_LOCATIONS * 4;

static const char *const LOCATION_FILES[] = { "towne.dat", "dwelling.dat", "castle.dat", "keep.dat" };

bool LocationMap::load(byte mapId, byte level) {
	_loaded = false;
	if (mapId < 1 || mapId > MAX_LOCATION_ID)
		return false;

	int block = LOCATION_FIRST_BLOCK[mapId - 1] + level;
	if (level >= FIRST_BASEMENT_LEVEL)
		block -= 0x100;
	if (block < 0)
		return false;

	Common::File f;
	if (!f.open(LOCATION_FILES[(mapId - 1) / LOCATION_FILE_LOCATIONS]) || !f.seek(block * BLOCK_SIZE) ||
			f.read(_tiles, BLOCK_SIZE) != BLOCK_SIZE)
		return false;

	_bridgeCount = 0;
	for (int y = 0; y < LOCATION_SIZE; ++y) {
		for (int x = 0; x < LOCATION_SIZE; ++x) {
			const byte tile = getTile(x, y);

			if ((tile & 0xFE) == TILE_BRIDGE_FIRST && _bridgeCount < LOCATION_BRIDGE_MAX) {
				_bridgeX[_bridgeCount] = x;
				_bridgeY[_bridgeCount] = y;
				_bridgeTile[_bridgeCount] = tile;
				++_bridgeCount;
			}
		}
	}

	_mapId = mapId;
	_level = level;
	_loaded = true;
	return true;
}

} // namespace Data
} // namespace Ultima5
} // namespace Ultima
