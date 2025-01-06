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

//
//  Run-time object management
//

#ifndef AESOP_RTOBJECT_H
#define AESOP_RTOBJECT_H

#include "aesop/defs.h"

namespace Aesop {

typedef struct {
	UWORD slot;       // object list index
	ULONG name;       // code object name
	UWORD size;       // size of instance data (unused in text files)
} CDESC;               // static context descriptor

#define SF_TXT 1     // text savetype
#define SF_BIN 0     // binary savetype

extern UBYTE objflags[NUM_OBJECTS];
extern HRES objlist[NUM_OBJECTS];

extern BYTE lvlmap[LVL_X][LVL_Y];
extern WORD lvlobj[3][LVL_X][LVL_Y];

void init_object_list();

void restore_range(BYTE *filename, ULONG first, ULONG last, ULONG restoring);
LONG save_range(BYTE *filename, ULONG filetype, ULONG first, ULONG last);
void translate_file(BYTE *TXT_filename, BYTE *BIN_filename, ULONG first,
	ULONG last);

LONG create_object(LONG argcnt, ULONG name);
LONG create_program(LONG argcnt, LONG index, ULONG name);
LONG destroy_object(LONG argcnt, LONG index);

} // namespace Aesop

#endif
