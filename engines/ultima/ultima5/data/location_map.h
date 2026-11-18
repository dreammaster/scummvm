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

#ifndef ULTIMA5_DATA_LOCATION_MAP_H
#define ULTIMA5_DATA_LOCATION_MAP_H

#include "common/scummsys.h"

namespace Ultima {
namespace Ultima5 {
namespace Data {

constexpr int LOCATION_SIZE = 32;
constexpr int LOCATION_BRIDGE_MAX = 32;

// What the map shows beyond its edges, and for what can't be seen
constexpr byte TILE_DARKNESS = 0xFF;

/**
 * One level of the map of a town, castle, keep or dwelling, which is 32x32 tiles
 */
class LocationMap {
private:
	byte _tiles[LOCATION_SIZE * LOCATION_SIZE] = {};
	bool _loaded = false;
	byte _mapId = 0;
	byte _level = 0;

	// The drawbridge and gate tiles, which change with the time of day
	int _bridgeCount = 0;
	byte _bridgeX[LOCATION_BRIDGE_MAX] = {};
	byte _bridgeY[LOCATION_BRIDGE_MAX] = {};
	byte _bridgeTile[LOCATION_BRIDGE_MAX] = {};

public:
	/**
	 * Loads a level of a location
	 * @param mapId		The number of the location, from 1
	 * @param level		Zero for the ground, up for the floors above and 255 for
	 *					the basement, going down from there
	 * @returns			False if the map couldn't be loaded
	 */
	bool load(byte mapId, byte level);

	/**
	 * Forgets the map, such as when a game is loaded
	 */
	void unload() {
		_loaded = false;
	}

	bool isLoaded(byte mapId, byte level) const {
		return _loaded && _mapId == mapId && _level == level;
	}

	/**
	 * Returns a tile, or darkness for anywhere outside the map
	 */
	byte getTile(int x, int y) const {
		return (x < 0 || y < 0 || x >= LOCATION_SIZE || y >= LOCATION_SIZE) ? TILE_DARKNESS :
			_tiles[y * LOCATION_SIZE + x];
	}

	void setTile(int x, int y, byte tile) {
		assert(x >= 0 && y >= 0 && x < LOCATION_SIZE && y < LOCATION_SIZE);
		_tiles[y * LOCATION_SIZE + x] = tile;
	}

	int getBridgeCount() const {
		return _bridgeCount;
	}

	byte getBridgeX(int idx) const {
		return _bridgeX[idx];
	}

	byte getBridgeY(int idx) const {
		return _bridgeY[idx];
	}

	byte getBridgeTile(int idx) const {
		return _bridgeTile[idx];
	}
};

} // namespace Data
} // namespace Ultima5
} // namespace Ultima

#endif
