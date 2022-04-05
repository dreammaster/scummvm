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

#include "dink/file.h"

namespace Dink {

bool File::open(const Common::String &name) {
	return Common::File::open(Common::Path(formatPath(name)));
}

bool File::exists(const Common::String &name) {
	return Common::File::exists(Common::Path(formatPath(name)));
}

Common::String File::formatPath(const Common::String &name) {
	size_t pos = 0;
	Common::String result = name;
	while ((pos = result.findFirstOf('\\', pos)) != Common::String::npos) {
		result.setChar('/', pos);
	}

	return result;
}

bool hardness::load() {
	File f;

	if (f.open("hard.dat")) {
		load(f);
		return true;
	} else {
		return false;
	}
}

void hardness::load(Common::SeekableReadStream &s) {
	for (int i = 0; i < 800; ++i)
		tile[i].load(s);
	for (int i = 0; i < 8000; ++i)
		index[i] = s.readUint32LE();
}

void ts_block::load(Common::SeekableReadStream &s) {
	for (int i = 0; i < 51; ++i)
		x[i].load(s);
	used = s.readByte() != 0;
	hold = s.readUint32LE();
}

void block_y::load(Common::SeekableReadStream &s) {
	s.read(y, 51);
}

} // namespace Dink
