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

#include "spycraft/game/file.h"

namespace Spycraft {

File::File() {
	hFile = nullptr;
	filesize = 0;
}

int File::open(const char *filename, int mode) {
	hFile = sfxOpenFile(filename, mode);
	if (!hFile) {
		sfxPrintf("Can't find %s, please change to game directory.", filename);
		return false;
	}

	filesize = sfxGetFileSize(hFile);
	return true;
}

int File::close() {
	return (sfxCloseFile(hFile));
}

void File::seek(int offset, int mode) {
	sfxSeekFile(hFile, offset, mode);
}

/*********************
 *
 *		File Read
 *
 *********************/
int File::read(void *buffer, int size) {
	return (sfxReadFile(hFile, buffer, size));
}

int File::readByte(BYTE *value) {
	return (sfxReadFile(hFile, value, sizeof(BYTE)));
}

int File::readWord(WORD *value) {
	return (sfxReadFile(hFile, value, sizeof(WORD)));
}

char *File::readString() {
	char *string;

	string = sfxReadFileString(hFile);

	return (string);
}


/*********************
 *
 *		File Write
 *
 *********************/
int File::write(void *buffer, int size) {
	return (sfxWriteFile(hFile, buffer, size));
}

int File::writeByte(BYTE *value) {
	return (sfxWriteFile(hFile, value, sizeof(BYTE)));
}

int File::writeWord(WORD *value) {
	return (sfxWriteFile(hFile, value, sizeof(WORD)));
}

unsigned long File::writeASCFile(const char *format, ...) {
	return (sfxWriteASCFile(hFile, format));
}

} // namespace Spycraft
