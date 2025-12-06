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

#ifndef SPYCRAFT_GAME_HTML_H
#define SPYCRAFT_GAME_HTML_H

#include "spycraft/game/made.h"
#include "spycraft/game/game.h"
#include "spycraft/game/feature.h"
#include "spycraft/game/script.h"
#include "spycraft/game/actor.h"
#include "spycraft/game/sound.h"
#include "spycraft/game/colby.h"
#include "spycraft/game/interface_puzzle.h"
#include "spycraft/game/flag.h"
#include "spycraft/game/globals.h"
#include "spycraft/game/file.h"
#include "spycraft/dmade/advtext.h"

namespace Spycraft {

#define MAX_LENGTH 	3200
#define MAX_COLUMNS 1024
#define MAX_ROWS 50
#define LEFT_MARGIN 3
#define TOP_MARGIN 63
#define RIGHT_MARGIN 570
#define BOTTOM_MARGIN 400

extern int LeftMargin;
extern int TopMargin;

class HTMLMarker : public Object {
	public:
	HTMLMarker();
	void dispose(void);
};
class AnnounceMarker : public Object {
	public:
	AnnounceMarker();
	void dispose(void);
};

class HTMLAnnounce: public Object {
	public:
	int soundNum;
	int stream;
	Sound* entrance;
	HTMLAnnounce(int value,int stream);
	~HTMLAnnounce();
};

class HTMLPause: public Object {
	public:
	int wait;
	int page;
	HTMLPause(int value,int page);
};

class AnnounceScript : public Script {
public:
	AnnounceScript() {name = "announcescript";};
	void changeState ( int );
};
class ScrollerScript : public Script {
public:
	ScrollerScript() {name = "scrollerscript";};
	void changeState ( int );
};
class PauseScript : public Script {
public:
	PauseScript() {name = "pausescript";};
	void changeState ( int );
};


class HTMLAnim : public IntrPuzzProp {
public:
	HTMLAnim(int num, int hypX, int hypY, int skip);
	void cue(void);
};

class HTMLBack : public IntrPuzzView {
protected:
	const char *name;
public:
	HTMLBack(int num);
};

class HTMLButt : public IntrPuzzView {
protected:
	const char *name;
public:
	HTMLButt(int num, int hypX, int hypY, int skip);
};

class HTMLPage : public IntrPuzzPage {
public:
	HTMLBack *back;
	Sound *press;
	HTMLPage();
	HTMLPage(char *pagename);
	HTMLPage(int pagenumber);
	void dispose(void);
	void init(char *pagename);
	int handleEvent(MADEEventStamp *);
};

class HTMLFile : public File
{
public:
	int dormant;
	int theX;
	int theY;
	int theColumn;
	int theRow;
	int bytesRead;
	int newRow;
	int currentAction;
	int rowHeight;
	unsigned char pagecopy[4096];

	//variables for text typing
	int currentLetter;
	int typeX;
	int typeY;
	int typeWidth;
	bool typing;
	unsigned char typeText[MAX_LENGTH];
	List *typeList;

	HTMLFile();
	void readByteVols(unsigned char *theChar);
	void dispose();
	void readToken(void);
	void typeToken();
	void translateToken(unsigned char *);
	void displayToken(unsigned char *);
	void newFile(void);
	void cue();
	FeatureList *hyperList;
	List *myPrints;
};

class HyperCursor : public MadeCursor {
public:
	HyperCursor();
};

class HyperFeature : public Feature {
public:
	int myAction;
	char linkTo[128];
	Print *myPrint;
	int setFlag;
	HyperFeature(unsigned char *);
	void dispose(void);
	void respond(void);
	int doVerb(int);
};

class Secretary : public Object {
public:
	IntrPuzzTimer *TypeTimer;

	Secretary();
	void cue();
};

class Scroller : public Object {
public:
	Print *myPrint;
	char	text[1024];
	char	displayText[1024];
	int x;
	int y;
	int width;
	int speed;
	int offset;
	int size;
	Scroller(unsigned char *curToken);
	~Scroller();
	void init(void);
};

class HyperPrint : public Print {
public:
	HyperPrint(unsigned char *);
	int init(char *);
};

class TypePrint : public Print {
public:
	TypePrint(int theX, int theY, unsigned char*);
};
	
extern HTMLBack * htmlback;
extern HTMLFile* currentHFile;
extern HyperCursor* hyperCursor;

extern void changePause();

} // namespace Spycraft

#endif
