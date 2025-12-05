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
#include "37200.h"
#include "37300.h"
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
extern EastCursor* eastCursor;
//extern SouthCursor* southCursor;
//extern Print* testPrint;

Rm37300::Rm37300()
{
	name = "Rm37300";
	show_style = VE_FLIP;
}

Rm37300::~Rm37300()
{
   //testPrint->erase();
}

void Rm37300::init()
{	
  	drawPic( 37300 );

    Room::init();
  
//	new(SouthExit37300);
	new(EastExit37300);
	new(EgyptianPhotos37300);
	
  	intrface->show();
  	theGame->handsOn();
}

int Rm37300::handleEvent ( MADEEventStamp *event )
{

	return FALSE;
}
EastExit37300::EastExit37300()
{
	name = "EastExit37300";
	setHotspot ( DO_V );
	init(EAST_EXIT_X, EAST_EXIT_Y, EAST_EXIT_WIDTH, EAST_EXIT_HEIGHT);
}

void EastExit37300::respond()
{
	theGame->setCursor(eastCursor);
	aniCursor();
}

int EastExit37300::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm37200);
    	return TRUE;
	}
	return FALSE;
}
/*
SouthExit37300::SouthExit37300()
{
	name = "SouthExit37300";
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
//	init(0,450,600,480);
    setHotspot ( DO_V );
}

void SouthExit37300::respond()
{
	theGame->setCursor(southCursor);
}

int SouthExit37300::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm37200);
    	return TRUE;
	}
	return FALSE;
}
*/

/*******************************
 *
 *	Features
 *
/*******************************/
EgyptianPhotos37300::EgyptianPhotos37300()
{
	name = "EgyptianPhotos37300";
	setHotspot( DO_V );
	init( 197, 58, 155, 143 );
}

void EgyptianPhotos37300::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int EgyptianPhotos37300::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		new EgyptianPhotosInset37300();
		return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Insets
 *
 *******************************/

EgyptianPhotosInset37300::EgyptianPhotosInset37300()
{
	normalCursor->rest();
 	init(37320, 0, 15, 512, 385);
}
 
 
} // namespace Spycraft 
