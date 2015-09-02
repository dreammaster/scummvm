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
//  Run-time code resources
//

#ifndef AESOP_RTCODE_H
#define AESOP_RTCODE_H

namespace Aesop {

void diagnose(LONG argcnt, ULONG dtype, ULONG parm);

extern ULONG diag_flag;

extern ULONG absv(LONG argcnt, LONG val);
extern ULONG rnd(LONG argcnt, ULONG low, ULONG high);
extern LONG envval(LONG argcnt, BYTE *name);

void beep(void);

} // End of namespace Aesop

#endif

