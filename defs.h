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
//  AESOP common definitions for 16-bit DOS
//
//  (Shared by ARC and runtime sources)
//

#ifndef AESOP_DEFS_H
#define AESOP_DEFS_H

namespace Aesop {

#ifndef TYPEDEFS
#define TYPEDEFS
typedef unsigned short UWORD;
typedef unsigned char UBYTE;
typedef unsigned long ULONG;
typedef short WORD;
typedef signed char BYTE;
typedef long LONG;
#endif

typedef ULONG HRES;           // run-time resource handle
typedef UWORD HSTR;           // run-time len-prefixed string descriptor

#define MSG_CREATE  0         // predefined message tokens (sent by system)
#define MSG_DESTROY 1
#define MSG_RESTORE 2

#define MAX_G 16              // Maximum depth of "family trees"

// Instance header
struct IHDR {
   HRES thunk;
};

// Thunk header
struct THDR {
   UWORD MV_list;
   UWORD max_msg;
   UWORD SD_list;
   UWORD nprgs;
   UWORD isize;
   UWORD use_cnt;
};

// SOP program header
struct PRG_HDR {
   UWORD static_size;
   ULONG imports;
   ULONG exports;
   ULONG parent;
};

// Message handler header
struct MHDR {
   UWORD auto_size;
};

// Thunk message vector list entry
struct MV_entry {
   UWORD msg;
   ULONG handler;
   UWORD SD_offset;
};

// Thunk SOP descriptor list entry
struct SD_entry {
   HRES SOP;
   ULONG exports;
   UWORD static_base;
   UWORD extern_base;
};                     

// Palette resource header
struct PAL_HDR {
   UWORD ncolors;
   UWORD RGB;
   UWORD fade[11];
};

} // End of namespace Aesop

#endif

