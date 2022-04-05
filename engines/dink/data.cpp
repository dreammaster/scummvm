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

#include "dink/data.h"
#include "dink/file.h"
#include "dink/globals.h"

namespace Dink {

void hardness::load() {
	File f;

	if (f.open("hard.dat")) {
		hmap.load(f);
	} else {
		ZeroMemory(&hmap, sizeof(hardness));
	}
}

void hardness::load(Common::SeekableReadStream &src) {
	for (int i = 0; i < 800; ++i)
		tile[i].load(src);
	for (int i = 0; i < 8000; ++i)
		index[i] = src.readUint32LE();
}

void ts_block::load(Common::SeekableReadStream &src) {
	for (int i = 0; i < 51; ++i)
		x[i].load(src);
	used = src.readByte() != 0;
	hold = src.readSint32LE();
}

void block_y::load(Common::SeekableReadStream &src) {
	src.read(y, 51);
}

void map_info::load() {
	File f;

	if (f.open(current_dat)) {
		map.load(f);
	} else {
		ZeroMemory(&hmap, sizeof(hardness));
		strcpy(map.name, "Smallwood");
	}
}

void map_info::load(Common::SeekableReadStream &src) {
	src.read(name, 20);
	for (int i = 0; i < 769; ++i)
		loc[i] = src.readSint32LE();
	for (int i = 0; i < 769; ++i)
		music[i] = src.readSint32LE();
	for (int i = 0; i < 769; ++i)
		indoor[i] = src.readSint32LE();
	for (int i = 0; i < 40; ++i)
		v[i] = src.readSint32LE();
	src.read(s, 80);
	src.read(buffer, 2000);
}

} // namespace Dink
