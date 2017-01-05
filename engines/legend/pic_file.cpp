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

#include "legend/pic_file.h"
#include "legend/legend.h"

namespace Legend {

#define INDEXES_COUNT 2

PicFile::PicFile() : _currentFileNumber(-1), _index(nullptr),
		_paletteCheck(false), _skipPreload(false), _val1(0), _val2(0) {
	_indexes.resize(INDEXES_COUNT);
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
		loadIndex(pictureNum);
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
	_fieldA = s.readUint16LE();
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

} // End of namespace Legend
