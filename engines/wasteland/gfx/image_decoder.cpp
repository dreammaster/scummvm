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
#include "wasteland/gfx/image_decoder.h"

namespace Wasteland {
namespace Gfx {

#define IMAGE_BUFFER_COUNT 0x1000

bool ImageDecoder::load(const Common::Path &name, uint16 w, uint16 h) {
	Common::File f;

	if (!f.open(name))
		return false;

	loadStream(f, w, h);
	return true;
}

#define OUTPUT_PIXEL(VAL) *dest++ = VAL >> 4; *dest++ = VAL & 0xF

void ImageDecoder::loadStream(Common::SeekableReadStream &stream, uint16 w, uint16 h) {
	int size = stream.readUint32LE();
	int bufferOffset = 0xfee;
	int bitCtr = 1;
	byte srcByte = 0, val = 0;
	int seq1 = 0, seq2 = 0, srcOffset = 0, srcCount = 0;
	byte buffer[IMAGE_BUFFER_COUNT];
	bool bit = false;

	assert((size * 2) == (w * h));
	_decompressedSize = size;
	_surface.create(w, h);
	byte *dest = (byte *)_surface.getPixels();

	Common::fill(&buffer[0], &buffer[IMAGE_BUFFER_COUNT], 0x20);

	for (;;) {
		// A single byte will use it's 8 bits to indicate where the following
		// 8 'entries' are individual bytes to copy, or a sequence from the buffer
		if (--bitCtr == 0) {
			srcByte = stream.readByte();
			bitCtr = 8;
		}

		bit = (srcByte & 1) != 0;
		srcByte >>= 1;
		if (bit) {
			// Output single byte
			val = stream.readByte();
			OUTPUT_PIXEL(val);

			if (--size == 0)
				return;
			buffer[bufferOffset] = val;
			bufferOffset = (bufferOffset + 1) & 0xfff;
		} else {
			// Sequence from buffer
			seq1 = stream.readByte();
			seq2 = stream.readByte();
			srcCount = (seq2 & 0xf) + 3;
			seq2 >>= 4;
			srcOffset = (seq2 << 8) | seq1;

			while (srcCount-- > 0) {
				val = buffer[srcOffset];
				srcOffset = (srcOffset + 1) & 0xfff;
				OUTPUT_PIXEL(val);

				if (--size == 0)
					return;
				buffer[bufferOffset] = val;
				bufferOffset = (bufferOffset + 1) & 0xfff;
			}
		}
	}
}

} // namespace Gfx
} // namespace Wasteland
