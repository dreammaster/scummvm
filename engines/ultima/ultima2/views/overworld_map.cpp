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

#include "ultima/ultima2/views/overworld_map.h"
#include "ultima/ultima2/ultima2.h"

namespace Ultima {
namespace Ultima2 {
namespace Views {

void OverworldMap::draw() {
	auto &map = g_engine->_map;
	auto s = getSurface();
	s.clear();

	prepareMapForDrawing();

	// Draw the visible map contents
	for (int oy = 0; oy < Data::MAP_VISIBLE_HEIGHT; oy++) {
		for (int ox = 0; ox < Data::MAP_VISIBLE_WIDTH; ox++) {
			uint8 tileId = map._mapTilesId[oy][ox];
			if (!(tileId & 0x80)) {
				const Graphics::ManagedSurface &tileImg = g_engine->_tiles[tileId];
				s.blitFrom(tileImg, Common::Point(ox * Data::TILE_WIDTH, oy * Data::TILE_HEIGHT));
			}
		}
	}

	// Copy rendered tile IDs for next frame's dirty comparison
	for (int oy = 0; oy < Data::MAP_VISIBLE_HEIGHT; oy++)
		for (int ox = 0; ox < Data::MAP_VISIBLE_WIDTH; ox++)
			map._priorTileIds[oy][ox] = map._mapTilesId[oy][ox] & 0x7F;
}

void OverworldMap::prepareMapForDrawing() {
	// Load the map if it's not already active
	const auto &player = g_engine->_player;
	auto &map = g_engine->_map;

	//int mapOffsetX = 0, mapOffsetY = 0;
	int mapLeft, mapTop;

	if (map._outsideMapTile != -1) {
		// Non-wrapping draw (used for indoor maps/dungeons with a border tile)
		mapLeft = (map._mapX - 9) + 1;
		mapTop = (map._mapY - 5) + 1;

		for (int oy = 0; oy < Data::MAP_VISIBLE_HEIGHT; oy++) {
			for (int ox = 0; ox < Data::MAP_VISIBLE_WIDTH; ox++) {
				uint8 x = mapLeft + ox;
				uint8 y = mapTop + oy;

				uint8 tile;
				if (x >= 64 || y >= 64) {
					// Out of bounds: fill with the designated outside tile
					tile = map._outsideMapTile;
				} else {
					tile = map._tiles[y][x];
					tile = (tile >> 1) & 0xFE;
				}

				if (tile == map._priorTileIds[oy][ox]) {
					if (tile == 0) {
						if (map._flag1 == 0xFF)
							tile |= 0x80;
					} else {
						if (tile != 46)
							tile |= 0x80;
					}
				}

				map._mapTilesId[oy][ox] = tile / 2;
			}
		}

	} else {
		// Normal (wrapping) draw - overworld map
		mapTop = (map._mapY - 5) & 63;
		mapLeft = (map._mapX - 9) & 63;

		for (int oy = 0; oy < Data::MAP_VISIBLE_HEIGHT; oy++) {
			for (int ox = 0; ox < Data::MAP_VISIBLE_WIDTH; ox++) {
				uint8 x = (mapLeft + ox) & 63;
				uint8 y = (mapTop + oy) & 63;
				uint8 tile = map._tiles[y][x];
				tile = (tile >> 1) & 0xFE;

				if (tile == map._priorTileIds[oy][ox]) {
					if (tile == 0) {
						if (map._flag1 == 0xFF)
							tile |= 0x80;
					} else {
						if (tile != 46)
							tile |= 0x80;
					}
				}

				map._mapTilesId[oy][ox] = tile / 2;
			}
		}
	}

	// Player is at center of the viewport. Save tiles around the player
	map._tilePlayerCenter = map._mapTilesId[Data::MAP_VISIBLE_CENTER_Y][Data::MAP_VISIBLE_CENTER_X];
	map._tilePlayerUp = map._mapTilesId[Data::MAP_VISIBLE_CENTER_Y - 1][Data::MAP_VISIBLE_CENTER_X];
	map._tilePlayerDown = map._mapTilesId[Data::MAP_VISIBLE_CENTER_Y + 1][Data::MAP_VISIBLE_CENTER_X];
	map._tilePlayerLeft = map._mapTilesId[Data::MAP_VISIBLE_CENTER_Y][Data::MAP_VISIBLE_CENTER_X - 1];
	map._tilePlayerRight = map._mapTilesId[Data::MAP_VISIBLE_CENTER_Y][Data::MAP_VISIBLE_CENTER_X + 1];

	// Place the player tile at the center
	map._mapTilesId[Data::MAP_VISIBLE_CENTER_Y][Data::MAP_VISIBLE_CENTER_X] = map._playerTileId;
}

} // namespace Views
} // namespace Ultima2
} // namespace Ultima
