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
	File f(filename);
	Common::Array<HeaderEntry> headers;

	// Load headers
	headers.resize(f.readUint16LE());
	for (HeaderEntry &he : headers) {
		he._stringsCount = f.readUint16LE();
		he._size = f.readUint16LE();

		_maxEntryCount = MAX(_maxEntryCount, he._stringsCount);
		_totalStrings += he._stringsCount;
	}

	_strOffset1 = f.pos();
	f.seek(_totalStrings * 2, SEEK_CUR);

	// Read in the table
	Common::Array<int16> huffmanTable;
	huffmanTable.resize(f.readUint16LE());
	for (uint idx = 0; idx < huffmanTable.size(); ++idx)
		huffmanTable[idx] = f.readSint16LE();

	int stringsCount = f.readUint16LE();
	assert(stringsCount <= 128);

	uint16 indexTable[128];
	if (stringsCount > 0) {
		for (int i = 0; i < stringsCount; ++i)
			indexTable[i] = f.readUint16LE();

		_tableData.resize(f.readUint16LE());
		f.read(&_tableData[0], _tableData.size());

		_table.resize(stringsCount);
		for (int i = 0; i < stringsCount; ++i)
			_table[i] = &_tableData[indexTable[i]];
	}

	_strOffset2 = f.pos();
}

} // namespace Parser
} // namespace Early
} // namespace Legend
