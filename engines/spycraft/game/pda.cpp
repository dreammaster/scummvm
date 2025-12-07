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

#include "spycraft/game/pda.h"
#include "spycraft/game/web.h"
#include "spycraft/dmade/aviread.h"
#include "spycraft/logic/1000.h"
#include "spycraft/logic/91000.h"
#include "spycraft/game/invent.h"
#include "spycraft/dmade/winscreen.h"

namespace Spycraft {

PDA* thePDA;
extern Briefcase* theCase;
Print* accessPrint;

//PDATimer* thePDATimer = new PDATimer;

extern int currentFocus;
extern Intrface* intrface;

extern PDAProp* pdaProp;

extern Rm1000* rm1000;

class accessDenied: public Script
{
public:
	void changeState ( int );
};



PDA::PDA()
{
	name = "PDA";
	view = 904;
	loop = 2;
	cel = 0;
	priority = 94;
	whoToCue = NULL;
	myDialogTree = NULL;
	PDAOpen = false;;
    myMovie = NULL;
  	blinking = false;
}

void PDA::respond( void )
{
	sfxSetCursor(970,0,1);
}

void PDA::VLinkNotify(const char *filename, unsigned int waitTime)
{
/*	thePDATimer->myMovie = filename;
	if (waitTime)
		thePDATimer->setReal(waitTime);
	else
		thePDA->blink();*/
}

void PDA::VLinkNotify(const char *filename, unsigned int waitTime, Object* toCue)
{
/*	whoToCue = toCue;
	VLinkNotify(theMovie, waitTime);*/
}
	
void PDA::playVLink()
{
//	if (thePDATimer->myMovie)	
//		setScript(new VLinkScript);
}

void PDA::blink()
{
	//blinking = true;
//	pdaProp = new Prop;
//	pdaProp->init(400, 410);
}

void PDA::init(int theX, int theY)
{
	//PDAOpen = true;
	//currentFocus = PDA_FOCUS;
	IntrView::init(theX, theY);
//	if (thePDATimer->myMovie)
	//	playVLink();
}

class PDAATS : public View  //DEBUG move to pda.h
{
public:
	PDAATS();
};

PDAATS* thePDAATS;

PDAATS::PDAATS( void )
{
	name = "PDAATS";
	view = 910;
	loop = 0;
	cel = 0;
} 

/*void VLinkScript::changeState ( int newState )
{
	switchTo
     	ticks = 6;
	END

	BEG
		theGame->handsOff();
     	theMovie->play(thePDATimer->myMovie, 235, 100, 200, 200, this);
	END

	BEG
		theGame->handsOn();
		dispose();
	END
}*/	

/*void PDA::dispose()
{
	intrface->release ( this );
	deathList->add ( this);
}*/
		
int PDA::handleEvent(MADEEventStamp* event)
{
 	//if (user->message == DO_V) {
	    if ((onMe(event)) && (event_type & USER_LEFT_UP) && user->input) { 
      		if (GameFlag.test(fWahKa79))	{
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
			}
			if (theGame->enableIntrDuringMovie)	{  //DEBUG
				theGame->setScript(0);
			//	if (!strcmp(curRoom->name, "Rm1000"))
				//	rm1000->cleanUp();
				if (!strcmp(curRoom->name, "Rm1270"))
					theGame->setScript(new openPDAMovie());
				else	{
					theMovie->caller = 0;
					theMovie->stop();
					theGame->handsOn();
				}
				
			}
			intrface->disable();
			if (theCase)	{
				intrface->release(theCase);
				theCase->setHotspot(NULL);
				delete theCase;
				theCase = NULL;
			}

			if (inventry->theHand)	{
				intrface->release(inventry->theHand);
				inventry->theHand->setHotspot(NULL);
				delete inventry->theHand;
				inventry->theHand = NULL;
			}
			inventry->hide(); //don't use delete, dispose of InvItem does not use deathlist or do delete
			if (thePDA)	{ 
				intrface->release(thePDA);
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
			if (strcmp(curRoom->name, "Rm1270"))	{
		 		new (Web);
		  		new (Home);
			}
			
	     	return(true);
	    }  
  //}
  return(false);
}
  

void PDATimer::cue()
{
	thePDA->blink();
}




openPDAMovie::openPDAMovie()
{
	name = "openPDAMovie";
}

void openPDAMovie::changeState ( int newState )
{
	switchTo
		user->canInput(false);
		
		theMovie->pause(false, false);
		theMovie->fromTo(195, 221);	
		theMovie->play( "1271.avi", this, false, true, false );
	END

	BEG
		AddScreen2Pic();
		ticks = 10;	
	END

	BEG	
		user->canInput(true);
		new (Web);
	  	new (Home);
		dispose();
	END			
}

void accessDenied::changeState ( int newState )
{
	switchTo
		accessPrint = new Print;
		accessPrint->x = 406;
		accessPrint->y = 438;
		accessPrint->priority = 500;
		accessPrint->fore_color =  madeRGB(181, 24, 60);
		accessPrint->init("Access Denied");
		ticks = 60;	
	END

	BEG	
		accessPrint->dispose();
		user->canInput(true);
		dispose();
	END			
}

} // namespace Spycraft
