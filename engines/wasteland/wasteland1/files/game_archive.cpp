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

#include "common/hashmap.h"
#include "wasteland/wasteland1/files/game_archive.h"
#include "wasteland/wasteland1/files/rotating_xor_stream.h"
#include "wasteland/core/file.h"

namespace Wasteland {
namespace Wasteland1 {

void GameArchive::load(int gameNum) {
	assert(gameNum == 1 || gameNum == 2);
	_gameNum = gameNum;
	_filename = Common::String::format("game%d", gameNum);

	int nextOffset;
	int offset;
	int disk;
	int stage;
	int b;

	File f(_filename);
	uint32 header = f.readUint32BE();
	if (header != MKTAG('m', 's', 'q', '0') && header != MKTAG('m', 's', 'q', '1'))
		error("No msq header found in stream");

	disk = header & 0xff;
	nextOffset = 0;
	offset = 4;

	// Read the rest of the file and scan for msq blocks.
	_blocks.clear();
	stage = 0;
	for (b = f.readByte(); !f.eos(); b = f.readByte()) {
		switch (stage) {
		case 0:
			if (b == 'm') {
				stage = 1;
			}
			break;

		case 1:
			if (b == 's') {
				stage = 2;
			} else {
				stage = 0;
			}
			break;

		case 2:
			if (b == 'q') {
				stage = 3;
			} else {
				stage = 0;
			}
			break;

		case 3:
			if (b == disk) {
				_blocks.push_back(GameMsqBlock(nextOffset, offset - 3
					- nextOffset));
				nextOffset = offset - 3;
			}
			stage = 0;
			break;
		}

		offset++;
	}

	_blocks.push_back(GameMsqBlock(nextOffset, offset - nextOffset));

	// Iterate over the blocks to get the types
	_savegameIndex = _shopsIndex = -1;
	for (uint i = 0; i < _blocks.size(); ++i) {
		switch (getBlockType(_blocks[i], &f)) {
		case TYPE_SAVEGAME:
			_savegameIndex = i;
			break;
		case TYPE_SHOPLIST:
			_shopsIndex = i;
			break;
		default:
			break;
		}
	}
}

bool GameArchive::hasFile(const Common::Path &path) const {
	Common::String name = path.baseName();
	if (name.equalsIgnoreCase("savegame") || name.equalsIgnoreCase("shops"))
		return true;

	if (name.hasPrefixIgnoreCase("map")) {
		int mapNum = atoi(name.c_str() + 3);
		return getBlockIndex(mapNum) != -1;

	}

	if (name.hasPrefixIgnoreCase("game-")) {
		int blockNum = atoi(name.c_str() + 5);
		return getBlockIndex(blockNum) != -1;
	}

	return false;
}

int GameArchive::listMembers(Common::ArchiveMemberList &list) const {
	return 0;
}

const Common::ArchiveMemberPtr GameArchive::getMember(const Common::Path &path) const {
	if (!hasFile(path))
		return Common::ArchiveMemberPtr();

	return Common::ArchiveMemberPtr(new Common::GenericArchiveMember(path, *this));
}

Common::SeekableReadStream *GameArchive::createReadStreamForMember(const Common::Path &path) const {
	Common::String name = path.baseName();
	int blockIndex;

	if (name.equalsIgnoreCase("savegame")) {
		blockIndex = _savegameIndex;
	} else if (name.equalsIgnoreCase("shops")) {
		blockIndex = _shopsIndex;
	} else if (name.hasPrefixIgnoreCase("map")) {
		blockIndex = getBlockIndex(atoi(name.c_str() + 3));
	} else if (name.hasPrefixIgnoreCase("game-")) {
		blockIndex = getBlockIndex(atoi(name.c_str() + 5));
	} else {
		blockIndex = -1;
	}

	if (blockIndex == -1)
		return nullptr;

	// Return the correct block
	File f(_filename);
	f.seek(_blocks[blockIndex]._offset + 4);

	auto *blockStream = f.readStream(_blocks[blockIndex]._size - 4);
	RotatingXorStream xorStream(blockStream);
	delete blockStream;

	return xorStream.get();
}

GameArchive::BlockType GameArchive::getBlockType(const GameArchive::GameMsqBlock &block, Common::SeekableReadStream *src) {
	src->seek(block._offset);
	size_t msqBlockSize = block._size;

	// Validate header
	uint32 header = src->readUint32BE();
	if (header != MKTAG('m', 's', 'q', '0') && header != MKTAG('m', 's', 'q', '1'))
		error("No msq header found in stream");

	// Read the first 9 decrypted bytes
	Common::SeekableReadStream *blockStream = src->readStream(9);
	RotatingXorStream xorStream(blockStream);
	delete blockStream;

	Common::SeekableReadStream *decodedStream = xorStream.get();
	byte bytes[9];
	decodedStream->read(bytes, 9);
	delete decodedStream;

	// Determine and return the type
	if (msqBlockSize == 4614 && isSaveGame(bytes)) {
		return TYPE_SAVEGAME;
	} else if (msqBlockSize == 766 && isShopItems(bytes)) {
		return TYPE_SHOPLIST;
	} else {
		return TYPE_MAP;
	}
}

bool GameArchive::isSaveGame(const byte bytes[]) {
	bool seen[6];
	byte b;
	Common::fill(seen, seen + 6, false);

	for (int i = 1; i < 6; i++) {
		b = bytes[i];
		if (b > 7)
			return false;
		if (b != 0 && seen[b])
			return false;

		seen[b] = true;
	}

	return true;
}

bool GameArchive::isShopItems(const byte bytes[]) {
	return bytes[0] == 0x60 && bytes[1] == 0x60 && bytes[2] == 0x60;
}

int GameArchive::getBlockIndex(int mapNum) const {
	if (mapNum >= 0 && mapNum < (int)_blocks.size())
		return mapNum;

	for (uint i = 0; i < _blocks.size(); ++i) {
		if (_blocks[i]._offset == (uint)mapNum)
			return i;
	}

	return -1;
}

} // namespace Wasteland1
} // namespace Wasteland
