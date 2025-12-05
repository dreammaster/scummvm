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
#include "23000.h"
#include "23100.h"
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


Rm23100::Rm23100()
{
	name = "Rm23100";
	show_style = VE_FLIP;
}

//Rm23100::~Rm23100()
//{
   //testPrint->erase();
//}

void Rm23100::init()
{
  	drawPic( 23100 );
    Room::init();
  
	new(SouthExit23100);
	new(Microphone23100);
	new(TapePlayer23100);
	new(PolygraphFinger23100);
//	new(PolygraphRead23100);
	
  	intrface->show();
  	theGame->handsOn();
}

int Rm23100::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}

SouthExit23100::SouthExit23100()
{
	name = "SouthExit23100";
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
    setHotspot ( DO_V );
}

void SouthExit23100::respond()
{
	theGame->setCursor(southCursor);
}

int SouthExit23100::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm23000);
    	return TRUE;
	}
	return FALSE;
}
  
/*******************************
 *
 *	Features
 *
/*******************************/

Microphone23100::Microphone23100()
{
	name = "Microphone23100";
	setHotspot( DO_V );
	init( 130, 59, 68, 54 );
}

void Microphone23100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Microphone23100::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		new MicrophoneInset23100();
		return TRUE;
	}
	return FALSE;
}

TapePlayer23100::TapePlayer23100()
{
	name = "TapePlayer23100";
	setHotspot( DO_V );
	init( 58, 129, 107, 76 );
}

void TapePlayer23100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int TapePlayer23100::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		new TapePlayerInset23100();
		return TRUE;
	}
	return FALSE;
}

PolygraphFinger23100::PolygraphFinger23100()
{
	name = "PolygraphFinger23100";
	setHotspot( DO_V );
	init( 204, 175, 55, 113 );
}

void PolygraphFinger23100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int PolygraphFinger23100::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		new PolygraphFingerInset23100();
		return TRUE;
	}
	return FALSE;
}
/*
PolygraphRead23100::PolygraphRead23100()
{
	name = "PolygraphRead23100";
	setHotspot( DO_V );
	init( 53, 247, 81, 43 );
// put in the correct number, no picture to look at
}

void PolygraphRead23100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int PolygraphRead23100::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		new PolygraphReadInset23100();
		return TRUE;
	}
	return FALSE;
}
*/
/*******************************
 *
 *	Insets
 *
 *******************************/

MicrophoneInset23100::MicrophoneInset23100()
{
	normalCursor->rest();
 	init(23120, 0, 15, 512, 385);
}


TapePlayerInset23100::TapePlayerInset23100()
{
	normalCursor->rest();
 	init(23140, 0, 15, 512, 385);
}

PolygraphFingerInset23100::PolygraphFingerInset23100()
{
	normalCursor->rest();
 	init(23160, 0, 15, 512, 385);
}
/*
PolygraphReadInset23100::PolygraphReadInset23100()
{
	normalCursor->rest();
 	init(23180, 0, 15, 512, 385);
}
*/ 
 
} // namespace Spycraft 
