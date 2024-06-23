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
#include "wasteland/wasteland1/files/vertical_xor_stream.h"

namespace Wasteland {
namespace Wasteland1 {

VerticalXorStream::VerticalXorStream(Common::SeekableReadStream *stream,
		int width, DisposeAfterUse::Flag disposeAfterUse) :
		_stream(stream), _disposeAfterUse(disposeAfterUse),
		_width(width / 2) {
	_lastLine.resize(width);
}

VerticalXorStream::~VerticalXorStream() {
	if (_disposeAfterUse == DisposeAfterUse::YES)
		delete _stream;
}

uint32 VerticalXorStream::read(void *dataPtr, uint32 dataSize) {
	assert((dataSize % 2) == 0);
	Common::Array<byte> buf;
	buf.resize(dataSize);

	int bytesRead = _stream->read(&buf[0], dataSize);

	// Iterate through the bytes
	byte *dest = (byte *)dataPtr;
	for (int i = 0; i < bytesRead; ++i) {
		byte b = buf[i];

		// Decode the byte it it's not in the first (unencoded) row
		if (_y > 0) {
			b = b ^ _lastLine[_x];
		}

		// Remember the decoded byte for the next row
		_lastLine[_x] = b;

		// Move on the cursor
		if (_x < _width - 1) {
			_x++;
		} else {
			_y++;
			_x = 0;
		}

		*dest++ = b >> 4;
		*dest++ = b & 0xf;
	}

	return bytesRead * 2;
}

} // namespace Wasteland1
} // namespace Wasteland
