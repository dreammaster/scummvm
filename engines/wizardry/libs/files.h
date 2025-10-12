
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

#ifndef WIZARDRY_LIBS_FILES_H
#define WIZARDRY_LIBS_FILES_H

#include "common/scummsys.h"

namespace Wizardry {

enum {
	DRIVE1 = 0,
	DRIVE2 = 1
};
constexpr int BLOCKSZ = 512;

/**
 * Reads blocks from a WIZBOOT DSK file
 * @param unit			Drive number (0 = DRIVE1, 1 = DRIVE2, etc.)
 * @param buffer		Pointer to destination memory
 * @param byteCount		Number of bytes to read (usually 512)
 * @param blockNumber	Which 512-byte block to read
 * @param offset		Byte offset within block (normally 0)
 * @returns				Zero if successful, non-zero if not
 */
extern int UNITREAD(int unit, void *buffer, int byteCount, int blockNumber, int offset = 0);

/**
 * Scan for a given file's start
 * @param drive 		Drive number (0 = DRIVE1, 1 = DRIVE2, etc.)
 * @param filename		Filename to find
 * @return				Block number (>= 0) if found
 */
extern int FINDFILE(int drive, const char *filename);

} // namespace Wizardry

#endif
