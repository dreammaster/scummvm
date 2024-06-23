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

#include "wasteland/wasteland1/files/bit_stream.h"

namespace Wasteland {
namespace Wasteland1 {

BitStream::BitStream(Common::ReadStream *src,
		DisposeAfterUse::Flag disposeAfterUse) :
		_src(src), _disposeAfterUse(disposeAfterUse),
		_bitNum(8), _currentByte(0) {
}

void BitStream::close() {
	if (_disposeAfterUse == DisposeAfterUse::YES)
		delete _src;
	_src = nullptr;
}

int BitStream::readBit() {
	if (_bitNum == 8) {
		if (hasNextBit())
			return -1;

		_currentByte = _src->readByte();
		_bitNum = 0;
	}

	int result = _currentByte & 1;
	_currentByte >>= 1;
	++_bitNum;

	return result;
}

} // namespace Wasteland1
} // namespace Wasteland
