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
#include "wasteland/fod/gfx/font.h"

namespace Wasteland {
namespace FOD {
namespace Gfx {

bool Font::load(const Common::Path &filename) {
	Common::File f;
	if (!f.open(filename))
		return false;

	// Get the size of the font
	_count = f.readUint16LE();
	_data = new byte[_count * 32];
	f.read(_data, _count * 32);
	return true;
}

Font::~Font() {
	delete[] _data;
}

void Font::drawChar(Graphics::Surface *dst, uint32 chr, int x, int y, uint32 color) const {
	assert(chr < _count);
	assert(x >= 0 && y >= 0 && x <= (dst->w - 8) && y <= (dst->h - 8));
	const byte *src = &_data[chr * 32];

	for (int yCtr = 0; yCtr < 8; ++yCtr) {
		byte *dest = (byte *)dst->getBasePtr(x, y + yCtr);

		// The 8 pixels of the font character are stored in 4 nibbles
		uint32 srcVal = READ_LE_UINT32(src);
		src += 4;
		if (color == -1)
			// Drawing inverse colors
			srcVal ^= 0xffffffff;

		for (int xCtr = 0; xCtr < 4; ++xCtr, srcVal >>= 8) {
			*dest++ = (srcVal >> 4) & 0xf;
			*dest++ = srcVal & 0xf;
		}
	}
}

} // namespace Gfx
} // namespace FOD
} // namespace Wasteland
