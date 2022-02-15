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

#ifndef DINK_FAST_FILE_H
#define DINK_FAST_FILE_H

#include "common/file.h"

namespace Dink {

typedef void *HFASTFILE;

/**
 * Initialize for fast file access. The master file and maximum number
 * of open "files" are specified.
 */
extern bool FastFileInit(const char *fname, int max_handles);

/**
 * Clean up resources
 */
extern void FastFileFini();

/**
 * Search the directory for the file, and return a file handle if found.
 */
extern HFASTFILE FastFileOpen(const char *name);

/**
 * Mark a fast file handle as closed
 */
extern bool FastFileClose(HFASTFILE ff);

/**
 * Return a memory pointer into a fast file
 */
extern void *FastFileLock(HFASTFILE ff, int off, int len);

/**
 * Unlock a memory block
 */
extern bool FastFileUnlock(HFASTFILE ff, int off, int len);

/**
 * Read from a fast file (memcpy!)
 */
extern bool FastFileRead(HFASTFILE ff, void *ptr, int size);

/**
 * Set to a new position in a fast file.  Uses standard SEEK_SET, SEEK_CUR,
 * SEEK_END definitions.
 */
extern bool FastFileSeek(HFASTFILE ff, int off, int how);

/**
 * Get the current position in a fast file
 */
extern long FastFileTell(HFASTFILE ff);

} // namespace Dink

#endif
