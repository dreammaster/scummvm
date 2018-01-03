/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "legend/early/core/huffman.h"
#include "common/util.h"

namespace Legend {
namespace Early {

Common::MemoryWriteStreamDynamic *Huffman::decompress(Common::SeekableReadStream &stream,
		const int16 *huffmanTable, size_t nodeCount, byte **symbols, int symbolsCount) {
	int symIndex = -1, bitCtr = 0;
	int16 huffBase;
	byte dataByte = 0;
	Common::MemoryWriteStreamDynamic *dest = new 	Common::MemoryWriteStreamDynamic(DisposeAfterUse::YES);

	while (stream.pos() < stream.size() || symIndex) {
		huffBase = nodeCount - 2;

		// Inner loop to navigate huffman table until a terminator entry is found
		do {
			if (stream.pos() >= stream.size() && !symIndex)
				goto outer;

			if (!bitCtr) {
				// Get the next byte to process
				if (stream.pos() >= stream.size()) {
					symIndex = 0;
					goto outer;
				}

				dataByte = stream.readByte();
				bitCtr = 8;
			}

			// Get next bit, and move to next relevant node
			huffBase = huffmanTable[huffBase | (dataByte & 1)];
			dataByte >>= 1;
			--bitCtr;
		} while (huffBase >= 0);

		symIndex = ABS(huffBase) - 1;
		if (symIndex < 0x80 || (symIndex - 0x80) >= symbolsCount) {
			dest->writeByte(symIndex);
		} else {
			byte *symPtr = symbols[symIndex - 0x80];
			for (; *symPtr; ++symPtr)
				dest->writeByte(*symPtr);			
		}
		
	outer:
		continue;
	}

	return dest;
}

} // End of namespace Early
} // End of namespaceCo Legend
