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

#include "ags/shared/util/stdio_compat.h"
#include "ags/shared/core/platform.h"
//include <sys/types.h>
//include <sys/stat.h>

#if AGS_PLATFORM_OS_WINDOWS
#define NOMINMAX
//include <windows.h>
//include <shlwapi.h>
#endif

namespace AGS3 {

int	 ags_fseek(FILE *stream, file_off_t offset, int whence) {
#if defined(HAVE_FSEEKO) // Contemporary POSIX libc
	return fseeko(stream, offset, whence);
#elif AGS_PLATFORM_OS_WINDOWS // MSVC
	return _fseeki64(stream, offset, whence);
#else // No distinct interface with off_t
	return fseek(stream, offset, whence);
#endif
}

file_off_t ags_ftell(FILE *stream) {
#if defined(HAVE_FSEEKO) // Contemporary POSIX libc
	return ftello(stream);
#elif AGS_PLATFORM_OS_WINDOWS // MSVC
	return _ftelli64(stream);
#else // No distinct interface with off_t
	return ftell(stream);
#endif
}

int  ags_file_exists(const char *path) {
#if AGS_PLATFORM_OS_WINDOWS
	return PathFileExistsA(path) && !PathIsDirectoryA(path);
#else
	struct stat path_stat;
	if (stat(path, &path_stat) != 0) {
		return 0;
	}
	return S_ISREG(path_stat.st_mode);
#endif
}

int ags_directory_exists(const char *path) {
#if AGS_PLATFORM_OS_WINDOWS
	return PathFileExistsA(path) && PathIsDirectoryA(path);
#else
	struct stat path_stat;
	if (stat(path, &path_stat) != 0) {
		return 0;
	}
	return S_ISDIR(path_stat.st_mode);
#endif
}

int ags_path_exists(const char *path) {
#if AGS_PLATFORM_OS_WINDOWS
	return PathFileExistsA(path);
#else
	struct stat path_stat;
	if (stat(path, &path_stat) != 0) {
		return 0;
	}
	return S_ISREG(path_stat.st_mode) || S_ISDIR(path_stat.st_mode);
#endif
}

file_off_t ags_file_size(const char *path) {
	struct stat path_stat;
	if (stat(path, &path_stat) != 0) {
		return -1;
	}
	return path_stat.st_size;
}

} // namespace AGS3
