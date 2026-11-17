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

#ifndef ULTIMA5_DATA_WORLD_MAP_H
#define ULTIMA5_DATA_WORLD_MAP_H

#include "common/scummsys.h"
#include "ultima/ultima5/data/savegame.h"

namespace Ultima {
namespace Ultima5 {
namespace Data {

constexpr int WORLD_SIZE = 256;
constexpr int CHUNK_SIZE = 16;
constexpr int CHUNK_COUNT = (WORLD_SIZE / CHUNK_SIZE) * (WORLD_SIZE / CHUNK_SIZE);

constexpr byte TILE_WATER = 1;
constexpr byte TILE_POISON = 4;
constexpr byte TILE_GRASS = 5;
constexpr byte TILE_HUT = 0x10;
constexpr byte TILE_CAVE = 0x16;
constexpr byte TILE_DUNGEON = 0x18;
constexpr byte TILE_SHRINE = 0x19;
constexpr byte TILE_RUINS = 0x1A;
constexpr byte TILE_SEALED_DUNGEON = 0xDF;

/**
 * One of the two great maps, Britannia or the underworld. They are 256x256
 * tiles, made of 16x16 chunks, and wrap around at the edges
 */
class WorldMap {
private:
	byte _tiles[WORLD_SIZE * WORLD_SIZE] = {};
	bool _loaded = false;
	bool _underworld = false;

	/**
	 * Seals the dungeon entrances that aren't open yet, and ruins the shrines that have been destroyed
	 */
	void applyQuestState(const Savegame &savegame);

public:
	/**
	 * Loads Britannia or the underworld
	 * @returns		False if the map file couldn't be loaded
	 */
	bool load(bool underworld, const Savegame &savegame);

	bool isLoaded(bool underworld) const {
		return _loaded && _underworld == underworld;
	}

	/**
	 * Forgets the map, such as when the quests have changed it
	 */
	void unload() {
		_loaded = false;
	}

	byte getTile(int x, int y) const {
		return _tiles[((y & (WORLD_SIZE - 1)) << 8) | (x & (WORLD_SIZE - 1))];
	}

	void setTile(int x, int y, byte tile) {
		_tiles[((y & (WORLD_SIZE - 1)) << 8) | (x & (WORLD_SIZE - 1))] = tile;
	}
};

} // namespace Data
} // namespace Ultima5
} // namespace Ultima

#endif
