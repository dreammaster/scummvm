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

#ifndef AESOP_RTOBJECT_H
#define AESOP_RTOBJECT_H

#include "aesop/defs.h"

namespace Aesop {

typedef struct
{
   UWORD slot;       // object list index
   uint32 name;       // code object name
   UWORD size;       // size of instance data (unused in text files)
}
CDESC;               // static context descriptor

#define SF_TXT 1     // text savetype
#define SF_BIN 0     // binary savetype
                  
extern UBYTE objflags[NUM_OBJECTS];
extern HRES objlist[NUM_OBJECTS];

extern BYTE lvlmap[LVL_X][LVL_Y];
extern WORD lvlobj[3][LVL_X][LVL_Y];

void init_object_list(void);

void restore_range(const char *filename, uint32 first, uint32 last, uint32 restoring);
int32 save_range(const char *filename, int32 filetype, int32 first, int32 last);
void translate_file(const char *TXT_filename, const char *BIN_filename, uint32 first,
   uint32 last);

int32 create_object(int32 argcnt, uint32 name);
int32 create_program(int32 argcnt, int32 index, uint32 name);
int32 destroy_object(int32 argcnt, int32 index);

} // namespace Aesop

#endif
