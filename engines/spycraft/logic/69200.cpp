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
#include "spycraft/logic/69000.h"
#include "spycraft/logic/69200.h"
#include "spycraft/game/flag.h"
#include "spycraft/game/roomsnd.h"
#include "spycraft/game/sound.h"

namespace Spycraft {

#define CRSBLINK_SPEED 5

/*********************
*
*		Room Code 
*
**********************/

extern Intrface* intrface;
extern BlinkCursor* normalCursor;
extern WestCursor* westCursor;
//extern Print* testPrint;

Rm69200::Rm69200()
{
	name = "Rm69200";
	show_style = VE_FLIP;
}

//Rm69200::~Rm69200()
//{
   //testPrint->erase();
//}

void Rm69200::init()
{	
  	drawPic( 69200 );
    Room::init();

  	if (GameFlag.test(fGotHandcuffs))
		new ( Cuffs69000 );
	new(WestExit69200);
//	new(Doll69200);
	new(Card69200);
	
  	intrface->show();
  	theGame->handsOn();

   //testPrint = new(Print); 
   //testPrint->init("Hotel Schlump Int.");
}

int Rm69200::handleEvent ( MADEEventStamp *event )
{
//	if (event_type == USER_LEFT_UP)	{
//		new(DollInset69200);
//	}	

	return false;
}

/*******************************
 *
 *	Features
 *
/*******************************/

Doll69200::Doll69200()
{
	name = "Doll69200";
	setHotspot( DO_V );
	init( 191, 181, 61, 147 );
}

void Doll69200::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Doll69200::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		curRoom->zoomTo ( 284, 262 );
	//	curRoom->zoomTo ( 241, 259 );
		new DollInset69200();
		return true;
	}
	return false;
}


Card69200::Card69200()
{
	name = "Card69200";
	setHotspot( DO_V );
	init( 53, 247, 81, 43 );
}

void Card69200::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Card69200::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		curRoom->zoomTo ( 138, 272 );
		new CardInset69200();
		return true;
	}
	return false;
}

/*******************************
 *
 *	Insets
 *
 *******************************/

DollInset69200::DollInset69200()
{
	normalCursor->rest();
 	init(69220, 0, 15, 512, 385);
}

CardInset69200::CardInset69200()
{
	normalCursor->rest();
 	init(69240, 0, 15, 512, 385);
}

WestExit69200::WestExit69200()
{
	name = "WestExit69200";
    setHotspot ( DO_V );
	init(WEST_EXIT_X, WEST_EXIT_Y, WEST_EXIT_WIDTH, WEST_EXIT_HEIGHT);
}

void WestExit69200::respond()
{
	theGame->setCursor(westCursor);
	aniCursor();
}

int WestExit69200::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm69002);
		return true;
	}
	return false;
}

 
} // namespace Spycraft 
