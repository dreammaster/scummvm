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
#include "wasteland/gfx/image_decoder.h"

namespace Wasteland {
namespace Gfx {

#define IMAGE_BUFFER_COUNT 0x1000

bool ImageDecoder::load(const Common::Path &name, uint16 w, uint16 h) {
	Common::File f;
	Common::MemoryWriteStreamDynamic tmp(DisposeAfterUse::YES);

	if (!f.open(name))
		return false;

	// Decompress the stream
	loadStream(f, tmp);
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
	loadStream(f, tmp);
	assert(tmp.size() == 4236);

	// Create the surface and copy the data in
	_surface.create(96, 88);
	Common::MemoryReadStream tmp2(tmp.getData() + 9, tmp.size() - 9);
	copyPixelsToSurface(tmp2);

	return true;
}

void ImageDecoder::loadStream(Common::ReadStream &src, Common::WriteStream &dest) {
	int size = src.readUint32LE();
	int bufferOffset = 0xfee;
	int bitCtr = 1;
	byte srcByte = 0, val = 0;
	int seq1 = 0, seq2 = 0, srcOffset = 0, srcCount = 0;
	byte buffer[IMAGE_BUFFER_COUNT];
	bool bit = false;

	Common::fill(&buffer[0], &buffer[IMAGE_BUFFER_COUNT], 0x20);

	for (;;) {
		// A single byte will use it's 8 bits to indicate where the following
		// 8 'entries' are individual bytes to copy, or a sequence from the buffer
		if (--bitCtr == 0) {
			srcByte = src.readByte();
			bitCtr = 8;
		}

		bit = (srcByte & 1) != 0;
		srcByte >>= 1;
		if (bit) {
			// Output single byte
			val = src.readByte();
			dest.writeByte(val);

			if (--size == 0)
				return;
			buffer[bufferOffset] = val;
			bufferOffset = (bufferOffset + 1) & 0xfff;
		} else {
			// Sequence from buffer
			seq1 = src.readByte();
			seq2 = src.readByte();
			srcCount = (seq2 & 0xf) + 3;
			seq2 >>= 4;
			srcOffset = (seq2 << 8) | seq1;

			while (srcCount-- > 0) {
				val = buffer[srcOffset];
				srcOffset = (srcOffset + 1) & 0xfff;
				dest.writeByte(val);

				if (--size == 0)
					return;
				buffer[bufferOffset] = val;
				bufferOffset = (bufferOffset + 1) & 0xfff;
			}
		}
	}
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
} // namespace Wasteland
