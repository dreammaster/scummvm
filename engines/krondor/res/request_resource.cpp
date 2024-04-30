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

#include "krondor/res/request_resource.h"

namespace Krondor {

void RequestResource::clear() {
	_popup = false;
	_rect.clear();
	_xoff = _yoff = 0;
	_data.clear();
}

void RequestResource::read(Common::SeekableReadStream *src) {
	clear();

	src->skip(2);
	_popup = src->readSint16LE() != 0;
	src->skip(2);
	_rect.read(src);

	src->skip(2);
	_xoff = src->readSint16LE();
	_yoff = src->readSint16LE();
	src->skip(2);
	src->skip(2);
	src->skip(2);
	src->skip(2);

	uint numRecords = src->readUint16LE();
	Common::Array<int> offsets;
	offsets.resize(numRecords);
	_data.resize(numRecords);

	for (uint i = 0; i < numRecords; i++) {
		RequestData &rd = _data[i];

		rd._widget = src->readUint16LE();
		rd._action = src->readSint16LE();
		rd._visible = (src->readByte() != 0);
		src->skip(2);
		src->skip(2);
		src->skip(2);
		rd._xpos = src->readSint16LE();
		rd._ypos = src->readSint16LE();
		rd._width = src->readUint16LE();
		rd._height = src->readUint16LE();
		src->skip(2);
		offsets[i] = src->readSint16LE();
		rd._teleport = src->readSint16LE();
		rd._image = src->readUint16LE();
		rd._image = (rd._image >> 1) + (rd._image & 1);
		src->skip(2);
		rd._group = src->readUint16LE();
		src->skip(2);
	}

	src->skip(2);
	uint start = src->pos();

	for (uint i = 0; i < numRecords; i++) {
		if (offsets[i] >= 0) {
			src->seek(start + offsets[i]);
			_data[i]._label = src->readString();
		}
	}
}

} // namespace Krondor
