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

#include "ultima/ultima2/data/game.h"
#include "ultima/ultima2/ultima2.h"

namespace Ultima {
namespace Ultima2 {
namespace Data {

void Game::startGame() {
	const auto &player = g_engine->_player;

	if ((player._mapNum % 10) < 4)
		// Planet overworld or town map
		g_engine->replaceView("Overworld");
	else
		g_engine->replaceView("Dungeon");

	promptForCommand();
}

void Game::message(const Common::String &name, const char *param) {
	GameMessage msg(name);
	if (param)
		msg._stringValue = param;
	g_engine->focusedView()->send(msg);
}

void Game::promptForCommand() {
	g_engine->focusedView()->send(GameMessage("COMMAND"));
}

void Game::doCommand(KeybindingAction action) {
	switch (action) {
	case KEYBIND_PASS:
		message("INFO", "PASS");
		break;

	default:
		message("INFO", "-ILLEGAL COMMAND!");
		break;
	}

	endOfTurn();
}

void Game::endOfTurn() {
	auto &player = g_engine->_player;

	bool foodFlag = player._foodSubCtr < 10;
	player._foodSubCtr -= 10;
	if (foodFlag && player._food-- == 0) {
		dead();
	} else {
		g_engine->focusedView()->redraw();
		promptForCommand();
	}
}

void Game::dead() {
	auto &player = g_engine->_player;
	player._hp = 0;
	player._food = 0;
	player._foodSubCtr = 0;
	player._experience = 0;
	player._gold = 0;

	message("DEAD");
}

} // namespace Data
} // namespace Ultima2
} // namespace Ultima
