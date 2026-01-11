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

#ifndef SPYCRAFT_GAME_ACTOR_H
#define SPYCRAFT_GAME_ACTOR_H

#include "spycraft/game/feature.h"
#include "spycraft/game/list.h"
#include "spycraft/game/motion.h"
#include "spycraft/game/made.h"
#include "spycraft/game/print.h"

namespace Spycraft {

class View : public Feature {
public:
	int view = 0;				// the number of the resource
	int loop = 0;
	int cel = 0;
	uint16 priority = 0;		// Z position
	SpriteSharedPtr mySprite;
	int height = 0;
	int width = 0;
	ATS *myATS = nullptr;
	Print *myPrint = nullptr;

	View();
	~View();

	void init(int x_start, int y_start);
	void init(int x_start, int y_start, int showName); 	//DEBUG FOR GREG
	virtual void show(void);
	virtual void hide(void);
	void draw(void);      //show me on the screen
	void draw(bufferNum theBuffer);      //show me on the screen
	void posn(int theX, int theY);
	void setRect(void);
	void setView(int theView);
	void setLoop(int theLoop);
	void setCel(int theCel);
	void addToPic(void);
	int onMe(int theX, int theY);
	int onMe(MADEEventStamp *event);
	int onMe(Feature *obj);
	void setPri(uint16 thePri);	   //change the Z psuedo coordinate
	void noSkip();                 //announce that this view's sprite ha s no skip
	void dispose(void);
};

class Prop : public View {
public:
	unsigned __cycle_cnt = 0;
	Cycle *cycler = nullptr;
	int cycleSpeed = 0;

	Prop();
	~Prop();

	void doit();
	void setLoop(int theLoop);

	virtual void setCycle(Cycle *theCycle);
};

class Cast : public List {
public:
	Cast();

	int handleEvent(MADEEventStamp *);
	void updateRects(int x_offset, int y_offset); //for scrolling
};

extern Cast *cast;

} // namespace Spycraft

#endif
