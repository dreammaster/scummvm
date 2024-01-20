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

#ifndef WASTELAND_FOD_DATA_DISK_H
#define WASTELAND_FOD_DATA_DISK_H

#include "common/file.h"
#include "wasteland/fod/data/map.h"

namespace Wasteland {
namespace FOD {
namespace Data {

#define MAP_CONTENTS_COUNT 28
#define ANI_CONTENTS_COUNT 53

class Disk {
	struct DiskEntry {
		const char *_name;
		const char *_disk;
		const char *_map;
		const char *_scr;
		const char *_ani;
	};
	struct FileEntry {
		size_t _offset;
		size_t _size;
	};
private:
	static const byte MAP_DISKS[29];
	static const DiskEntry DISKS[4];

	const DiskEntry *_currentDisk = nullptr;
	int _currentDiskNum = 0;
	FileEntry _mapContents[MAP_CONTENTS_COUNT];
	FileEntry _scrContents[MAP_CONTENTS_COUNT];
	FileEntry _aniContents[ANI_CONTENTS_COUNT];
	int _scriptsArray[3];
	int _scriptsCtr;

	void saveMap();
	void loadFileContents(const Common::Path &path, FileEntry *table, size_t count);
	Common::SeekableReadStream *getFile(const Common::Path &path, const FileEntry *table,
		int index);

public:
	Map _map;

	/**
	 * Load a specified map
	 */
	void loadMap(int mapNum);

	void processMap();

	void loadScripts();
};

} // namespace Data
} // namespace FOD
} // namespace Wasteland

#endif
