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

#include "spycraft/afxwin.h"
#include "spycraft/game/game.h"
#include "spycraft/game/globals.h"
#include "spycraft/game/made.h"
#include "spycraft/game/macro.h"
#include "spycraft/game/user.h"
#include "spycraft/game/feature.h"
#include "spycraft/game/actor.h"
#include "spycraft/game/timer.h"
#include "spycraft/game/views.h"
#include "spycraft/game/globals.h"
#include "spycraft/game/interface_puzzle.h"
#include "spycraft/game/sound.h"
#include "spycraft/game/interface.h"
#include "spycraft/game/pda.h"
#include "spycraft/game/vlink.h"
#include "spycraft/dmade/adveff.h"
#include "spycraft/game/save.h"
#include "spycraft/dmade/advsound.h"
#include "spycraft/dmade/advmusic.h"
#include "spycraft/dmade/mcimovie.h"
#include "spycraft/game/actsock.h"
#include "spycraft/vars.h"

namespace Spycraft {

extern char theNetPath[MAX_PATH];

extern SaveGame *saveGame;
extern int currentFocus;
extern Intrface *intrface;
extern PDA *thePDA;
extern Briefcase *theCase;
extern int tryingToQuit;
extern HWND hGameWnd;

extern SoundList *sounds;
extern short restoring;
List *permList;
Game *theGame = NULL;
Room *curRoom = NULL;
void (*gameHandler) (void);

#define BLINK_YAKE     !sfxIsMCIOpen()

Print *roomPrint;

Game::Game() {
	name = "Game";
	theGame = this;
	gameHandler = NULL;
	features = new FeatureList;
	hotspots = new HotspotList;
	cast = new Cast;
	sounds = new SoundList;
	timers = new TimerList;
	permList = new List;
	curVolume = 127;
	cursor = NULL;
	//srand((unsigned)time(NULL));
	display_buffer = -1;
	roomPrint = new Print;

}

Game::~Game() {
	theGame = NULL;

	if (curRoom) {
		delete curRoom;
		curRoom = NULL;
	}

	if (cast) {
		delete cast;
		cast = NULL;
	}

	if (features) {
		delete features;
		features = NULL;
	}

	if (intrface) {
		delete intrface;
		intrface = NULL;
	}

	if (sounds) {
		delete sounds;
		sounds = NULL;
	}

	if (hotspots) {
		delete hotspots;
		hotspots = NULL;
	}


	if (timers->size) {
		Timer *obj;
		Node *n = timers->head;
		while (n) {
			obj = (Timer *)n->data;
			n = n->next;
			delete obj;
		}
	}

	if (timers) {
		delete timers;
		timers = NULL;
	}







	if (permList) {
		delete permList;
		permList = NULL;
	}

	if (cursor)
		setCursor(NULL);

	delete roomPrint;

#ifdef DO_DEBUG
	sfxPrintf("objs left: %d", _object_allocated);
	//if ( _object_allocated ) {
	//	sfxPrintf ( "Warning: %d objects remains", _object_allocated );
	//}

#endif

}

void Game::doit() {
	if (gameHandler) {
		(*gameHandler)();
		event_type = NULL;
		return;
	}
	Object::doit();

	if (cursor)
		cursor->doit();

	//if ( sounds )
	//	sounds->doit();







	if (permList)
		permList->doit();

	if ((comlinks) && (!comlinks->isEmpty()) && ((!intrface->isOpen) || (!strcmp(curRoom->name, "Rm1270"))) && (currentFocus != PUZZLE_FOCUS) && (currentFocus != DIALOG_FOCUS))
		if ((intrface) && (user->input) && (!intrface->blinking) && (intrface->canBlink) && ((intrface->enabled) || (theGame->enableIntrDuringMovie)))
			if ((!strcmp(curRoom->name, "Rm1270")) || BLINK_YAKE)
				intrface->blink();

	if (!strcmp(curRoom->name, "Rm91000"))
		if ((user->input) && (!intrface->blinking) && (intrface->canBlink) && ((intrface->enabled) || (theGame->enableIntrDuringMovie)))
			intrface->blink();

	if (!currentFocus) {
		if (hotspots && user->input)
			hotspots->doit();

		if (timers)
			timers->doit();

		if (cast)
			cast->doit();

		if (curRoom)
			curRoom->doit();
	}

	if (intrface)
		intrface->doit();

	if (intrpuzz)
		intrpuzz->doit();

	if (deathList->size)
		deathList->dispose();

	if (cueList->size) {
		cueList->cue();
		cueList->releaseAll();
	}

}

void Game::suspend() {
	suspended = true;
}

void Game::handsOn() {
	user->canInput(true);
}

void Game::handsOff() {
	user->canInput(false);
}

void Game::setCursor(MadeCursor *theCursor) {
	cursor = theCursor;
	if (cursor) {
		user->message = cursor->verb;
		cursor->init();
	}
}

void Game::newRoom(Room *theRoom) {
	if (curRoom) {
		if (pdaProp) {
			intrface->blinking = false;
			intrface->canBlink = false;
			pdaProp->setCycle(0);
			intrface->release(pdaProp);
			pdaProp->setHotspot(NULL);
			delete pdaProp;
			pdaProp = NULL;

		}
		intrface->hide();

		if (!tryingToQuit)
			prevRoomName = curRoom->name;

		if (GameFlag.test(fRoomPrintActive))
			roomPrint->erase();//DEBUG
		delete curRoom;
	}

	//AIL_serve();

	curRoom = theRoom;
	hotObj = 0;
	if (GameFlag.test(fRoomPrintIndicate)) {
		GameFlag.set(fRoomPrintActive);
		roomPrint->x = 0;//DEBUG
		roomPrint->y = 0;//DEBUG
	}
	//if (!restoring && (strcmp(theGame->prevRoomName, "None!")) && (strcmp(curRoom->name, "RestRoom"))) 
	//	saveGame->save("bookMark.sgm");
	curRoom->init();
	restoring = false;
	intrface->canBlink = true;
	if (GameFlag.test(fRoomPrintActive))
		roomPrint->init(curRoom->name);  //DEBUG
}

void Game::masterVolume(uint16 theVolume) {
	if ((theVolume <= 127) && (theVolume >= 0)) {
		curVolume = theVolume;
		//theMusic->setVolume ( theVolume );
	 //	sounds->setVolume ( theVolume );
	}
}

void Game::resetHotspot() {
	hotObj = NULL;
}

Room::Room() {
	name = "Room";
	display_rect.top = BACKGROUND_TOP;
	display_rect.left = BACKGROUND_LEFT;
	display_rect.bottom = BACKGROUND_BOTTOM;
	display_rect.right = BACKGROUND_RIGHT;
	display_buffer = -1;
	show_style = VE_FLIP;
	steps = 1;
	stepTime = 1;
	number = -1;
	display_buffer = theGame->display_buffer;

}

Room::~Room() {
	if (!features->isEmpty())
		features->dispose();

	if (!cast->isEmpty())
		cast->dispose();

	//if ( !sounds->isEmpty() )
		//sounds->dispose(); 

	if (!hotspots->isEmpty())
		hotspots->releaseAll();

	//PICif ( display_buffer >= 0 ) {
	//PIC	sfxKillDisplayBuffer ( display_buffer );
	//PIC	display_buffer = -1;
	//PIC}
	number = -1;
}

void Room::init() {
	hotObj = NULL;
}

void Room::init(int draw_pic) {
	drawPic(draw_pic);
	hotObj = NULL;
}

void Room::zoomTo(int theX, int theY) {
	/*	static SRect zoomRect;

		SRect_Init(&zoomRect, 0, 16, 511, 399);
		sfxZoomEffectRect(&zoomRect);
		sfxZoomEffect (theX, theY );*/		//take out until aspect ratio polished
}

int Game::startNet(char *urlAdd) {
#ifndef DOS4GW
	switch (ACTSOCK_GetServiceType()) {
	case ACTSOCK_SERVICETYPE_WINSOCK:

		if (sounds->size) {
			if (sound1->isPlaying())
				sound1->stop();
			Node *n = sounds->head;
			SoundObject *obj;
			while (n) {
				obj = (SoundObject *)n->data;
				n = n->next;
				if (obj->type == RES_WAVE) {
					obj->stop();
					//sfxPrintf("Trying to play movie while WAVE playing");
					//return;
				}

			}
		}

		sfxDisableSound();
		theMusic->stop();
		sfxDisableMusic();

		ACTSOCK_SetParentHWND(hGameWnd);
		ACTSOCK_EnableErrorDialogs(true);
		ACTSOCK_FTPSetTimeouts(75000, 75000);
		ACTSOCK_StartService();
		ACTSOCK_SetUserInfo("spycraft", "shadow1", "");
		ACTSOCK_WWWRunBrowser(urlAdd, true);
		ACTSOCK_EndService();

		sfxEnableMusic();
		sfxEnableSound();

		break;
	}
#endif
	return true;

}

void Room::drawPic(int pic_num) {
	ADV_ASSERT((theGame->display_buffer == display_buffer), 0);
	if (deathList->size)
		deathList->dispose();

	if (theGame->display_buffer >= 0) {
		sfxKillDisplayBuffer(theGame->display_buffer);
		theGame->display_buffer = -1;
		display_buffer = -1;
	}


	display_buffer = sfxMakeDisplayBuffer(pic_num, &display_rect, 0, 0);
	theGame->display_buffer = display_buffer;
	if (display_buffer == -1) {
		sfxPrintf("Error: Can't draw background - exiting");
		sfxQuit();
	} else {
		sfxVisualEffect(show_style, display_buffer, steps, stepTime);
	}
}

void Room::horizontalScroll(int theX) {
	display_rect.left += theX;
	display_rect.right += theX;

	features->updateRects(theX, 0);
	cast->updateRects(theX, 0);

	/* scroll it */
	sfxSetDisplayRect(display_buffer, &display_rect);

	/* update the hotspot */
	hotObj = NULL;
}

void Room::verticalScroll(int theY) {
	display_rect.top += theY;
	display_rect.bottom += theY;

	features->updateRects(0, theY);
	cast->updateRects(0, theY);

	/* scroll it */
	sfxSetDisplayRect(display_buffer, &display_rect);

	/* update the hotspot */
	hotObj = NULL;
}

} // namespace Spycraft
