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

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/* Based on the Adventure Game Studio source code, copyright 1999-2011 Chris Jones,
 * which is licensed under the Artistic License 2.0.
 */

#ifndef AGS_RESOURCEMAN_H
#define AGS_RESOURCEMAN_H

#include "common/types.h"
#include "common/str.h"
#include "common/hashmap.h"
#include "common/file.h"

namespace Common {
	class SeekableReadStream;
}

namespace AGS {

class ResourceManager {
public:
	ResourceManager();
	~ResourceManager();

	/** Init the resource manager with the master archive. */
	bool init(const Common::String &masterArchive);

	/** Does that file exist in the game's archives? */
	bool hasFile(const Common::String &file) const;
	/** Get the specified archived file. */
	Common::SeekableReadStream *getFile(const Common::String &file) const;

	Common::Array<Common::String> getFilenames() const;

	/** Debug method to quickly dump an archived file. */
	bool dumpFile(const Common::String &file) const;

private:
	/** The master archive. */
	struct MasterArchive {
		Common::String name; ///< The master archive's file name.
		Common::File   file; ///< The master archive file.

		uint8 version; ///< The master archive's version.

		uint32 offset; ///< The offset the actual archive start at.

		MasterArchive(const Common::String n);
		~MasterArchive();
	};

	/** An archived file. */
	struct File {
		Common::String name; ///< The file name.

		uint32 archive; ///< Index into the archives.

		uint32 size;   ///< The file's size.
		uint32 offset; ///< The file's offset within the archive.
	};

	typedef Common::HashMap<Common::String, File *, Common::IgnoreCase_Hash, Common::IgnoreCase_EqualTo> FileMap;


	uint8 _libVersion; ///< The current game's library version.

	Common::Array<Common::File *> _archives; ///< The game's archives.
	Common::Array<File>           _files;    ///< The game's archived files.

	FileMap _fileMap; ///< A map over all archived files.


	// Open the master archive.
	bool openMasterArchive(MasterArchive &master);

	// Read the archive file lists, different engine versions.
	bool readArchiveList_v06(MasterArchive &master);
	bool readArchiveList_v10(MasterArchive &master);
	bool readArchiveList_v20(MasterArchive &master);
	bool readArchiveList_v21(MasterArchive &master);

	// Open the various archives.
	bool openArchives(MasterArchive &master, const Common::Array<Common::String> &archives);

	// Create the map over all archived files.
	void createFileMap();

	// Helpers
	static void getCString(Common::File &file, Common::String &str);
	static void decryptText(uint8 *str, uint32 max);
};

} // End of namespace AGS

#endif // AGS_RESOURCEMAN_H
