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
	Common::File f;
	if (!f.open(filename))
		return false;

	// Get the size of the font
	_count = f.readUint16LE();
	_data = new byte[_count * TILE_SIZE];
	f.read(_data, _count * TILE_SIZE);
	return true;
}

Tiles::~Tiles() {
	delete[] _data;
}

void Tiles::drawTile(Surface *dst, uint tileId, int x, int y) const {
	assert(x >= 0 && y >= 0 && x <= (dst->w - TILE_W) && y <= (dst->h - TILE_H));
	const byte *src = &_data[tileId * TILE_SIZE];

	for (int yCtr = 0; yCtr < TILE_H; ++yCtr) {
		byte *dest = (byte *)dst->getBasePtr(x, y + yCtr);

		// The 16 pixels are stored in two pixels per byte
		uint64 srcVal = READ_LE_UINT64(src);
		src += 8;

		for (int xCtr = 0; xCtr < (TILE_W / 2); ++xCtr, srcVal >>= 8) {
			*dest++ = (srcVal >> 4) & 0xf;
			*dest++ = srcVal & 0xf;
		}
	}

	dst->addDirtyRect(Common::Rect(x, y, x + TILE_W, y + TILE_H));
}

} // namespace Gfx
} // namespace Wasteland
