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
#include "common/memstream.h"
#include "wasteland/fod/core/decoder.h"
#include "wasteland/fod/gfx/image_decoder.h"

namespace Wasteland {
namespace FOD {
namespace Gfx {

bool ImageDecoder::load(const Common::Path &name, uint16 w, uint16 h) {
	Common::File f;
	Common::MemoryWriteStreamDynamic tmp(DisposeAfterUse::YES);

	if (!f.open(name))
		return false;

	// Decompress the stream
	Decoder::loadStream(f, tmp);
	assert((tmp.size() * 2) == (w * h));

	// Create the surface and copy the data in
	_surface.create(w, h);
	Common::MemoryReadStream tmp2(tmp.getData(), tmp.size());
	copyPixelsToSurface(tmp2);

	return true;
}

bool ImageDecoder::loadWelcome(const Common::Path &name) {
	Common::File f;
	Common::MemoryWriteStreamDynamic tmp(DisposeAfterUse::YES);

	if (!f.open(name))
		return false;

	// Decompress the stream
	Decoder::loadStream(f, tmp);
	assert(tmp.size() == 4236);

	// Create the surface and copy the data in
	_surface.create(96, 88);
	Common::MemoryReadStream tmp2(tmp.getData() + 9, tmp.size() - 9);
	copyPixelsToSurface(tmp2);

	return true;
}

bool ImageDecoder::loadTiles(const Common::Path &name) {
	Common::File f;
	Common::MemoryWriteStreamDynamic tmp(DisposeAfterUse::YES);

	if (!f.open(name))
		return false;

	// Decompress the stream
	Decoder::loadStream(f, tmp);
	assert((tmp.size() % 128) == 0);

	// Create the surface and copy the data in
	_surface.create(16, (tmp.size() / 128) * 16);
	Common::MemoryReadStream tmp2(tmp.getData(), tmp.size());
	copyPixelsToSurface(tmp2);

	return true;
}

void ImageDecoder::copyPixelsToSurface(Common::SeekableReadStream &src) {
	assert(!_surface.empty());

	for (int y = 0; y < _surface.h; ++y) {
		byte *dest = (byte *)_surface.getBasePtr(0, y);

		for (int x = 0; x < _surface.w / 2; ++x) {
			byte v = src.readByte();
			*dest++ = v >> 4;
			*dest++ = v & 0xf;
		}
	}
}

} // namespace Gfx
} // namespace FOD
} // namespace Wasteland
