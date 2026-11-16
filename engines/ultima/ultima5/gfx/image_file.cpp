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
#include "ultima/ultima5/gfx/image_file.h"
#include "ultima/ultima5/gfx/lzw.h"

namespace Ultima {
namespace Ultima5 {
namespace Gfx {

bool ImageFile::load(const char *name) {
	_count = 0;
	if (!loadLzwFile(name, _data) || _data.size() < 2)
		return false;

	const uint count = READ_LE_UINT16(&_data[0]);
	if (_data.size() < 2 + count * 4)
		return false;

	_count = count;
	return true;
}

const byte *ImageFile::imagePtr(uint idx) const {
	assert(idx < _count);
	return &_data[READ_LE_UINT16(&_data[2 + idx * 4])];
}

const byte *ImageFile::maskPtr(uint idx) const {
	assert(idx < _count);
	const uint offset = READ_LE_UINT16(&_data[2 + idx * 4 + 2]);
	return offset ? &_data[offset] : nullptr;
}

int ImageFile::getWidth(uint idx) const {
	return READ_LE_UINT16(imagePtr(idx));
}

int ImageFile::getHeight(uint idx) const {
	return READ_LE_UINT16(imagePtr(idx) + 2);
}

void ImageFile::draw(Graphics::ManagedSurface &dst, uint idx, int x, int y, int flags) const {
	const byte *image = imagePtr(idx);
	const byte *mask = maskPtr(idx);
	const int width = READ_LE_UINT16(image);
	const int height = READ_LE_UINT16(image + 2);
	const int stride = ((width + 7) / 8) * 4;
	const int maskStride = mask ? (READ_LE_UINT16(mask) + 7) / 8 : 0;
	const bool vflip = (flags & IMAGE_VFLIP) != 0;
	const bool hflip = (flags & IMAGE_HFLIP) != 0;

	for (int yy = 0; yy < height; ++yy) {
		const int srcY = vflip ? height - 1 - yy : yy;
		const int dstY = y + yy + (vflip ? 1 : 0);
		if (dstY < 0 || dstY >= dst.h)
			continue;

		const byte *row = image + 4 + srcY * stride;
		const byte *maskRow = mask ? mask + 4 + srcY * maskStride : nullptr;

		for (int xx = 0; xx < width; ++xx) {
			if (maskRow && (maskRow[xx >> 3] & (0x80 >> (xx & 7))))
				continue;

			const int dstX = x + (hflip ? width - 1 - xx : xx);
			if (dstX < 0 || dstX >= dst.w)
				continue;

			const byte b = row[xx >> 1];
			*(byte *)dst.getBasePtr(dstX, dstY) = (xx & 1) ? (b & 0x0F) : (b >> 4);
		}
	}
}

} // namespace Gfx
} // namespace Ultima5
} // namespace Ultima
