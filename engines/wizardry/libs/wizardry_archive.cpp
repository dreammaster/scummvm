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

#include "wizardry/libs/file.h"
#include "wizardry/libs/wizardry_archive.h"

namespace Wizardry {

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
	if (fe)
		return new Common::MemoryReadStream(&fe->_data[0], fe->_data.size());

	return nullptr;
}

/*------------------------------------------------------------------------*/

namespace Uncompressed {

Wizardry1V1Archive::Wizardry1V1Archive() {
	// Cache a copy of the scenario data files
	if (File::exists("scenario.data")) {
		File f1("scenario.data");
		File f2("scenario.mesgs");
		_files.push_back(FileEntry("scenario.data", f1));
		_files.push_back(FileEntry("scenario.mesgs", f2));

	} else if (Common::File::exists("scenario.data#050000")) {
		File f1("scenario.data#050000");
		File f2("scenario.mesgs#050000");
		_files.push_back(FileEntry("scenario.data", f1));
		_files.push_back(FileEntry("scenario.mesgs", f2));
	} else {
		error("Could not locate scenario.data");
	}

	// Create separate file aliases for data subsets
	const FileEntry *sd = findFile("scenario.data");
	_files.push_back(FileEntry("font", &sd->_data[0] + 512, 64 * 8));
	_files.push_back(FileEntry("gfx_font", &sd->_data[0] + 1024, 64 * 8));
}

} // namespace Uncompressed

/*------------------------------------------------------------------------*/

namespace Compressed {

void WizardryV1Archive::DirEntry::load(Common::MemoryReadStream &src) {
	_firstBlock = src.readUint16LE();
	_lastBlock = src.readUint16LE();
	_fileKind = src.readUint16LE() & 0xff;

	byte strLen = src.readByte();
	char buf[15];
	src.read(buf, 14);
	buf[strLen] = '\0';
	_name = Common::String(buf);

	src.skip(5);
}

WizardryV1Archive::WizardryV1Archive() {
	Common::Array<byte> data;

	// Load the data
	File f("Wizardry-Proving-Grounds-Original-Scenario.dsk");

	data.resize(f.size());
	f.read(&data[0], data.size());

	// Parse the directory
	const byte *buf = &data[3 * BLOCK_SIZE + DISK_OFFSET];
	DirEntry dirEntry;

	Common::MemoryReadStream src(buf + 26, 256 - 26);
	const int count = READ_LE_UINT16(buf + 16);

	for (int ctr = 0; ctr < count; ++ctr) {
		dirEntry.load(src);

		if (dirEntry._fileKind >= BADBLK && dirEntry._fileKind <= FOTOFILE)
			_files.push_back(FileEntry(dirEntry._name,
				&data[dirEntry._firstBlock * BLOCK_SIZE - DISK_OFFSET],
				(dirEntry._lastBlock - dirEntry._firstBlock) * BLOCK_SIZE));
	}

	// Add in special entries for subsets of the other files
	const FileEntry *fe = findFile("scenario.data");
	_files.push_back(FileEntry("font1", &fe->_data[512], 64 * 8));
	_files.push_back(FileEntry("font2", &fe->_data[1024], 64 * 8));
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
	Common::Array<byte> data;

	// Load the data
	Common::File f;
	if (!f.open("wiz1.dsk"))
		error("Could not open - wiz1.dsk");

	data.resize(f.size());
	f.read(&data[0], f.size());

	// Parse the directory
	const byte *buf = &data[2 * BLOCK_SIZE];
	DirEntry dirEntry;

	Common::MemoryReadStream src(buf, 26 * 20);
	dirEntry.load(src);		// Skip over the WIZBOOT entry

	for (;;) {
		dirEntry.load(src);
		if (dirEntry.SIGNATURE != 0x6776)
			break;

		if (dirEntry.FILEKIND.FT >= BADBLK && dirEntry.FILEKIND.FT <= FOTOFILE)
			_files.push_back(FileEntry(dirEntry.FILENAME,
				&data[dirEntry.FIRSTBLK * BLOCK_SIZE],
				(dirEntry.LASTBLK - dirEntry.FIRSTBLK) * BLOCK_SIZE));
	}
}

} // namespace Compressed
} // namespace Wizardry
