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

#ifndef SPYCRAFT_DMADE_ADVLIB_H
#define SPYCRAFT_DMADE_ADVLIB_H

#include "common/scummsys.h"
#include "spycraft/dmade/advcompat.h"
#include "spycraft/dmade/advdebug.h"
#include "spycraft/dmade/adverror.h"
#include "spycraft/dmade/advmem.h"

namespace Spycraft {

/* common types */
typedef float float32;			// 32 bit floating point type
typedef double float64;			// 64 bit floating point type
typedef uint32 BitField;		// Base type for bit-fields

struct MADEPt {
	int16 x;				// x-coordinate of point
	int16 y;				// y-coordinate of point
};

struct MADERect {
	int16 top;				// y-coordinate of top-left point
	int16 left;				// x-coordinate of top-left point
	int16 bottom;			// y-coordinate of bottom-right point
	int16 right;			// x-coordinate of bottom-right point
};

/* common macros */

/* windows color macros */
#define madeRGB(r, g, b) ((uint32)(((uint8)(r)|((uint16)(g)<<8))|(((uint32)(uint8)(b))<<16)))

/* 24 bit windows color macros */
#define sfxColor24(r, g, b) \
	((uint32)	((uint32)(r))		| \
				((uint32)(g) << 8)	| \
				((uint32)(b) << 16) )
#define sfxRed24(c) ((uint8)((c) & 0xff))
#define sfxGreen24(c) ((uint8)(((c) >> 8) & 0xff))
#define sfxBlue24(c) ((uint8)(((c) >> 16) & 0xff))

/* 16 bit windows color macros */
#define sfxColor16(r, g, b) \
	((uint16)	((uint16)(r))		| \
				((uint16)(g) << 5)	| \
				((uint16)(b) << 10) )
#define sfxRed16(c) ((uint8)((c) & 0x1f))
#define sfxGreen16(c) ((uint8)(((c) >> 5) & 0x1f))
#define sfxBlue16(c) ((uint8)(((c) >> 10) & 0x1f))

} // namespace Spycraft

#endif
