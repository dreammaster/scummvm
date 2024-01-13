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

#ifndef WASTELAND_FOD_DATA_ARCHIVE_H
#define WASTELAND_FOD_DATA_ARCHIVE_H

#include "common/archive.h"
#include "common/array.h"
#include "common/path.h"

namespace Wasteland {
namespace FOD {
namespace Data {

/**
 * Currently deprecated
 */
class Archive : public Common::Archive {
	struct FileEntry {
		size_t _offset = 0;
		size_t _size = 0;
	};
	struct FileContents {
		Common::Path _filename;
		Common::Array<FileEntry> _contents;
	};
	struct DiskEntry {
		FileContents *_files[3]; // map, scr, ani

		DiskEntry();
		DiskEntry(FileContents *map, FileContents *scr, FileContents *ani);
	};

private:
	static const byte MAP_DISKS[29];
	FileContents _files[9];
	DiskEntry _disks[4];

public:
	Archive();
	~Archive() override {}

	/**
	 * Check if a member with the given name is present in the Archive.
	 * Patterns are not allowed, as this is meant to be a quick File::exists()
	 * replacement.
	 */
	bool hasFile(const Common::Path &path) const override;

	/**
	 * Add all members of the Archive to list.
	 * Must only append to list, and not remove elements from it.
	 *
	 * @return the number of names added to list
	 */
	int listMembers(Common::ArchiveMemberList &list) const override;

	/**
	 * Returns a ArchiveMember representation of the given file.
	 */
	const Common::ArchiveMemberPtr getMember(const Common::Path &path)
		const override;

	/**
	 * Create a stream bound to a member with the specified name in the
	 * archive. If no member with this name exists, 0 is returned.
	 * @return the newly created input stream
	 */
	Common::SeekableReadStream *createReadStreamForMember(
		const Common::Path &path) const override;
};

} // namespace Data
} // namespace FOD
} // namespace Wasteland

#endif
