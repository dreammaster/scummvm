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
#include "legend/early/utils/huffman.h"
#include "legend/early/engine.h"

namespace Legend {
namespace Early {
namespace Parser {

Strings::Strings(const Common::Path &filename) {
	if (!_file.open(filename))
		error("Could not open - %s", filename.baseName().c_str());

	// Load headers
	_sections.resize(_file.readUint16LE());
	for (SectionEntry &he : _sections) {
		he._stringOffsets.resize(_file.readUint16LE());
		he._size = _file.readUint16LE();

		_maxEntryCount = MAX<int>(_maxEntryCount, he._stringOffsets.size());
		_totalStrings += he._stringOffsets.size();
	}

	_sectionsOffset = _file.pos();

	// Read in the string offsets for each section
	_file.seek(_sectionsOffset);
	for (auto &section : _sections) {
		for (uint i = 0; i < section._stringOffsets.size(); ++i)
			section._stringOffsets[i] = _file.readUint16LE();
	}

	// Read in the huffman table
	_huffmanTable.resize(_file.readUint16LE());
	for (uint idx = 0; idx < _huffmanTable.size(); ++idx)
		_huffmanTable[idx] = _file.readSint16LE();

	// Read in the common strings data
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

	_huffmanOffset = _file.pos();

	// Read in the huffman data for each section and decompress it
	for (auto &section : _sections) {
		Common::SeekableReadStream *srcStream = _file.readStream(section._size);

		// Decompress string data for section
		auto *decompressed = Huffman::decompress(*srcStream, &_huffmanTable[0], _huffmanTable.size(),
			&_commonStrings[0], _commonStrings.size());
		section._data.resize(decompressed->size());
		decompressed->read(&section._data[0], decompressed->size());
		delete decompressed;
	}
}

const char *Strings::getMessage(const MESSAGE &msg) {
	uint16 msgId;
	uint16 sectionNum, sectionSubnum;

	if (msg.isPointer())
		return msg;

	// Decode message ID
	msgId = msg;
	sectionNum = msgId >> 10;
	sectionSubnum = msgId & 0x03FF;

	// For out of range values, return a nullptr
	if (sectionNum >= _sections.size() || sectionSubnum >= _sections[sectionNum]._stringOffsets.size())
		return nullptr;

	// Get the designated string
	const auto &section = _sections[sectionNum];
	return &section._data[section._stringOffsets[sectionSubnum]];
}

/*------------------------------------------------------------------------*/

MESSAGE::operator const char *() const {
	assert(_isPointer);
	return _ptr;
}

MESSAGE::operator const char *() {
	if (!_isPointer) {
		assert(_id);
		_ptr = g_engine->_strings->getMessage(*this);
		_isPointer = true;
	}

	return _ptr;
}

MESSAGE::operator uint32() const {
	assert(!_isPointer);
	return _id;
}

} // namespace Parser
} // namespace Early
} // namespace Legend
