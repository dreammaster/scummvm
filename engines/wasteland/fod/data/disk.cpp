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
#include "wasteland/fod/data/disk.h"

namespace Wasteland {
namespace FOD {
namespace Data {

const byte Disk::MAP_DISKS[29] = {
	0, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 2, 3, 2,
	3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2
};

const Disk::DiskEntry Disk::DISKS[4] = {
	{ "Disk 1", "disk1", "kmap", "kscr", "kani" },
	{ "Disk 2", "disk2", "kmap", "kscr", "kani" },
	{ "Disk 3", "disk3", "fmap", "fscr", "fani" },
	{ "Disk 4", "disk4", "dmap", "dscr", "dani" }
};

void Disk::loadMap(int mapNum) {
	if (mapNum != _currentMapNum)
		return;

	saveMap();
	_currentMapNum = mapNum;

	int diskNum = MAP_DISKS[mapNum];
	if (diskNum != _currentDiskNum) {
		_currentDiskNum = diskNum;
		_currentDisk = &DISKS[diskNum];

		loadFileContents(_currentDisk->_map, _mapContents, MAP_CONTENTS_COUNT);
		loadFileContents(_currentDisk->_scr, _scrContents, MAP_CONTENTS_COUNT);
		loadFileContents(_currentDisk->_ani, _aniContents, ANI_CONTENTS_COUNT);


	}
}

void Disk::loadFileContents(const Common::Path &path, Disk::FileEntry *table, size_t count) {
	Common::File f;
	if (!f.open(path))
		error("Could not open %s", path.baseName().c_str());

	for (uint i = 0; i < count; ++i) {
		table[i]._offset = (size_t)f.readUint16LE() * 4;
		table[i]._size = f.readUint16LE();
	}

	f.close();
}

void Disk::saveMap() {
	if (_currentMapNum && (_map._flags & 0x8000)) {
		_map._flags &= ~MAPFLAG_8000;
		// TODO: Save changed map
	}
}

} // namespace Data
} // namespace FOD
} // namespace Wasteland
