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

#include "legend/file.h"
#include "legend/legend.h"

namespace Legend {

#define ERROR error("Could not open file - %s", name.c_str())

File::File(const Common::String &name) : Common::File() {
	if (!Common::File::open(name))
		ERROR;
}

bool File::open(const Common::String &name) {
	if (!Common::File::open(name))
		ERROR;
	return true;
}

bool File::open(const Common::String &name, Common::Archive &archive) {
	if (!Common::File::open(name, archive))
		ERROR;
	return true;
}

bool File::open(const Common::FSNode &node) {
	Common::String name = node.getName();
	if (!Common::File::open(node))
		ERROR;
	return true;
}

bool File::open(SeekableReadStream *stream, const Common::String &name) {
	if (!Common::File::open(stream, name))
		ERROR;
	return true;
}

void File::open(FileType fileType, int fileNumber) {
	Common::String name = getFilename(fileType, fileNumber);
	open(name);
}

#undef ERROR

Common::String File::getFilename(FileType fileType, int fileNumber) {
	Common::String prefix = _vm->getGameFilePrefix();

	const char *const EXTENSIONS[] = { "PIC", "RGN", "FNT", "MUS", "SAV", "SAV" };
	return Common::String::format("%s_%02d.%s", prefix.c_str(), fileNumber,
		EXTENSIONS[fileType]);
}

} // End of namespace Legend
