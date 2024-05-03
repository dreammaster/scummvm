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

#include "common/error.h"
#include "common/memstream.h"
#include "krondor/files/file.h"

namespace Krondor {

bool File::open(const Common::Path &path) {
	if (!Common::File::open(path))
		error("Could not open - %s", path.toString().c_str());

	return true;
}

Common::SeekableReadStream *File::decompressRLE() {
	uint size = readUint32LE();
	Common::SeekableReadStream *src = readStream(size);
	Common::MemoryWriteStreamDynamic buf(DisposeAfterUse::NO);
	uint control, val, i;

	while (!src->eos()) {
		control = src->readByte();
		if (control & 0x80) {
			// Repeat following byte number of times
			val = src->readByte();
			for (i = 0; i < (control & 0x7f); ++i)
				buf.writeByte(val);
		} else {
			// Copy a range of bytes
			for (i = 0; i < control; ++i)
				buf.writeByte(src->readByte());
		}
	}

	return new Common::MemoryReadStream(buf.getData(), buf.size(), DisposeAfterUse::YES);
}

} // namespace Krondor
