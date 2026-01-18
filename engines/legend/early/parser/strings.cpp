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

#include "legend/early/parser/strings.h"
#include "legend/utils/file.h"

namespace Legend {
namespace Early {
namespace Parser {

struct HeaderEntry {
	int _stringsCount = 0;
	size_t _size = 0;
};

Strings::Strings(const Common::Path &filename) {
	if (!_file.open(filename))
		error("Could not open - %s", filename.baseName().c_str());

	// Load headers
	Common::Array<HeaderEntry> headers;
	headers.resize(_file.readUint16LE());
	for (HeaderEntry &he : headers) {
		he._stringsCount = _file.readUint16LE();
		he._size = _file.readUint16LE();

		_maxEntryCount = MAX(_maxEntryCount, he._stringsCount);
		_totalStrings += he._stringsCount;
	}

	_strOffset1 = _file.pos();
	_file.seek(_totalStrings * 2, SEEK_CUR);

	// Read in the table
	Common::Array<int16> huffmanTable;
	huffmanTable.resize(_file.readUint16LE());
	for (uint idx = 0; idx < huffmanTable.size(); ++idx)
		huffmanTable[idx] = _file.readSint16LE();

	int commonStringsCount = _file.readUint16LE();
	assert(commonStringsCount <= 128);

	uint16 strOffsets[128];
	if (commonStringsCount > 0) {
		for (int i = 0; i < commonStringsCount; ++i)
			strOffsets[i] = _file.readUint16LE();

		_commonData.resize(_file.readUint16LE());
		_file.read(&_commonData[0], _commonData.size());

		_commonStrings.resize(commonStringsCount);
		for (int i = 0; i < commonStringsCount; ++i)
			_commonStrings[i] = &_commonData[strOffsets[i]];
	}

	_strOffset2 = _file.pos();
}

} // namespace Parser
} // namespace Early
} // namespace Legend
