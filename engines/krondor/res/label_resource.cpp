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

void LabelResource::load(const Common::String &name) {
	clear();
	File f(name);

	_data.resize(f.readUint16LE());
	Common::Array<int> offsets(_data.size());

	for (uint i = 0; i < _data.size(); ++i) {
		LabelData &lbl = _data[i];
        offsets[i] = f.readSint16LE();
        lbl._xpos = f.readSint16LE();
        lbl._ypos = f.readSint16LE();
        lbl._type = f.readSint16LE();
        lbl._color = f.readSByte();
        lbl._shadow = f.readSByte();
    }

	f.skip(2);
    uint start = f.pos();

	for (uint i = 0; i < _data.size(); i++) {
        if (offsets[i] >= 0) {
            f.seek(start + offsets[i]);
            _data[i]._label = f.readString();
        }
    }
}

} // namespace Krondor
