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
//  AESOP common definitions
//

#ifndef AESOP_DEFS_H
#define AESOP_DEFS_H

#include "common/rect.h"
#include "graphics/font.h"

namespace Aesop {

typedef uint16 UWORD;
typedef byte UBYTE;
typedef uint32 ULONG;
typedef int16 WORD;
typedef char BYTE;
typedef int32 LONG;

typedef ULONG HRES;           // run-time resource handle
typedef UWORD HSTR;           // run-time len-prefixed string descriptor
typedef Common::Rect RECT;

typedef Graphics::Font FONT;

#define MSG_CREATE  0         // predefined message tokens (sent by system)
#define MSG_DESTROY 1
#define MSG_RESTORE 2

#define MAX_G 16              // Maximum depth of "family trees"

typedef struct {
	HRES thunk;
} IHDR;                         // Instance header

typedef struct {
	UWORD MV_list;
	UWORD max_msg;
	UWORD SD_list;
	UWORD nprgs;
	UWORD isize;
	UWORD use_cnt;
} THDR;                         // Thunk header

typedef struct {
	UWORD static_size;
	ULONG imports;
	ULONG exports;
	ULONG parent;
} PRG_HDR;                      // SOP program header

typedef struct {
	UWORD auto_size;
} MHDR;                         // Message handler header

typedef struct {
	UWORD msg;
	ULONG handler;
	UWORD SD_offset;
} MV_entry;                     // Thunk message vector list entry

typedef struct {
	HRES SOP;
	ULONG exports;
	UWORD static_base;
	UWORD extern_base;
} SD_entry;                     // Thunk SOP descriptor list entry

typedef struct {
	UWORD ncolors;
	UWORD RGB;
	UWORD fade[11];
} PAL_HDR;                      // Palette resource header

} // namespace Aesop

#endif
