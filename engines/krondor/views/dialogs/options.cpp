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

#include "krondor/views/dialogs/options.h"
#include "krondor/krondor.h"

namespace Krondor {
namespace Views {
namespace Dialogs {

enum {
	OPT_CANCEL        = 18,
	OPT_CONTENTS      = 46,
	OPT_NEW_GAME      = 49,
	OPT_PREFERENCES   = 25,
	OPT_QUIT          = 32,
	OPT_RESTORE       = 19,
	OPT_SAVE          = 31
};

Options::Options() : Dialog("Options",
		"req_opt0.dat", "options.pal", "options0.scx",
		"game.fnt") {
}

bool Options::msgFocus(const FocusMessage &msg) {
	bool inGame = msg._priorView && msg._priorView->getName() == "Game";
	_requestName = inGame ? "req_opt1.dat" : "req_opt0.dat";
	_screenName = inGame ? "options1.scx" : "options0.scx";

	return Dialog::msgFocus(msg);
}

bool Options::msgAction(const ActionMessage &msg) {
	switch (msg._action) {
	case OPT_PREFERENCES:
		replaceView("Preferences");
		break;

	case OPT_QUIT:
		g_engine->quitGame();
		break;

	default:
		break;
	}

	return true;
}

} // namespace Dialogs
} // namespace Views
} // namespace Krondor
