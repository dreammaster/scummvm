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

#ifndef SPYCRAFT_DMADE_DUCK_PLATFORM_H
#define SPYCRAFT_DMADE_DUCK_PLATFORM_H

namespace Spycraft {

#define DOS_WINDOWS

#define DISPLAYDIB 0

#define SWAPENDS 0      
#define HW_CD_BUFFER 0  
#define CD_ONLY 0

#define duck_close close
#define duck_read read 
#define duck_seek lseek
#define slow_seek lseek

int duck_open(const char *fname);

#if defined(SCUMM_BIG_ENDIAN)
short invert2(unsigned short);
long invert4(unsigned long);

#define highByte(x) (BYTE)x
#define mid2Byte(x) (BYTE)(x >> 8)
#define mid1Byte(x) (BYTE)(x >> 16)
#define lowByte(x) (BYTE)(x >> 24)

#else

#define invert2(x) x
#define invert4(x) x

#define lowByte(x) (BYTE)x
#define mid1Byte(x) (BYTE)(x >> 8)
#define mid2Byte(x) (BYTE)(x >> 16)
#define highByte(x) (BYTE)(x >> 24)

#endif

} // namespace Spycraft

#endif
