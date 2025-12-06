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

#ifndef SPYCRAFT_INTRPUZZ_H
#define SPYCRAFT_INTRPUZZ_H

#include "spycraft/game/actor.h"
#include "spycraft/game/act.h"
#include "spycraft/game/interface.h"
#include "spycraft/game/made.h"
#include "spycraft/game/dlgbox.h"

namespace Spycraft {

#define PUZZLE_FOCUS		10

enum {
	WEB_HOME,						   // 0
	WEB_SOUND_BIRD,				   // 1
	WEB_SOUND_C8,					   // 2
	WEB_SOUND_C13,					   // 3
	WEB_SOUND_C21,					   // 4
	WEB_SOUND_C26,					   // 5
	WEB_SOUND_B6,					   // 6
	WEB_SOUND_B14,					   // 7
	WEB_SOUND_B14B,				   // 8
	WEB_SOUND_B20,					   // 9
	WEB_SOUND_M14,					   //10
	WEB_SOUND_M28,					   //11
	WEB_SOUND_S19,					   //12
	WEB_SOUND_S22,					   //13
	WEB_SOUND_VRC5,				   //14
	WEB_SOUND_VRC9,				   //15
	WEB_SOUND_VRC23,				   //16
	WEB_SOUND_VRB9,				   //17
	WEB_SOUND_VRB15,				   //18
	WEB_SOUND_VRM5,				   //19
	WEB_SOUND_VRM9,				   //20
	WEB_SOUND_VRS9,				   //21
	WEB_SOUND_VRS29,				   //22
	WEB_KAT,						   //23
	WEB_VLINKS,					   //24
	WEB_IMAGE,				    	   //25
	WEB_MAP,				    	   //26
	WEB_COM,						   //27
	WEB_DATA,						   //28
	WEB_NOTE,				    	   //29		  
	WEB_TRAVEL,				       //30
	WEB_IMAGEPLATE,				   //31
	WEB_KATMATCH,					   //32
	WEB_AMESMATCH,					   //33
	WEB_IMAGETANK,					   //34
	WEB_IMAGECOURIER,				   //35
	WEB_IMAGEPEG,					   //36
	WEB_IMAGEKNEEID,				   //37
	WEB_KNEECAPSMATCH,				   //38
	WEB_GRENDELMATCH,				   //39
	WEB_PHOTODOC,					   //40
	WEB_CYPHERPITONE,				   //41
	WEB_CYPHERPITTWO,				   //42
	WEB_CYPHERPITTHREE,			   //43
	WEB_CYPHERMOLEONE,				   //44
	WEB_CYPHERMOLETWO,				   //45
	WEB_CYPHERMOLETHREE,			   //46
	WEB_CYPHERMOLEFOUR,			   //47
	WEB_YINGCYPHER,				   //48
	WEB_TARGET,					   //49
	WEB_POWERBOOKONE,				   //50
	WEB_POWERBOOKTWO,				   //51
	WEB_POWERBOOKTHREE,			   //52
	WEB_POWERBOOKFOUR,				   //53
	WEB_POWERBOOKFIVE,				   //54
	WEB_POWERBOOKSIX,				   //55
	WEB_POWERBOOKSEVEN,			   //56
	WEB_KNEE,						   //57
	WEB_ONYXHUNTONE,				   //58
	WEB_ONYXHUNTTWO,				   //59
	WEB_CYPHEREMPTY,				   //60
	WEB_REPORT						   //61
};

#define WEB_HTMLPAGE    10100					  

class WebHome;
extern int helppage;
extern int lasttool;
extern int reportnum;
extern int stopglobe;
extern int dumping;
extern int currentFocus;
extern Intrface *intrface;
extern WebHome *homobutt;
extern List *announceList;
extern List *usedList;

class IntrPuzzTextFeature : public TextFeature {
public:
	~IntrPuzzTextFeature();
	void init(int theX, int theY, int theWidth, int theHeight, int color);
	void dispose();
};

class CommandLine : public Feature {
public:
	int active;
	char	text[256];
	Print *commandPrint;

	CommandLine() {
	};
	CommandLine(int x, int y, int width, int height, const char *text);
	~CommandLine();
	void dispose();
	void init(int x, int y, int width, int height);
	virtual void hide(void);
	virtual void show(void);
	int handleEvent(MADEEventStamp *event);
};

class WebCommandLine : public CommandLine {
public:
	WebCommandLine(int x, int y, int width, int height, const char *text);
	void dispose();
	void init(int x, int y, int width, int height);
};

class IntrPuzzFeature : public Feature {
public:
	void init(int theX, int theY, int theWidth, int theHeight);
	void dispose();
};

class IntrPuzzView : public View {
public:
	void init(int x, int y, int pic_id, int loop_id, int cel_id);
	void init(int x, int y);
	void dispose();
	virtual void draw(void);      //show me on the screen
	//		int handleEvent ( MADEEventStamp * );
};

class IntrPuzzProp : public IntrPuzzView {
public:

	unsigned __cycle_cnt;
	Cycle *cycler;
	int 		cycleSpeed;

	IntrPuzzProp();
	~IntrPuzzProp();
	void dispose(void);
	void doit();

	virtual void setCycle(Cycle *theCycle);
};

class IntrPuzzActor : public IntrPuzzProp {
public:

	unsigned __move_cnt;

	int xInc;
	int yInc;
	int moveSpeed;
	MoveTo *mover;
	void setSpeed(int theSpeed);
	void setInc(int theXInc, int theYInc);
	IntrPuzzActor();
	~IntrPuzzActor();
	void doit();
	virtual void setMotion(MoveTo *theMover);
};

class Button : public IntrPuzzView {
public:
	int handleEvent(MADEEventStamp *event);
	virtual void activate();
};

class WebButton : public IntrPuzzView {
public:
	void init(int x, int y, int pic_id, int loop_id, int cel_id);
	int handleEvent(MADEEventStamp *event);
	virtual void activate();
};

class WebView : public IntrPuzzView {
public:
	void init(int x, int y, int pic_id, int loop_id, int cel_id);
};

class Marker : public IntrPuzzView {
protected:
	const char *name;
public:
	int x;
	int y;
	void init(int theX, int theY, int pic_id, int loop_id);
	Marker();
};

class Slider : public IntrPuzzFeature {
public:
	int			num_steps;
	int			step;
	Marker *marker;
	int 			buttonDown;
	int           IDnum;

	Slider();
	~Slider();
	void dispose(void);
	bool init(int theX, int theY, int theWidth, int theHeight, int id, int loop_id);
	int handleEvent(MADEEventStamp *event);
};

class Website : public Object {
public:
	char		htmlname[256];
	Website();
	void dispose(void);
};

class Webmap : public List {
public:

	Website *findPage(void);
	Website *findPrevInst(int pagenum);
	Website *findPrevInst(char *pagename);
	Website *findPrevPage(void);
	Website *findNextPage(void);
	void dumpAfter(void);
	void clearFlags(void);
};

class IntrPuzzPage : public Object {
public:
	Website *site;

	IntrPuzzPage();
	void init(int pagenum);
	void init(char *pagename);
	void dispose(void);
	virtual void saveState();
};

class IntrPuzz : public Intrface {
public:
	MADERect 	display_rect;		// the rectangle by which our backround is bounded
	bufferNum 	display_buffer;		// this is where we draw this room's pics, text, movies...
	int			number;				// backround resrcID
	int			show_style;			// fade, wipe, etc.
	int			puzzle_focus;
	Object *caller;
	IntrPuzzPage *page;
	int 		soundNum;
	int 		musicNum;

	IntrPuzz();
	~IntrPuzz();

	void clear_sliderDown(MADEEventStamp *event);
	void init(int pic_num);
	virtual void drawPic(int pic_num);
	void dispose(void);
	void quitIntrface(void);
	int handleEvent(MADEEventStamp *event);
	int isStateSaved(const char *save_name);
	void remove_MarkedItems();
	void doit(void);

	void show();
	void hide();
};

class IntrPuzzTimer : public Object {
public:
	Object *caller;
	long 		cycles;
	long 		ticks;
	long 		seconds;
	long 		old_ticks;
	long 		old_seconds;

	IntrPuzzTimer();

	void doit(void);
	void cue(void);
	void dispose(void);
	virtual void pause(int tOrF);
	virtual void setCycles(int num_cycles);
	virtual void setCycles(int num_cycles, Object *whoToCue);
	virtual void setTicks(int num_ticks);
	virtual void setTicks(int num_ticks, Object *whoToCue);
	virtual void setReal(int num_seconds);
	virtual void setReal(int num_seconds, Object *whoToCue);
};

class WebHome : public WebButton {
public:
	WebHome();
	void activate(void);
};

extern HotspotList *oldlist;
extern HotspotList *puzzle_hotspots;
extern IntrPuzz *intrpuzz;
extern List *web_intrface;
extern Webmap *webmap;
extern int backbutton;
extern int fwdbutton;
extern int usedbutton;

extern void changePage(Website *id);
extern void launchPuzz(char *linkTo);

} // namespace Spycraft

#endif
