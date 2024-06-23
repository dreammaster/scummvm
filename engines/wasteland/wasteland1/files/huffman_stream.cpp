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

#include "wasteland/wasteland1/files/huffman_stream.h"

namespace Wasteland {
namespace Wasteland1 {

int HuffmanStream::read() {
	byte bit;
	Huffman::HuffmanNode *node;
	int payload;

	node = _tree->getRootNode();
	while ((payload = node->getPayload()) == -1) {
		bit = _bitStream->readBit();
		if (bit == -1)
			error("Unexpected end of stream while reading Huffman data");

		if (bit == 0) {
			node = node->getLeft();
		} else {
			node = node->getRight();
		}
	}

	return payload;
}

} // namespace Wasteland1
} // namespace Wasteland
