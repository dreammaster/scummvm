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

#ifndef SPYCRAFT_GAME_PDA_H
#define SPYCRAFT_GAME_PDA_H

#include "spycraft/game/script.h"
#include "spycraft/game/game.h"
#include "spycraft/game/user.h"
#include "spycraft/game/timer.h"
#include "spycraft/game/dialog.h"
#include "spycraft/game/interface.h"
#include "spycraft/game/movie.h"

namespace Spycraft {

extern Movie *theMovie;

class PDA : public IntrView {
public:
	char *name;
	DialTree *myDialogTree;
	int PDAOpen;
	char *myMovie;
	Object *whoToCue;
	short blinking;

	PDA();
	//void dispose();
	void init(int theX, int theY);
	void blink(void);
	void playVLink(void);
	void VLinkNotify(char *theMovie, unsigned int waitTime);
	void VLinkNotify(char *theMovie, unsigned int waitTime, Object *whoToCue);
	int handleEvent(MADEEventStamp *event);
	void respond(void);
};

class VLinkScript : public Script {
	void changeState(int newState);
};

class PDATimer : public Timer {
public:
	char *myMovie;
	void cue();
};

class openPDAMovie : public Script {
public:
	openPDAMovie();
	void changeState(int);
};

extern PDA *thePDA;

} // namespace Spycraft

#endif

