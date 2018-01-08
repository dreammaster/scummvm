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

#include "common/memstream.h"
#include "common/ptr.h"
#include "graphics/managed_surface.h"
#include "legend/core/file.h"

namespace Legend {
namespace Gfx {

enum {
	PIC_BIT_DEPTH = 0xF,
	PIC_HAS_PALETTE = 0x10,
	PIC_40 = 40
};

class PictureDecoder {
private:
	static const uint16 _SETUP_DATA[256];
	static const byte _DATA1[256];
	static const byte _DATA2[16];
	static const byte _DATA3[16];
	static const byte _DATA4[32];
	static const byte _DATA5[64];
	static const byte _REF1[16];
	static const byte _REF2[64];
private:
	Common::SeekableReadStream *_inputStream;
	Common::WriteStream *_outputStream;
	int _field0;
	int _field2;
	int _field6;
	int _field8;
	uint _bits;
	int _bitsRemaining;
	byte _array1[256];
	byte _array2[256];
	byte _array3[256];
	byte _array4[64];
	byte _array5[16];
	byte _array6[16];
	byte _array7[16];
	
	byte _array10[256];
	byte _array11[256];
	byte _array12[128];
	byte _array13[256];

private:
	/**
	 * Inner method for doing the decoding
	 */
	int decodeInner();

	/**
	 * Does some general setup of reference arrays
	 */
	void setup();

	/**
	 * Sets up an array used in unpacking
	 */
	void setupArray(const byte *src, int srcSize, const byte *ref, byte *dest);

	/**
	 * Does the actual unpacking
	 */
	int unpack();

	/**
	 * Skips over the specified number of bits from the input stream,
	 * handling reading further bytes as necessary
	 * @param numBits	Number of bits that will be skipped
	 * @return		True if the end of stream has been reached
	 */
	bool skipBits(int numBits);

	/**
	 * Fetches a new value from the input
	 */
	int fetch();

	/**
	 * Figures out the backwards offset for the start of a
	 * previously generated block of values to copy
	 */
	int getBlockOffset(int numBits);
public:
	PictureDecoder();
	virtual ~PictureDecoder();

	/**
	 * Decodes a picture from a passed stream
	 */
	int decode(Common::SeekableReadStream *inStream, Common::WriteStream *outStream);
};

class Picture : public Graphics::ManagedSurface, public PictureDecoder {
public:
	Common::Point _origin;
public:
	Picture() : Graphics::ManagedSurface(), PictureDecoder() {}
	Picture(Picture &surf) : Graphics::ManagedSurface(surf), PictureDecoder() {}
	Picture(int width, int height) : Graphics::ManagedSurface(width, height), PictureDecoder() {}
	Picture(Picture &surf, const Common::Rect &bounds) : Graphics::ManagedSurface(surf, bounds), PictureDecoder() {}

	/**
	 * Decodes a picture from a passed stream
	 */
	int decode(Common::SeekableReadStream *inStream);
};

class PictureArray : public Common::Array<Picture *> {
public:
	~PictureArray();

	/**
	 * Gets a picture
	 * @param picNum		Picture number
	 * @param frameNum		Frame number
	 * @returns True if picture was loaded
	 */
	bool load(uint picNum, uint frameNum = 0);

	/**
	 * Load a sequential range of pictures
	 * @param count			Maximum number of images. 0 = Up to 256
	 * @param picNum		Starting picture number
	 * @param frameNum		Frame number
	 * @returns Number of images loaded
	 */
	size_t loadRange(size_t count, uint picNum, uint frameNum = 0);
};

class PicFile {
	struct IndexEntry {
		int _offset;
		uint _frameCount;
		byte _flags;
		uint _width;
		uint _height;
		byte _fieldA;
		byte _fieldB;
		// Calculated after loading
		uint _size;

		IndexEntry() : _offset(0), _frameCount(0), _flags(0), _width(0),
			_height(0), _fieldA(0), _fieldB(0), _size(0) {}

		/**
		 * Load an index entry
		 */
		void load(Common::SeekableReadStream &s);
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
	bool _paletteCheck;
	bool _skipPreload;
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

	/**
	 * Reads in a palette from the file
	 */
	void readPalette(uint bitDepth);
public:
	IndexEntry _currentPic;
	Common::Point _origin;
public:
	PicFile();
	~PicFile();

	/**
	 * Opens up a picture
	 */
	bool open(uint pictureNum, uint frameNum = 0);

	/**
	 * Gets a picture
	 * @param picNum		Picture number
	 * @param frameNum		Frame number
	 * @returns Picture
	 */
	Picture *load(uint picNum, uint frameNum = 0);

	/**
	 * Load a sequential range of pictures
	 * @param images		Picture array to add items to
	 * @param count			Maximum number of images. 0 = Up to 256
	 * @param picNum		Starting picture number
	 * @param frameNum		Frame number
	 * @returns Number of images loaded
	 */
	size_t loadRange(PictureArray &images, size_t count, uint picNum, uint frameNum = 0);
};

} // End of namespace Gfx
} // End of namespace Legend

#endif
