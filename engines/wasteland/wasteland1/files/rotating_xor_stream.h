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

#ifndef WASTELAND_WASTELAND1_FILES_ROTATING_XOR_STREAM_H
#define WASTELAND_WASTELAND1_FILES_ROTATING_XOR_STREAM_H

#include "common/stream.h"

namespace Wasteland {
namespace Wasteland1 {

class RotatingXorStream {
private:
	Common::SeekableReadStream *_stream = nullptr;
	int _enc = 0;
	int _endChecksum = 0;
	int _checksum = 0;

	void init(Common::SeekableReadStream *src);
	void decode(Common::SeekableReadStream *src);

public:
	RotatingXorStream(Common::SeekableReadStream *src) {
		init(src);
		decode(src);
	}
	~RotatingXorStream() {
		delete _stream;
	}

	Common::SeekableReadStream *get() {
		auto *result = _stream;
		_stream = nullptr;
		return result;
	}
};

} // namespace Wasteland1
} // namespace Wasteland

#endif
