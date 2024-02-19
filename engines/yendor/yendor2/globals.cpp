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

#include "engines/engine.h"
#include "graphics/fonts/ttf.h"
#include "yendor/utils/engine_data.h"
#include "yendor/yendor2/globals.h"

namespace Yendor {
namespace Yendor2 {

Globals *g_globals;

Globals::Globals() :
		_curGame("curgame"),
		_picturesVga("pictures.vga"),
		_savegameX("savegamex"),
		_worldDat("world.dat") {
	g_globals = this;
}

Globals::~Globals() {
	g_globals = nullptr;
}

bool Globals::load() {
	// Initialise engine data for the game
	Common::U32String errMsg;
	if (!load_engine_data("yendor2", 1, 0, errMsg)) {
		GUIErrorMessage(errMsg);
		return false;
	}

	for (int i = 0; i < 4; ++i)
		_fonts[i].load(Common::Path(Common::String::format("font%d.fnt", i)));

	Font::_bgTransparent = false;
	Font::_fgColor = 0;
	Font::_bgColor = 0;

	return true;
}

} // namespace Yendor2
} // namespace Yendor
