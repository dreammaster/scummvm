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
#include "wasteland/wasteland1/files/nibble_stream.h"

namespace Wasteland {
namespace Wasteland1 {

NibbleStream::NibbleStream(Common::SeekableReadStream *stream,
		DisposeAfterUse::Flag disposeAfterUse) :
		_stream(stream), _disposeAfterUse(disposeAfterUse) {
}

NibbleStream::~NibbleStream() {
	if (_disposeAfterUse == DisposeAfterUse::YES)
		delete _stream;
}

uint32 NibbleStream::read(void *dataPtr, uint32 dataSize) {
	assert((dataSize % 2) == 0);
	Common::Array<byte> buf;
	buf.resize(dataSize / 2);

	int bytesRead = _stream->read(&buf[0], dataSize / 2);

	// Iterate through the bytes
	const byte *src = &buf[0];
	byte *dest = (byte *)dataPtr;

	for (int i = 0; i < bytesRead; ++i, ++src) {
		*dest++ = *src >> 4;
		*dest++ = *src & 0xf;
	}

	return bytesRead * 2;
}

} // namespace Wasteland1
} // namespace Wasteland
