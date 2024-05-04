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

#ifndef KRONDOR_FILES_DECOMPRESS_H
#define KRONDOR_FILES_DECOMPRESS_H

#include "common/file.h"

namespace Krondor {

class DecompressRLE {
private:
	Common::SeekableReadStream *_src = nullptr;
	size_t _decompressedSize = 0;

public:
	DecompressRLE(Common::SeekableReadStream *src);

	Common::SeekableReadStream *decompress();
};

class DecompressLZSS {
private:
	Common::SeekableReadStream *_src = nullptr;
	size_t _decompressedSize = 0;

public:
	DecompressLZSS(Common::SeekableReadStream *src);

	Common::SeekableReadStream *decompress();
};

class DecompressLZW {
	struct CodeTableEntry {
		uint16 _prefix;
		uint8 _append;
	};
private:
	Common::SeekableReadStream *_src;
	size_t _decompressedSize = 0;
	int _nextBit = 0;
	byte _currentByte = 0;

	uint getBits(uint numBits);
	void skipBits();

public:
	DecompressLZW(Common::SeekableReadStream *src);

	Common::SeekableReadStream *decompress();
};

} // namespace Krondor

#endif
