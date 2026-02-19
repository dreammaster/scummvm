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

#include "ultima/ultima2/console.h"
#include "ultima/ultima2/ultima2.h"
#include "ultima/ultima2/data/tiles.h"

namespace Ultima {
namespace Ultima2 {

using namespace Shared::Core;

Console::Console() : GUI::Debugger() {
	registerCmd("tiles", WRAP_METHOD(Console, cmdTiles));
	registerCmd("view", WRAP_METHOD(Console, cmdView));
}

Console::~Console() {
}

bool Console::cmdTiles(int argc, const char **argv) {
	const auto &tiles = g_engine->_tiles;
	Graphics::Screen *screen = g_engine->getScreen();
	screen->fillRect(Common::Rect(0, 0, (Data::TILE_WIDTH + 2) * 16, (Data::TILE_HEIGHT + 2) * 4), 1);

	for (int y = 0; y < 4; ++y) {
		for (int x = 0; x < 16; ++x) {
			screen->blitFrom(tiles[y * 16 + x],
				Common::Point(x * (Data::TILE_WIDTH + 2) + 1, y * (Data::TILE_HEIGHT + 2) + 1));
		}
	}

	return false;
}

bool Console::cmdView(int argc, const char **argv) {
	if (argc == 2) {
		g_engine->replaceView(argv[1]);
		return false;
	} else {
		debugPrintf("view <view name>\n");
		return true;
	}
}

} // namespace Ultima2
} // namespace Ultima
