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

#ifndef SPYCRAFT_GAME_GAME_H
#define SPYCRAFT_GAME_GAME_H

#include "spycraft/game/made.h"
#include "spycraft/game/object.h"
#include "spycraft/game/script.h"
#include "spycraft/game/list.h"
#include "spycraft/game/cursor.h"

namespace Spycraft {

class Room : public Object {
public:
	MADERect 	display_rect;		//the rectangle by which our backround is bounded
	bufferNum 	display_buffer;		//this is where we draw this room's pics, text, movies...
	int			number;				//backround resrcID
	int			show_style;			//fade, wipe, etc.
	int			steps;
	int			stepTime;

	Room();
	~Room();

	void zoomTo(int theX, int theY);
	virtual void init(int);
	virtual void init(void);
	virtual void drawPic(int pic_num);
	virtual void horizontalScroll(int x_offset);
	virtual void verticalScroll(int y_offset);
};

class Game : public Object {
public:
	MadeCursor *cursor;
	uint16  	curVolume;
	int 	  	project_num;
	bufferNum	display_buffer;
	char *prevRoomName;   //don't use ptr to room object because it has been disposed
	int			isMoviePlaying;
	short		enableIntrDuringMovie;
	Game();
	~Game();

	void doit(void);
	virtual void init(void) {
	};
	virtual void newRoom(Room *nextRoom);
	virtual void handsOff(void);
	virtual void handsOn(void);
	virtual void setCursor(MadeCursor *theCursor);
	virtual void save(void) {
	};
	virtual void restore(void) {
	};
	virtual void masterVolume(uint16 newVol);
	virtual void resetHotspot(void);
	void suspend(void);
	int startNet(char *);
};

extern Room *curRoom;
extern Game *theGame;
extern List *permList;

extern void (*gameHandler) (void);

} // namespace Spycraft

#endif

