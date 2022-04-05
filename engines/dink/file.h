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

#ifndef DINK_FILE_H
#define DINK_FILE_H

#include "common/file.h"

namespace Dink {

class File : public Common::File {
private:
	static Common::String formatPath(const Common::String &name);
public:
	bool open(const Common::String &name);
	static bool exists(const Common::String &name);
};

/**
 * Sub-structure for tile hardness
 */
struct block_y {
	byte y[51] = { 0 };

	void load(Common::SeekableReadStream &s);
};

struct ts_block {
	block_y x[51];
	bool used = false;
	int hold = 0;

	void load(Common::SeekableReadStream &s);
};

/**
 * Structure for hardness info, INDEX controls which
 * hardness each block has. 800 max types available.
 */
struct hardness {
	ts_block tile[800];
	int index[8000];

	void load(Common::SeekableReadStream &s);
	bool load();
};


} // namespace Dink

#endif
