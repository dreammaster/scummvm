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
#include "spycraft/logic/69000.h"
#include "spycraft/logic/69300.h"
#include "spycraft/game/flag.h"

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
extern BlinkCursor* normalCursor;
//extern Print* testPrint;

Rm69300::Rm69300()
{
	name = "Rm69300";
	show_style = VE_FLIP;
}

//Rm69300::~Rm69300()
//{
   //testPrint->erase();
//}

void Rm69300::init()
{
  	drawPic( 69300 );
    Room::init();
  
	new(SouthExit69300);
	new(DisguiseKit69300);
	
  	intrface->show();
  	theGame->handsOn();

   //testPrint = new(Print); 
   //testPrint->init("Hotel Schlump Int.");
}

int Rm69300::handleEvent ( MADEEventStamp *event )
{
//	if (event_type == USER_LEFT_UP)	{
//		new(DisguiseKitInset69320);
//	}	
	return false;
}
/*
int Rm69300::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	new(DisguiseKitInset69320);
    	return true;
	}
	return false;
}
*/

/*******************************
 *
 *	Features
 *
/*******************************/

DisguiseKit69300::DisguiseKit69300()
{
	name = "DisguiseKit69300";
	setHotspot( DO_V );
	init( 150, 150, 211, 159 );
}

void DisguiseKit69300::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int DisguiseKit69300::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		new DisguiseKitInset69320();
		return true;
	}
	return false;
}

/*******************************
 *
 *	Insets
 *
 *******************************/

DisguiseKitInset69320::DisguiseKitInset69320()
{
	normalCursor->rest();
// switch places with 69320
 	init(69340, 0, 15, 512, 385);
}

void DisguiseKitInset69320::respond()
{

}

int DisguiseKitInset69320::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
		dispose();
		new DisguiseKitInset69340();
    	return true;
	}
	return false;
}

DisguiseKitInset69340::DisguiseKitInset69340()
{
	normalCursor->rest();
// switch places with 69340 
 	init(69320, 0, 15, 512, 385);
}

SouthExit69300::SouthExit69300()
{
	name = "SouthExit69300";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
    
}

void SouthExit69300::respond()
{
	theGame->setCursor(southCursor);
}

int SouthExit69300::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm69000);
    	return true;
	}
	return false;
}

 
} // namespace Spycraft 
