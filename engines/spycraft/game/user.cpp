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

#include "spycraft/game/user.h"
#include "spycraft/game/macro.h"
#include "spycraft/game/feature.h"
#include "spycraft/game/actor.h"
#include "spycraft/game/game.h"
#include "spycraft/game/verbs.h"
#include "spycraft/game/interface.h"

namespace Spycraft {

int event_type;
int mouse_x;
int mouse_y;
unsigned long gameTime;
MADEEventStamp theEvent;

extern Intrface *intrface;

User TheUser;
User *user;

User::User() {
	name = "User";
	user = this;
	event_type = 0;
	input = true;
	message = DO_V;
}

User::~User() {
	user = NULL;
}

int User::handleEvent(MADEEventStamp *event) {
	int ret = 0;
	if (gameHandler) {
		memcpy(&theEvent, event, sizeof(MADEEventStamp *));
		return false;
	}
	if (!theGame->handleEvent(event) && input) {

		if (!ret && intrface)
			ret = intrface->handleEvent(event);

		if (!ret)
			ret = cast->handleEvent(event);

		if (!ret)
			ret = features->handleEvent(event);

		if (!ret) {
			ret = curRoom->handleEvent(event);
		}
		//if (event_type)
			//flushMessageCue(); //added 2/17

		event_type = NULL;

		//flushMessageCue(); //added 2/17

		return (ret);
	} else
		//flushMessageCue(); //added 2/17
		return false;
}

void User::canInput(int tOrF) {
	input = tOrF;
}

} // namespace Spycraft
