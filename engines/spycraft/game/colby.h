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

#ifndef SPYCRAFT_GAME_COLBY_H
#define SPYCRAFT_GAME_COLBY_H

#include "spycraft/game/made.h"
#include "spycraft/game/game.h"
#include "spycraft/game/cursor.h"
#include "spycraft/game/globals.h"

namespace Spycraft {

class Colby : public Game {
public:
	bool gunning;   //the player has the gun drawn

	Colby();
	~Colby();

	void init(void);
	void newRoom(Room *);
	void handsOn(void);        //player can input
	void handsOff(void);       //player can not input
	int handleEvent(MADEEventStamp *);
	void resetHotspot(void);   //clear current hotspot
	void checkINI(void);
};

void flushMessageCue(void);

//Cursors

#define DoorCursor NorthCursor

class EastCursor : public MadeCursor {
public:
	EastCursor();
};

class WestCursor : public AniCursor {
public:
	WestCursor();
};

class NorthCursor : public AniCursor {
public:
	NorthCursor();
};

class SouthCursor : public AniCursor {
public:
	SouthCursor();
};

class NortheastCursor : public AniCursor {
public:
	NortheastCursor();
};

class NorthwestCursor : public MadeCursor {
public:
	NorthwestCursor();
};

class SoutheastCursor : public MadeCursor {
public:
	SoutheastCursor();
};

class SouthwestCursor : public MadeCursor {
public:
	SouthwestCursor();
};

class GunCursor : public BlinkCursor {
public:
	GunCursor();
};

class ArrowCursor : public BlinkCursor {
public:
	ArrowCursor();
};

class InvCursor : public BlinkCursor {
public:
	InvCursor();
};

class WaitCursor : public MadeCursor {
public:
	WaitCursor();
};

} // namespace Spycraft

#endif
