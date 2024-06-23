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

#include "common/memstream.h"
#include "wasteland/wasteland1/files/rotating_xor_stream.h"

namespace Wasteland {
namespace Wasteland1 {

void RotatingXorStream::init(Common::SeekableReadStream *src) {
	int e1, e2;

	// Get encryption byte and checksum end marker
	e1 = src->readByte();
	e2 = src->readByte();
	_enc = e1 ^ e2;
	_endChecksum = e1 | (e2 << 8);

	// Initialize checksum
	_checksum = 0;
}

void RotatingXorStream::decode(Common::SeekableReadStream *src) {
	Common::MemoryWriteStreamDynamic buf(DisposeAfterUse::NO);

	while (!src->eos()) {
		int crypted;
		int b;

		// Read crypted byte
		crypted = src->readByte();

		// Decrypt the byte
		b = crypted ^ _enc;
		buf.writeByte(b);

		// Update checksum
		_checksum = (_checksum - b) & 0xffff;

		// Updated encryption byte
		_enc = (_enc + 0x1f) % 0x100;
	}

	_stream = new Common::MemoryReadStream(buf.getData(), buf.size());
}

} // namespace Wasteland1
} // namespace Wasteland
