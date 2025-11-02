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

#include "common/file.h"
#include "ags2/common/misc.h"
#include "ags2/lib/allegro/file.h"

namespace AGS2 {

Common::WriteStream *ci_fopen_w(const char *file_name, const char *mode) {
	error("TODO: write ci_fopen");
}

Common::SeekableReadStream *ci_fopen(const char *file_name, const char *mode) {
	assert(!strcmp(mode, "r") || !strcmp(mode, "rb"));
	Common::File *f = new Common::File();
	if (f->open(file_name))
		return f;

	delete f;
	return nullptr;
}

char *ci_find_file(const char *dir_name, const char *file_name) {
	char *diamond = nullptr;

	if (dir_name == nullptr && file_name == nullptr)
		return nullptr;

	if (dir_name == nullptr) {
		size_t len = strlen(file_name);
		diamond = (char *)malloc(len + 3);
		Common::strcpy_s(diamond, len + 3, file_name);
	} else {
		diamond = (char *)malloc(strlen(dir_name) + strlen(file_name) + 2);
		append_filename(diamond, dir_name, file_name, strlen(dir_name) + strlen(file_name) + 2);
	}

	fix_filename_case(diamond);
	fix_filename_slashes(diamond);
	return diamond;
}

} // namespace AGS2
