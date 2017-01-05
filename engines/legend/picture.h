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

#ifndef LEGEND_PIC_H
#define LEGEND_PIC_H

#include "graphics/managed_surface.h"
#include "legend/file.h"

namespace Legend {

enum {
	PIC_HAS_PALETTE = 0x10
};

class Picture : public Graphics::ManagedSurface {
};

class PicFile {
	struct IndexEntry {
		uint _offset;
		uint _frameCount;
		byte _flags;
		uint _width;
		uint _height;
		int _fieldA;
		// Calculated after loading
		uint _size;

		IndexEntry() : _offset(0), _frameCount(0), _flags(0), _width(0),
			_height(0), _fieldA(0), _size(0) {}

		/**
		 * Load an index entry
		 */
		bool load(Common::SeekableReadStream &s);
	};
	class IndexEntries : public Common::Array<IndexEntry> {
	public:
		/**
		 * Load an index entry
		 */
		void load(Common::SeekableReadStream &s);
	};

	struct Index {
		int _fileNumber;
		int _refCounter;
		IndexEntries _entries;

		Index() : _fileNumber(-1), _refCounter(0) {}
	};
	typedef Common::Array<Index> Indexes;
private:
	File _file;
	int _currentFileNumber;
	Indexes _indexes;
	IndexEntries *_index;
	IndexEntry _currentPic;
	bool _paletteCheck;
	bool _skipPreload;
	int _val1;
	int _val2;
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
	~PicFile();

	/**
	 * Opens up a picture
	 */
	bool open(uint pictureNum, uint frameNum = 0);

	/**
	 * Gets a picture
	 */
	Picture *load(uint pictureNum, uint frameNum = 0);
};

} // End of namespace Legend

#endif
