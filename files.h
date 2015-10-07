/* RogueVM - RPG Engines
 *
 * RogueVM is the legal property of its developers, whose names
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

//
//  Runtime heap/resource class
//

#ifndef AESOP_FILES_H
#define AESOP_FILES_H

#include "common/file.h"
#include "common/savefile.h"
#include "aesop/resource_structures.h"

namespace Aesop {

/**
 * Text file access mode
 */
enum FileMode { FILE_WRITE = 0, FILE_READ = 1 };

/**
 * File type
 */
enum FileType { SF_BIN = 0, SF_TXT = 1 };

/**
 * Universal disk error codes
 */
enum FileError {
	NO_ERROR = 0,
	IO_ERROR = 1,
	OUT_OF_MEMORY = 2,
	FILE_NOT_FOUND = 3,
	CANT_WRITE_FILE = 4,
	CANT_READ_FILE = 5,
	DISK_FULL = 6
};

class AesopEngine;

class BinaryFile {
private:
	Common::File _inFile;
	Common::InSaveFile *_inSave;
	Common::OutSaveFile *_outSave;
	FileMode _mode;
public:
	BinaryFile();
	BinaryFile(const Common::String &filename, FileMode oFlag);
	virtual ~BinaryFile();

	/**
	 * Try and open a specified file
	 */
	bool open(const Common::String &filename, FileMode oFlag);

	/**
	 * Close any active open file
	 */
	void close();

	/**
	 * Read data from the file
	 */
	uint read(void *buffer, uint bufSize);

	/**
	 * Read a single byte from the file
	 */
	byte readByte();

	/**
	 * Write data to the file
	 */
	uint write(const void *buffer, uint bufSize);

	/**
	 * Write a single byte to the file
	 */
	void writeByte(byte v);

	/**
	 * Return the current position within the file
	 */
	uint pos() const;

	/**
	 * Return the size of the file
	 */
	uint size() const;

	operator Common::SeekableReadStream &();

	operator Common::WriteStream &();
};

class TextFile: public BinaryFile {
public:
	TextFile() : BinaryFile() {}
	TextFile(const Common::String &filename, FileMode oFlag) : BinaryFile(filename, oFlag) {}
	virtual ~TextFile() {}

	/**
	 * Read character from text file
	 * @returns		0 if EOF reached
	 */
	char rchar();

	/**
	 * Write character to text file
	 * @returns		0 if write attempt failed
	 */
	void wchar(char ch);

	/**
	 * Read a line from the text and return it
	 * \r's are skipped
	 * \n's are truncated
	 *
	 * Blank lines are ignored
	 * @returns		False if EOF reached
	 */
	bool readln(Common::String &buffer, uint maxlen);

	/**
	 * Write buffer line to text file
	 * @returns		False if write attempt failed
	 * \r\n added at end of each buffer line
	 */
	bool writeln(const Common::String &line);
};

class Files {
private:
	AesopEngine *_vm;
public:
	FileError _diskError;
public:
	Files(AesopEngine *vm) : _vm(vm), _diskError(NO_ERROR) {}

	/**
	 * Delete a file
	 */
	bool deleteFile(const Common::String &filename);

	/**
	 * Copy the specified file
	 */
	int copyFile(const Common::String &srcFilename, const Common::String &destFilename);

	/**
	 * Return the age of the given file
	 */
	uint fileTime(const Common::String &filename);

	//
	// Binary file management
	//

	/**
	 * Return the size of a given file
	 */
	uint fileSize(const Common::String &filename);

	/**
	 * Read a file directly into memory
	 */
	byte *readFile(const Common::String &filename, byte *dest);

	/**
	 * Create a file directly from the passed buffer
	 */
	uint writeFile(const Common::String &filename, void *buf, uint len);

	/**
	 * Append the passed data block onto the end of the given file
	 */
	uint appendFile(const Common::String &filename, void *buf, uint len);

	/**
	 * Save static context of range of objects to text or binary file
	 * @returns		1 if write succeeded (disk full or other error if 0)
	 */
	int saveRange(const Common::String &filename, FileType fileType, uint first, uint last);

	/**
	 * Restore static context of range of objects from text or binary file
	 *
	 * For each object list entry between first and last:
	 *
	 *   Read entry's context descriptor
	 *
	 *   If EOF or CDESC.slot != entry #
	 *       Report corrupted context file
	 *
	 *   If CDESC.name == -1 (i.e., slot unused in file)
	 *       If slot entry exists, destroy it
	 *       Continue
	 *
	 *   If slot entry exists
	 *       If CDESC.name != current entry's name
	 *          Destroy the current entry to make room for the new one
	 *       Else
	 *          Destruction of current entry not necessary; deallocate any
	 *          assigned resources but leave entry otherwise intact
	 *
	 *   If slot entry available (perhaps newly destroyed), create the object
	 *
	 *   Restore object's instance data from context file
	 *
	 *   If restoring, send "restore" message to newly instantiated object
	 *
	 * Errors during context restoration are fatal at the system level;
	 * this routine will not return if restoration fails
	 */
	void restoreRange(const Common::String &filename, uint first, uint last, uint restoring);

	/**
	 * Fetch slot and ordinal entry # from context file
	 * @returns		NULL if EOF reached
	 */
	CDESC *readContextDescriptor(TextFile *TF);

	/**
	 * Restore object's static context from text file
	 */
	void restoreStaticContext(HRES instance, TextFile *TF);

	/**
	 * Translate static context file from text to binary format
	 *
	 * For each entry in file:
	 *
	 * Read entry's context descriptor
	 * Save descriptor to binary file
	 *
	 *   If EOF or CDESC.slot != entry #
	 *       Report corrupted context file
	 *
	 *   If CDESC.name == -1 (i.e., slot unused in file)
	 *       Continue
	 *
	 *   Create source object instance
	 *   Restore instance from text file
	 *   Save instance to binary file
	 *   Destroy instance
	 *
	 * Errors during context file translation are fatal at the system level;
	 * this routine will not return if translation fails
	 */
	void translateFile(const Common::String &txtFilename, const Common::String &binFilename, uint first, uint last);
};

} // End of namespace Aesop

#endif

