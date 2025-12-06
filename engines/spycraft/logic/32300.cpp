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
#include "spycraft/game/views.h"
#include "spycraft/logic/32300.h"
#include "spycraft/logic/32200.h"
//#include "spycraft/game/movie.h"
//#include "spycraft/game/vlink.h"

namespace Spycraft {

#define CRSBLINK_SPEED 5

extern Intrface* intrface;
extern SouthCursor* southCursor;
//extern NorthwestCursor* northwestCursor;
extern BlinkCursor* normalCursor;

/*********************
*
*	Room 
*
**********************/

Rm32300::Rm32300()
{
	name = "Rm32300";
	show_style = VE_FLIP;
}

void Rm32300::init()
{
	drawPic( 32300 );
	Room::init();
	new SouthExit32300;
//	new NorthWestExit32300;
	intrface->show();
	theGame->handsOn();
}

/*********************
*
*	Exit Features 
*
**********************/

SouthExit32300::SouthExit32300()
{
	name = "SouthExit32300";
	setHotspot( DO_V );
	init( SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT );  
}

void SouthExit32300::respond()
{
	theGame->setCursor( southCursor );
	aniCursor();
}

int SouthExit32300::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
    	theGame->newRoom( new Rm32200 );
    	return true;
	}
	return false;
}
/*
NorthWestExit32300::NorthWestExit32300()
{
	name = "NorthWestExit32300";
	setHotspot( DO_V );
	init(155, 63, 69, 123);
}

void NorthWestExit32300::respond()
{
	theGame->setCursor( northwestCursor );
}

int NorthWestExit32300::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
//    	theGame->newRoom( new Rm32200 );
			if( !GameFlag.test( fVCRoomEnter ))	   
				setScript( new EnterScript32300 );
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
/*
EnterScript32300::EnterScript32300()
{
	name = "EnterScript32300"; 
}

void EnterScript32300::changeState( int newState )
{
	switchTo
		GameFlag.set(fVCRoomEnter);
		theGame->handsOff();
		theMovie->play("32300.avi", this, 0, 0, 1);
	END
	BEG
		theGame->handsOn();
		dispose();
		new VLink( "32301.avi", 0, this );	
	END
}
*/
 
} // namespace Spycraft 
