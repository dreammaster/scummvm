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

#ifndef SPYCRAFT_GAME_SNDANAL_H
#define SPYCRAFT_GAME_SNDANAL_H

#include "spycraft/game/globals.h"
#include "spycraft/game/flag.h"
#include "spycraft/game/print.h"
#include "spycraft/game/movie.h"
#include "spycraft/game/made.h"
#include "spycraft/game/game.h"
#include "spycraft/game/feature.h"
#include "spycraft/game/script.h"
#include "spycraft/game/actor.h"								   
#include "spycraft/game/sound.h"
#include "spycraft/game/colby.h"
#include "spycraft/game/interface_puzzle.h"
#include "spycraft/game/web.h"
#include "spycraft/game/vlink.h"

namespace Spycraft {

#define		SUB	10
class Sndanal;

extern Sndanal *anal;

class Speaker : public IntrPuzzProp {
public:
	int looping;
	IntrPuzzTimer *timer;
	Speaker();
	void doit(void);
	void cue(void);
};

class ABS : public IntrPuzzView {
public:
	ABS();
};

class Cover : public IntrPuzzView {
public:
	Cover();
};

class LinkButt : public Button {
protected:
	char *name;

public:
	int linkto;
	LinkButt();
	void activate(void);
};

class Tab : public IntrPuzzTextFeature {
public:
	int highlighted;
	int clicked;
	int slot;
	int pos;
	int in;
	int out;
	int linkto;
	int wavnum;
	char label[64];
	LinkButt *doss;

	Tab();
	void init(int theX, int theY, int color);
	int handleEvent(MADEEventStamp *);
	void specific(void);
	virtual void activate(void);
};

class PuzzBack : public IntrPuzzView {
protected:
	char *name;

public:
	PuzzBack();
};

class TextWin : public IntrPuzzView {
protected:
	char *name;
public:
	TextWin();
};

class MarkPoint : public IntrPuzzView {
protected:
	char *name;
public:
	MarkPoint(int startx, int startcel);
};

class IdButt : public Button {
protected:
	char *name;

public:
	TextWin *textwin;
	int 	  go;
	int 	  count;
	int 	  count2;
	int 	  count3;
	IdButt();
	void activate(void);
	void doit(void);
	void cue(void);
};

class InButt : public Button {
protected:
	char *name;

public:
	ABS *abs;
	InButt();
	void activate(void);
};

class CanButt : public Button {
protected:
	char *name;
public:
	CanButt();
	void activate(void);
};

class MapButt : public Button {
protected:
	char *name;
public:
	MapButt();
	void activate(void);
};

class PlayButt : public Button {
protected:
	char *name;
public:
	PlayButt();
	void cue(void);
	void activate(void);
};

class StopButt : public Button {
protected:
	char *name;
public:
	StopButt();
	void activate(void);
};

class LtButt : public Button {
protected:
	char *name;
public:
	LtButt();
	void activate(void);
	void doit(void);
};

class RtButt : public Button {
protected:
	char *name;
public:
	RtButt();
	void activate(void);
	void doit(void);
};

class SeekSlider : public Slider {
protected:
	char *name;
public:
	SeekSlider();
	int handleEvent(MADEEventStamp *);
};

class Sndanal : public IntrPuzzPage {
public:
	Tab *tabs[8];
	char moviename[256];
	int curTab;
	char *names[8];
	int links[8];
	int namesPoints[8][2];
	int marks[100][4];
	int range;
	int playing;
	int puzzle;
	int inmark;
	int anchor;
	int outmark;
	int outclick;
	int lastloc;
	SeekSlider *seeker;
	MapButt *mapbutt;
	IdButt *idbutt;
	InButt *inbutt;
	PlayButt *playbutt;
	StopButt *stopbutt;
	MarkPoint *ledmarks[25];
	WebReport *report;

	Sndanal(int number);
	~Sndanal();
	void dispose(void);
	void init(void);
	int handleEvent(MADEEventStamp *);
	void saveState(void);
	void doit(void);
	void specific(void);
	void updateplay(void);
	void discharge(void);
};

class SndInfo : public Object {
public:
	Tab	tabs[8];
	int lastloc;
	int puzzle;
	int anchor;
	int inmark;
	int outmark;
	int	showin;
	int	showout;
	int curTab;
	char moviename[256];
	int markerx;
	int seekerstep;
	SndInfo();
	void load();
	void save();
	void dispose();
};

class MsgScript : public Script {
public:
	MsgScript() {
		name = "msgscript";
	};
	void changeState(int);
};

class MovieScript : public Script {
public:
	MovieScript() {
		name = "moviescript";
	};
	void changeState(int);
};

class RedrawScript : public Script {
public:
	RedrawScript() {
		name = "redrawscript";
	};
	void changeState(int);
};

class DelayButtScript : public Script {
public:
	DelayButtScript() {
		name = "delayscript";
	};
	void changeState(int);
};

} // namespace Spycraft

#endif
