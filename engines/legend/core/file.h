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

#ifndef LEGEND_SIMPLE_FILE_H
#define LEGEND_SIMPLE_FILE_H

#include "common/file.h"
#include "common/rect.h"
#include "common/savefile.h"
#include "common/stream.h"

namespace Legend {

class Decompressor;
class DecompressorData;

enum FileType {
	FILETYPE_PIC = 0,
	FILETYPE_RGN = 1,
	FILETYPE_FNT = 2,
	FILETYPE_MUS = 3,
	FILETYPE_SAV = 4,
	FILETYPE_SAV2 = 5,
	FILETYPE_VOC = 6,
	FILETYPE_RS = 7
};

/**
 * Simple ScummVM File descendent that throws a wobbly if
 * the file it tries to open isn't present
 */
class File : public Common::File {
public:
	File() : Common::File() {}
	File(const Common::String &name);

	/**
	 * Open the file with the given filename, by searching SearchMan.
	 * @param	name	the name of the file to open
	 */
	virtual bool open(const Common::String &name);

	/**
	 * Open the file with the given filename from within the given archive.
	 * @param	name	the name of the file to open
	 * @param	archive		the archive in which to search for the file
	 */
	virtual bool open(const Common::String &name, Common::Archive &archive);

	/**
	 * Open the file corresponding to the give node.
	 * @param   node        the node to consider.
	 */
	virtual bool open(const Common::FSNode &node);

	/**
	 * 'Open' the given stream. That is, we just wrap around it
	 * @param	stream		a pointer to a SeekableReadStream, or 0
	 * @param	name		a string describing the 'file' corresponding to stream
	 */
	virtual bool open(SeekableReadStream *stream, const Common::String &name);

	/**
	 * Open a file of a specified type and number
	 * @param	fileType	The filetype of the file to open
	 * @param	fileNumber	The file number to open
	 */
	void open(FileType fileType, int fileNumber);

	/**
	 * Returns a formatted filename for a given resource type and file number
	 */
	static Common::String getFilename(FileType fileType, int fileNumber);
};

/**
 * This class implements basic reading and writing to files
 */
class SimpleFile {
private:
	/**
	 * Skip over any pending spaces
	 */
	void skipSpaces();
protected:
	Common::SeekableReadStream *_inStream;
	Common::OutSaveFile *_outStream;
	int _lineCount;
public:
	SimpleFile();
	virtual ~SimpleFile();

	operator Common::SeekableReadStream &() { return *_inStream; }
	operator Common::WriteStream &() { return *_outStream; }

	/**
	 * Set up a stream for read access
	 */
	virtual void open(Common::SeekableReadStream *stream);

	/**
	 * Set up a stream for write access
	 */
	virtual void open(Common::OutSaveFile *stream);

	/**
	 * Close the file
	 */
	virtual void close();

	/**
	 * Read from the file with validation
	 */
	virtual void safeRead(void *dst, size_t count);

	/**
	 * Read from the file
	 */
	virtual size_t unsafeRead(void *dst, size_t count);

	/**
	 * Write out data
	 */
	virtual size_t write(const void *src, size_t count) const;

	/**
	 * Seek
	 */
	virtual void seek(int offset, int origin);
	/**
	 * Read a byte
	 */
	byte readByte();

	/**
	 * Read a 16-bit LE number
	 */
	uint readUint16LE();

	/**
	 * Read a 32-bit LE number
	 */
	uint readUint32LE();

	/**
	 * Read a string from the file
	 */
	Common::String readString();

	/**
	 * Read a number from the file
	 */
	int readNumber();

	/**
	 * Read a floating point number from the file
	 */
	double readFloat();

	/**
	 * Read in a point
	 */
	Common::Point readPoint();

	/**
	 * Read in a rect
	 */
	Common::Rect readRect();

	/**
	 * Rect in a bounds
	 */
	Common::Rect readBounds();

	/**
	 * Read a string and copy it into an optionally passed buffer
	 */
	void readBuffer(char *buffer = nullptr, size_t count = 0);

	/**
	 * Scan in values from the file
	 */
	bool scanf(const char *format, ...);

	/**
	 * Write out a byte
	 */
	void writeByte(byte b) { write(&b, 1); }

	/**
	 * Write out a raw 16-bit LE number
	 */
	void writeUint16LE(uint val);

	/**
	 * Write out a raw 32-bit LE number
	 */
	void writeUint32LE(uint val);

	/**
	 * Write a string line
	 */
	void writeLine(const Common::String &str) const;

	/**
	 * Write a string
	 */
	void writeString(const Common::String &str) const;

	/**
	 * Write a quoted string
	 */
	void writeQuotedString(const Common::String &str) const;

	/**
	 * Write a quoted string line
	 */
	void writeQuotedLine(const Common::String &str, int indent) const;

	/**
	 * Write a number to file
	 */
	void writeNumber(int val) const;

	/**
	 * Write a number line to file
	 */
	void writeNumberLine(int val, int indent) const;

	/**
	 * Write a floating point number
	 */
	void writeFloat(double val) const;

	/**
	 * Write a floating point number as a line
	 */
	void writeFloatLine(double val, int indent) const;

	/**
	 * Write out a point line
	 */
	void writePoint(const Common::Point &pt, int indent)const;

	/**
	 * Write out a rect line
	 */
	void writeRect(const Common::Rect &r, int indent) const;

	/**
	 * Write out a bounds line
	 */
	void writeBounds(const Common::Rect &r, int indent) const;

	/**
	 * Write out a string using a format specifier, just like fprintf
	 */
	void writeFormat(const char *format, ...) const;

	/**
	 * Write out a number of tabs to form an indent in the output
	 */
	void writeIndent(uint indent) const;

	/**
	 * Validates that the following non-space character is either
	 * an opening or closing squiggly bracket denoting a class
	 * definition start or end. Returns true if it's a class start
	 */
	bool IsClassStart();

	/**
	 * Write the starting header for a class definition
	 */
	void writeClassStart(const Common::String &classStr, int indent);

	/**
	 * Write out the ending footer for a class definition
	 */
	void writeClassEnd(int indent);

	/**
	 * Return true if the stream has finished being read
	 */
	bool eos() const {
		assert(_inStream);
		return _inStream->pos() >= _inStream->size();
	}
};

/**
 * General purpose support method for reading an ASCIIZ string from a stream
 */
Common::String readStringFromStream(Common::SeekableReadStream *s);

} // End of namespace Legend

#endif
