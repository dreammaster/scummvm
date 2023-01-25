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

#include "legend/early/engine.h"
#include "legend/early/gfx/screen.h"
#include "legend/gfx/font.h"

namespace Legend {
namespace Early {

Engine *g_engine;

Engine::Engine(OSystem *syst, const LegendGameDescription *gameDesc) :
		LegendEngine(syst, gameDesc) {
	g_engine = this;
}


Engine::~Engine() {
	delete _logics;
	delete _vocab;
	delete _parser;
}

bool Engine::initialize() {
	(void)LegendEngine::initialize();

	Legend::Gfx::Font::init();
	_screen = new Early::Gfx::Screen();
	_vocab = new Parser::Vocab();

	return true;
}

void Engine::deinitialize() {
	Legend::Gfx::Font::deinit();
}

bool Engine::msgGame(const GameMessage &msg) {
	if (msg._name == "LINE") {
		_parser->parse(msg._stringValue);
		return true;
	}

	return false;
}

bool Engine::msgLogic(const LogicMessage &msg) {
	int logicNum = msg._logicNum == -1 ? _roomLogicNum : msg._logicNum;
	(*_logics)[logicNum]->call(msg._action);
	return true;
}

void Engine::setRoom(int roomNum) {
	_roomLogicNum = roomNum;
	_currentRoom = dynamic_cast<const Room *>((*_logics)[roomNum]);
}

int Engine::loadGame(LoadType mode, const char *filename) {
	String name;
	Common::SeekableReadStream *stream = nullptr;

	switch (mode) {
	case LOAD_RESTART:
	case LOAD_SAVE:
		if (mode == LOAD_RESTART) {
			// restart.dat contains the initial game state
			name = "restart.dat";
			Common::File *f = new Common::File();
			if (f->open(name)) {
				stream = f;
			} else {
				delete f;
			}
		} else {
			// Load from a savegame
			assert(filename);
			stream = g_system->getSavefileManager()->openForLoading(filename);
		}
		assert(stream);


	}

	// TODO
	error("TODO: loadGame/undo");
}

Common::Error Engine::loadGameState(int slot) {
	// In case autosaves are on, do a save first before loading the new save
	saveAutosaveIfEnabled();

	Common::InSaveFile *saveFile = _saveFileMan->openForLoading(getSaveStateName(slot));

	if (!saveFile)
		return Common::kReadingFailed;

	// Check for an original save
	saveFile->seek(-4, SEEK_END);
	saveFile->seek(saveFile->readUint32LE());
	char buffer[6];
	saveFile->read(buffer, 5);
	buffer[5] = '\0';
	saveFile->seek(0);

	bool isOriginal = strcmp(buffer, "SVMCR") != 0;
	if (isOriginal)
		// Not present, so it's an original save. Hopefully.
		// Skip over savegame name field
		saveFile->seek(34);

	Common::Error result = loadGameStream(saveFile);

	if (!isOriginal && result.getCode() == Common::kNoError) {
		ExtendedSavegameHeader header;
		if (MetaEngine::readSavegameHeader(saveFile, &header))
			setTotalPlayTime(header.playtime);
	}

	delete saveFile;
	return result;
}

Common::Error Engine::syncGameStream(Common::Serializer &s) {
	 // Synchronize game logic, persisted fields, and queue
	_logics->synchronize(s);
	Persisted::synchronize(s);
	_queue.synchronize(s);

	return Common::kNoError;
}

} // namespace Early
} // namespace Legend
