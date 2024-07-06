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

#include "common/array.h"
#include "common/memstream.h"
#include "wasteland/wasteland1/files/huffman_tree.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Huffman {

HuffmanTree *HuffmanTree::load(Common::ReadStream *src) {
	HuffmanTree *tree = new HuffmanTree();
	BitStream *bitStream = dynamic_cast<BitStream *>(src);

	if (bitStream) {
		tree->loadTree(bitStream);
	} else {
		BitStream streamWrapper(src);
		tree->loadTree(&streamWrapper);
	}

	return tree;
}

HuffmanTree::~HuffmanTree() {
	for (auto it = _index.begin(); it != _index.end(); ++it)
		delete it->_value;
}

void HuffmanTree::loadTree(BitStream *stream) {
	_rootNode = loadNode(stream);
}

HuffmanNode *HuffmanTree::loadNode(BitStream *stream) {
	int b;
	HuffmanNode *node;

	if (stream->readBit() == 0) {
		HuffmanNode *left, *right;
		left = loadNode(stream);
		stream->readBit();
		right = loadNode(stream);
		return new HuffmanNode(left, right);

	} else {
		b = stream->readByte();
		node = new HuffmanNode(b);
		this->_index[b] = node;
		return node;
	}
}

} // namespace Huffman
} // namespace Wasteland1
} // namespace Wasteland
