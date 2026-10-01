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

#include "audio/mixer.h"
#include "common/config-manager.h"
#include "common/scummsys.h"
#include "common/system.h"
#include "common/translation.h"
#include "engines/util.h"
#include "mm/mm2/mm2.h"
#include "mm/mm2/console.h"
#include "mm/mm2/views/views.h"

namespace MM {
namespace MM2 {

#define SAVEGAME_VERSION 1

MM2Engine *g_engine;

MM2Engine::MM2Engine(OSystem *syst, const MightAndMagicGameDescription *gameDesc) : MMEngine(syst, gameDesc) {
	g_engine = this;
}

MM2Engine::~MM2Engine() {
	_mixer->stopAll();
}

uint32 MM2Engine::getFeatures() const {
	return _gameDescription->desc.flags;
}

bool MM2Engine::isDemo() const {
	return (_gameDescription->desc.flags & ADGF_DEMO) != 0;
}

Common::String MM2Engine::getGameId() const {
	return _gameDescription->desc.gameId;
}

Common::Error MM2Engine::run() {
	// Initialize 320x240 paletted graphics mode. Note that the original
	// main menu/dialogs ran at 320x200, but the game ran at 320x240.
	initGraphics(320, 240);

	// Set the engine's debugger console
	setDebugger(new Console());

	Views::Views views; // Loads all views in the structure

	// Set up the initial game view
	int saveSlot = ConfMan.getInt("save_slot");
	if (saveSlot != -1) {
		if (g_engine->loadGameState(saveSlot).getCode() != Common::kNoError)
			saveSlot = -1;
	}
	if (saveSlot == -1)
		addView("Startup");

	runGame();
	(void)views;	// Suppress unreferenced local warning

	return Common::kNoError;
}

Common::Error MM2Engine::saveGameStream(Common::WriteStream *stream, bool isAutosave) {
	stream->writeByte(SAVEGAME_VERSION);
	Common::Serializer s(nullptr, stream);
	s.setVersion(SAVEGAME_VERSION);

	return syncGame(s);
}

Common::Error MM2Engine::loadGameStream(Common::SeekableReadStream *stream) {
	byte version = stream->readByte();
	if (version != SAVEGAME_VERSION)
		error("Invalid savegame version");

	Common::Serializer s(stream, nullptr);
	s.setVersion(version);

	return syncGame(s);
}

Common::Error MM2Engine::syncGame(Common::Serializer &s) {
	// TODO
	return Common::kNoError;
}

bool MM2Engine::canLoadGameStateCurrently(Common::U32String *msg) {
	return false;
}

bool MM2Engine::canSaveGameStateCurrently(Common::U32String *msg) {
	return false;
}

} // namespace MM2
} // namespace MM
