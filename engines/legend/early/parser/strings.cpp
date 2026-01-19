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
	Common::File file;
	Common::Array<char> commonData;
	Common::Array<const char *> commonStrings;
	Common::Array<int16> huffmanTable;

	if (!file.open(filename))
		error("Could not open - %s", filename.baseName().c_str());

	// Load headers
	typedef Common::Array<int> IntArray;
	Common::Array<IntArray> sectionStrSizes;

	_sections.resize(file.readUint16LE());
	sectionStrSizes.resize(_sections.size());

	for (auto &sse : sectionStrSizes) {
		sse.resize(file.readUint16LE());
		(void)file.readUint16LE();		// Skip size
	}

	// Read in the string offsets for each section
	for (auto &section : sectionStrSizes) {
		for (uint i = 0; i < section.size(); ++i)
			section[i] = file.readUint16LE();
	}

	// Read in the Huffman table
	huffmanTable.resize(file.readUint16LE());
	for (uint idx = 0; idx < huffmanTable.size(); ++idx)
		huffmanTable[idx] = file.readSint16LE();

	// Read in the common strings data
	int commonStringsCount = file.readUint16LE();
	assert(commonStringsCount <= 128);

	uint16 strOffsets[128];
	if (commonStringsCount > 0) {
		for (int i = 0; i < commonStringsCount; ++i)
			strOffsets[i] = file.readUint16LE();

		commonData.resize(file.readUint16LE());
		file.read(&commonData[0], commonData.size());

		commonStrings.resize(commonStringsCount);
		for (int i = 0; i < commonStringsCount; ++i)
			commonStrings[i] = &commonData[strOffsets[i]];
	}

	// Decompress all the strings
	for (uint i = 0; i < sectionStrSizes.size(); ++i) {
		const IntArray &src = sectionStrSizes[i];
		Common::StringArray &dest = _sections[i];
		dest.resize(src.size());

		for (uint j = 0; j < src.size(); ++j) {
			// Decompress string data
			Common::SeekableReadStream *srcStream = file.readStream(src[j]);
			auto *decompressed = Huffman::decompress(*srcStream, &huffmanTable[0], huffmanTable.size(),
				&commonStrings[0], commonStrings.size());

			// Get string
			char *buf = new char[decompressed->size() + 1];
			decompressed->read(buf, decompressed->size());
			buf[decompressed->size()] = '\0';

			dest[j] = Common::String(buf);

			delete[] buf;
		}
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
	if (sectionNum >= _sections.size() || sectionSubnum >= _sections[sectionNum].size())
		return nullptr;

	// Get the designated string
	return _sections[sectionNum][sectionSubnum].c_str();
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
