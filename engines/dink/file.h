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

	/**
	 * Returns true if there are CD tracks in the game folder
	 */
	static bool audioTracksExists();
};

} // namespace Dink

#endif
