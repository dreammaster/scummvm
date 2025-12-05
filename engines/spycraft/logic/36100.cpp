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
#include "spycraft/logic/36100.h"
#include "spycraft/logic/36000.h"
#include "spycraft/logic/36300.h"
#include "flag.h"
#include "movie.h"

namespace Spycraft {

#define CRSBLINK_SPEED 5

extern Intrface* intrface;
extern SouthCursor* southCursor;
extern BlinkCursor* normalCursor;

/*********************
*
*	Room 
*
**********************/

Rm36100::Rm36100()
{
	name = "Rm36100";
	show_style = VE_FLIP;
}

void Rm36100::init()
{
	drawPic( 36100 );
	Room::init();

	new SouthExit36100;
	new	Desk36100;
//	new Photo36100;
	new Computer36100;

	intrface->show();
	theGame->handsOn();

	//if(GameFlag.test(fPlayerTurnsProcat) && !GameFlag.test(fGetProcatFile))  //1 Minute Max caught player
	//	setScript( new EnterScript36100 );	   //DEBUG
}

/*********************
*
*	Exit Features 
*
**********************/

SouthExit36100::SouthExit36100()
{
	name = "SouthExit36100";
	setHotspot( DO_V );
	init( SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT );  
}

void SouthExit36100::respond()
{
	theGame->setCursor( southCursor );
	aniCursor();
}

int SouthExit36100::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
	
    	theGame->newRoom( new Rm36000 );
    	return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Features
 *
/*******************************/

Desk36100::Desk36100()
{
	name = "Desk36100";
	setHotspot( DO_V );
	init( 261, 130, 230, 265 );
}

void Desk36100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Desk36100::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		GameFlag.set(fGetProcatFile);
		theGame->newRoom( new Rm36300 );
		return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Text Features
 *
/*******************************/
/*
Photo36100::Photo36100()
{
	name = "Photo36100";
	setHotspot( DO_V );
	init ( 100, 100, 200, 50 );
}

void Photo36100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Photo36100::doVerb( int theVerb )
{
	if ( theVerb == DO_V ) {
		sfxPrintf( "Photo");
		return TRUE;
	}
	return FALSE;
}
*/

Computer36100::Computer36100()
{
	name = "Computer36100";
	setHotspot( DO_V );
	init ( 414, 17, 166, 140 );
}

void Computer36100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Computer36100::doVerb( int theVerb )
{
	if ( theVerb == DO_V ) {
		new ComputerInset36100();
		return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Insets
 *
 *******************************/

ComputerInset36100::ComputerInset36100()
{
	normalCursor->rest();
 	init(36120, 0, 15, 512, 385);
}

/*******************************
 *
 *	Scripts
 *
 *******************************/
 
EnterScript36100::EnterScript36100()
{
	name = "EnterScript36100"; 
}

void EnterScript36100::changeState( int newState )
{
	switchTo
		seconds = 60;

	END
	BEG
		GameFlag.set(fGetProcatFile);	
		theGame->handsOff();
///		theMovie->fromTo(390, 630);
   		theMovie->play("36100.avi", this, 0, 1, 1);
	END
	BEG
		theGame->handsOn();
		dispose();
	END
}
 
} // namespace Spycraft 
