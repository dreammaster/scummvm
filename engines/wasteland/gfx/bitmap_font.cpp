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
#include "wasteland/gfx/bitmap_font.h"

namespace Wasteland {
namespace Gfx {

bool BitmapFont::load(const Common::Path &filename) {
	Common::File f;
	if (!f.open(filename))
		return false;

	// Get the size of the font
	_count = f.readUint16LE();
	_data.resize(_count * 23);
	f.read(&_data[0], _count * 32);
}

void BitmapFont::drawChar(Graphics::Surface *dst, uint32 chr, int x, int y, uint32 color) const {
	const byte *src = &_data[chr * 32];
	byte v = 0;

	for (int yCtr = 0; yCtr < _size.y; ++yCtr) {
		byte *dstP = (byte *)dst->getBasePtr(x, y + yCtr);

		for (int xCtr = 0; xCtr < _size.x; ++xCtr, ++dstP, v >>= 1) {
			if ((xCtr % 8) == 0)
				v = *src++;

			if (v & 1)
				*dstP = color;
		}
	}
}

} // namespace Gfx
} // namespace Wasteland
