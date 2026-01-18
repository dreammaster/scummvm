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

#ifndef LEGEND_EARLY_PARSER_STRINGS_H
#define LEGEND_EARLY_PARSER_STRINGS_H

#include "common/array.h"
#include "common/file.h"
#include "legend/utils/string.h"

namespace Legend {
namespace Early {
namespace Parser {

class Strings {
private:
	Common::File _file;
	Common::Array<char> _commonData;
	Common::Array<const char *> _commonStrings;
	int _maxEntryCount = 0;
	int _totalStrings = 0;
	int _strOffset1 = 0, _strOffset2 = 0;

public:
	Strings(const Common::Path &filename);
};

} // namespace Parser
} // namespace Early
} // namespace Legend

#endif
