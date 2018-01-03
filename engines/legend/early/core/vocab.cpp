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

#include "common/file.h"
#include "legend/early/core/vocab.h"
#include "legend/early/core/huffman.h"

namespace Legend {
namespace Early {

Vocab::Vocab() {
	char vocabData[18842];
	uint16 huffmanTable[256];

	Common::File f;
	if (!f.open("vocab.dat"))
		error("Unable to open vocab");

	// Read in the table
	size_type nodeCount = f.readUint16LE();
	for (size_type idx = 0; idx < nodeCount; ++idx)
		huffmanTable[idx] = f.readUint16LE();

	size_type streamSize = f.readUint16LE();
	Huffman::decompress((byte *)vocabData, &f, huffmanTable, nodeCount);

	size_type vocabCount = f.readUint16LE();
	reserve(vocabCount);

	for (size_type vocabIdx = 0; vocabIdx < vocabCount; ++vocabIdx) {
		VocabEntry &v = (*this)[vocabIdx];
		uint16 vocabOffset = f.readUint16LE();
		uint16 flags = f.readUint16LE();
		v = VocabEntry(&vocabData[vocabOffset], flags);
	}

	vocabCount = f.readUint16LE();

	for (size_type vocabIdx = 0; vocabIdx < vocabCount; ++vocabIdx) {
		uint16 vocabNum = f.readUint16LE();
		uint16 linkCount = f.readUint16LE() & 0xff;
		VocabEntry &v = (*this)[vocabNum];

		for (uint idx = 0; idx < linkCount; ++idx) {
			v._field6 = f.readUint16LE();
			v._field8 = f.readUint16LE() & 0xff;
		}
	}

	f.close();
}

} // End of namespace Early
} // End of namespaceCo Legend
