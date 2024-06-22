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

#ifndef WASTELAND_WASTELAND1_FILES_HUFFMAN_NODE_H
#define WASTELAND_WASTELAND1_FILES_HUFFMAN_NODE_H

#include "common/array.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Huffman {

struct HuffmanNode {
private:
	HuffmanNode *_left = nullptr;		// Left sub-node
	HuffmanNode *_right = nullptr;		// Right sub-node
	HuffmanNode *_parent = nullptr;		// Parent node
	
	bool _key = false;				// Node key: false = left, true = right
	Common::Array<bool> _fullKey;	// The full node key
	int _payload = 0;				// The payload
	int _probability = 0;			// The probability
	int _id = 0;					// The serial Id
	static int _nextId;

public:
	/**
	 * Constructor
	 */
	HuffmanNode(HuffmanNode *left, HuffmanNode *right);

	/**
	 * Constructor
	 */
	HuffmanNode(int payload) : _payload(payload) {}

	/**
	 * Constructor
	 */
	HuffmanNode(int payload, int probability) :
		_payload(payload), _probability(probability) {
	}

	HuffmanNode *getLeft() const { return _left; }
	HuffmanNode *getRight() const { return _right; }
	HuffmanNode *getParent() const { return _parent; }
	int getPayload() const { return _payload; }
	int getProbability() const { return _probability; }
	bool getKey() const { return _key; }

	/**
	 * Returns the full key of this node (Beginning from the root).
	 * @return The full key
	 */
	const Common::Array<bool> &getFullKey();

	static int compareNodes(const HuffmanNode *a, const HuffmanNode *b);
};

} // namespace Huffman
} // namespace Wasteland1
} // namespace Wasteland

#endif
