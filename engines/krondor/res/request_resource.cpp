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
	_bounds.clear();
	_data.clear();
}

void RequestResource::load(const Common::String &name) {
	clear();
	File f(name);

	f.skip(2);
	_popup = f.readSint16LE() != 0;
	f.skip(2);
	_bounds.read(&f);

	f.skip(2);
	_xoff = f.readSint16LE();
	_yoff = f.readSint16LE();
	f.skip(2);
	f.skip(2);
	f.skip(2);
	f.skip(2);

	uint numRecords = f.readUint16LE();
	Common::Array<int> offsets;
	offsets.resize(numRecords);
	_data.resize(numRecords);

	for (uint i = 0; i < numRecords; i++) {
		RequestData &rd = _data[i];

		rd._widget = f.readUint16LE();
		rd._action = f.readSint16LE();
		rd._visible = (f.readByte() != 0);
		f.skip(2);
		f.skip(2);
		f.skip(2);
		rd._bounds.read(&f);
		f.skip(2);
		offsets[i] = f.readSint16LE();
		rd._teleport = f.readSint16LE();
		rd._image = f.readUint16LE();
		rd._image = (rd._image >> 1) + (rd._image & 1);
		f.skip(2);
		rd._group = f.readUint16LE();
		f.skip(2);
	}

	f.skip(2);
	uint start = f.pos();

	for (uint i = 0; i < numRecords; i++) {
		if (offsets[i] >= 0) {
			f.seek(start + offsets[i]);
			_data[i]._label = f.readString();
		}
	}
}

} // namespace Krondor
