
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

#ifndef WASTELAND_ARRAY_H
#define WASTELAND_ARRAY_H

#include "common/array.h"
#include "common/str.h"

namespace Wasteland {

template<class T>
class Array : public Common::Array<T> {
public:
	Array<T>() : Common::Array<T>() {}

	int indexOf(T elem) const {
		for (uint i = 0; i < this->size(); ++i) {
			if (this->operator[](i) == elem)
				return i;
		}

		return -1;
	}

	void remove(T elem) {
		int index = this->indexOf(elem);
		if (index != -1)
			this->remove_at(index);
	}

	bool contains(T elem) const {
		int index = this->indexOf(elem);
		return index != -1;		
	}
};

class StringArray : public Array<Common::String> {
public:
	/**
	 * Splits a passed string based on a separator character
	 */
	void split(const Common::String &src, char separator);
};

} // namespace Wasteland

#endif
