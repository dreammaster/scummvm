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

#ifndef SPYCRAFT_GAME_VLINK_H
#define SPYCRAFT_GAME_VLINK_H

#include "spycraft/game/web.h"
#include "spycraft/game/movie.h"
#include "spycraft/dmade/mcimovie.h"
#include "spycraft/game/email.h"
#include "spycraft/game/sound.h"

namespace Spycraft {

#define	ASAI		406 
#define	FOSTER		407
#define	HOLT		408
#define	LANGE		409
#define	MILK		410
#define	SEATON		411
#define	WARHURST	412

#define VLINK		1
#define ELINK		2
#define UP			1
#define DOWN		-1

#define VLINKMAIL	0
#define EMAIL		1
#define VOICEMAIL	2

#define ATTACH		1


class MessageSound : public Sound {
public:
	void play(int number);
};

/******************************
 *
 * 		Base class for all mail objects
 *
 ******************************/
class ComData : public Object {
public:
	short comType;
	unsigned long time0;
	unsigned short myDelay;
	unsigned short myFlag;
	bool ready;
	char msgName[128];
	Object *caller;

	ComData();
	int isReady();
	void dispose();
};

/******************************
 *
 * 		Email item
 *
 ******************************/
class EMail : public ComData {
public:
	unsigned long pageNumber;

	EMail(const char *Title, int pagenum, int archive);
	EMail(const char *Title, int pagenum);
	EMail(const char *Title, int pagenum, int flag, int delay);

};

class EMailAttachment : public ComData {
public:
	unsigned long pageNumber;

	EMailAttachment(const char *Title, int pagenum);
	EMailAttachment(const char *Title, int pagenum, int flag, int delay);
};

/******************************
 *
 * 		Voice mail item
 *
 ******************************/

class VoiceMail : public ComData {
public:
	unsigned long soundNumber;
	int idNumber;

	VoiceMail(const char *Title, int soundnum, int idNumber, int archive);
	VoiceMail(const char *Title, int soundnum, int idNumber);
	VoiceMail(const char *Title, int soundnum, int idNumber, int flag, int delay);
};

/******************************
 *
 * 		Video Link item
 *
 ******************************/
class VLink : public ComData {
public:
	char movieName[100];
	EMailAttachment *emailAttachment;

	VLink(const char *moviename, EMailAttachment *attachment);
	VLink(const char *moviename);
	VLink(const char *moviename, int flag, Object *whoToCue);
	VLink(const char *moviename, int flag, int delay);

	VLink(const char *moviename, int flag, int delay, EMailAttachment *attachment);
	VLink(const char *moviename, EMailAttachment *attachment, int archive);

	//delete above when everyone has there movienames in
	VLink(const char *title, const char *moviename, EMailAttachment *attachment);
	VLink(const char *title, const char *moviename);
	VLink(const char *title, const char *moviename, int flag, Object *whoToCue);
	VLink(const char *title, const char *moviename, int flag, int delay);

	VLink(const char *title, const char *moviename, int flag, int delay, EMailAttachment *attachment);
	VLink(const char *title, const char *moviename, EMailAttachment *attachment, int archive);
};

/******************************
 *
 * 		Global ComLinks List
 *
 ******************************/
class ArchiveList : public List {
public:
	int isOnList(Object *obj);
};

class ComLinks : public List {
public:
	int isEmpty() override;
	int removeVLink(const char *moviename);
	int removeVoiceMail(unsigned long idNumber);
	int removeEMail(unsigned long idNumber);
	int isOnList(Object *obj);
};
extern ComLinks *comlinks;
extern ArchiveList *archiveList;

/******************************
 *
 * 		Com Link Props
 *
 ******************************/
class ComWindow : public IntrPuzzView {
public:
	IntrPuzzTimer *moveTimer;
	int endCel;
	int increment;
	int currentWindowOffset;
	Object *caller;

	ComWindow();
	void cue(void);
	void open(int theLoop, Object *whoToCue);
	void close(Object *whoToCue);
};

class WireDot : public IntrPuzzView {
public:
	IntrPuzzTimer *moveTimer;
	int totalFrames;
	int increment;
	bool kill;

	WireDot();
	void cue();
};

class ComBack : public IntrPuzzView {
public:
	ComBack();
};

class WindowTitle : public Print {
public:
	WindowTitle(int theX, int theY, char *name);
};

class AttachBlink : public IntrPuzzProp {
public:
	AttachBlink();
	void cue(void);
};

class VideoBlink : public IntrPuzzProp {
public:
	VideoBlink();
	void cue(void);
};

class VidWindow : public IntrPuzzView {
public:
	WindowTitle *title;
	AttachBlink *attachblink;
	VideoBlink *videoblink;

	VidWindow();
	VidWindow(char *name);
	void dispose();
};

class Icon : public IntrPuzzView {
public:
	Icon(int theX, int theY, int theCel);
	~Icon();
	void dispose();
};

class SelectBar : public IntrPuzzView {
public:
	SelectBar();
};

class MailButt : public Button {
public:
	MailButt();
	virtual void activate();
};

class OpManager : public Button {
public:
	OpManager();
	virtual void activate();
};

class MailActivateScript : public Script {
public:
	MailActivateScript() {
		name = "mailscript";
	};
	void changeState(int);
};

class CloseButt : public Button {
public:
	CloseButt();
	virtual void activate();
};

class VideoCloseButt : public Button {
public:
	Object *caller;

	VideoCloseButt(Object *whoToCue);
	virtual void activate();
};

class CloseActivateScript : public Script {
public:
	bool closingMail;

	CloseActivateScript() {
		name = "closescript";
	};
	void changeState(int);
};

class ArchiveButt : public Button {
public:
	ArchiveButt();
	virtual void activate();
};

class ArchiveActivateScript : public Script {
public:
	ArchiveActivateScript() {
		name = "archivescript";
	};
	void changeState(int);
};

class PrevPage : public Button {
public:
	CommandLine *prevCL;

	PrevPage();
	virtual void activate();
};

class NextPage : public Button {
public:
	CommandLine *nextCL;

	NextPage();
	virtual void activate();
};

class ComLinkInfo : public Object {
public:
	ComLinkInfo();

	void load();
	void save();
	void dispose();

	int emailNum;
};
extern ComLinkInfo *comlinkinfo;

class Vplay : public IntrPuzzPage {
protected:
	const char *name;
public:
	bool menuHidden;
	int textHeight;
	int numPageItems;
	int numPages;
	int currentPage;
	Sound *windowSFX;
	Sound *voiceSound;

	NextPage *nextpage;
	PrevPage *prevpage;
	MailButt *mailbutt;
	ArchiveButt *archivebutt;
	WireDot *wiredot;
	CloseButt *closebutt;

	Vplay();

	void dispose();
	int handleEvent(MADEEventStamp *);
	void init(void);
	void saveState();
};

/******************************
 *
 * 		Mail Items
 *
 ******************************/
class MailItem : public IntrPuzzTextFeature {
public:
	bool printHidden;
	bool onIt;
	Icon *icon;

	~MailItem();
	void init(int theX, int theY, char *name);
};

class Vitem : public MailItem {
public:
	VLink *vlink;

	int handleEvent(MADEEventStamp *);
	void cue(void);
	void activate(void);
};

class VitemActivateScript : public Script {
public:
	Vitem *vitem;

	VitemActivateScript(Vitem *videoItem);
	void changeState(int);
};

class Voiceitem : public MailItem {
public:
	VoiceMail *voicemail;

	int handleEvent(MADEEventStamp *);
	void cue(void);
	void activate(void);
};

class IDCard : public IntrPuzzView {
public:
	IDCard(int IdNumber);
};

class VoiceitemActivateScript : public Script {
public:
	Voiceitem *voiceitem;
	IDCard *idcard;

	VoiceitemActivateScript(Voiceitem *voiceitem);
	void changeState(int);
};

class Eitem : public MailItem {
public:
	EMail *email;

	int handleEvent(MADEEventStamp *);
	void activate(void);
};

class EitemActivateScript : public Script {
public:
	Eitem *eitem;
	int pageNum;

	EitemActivateScript(Eitem *mailItem);
	void changeState(int);
};

class ComItemList : public List {
	unsigned char reserved;
public:
	ComItemList();
	virtual void dispose();
	virtual void dispose(MailItem *thisItem);
};

class maxDItem : public vlinkDItem {
public:
	void doit();
};

class ScriptMax : public Script {
public:
	ScriptMax() {
		name = "maxscript";
	};
	void changeState(int);
};

class ScriptHolt : public Script {
public:
	ScriptHolt() {
		name = "Holtscript";
	};
	void changeState(int);
};

extern void playLink(Script *cueme);

} // namespace Spycraft

#endif
