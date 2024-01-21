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

#include "common/algorithm.h"
#include "wasteland/core/decoder.h"

namespace Wasteland {

#define DECODER_BUFFER_COUNT 0x1000

void Decoder::loadStream(Common::ReadStream &src, Common::WriteStream &dest) {
	int size = src.readUint32LE();
	int bufferOffset = 0xfee;
	int bitCtr = 1;
	byte srcByte = 0, val = 0;
	int seq1 = 0, seq2 = 0, srcOffset = 0, srcCount = 0;
	byte buffer[DECODER_BUFFER_COUNT];
	bool bit = false;

	Common::fill(&buffer[0], &buffer[DECODER_BUFFER_COUNT], 0x20);

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

} // namespace Wasteland
