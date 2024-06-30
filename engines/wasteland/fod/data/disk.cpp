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
#include "wasteland/fod/core/decoder.h"
#include "wasteland/fod/data/disk.h"
#include "wasteland/fod/fod.h"

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

Disk::Disk(uint16 &mapX, uint16 &mapY) : _map(mapX, mapY) {
}

void Disk::loadMap(int mapNum) {
	Common::SeekableReadStream *src;
	if (mapNum == _map._mapNum)
		return;

	saveMap();
	_map._mapNum = mapNum;

	int diskNum = MAP_DISKS[mapNum];
	if (diskNum != _currentDiskNum) {
		_currentDiskNum = diskNum;
		_currentDisk = &DISKS[diskNum];

		loadFileContents(_currentDisk->_map, _mapContents, MAP_CONTENTS_COUNT);
		loadFileContents(_currentDisk->_scr, _scrContents, MAP_CONTENTS_COUNT);
		loadFileContents(_currentDisk->_ani, _aniContents, ANI_CONTENTS_COUNT);
	}

	// Load the map
	src = getFile(_currentDisk->_map, _mapContents, mapNum - 1);
	_map.load(*src);
	delete src;

	applyMapOverrides();
	loadScripts();
}

void Disk::applyMapOverrides() {
	TileOverrides &overrides = g_engine->_disk1._tileOverrides;
	int newCount = 0;

	for (uint tableCtr = 0; tableCtr < overrides.size(); ++tableCtr) {
		int entryIndex = tableCtr - newCount;
		const TileOverride &entry = overrides[entryIndex];

		if (entry._mapNum == _map._mapNum) {
			Map::MapTile &tile = _map._tiles[entry._x][entry._y];

			if (entry._flags & 0x80)
				tile._id = (tile._id & 0xf800) | entry._tile._id;

			if (entry._flags & 0x40) {
				tile._field3 = entry._tile._field3;
				tile._field4 = entry._tile._field4;
				tile._flags = (tile._flags & 0x80) | entry._tile._flags;
			}

			if (entry._flags & 0x20)
				tile._actionId = entry._tile._actionId;

			++newCount;

			if (((int)overrides.size() - 1) != entryIndex) {
				for (int idx = entryIndex; idx <= ((int)overrides.size() - newCount); ++idx)
					overrides[idx] = overrides[idx + 1];
			}
		}
	}

	if (newCount > 0) {
		overrides.resize(newCount);
		_map._flags |= MAPFLAG_8000;
	}
}

void Disk::loadScripts() {
	Common::fill(_scriptsArray, _scriptsArray + 3, -1);

	// Get and decode the scripts for the map
	Common::SeekableReadStream *src = getFile(_currentDisk->_scr,
		_scrContents, _map._mapNum - 1);
	Common::MemoryWriteStreamDynamic scripts(DisposeAfterUse::YES);
	Decoder::loadStream(*src, scripts);
	delete src;

	_scripts.resize(scripts.size());
	const byte *data = scripts.getData();
	Common::copy(data, data + scripts.size(), &_scripts[0]);
}

void Disk::loadFileContents(const Common::Path &path, Disk::FileEntry *table, size_t count) {
	Common::File f;
	if (!f.open(path))
		error("Could not open %s", path.baseName().c_str());

	for (uint i = 0; i < count; ++i) {
		table[i]._offset = (size_t)f.readUint16LE() * 16;
		table[i]._size = f.readUint16LE();
	}

 	f.close();
}

Common::SeekableReadStream *Disk::getFile(const Common::Path &path,
		const FileEntry *table, int index) {
	Common::File f;
	if (!f.open(path))
		error("Error opening file");

	f.seek(table[index]._offset);
	Common::SeekableReadStream *result = f.readStream(table[index]._size);
	f.close();

	return result;
}


void Disk::saveMap() {
	if (_map._mapNum && (_map._flags & 0x8000)) {
		_map._flags &= ~MAPFLAG_8000;
		// TODO: Save changed map
	}
}

} // namespace Data
} // namespace FOD
} // namespace Wasteland
