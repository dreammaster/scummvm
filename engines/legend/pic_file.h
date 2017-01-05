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

#ifndef LEGEND_PIC_FILE_H
#define LEGEND_PIC_FILE_H

#include "legend/file.h"

namespace Legend {

class PicFile {
	struct Index1Entry {
		int _field0;
		int _field2;
		int _field4;
		int _field6;
		int _field8;
		int _fieldA;

		Index1Entry() : _field0(0), _field2(0), _field4(0), _field6(0),
			_field8(0), _fieldA(0) {}

		/**
		 * Load an index entry
		 */
		void load(Common::SeekableReadStream &s);
	};
	class Index1Entries : public Common::Array<Index1Entry> {
	public:
		/**
		 * Load an index entry
		 */
		void load(Common::SeekableReadStream &s);
	};

	struct Index2Entry {
		int _field0;

		Index2Entry() : _field0(0) {}
	};
	typedef Common::Array<Index2Entry> Index2Entries;

	struct Index {
		int _fileNumber;
		int _refCounter;
		Index1Entries _entries1;
		Index2Entries _entries2;

		Index() : _fileNumber(-1), _refCounter(0) {}
	};
	typedef Common::Array<Index> Indexes;
private:
	File _file;
	int _currentFileNumber;
	Indexes _indexes;
	Index1Entries *_index1;
	Index2Entries *_index2;
private:
	/**
	 * Loads the index of a newly opened PIC file
	 */
	void loadIndex(int fileNumber);

	/**
	 * Selects an index in the main indexes array to load the index
	 * for a newly opened file into
	 */
	int findIndexesSlot() const;
public:
	PicFile();

	/**
	 * Opens up a picture file
	 */
	void open(int pictureNum, int paletteNum = 0);
};

} // End of namespace Legend

#endif
