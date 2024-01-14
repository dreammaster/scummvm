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
#include "wasteland/gfx/tiles.h"

namespace Wasteland {
namespace Gfx {

#define TILE_SIZE (TILE_W / 2 * TILE_H)

bool Tiles::load(const Common::Path &filename) {
	return _decoder.loadTiles(filename);
}

void Tiles::drawTile(Surface *dst, uint tileId, int x, int y) const {
	assert(x >= 0 && y >= 0 && x <= (dst->w - TILE_W) && y <= (dst->h - TILE_H));

	const Graphics::ManagedSurface *src = _decoder.getSurface();
	Common::Rect r(0, tileId * 16, 16, (tileId + 1) * 16);

	dst->blitFrom(*src, r, Common::Point(x, y));
}

} // namespace Gfx
} // namespace Wasteland
