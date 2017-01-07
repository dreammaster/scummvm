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
			byte pal[PALETTE_SIZE];
			_file.read(pal, PALETTE_SIZE);
			_vm->_screen->setGamePalette(pal);
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

	// Decode the picture contents
	int result = pic->decode(&_file);
	assert(!result);

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

const uint16 PictureDecoder::_SETUP_DATA[256] = {
	1168, 4064, 2016, 3040,  992, 3552, 1504, 2528,  480,  184,   98, 3808, 1760,   34, 2784,  736,	
	3296, 1248, 2272,  224, 3936, 1888, 2912,  864, 3424, 1376, 4672, 2400,  352, 3680, 1632, 2656,
	  15,  592,   56,  608,   80, 3168,  912,  216,   66,    2,   88,  432,  124,   41,   60,  152,
	  92,    9,   28,  108,   44,   76,   24,   12,  116,  232,  104, 1120,  144,   52,  176, 1808,
	2144,   49,   84,   17,   33,   23,   20,  168,   40,    1,  784,  304,   62,  100,   30,   46,
	  36, 1296,   14,   54,   22,   68,   48,  200,  464,  208,  272,   72, 1552,  336,   96,  136,
	4000,    7,   38,    6,   58,   27,   26,   42,   10,   11,  528,    4,   19,   50,    3,   29,
	  18,  400,   13,   21,    5,   25,    8,  120,  240,  112,  656, 1040,   16, 1952, 2976,  928,
	 576, 7232, 3136, 5184, 1088, 6208, 2112, 4160,   64, 8064, 3968, 6016, 1920, 7040, 2944, 4992,
	 896, 7552, 3456, 5504, 1408, 6528, 2432, 4480,  384, 7808, 3712, 5760, 1664, 6784, 2688, 4736,
	 640, 7296, 3200, 5248, 1152, 6272, 2176, 4224,  128, 7936, 3840, 5888, 1792, 6912, 2816, 4864,
	3488, 1440, 2464,  416, 3744, 1696, 2720,  672, 3232, 1184, 2208,  160, 3872, 1824, 2848,  800,
	3360, 1312, 2336,  288, 3616, 1568, 2592,  544, 3104, 1056, 2080,   32, 4032, 1984, 3008,  960,
	3520, 1472, 2496,  448, 3776, 1728, 2752,  704, 3264, 1216, 2240,  192, 3904, 1856, 2880,  832,
	 768, 3392, 7424, 3328, 5376, 1344, 1280, 6400, 2304, 2368, 4352,  256, 7680, 3584,  320, 5632,
	1536, 6656, 3648, 1600, 2624, 2560, 4608,  512, 7168, 3072, 5120, 1024, 6144, 2048, 4096,    0
};

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

#define END_OF_STREAM 0x305
#define ERROR_CODE 0x306

PictureDecoder::PictureDecoder() {
	_inputStream = nullptr;
	_outputStream = nullptr;
	_field0 = 0;
	_field2 = 0;
	_field6 = 0;
	_field8 = 0;
	_bits = 0;
	_bitsRemaining = 0;
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
	_bits = _inputStream->readByte();
	_bitsRemaining = 0;

	if (_field6 < 4 || _field6 > 6)
		return 1;

	_field8 = 0xFFFF >> (16 - _field6);
	if (_field2) {
		if (_field2 != 1)
			return 2;
		Common::copy(_DATA1, _DATA1 + 256, _array3);
		setup();
	}

	Common::copy(_DATA2, _DATA2 + 16, _array5);
	setupArray(_array5, 16, _REF1, _array2);

	Common::copy(_DATA3, _DATA3 + 16, _array6);
	Common::copy(_DATA4, _DATA4 + 32, _array7);
	Common::copy(_DATA5, _DATA5 + 64, _array4);
	setupArray(_array4, 64, _REF2, _array1);

	int result = unpack();
	return (result == ERROR_CODE) ? 4 : 0;
}

void PictureDecoder::setup() {
	uint val;
	int numBits, shift;
	int mask;

	for (int idx1 = 255; idx1 >= 0; --idx1) {
		mask = 0xff;
		val = _SETUP_DATA[idx1];
		numBits = _array3[idx1];
		
		byte *arrP = _array10;
		if (numBits > 8) {
			if (val & 0xff) {
				_array10[val & 0xff] = 0xff;

				if (val & 0x3f) {
					shift = 4;
					arrP = _array11;
				} else {
					shift = 6;
					arrP = _array12;
					mask = 0x7f;
				}
			} else {
				shift = 8;
				arrP = _array13;
			}
		} else {
			shift = 0;
		}

		val >>= shift;
		numBits -= shift;
		_array3[idx1] = shift;

		for (int idx2 = val; idx2 <= mask; idx2 += (1 << shift))
			arrP[val] = idx1;
	}
}

void PictureDecoder::setupArray(const byte *src, int srcSize, const byte *ref, byte *dest) {
	for (int idx = srcSize - 1; idx >= 0; --idx) {
		int jump = 1 << src[idx];	
		for (int offset = ref[idx]; offset < 256; offset += jump)
			dest[offset] = idx;
	}
}

#define BUFFER_SIZE 0x1000
#define BUFFER_SIZE2 0x2000
#define PADDING_SIZE 516

int PictureDecoder::unpack() {
	byte buffer[BUFFER_SIZE2 + PADDING_SIZE];
	Common::fill(&buffer[0], &buffer[BUFFER_SIZE2 + PADDING_SIZE], 0);

	uint offset = BUFFER_SIZE;
	int result = 0;
	int val, blockOffset;
	while ((val = fetch()) < END_OF_STREAM) {
		if (val < 0x100) {
			// Simple 8-bit value to add to output
			buffer[offset++] = val;
		} else {
			// It's instructions to copy a prior block of data
			int count = val - 0xfe;
			blockOffset = getBlockOffset(count);
			if (!blockOffset) {
				result = ERROR_CODE;
				break;
			}

			byte *destP = &buffer[offset];
			byte *srcP = destP - blockOffset;
			Common::copy(srcP, srcP + count, destP);
			offset += count;
		}

		if (offset >= BUFFER_SIZE2) {
			// Write out the data to the output stream
			_outputStream->write(&buffer[BUFFER_SIZE], BUFFER_SIZE);
			
			// Shift the written data into the low portion of the buffer,
			// so it's available for future block copy operations
			Common::copy(&buffer[BUFFER_SIZE], &buffer[offset], &buffer[0]);
			offset -= BUFFER_SIZE;
		}
	}

	// Write any final bytes out to file
	_outputStream->write(&buffer[BUFFER_SIZE], offset - BUFFER_SIZE);

	return result;
}

bool PictureDecoder::skipBits(int numBits) {
	if (numBits <= _bitsRemaining) {
		// There's enough bits left
		_bits >>= numBits;
		_bitsRemaining -= numBits;
		return false;
	}

	if (_inputStream->pos() >= _inputStream->size())
		return true;

	_bits >>= _bitsRemaining;
	_bits |= ((uint)_inputStream->readByte() << 8);
	_bits >>= numBits - _bitsRemaining;
	_bitsRemaining = -((numBits - _bitsRemaining) - 8);
	return false;
}

#define SKIP_BITS(COUNT) if (skipBits(COUNT)) return ERROR_CODE

int PictureDecoder::fetch() {
	int offset, val;
	int numBits, shift;

	if (_bits & 1) {
		// Will be copying a block of previously generated data
		SKIP_BITS(1);
		offset = _array2[_bits & 0xff];
		numBits = _array5[offset];
		
		SKIP_BITS(numBits);
		shift = _array6[offset];
		if (!shift)
			return offset + 0x100;

		val = _bits & ((1 << shift) - 1);
		val += READ_LE_UINT16(&_array7[offset * 2]);
		SKIP_BITS(shift);
		return val + 0x100;
	}

	SKIP_BITS(1);
	val = _bits & 0xff;
	if (!_field2) {
		// It's a plain 8-bit value
		SKIP_BITS(8);
		return val;
	}

	if (!val) {
		SKIP_BITS(8);
		val = _array13[_bits & 0xff];
	} else {
		offset = val;
		val = _array10[offset];
		if (val == 0xff) {
			if (_bits & 0x3f) {
				SKIP_BITS(4);
				val = _array11[_bits & 0xff];
			} else {
				SKIP_BITS(6);
				val = _array12[_bits & 0x7f];
			}
		}
	}

	numBits = _array3[val];
	SKIP_BITS(numBits);
	return val;
}

#undef SKIP_BITS
#define SKIP_BITS(COUNT) if (skipBits(COUNT)) return 0

int PictureDecoder::getBlockOffset(int numBits) {
	int val = _array1[_bits & 0xff];
	SKIP_BITS(_array4[val]);

	if (numBits == 2) {
		val = (val << 2) | (_bits & 3);
		SKIP_BITS(numBits);
	} else {
		val = (val << _field6) | (_bits & _field8);
		SKIP_BITS(_field6);
	}

	return val + 1;
}

#undef SKIP_BITS

/*-------------------------------------------------------------------*/

int Picture::decode(Common::SeekableReadStream *inStream) {
	assert(!empty());

	Graphics::Surface s = this->getSubArea(Common::Rect(0, 0, this->w, this->h));
	Common::MemoryWriteStream *outStream = new Common::MemoryWriteStream(
		(byte *)s.getPixels(), s.w * s.h);

	int result = PictureDecoder::decode(inStream, outStream);
	delete outStream;
	
	/* Debug code for dumping the data for a picture to a format
	 * I can compare directly with a MEMDUMP from Dosbox Debugger
	const byte *p = (const byte *)s.getPixels();
	for (uint idx = 0; idx < 0xFA00; ) {
		Common::String s = Common::String::format("515D:%.4x   ", idx);
		for (int idx2 = 0; idx2 < 16; ++idx2, ++p, ++idx)
			s += Common::String::format("%.2x ", *p);
		s.toUppercase();
		debug(s.c_str());
	}
	*/

	return result;
}

} // End of namespace Legend
