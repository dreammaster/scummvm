/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef YENDOR_UTILS_FILE_ENTRY_H
#define YENDOR_UTILS_FILE_ENTRY_H

#include "common/stream.h"

namespace Yendor {

struct FileEntry {
	const Common::String _filename;
	Common::Stream *_stream = nullptr;
	byte *_buffer = nullptr;
	size_t _blockSize = 0;
	size_t _blockIndex = 0;
	size_t _blockOffset = 0;

	FileEntry(const Common::String &filename) :
		_filename(filename) {
	}

	/**
	 * Close an open file
	 */
	void close();

	/**
	 * Create the file
	 */
	bool create();

	/**
	 * Open an existing file
	 */
	bool open();

	/**
	 * Read data from the file
	 */
	bool read();

	/**
	 * Write data to the file
	 */
	bool write();

	/**
	 * Seek within the file
	 */
	void seek();

};

} // namespace Yendor

#endif
