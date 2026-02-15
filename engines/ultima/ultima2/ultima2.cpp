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

#include "common/system.h"
#include "common/file.h"
#include "common/savefile.h"
#include "engines/util.h"
#include "graphics/paletteman.h"
#include "ultima/ultima2/ultima2.h"
#include "ultima/ultima2/console.h"
#include "ultima/ultima2/metaengine.h"
#include "ultima/ultima2/views/views.h"

namespace Ultima {
namespace Ultima2 {

static const byte PALETTE[][3] = {
	{ 0, 0, 0 },		// Black
	{ 85, 255, 255 },	// Light Cyan
	{ 255, 85, 255 },	// Light Magenta
	{ 255, 255, 255 }	// White
};

Ultima2Engine *g_engine;

Ultima2Engine::Ultima2Engine(OSystem *syst, const Ultima::UltimaGameDescription *gameDesc) :
		Engine(syst) /*, _gameDescription(gameDesc) */ {
	g_engine = this;
	_palette = Graphics::Palette(&PALETTE[0][0], sizeof(PALETTE) / 3);
}

Ultima2Engine::~Ultima2Engine() {
}

Common::Error Ultima2Engine::run() {
	// Initialize the graphics
	initGraphics(320, 200);
	setBounds(Common::Rect(0, 0, 320, 200));
	g_system->getPaletteManager()->setPalette(_palette);

	// Set the debugger console
	setDebugger(new Console());
	MetaEngine::setKeybindingMode(KBMODE_MINIMAL);

	// Load data
	loadData();

	// Play the game
	runGame();

	return Common::kNoError;
}

void Ultima2Engine::loadData() {
	// Set up default archive for accessing tile data from executable
	SearchMan.add("Tiles", new Data::Tiles(), 1);

	// Load the tile data
	Common::File f;
	if (!f.open("tiles"))
		error("Could not read tile data");
	for (Graphics::ManagedSurface &s : _tiles) {
		s.create(Data::TILE_WIDTH, Data::TILE_HEIGHT);
		f.read((byte *)s.getPixels(), Data::TILE_WIDTH * Data::TILE_HEIGHT);
	}
}

void Ultima2Engine::runGame() {
	Views::Views views;	// Loads all views in the structure

	// Check for savegame
	int saveSlot = ConfMan.getInt("save_slot");
	if (saveSlot == -1 || g_engine->loadGameState(saveSlot).getCode() != Common::kNoError)
		// Except when loading a savegame from the launcher, default to first screen
		addView("Startup");

	// Delegate to play the game
	Events::runGame();
}

bool Ultima2Engine::hasFeature(EngineFeature f) const {
	return (f == kSupportsReturnToLauncher) ||
		(f == kSupportsLoadingDuringRuntime) ||
		(f == kSupportsSavingDuringRuntime);

}

bool Ultima2Engine::canSaveGameStateCurrently(Common::U32String *msg) {
	return focusedView()->getName() == "Game";
}

Common::Error Ultima2Engine::loadGameStream(Common::SeekableReadStream *stream) {
	Common::Serializer s(stream, nullptr);
	syncSavegame(s);
	return Common::kNoError;
}

Common::Error Ultima2Engine::saveGameStream(Common::WriteStream *stream, bool isAutosave) {
	Common::Serializer s(nullptr, stream);
	syncSavegame(s);
	return Common::kNoError;
}

void Ultima2Engine::syncSavegame(Common::Serializer &s) {
	_player.synchronize(s);
}

bool Ultima2Engine::savegamesExist() const {
	Common::String slotName = getSaveStateName(1);
	Common::InSaveFile *saveFile = g_system->getSavefileManager()->openForLoading(slotName);
	bool result = saveFile != nullptr;

	delete saveFile;
	return result;
}

} // namespace Ultima2
} // namespace Ultima
