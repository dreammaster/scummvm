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

#include "wasteland/wasteland1/files/huffman_node.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Huffman {

int HuffmanNode::_nextId = 0;

HuffmanNode::HuffmanNode(HuffmanNode *left, HuffmanNode *right) {
	_left = left;
	if (_left != nullptr) {
		_left->_parent = this;
		_left->_key = false;
	}

	_right = right;
	if (_right != nullptr) {
		_right->_parent = this;
		_right->_key = true;
	}

	_payload = -1;
	_probability = _left->_probability + _right->_probability;
}

const Common::Array<bool> &HuffmanNode::getFullKey() {
	const HuffmanNode *current;
	int count;
	int index;

	// If there is already a cached full key then use it
	if (_fullKey.empty()) {
		// Find out how many key bits are needed for the full key
		count = 0;
		current = this;
		while (current->_parent != nullptr) {
			count++;
			current = current->_parent;
		}

		// Build the full key
		current = this;
		index = 0;
		while (current->_parent != nullptr) {
			_fullKey[index] = current->_key;
			current = current->_parent;
			index++;
		}
	}

	return _fullKey;
}

int HuffmanNode::compareTo(const HuffmanNode &rhs) const {
	if (_probability < rhs._probability) {
		return 1;
	} else if (_probability > rhs._probability) {
		return -1;
	} else {
		return _id == rhs._id;
	}
}

} // namespace Huffman
} // namespace Wasteland1
} // namespace Wasteland
