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

#ifndef LEGEND_RESOURCES_H
#define LEGEND_RESOURCES_H

#include "common/scummsys.h"

namespace Legend {

struct StringPtr {
	bool _isString;
	const char *_str;
	uint32 _val;

	StringPtr(const char *s) : _isString(true), _str(s), _val(0) {}
	StringPtr(uint32 v) : _isString(false), _val(v), _str(nullptr) {}
	operator const char *() const;
};

} // namespace Legend

#endif
