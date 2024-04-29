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
#include "common/memstream.h"
#include "common/textconsole.h"
#include "krondor/files/resource_archive.h"

namespace Krondor {

#define RES_FILENAME_LEN 13
#define INDEX_FILENAME "krondor.rmf"
#define DATA_FILENAME "krondor.001"

bool ResourceArchive::initialize() {
	Common::File rmf, data;

	if (!rmf.open(INDEX_FILENAME) || !data.open(DATA_FILENAME))
		return false;
	if (rmf.readUint32LE() != 1 || rmf.readUint16LE() != 4)
		return false;

	// Create new archive
	ResourceArchive *archive = new ResourceArchive();

	// Validate the resource file
	char filename[RES_FILENAME_LEN + 1];
	rmf.read(filename, RES_FILENAME_LEN);
	filename[RES_FILENAME_LEN] = '\0';
	assert(Common::String(filename).equalsIgnoreCase(DATA_FILENAME));

	// Load the index contents
	int count = rmf.readUint16LE();

	for (int idx = 0; idx < count; ++idx) {
		uint32 hashKey = rmf.readUint32LE();
		uint32 offset = rmf.readUint32LE();
		data.seek(offset);

		// Get the filename and size from the entry in the data file
		data.read(filename, RES_FILENAME_LEN);
		filename[RES_FILENAME_LEN] = '\0';
		Common::String fname(filename);

		uint32 size = data.readUint32LE();

		archive->_index[fname] = ResourceEntry(hashKey, offset + RES_FILENAME_LEN + 4, size);
	}

	// Add the archive to the search manager
	SearchMan.add("resources", archive);
	return true;
}

bool ResourceArchive::hasFile(const Common::Path &path) const {
	return _index.contains(path.toString());
}

const Common::ArchiveMemberPtr ResourceArchive::getMember(const Common::Path &path) const {
	if (!hasFile(path))
		return Common::ArchiveMemberPtr();

	return Common::ArchiveMemberPtr(new Common::GenericArchiveMember(path, *this));
}

int ResourceArchive::listMembers(Common::ArchiveMemberList &list) const {
	int count = 0;

	for (ResourceHash::const_iterator i = _index.begin(); i != _index.end(); ++i) {
		list.push_back(Common::ArchiveMemberList::value_type(
			new Common::GenericArchiveMember(i->_key, *this)));
		++count;
	}

	return count;
}

Common::SeekableReadStream *ResourceArchive::createReadStreamForMember(const Common::Path &path) const {
	if (!_index.contains(path.toString()))
		return nullptr;

	const ResourceEntry &entry = _index[path.toString()];

	Common::File f;
	if (!f.open(DATA_FILENAME) || !f.seek(entry._offset))
		error("Could not open data file");

	// Read in the data
	byte *data = (byte *)malloc(entry._size);
	if (f.read(data, entry._size) != entry._size)
		error("Failed to read resource entry");

	// Return the data as a stream
	return new Common::MemoryReadStream(data, entry._size, DisposeAfterUse::YES);
}

} // namespace Krondor
