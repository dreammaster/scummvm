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

#ifndef WASTELAND_GFX_TILES_H
#define WASTELAND_GFX_TILES_H

#include "common/array.h"
#include "graphics/managed_surface.h"
#include "wasteland/gfx/surface.h"
#include "wasteland/gfx/image_decoder.h"

namespace Wasteland {
namespace Gfx {

#define TILE_W 16
#define TILE_H 16

/**
 * Manages a tileset
 */
class Tiles {
private:
	ImageDecoder _decoder;

public:
	/**
	 * Loads the tile set from the specified file
	 */
	bool load(const Common::Path &filename);

	/**
	 * Draw a tile
	 */
	void drawTile(Surface *dst, uint tileId, int x, int y, bool transparent) const;
};

} // namespace Gfx
} // namespace Wasteland

#endif
