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

#ifndef AGS2_COMMON_MISC_H
#define AGS2_COMMON_MISC_H

#include "common/stream.h"

namespace AGS2 {

extern Common::WriteStream *ci_fopen_w(const char *file_name, const char *mode);
Common::SeekableReadStream *ci_fopen(const char *file_name, const char *mode);

char *ci_find_file(const char *dir_name, const char *file_name);

inline void ags_strlwr(char *str) {
	while (*str)
		*str++ = tolower(*str);
}

inline void ags_strupr(char *str) {
	while (*str)
		*str++ = toupper(*str);
}

} // namespace AGS2

#endif
