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
#include "wasteland/fod/data/archive.h"

namespace Wasteland {
namespace FOD {
namespace Data {

const byte Archive::MAP_DISKS[29] = {
	0, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 2, 3, 2,
	3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2
};

Archive::DiskEntry::DiskEntry() {
	_files[0] = _files[1] = _files[2] = nullptr;
}

Archive::DiskEntry::DiskEntry(FileContents *map, FileContents *scr, FileContents *ani) {
	_files[0] = map;
	_files[1] = scr;
	_files[2] = ani;
}

Archive::Archive() : Common::Archive() {
	const char *const FILENAMES[9] = {
		"kmap", "kscr", "kani", "fmap", "fcr", "fani", "dmap", "dscr", "dani"
	};

	// Loop to load the table of contents for each of the files
	for (int fileCtr = 0; fileCtr < 9; ++fileCtr) {
		Common::File f;
		auto &file = _files[fileCtr];

		// Open up the file
		file._filename = FILENAMES[fileCtr];
		if (!f.open(FILENAMES[fileCtr]))
			error("Error opening %s", FILENAMES[fileCtr]);

		// Read it's table of contents
		file._contents.resize((fileCtr % 3) == 2 ? 53 : 28);
		for (uint i = 0; i < file._contents.size(); ++i) {
			file._contents[i]._offset = (size_t)f.readUint16LE() * 4;
			file._contents[i]._size = f.readUint16LE();
		}

		f.close();
	}

	// Set up the disk records to point to the appropriate files
	_disks[0] = Archive::DiskEntry(&_files[0], &_files[1], &_files[2]);
	_disks[1] = Archive::DiskEntry(&_files[0], &_files[1], &_files[2]);
	_disks[2] = Archive::DiskEntry(&_files[3], &_files[4], &_files[5]);
	_disks[3] = Archive::DiskEntry(&_files[6], &_files[7], &_files[8]);
}

bool Archive::hasFile(const Common::Path &path) const {
	Common::String name = path.baseName();

	if (name.size() < 4 || !Common::isDigit(name[3]))
		return false;

	return name.hasPrefixIgnoreCase("MAP") ||
		name.hasPrefixIgnoreCase("SCR") ||
		name.hasPrefixIgnoreCase("ANI");
}

int Archive::listMembers(Common::ArchiveMemberList &list) const {
	return 0;
}

const Common::ArchiveMemberPtr Archive::getMember(const Common::Path &path) const {
	return Common::ArchiveMemberPtr();
}

Common::SeekableReadStream *Archive::createReadStreamForMember(
		const Common::Path &path) const {
	return nullptr;
}

} // namespace Data
} // namespace FOD
} // namespace Wasteland
