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
//  Run-time graphics subs
//

#ifndef AESOP_GRAPHICS_H
#define AESOP_GRAPHICS_H

#include "aesop/defs.h"

namespace Aesop {

extern UBYTE *fade_tables[5][16];
extern UWORD first_color[5];
extern UWORD in_GIL;

//#define MODE_X 1                 // 1 for VGA mode X, 0 for MCGA mode 13h

#define NTW 32                   // # of text windows available

void dprint(LONG argcnt, BYTE *format, ...);
void sprint(LONG argcnt, ULONG wndnum, BYTE *format, ...);
void text_color(LONG argcnt, ULONG wndnum, ULONG current, ULONG new);
void release_owned_windows(LONG owner);

} // namespace Aesop

#endif

