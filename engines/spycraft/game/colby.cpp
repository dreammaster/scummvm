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

#include "spycraft/game/colby.h"
#include "spycraft/game/made.h"
#include "spycraft/game/user.h"
#include "spycraft/game/globals.h"
#include "spycraft/game/dialog.h"
#include "spycraft/game/inset.h"
#include "spycraft/game/pda.h"
#include "spycraft/game/interface.h"
#include "spycraft/game/movie.h"
#include "spycraft/game/interface_puzzle.h"
#include "spycraft/dmade/gamebox.h"
#include "spycraft/game/html.h"
#include "spycraft/logic/60000.h"
#include "spycraft/logic/94000.h"
#include "spycraft/game/vlink.h"
#include "spycraft/game/email.h"
#include "spycraft/game/gun.h"
#include "spycraft/game/save.h"
#include "spycraft/game/invinset.h"
#include "spycraft/game/invent.h"
#include "spycraft/afxwin.h"

#ifndef DOS4GW
#include "spycraft/game/actsock.h"
#endif
//DEBUG
#include "spycraft/game/debugmod.h"

namespace Spycraft {

#ifndef DOS4GW
extern HWND hGameWnd;
#endif
#ifdef DOS4GW
MADEEventStamp *GetDOSMessage(void);
#endif

extern DebugModule *theDebug;
//DEBUG
extern Dialog *theDialog;
extern PDA *thePDA;
//extern GameMenu* theMenu;
extern DialTree *dialogTree;
extern Inset *theInset;
extern Intrface *intrface;
extern SaveGame *saveGame;
int canSaveColby = true;
extern File *bufFile;
extern SaveGame *saveGame;
extern HyperCursor *hyperCursor;
extern Sound *instructions;
//extern List * vlinks;

extern Sound *briefSound;

MADEEnvirons *colbyInfo;

int currentFocus;		//where we want events to go

int quitting, tryingToQuit;

Colby colby;

BlinkCursor *normalCursor;
NorthCursor *doorCursor = new NorthCursor;
EastCursor *eastCursor = new(EastCursor);
WestCursor *westCursor = new(WestCursor);
NorthCursor *northCursor = new(NorthCursor);
SouthCursor *southCursor = new(SouthCursor);
NortheastCursor *northeastCursor = new NortheastCursor;
NorthwestCursor *northwestCursor = new NorthwestCursor;
SoutheastCursor *southeastCursor = new SoutheastCursor;
SouthwestCursor *southwestCursor = new SouthwestCursor;
GunCursor *gunCursor = new(GunCursor);
InvCursor *invCursor = new(InvCursor);
ArrowCursor *arrowCursor = new(ArrowCursor);
WaitCursor *waitCursor = new(WaitCursor);
ComLinks *comlinks = new ComLinks;
ArchiveList *archiveList = new ArchiveList;
Sound *sound1 = new Sound;	// general purpose sounds or sampled music.  sound1 continues across rooms.
Sound *sound2 = new Sound;	// general purpose sounds or sampled music.  sound2 continues across rooms.
Music *theMusic = new Music;  // our global MIDI Music;
Timer *hurryTimer = NULL;
//Door*		theDoor = new Door;
//Movie* theMovie;

extern short restoring;
extern InvInset *theInvInset;

Print *testPrint; //DEBUG

char szHDRoot[MAX_PATH];
char theNetPath[MAX_PATH];
char spycraftPath[MAX_PATH];

void flushMessageCue(void) {
#ifdef DOS4GW
	while (GetDOSMessage());
#else
	MSG msg;
	while (PeekMessage(&msg, NULL, WM_KEYFIRST, WM_KEYLAST, PM_REMOVE));

	while (PeekMessage(&msg, NULL, WM_MOUSEFIRST, WM_MOUSELAST, PM_REMOVE));
#endif
}

int setSpycraftPath(void) {
#if 0
	HKEY PathKey;
	unsigned long HDRootLength = MAX_PATH;
	DWORD regtype = REG_SZ;
	char tempPath[MAX_PATH];

	LONG ret = RegOpenKey(HKEY_LOCAL_MACHINE,
		"Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\spycraft.exe",
		&PathKey);
	if (ret != ERROR_SUCCESS) return false;


	ret = RegQueryValueEx(PathKey, NULL, NULL, &regtype,
		(BYTE *)tempPath, &HDRootLength);

	RegCloseKey(PathKey);
	if (ret != ERROR_SUCCESS) return false;

	int numChars = strlen(tempPath);
	strncpy(spycraftPath, tempPath, numChars - 12);
#endif
	return true;
}

int getNetPath(void) {
#if 0
	HKEY PathKey;
	unsigned long HDRootLength = MAX_PATH;
	DWORD regtype = REG_SZ;
	// Get HDROOT from the App Paths key that the setup program created.
	LONG foo = RegOpenKey(HKEY_LOCAL_MACHINE,
		"Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Netscape.exe",
		&PathKey);
	if (foo != ERROR_SUCCESS) return false;
	foo = RegQueryValueEx(PathKey, NULL, NULL, &regtype,
		(BYTE *)szHDRoot, &HDRootLength);
	// After the line above, if the function succeeded, szHDRoot will
	// have the path to netscape.exe  

	RegCloseKey(PathKey);
	if (foo != ERROR_SUCCESS) return false;

	int i = 0;
	int j = -1;

	int numChars;
	numChars = strlen(szHDRoot);
	for (i = 0; i < numChars + 1; i++) {
		j++;
		theNetPath[j] = szHDRoot[i];
	}
#endif

	return true;
}

Colby::Colby() {
	name = "Colby";
	cursor = NULL;
	theMovie = new Movie;
	theInvInset = new InvInset;
	//	theSilentMovie = new SilentMovie;
}

void Colby::init() {
	if (sfxGetClientMem() < 9000000)
	{
#ifdef DOS4GW
		GameFlag.set(fGreg39);//we are under 8 megs	in dos
#endif
	}
	setSpycraftPath();

	user->canInput(false);
	normalCursor = arrowCursor;
	intrface = new Intrface();
	theGame->prevRoomName = "None!";
	sfxLoadRes(vINTERFACE, RES_ATS);
	sfxLoadRes(909, RES_ATS);
	GameFlag.set(fWashingtonAvailable);
	colbyInfo = sfxEnvirons(MADE_VERS);
	checkINI();
#ifndef DOS4GW

	if (GameFlag.test(fOnlineAvailable)) {
#if 0
		char currentDir[MAX_PATH];
		unsigned long currentDirLength = MAX_PATH;
		GetCurrentDirectory(currentDirLength, currentDir);
		Common::strcat_s(currentDir, "\\resource\\");
		ACTSOCK_SetParentHWND(hGameWnd);
		ACTSOCK_EnableErrorDialogs(true);
		ACTSOCK_FTPSetTimeouts(75000, 75000);
		ACTSOCK_StartService();
		ACTSOCK_SetUserInfo("spycraft", "shadow1", "");
		ACTSOCK_FTPDailyDownload("spycraft.activision.com", "\\home\\spycraft\\", currentDir, true, true, true);
		ACTSOCK_EndService();
#else
		warning("fOnlineAvailable game flag unavailable");
#endif
	}
#endif
	this->newRoom(new Rm60500);

}

Colby::~Colby() {
	//KillMono();
	sfxUnlockRes(904, RES_ATS);
	sfxUnlockRes(960, RES_ATS);
	sfxUnlockRes(40002, RES_WAVE);
	sfxUnlockRes(103, RES_WAVE);
	sfxUnlockRes(136, RES_WAVE);
	delete briefSound;
	delete arrowCursor;
	delete eastCursor;
	delete westCursor;
	delete northCursor;
	delete doorCursor;
	delete southCursor;
	delete northeastCursor;
	delete northwestCursor;
	delete southeastCursor;
	delete southwestCursor;
	delete gunCursor;
	delete invCursor;
	delete theDebug;
	delete theMovie;
	//	delete theSilentMovie;
	delete theInvInset;
	delete waitCursor;
	if (!sound1->isPlaying())
		delete sound1;
	if (!sound2->isPlaying())
		delete sound2;
	if (!theMusic->isPlaying())
		delete theMusic;
	delete inventry;
	delete comlinks;
	delete archiveList;
	delete instructions;
	delete bufFile;
	delete hyperCursor;
	delete saveGame;
	//	delete theDoor;

}

int Colby::handleEvent(MADEEventStamp *event) {

	if (theDebug->handleEvent(event))
		return true;

	permList->handleEvent(event);
	if (theGun)
		theGun->handleEvent(event);

	if (currentFocus == DIALOG_FOCUS) {
		// if (theDialog)
		 //	theDialog->handleEvent(event);
		 //else if (dialogTree)
		dialogTree->handleEvent(event);
		return true;
	} else if (currentFocus == PDA_FOCUS) {
		thePDA->handleEvent(event);
		return true;
	}
	/*else if (currentFocus == MENU_FOCUS)  {
		theMenu->handleEvent(event);
		return true;
	}*/
	else if (currentFocus == PUZZLE_FOCUS)/* && (~event_type & USER_RIGHT_UP))*/ {
		intrpuzz->handleEvent(event);
		return true;
	} else if (currentFocus == INSET_FOCUS) {
		if (!intrface->handleEvent(event))
			theInset->handleEvent(event);
		return true;
	} else if (currentFocus == INV_INSET_FOCUS) {
		theInvInset->handleEvent(event);
		return true;
	}
	/*else if (currentFocus == ALL_EVENT_FOCUS)  {
		if (curRoom->handleEvent ( event ))
		return true;
	}*/


	if ((event_type & USER_RIGHT_UP) || (event_type & USER_KEY_DOWN && event->message == 27)) {

		//quitting = true;
		//theMovie->caller = 0;
		//theMovie->stop();
		if ((intrface->enabled /*|| currentFocus == PUZZLE_FOCUS*/) && (strcmp(curRoom->name, "Rm1270")) && canSaveColby && user->input && (!currentFocus /*|| currentFocus == PUZZLE_FOCUS*/)) {
			char *tmpSave;
			int theAction = sfxGameBox();


			switch (theAction) {
			case IDSAVE:
				tmpSave = sfxGetSaveFile();
				if (strcmp(tmpSave, ""))
					saveGame->save(tmpSave);
				//sfxCopyFile("bookMark.sgm", tmpSave);
				//#ifndef DOS4GW
				//SetCurrentDirectory( spycraftPath );
				//#endif
				break;
			case IDRESTORE:
				tmpSave = sfxGetRestoreFile();



				if (strcmp(tmpSave, "")) {
					restoring = true;
					//if (currentFocus == PUZZLE_FOCUS)	{
						//comlinkshort = NULL
					//	intrpuzz->quitIntrface();
				//	} 

					theGame->newRoom(new RestRoom(tmpSave));
				}

				//#ifndef DOS4GW
				//	SetCurrentDirectory( spycraftPath );
				//#endif

				break;
			case IDQUIT:
				tryingToQuit = true;
				if (currentFocus == PUZZLE_FOCUS)
					intrpuzz->quitIntrface();

				theMovie->caller = 0;
				theMovie->stop();
				theMusic->stop();

				if (sounds->size) {

					Node *n = sounds->head;
					SoundObject *obj;
					while (n) {
						obj = (SoundObject *)n->data;
						n = n->next;
						if (obj->type == RES_WAVE) {
							obj->caller = NULL;
							obj->stop();
						}
					}
				}
				theMusic->stop();
				theGame->newRoom(new Rm94000);

				break;
			}
		}
		//sfxQuit();
		return true;
	}
	return false;
}

void Colby::resetHotspot() {
	if (!quitting) {
		Game::resetHotspot();
		if (!strcmp(curRoom->name, "Rm91000") || !strcmp(curRoom->name, "Rm1270")) {
			sfxSystemCursor(SYS_WAIT);
			return;
		}
		if (cursor == normalCursor)
			if ((!strcmp(curRoom->name, "Rm1000")) || (!strcmp(curRoom->name, "Rm1270")))
				sfxSystemCursor(SYS_WAIT);
			else
				normalCursor->rest();		//set to lowLight state
		else
			setCursor(normalCursor);
	}
}

void Colby::handsOff()					//take control from user
{
	Game::handsOff();
	/*	if (cursor == normalCursor)
			normalCursor->rest();
		setCursor ( waitCursor );*/

	sfxSystemCursor(SYS_WAIT);
}

void Colby::handsOn()					//give control to user
{
	Game::handsOn();
	setCursor(normalCursor);
	if (normalCursor->view == 906)
		sfxCursorPriority(99);
}

void Colby::newRoom(Room *theRoom) {
	theGame->handsOff();

	//NEW MADE resetHotspot();
	Game::newRoom(theRoom);
}

void Colby::checkINI(void) {
	File *tmpINIFile;
	unsigned char theValue;
	tmpINIFile = new File;

	if (tmpINIFile->open("spycraft.ini", FILE_READ)) {
		tmpINIFile->readByte(&theValue);
		if (theValue == '1') //'1'
			GameFlag.set(fLockOut);
		tmpINIFile->readByte(&theValue);
		while (theValue == '\r' || theValue == '\n')
			tmpINIFile->readByte(&theValue);
		if (theValue == '1') //'1'
			GameFlag.set(fOnlineAvailable);
		while (theValue == '\r' || theValue == '\n')
			tmpINIFile->readByte(&theValue);

		tmpINIFile->close();
	}
	delete tmpINIFile;
#ifndef DOS4GW
	char theNumberStr[10];
	Common::strcat_s(spycraftPath, "spysound");
	GetPrivateProfileString("soundOption", "option", "0", theNumberStr, 10, spycraftPath);
	if (atoi(theNumberStr))
		colbyInfo->slow_system = 1;
	else
		colbyInfo->slow_system = 0;

#endif
}

/*********************
*
*		Cursors
*
**********************/
EastCursor::EastCursor() {
	name = "eastCursor";
	view = 970;
	loop = 3;
	cel = 9;
	orig_pt.x = 0;
	orig_pt.y = 10;
	verb = DO_V;

}

WestCursor::WestCursor() {
	name = "westCursor";
	view = 970;
	loop = 2;
	cel = 9;
	orig_pt.x = 0;
	orig_pt.y = 10;
	verb = DO_V;
}

NorthCursor::NorthCursor() {
	name = "northCursor";
	view = 970;
	loop = 4;
	cel = 9;
	orig_pt.x = 0;
	orig_pt.y = 10;
	verb = DO_V;
}

SouthCursor::SouthCursor() {
	name = "SouthCursor";
	view = 970;
	loop = 1;
	cel = 9;
	orig_pt.x = 0;
	orig_pt.y = 10;
	verb = DO_V;

}

NortheastCursor::NortheastCursor() {
	name = "NortheastCursor";
	view = vCURSORS;
	loop = 6;
	cel = 0;
	orig_pt.x = 0;
	orig_pt.y = 10;
	verb = DO_V;
}

NorthwestCursor::NorthwestCursor() {
	name = "NorthwestCursor";
	view = vCURSORS;
	loop = 7;
	cel = 0;
	orig_pt.x = 0;
	orig_pt.y = 10;
	verb = DO_V;
}

SoutheastCursor::SoutheastCursor() {
	name = "SoutheastCursor";
	view = vCURSORS;
	loop = 8;
	cel = 0;
	orig_pt.x = 0;
	orig_pt.y = 10;
	verb = DO_V;
}

SouthwestCursor::SouthwestCursor() {
	name = "SouthwestCursor";
	view = vCURSORS;
	loop = 9;
	cel = 0;
	orig_pt.x = 0;
	orig_pt.y = 10;
	verb = DO_V;
}

ArrowCursor::ArrowCursor() {
	name = "arrowCursor";
	view = 970;
	loop = 0;
	cel = 0;
	orig_pt.x = 10;
	orig_pt.y = 10;
	verb = DO_V;
}

InvCursor::InvCursor() {
	name = "invCursor";
	view = 15;
	orig_pt.x = 0;
	orig_pt.y = 0;
	verb = CAMERA_V;
}

WaitCursor::WaitCursor() {
	name = "waitCurosr";
	view = 943;
	loop = 0;
	cel = 0;
	orig_pt.x = 15;
	orig_pt.y = 15;
	verb = DO_V;
}

GunCursor::GunCursor() {
	name = "gunCurosr";
	view = 906;
	loop = 0;
	cel = 0;
	orig_pt.x = 25;
	orig_pt.y = 25;
	verb = GUN_V;
}

} // namespace Spycraft
