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

#ifndef WASTELAND_WASTELAND1_FILES_HUFFMAN_TREE_H
#define WASTELAND_WASTELAND1_FILES_HUFFMAN_TREE_H

#include "common/hashmap.h"
#include "common/stream.h"
#include "wasteland/wasteland1/files/huffman_node.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Huffman {

class HuffmanTree {
private:
	HuffmanNode *_rootNode = nullptr;
	Common::HashMap<int, HuffmanNode *> _index;

	HuffmanTree() {}

	/**
	 * Creates the root node of the Huffman Tree.
	 *
	 * @param stream	The input stream to build the root node for.
	 */
	void createTree(Common::SeekableReadStream *stream);

public:
	~HuffmanTree();

	static HuffmanTree *create(Common::SeekableReadStream *src);

	/**
	 * Creates a Huffman tree from a byte array.
	 * @param bytes The bytes to build the Huffman tree for
	 * @return The Huffman tree
	 */
	static HuffmanTree *create(const Common::Array<byte> &bytes);


	/**
	 * Returns the root node of the Huffman tree.
	 *
	 * @return The root node
	 */
	HuffmanNode *getRootNode() const {
		return _rootNode;
	}


	/**
	 * Returns the Huffman node for the given payload.
	 *
	 * @param payload	The payload
	 * @return The Huffman node
	 */
	HuffmanNode *getNode(int payload) const {
		return _index[payload];
	}

};

} // namespace Huffman
} // namespace Wasteland1
} // namespace Wasteland

#endif
