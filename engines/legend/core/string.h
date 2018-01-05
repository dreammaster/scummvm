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

#ifndef LEGEND_STRING_H
#define LEGEND_STRING_H

#include "common/array.h"
#include "common/str.h"

namespace Legend {

/**
 * String class that derives from the standard ScummVM String class
 */
class String : public Common::String {
public:
	String() : Common::String() {}
	String(const char *str) : Common::String(str) {}
	String(const char *str, uint32 len) : Common::String(str, len) {}
	String(const char *beginP, const char *endP) : Common::String(beginP, endP) {}
	String(const Common::String &str) : Common::String(str) {}
	explicit String(char c) : Common::String(c) {}

	/**
	 * Initialises a string from a given resource Id
	 */
	String(uint id);

	/**
	 * Capitalizes the string
	 */
	void capitalize();
};

typedef Common::Array<String> StringArray;

} // End of namespace Legend

#endif
