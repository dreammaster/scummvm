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

#include "ultima/ultima5/console.h"
#include "ultima/ultima5/ultima5.h"
#include "ultima/ultima5/logic/location_logic.h"
#include "ultima/ultima5/logic/world_logic.h"
#include "ultima/ultima5/views/asset_viewer.h"

namespace Ultima {
namespace Ultima5 {

Console::Console() : GUI::Debugger() {
	registerCmd("tiles", WRAP_METHOD(Console, cmdTiles));
	registerCmd("image", WRAP_METHOD(Console, cmdImage));
	registerCmd("teleport", WRAP_METHOD(Console, cmdTeleport));
	registerCmd("location", WRAP_METHOD(Console, cmdLocation));
}

Console::~Console() {
}

static Views::AssetViewer *assetViewer() {
	return dynamic_cast<Views::AssetViewer *>(g_engine->findView("AssetViewer"));
}

bool Console::cmdTiles(int, const char **) {
	assetViewer()->showTiles();
	g_engine->replaceView("AssetViewer", true);
	return false;
}

bool Console::cmdImage(int argc, const char **argv) {
	if (argc < 2) {
		debugPrintf("image <file> [index]  - Browses the images of a picture file, such as ULTIMA\n");
		return true;
	}

	// Drop any extension, which comes from the graphics mode
	Common::String name(argv[1]);
	const size_t dot = name.findLastOf('.');
	if (dot != Common::String::npos)
		name.erase(dot);

	if (!assetViewer()->showImages(name, argc > 2 ? atoi(argv[2]) : 0)) {
		debugPrintf("Could not load %s\n", argv[1]);
		return true;
	}

	g_engine->replaceView("AssetViewer", true);
	return false;
}

bool Console::cmdTeleport(int argc, const char **argv) {
	Data::Savegame &sg = _G(savegame);

	if (!sg.hasActiveGame()) {
		debugPrintf("Create a character first\n");
		return true;
	}

	if (argc < 3) {
		debugPrintf("teleport <x> <y> [under]  - Moves the party to Britannia, or the underworld\n");
		return true;
	}

	sg._mapId = 0;
	sg._mapX = atoi(argv[1]);
	sg._mapY = atoi(argv[2]);
	sg._mapLevel = (argc > 3) ? Data::MAP_LEVEL_UNDERGROUND : 0;
	if (sg._partyTile == 0)
		sg._partyTile = TILE_AVATAR;

	if (!g_engine->loadWorld()) {
		debugPrintf("Could not load the map\n");
		return true;
	}

	g_engine->replaceView("Game", true);
	return false;
}

bool Console::cmdLocation(int argc, const char **argv) {
	Data::Savegame &sg = _G(savegame);

	if (!sg.hasActiveGame()) {
		debugPrintf("Create a character first\n");
		return true;
	}

	if (argc < 2) {
		debugPrintf("location <id> [level]  - Moves the party into a town, castle, keep or dwelling (1-32)\n");
		return true;
	}

	const int id = atoi(argv[1]);
	if (id < 1 || id > 32) {
		debugPrintf("The id must be from 1 to 32\n");
		return true;
	}

	if (sg._mapId == 0)
		sg.storeWorldObjects(sg._mapLevel != 0);

	sg.clearObjects();
	sg._mapId = id;
	sg._mapLevel = (argc > 2) ? atoi(argv[2]) : 0;
	sg._mapX = 15;
	sg._mapY = 30;
	if (sg._partyTile == 0)
		sg._partyTile = TILE_AVATAR;

	LocationLogic::arrive();

	if (!g_engine->loadMap()) {
		debugPrintf("Could not load the map\n");
		return true;
	}

	g_engine->replaceView("Game", true);
	return false;
}

} // namespace Ultima5
} // namespace Ultima
