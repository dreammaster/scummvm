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
#include "legend/file.h"

namespace Legend {
namespace Shannara {

Stream *fp_open(const char *filename, const char *mode) {
	assert(!strcmp(mode, "rb"));
	Common::File *f = new Common::File();

	if (f->open(filename)) {
		return f;
	} else {
		delete f;
		return nullptr;
	}
}

void fp_close(Stream *fp) {
	delete fp;
}

size_t fp_size(Stream *fp) {
	return fp->size();
}

void fp_set_bufsiz(Stream *fp, size_t bufSize) {
	// No implementation
}

size_t fp_read(Stream *fp, void *buf, size_t size) {
	return fp->read(buf, size);
}

int fp_seek(Stream *fp, int offset, int origin) {
	return fp->seek(offset, origin);
}

} // namespace Shannara
} // namespace Legend
