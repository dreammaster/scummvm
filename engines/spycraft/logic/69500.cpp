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
#include "spycraft/logic/69500.h"
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
//extern Print* testPrint;

Rm69500::Rm69500()
{
	name = "Rm69500";
	show_style = VE_FLIP;
}

//Rm69500::~Rm69500()
//{
   //testPrint->erase();
//}

void Rm69500::init()
{
  	drawPic( 69500 );
    Room::init();
  
	new(SouthExit69500);
	new(Menu69500);
	
  	intrface->show();
  	theGame->handsOn();
   //testPrint = new(Print); 
   //testPrint->init("Hotel Schlump Int.");
}

int Rm69500::handleEvent ( MADEEventStamp *event )
{
	return false;
}

SouthExit69500::SouthExit69500()
{
	name = "SouthExit69500";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
}

void SouthExit69500::respond()
{
	theGame->setCursor(southCursor);
}

int SouthExit69500::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm69000);
    	return true;
	}
	return false;
}

/*******************************
 *
 *	Features
 *
/*******************************/

Menu69500::Menu69500()
{
	name = "Menu69500";
	setHotspot( DO_V );
	init( 282, 238, 107, 92 );
}

void Menu69500::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Menu69500::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		curRoom->zoomTo ( 302, 296 );
		new MenuInset69500();
		return true;
	}
	return false;
}

/*******************************
 *
 *	Insets
 *
 *******************************/

MenuInset69500::MenuInset69500()
{
	normalCursor->rest();
 	init(69520, 0, 15, 512, 385);
}
 
} // namespace Spycraft 
