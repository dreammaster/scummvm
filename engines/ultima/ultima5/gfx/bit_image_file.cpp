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

#include "common/endian.h"
#include "ultima/ultima5/gfx/bit_image_file.h"
#include "ultima/ultima5/gfx/lzw.h"

namespace Ultima {
namespace Ultima5 {
namespace Gfx {

bool BitImageFile::load(const char *name) {
	_count = 0;
	if (!loadLzwFile(name, _data) || _data.size() < 2)
		return false;

	const uint count = READ_LE_UINT16(&_data[0]);
	if (_data.size() < 2 + count * 2)
		return false;

	_count = count;
	return true;
}

const byte *BitImageFile::imagePtr(uint idx) const {
	assert(idx < _count);
	return &_data[READ_LE_UINT16(&_data[2 + idx * 2])];
}

int BitImageFile::getWidth(uint idx) const {
	return READ_LE_UINT16(imagePtr(idx));
}

int BitImageFile::getHeight(uint idx) const {
	return READ_LE_UINT16(imagePtr(idx) + 2);
}

void BitImageFile::draw(Graphics::ManagedSurface &dst, uint idx, int x, int y, byte color) const {
	const byte *image = imagePtr(idx);
	const int width = READ_LE_UINT16(image);
	const int height = READ_LE_UINT16(image + 2);
	const int stride = (width + 7) / 8;

	for (int yy = 0; yy < height; ++yy) {
		if (y + yy < 0 || y + yy >= dst.h)
			continue;

		const byte *row = image + 4 + yy * stride;
		for (int xx = 0; xx < width; ++xx) {
			if ((row[xx >> 3] & (0x80 >> (xx & 7))) && x + xx >= 0 && x + xx < dst.w)
				*(byte *)dst.getBasePtr(x + xx, y + yy) = color;
		}
	}
}

} // namespace Gfx
} // namespace Ultima5
} // namespace Ultima
