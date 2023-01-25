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

#ifndef LEGEND_EARLY_ENGINE_H
#define LEGEND_EARLY_ENGINE_H

#include "legend/legend.h"
#include "legend/early/core/persisted.h"
#include "legend/early/logic/logics.h"
#include "legend/early/logic/room.h"
#include "legend/early/logic/queue.h"
#include "legend/early/parser/parser.h"
#include "legend/early/parser/vocab.h"

namespace Legend {
namespace Early {

enum LoadType { LOAD_NONE = 0, LOAD_RESTART = 1, LOAD_SAVE = 2, LOAD_UNDO = 3 };

class Engine : public LegendEngine, virtual public Early::Persisted {
public:
	Logics *_logics = nullptr;
	Parser::Parser *_parser = nullptr;
	Parser::Vocab *_vocab = nullptr;
	const Room *_currentRoom = nullptr;
	Queue _queue;

public:
	Engine(OSystem *syst, const LegendGameDescription *gameDesc);
	~Engine() override;

	bool initialize() override;
	void deinitialize() override;

	/**
	 * Load game
	 */
	Common::Error loadGameState(int slot);

	/**
	 * Load game data
	 */
	Common::Error loadGameStream(Common::SeekableReadStream *stream) override {
		Common::Serializer s(stream, nullptr);
		return syncGameStream(s);
	}

	/**
	 * Save game data
	 */
	Common::Error saveGameStream(Common::WriteStream *stream, bool isAutosave = false) override {
		Common::Serializer s(nullptr, stream);
		return syncGameStream(s);
	}

	/**
	 * Synchronize savegame data
	 */
	Common::Error syncGameStream(Common::Serializer &s);

	/**
	 * Handles game control messages
	 */
	bool msgGame(const GameMessage &msg) override;

	/**
	 * Handles logic messages
	 */
	bool msgLogic(const LogicMessage &msg) override;

	/**
	 * Sets the current room
	 */
	void setRoom(int roomNum);

	int loadGame(LoadType mode, const char *filename = nullptr);
};

extern Engine *g_engine;

} // namespace Early
} // namespace Legend

#endif
