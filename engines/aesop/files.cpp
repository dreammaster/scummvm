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
#include "common/savefile.h"
#include "common/system.h"
#include "aesop/files.h"

namespace Aesop {

Common::Stream *open(const char *filename, int flags) {
	Common::Stream *file = nullptr;
	assert(!(flags & O_APPEND));

	if (flags == ATF_WRITE) {
		file = g_system->getSavefileManager()->openForSaving(filename, false);
	} else {
		file = g_system->getSavefileManager()->openForLoading(filename);

		if (!file) {
			Common::File *f = new Common::File();
			if (f->open(filename)) {
				file = f;
			} else {
				delete f;
			}
		}
	}

	return file;
}

void close(Common::Stream *&file) {
	delete file;
	file = nullptr;
}

size_t read(Common::Stream *file, void *buffer, size_t size) {
	Common::ReadStream *rs = dynamic_cast<Common::ReadStream *>(file);
	return rs ? rs->read(buffer, size) : 0;
}

size_t write(Common::Stream *file, void *buffer, size_t size) {
	Common::WriteStream *ws = dynamic_cast<Common::WriteStream *>(file);
	assert(ws);
	return ws->write(buffer, size);
}

} // namespace Aesop
