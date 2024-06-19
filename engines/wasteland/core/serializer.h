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

#ifndef MM_WASTELAND_CORE_SERIALIZER_H
#define MM_WASTELAND_CORE_SERIALIZER_H

#include "common/serializer.h"

namespace Wasteland {

/**
 * Derived serializer class
 */
class Serializer : public Common::Serializer {
public:
	Serializer(Common::SeekableReadStream *in) : Common::Serializer(in, nullptr) {}
	Serializer(Common::WriteStream *out) : Common::Serializer(nullptr, out) {}

	/**
	 * Serializes a 3 byte integer to or from the serializer
	 */
	void syncAsInt3(int &val);

	/**
	 * Serializes a string using a fixed size
	 */
	void syncAsString(Common::String &str, size_t size);
};

} // namespace Wasteland

#endif
