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

#include "spycraft/game/globals.h"
#include "spycraft/game/game.h"
#include "spycraft/game/verbs.h"
#include "views.h"
#include "spycraft/logic/25100.h"
#include "spycraft/logic/25000.h"
//#include "spycraft/logic/22300.h"
#include "spycraft/logic/1000.h"
#include "spycraft/game/flag.h"
#include "spycraft/game/movie.h"
#include "spycraft/game/vlink.h"
#include "torture.h"
#include "spycraft/logic/60000.h"

namespace Spycraft {

#define CRSBLINK_SPEED 5

/*********************
*
*		Room Code 
*
**********************/

extern Intrface* intrface;
extern BlinkCursor* normalCursor;
extern SouthCursor* southCursor;
extern EastCursor* eastCursor;
extern Sound* sound1;

Rm25000::Rm25000()
{
	name = "Rm25000";
	show_style = VE_FLIP;
//	show_style = VE_FULL_DISSOLVE;
}

Rm25000::~Rm25000()
{
   	sound1->stop();  
}

void Rm25000::init()
{
    Room::init();

  	new(SouthExit25000);
	new(EastExit25000);
	new(ControlPanel25000);

  	intrface->show();

//	if( GameFlag.test(fDoneInterrogation) && !GameFlag.test( fOnceBullpen25000 ))    
// 		setScript( new EnterScript25000 );
//	else if( GameFlag.test(fStartBullpen) && !GameFlag.test( fOnceBullpen25000 ))

	if( GameFlag.test(fLockOut)) {
		drawPic( 25000 );
		theGame->handsOn();
		if( sound1->number != 25000  || !sound1->isPlaying() )	{
			sound1->loop = -1; 
			sound1->playBuffered(25000, 0);
		}
		sfxPrintf("Bullpen has been locked out.");
	}
		else if( GameFlag.test(fStartBullpen) && !GameFlag.test( fOnceBullpen25000 )) {
////	else if( GameFlag.test(fStartBullpen) && GameFlag.test(fSaw646) && !GameFlag.test( fOnceBullpen25000 )) {
///		drawPic( 1 );
		setScript( new EnterScript25000 );
	}
	else {
		drawPic( 25000 );
		theGame->handsOn();
	
		if( sound1->number != 25000  || !sound1->isPlaying() )	{
			sound1->loop = -1; 
			sound1->playBuffered(25000, 0);
		}
	}
}

int Rm25000::handleEvent ( MADEEventStamp *event )
{
	return false;
}

SouthExit25000::SouthExit25000()
{
	name = "SouthExit25000";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);  
}

void SouthExit25000::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit25000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
//		theGame->newRoom(new Rm22300);
		theGame->newRoom(new Rm1000);
    	return true;
	}
	return false;
}

EastExit25000::EastExit25000()
{
	name = "EastExit25000";
	setHotspot ( DO_V );
//	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);  
}

void EastExit25000::respond()
{
	theGame->setCursor(eastCursor);
	aniCursor();
}

int EastExit25000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm25100);
    	return true;
	}
	return false;
}

ControlPanel25000::ControlPanel25000()
{
	name = "Control Panel";
	setHotspot ( DO_V );
	init ( 124, 93, 152, 148 );
}

void ControlPanel25000::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int ControlPanel25000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	new(ControlPanelInset25000);
    	return true;
	}
	return false;
}

ControlPanelInset25000::ControlPanelInset25000()
{
//	init(25050, 0, 50, 50, 50);
	normalCursor->rest();
	init(25020, 0, 15, 512, 385);
}
/*
void ControlPanelInset25000::respond()
{
}

int ControlPanelInset25000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	sfxPrintf("doVerb for Gift inset");
    	return true;
	}
	return false;
}
*/

/*******************************
 *
 *	Scripts
 *
/*******************************/

EnterScript25000::EnterScript25000()
{
	name = "EnterScript25000"; 
}

void EnterScript25000::changeState( int newState )
{
	switchTo
		ticks = 2;
	END

	BEG
		GameFlag.set(fOnceBullpen25000);			 
		normalCursor->rest();
		intrface->disable();
		curRoom->drawPic( -1 );
		new(Torture);
//		ticks = 2;
		seconds = 1;
	END
	BEG
		theGame->handsOff();
		ticks = 10;
	END
	BEG
//		if(GameFlag.test(fWahKa63)) {						//Ying is fried
		if(GameFlag.test(fTortureYingDead)) {
			theMovie->play("25013.avi", this, 0, 1, 1);	
//			theGame->newRoom(new Rm60000(END_JAIL));
		}
		else if(GameFlag.test(fBrokenYingInterrogation)) {	//Ying is broken
				GameFlag.set(fPyramidInfoAvailable);
//				new VLink( "25003.avi", 0, 0);			//vlink 654, Look into other thefts
//				new VLink( "25004.avi", 0, 10);			//vlink 655, Skodi details
////			new VoiceMail("98072.wav", 98072, SEATON );//Jaimie
//				new VLink( "Dangerous new weapon: Warhurst, Eugene J.", "25014.avi", 0, 15);			//vlink new, Skodi
//				GameFlag.set(fWahKa19);					//link to vlink 205, Warhurst
//				new VLink( "Dangerous new weapon: Warhurst, Eugene J.", "25014.avi", fSaw25014, 15);	//vlink new, Skodi
//				new VLink( "Brooks cuts peace trip short: Newscast", "25015.avi", 0, 60);			//vlink 540 
				GameFlag.set(fWahKa32);					//link to vlink 657, Jaimie
				new EMail( "Killers for hire: Seaton, Jaimie A.", 98070, fWahKa113, 10);//Jaimie about the fake Procat file (fake?)

				theGame->newRoom(new Rm1000);
		}
		else if(!GameFlag.test(fTortureYingDead) && !GameFlag.test(fBrokenYingInterrogation)) {
			GameFlag.clear(fStartBullpen);
			GameFlag.clear(fOnceBullpen25000);
			GameFlag.clear(fDoneInterrogation);
			GameFlag.set(fStartInterrogation);
			new EMail( "I've moved Ying: Foster, Maxine M.", 98097, NULL, 10);	//didn't break Ying
			theGame->newRoom(new Rm1000);
		}
			
	END
	BEG
		theGame->newRoom(new Rm60000(END_JAIL));
	END
/*
		theGame->handsOff();
		theMovie->play("25000.avi", this, 0, 1, 1);
		new VLink( "25001.avi", 0, 0);
		new VLink( "25002.avi", 0, 30);			//vlink 722
		new VLink( "25003.avi", 0, 0);			//vlink 654
	    new VLink( "25004.avi", 0, 0);			//vlink 655
		new VLink( "25005.avi", 0, 0);			//vlink 205
	    new VLink( "25006.avi", 0, 0);			//vlink 596
		new VLink( "25007.avi", 0, 0);			//vlink 206
	    new VLink( "25008.avi", 0, 0);			//vlink 657
		new VLink( "25009.avi", 0, 0);			//vlink 27
	    new VLink( "25010.avi", 0, 0);			//vlink 752
		sfxPrintf("Topic: about Procat");
		new VLink( "25011.avi", 0, 0);			//vlink 207
	END
*/
}
 
} // namespace Spycraft 
