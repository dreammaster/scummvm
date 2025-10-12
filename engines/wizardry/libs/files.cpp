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

#include "common/memstream.h"
#include "wizardry/libs/files.h"
#include "wizardry/data/globals.h"

namespace Wizardry {

enum FileType {
	VOLHEAD, BADBLK, MACH6502, TEXT, DEBUG,
	DATA, GRAFFILE, FOTOFILE, SUBDIR
};

struct DirEntry {
	uint16 FIRSTBLK = 0;
	uint16 LASTBLK = 0;
	struct {
		FileType FT = VOLHEAD;
	} FILEKIND;
	Common::String FILENAME;
	uint16 SIGNATURE = 0;
	uint16 DISK_FLAGS = 0;
	uint16 CHECKSUM = 0;

	void load(Common::MemoryReadStream &src) {
		FIRSTBLK = src.readUint16LE();
		LASTBLK = src.readUint16LE();
		FILEKIND.FT = (FileType)src.readByte();
		src.skip(1);

		byte len = src.readByte();
		char buf[14];
		assert(len <= 13);
		src.read(buf, 13);
		buf[len] = '\0';
		FILENAME = Common::String(buf);

		SIGNATURE = src.readUint16LE();
		DISK_FLAGS = src.readUint16LE();
		CHECKSUM = src.readUint16LE();
	}
};

int UNITREAD(int unit, void *buffer, int byteCount, int blockNumber, int offset) {
	assert(unit == DRIVE1);
	if (blockNumber < 0 || blockNumber >= 640)
		return -1;

	auto &f = _G(dsk);
	f.seek(blockNumber * 512 + offset);
	return f.read(buffer, byteCount) == (uint32)byteCount ? 0 : -1;
}

int FINDFILE(int drive, const char *filename) {
	DirEntry dirEntry;
	byte buf[BLOCKSZ];

	// Read the index block
	if (UNITREAD(drive, buf, BLOCKSZ, 2, 0) != 0)
		return -1;

	Common::MemoryReadStream src(buf, 26 * 20);
	dirEntry.load(src);		// Skip over the WIZBOOT entry

	for (;;) {
		dirEntry.load(src);
		if (dirEntry.SIGNATURE != 0x6776)
			break;

		if (dirEntry.FILEKIND.FT >= BADBLK && dirEntry.FILEKIND.FT <= FOTOFILE) {
			if (dirEntry.FILENAME.equalsIgnoreCase(filename))
				return dirEntry.FIRSTBLK;
		}
	}

	// Not found
	return -9;
}

} // namespace Wizardry
