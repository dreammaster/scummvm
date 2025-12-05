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
#include "69000.h"
#include "69400.h"
#include "flag.h"

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

Rm69400::Rm69400()
{
	name = "Rm69400";
	show_style = VE_FLIP;
}

//Rm69400::~Rm69400()
//{
   //testPrint->erase();
//}

void Rm69400::init()
{
  	drawPic( 69400 );
    Room::init();
  
	new(SouthExit69400);
	new(Bond69400);
	
  	intrface->show();
  	theGame->handsOn();

   //testPrint = new(Print); 
   //testPrint->init("Hotel Schlump Int.");
}

int Rm69400::handleEvent ( MADEEventStamp *event )
{

	return FALSE;
}

SouthExit69400::SouthExit69400()
{
	name = "SouthExit69400";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
}

void SouthExit69400::respond()
{
	theGame->setCursor(southCursor);
}

int SouthExit69400::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm69000);
    	return TRUE;
	}
	return FALSE;
}


/*******************************
 *
 *	Features
 *
/*******************************/

Bond69400::Bond69400()
{
	name = "Bond69400";
	setHotspot( DO_V );
	init( 38, 78, 362, 323 );
}

void Bond69400::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Bond69400::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
//		curRoom->zoomTo ( 220, 211 );
		new BondInset69400();
		return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Insets
 *
 *******************************/

BondInset69400::BondInset69400()
{
	normalCursor->rest();
 	init(69420, 0, 15, 512, 385);
}

void BondInset69400::respond()
{
}

int BondInset69400::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
		dispose();
		new BondInset69420();
    	return TRUE;
	}
	return FALSE;
}

BondInset69420::BondInset69420()
{
	normalCursor->rest();
 	init(69440, 0, 15, 512, 385);
}
 
} // namespace Spycraft 
