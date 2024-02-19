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

#ifndef YENDOR2_GLOBALS_H
#define YENDOR2_GLOBALS_H

#include "common/serializer.h"
#include "yendor/events.h"
#include "yendor/gfx/font.h"
#include "yendor/utils/file_entry.h"

namespace Yendor {
namespace Yendor2 {

class Globals {
public:
	Font _fonts[4];
	FileEntry _curGame;
	FileEntry _picturesVga;
	FileEntry _savegameX;
	FileEntry _worldDat;

	const int _blockSize1 = 26472;
	const int _blockSize2 = 18844;

public:
	Globals();
	virtual ~Globals();

	bool load();

	/**
	 * Saves global data to/from savegames
	 */
	void synchronize(Common::Serializer &s) {}
};

extern Globals *g_globals;

#define STRING (*g_globals)

} // namespace Yendor2
} // namespace Yendor

#endif
