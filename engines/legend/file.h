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

class File : public Common::File {
public:
	File() : Common::File() {}
	File(const Common::String &name);

	/**
	 * Try to open the file with the given filename, by searching SearchMan.
	 * @note Must not be called if this file already is open (i.e. if isOpen returns true).
	 *
	 * @param	name	the name of the file to open
	 * @return	true if file was opened successfully, false otherwise
	 */
	virtual bool open(const Common::String &name);

	/**
	 * Try to open the file with the given filename from within the given archive.
	 * @note Must not be called if this file already is open (i.e. if isOpen returns true).
	 *
	 * @param	name	the name of the file to open
	 * @param	archive		the archive in which to search for the file
	 * @return	true if file was opened successfully, false otherwise
	 */
	virtual bool open(const Common::String &name, Common::Archive &archive);

	/**
	 * Try to open the file corresponding to the give node. Will check whether the
	 * node actually refers to an existing file (and not a directory), and handle
	 * those cases gracefully.
	 * @note Must not be called if this file already is open (i.e. if isOpen returns true).
	 *
	 * @param   node        the node to consider.
	 * @return	true if file was opened successfully, false otherwise
	 */
	virtual bool open(const Common::FSNode &node);

	/**
	 * Try to 'open' the given stream. That is, we just wrap around it, and if stream
	 * is a NULL pointer, we gracefully treat this as if opening failed.
	 * @note Must not be called if this file already is open (i.e. if isOpen returns true).
	 *
	 * @param	stream		a pointer to a SeekableReadStream, or 0
	 * @param	name		a string describing the 'file' corresponding to stream
	 * @return	true if stream was non-zero, false otherwise
	 */
	virtual bool open(SeekableReadStream *stream, const Common::String &name);

};

} // End of namespace Legend

#endif
