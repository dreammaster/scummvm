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

#include "common/archive.h"
#include "common/config-manager.h"
#include "common/file.h"
#include "common/translation.h"
#include "common/compression/unzip.h"
#include "legend/utils/engine_data.h"

#define DATA_FILENAME "legend.dat"

namespace Legend {

class DataArchiveMember : public Common::ArchiveMember {
private:
	Common::SharedPtr<Common::ArchiveMember> _member;
	Common::String _publicFolder;
	Common::String _innerfolder;
public:
	DataArchiveMember(Common::SharedPtr<Common::ArchiveMember> member,
		const Common::String &subfolder, const Common::String &publicFolder) :
			_member(member), _publicFolder(publicFolder), _innerfolder(subfolder) {
	}
	~DataArchiveMember() override {
	}
	Common::SeekableReadStream *createReadStream() const override {
		return _member->createReadStream();
	}
	Common::String getName() const override {
		Common::String name = _member->getName();
		assert(name.hasPrefixIgnoreCase(_innerfolder));
		return _publicFolder + Common::String(name.c_str() + _innerfolder.size());
	}
	Common::U32String getDisplayName() const override {
		return _member->getDisplayName();
	}
};

/**
 * The data archive class encapsulates access to a specific subfolder
 * for the game within the engine data zip file as a generic "data" folder
 */
class DataArchive : public Common::Archive {
	struct Pair {
		Common::String _name1, _name2;
	};
private:
	Common::Archive *_zip;
	Common::String _publicFolder;
	Common::String _innerFolder1, _innerFolder2;

	Pair innerToPublic(const Common::String &filename) const {
		assert(filename.hasPrefixIgnoreCase(_publicFolder));
		Pair p;
		p._name1 = _innerFolder1 + Common::String(filename.c_str() + _publicFolder.size());
		p._name2 = _innerFolder1 + Common::String(filename.c_str() + _publicFolder.size());
		return p;
	}
public:
	DataArchive(Common::Archive *zip, const Common::String &subFolder1,
			Common::String &subFolder2, bool useDataPrefix) :
			_zip(zip), _publicFolder(useDataPrefix ? "data/" : ""),
			_innerFolder1(subFolder1 + "/"), _innerFolder2(subFolder2 + "/") {
	}
	~DataArchive() override {
		delete _zip;
	}

	/**
	 * Check if a member with the given name is present in the Archive.
	 * Patterns are not allowed, as this is meant to be a quick File::exists()
	 * replacement.
	 */
	bool hasFile(const Common::Path &path) const override;

	/**
	 * Add all members of the Archive matching the specified pattern to list.
	 * Must only append to list, and not remove elements from it.
	 *
	 * @return the number of members added to list
	 */
	int listMatchingMembers(Common::ArchiveMemberList &list, const Common::Path &pattern,
		bool matchPathComponents = false) const override;

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

#ifndef RELEASE_BUILD

/**
 * The data archive proxy class is used for debugging purposes to access engine data
 * files when the create_mm folder is in the search path. It will allow for
 * local mucking around with the data files and committing changes without having to
 * recreate the data file every time a change is made. mm.dat then just has
 * to be recreated prior to a release or when the changes are completed and stable
 */
class DataArchiveProxy : public Common::Archive {
	friend class DataArchive;
private:
	Common::FSNode _folder1, _folder2;
	const Common::String _publicFolder;

	/**
	 * Gets a file node from the passed filename
	 */
	Common::FSNode getNode(const Common::String &name) const;
public:
	DataArchiveProxy(const Common::FSNode &folder1, const Common::FSNode &folder2,
		bool useDataPrefix) :
		_folder1(folder1), _folder2(folder2), _publicFolder(useDataPrefix ? "data/" : "") {}
	~DataArchiveProxy() override {}

	/**
	 * Check if a member with the given name is present in the Archive.
	 * Patterns are not allowed, as this is meant to be a quick File::exists()
	 * replacement.
	 */
	bool hasFile(const Common::Path &path) const override {
		Common::String name = path.toString();
		return name.hasPrefixIgnoreCase(_publicFolder) && getNode(name).exists();
	}

	/**
	 * Add all members of the Archive matching the specified pattern to list.
	 * Must only append to list, and not remove elements from it.
	 *
	 * @return the number of members added to list
	 */
	int listMatchingMembers(Common::ArchiveMemberList &list, const Common::Path &pattern,
			bool matchPathComponents = false) const override {
		return 0;
	}

	/**
	 * Add all members of the Archive to list.
	 * Must only append to list, and not remove elements from it.
	 *
	 * @return the number of names added to list
	 */
	int listMembers(Common::ArchiveMemberList &list) const override {
		return 0;
	}

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

#endif

bool DataArchive::hasFile(const Common::Path &path) const {
	Common::String name = path.toString();
	if (!name.hasPrefixIgnoreCase(_publicFolder))
		return false;

	Pair pair = innerToPublic(name);
	return _zip->hasFile(pair._name1) || _zip->hasFile(pair._name2);
}

int DataArchive::listMatchingMembers(Common::ArchiveMemberList &list, const Common::Path &pattern,
		bool matchPathComponents) const {
	return 0;
}

int DataArchive::listMembers(Common::ArchiveMemberList &list) const {
	return 0;
}

const Common::ArchiveMemberPtr DataArchive::getMember(const Common::Path &path) const {
	Common::String name = path.toString();
	if (!hasFile(name))
		return Common::ArchiveMemberPtr();

	return Common::ArchiveMemberPtr(new Common::GenericArchiveMember(name, *this));
}

Common::SeekableReadStream *DataArchive::createReadStreamForMember(const Common::Path &path) const {
	Common::SeekableReadStream *rs = nullptr;
	Common::String name = path.toString();

	if (hasFile(name)) {
		Pair pair = innerToPublic(name);
		rs = _zip->createReadStreamForMember(pair._name1);
		if (!rs)
			rs = _zip->createReadStreamForMember(pair._name2);
	}

	return rs;
}

/*------------------------------------------------------------------------*/

#ifndef RELEASE_BUILD

const Common::ArchiveMemberPtr DataArchiveProxy::getMember(const Common::Path &path) const {
	Common::String name = path.toString();
	if (!hasFile(name))
		return Common::ArchiveMemberPtr();

	return Common::ArchiveMemberPtr(new Common::GenericArchiveMember(name, *this));
}

Common::SeekableReadStream *DataArchiveProxy::createReadStreamForMember(const Common::Path &path) const {
	Common::String name = path.toString();
	if (hasFile(name))
		return getNode(name).createReadStream();

	return nullptr;
}

Common::FSNode DataArchiveProxy::getNode(const Common::String &name) const {
	size_t pos;

	Common::FSNode node = _folder1;
	for (int i = 0; i < 2; ++i, node = _folder2) {
		Common::String remainingName = name.substr(_publicFolder.size());

		while ((pos = remainingName.findFirstOf('/')) != Common::String::npos) {
			node = node.getChild(remainingName.substr(0, pos));
			if (!node.exists())
				break;

			remainingName = remainingName.substr(pos + 1);
		}

		if (!remainingName.empty())
			node = node.getChild(remainingName);

		if (node.exists())
			break;
	}

	return node;
}

#endif

/*------------------------------------------------------------------------*/

bool load_engine_data(const Common::String &subfolder, bool isEarlyGame,
		int reqMajorVersion, int reqMinorVersion, Common::U32String &errorMsg, bool useDataPrefix) {
	Common::Archive *dataArchive = nullptr;
	Common::File f;
	Common::String commonFolder(isEarlyGame ? "early" : "later");

#ifndef RELEASE_BUILD
	Common::FSNode rootFolder, folder1, folder2;
	if (ConfMan.hasKey("extrapath")) {
		if ((rootFolder = Common::FSNode(ConfMan.get("extrapath"))).exists()
				&& (rootFolder = rootFolder.getChild("files")).exists()) {
			folder2 = rootFolder.getChild(commonFolder);
			if ((folder1 = rootFolder.getChild(subfolder)).exists())
				f.open(folder1.getChild("version.txt"));
		}
	}

#endif
	if (!f.isOpen()) {
		if (!Common::File::exists(DATA_FILENAME) ||
			(dataArchive = Common::makeZipArchive(DATA_FILENAME)) == 0 ||
			!f.open(Common::String::format("%s/version.txt", subfolder.c_str()), *dataArchive)) {
			delete dataArchive;
			errorMsg = Common::U32String::format(_("Could not locate engine data %s"), DATA_FILENAME);
			return false;
		}
	}

	// Validate the version
	char buffer[5];
	f.read(buffer, 4);
	buffer[4] = '\0';

	int major = 0, minor = 0;
	if (buffer[1] == '.') {
		major = buffer[0] - '0';
		minor = atoi(&buffer[2]);
	}

	if (major != reqMajorVersion || minor != reqMinorVersion) {
		delete dataArchive;
		errorMsg = Common::U32String::format(_("Out of date engine data. Expected %d.%d, but got version %d.%d"),
			reqMajorVersion, reqMinorVersion, major, minor);
		return false;
	}

	// It was all validated correctly
	Common::Archive *archive;
#ifndef RELEASE_BUILD
	if (!dataArchive)
		archive = new DataArchiveProxy(folder1, folder2, useDataPrefix);
	else
#endif
		archive = new DataArchive(dataArchive, subfolder, commonFolder, useDataPrefix);

	SearchMan.add("data", archive);
	return true;
}

} // namespace MM
