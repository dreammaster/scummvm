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

#include "common/array.h"
#include "ultima/ultima5/gfx/tileset.h"
#include "ultima/ultima5/gfx/lzw.h"

namespace Ultima {
namespace Ultima5 {
namespace Gfx {

constexpr int TILE_BYTES = TILE_SIZE * TILE_SIZE / 2;

bool Tileset::load() {
	Common::Array<byte> data;
	if (!loadLzwFile("TILES.16", data) || data.size() < TILE_COUNT * TILE_BYTES)
		return false;

	for (int i = 0; i < TILE_COUNT * TILE_SIZE * TILE_SIZE; ++i)
		_pixels[i] = (i & 1) ? (data[i >> 1] & 0x0F) : (data[i >> 1] >> 4);

	return true;
}

void Tileset::drawTile(Graphics::ManagedSurface &dst, int x, int y, int tile) const {
	assert(tile >= 0 && tile < TILE_COUNT);
	const byte *src = &_pixels[tile * TILE_SIZE * TILE_SIZE];

	for (int yy = 0; yy < TILE_SIZE; ++yy, src += TILE_SIZE) {
		if (y + yy < 0 || y + yy >= dst.h)
			continue;

		for (int xx = 0; xx < TILE_SIZE; ++xx) {
			if (x + xx >= 0 && x + xx < dst.w)
				*(byte *)dst.getBasePtr(x + xx, y + yy) = src[xx];
		}
	}
}

} // namespace Gfx
} // namespace Ultima5
} // namespace Ultima
