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

#include "engines/util.h"
#include "legend/games/gateway/engine.h"
#include "legend/games/gateway/logic/logics.h"
#include "legend/games/gateway/parser/parser.h"
#include "legend/games/gateway/parser/vocab.h"
#include "legend/games/gateway/views/views.h"
#include "legend/early/gfx/screen.h"
#include "legend/utils/engine_data.h"
#include "legend/console.h"

namespace Legend {
namespace Early {
namespace Gateway {

GatewayEngine *g_engine;

GatewayEngine::GatewayEngine(OSystem *syst, const LegendGameDescription *gameDesc) :
		Early::Engine(syst, gameDesc) {
	g_engine = this;
}

GatewayEngine::~GatewayEngine() {
	g_engine = nullptr;
}

bool GatewayEngine::initialize() {
	// Initialize 640x480 paletted graphics mode
	initGraphics(640, 480);

	// Set the engine's debugger console
	setDebugger(new Console());

	Common::U32String errorMsg;
	if (!load_engine_data("gateway", true, 1, 0, errorMsg)) {
		GUIErrorMessage(errorMsg);
		return false;
	}

	(void)Engine::initialize();

	_logics = new Logic::Logics();
	_parser = new Gateway::Parser::Parser();
	_vocab = new Gateway::Parser::Vocab();
	setRoom(LOGIC_STARTING_ROOM);

	return true;
}

void GatewayEngine::runGame() {
	// Create the collection of game views and set the active view
	Views::Views views;
	g_events->replaceView("Interface");
	_mouseCursor->show();

	// Some dummy text
	for (int i = 0; i < 10; ++i)
		TextMessage::display("This is a test. This is only a test. ");

	send("Input", GameMessage("GET_INPUT"));

	// Run the game
	Engine::runGame();
}

void GatewayEngine::showStartup() {

}

void GatewayEngine::startGameplay() {
	_struc24.clear();
	Common::fill(&_array1[0], &_array1[8], 0);
	for (int i = 0; i < 5; ++i)
		_random04[i] = getRandomNumber(4);

	_val4 = 720;
	_val194 = 8;

	_queue.add(33, 179);
	_queue.add(23, 1200 - _val4);
	_queue.add(24, 1320 - _val4);
	_queue.add(37, 1440 - _val4);
	_queue.add(42, 1500 - _val4);

	initPersisted(238);
	initPersisted(247);
	initPersisted(248);
	initPersisted(249);

	_currentRoom->call(25);
	// TODO: ?
	_currentRoom->call(7);
}

void GatewayEngine::initPersisted(int roomNum) {
	const uint16 ROOMS[4] = { 238,  247,  248,  249 };
	int idx;
	for (idx = 0; idx < 4 && ROOMS[idx] != roomNum; ++idx) {
	}

	if (idx != 4) {
		_logics->updateHandler(365, 0, 0);
		int logicNum = _logics->getUnkHandler(353, 0);
		_val235[idx] = logicNum;
		_logics->setUnkHandler(353, 0, 0);

		while (logicNum) {
			_logics->setPrehandler(logicNum, 0);
			logicNum = _logics->getVal1(logicNum);
		}

		const byte *bits1 = _logics->getBits(353);
		const byte *bits2 = _logics->getBits(354);
		Common::copy(bits1, bits1 + 4, &_val233[idx][0]);
		Common::copy(bits2, bits1 + 4, &_val234[idx][0]);

		_logics->updateHandler(365, 353, 0);
	}
}


} // namespace Gateway
} // namespace Early
} // namespace Legend
