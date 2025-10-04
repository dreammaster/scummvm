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

#ifndef SPYCRAFT_ADVVOLS_H
#define SPYCRAFT_ADVVOLS_H

#include "spycraft/advlib.h"

namespace Spycraft {

#define MAX_BUFFER_SIZE		1024
#define MAX_DISC_LIMIT		32
#define MAX_STREAM_LIMIT	2100
#define MAX_VOLS_FILE		4096

enum {
	VOLS_COMMENT,
	VOLS_SIZE,
	VOLS_DISC,
	VOLS_PATH,
	VOLS_LINE
};

enum {
	COMPRESS_NONE,
	COMPRESS_PKWARE,
	COMPRESS_ZLIB,
	COMPRESS_HARC
};

struct FileChunk {
	int vid;
	int offset;
	unsigned char compressor;
	unsigned char location;		/* WHICH CD IS ON */
};

extern int MakeVolsID(int vid, int type);
extern void GetVolsID(int vid, int *id, int *type);
extern void sfxMakeVols(char *src);
extern int sfxGetFileLength(char *filename);

} // namespace Spycraft

#endif
