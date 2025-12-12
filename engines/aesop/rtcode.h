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

void diagnose(int32 argcnt, uint32 dtype, uint32 parm);

typedef void (*FARPROC)();
extern FARPROC code_resources[];

uint32 absv(int32 argcnt, int32 val);
uint32 rnd(int32 argcnt, uint32 low, uint32 high);
int32 envval(int32 argcnt, BYTE *name);

void beep(void);

} // namespace Aesop

#endif
