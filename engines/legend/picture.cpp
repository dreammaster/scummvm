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

#include "legend/picture.h"
#include "legend/legend.h"

namespace Legend {

#define INDEXES_COUNT 2

PicFile::PicFile() : _currentFileNumber(-1), _index(nullptr),
		_paletteCheck(false), _skipPreload(false), _val1(0), _val2(0) {
	_indexes.resize(INDEXES_COUNT);
}

PicFile::~PicFile() {
	_file.close();
}

bool PicFile::open(uint pictureNum, uint frameNum) {
	int fileNumber = (pictureNum >> 8) & 0xff;
	int picIndex = pictureNum & 0xff;
	
	// Check if the desired picture is in another PIC file
	if (fileNumber != _currentFileNumber) {
		// Close any previously open PIC file
		if (_file.isOpen())
			_file.close();

		// Open the new picture file
		_file.open(FILETYPE_PIC, fileNumber);
	
		// Load in the index
		loadIndex(fileNumber);
		assert(_index);
	}

	if (picIndex >= (int)_index->size())
		// No such entry in the file
		return false;

	const IndexEntry &entry = (*_index)[picIndex];
	_currentPic = entry;
	_file.seek(entry._offset);

	if (_paletteCheck && (entry._flags & PIC_HAS_PALETTE))
		return false;

	if (frameNum > entry._frameCount)
		frameNum = entry._frameCount;

	if (frameNum > 0) {
		// Get the offset of the frame
		_file.seek(entry._offset + (frameNum - 1) * 4);
		_val1 = _file.readUint16LE();
		_val2 = _file.readUint16LE();
	} else {
		_val1 = 0;
		_val2 = 0;
	}

	if (!_skipPreload) {
		_file.seek(entry._offset + entry._frameCount * 4);
		if (entry._flags & PIC_HAS_PALETTE) {
			// Read in the game palette for the picture
			_file.read(_vm->_screen->_gamePalette, PALETTE_SIZE);
			_vm->_screen->_picPalette = true;
		}

		return entry._width > 0 && entry._height > 0;
	}

	return true;
}

Picture *PicFile::load(uint pictureNum, uint frameNum) {
	if (!open(pictureNum, frameNum))
		return nullptr;

	// Create the new picture instance
	Picture *pic = new Picture;
	pic->create(_currentPic._width, _currentPic._height);

	// Return the picture
	return pic;
}

void PicFile::loadIndex(int fileNumber) {
	for (uint idx = 0; idx < _indexes.size(); ++idx) {
		if (_indexes[idx]._refCounter)
			_indexes[idx]._refCounter++;
	}

	for (uint idx = 0; idx < _indexes.size(); ++idx) {
		if (_indexes[idx]._fileNumber == fileNumber) {
			// Found the index for the file already loaded
			_indexes[idx]._refCounter = 1;
			_index = &_indexes[idx]._entries;
			return;
		}
	}

	// The index isn't present, so set up the slot with the least number of 
	// references to hold the new index to load
	Index &index = _indexes[findIndexesSlot()];
	index._fileNumber = fileNumber;
	index._refCounter = 1;
	_index = &index._entries;

	// Load in the first index array
	index._entries.load(_file);

	uint idx;
	for (idx = 0; idx < index._entries.size(); ++idx) {
		IndexEntry &e = index._entries[idx];
		uint frameListSize = e._frameCount * 4;
		uint palSize = (e._flags & PIC_HAS_PALETTE) ? PALETTE_SIZE : 0;

		// Calculate each entry's size by getting either the following entry's
		// offset, or to the end of the file for the last entry
		if (idx == (index._entries.size() - 1)) {
			e._size = _file.size() - e._offset - palSize - frameListSize;
		} else {
			e._size = index._entries[idx + 1]._offset - e._offset - palSize - frameListSize;
		}
	}
}

int PicFile::findIndexesSlot() const {
	if (_indexes[0]._refCounter == 0)
		// First time loading a PIC file, so simply use first slot
		return 0;

	int minRefConter = _indexes[0]._refCounter;
	int minIndex = 0;

	// Scan through the remaining slots to find the one with the smallest reference
	// count (i.e. the one that's least used), and thus most suitable for replacement
	for (uint idx = 1; idx < _indexes.size(); ++idx) {
		if (_indexes[idx]._refCounter < minRefConter) {
			minIndex = idx;
			minRefConter = _indexes[idx]._refCounter;
		}
	}

	return minIndex;
}

/*-------------------------------------------------------------------*/

bool PicFile::IndexEntry::load(Common::SeekableReadStream &s) {
	_offset = s.readUint32LE();
	if (!_offset)
		return false;
	
	_frameCount = s.readByte();
	_flags = s.readByte();
	_width = s.readUint16LE();
	_height = s.readUint16LE();
	_fieldA = s.readByte();
	_fieldB = s.readByte();
	return true;
}

/*-------------------------------------------------------------------*/

void PicFile::IndexEntries::load(Common::SeekableReadStream &s) {
	clear();

	// Load up to 256 index entries
	IndexEntry entry;
	int idx = 0;
	while (idx++ < 256 && entry.load(s))
		push_back(entry);
}

/*-------------------------------------------------------------------*/

const byte PictureDecoder::_DATA1[256] = {
	11, 12, 12, 12, 12, 12, 12, 12, 12,  8,  7, 12, 12,  7, 12, 12,
	12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 13, 12, 12, 12, 12, 12,
	 4, 10,  8, 12, 10, 12, 10,  8,  7,  7,  8,  9,  7,  6,  7,  8,
	 7,  6,  7,  7,  7,  7,  8,  7,  7,  8,  8, 12, 11,  7,  9, 11,
	12,  6,  7,  6,  6,  5,  7,  8,  8,  6, 11,  9,  6,  7,  6,  6,
	 7, 11,  6,  6,  6,  7,  9,  8,  9,  9, 11,  8, 11,  9, 12,  8,
	12,  5,  6,  6,  6,  5,  6,  6,  6,  5, 11,  7,  5,  6,  5,  5,
	 6, 10,  5,  5,  5,  5,  8,  7,  8,  8, 10, 11, 11, 12, 12, 12,
	13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
	13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
	13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
	12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
	12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
	12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
	13, 12, 13, 13, 13, 12, 13, 13, 13, 12, 13, 13, 13, 13, 12, 13,
	13, 13, 12, 12, 12, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13
};

const byte PictureDecoder::_DATA2[16] = {
	3, 2, 3, 3, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 7, 7
};

const byte PictureDecoder::_DATA3[16] = {
	0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8
};

const byte PictureDecoder::_DATA4[32] = {
	0, 0,  1, 0,  2, 0,  3, 0,  4, 0,  5, 0,   6, 0, 7, 0,
	8, 0, 10, 0, 14, 0, 22, 0, 38, 0, 70, 0, 134, 0, 6, 1 
};

const byte PictureDecoder::_DATA5[64] = {
	2, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8
};

const byte PictureDecoder::_REF1[16] = {
	5, 3, 1, 6, 10, 2, 12, 20, 4, 24, 8, 48, 16, 32, 64, 0
};

const byte PictureDecoder::_REF2[64] = {
	  3,  13,   5, 25,   9, 17,   1, 62,  30, 46,  14, 54,  22, 38,   6, 58,
	 26,  42,  10, 50,  18, 34,  66,  2, 124, 60,  92, 28, 108, 44,  76, 12,
	116,  52,  84, 20, 100, 36,  68,  4, 120, 56,  88, 24, 104, 40,  72,  8,
	240, 112, 176, 48, 208, 80, 144, 16, 224, 96, 160, 32, 192, 64, 128,  0
};

PictureDecoder::PictureDecoder() {
	_inputStream = nullptr;
	_outputStream = nullptr;
	_field0 = 0;
	_field2 = 0;
	_field4 = 0;
	_field6 = 0;
	_field8 = 0;
	_fieldA = 0;
	_fieldC = 0;
}

PictureDecoder::~PictureDecoder() {
	// The streams should only be active during decoding
	assert(!_inputStream && !_outputStream);
}

int PictureDecoder::decode(Common::SeekableReadStream *inStream, Common::WriteStream *outStream) {
	_inputStream = inStream;
	_outputStream = outStream;

	int result = decodeInner();
	_inputStream = nullptr;
	_outputStream = nullptr;

	return result;
}

int PictureDecoder::decodeInner() {
	if (_inputStream->size() <= 4)
		// Input stream is too small
		return 3;

	_field2 = _inputStream->readByte();
	_field6 = _inputStream->readByte();
	_fieldA = _inputStream->readByte();

	if (_field6 < 4 || _field6 > 6)
		return 1;

	_field8 = 0xFFFF >> (16 - _field6);
	if (_field2) {
		if (_field2 != 1)
			return 2;
		Common::copy(_DATA1, _DATA1 + 256, _array3);
	}

	Common::copy(_DATA2, _DATA2 + 16, _array5);
	setupArray(_array5, 16, _REF1, _array2);

	Common::copy(_DATA3, _DATA3 + 16, _array6);
	Common::copy(_DATA4, _DATA4 + 32, _array7);
	Common::copy(_DATA5, _DATA5 + 64, _array4);
	setupArray(_array4, 64, _REF2, _array1);

	int result = unpack();
	return (result == 0x306) ? 4 : 0;
}

void PictureDecoder::setupArray(const byte *src, int srcSize, const byte *ref, byte *dest) {
	for (int idx = srcSize - 1; idx >= 0; --idx, ++src, ++ref) {
		int jump = 1 << *src;	
		for (int offset = *ref; offset < 256; offset += jump)
			dest[offset] = idx;
	}
}

int PictureDecoder::unpack() {
	// TODO
	return 0;
}

/*-------------------------------------------------------------------*/

int Picture::decode(Common::SeekableReadStream *inStream) {
	assert(!empty());

	Graphics::Surface s = this->getSubArea(Common::Rect(0, 0, this->w, this->h));
	Common::MemoryWriteStream *outStream = new Common::MemoryWriteStream(
		(byte *)s.getPixels(), s.w * s.h);

	int result = PictureDecoder::decode(inStream, outStream);
	delete outStream;
	return result;
}

} // End of namespace Legend
