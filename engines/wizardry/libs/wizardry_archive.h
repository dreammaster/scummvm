
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

#ifndef WIZARDRY_LIBS_WIZARDRY_ARCHIVE_H
#define WIZARDRY_LIBS_WIZARDRY_ARCHIVE_H

#include "common/archive.h"
#include "common/memstream.h"

namespace Wizardry {

class WizardryArchive : public Common::Archive {
protected:
	enum FileType {
		VOLHEAD, BADBLK, MACH6502, TEXT, DEBUG,
		DATA, GRAFFILE, FOTOFILE, SUBDIR
	};

	struct FileEntry {
		const Common::String _filename;
		uint _offset = 0, _size = 0;

		FileEntry() {
		}
		FileEntry(const Common::String &filename, uint offset, uint size) :
			_filename(filename), _offset(offset), _size(size) {
		}
	};
	Common::Array<byte> _data;
	Common::Array<FileEntry> _files;

	const FileEntry *findFile(const Common::Path &path) const;

public:
	WizardryArchive() {}

	int listMembers(Common::ArchiveMemberList &list) const override {
		return 0;
	}
	bool hasFile(const Common::Path &path) const override;
	const Common::ArchiveMemberPtr getMember(const Common::Path &path) const override;
	Common::SeekableReadStream *createReadStreamForMember(const Common::Path &path) const override;
};

class WizardryV1Archive : public WizardryArchive {
	const int BLOCK_SIZE = 256;

	struct DirEntry {
		uint16 _firstBlock;
		uint16 _lastBlock;
		uint8 _fileKind;
		Common::String _name;

		void load(Common::MemoryReadStream &src);
	};
public:
	WizardryV1Archive();
};

class WizardryV2Archive : public WizardryArchive {
	const int BLOCK_SIZE = 512;

	struct DirEntry {
		uint16 FIRSTBLK = 0;
		uint16 LASTBLK = 0;
		struct {
			FileType FT = VOLHEAD;
		} FILEKIND;
		Common::String FILENAME;
		uint16 SIGNATURE = 0;
		uint16 DISK_FLAGS = 0;
		uint16 CHECKSUM = 0;

		void load(Common::MemoryReadStream &src);
	};
public:
	WizardryV2Archive();
};

} // namespace Wizardry

#endif
