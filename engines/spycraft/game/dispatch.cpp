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

#include "spycraft/game/dispatch.h"
#include "spycraft/game/made.h"
#include "spycraft/game/object.h"
#include "spycraft/game/macro.h"
#include "spycraft/game/user.h"
#include "spycraft/game/game.h"
#include "spycraft/game/colby.h"
#include "spycraft/game/movie.h"

namespace Spycraft {

extern int currentFocus;
extern short restoring;
extern Movie *theMovie;
unsigned long absoluteTime;
unsigned long oldTime;
unsigned long elapsedTime;
bool suspended;

void StartScript() {
	theGame->init();
}

//DEBUG////////////////////////
short OnRightButtonDown(MADEEventStamp *event) {

	if (!suspended) {
		event_type = USER_RIGHT_DOWN;
		return (user->handleEvent(event));
	}
	return false;
}

short OnRightButtonUp(MADEEventStamp *event) {

	if (!suspended) {
		event_type = USER_RIGHT_UP;
		return (user->handleEvent(event));
	}
	return false;
}

short OnMiddleButtonDown(MADEEventStamp *event) {
	if (!suspended) {
		event_type = USER_MID_DOWN;
		return (user->handleEvent(event));
	}
	return false;
}

short OnMiddleButtonUp(MADEEventStamp *event) {
	event_type = USER_MID_UP;
	return (user->handleEvent(event));
}

short OnLeftDouble(MADEEventStamp *event) {
	return true;
}

short OnLeftButtonDown(MADEEventStamp *event) {
	if (!suspended) {
		event_type = USER_LEFT_DOWN;
		return (user->handleEvent(event));
	}
	return false;
}

short OnLeftButtonUp(MADEEventStamp *event) {
	if (!suspended) {
		event_type = USER_LEFT_UP;
		return (user->handleEvent(event));
	}
	return false;
}

short OnKeyDown(MADEEventStamp *event) {

	if (!suspended) {
		event_type = USER_KEY_DOWN;
		return (user->handleEvent(event));
	}
	return false;
}

short OnKeyUp(MADEEventStamp *event) {
	if (!suspended) {
		event_type = USER_KEY_UP;
		return (user->handleEvent(event));
	}
	return false;
}

short OnIdle(MADEEventStamp *event) {

	absoluteTime = (event->clock_lo) >> 4;
	if ((!restoring) && (!suspended)) {

		elapsedTime = absoluteTime - oldTime;
		event_type = 0;
		mouse_x = event->cursor_pt.x;
		mouse_y = event->cursor_pt.y;
		gameTime += elapsedTime;
		if (currentFocus)
			user->handleEvent(event);

		if (theGame)
			theGame->doit();
	}
	oldTime = absoluteTime;
	return 0;
}

void EndScript() {
	if (cast) {
		Node *n = cast->head;

		while (n) {
			((View *)(n->data))->mySprite = NULL;
			n = n->next;
		}
	}
}

// Disk has been inserted
short OnDiskInsert(MADEEventStamp *event) {
	// No implementation
	return 0;
}

} // namespace Spycraft
