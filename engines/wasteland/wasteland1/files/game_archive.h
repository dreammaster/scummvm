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

#ifndef WASTELAND_WASTELAND1_FILES_GAME_ARCHIVE_H
#define WASTELAND_WASTELAND1_FILES_GAME_ARCHIVE_H

#include "common/archive.h"
#include "common/file.h"

namespace Wasteland {
namespace Wasteland1 {

class GameArchive : public Common::Archive {
	enum BlockType {
		TYPE_MAP = 0,
		TYPE_SAVEGAME = 1,
		TYPE_SHOPLIST = 2
	};

	struct GameMsqBlock {
		size_t _offset = 0;
		size_t _size = 0;

		GameMsqBlock() {}
		GameMsqBlock(size_t offset, size_t size) :
			_offset(offset), _size(size) {}
	};
private:
	Common::Path _filename;
	Common::Array<GameMsqBlock> _blocks;
	int _gameNum;
	int _shopsIndex = -1;
	int _savegameIndex = -1;

	/**
	 * Gets a block's type
	 */
	BlockType getBlockType(const GameMsqBlock &block, Common::SeekableReadStream *src);

	/**
	 * Checks if the specified byte array represents a Wasteland save game. Save
	 * games are discovered by the block size and by the byte offsets 1-8 which
	 * represents the character order and must contain values between 0 and 7
	 * while all non-zero numbers can only occur once.
	 *
	 * @param bytes The byte array to check
	 * @return		If it's a save game or not
	 */
	static bool isSaveGame(const byte bytes[]);

	/**
	  * Checks if the specified byte array represents an unknown block which is
	  * one of the blocks following the save game block. Purpose of these blocks
	  * is currently unknown.
	  *
	  * @param bytes	The byte array to check
	  * @return			If it's an unknwon block or not
	  */
	static bool isShopItems(const byte bytes[]);

	/**
	 * Gets the index of a map block
	 */
	int getMapIndex(int mapNum) const;

public:
	GameArchive() {
		load(1);
	}
	~GameArchive() override {}
	void load(int gameNum);

	bool hasFile(const Common::Path &path) const override;
	int listMembers(Common::ArchiveMemberList &list) const override;
	const Common::ArchiveMemberPtr getMember(const Common::Path &path) const override;
	Common::SeekableReadStream *createReadStreamForMember(const Common::Path &path) const override;
};

} // namespace Wasteland1
} // namespace Wasteland

#endif
