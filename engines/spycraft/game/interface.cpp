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

#include "spycraft/game/interface.h"
#include "spycraft/game/invent.h"
#include "spycraft/game/pda.h"
#include "spycraft/game/web.h"
#include "spycraft/game/invinset.h"
#include "spycraft/logic/1000.h"
#include "spycraft/logic/91000.h"
#include "spycraft/dmade/advsound.h"

#ifdef DIRECTX
#include "spycraft/dmade/advmovie.h"
#define PLAY_BRIEF     !sfxIsMovieOpen()
#else
#include "spycraft/dmade/mcimovie.h"
#define PLAY_BRIEF     !sfxIsMCIOpen()
#endif

namespace Spycraft {

Briefcase *theCase;
Intrface *intrface;
//Holster* theHolster;

extern Invent *inventry;
//extern InvView* theInvView;
extern PDA *thePDA;
extern Colby colby;
extern InvInset *theInvInset;

//CoatLeft* theLeftCoat;

PDAProp *pdaProp;

extern Rm1000 *rm1000;
extern BlinkCursor *normalCursor;
extern ArrowCursor *arrowCursor;
extern InvCursor *invCursor;
extern GunCursor *gunCursor;

#define THECASE_X 	10  //74
#define THEPDA_X  	437 //469 //391 //455
#define THEGUN_X	466 //530

int thePos, invPos;

Sound *briefSound = new Sound;


void IntrFeature::init(int theX, int theY, int theWidth, int theHeight) {
	left = theX;
	top = theY;
	right = left + theWidth - 1;
	bottom = top + theHeight - 1;
	intrface->addToFront(this);
}

void IntrFeature::dispose() {
	intrface->release(this);
	deathList->add(this);
}

void IntrView::init(int theX, int theY) {
	x = theX;
	y = theY;
	draw();
	intrface->addToFront(this);
	setHotspot(ALL_V);
}

void IntrView::dispose() {
	intrface->release(this);
	setHotspot(NULL);
	deathList->add(this);
}

IntrProp::~IntrProp() {
	if (cycler) {
		delete cycler;
		cycler = NULL;
	}
}


IntrProp::IntrProp() {
	name = "IntrProp";
	cycler = NULL;
	cycleSpeed = 10;
	__cycle_cnt = 0;
}

void IntrProp::setCycle(Cycle *theCycle) {
	if (cycler)
		delete cycler;

	cycler = theCycle;
}

void IntrProp::doit() {

	IntrView::doit();

	if (cycler && ((gameTime - __cycle_cnt) > (unsigned)cycleSpeed)) {
		__cycle_cnt = gameTime;

		int oldCel = cel;
		cycler->doit();
		if ((cycler) && (oldCel != cel)) {
			//sfxSpriteFrame(mySprite, myATS->reels[loop]->frames[cel]);
			sfxSpriteFrame(mySprite, sfxAnimFrame(myATS, loop, cel));
			setRect();
		}
	}

	/*	if ((closed) && (!cycler) && (onMe(mouse_x, mouse_y)) && (user->message == DO_V)) {
			if (!event_type)	{
				if (!myTime && closed)
					myTime = gameTime;

				if (myTime && (gameTime - myTime > 50) && (closed))	{
					open();
					myTime = 0;
				}
			}
		}
		else
			myTime = 0;*/
}

void IntrProp::open() {
	show();
	setCycle(new EndLoop((Prop *)this, this));
	closed = false;
}

void IntrProp::close() {
	setCel(0);
	//setCycle(new BegLoop ( (Prop*) this, this));
	closed = true;
	hide();
}

PDAProp::PDAProp() {
	name = "PDAProp";
	view = 904;
	loop = 2;
	cel = 0;
	cycleSpeed = 20;
	setHotspot(ALL_V);
}

int PDAProp::handleEvent(MADEEventStamp *event) {
	//if (user->message == DO_V) {
	if ((onMe(event)) && (event_type & USER_LEFT_UP) && user->input) {
		/*if (GameFlag.test(fWahKa79))	{
			user->canInput(false);
			intrface->setScript(new accessDenied);
			return true;
		}
		if (pdaProp) {
			intrface->blinking = false;
			intrface->release(pdaProp);
			pdaProp->setCycle(0);
			pdaProp->setHotspot(NULL);
			delete pdaProp;
			pdaProp = NULL;
		}*/
		if (theGame->enableIntrDuringMovie) {  //DEBUG
			theGame->setScript(0);
			//	if (!strcmp(curRoom->name, "Rm1000"))
					//rm1000->cleanUp();
			if (!strcmp(curRoom->name, "Rm1270"))
				theGame->setScript(new openPDAMovie());
			else {
				theMovie->caller = 0;
				theMovie->stop();
				theGame->handsOn();
			}

		}
		intrface->disable();
		if (theCase) {
			intrface->release(theCase);
			theCase->setHotspot(NULL);
			delete theCase;
			theCase = NULL;
		}

		if (inventry->theHand) {
			intrface->release(inventry->theHand);
			inventry->theHand->setHotspot(NULL);
			delete inventry->theHand;
			inventry->theHand = NULL;
		}
		inventry->hide(); //don't use delete, dispose of InvItem does not use deathlist or do delete
		if (thePDA) {
			intrface->release(thePDA);
			pdaProp->setCycle(0);
			thePDA->setHotspot(NULL);
			delete thePDA;
			thePDA = NULL;
		}
		intrface->isOpen = false;
		if (!strcmp(curRoom->name, "Rm91000"))
		{
			curRoom->setScript(new GoTargetScript);
			return true;
		}
		if (strcmp(curRoom->name, "Rm1270")) {
			new (Web);
			new (Home);
		}

		return(true);
	}
	//}
	return(false);
}
#if 0
{
	if (user->message == DO_V) {
		if ((onMe(event)) && (event_type & USER_LEFT_UP)) {
			if (theGame->enableIntrDuringMovie) {  //DEBUG
				theGame->setScript(0);
				//if (!strcmp(curRoom->name, "Rm1000"))
				//	rm1000->cleanUp();
//				if (!strcmp(curRoom->name, "Rm91000"))
//				{
//					theMovie->caller = 0;
//					theMovie->stop();
//				}
				if (!strcmp(curRoom->name, "Rm1270"))
					return false;
				//theGame->setScript(new openPDAMovie());
				else {
					theMovie->caller = 0;
					theMovie->stop();
					theGame->handsOn();
				}

			}
			intrface->disable();
			if (theCase) {
				intrface->release(theCase);
				delete theCase;
				theCase = NULL;
			}
			inventry->hide();
			if (thePDA) {
				intrface->release(thePDA);
				delete thePDA;
				thePDA = NULL;
			}
			intrface->isOpen = false;
			if (!strcmp(curRoom->name, "Rm91000"))
			{
				curRoom->setScript(new GoTargetScript);
				return true;
			}
			if (strcmp(curRoom->name, "Rm1270")) {
				new (Web);
				new (Home);
			}
			/*	if (pdaProp) {
					intrface->blinking = false;
					intrface->release(pdaProp);
					pdaProp->setCycle(0);
					delete pdaProp;
					pdaProp = NULL;
				}*/

			return(true);
		}
	}
	retrn(false);
}
#endif
void PDAProp::respond(void) {
	theGame->setCursor(normalCursor);
	sfxSetCursor(970, 0, 1);
}

int Briefcase::handleEvent(MADEEventStamp *event) {
	if (onMe(event)) {
		if (event_type & USER_LEFT_UP) {
			if (closed) {
				if (!inventry->active) {
					if (PLAY_BRIEF) {
						sfxLoadRes(136, RES_WAVE);
						briefSound->play(136, NULL);
					}
					this->open();
				} else {                       //put item away
					//normalCursor = arrowCursor;
					//theGame->setCursor(normalCursor);
					normalCursor->verb = DO_V;
					user->message = DO_V;
					inventry->active = false;
					invVerb = 0;
					if (inventry->theHand)
						inventry->theHand->dispose();
					else
						ASSERT(0, 0);
					theGame->resetHotspot();
					return true;
				}
				return true;
			} else {

				if (!inventry->active)

					return(true);
				//sfxPrintf("This shouldn't happen"); //DEBUG
				else {                       //put item away

					normalCursor->verb = DO_V;
					user->message = DO_V;
					inventry->active = false;
					inventry->theHand->dispose();
					invVerb = 0;
					inventry->show();
					theGame->resetHotspot();
					return true;
				}
				return true;
			}
		}
	}
	return false;
}

Briefcase::Briefcase() {
	closed = true;
	name = "briefcase";
	myTime = 0;
	backButton = NULL;
	forwardButton = NULL;
}

class OpenCaseScript : public Script
{
public:
	void changeState(int);
};


class caseTimer : public Timer
{
public:
	void cue(void);
};

caseTimer *theCaseTimer;

void caseTimer::cue() {
	caller = 0;
	if (!inventry->active)
		inventry->show();
	theCase->setLoop(1);
	user->canInput(true);
	theCaseTimer = NULL;
	dispose();
}

void Briefcase::open() {

	closed = false;

	setLoop(1);
	if (!inventry->active)
		inventry->show();
	backButton = new BackInv;
	forwardButton = new ForwardInv;




	/*user->canInput(false);
	backButton = new BackInv;
	forwardButton = new ForwardInv;
	closed = false;
	//setScript(new OpenCaseScript);

	if (PLAY_BRIEF)	{
		sfxLoadRes(136, RES_WAVE);
		briefSound->play(136, NULL);
		theCaseTimer = new caseTimer;
		theCaseTimer->setTicks(10, theCaseTimer);
	}
	else	{
		if (!inventry->active)
			inventry->show();

		theCase->setLoop(1);
		user->canInput(true);
	}



	if (!inventry->active)
		inventry->show();
	backButton = new BackInv;
	forwardButton = new ForwardInv;
	setLoop(1);*/
}

void Briefcase::close() {
	inventry->hide();
	closed = true;
	if (backButton) {
		backButton->dispose();
		backButton = NULL;
	}
	if (forwardButton) {
		forwardButton->dispose();
		forwardButton = NULL;
	}
	if (!inventry->theHand)
		inventry->active = false;

	//theInvView->dispose();
	setLoop(0);
}

BackInv::BackInv() {
	name = "BackInv";
	init(18, 458, 26, 19);
}

int BackInv::handleEvent(MADEEventStamp *event) {
	if ((onMe(event)) && (event_type & USER_LEFT_UP) && !inventry->theHand) {
		if (inventry->curItem != -1)
			inventry->goBackward();

		return true;
	}
	return false;
}

ForwardInv::ForwardInv() {
	name = "ForwardInv";
	init(67, 458, 26, 119);
}

int ForwardInv::handleEvent(MADEEventStamp *event) {
	if ((onMe(event)) && (event_type & USER_LEFT_UP) && !inventry->theHand) {
		if (inventry->curItem != -1)
			inventry->goForward();

		return true;
	}
	return false;
}

void Intrface::doit() {
	if (!strcmp(curRoom->name, "Rm91000")) {
		List::doit();
		return;
	}
	if ((isOpen == false) && (mouse_y > 460) && (user->input) && (currentFocus != PUZZLE_FOCUS) && (currentFocus != DIALOG_FOCUS) &&
		(enabled || ((theGame->enableIntrDuringMovie) && (!currentFocus)))) {
		if (user->message == DO_V) {
			if (!script) {
				//	if (theMovie->isPlaying)	
						//theMovie->pause(false);

				isOpen = true;
				theCase = new(Briefcase);
				//theHolster = new(Holster);
				thePDA = new(PDA);

				theCase->view = vINTERFACE;
				theCase->loop = 0;
				theCase->cel = 0;
				theCase->init(10, 480); //74
				//theHolster->init(530,480);
				thePDA->init(391, 480);	//455
				setScript(new OpenInterface);
			}
		}
	} else if ((isOpen) && (mouse_y < 410) && (!inventry->active) && (strcmp(curRoom->name, "Rm1270"))) {

		if (!script) {

			/*1216	if (pdaProp) {
					intrface->blinking = false;
					pdaProp->setCycle(0);
					intrface->release(pdaProp);
					delete pdaProp;
					pdaProp = NULL;

				}*/

			thePos = 421;
			theCase->close();
			setScript(new CloseInterface);
		}
	}
	List::doit();
}

void Intrface::disable() {
	if (enabled) {

		if (script) {
			//	sfxPrintf("Disabling intrface when it is rising");
			setScript(NULL);
		}
		hide();
		enabled = false;
		if (pdaProp) {
			intrface->blinking = false;

			pdaProp->setCycle(0);
			intrface->release(pdaProp);
			pdaProp->setHotspot(NULL);
			delete pdaProp;
			pdaProp = NULL;

		}
	}
}

void Intrface::enable() {
	enabled = true;
	intrface->canBlink = true;
}

void Intrface::show() {

}

void Intrface::hide() {

	if (pdaProp) {
		intrface->blinking = false;
		pdaProp->setCycle(0);
		intrface->release(pdaProp);
		pdaProp->setHotspot(NULL);
		delete pdaProp;
		pdaProp = NULL;

	}
	if (theInvInset->invInsetView && theInvInset->invInsetView->view < 50000)
		theInvInset->dispose();

	if (inventry->theHand) {
		intrface->release(inventry->theHand);
		inventry->theHand->setHotspot(NULL);
		delete inventry->theHand;
		inventry->theHand = NULL;
	}
	if (isOpen) {
		if (!script) {
			if (theCase->backButton) {
				theCase->backButton->dispose();
				theCase->backButton = NULL;
			}
			if (theCase->forwardButton) {
				theCase->forwardButton->dispose();
				theCase->forwardButton = NULL;
			}
			if (theCase) {
				intrface->release(theCase);
				theCase->setHotspot(NULL);
				delete theCase;
				theCase = NULL;
			}
			inventry->hide(); //don't use delete, dispose of InvItem does not use deathlist or do delete
			if (thePDA) {
				intrface->release(thePDA);
				thePDA->setHotspot(NULL);
				delete thePDA;
				thePDA = NULL;
			}
			intrface->isOpen = false;
		}
	}
}

void Intrface::blink() {
	blinking = true;
	pdaProp = new PDAProp;
	pdaProp->init(THEPDA_X, 400);
	pdaProp->cycler = 0;
	pdaProp->noSkip();
	pdaProp->setPri(490);
	pdaProp->setCycle(new Forward((Prop *)pdaProp));
}

Intrface::Intrface() {
	name = "Interface";
	enabled = true;
	isOpen = false;
	blinking = false;
	canBlink = true;

}

Intrface :: ~Intrface() {
	//delete briefSound;
}

int Intrface::handleEvent(MADEEventStamp *event) {
	if (user->input && (List::handleEvent(event)))
		return true;
	else
		return false;
}


void OpenInterface::changeState(int newState) {
	switchTo
		user->canInput(false);
	flag = false;
	thePos = 477;
	invPos = 504;
	if (inventry->active) {
		if (!inventry->theHand) {
			inventry->theHand = new InvHand;
			inventry->theHand->init(inventry->items[inventry->curItem]->myInset, inventry->items[inventry->curItem]->myIndex, inventry->items[inventry->curItem]->handView, HAND_X, 504, inventry->items[inventry->curItem]->insetView);
		} else
			flag = true;
		theCase->setLoop(1);
		invVerb = inventry->items[inventry->curItem]->verb;
	}

	theCase->posn(THECASE_X, 477);
	//	theHolster->posn(THEGUN_X,477);
	thePDA->posn(THEPDA_X, 477);
	ticks = 1;

	END

		BEG
		//sfxPrintf("here1");
		if (thePos > 404) {
			thePos -= 4;
			invPos -= 4;
			theCase->posn(THECASE_X, thePos);
			//	theHolster->posn(THEGUN_X,thePos);
			thePDA->posn(THEPDA_X, thePos);
			if ((invPos > 426) && (inventry->active) && !flag)
				inventry->theHand->posn(HAND_X, invPos);
			state = 0;
		}
	ticks = 1;
	END
		BEG
		//	sfxPrintf("here2");
		theCase->posn(THECASE_X, 400);
	thePDA->posn(THEPDA_X, 400);
	if (inventry->active) {
		//inventry->theHand = new InvHand;
		//inventry->theHand->init(inventry->items[inventry->curItem]->myInset, inventry->items[inventry->curItem]->myIndex, inventry->items[inventry->curItem]->handView);
		if (!flag) {
			inventry->theHand->posn(HAND_X, HAND_Y);
			inventry->theHand->setHotspot(ALL_V);
		}
		theCase->open();
	}

	user->canInput(true);
	dispose();
	END
}

void CloseInterface::changeState(int newState) {
	switchTo
		ticks = 1;
	flag = user->input;
	user->canInput(false);
	//ASSERT(theCase, 0);
	//ASSERT(thePDA, 0);

	END

		BEG
		if (thePos < 480) {
			thePos += 4;
			theCase->posn(THECASE_X, thePos);
			//theHolster->posn(THEGUN_X,thePos);
			thePDA->posn(THEPDA_X, thePos);
			state = 0;
		}
	ticks = 1;
	END

		BEG
		theCase->dispose();
	//	theHolster->dispose(); 
	thePDA->dispose();
	theCase = 0;
	thePDA = 0;
	if (inventry->theHand)
		inventry->theHand->dispose();
	//theHolster = 0;
	//ticks = 10;
	intrface->isOpen = false;

	if (theGame->enableIntrDuringMovie && theMovie->paused)	 //special case for monument and travel link
		theMovie->resume();
	user->canInput(true);
	dispose();
	END

		/*	BEG
				intrface->isOpen = false;
			/*	if (intrface->blinking)	{
					pdaProp->show();
					pdaProp->setCycle(new Forward ((Prop*) pdaProp));
				}
				dispose();
			END*/
}




void OpenCaseScript::changeState(int newState) {
	switchTo
		theCase->closed = false;
	if (PLAY_BRIEF) {
		sfxLoadRes(136, RES_WAVE);
		briefSound->play(136, NULL);
		ticks = 15;
	} else
		cue();
	END
		BEG
		if (!inventry->active)
			inventry->show();

	theCase->setLoop(1);
	user->canInput(true);
	END
}

} // namespace Spycraft
