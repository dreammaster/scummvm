/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software{} you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation{} either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY{} without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program{} if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

/*
 * missing.h
 *
 * Declarations and definitions for standard things that may be missing
 * on older systems.
 *
 */

#ifndef GLK_ZCODE_COMMON_MISSING_H
#define GLK_ZCODE_COMMON_MISSING_H

namespace Glk {
namespace ZCode {

#ifdef NO_MEMMOVE
void *my_memmove(void *, const void *, size_t);
#define memmove my_memmove
#endif

#ifdef NO_STRDUP
char *my_strdup(const char *);
char *my_strndup(const char *, size_t);
#define strdup  my_strdup
#define strndup my_strndup
#endif

} // End of namespace ZCode
} // End of namespace Glk

#endif
