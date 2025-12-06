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

#ifndef SPYCRAFT_GAME_DIALOG_H
#define SPYCRAFT_GAME_DIALOG_H

#include "spycraft/game/object.h"
#include "spycraft/game/list.h"
#include "spycraft/game/made.h"
#include "spycraft/game/print.h"
#include "spycraft/game/actor.h"

namespace Spycraft {

class DialTree;

class DItem;

class Dialog : public List {
public:
	int     	bottom;
	int     	x;
	int     	y;
	char *resourceID;
	int		from;
	int		to;
	int 		loopFrom;
	int 		loopTo;
	DialTree *myTree;

	Dialog();
	virtual void show();
	void doit();
	void init(int theX, int theY, long resourceNum);
	void init(long resourceNum, DialTree *curTree);
	void init(long resourceNum);//TEMP DEBUG
	void add(long textNum);
	void add(char *theText, Dialog *nextDialog);
	void add(int index, Dialog *nextDialog);
	void add(DItem *theItem, Dialog *whatsNext);
	void add(DItem *theItem);

	void addMovieInfo(int fromFrame, int toFrame, int loopFromFrame, int loopToFrame);
	void	cue(void);
	virtual void hide();
	void dispose();
};

class DItem : public Object {
public:
	int     highlighted;
	Print *myPrint;
	int     width;
	int     top;
	int     left;
	int     bottom;
	int     right;
	bool 	  hasBeenChosen;
	Dialog *myDialog;
	Dialog *nextDialog;
	int 	  iconItem;
	View *myView;

	DItem();
	~DItem();
	bool init();
	bool onMe(MADEEventStamp *event);
	void setRect(int theTop, int theLeft, int theBottom, int theRight);
	void highlight(bool hiOrLow);
	int handleEvent(MADEEventStamp *event);
	void cue();
	void doit();
	void addInfo(int msgFile, int index, Dialog *whatsNext);
	void addInfo(int view, int loop, int cel, int x, int y, Dialog *whatsNext);
	void dispose();
};

class vlinkDItem : public DItem {
public:
	void cue();
	void doit();
};

class DialTree : public Object {
public:
	int negativeParam;
	const char *resourceID;
	Dialog *rootNode;
	Dialog *curNode;
	char **myArray;
	Object *caller;
	DialTree();
	int handleEvent(MADEEventStamp *event);
	void show(Object *whoToCue);
	virtual void init() {
	};
	void dispose();
};

} // namespace Spycraft

#endif

