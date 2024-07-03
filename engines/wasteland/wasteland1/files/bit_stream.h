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

#ifndef WASTELAND_WASTELAND1_FILES_BIT_STREAM_H
#define WASTELAND_WASTELAND1_FILES_BIT_STREAM_H

#include "common/stream.h"

namespace Wasteland {
namespace Wasteland1 {

class BitStream : public Common::ReadStream {
private:
	Common::ReadStream *_src;
	DisposeAfterUse::Flag _disposeAfterUse;
	byte _currentByte = 0;
	int _bitNum = 0;

public:
	BitStream(Common::ReadStream *src,
		DisposeAfterUse::Flag disposeAfterUse = DisposeAfterUse::NO);
	~BitStream() {
		close();
	}

	void close();
	bool hasNextBit() const {
		return _bitNum == 8 && !_src->eos();
	}
	int readBit();

	bool eos() const override {
		return _src->eos();
	}
	uint32 read(void *dataPtr, uint32 dataSize) override;
};

} // namespace Wasteland1
} // namespace Wasteland

#endif
