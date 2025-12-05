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

#include "globals.h"
#include "game.h"
#include "verbs.h"
#include "views.h"
#include "spycraft/logic/69000.h"
#include "spycraft/logic/69100.h"
#include "flag.h"
#include "roomsnd.h"
#include "sound.h"
#include "advsound.h"

namespace Spycraft {

#define CRSBLINK_SPEED 5

// flag : fLinus22 used to lock out hand cuff sound for exit
// flag : fLinus28 used to lock out changes until sound finishes

/*********************
*
*		Room Code 
*
**********************/

extern Intrface* intrface;
extern BlinkCursor* normalCursor;
extern EastCursor* eastCursor;
extern SouthCursor* southCursor;

Rm69100::Rm69100()
{
	name = "Rm69100";
	show_style = VE_FLIP;
}

Rm69100::~Rm69100()
{
}

void Rm69100::init()
{
	// Preload for next room
	sfxLoadRes ( 69101, RES_PIC );

	// Outside shot, closet doors closed
  	drawPic( 69100 );
    
    Room::init();
  
	new(EastExit69100);
	new(Closet69100);

	if (GameFlag.test(fGotHandcuffs))
		new ( Cuffs69000 );

 	intrface->show();
  	theGame->handsOn();
}

int Rm69100::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}

EastExit69100::EastExit69100()
{
	name = "EastExit69100";
    setHotspot ( DO_V );
	init(EAST_EXIT_X, EAST_EXIT_Y, EAST_EXIT_WIDTH, EAST_EXIT_HEIGHT);
}

void EastExit69100::respond()
{
	theGame->setCursor(eastCursor);
	aniCursor();
}

int EastExit69100::doVerb ( int theVerb )
{
	if (( theVerb == DO_V ) && !GameFlag.test(fLinus28)) {
		theGame->newRoom(new Rm69002);
		return TRUE;
	}
	return FALSE;
}

Rm69101::Rm69101()
{
	name = "Rm69101";
	show_style = VE_FLIP;
}

Rm69101::~Rm69101()
{
}

void Rm69101::init()
{
	// Display open closet with or without cuffs

	if (!GameFlag.test(fClosetWHand69100)) 	
		drawPic( 69101 );			// closet w/ handcuffs
	else
  		drawPic( 69102 );			// closet w/o handcuffs

    Room::init();

	if (GameFlag.test(fGotHandcuffs))
		new ( Cuffs69000 );
	new(EastExit69101);
	new(LeftClosetDoor69101);
	new(RightClosetDoor69101);
	new(Closet69101);
  	
 	intrface->show();
  	theGame->handsOn();
}

int Rm69101::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}

EastExit69101::EastExit69101()
{
	name = "EastExit69101";
    setHotspot ( DO_V );
	init(EAST_EXIT_X, EAST_EXIT_Y, EAST_EXIT_WIDTH, EAST_EXIT_HEIGHT);
}

void EastExit69101::respond()
{
	theGame->setCursor(eastCursor);
	aniCursor();
}

int EastExit69101::doVerb ( int theVerb )
{
	if (( theVerb == DO_V ) && !GameFlag.test(fLinus28)) {
		theGame->newRoom(new Rm69002);
    	return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Features
 *
/*******************************/

Closet69100::Closet69100()
{
	name = "Closet69100";
	setHotspot( DO_V );
	init( 177, 97, 248, 303 );
}

void Closet69100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Closet69100::doVerb( int theVerb )
{
	if(( theVerb == DO_V ) && !GameFlag.test(fLinus28))
	{
		if (GameFlag.test(fClosetOpen69100))
			theGame->newRoom(new Rm69101);
		else
		{
			GameFlag.set(fClosetOpen69100);
			curRoom->setScript ( new ClosetOpen69100 );
		}
	}
	return FALSE;
}

LeftClosetDoor69101::LeftClosetDoor69101()
{
	name = "LeftClosetDoor69101";
	setHotspot( DO_V );
	init( 93, 63, 104, 333 );
}

void LeftClosetDoor69101::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int LeftClosetDoor69101::doVerb( int theVerb )
{
	if(( theVerb == DO_V ) && !GameFlag.test(fLinus28))
	{
		if (GameFlag.test(fClosetOpen69100))
		{
			GameFlag.clear(fClosetOpen69100);
			curRoom->setScript ( new ClosetClose69100 );
		}
		return TRUE;
	}
	return FALSE;
}

RightClosetDoor69101::RightClosetDoor69101()
{
	name = "RightClosetDoor69101";
	setHotspot( DO_V );
	init( 416, 75, 77, 328 );
}

void RightClosetDoor69101::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int RightClosetDoor69101::doVerb( int theVerb )
{
	if(( theVerb == DO_V ) && !GameFlag.test(fLinus28))
	{
		if (GameFlag.test(fClosetOpen69100))
		{
			GameFlag.clear(fClosetOpen69100);
 			curRoom->setScript ( new ClosetClose69100 );
		}
		return TRUE;
	}
	return FALSE;
}

Closet69101::Closet69101()
{
	name = "Closet69101";
	setHotspot( DO_V );
	init(136, 200, 200, 220);
}

void Closet69101::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Closet69101::doVerb( int theVerb )
{
	if(( theVerb == DO_V ) && !GameFlag.test(fLinus28))
	{
		if (GameFlag.test(fClosetOpen69100))
			theGame->newRoom(new Rm69102);
		return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Insets
 *
 *******************************/
/*
ClosetInset69100::ClosetInset69100()
{
 	init(69120, 0, 15, 512, 385);
	normalCursor->rest();
}
*/

// Inside the closet
Rm69102::Rm69102()
{
	name = "Rm69102";
	show_style = VE_FLIP;
}

Rm69102::~Rm69102()
{
}

void Rm69102::init()
{
  	drawPic( 69140 );  
    Room::init();
  
	new(SouthExit69102);
	if (!GameFlag.test(fGotHandcuffs))
		new(Handcuffs69100);
  	else
		new(Cuffs69000);
 	intrface->show();
  	theGame->handsOn();
}

int Rm69102::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}

SouthExit69102::SouthExit69102()
{
	name = "SouthExit69102";
    setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
}

void SouthExit69102::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit69102::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	if (!GameFlag.test(fLinus22))
			theGame->newRoom(new Rm69101);
    	return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Views
 *
 *******************************/

Handcuffs69100::Handcuffs69100()
{
	name = "Handcuffs69100";
	setHotspot ( DO_V );
	setView(69150);
//	init(183, 267);
	init(183, 286);
	noSkip();
}

void Handcuffs69100::respond()
{
		theGame->setCursor( normalCursor );
		normalCursor->blink(CRSBLINK_SPEED);
}

int Handcuffs69100::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		GameFlag.set(fClosetWHand69100);
		curRoom->setScript ( new HandcuffSound69100 );
		new ( Cuffs69000 );
		dispose();
    	return TRUE;
	}
	return FALSE;
}

/*********************/
/* Scripts for Sound */
/*********************/

void ClosetOpen69100::changeState( int newState )
{
	switchTo
		GameFlag.set(fLinus28);
		closetSnd->play( 69011, this );
	END

	BEG
	   	ticks = 2;
	END

	BEG
		closetSnd->dispose();
		GameFlag.clear(fLinus28);
		theGame->newRoom(new Rm69101);
	END
}

void ClosetClose69100::changeState( int newState )
{
	switchTo
		GameFlag.set(fLinus28);
		closetSnd->play( 69013, this );
	END

	BEG
	   	ticks = 2;
	END

	BEG
		closetSnd->dispose();
		GameFlag.clear(fLinus28);
		theGame->newRoom(new Rm69100);
	END
}

void HandcuffSound69100::changeState( int newState )
{
	switchTo
		GameFlag.set(fLinus22);
		handCuffSnd->play( 69016, this );
	END

	BEG
	   	ticks = 2;
	END

	BEG
		handCuffSnd->dispose();
		GameFlag.clear(fLinus22);
		GameFlag.set(fGotHandcuffs);
	END
}
 
} // namespace Spycraft 
