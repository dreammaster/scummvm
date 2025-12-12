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

#ifndef AESOP_RTCODE_H
#define AESOP_RTCODE_H

namespace Aesop {

#ifdef __cplusplus
extern "C" {
#endif

void cdecl diagnose(LONG argcnt, ULONG dtype, ULONG parm);

typedef void (cdecl *FARPROC)();
extern FARPROC code_resources[];

ULONG cdecl absv(LONG argcnt, LONG val);
ULONG cdecl rnd(LONG argcnt, ULONG low, ULONG high);
LONG cdecl envval(LONG argcnt, BYTE *name);

void cdecl beep(void);

#ifdef __cplusplus
}
#endif

} // namespace Aesop

#endif
