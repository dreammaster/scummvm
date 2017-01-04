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

#ifndef LEGEND_FILE_H
#define LEGEND_FILE_H

#include "common/file.h"
#include "common/str.h"

namespace Legend {

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

class LegendEngine;

class File : public Common::File {
public:
	/**
	 * Form a filename for a specified file type and number
	 */
	static Common::String getFilename(FileType fileType, int fileNumber);
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
};

} // End of namespace Legend

#endif
