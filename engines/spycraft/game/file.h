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

#ifndef SPYCRAFT_GAME_FILE_H
#define SPYCRAFT_GAME_FILE_H

#include "spycraft/game/object.h"
#include "spycraft/dmade/advfile.h"

namespace Spycraft {

enum {
	FILE_WRITE,
	FILE_READ,
	FILE_APPEND
};

enum {
	FILE_SEEK_BEG,
	FILE_SEEK_CUR,
	FILE_SEEK_END
};

class File : public Object {
public:
	int hFile;
	int filesize;

	File();
	int open(char *filename, int mode);
	void seek(int offset, int mode);
	int read(void *buffer, int size);
	int readByte(uint8 *value);
	int readWord(uint16 *value);
	int write(void *buffer, int size);
	int writeByte(uint8 *value);
	int writeWord(uint16 *value);
	unsigned long writeASCFile(char *format, ...);
	int close(void);
	char *readString();
};

} // namespace Spycraft

#endif			
