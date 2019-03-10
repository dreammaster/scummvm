/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
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

#ifndef DUNGEONCRAFT_CDX_CDXTYPES
#define DUNGEONCRAFT_CDX_CDXTYPES

namespace DungeonCraft {
namespace CDX {

/////////////////////////////////////////////////////////////////////
// signed types
/////////////////////////////////////////////////////////////////////
#undef INT8
#define INT8 signed char

#undef INT16
#define INT16 signed short

#undef INT32
#define INT32 signed long

#undef INT64
#define INT64 __int64


/////////////////////////////////////////////////////////////////////
// unsigned types
/////////////////////////////////////////////////////////////////////
#undef UINT8
#define UINT8 unsigned char

#undef UINT16
#define UINT16 unsigned short

#undef UINT32
#define UINT32 unsigned long

#undef UINT64
#define UINT64 unsigned __int64


#undef CDXBOOL
#define CDXBOOL int

} // End of namespace CDX
} // End of namespace DungeonCraft

#endif
