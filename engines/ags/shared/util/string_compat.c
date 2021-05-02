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

#include "ags/shared/util/string_compat.h"
#include "ags/shared/core/platform.h"

namespace AGS3 {

char *ags_strlwr(char *s) {
	char *p = s;
	for (; *p; p++)
		*p = tolower(*p);
	return s;
}

char *ags_strupr(char *s) {
	char *p = s;
	for (; *p; p++)
		*p = toupper(*p);
	return s;
}

int ags_stricmp(const char *s1, const char *s2) {
#if AGS_PLATFORM_OS_WINDOWS
	return stricmp(s1, s2);
#else
	return strcasecmp(s1, s2);
#endif
}

int ags_strnicmp(const char *s1, const char *s2, size_t n) {
#if AGS_PLATFORM_OS_WINDOWS
	return strnicmp(s1, s2, n);
#else
	return strncasecmp(s1, s2, n);
#endif
}

char *ags_strdup(const char *s) {
	char *result = (char *)malloc(strlen(s) + 1);
	strcpy(result, s);
	return result;
}

} // namespace AGS3
