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

#ifndef ULTIMA5_GFX_TILESET_H
#define ULTIMA5_GFX_TILESET_H

#include "graphics/managed_surface.h"
#include "ultima/ultima5/data/data.h"

namespace Ultima {
namespace Ultima5 {
namespace Gfx {

constexpr int TILE_SIZE = 16;
constexpr int TILE_COUNT = 512;

/**
 * The 16x16 tiles of TILES.16 or TILES.4, kept a byte per pixel
 */
class Tileset {
private:
	byte _pixels[TILE_COUNT * TILE_SIZE * TILE_SIZE] = {};

public:
	/**
	 * Loads the tiles
	 * @returns		False if they couldn't be loaded
	 */
	bool load(Data::RenderMode mode);

	/**
	 * Draws a tile, clipped to the surface
	 */
	void drawTile(Graphics::ManagedSurface &dst, int x, int y, int tile) const;
};

} // namespace Gfx
} // namespace Ultima5
} // namespace Ultima

#endif
