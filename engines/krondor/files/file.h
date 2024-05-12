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

#ifndef KRONDOR_FILES_FILE_H
#define KRONDOR_FILES_FILE_H

#include "common/file.h"
#include "krondor/files/decompress.h"

namespace Krondor {

enum CompressionType {
	COMPRESSION_LZW = 0,
	COMPRESSION_LZSS = 1,
	COMPRESSION_RLE = 2
};

class File : public Common::File {
public:
	File() : Common::File() {}
	File(const Common::Path &name) : Common::File() {
		open(name);
	}
	File(const Common::String &name) : Common::File() {
		(void)open(name);
	}

	bool open(const Common::Path &path) override;
	void open(const Common::String &path) {
		open(Common::Path(path));
	}

	/**
	 * Do a RLE decompression of a stream
	 */
	Common::SeekableReadStream *decompressRLE() {
		return DecompressRLE(this).decompress();
	}

	/**
	 * Do a LZW decompression of a stream
	 */
	Common::SeekableReadStream *decompressLZW() {
		return DecompressLZW(this).decompress();
	}

	/**
	 * Do a LZSS decompression of a stream
	 */
	Common::SeekableReadStream *decompressLZSS() {
		return DecompressLZSS(this).decompress();
	}

	Common::SeekableReadStream *decompress(CompressionType compression);
};

} // namespace Krondor

#endif
