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

#include "krondor/res/label_resource.h"

namespace Krondor {

void LabelResource::clear() {
	_data.clear();
}

void LabelResource::read(Common::SeekableReadStream *src) {
	clear();

	_data.resize(src->readUint16LE());
	Common::Array<int> offsets(_data.size());

	for (uint i = 0; i < _data.size(); ++i) {
		LabelData &lbl = _data[i];
        offsets[i] = src->readSint16LE();
        lbl._xpos = src->readSint16LE();
        lbl._ypos = src->readSint16LE();
        lbl._type = src->readSint16LE();
        lbl._color = src->readSByte();
        lbl._shadow = src->readSByte();
    }

	src->skip(2);
    uint start = src->pos();

	for (uint i = 0; i < _data.size(); i++) {
        if (offsets[i] >= 0) {
            src->seek(start + offsets[i]);
            _data[i]._label = src->readString();
        }
    }
}

} // namespace Krondor
