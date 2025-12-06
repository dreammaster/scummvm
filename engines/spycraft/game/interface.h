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

#ifndef SPYCRAFT_DMADE_ADVBITS_H
#define SPYCRAFT_DMADE_ADVBITS_H

#include "spycraft/game/actor.h"
#include "spycraft/game/list.h"
#include "spycraft/game/script.h"
#include "spycraft/game/game.h"
#include "spycraft/game/user.h"
#include "spycraft/game/globals.h"
#include "spycraft/game/colby.h"
#include "spycraft/game/cursor.h"

namespace Spycraft {

#define INV_X 0 //63
#define INV_Y 374

#define GUN_X 186 //250
#define GUN_Y 350

class OpenInterface : public Script {
public:
	void changeState(int);
};

class CloseInterface : public Script {
public:
	void changeState(int);
};


class IntrFeature : public Feature {
public:
	void init(int theX, int theY, int theWidth, int theHeight);
	void dispose();
};

class IntrView : public View {
public:
	void init(int x, int y);
	void dispose();

};

class IntrProp : public IntrView {
public:
	unsigned __cycle_cnt;
	Cycle *cycler;
	int 		cycleSpeed;
	int	closed;
	int myTime;
	IntrProp();
	~IntrProp();
	void open();
	void close();
	void doit();
	void setCycle(Cycle *theCycle);
};

class BackInv : public IntrFeature {
public:
	BackInv();
	int handleEvent(MADEEventStamp *event);
};

class ForwardInv : public IntrFeature {
public:
	ForwardInv();
	int handleEvent(MADEEventStamp *event);
};

class Briefcase : public IntrView {
public:
	const char *name;
	int32  	closed;
	int32   myTime;
	BackInv *backButton;
	ForwardInv *forwardButton;

	Briefcase();
	void open();
	void close();
	int handleEvent(MADEEventStamp *event);
};



class Intrface : public List {
public:
	short isOpen;
	short blinking;
	short canBlink;
	short enabled;
	Intrface();
	~Intrface();
	void show();
	void hide();
	void doit();
	void blink();
	void disable();
	void enable();
	int handleEvent(MADEEventStamp *event);
};

class PDAProp : public IntrProp {
public:
	PDAProp();
	int handleEvent(MADEEventStamp *event);
	void respond(void);
};

extern PDAProp *pdaProp;

} // namespace Spycraft

#endif
