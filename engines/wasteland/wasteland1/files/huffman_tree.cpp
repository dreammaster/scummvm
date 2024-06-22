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

HuffmanTree *HuffmanTree::create(Common::SeekableReadStream *src) {
	HuffmanTree *tree = new HuffmanTree();
	tree->createTree(src);
	return tree;
}

HuffmanTree::~HuffmanTree() {
	for (auto it = _index.begin(); it != _index.end(); ++it)
		delete it->_value;
}

HuffmanTree *HuffmanTree::create(const Common::Array<byte> &bytes) {
	Common::MemoryReadStream stream(&bytes[0], bytes.size());
	return create(&stream);
}

void HuffmanTree::createTree(Common::SeekableReadStream *stream) {
	int *counter;
	int b;
	HuffmanNode *node, *left, *right;
	Common::SortedArray<HuffmanNode *, const HuffmanNode *> nodes(HuffmanNode::compareNodes);

	// Read the stream and build the counter array
	counter = new int[256];
	while ((b = stream->readByte()) != -1)
	{
		counter[b]++;
	}

	// Initialize the nodes with it's payloads and probabilities
	for (int i = 0; i < 256; i++) {
		if (counter[i] == 0)
			continue;

		node = new HuffmanNode(i, counter[i]);
		nodes.insert(node);
		_index[i] = node;
	}

	// Repeat until only one node (the root node) is left
	while (nodes.size() > 1) {
		// Take the last two nodes from the list
		left = nodes.back();
		nodes.remove(left);
		right = nodes.back();
		nodes.remove(right);

		// Create the parent node for them and add it to the list
		node = new HuffmanNode(left, right);
		nodes.insert(node);
	}

	// Save the root node
	_rootNode = nodes.front();
}

} // namespace Huffman
} // namespace Wasteland1
} // namespace Wasteland
