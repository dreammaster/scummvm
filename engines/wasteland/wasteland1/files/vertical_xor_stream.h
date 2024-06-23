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

#ifndef WASTELAND_WASTELAND1_FILES_VERTICAL_XOR_STREAM_H
#define WASTELAND_WASTELAND1_FILES_VERTICAL_XOR_STREAM_H

#include "common/array.h"
#include "common/stream.h"

namespace Wasteland {
namespace Wasteland1 {

class VerticalXorStream : public Common::SeekableReadStream {
private:
	Common::SeekableReadStream *_stream = nullptr;
	DisposeAfterUse::Flag _disposeAfterUse;
	Common::Array<byte> _lastLine;
	int _width;
	int _x = 0;
	int _y = 0;

public:
	VerticalXorStream(Common::SeekableReadStream *stream, int width,
		DisposeAfterUse::Flag disposeAfterUse);
	~VerticalXorStream();

	bool eos() const override {
		return _stream->eos();
	}
	int64 pos() const override {
		return _stream->pos();
	}
	int64 size() const override {
		return _stream->size();
	}
	bool seek(int64 offset, int whence = SEEK_SET) override {
		return _stream->seek(offset, whence);
	}

	uint32 read(void *dataPtr, uint32 dataSize) override;
};

} // namespace Wasteland1
} // namespace Wasteland

#endif
