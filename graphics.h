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
//  Run-time graphics subs
//

#ifndef AESOP_GRAPHICS_H
#define AESOP_GRAPHICS_H

namespace Aesop {

extern UBYTE *fade_tables[5][16];
extern UWORD first_color[5];
extern UWORD in_GIL;

//#define MODE_X 1                 // 1 for VGA mode X, 0 for MCGA mode 13h

#define NTW 32                   // # of text windows available

extern void dprint(LONG argcnt, const char *format, ...);
extern void sprint(LONG argcnt, ULONG wndnum, const char *format, ...);
extern void aprint(LONG argcnt, const char *format, ...);
extern void print(LONG argcnt, ULONG wndnum, ULONG format, ...);
extern void text_color(LONG argcnt, ULONG wndnum, ULONG current, ULONG newColor);
extern void release_owned_windows(LONG owner);

} // End of namespace Aesop

#endif

