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

constexpr int MAX_CACHE_SIZE = 32;

struct MESSAGE {
private:
	bool _isPointer;
	uint32 _id = 0;
	const char *_ptr = nullptr;

public:
	MESSAGE(uint32 id) : _isPointer(false), _id(id) {}
	MESSAGE(const char *ptr) : _isPointer(true), _ptr(ptr) {}

	operator const char *();
	operator const char *() const;
	operator uint32() const;
	bool isPointer() const {
		return _isPointer;
	}
};

class Strings {
	struct SectionEntry {
		Common::Array<int> _stringOffsets;
		size_t _size = 0;
		Common::Array<char> _data;
	};

private:
	Common::File _file;
	Common::Array<SectionEntry> _sections;
	Common::Array<uint16> _sectionEntries;
	Common::Array<char> _commonData;
	Common::Array<const char *> _commonStrings;
	Common::Array<int16> _huffmanTable;
	int _maxEntryCount = 0;
	int _totalStrings = 0;
	int _sectionsOffset = 0, _huffmanOffset = 0;
	int _currentSection = -1;
	int _stringOffset = 0;

public:
	Strings(const Common::Path &filename);

	const char *getMessage(const MESSAGE &msg);
};

} // namespace Parser
} // namespace Early
} // namespace Legend

#endif
