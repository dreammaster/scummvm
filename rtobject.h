/* RogueVM - RPG Engines
 *
 * RogueVM is the legal property of its developers, whose names
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

//
//  Run-time object management
//

#ifndef AESOP_RTOBJECT_H
#define AESOP_RTOBJECT_H

#include "aesop/shared.h"

namespace Aesop {

// static context descriptor
struct CDESC {
	UWORD slot;       // object list index
	ULONG name;       // code object name
	UWORD size;       // size of instance data (unused in text files)
};

#define SF_TXT 1     // text savetype
#define SF_BIN 0     // binary savetype

extern UBYTE objflags[NUM_OBJECTS];
extern HRES objlist[NUM_OBJECTS];

extern BYTE lvlmap[LVL_X][LVL_Y];
extern WORD lvlobj[3][LVL_X][LVL_Y];

extern void init_object_list(void);

extern void restore_range(const char *filename, ULONG first, ULONG last, ULONG restoring);
extern LONG save_range(const char *filename, ULONG filetype, ULONG first, ULONG last);
void translate_file(const char *TXT_filename, const char *BIN_filename, ULONG first,
                    ULONG last);

extern LONG create_object(LONG argcnt, ULONG name);
extern LONG create_program(LONG argcnt, LONG index, ULONG name);
extern LONG destroy_object(LONG argcnt, LONG index);

} // End of namespace Aesop

#endif

