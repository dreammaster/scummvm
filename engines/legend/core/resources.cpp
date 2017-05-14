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

#include "legend/core/resources.h"
#include "legend/legend.h"

namespace Legend {

#define MAX_TEXT_CACHE_SIZE 256

void TextIndexEntry::load(Common::SeekableReadStream &f) {
	_count = f.readUint16LE();
	_size = f.readUint32LE();
}

/*-------------------------------------------------------------------*/

Resources::Resources(LegendEngine *vm) : _currentTextIndexNum(-1) {
	loadResourceIndex();
	loadText();
}

void Resources::loadResourceIndex() {
	if (!_datFile.open("legend.dat"))
		error("Could not find titanic.dat data file");

	uint headerId = _datFile.readUint32BE();
	uint version = _datFile.readUint16LE();
	if (headerId != MKTAG('L', 'G', 'N', 'D') || version < 1)
		error("Invalid data file");

	// Read in entries
	uint offset, size;
	char c;
	String resourceName;
	for (;;) {
		offset = _datFile.readUint32LE();
		size = _datFile.readUint32LE();
		if (offset == 0 && size == 0)
			break;

		String resName;
		while ((c = _datFile.readByte()) != '\0')
			resName += c;

		_resources[resName] = ResourceEntry(offset, size);
	}
}

void Resources::loadText() {
	String prefix = g_vm->getGameFilePrefix();
	String filename = String::format("%sstr.dat", prefix.c_str());
	_textFile.open(filename);

	int count = _textFile.readUint16LE();
	_textList.resize(count);
	for (int idx = 0; idx < count; ++idx) {
		_textList[idx].load(_textFile);
	}

	// Set up file offsets for each text list section data, and skip over the data
	for (uint idx = 0; idx < _textList.size(); ++idx) {
		_textList[idx]._indexOffset = _textFile.pos();
		_textFile.skip(_textList[idx]._count * 2);
	}

	// Read in the second data block
	count = _textFile.readUint16LE();
	_decryptionTable.resize(count);
	for (int idx = 0; idx < count; ++idx)
		_decryptionTable[idx] = _textFile.readSint16LE();

	// Read in the word list
	count = _textFile.readUint16LE();

	_wordList.resize(count);
	if (count) {
		// Read in the data offsets
		Common::Array<uint> offsets;
		offsets.resize(count);
		for (int idx = 0; idx < count; ++idx)
			offsets[idx] = _textFile.readUint16LE();

		// Read in the raw text for the words
		count = _textFile.readUint16LE();
		char *data = new char[count];
		_textFile.read(data, count);

		// Extract the strings from the data block
		for (uint idx = 0; idx < _wordList.size(); ++idx)
			_wordList[idx] = String(data + offsets[idx]);

		delete[] data;
	}

	// Set up file offsets for each text list section raw values, and skip over the data
	for (uint idx = 0; idx < _textList.size(); ++idx) {
		_textList[idx]._dataOffset = _textFile.pos();
		_textFile.skip(_textList[idx]._size);
	}
}

Common::String Resources::getMessage(uint id) {
	assert((id >> 16) >= 0xF000 && (id >> 16) <= 0xF100);
	id &= 0xFFFFFFF;
	int sectionNum = id >> 16;
	int subNum = id & 0xffff;

	// Check for the presence of the Id in the cache
	for (uint idx = 0; idx < _textCache.size(); ++idx) {
		// Check for text entry
		if (_textCache[idx]._id == id) {
			// If the entry isn't already at the front of the list, put it there
			if (idx > 0) {
				_textCache.insert_at(0, _textCache[idx]);
				_textCache.remove_at(idx + 1);

				return _textCache[0]._text.c_str();
			}
		}
	}

	// Ensure that the section number is within bounds
	if (sectionNum >= (int)_textList.size())
		return nullptr;

	// If we're not already using the correct index, load it up
	TextIndexEntry &indexEntry = _textList[sectionNum];
	if (_currentTextIndexNum != sectionNum) {
		_currentTextIndexNum = sectionNum;
		_textFile.seek(indexEntry._indexOffset);

		_currentTextIndexVals.resize(indexEntry._count);
		for (int idx = 0; idx < indexEntry._count; ++idx)
			_currentTextIndexVals[idx] = _textFile.readUint16LE();
	}

	// Ensure that the sub-section number is within range
	if (subNum > indexEntry._count)
		return nullptr;

	// Figure out the offset of the data for the message
	size_t offset = indexEntry._dataOffset;
	for (int idx = 0; idx < subNum; ++subNum)
		offset += _currentTextIndexVals[idx];

	// Prepare a new cache entry, re-using the oldest if there are now
	// too many entries in the cache
	if (_textCache.size() <= MAX_TEXT_CACHE_SIZE)
		_textCache.resize(_textCache.size() + 1);
	TextEntry &cacheEntry = _textCache[_textCache.size() - 1];
	cacheEntry._id = id;

	// Read in the data
	_textFile.seek(offset);
	Common::SeekableReadStream *textStream = _textFile.readStream(_currentTextIndexVals[subNum]);

	// Decode the text
	cacheEntry._text = decompressText(textStream);
	delete textStream;

	// Return the text
	return cacheEntry._text.c_str();
}

#define EOS (stream->pos() >= stream->size())

String Resources::decompressText(Common::SeekableReadStream *stream) {
	String result;
	int nextVal = -1;
	int bitCounter = 0;
	byte srcByte = 0;

	while (!EOS && nextVal) {
		int decryptIndex = _decryptionTable.size() - 2;

		// Iterate across the decryption table to get the next value
		do {
			if (!bitCounter) {
				if (EOS) {
					// Reached end
					nextVal = 0;
					break;
				}

				srcByte = stream->readByte();
				bitCounter = 8;
			}

			decryptIndex = _decryptionTable[decryptIndex | (srcByte & 1)];
			srcByte >>= 1;
			--bitCounter;
		} while (decryptIndex >= 0);
		if (EOS && !nextVal)
			break;

		nextVal = -1 - decryptIndex;

		if (!nextVal) {
			// Ignore null ending terminator
		} else if (nextVal < 0x80 || (nextVal - 0x80) >= (int)_wordList.size()) {
			// Printable character
			result += (char)nextVal;
		} else {
			// An entry in the words list
			result += _wordList[nextVal - 0x80];
		}
	}

	return result;
}

#undef EOS

Common::SeekableReadStream *Resources::getResource(const String &name) {
	ResourceEntry resEntry = _resources[name];

	_datFile.seek(resEntry._offset);

	return (resEntry._size > 0) ? _datFile.readStream(resEntry._size) :
		new Common::MemoryReadStream(nullptr, 0);
}

} // End of namespace Legend
