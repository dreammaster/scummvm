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

#ifndef KRONDOR_FILES_RESOURCE_ARCHIVE_H
#define KRONDOR_FILES_RESOURCE_ARCHIVE_H

#include "common/archive.h"
#include "common/hashmap.h"
#include "common/rect.h"
#include "common/serializer.h"
#include "common/stream.h"

namespace Krondor {

/**
 * Betrayal at Krondor resource archive
 */
class ResourceArchive : public Common::Archive {
	/**
	 * Details of a single entry in a CC file index
	 */
	struct ResourceEntry {
		uint32 _hashKey = 0;
		uint32 _offset = 0;
		uint32 _size = 0;

		ResourceEntry() : _hashKey(0), _offset(0), _size(0) {
		}
		ResourceEntry(uint16 hashKey, uint32 offset, uint32 size)
			: _hashKey(hashKey), _offset(offset), _size(size) {
		}
	};

private:
	typedef Common::HashMap<Common::String, ResourceEntry,
		Common::IgnoreCase_Hash, Common::IgnoreCase_EqualTo> ResourceHash;
	ResourceHash _index;

	ResourceArchive() {}
public:
	static bool initialize();

	// Archive implementation
	bool hasFile(const Common::Path &path) const override;
	int listMembers(Common::ArchiveMemberList &list) const override;
	const Common::ArchiveMemberPtr getMember(const Common::Path &path) const override;
	Common::SeekableReadStream *createReadStreamForMember(const Common::Path &path) const override;
};

} // namespace Krondor

#endif
