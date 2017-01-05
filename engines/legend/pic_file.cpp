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

PicFile::PicFile() : _currentFileNumber(-1), _index1(nullptr), _index2(nullptr) {
	_indexes.resize(INDEXES_COUNT);
}

void PicFile::open(int pictureNum, int paletteNum) {
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
		assert(_index1 && _index2);
	}

	// TODO
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
			_index1 = &_indexes[idx]._entries1;
			_index2 = &_indexes[idx]._entries2;
			return;
		}
	}

	// The index isn't present, so set up the slot with the least number of 
	// references to hold the new index to load
	Index &index = _indexes[findIndexesSlot()];
	index._fileNumber = fileNumber;
	index._refCounter = 1;
	_index1 = &index._entries1;
	_index2 = &index._entries2;

	// Load in the first index array
	index._entries1.load(_file);

	// TODO: More setup stuff
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

void PicFile::Index1Entry::load(Common::SeekableReadStream &s) {
	_field0 = s.readUint16LE();
	_field2 = s.readUint16LE();
	_field4 = s.readUint16LE();
	_field6 = s.readUint16LE();
	_field8 = s.readUint16LE();
	_fieldA = s.readUint16LE();
}

/*-------------------------------------------------------------------*/

void PicFile::Index1Entries::load(Common::SeekableReadStream &s) {
	resize(256);
	for (uint idx = 0; idx < 256; ++idx)
		(*this)[idx].load(s);
}

} // End of namespace Legend
