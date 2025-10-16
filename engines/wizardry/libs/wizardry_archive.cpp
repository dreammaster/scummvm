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
#include "wizardry/libs/wizardry_archive.h"

namespace Wizardry {

constexpr int BLOCK_SIZE = 512;

const WizardryArchive::FileEntry *WizardryArchive::findFile(const Common::Path &path) const {
	for (const FileEntry &fe : _files) {
		if (fe._filename.equalsIgnoreCase(path.baseName()))
			return &fe;
	}

	return nullptr;
}

bool WizardryArchive::hasFile(const Common::Path &path) const {
	return findFile(path) != nullptr;
}

const Common::ArchiveMemberPtr WizardryArchive::getMember(const Common::Path &path) const {
	return Common::ArchiveMemberPtr(new Common::GenericArchiveMember(path, *this));
}

Common::SeekableReadStream *WizardryArchive::createReadStreamForMember(const Common::Path &path) const {
	const FileEntry *fe = findFile(path);

	if (fe) {
		return new Common::MemoryReadStream(&_data[fe->_blockStart * BLOCK_SIZE],
			(fe->_blockEnd - fe->_blockStart + 1) * BLOCK_SIZE);
	}

	return nullptr;
}

/*------------------------------------------------------------------------*/

WizardryV1Archive::WizardryV1Archive() {
}

/*------------------------------------------------------------------------*/

void WizardryV2Archive::DirEntry::load(Common::MemoryReadStream &src) {
	FIRSTBLK = src.readUint16LE();
	LASTBLK = src.readUint16LE();
	FILEKIND.FT = (FileType)src.readByte();
	src.skip(1);

	byte len = src.readByte();
	char buf[14];
	assert(len <= 13);
	src.read(buf, 13);
	buf[len] = '\0';
	FILENAME = Common::String(buf);

	SIGNATURE = src.readUint16LE();
	DISK_FLAGS = src.readUint16LE();
	CHECKSUM = src.readUint16LE();
}

WizardryV2Archive::WizardryV2Archive() {
	// Load the data
	Common::File f;
	if (!f.open("wiz1.dsk"))
		error("Could not open - wiz1.dsk");

	_data.resize(f.size());
	f.read(&_data[0], f.size());

	// Parse the directory
	const byte *buf = &_data[2 * BLOCK_SIZE];
	DirEntry dirEntry;

	Common::MemoryReadStream src(buf, 26 * 20);
	dirEntry.load(src);		// Skip over the WIZBOOT entry

	for (;;) {
		dirEntry.load(src);
		if (dirEntry.SIGNATURE != 0x6776)
			break;

		if (dirEntry.FILEKIND.FT >= BADBLK && dirEntry.FILEKIND.FT <= FOTOFILE)
			_files.push_back(FileEntry(dirEntry.FILENAME, dirEntry.FIRSTBLK, dirEntry.LASTBLK));
	}
}

} // namespace Wizardry
