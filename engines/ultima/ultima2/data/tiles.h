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

#ifndef ULTIMA2_DATA_TILES_H
#define ULTIMA2_DATA_TILES_H

#include "common/archive.h"

namespace Ultima {
namespace Ultima2 {
namespace Data {

constexpr int TILE_COUNT = 64;
constexpr int TILE_WIDTH = 16;
constexpr int TILE_HEIGHT = 16;

/**
 * This archive class virtualizes converting the CGA tile data from
 * the ultimaii.exe file into a flat pixel (palette indexed) for each tile.
 * This will hopefully make it easier in the future if anyone wants to
 * create an enhanced graphic pack.. they'll just need to drop a 'tiles'
 * file in the game folder and implement a replacement palette for the game.
 */
class Tiles : public Common::Archive {
public:
	~Tiles() override {
	}

	/**
	 * Check if a member with the given @p name is present in the Archive.
	 * Patterns are not allowed, as this is meant to be a quick File::exists()
	 * replacement.  This returns "true" for both files and directories.
	 */
	virtual bool hasFile(const Common::Path &path) const {
		return path.baseName().equalsIgnoreCase("tiles");
	}

	/**
	 * Add all members of the Archive to the list.
	 * Must only append to list, and not remove elements from it.
	 *
	 * @return The number of names added to list.
	 */
	virtual int listMembers(Common::ArchiveMemberList &list) const {
		return 0;
	}

	/**
	 * Return an ArchiveMember representation of the given file.
	 */
	const Common::ArchiveMemberPtr getMember(const Common::Path &path) const override;

	/**
	 * Create a stream bound to a member with the specified name in the
	 * archive. If no member with this name exists, 0 is returned.
	 *
	 * @return The newly created input stream.
	 */
	Common::SeekableReadStream *createReadStreamForMember(const Common::Path &path) const override;
};

} // namespace Data
} // namespace Ultima2
} // namespace Ultima

#endif
