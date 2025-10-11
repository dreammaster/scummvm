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

#ifndef SPYCRAFT_GAME_EMAIL_H
#define SPYCRAFT_GAME_EMAIL_H

#include "spycraft/game/html.h"
#include "spycraft/game/vlink.h"
#include "spycraft/game/dlgbox.h"

namespace Spycraft {

extern List *emails;
extern int choiceIndex[5];
extern bool mailFlags[5][80];

class EMailPage : public HTMLPage {
public:
	EMailPage(int pagenumber);
	EMailPage(int pagenumber, bool attach);
	void init(int pagename, int theLeft, int theTop);
};


class Border : public DialogView {
public:
	Border(int x, int y, int scalex, int scaley, int cel);
};

class ChoiceBar : public DialogView {
public:
	float ratioX;
	float ratioY;

	ChoiceBar(int width);
};

class ChoiceBox : public DialogView {
public:
	ChoiceBox(int x, int y, int scalex, int scaley);
};

class EMailSend : public DlgBox {
public:
	Object *caller;
	int currentChoiceGroup;
	int currentUserGroup;
	bool decision;
	ChoiceBox *choiceview;
	Border *hBordTop;
	Border *hBordBottom;
	Border *vBordLeft;
	Border *vBordRight;
	ChoiceBar *choicebar;
	Sound *sendSFX;
	int currentPageName;   //used for sound work around

	bool scratchFlags[5][80];

	EMailSend(int pagenumber, Object *whoToCue);
	EMailSend(int pagenumber, bool flags[5][80], Object *whoToCue);
	void cue();
	void init(int  pagename);
	int handleEvent(MADEEventStamp *event);
	void showChoices(char choices[40][80], int numChoices, bool doCondition);
	void dispose();
};

class MailClose : public DialogButton {
public:
	MailClose();
	virtual void activate();
};

class ChoiceFeature : public TextFeature {
public:
	int id;
	bool hilit;

	void init(int x, int y, char *name, int idnum);
	int handleEvent(MADEEventStamp *event);
	~ChoiceFeature();
};

class EMailClose : public DialogButton {
public:
	EMailClose();
	int handleEvent(MADEEventStamp *event);
	virtual void activate();
	void cue();
};

class MailOK : public DialogButton {
public:
	MailOK();
	int handleEvent(MADEEventStamp *event);
	void activate();
	void cue();
};

class MailCancel : public DialogButton {
public:
	MailCancel();
	int handleEvent(MADEEventStamp *event);
	void activate();
	void cue();
};

extern EMailSend *emailsend;

} // namespace Spycraft

#endif
