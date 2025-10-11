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

#ifndef SPYCRAFT_GAME_WEB_H
#define SPYCRAFT_GAME_WEB_H

#include "spycraft/game/made.h"
#include "spycraft/game/game.h"
#include "spycraft/game/feature.h"
#include "spycraft/game/script.h"
#include "spycraft/game/actor.h"
#include "spycraft/game/sound.h"
#include "spycraft/game/colby.h"
#include "spycraft/game/inset.h"
#include "spycraft/game/interface_puzzle.h"
#include "spycraft/game/html.h"
#include "spycraft/game/dlgbox.h"

namespace Spycraft {

class WebExit;
class WebBack;
class WebFwd;
class WebHome;
class WebHelp;
class Link;
class Back;

class ReportBox : public DlgBox {
public:
	ReportBox();
};

class ReportOk : public DialogButton {
public:
	ReportOk();
	virtual void activate();
};

class ReportCancel : public DialogButton {
public:
	ReportCancel();
	virtual void activate();
};

class WebReport : public WebButton {
public:
	WebReport();
	void activate(void);
	void cue(void);
	void dispose(void);
};

class MoleSendButt : public WebReport {
public:
	MoleSendButt();
	void activate();
	void cue();
};

class InfosciSendButt : public WebReport {
public:
	InfosciSendButt();
	void activate();
	void cue();
};

class NewsPage : public HTMLPage {
public:
	NewsPage(int pagenumber);
	void init(int pagenumber);
};

class Web : public IntrPuzz {
public:
	Web();
	~Web();
	WebBack *backButton;
	WebFwd *fwdButton;
	void init(int pic_num);
};

class Yellow : public IntrPuzz {
public:

	Yellow(int page);
	~Yellow();
	void init(int pic_num, int page);
};

/*******************************
* Web Buttons
*******************************/

class WebBack : public WebButton {
public:
	WebBack();
	void activate(void);
	int handleEvent(MADEEventStamp *);
};
class WebFwd : public WebButton {
public:
	WebFwd();
	void activate(void);
	int handleEvent(MADEEventStamp *);
};

class WebExit : public WebButton {
public:
	WebExit();
	int needToReport();
	void activate(void);
};
class WebHelp : public WebButton
{
public:
	WebHelp();
	void activate(void);
};

class LinkFlash : public IntrPuzzView {
public:
	bool killTimer;

	LinkFlash();
	void dispose();
};

class LinkFlashScript : public Script {
public:
	LinkFlashScript() {
		name = "flashscript";
	};
	void changeState(int);
};

class ComLinkShort : public WebButton {
public:
	LinkFlash *linkflash;

	ComLinkShort();
	void activate(void);
	void dispose();
};
extern ComLinkShort *comlinkshort;

/*******************************
* home page
*******************************/

class ComLink : public Button {
protected:
	char *name;
public:
	ComLink();
	void activate(void);
	void cue(void);
};

class DataLink : public Button {
protected:
	char *name;
public:
	DataLink();
	void activate(void);
	void cue(void);
};

class NoteLink : public Button {
protected:
	char *name;
public:
	NoteLink();
	void activate(void);
	void cue(void);
};

class TravelLink : public Button {
protected:
	char *name;
public:
	TravelLink();
	void activate(void);
	void cue(void);
};

class Home : public IntrPuzzPage {
public:
	NewsPage *news;
	Home();
	void init(void);
	void dispose(void);
	int handleEvent(MADEEventStamp *);
};


class Back : public IntrPuzzView {
protected:
	char *name;
public:
	Back(int num);
};

class DataPage : public IntrPuzzPage {
public:
	Sound *entrance;
	DataPage();
	~DataPage();
	void init(void);
};

class ATFButt : public Button {
protected:
	char *name;
public:
	ATFButt();
	void activate(void);
};

class CIAButt : public Button {
protected:
	char *name;
public:
	CIAButt();
	void activate(void);
};

class DEAButt : public Button {
protected:
	char *name;
public:
	DEAButt();
	void activate(void);
};

class FBIButt : public Button {
protected:
	char *name;
public:
	FBIButt();
	void activate(void);
};

class NROButt : public Button {
protected:
	char *name;
public:
	NROButt();
	void activate(void);
};

class NSAButt : public Button {
protected:
	char *name;
public:
	NSAButt();
	void activate(void);
};

class USSSButt : public Button {
protected:
	char *name;
public:
	USSSButt();
	void activate(void);
};

class TravelPage : public IntrPuzzPage {
public:
	TravelPage();
	void init(void);
};

class NotePage : public IntrPuzzPage {
public:
	NotePage();
	void init(void);
};

class ButtAnim : public IntrPuzzProp {
protected:
public:
	Button *cueme;
	ButtAnim(int pic_id, int loop_id, int thex, int they, Button *from);
};

class AnimScript : public Script {
public:
	Button *from;
	AnimScript(Button *frombutt) {
		name = "animscript"; from = frombutt;
	};
	AnimScript() {
		name = "animscript"; from = NULL;
	};
	void changeState(int);
};

class HomoScript : public Script {
public:
	Sound *entrance;
	HomoScript() {
		name = "homoscript";
	};
	~HomoScript();
	void changeState(int);
};
class AlertScript : public Script {
public:
	AlertScript() {
		name = "alertscript";
	};
	void changeState(int);
};

class GlobosPause : public Script {
public:
	GlobosPause() {
		name = "globospause";
	};
	void changeState(int);
};

class Alert : public IntrPuzzProp {
protected:
public:
	Alert(int theX, int theY, int pic_id, int loop_id, int cel_id);
	int handleEvent(MADEEventStamp *);
};

class QuitAlert : public WebView {
protected:
public:
	QuitAlert(int theX, int theY, int pic_id, int loop_id, int cel_id);
	int handleEvent(MADEEventStamp *);
};

class QuitAlertScript : public Script {
public:
	QuitAlertScript() {
		name = "Quitalertscript";
	};
	void changeState(int);
};

class GlobosAnim : public IntrPuzzProp {
protected:
public:
	GlobosAnim();
	void cue(void);
};

class WireAnim : public IntrPuzzProp {
protected:
public:
	WireAnim();
	void cue(void);
};

extern WebReport *webreport;

} // namespace Spycraft

#endif
