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
#include "spycraft/logic/32200.h"
#include "spycraft/logic/32300.h"
#include "spycraft/logic/32100.h"

namespace Spycraft {

#define CRSBLINK_SPEED 5

extern Intrface* intrface;
extern NorthCursor* northCursor;
extern SouthCursor* southCursor;
extern BlinkCursor* normalCursor;

/*********************
*
*	Room 
*
**********************/

Rm32200::Rm32200()
{
	name = "Rm32200";
	show_style = VE_FLIP;
}

void Rm32200::init()
{
	drawPic( 32200 );
	Room::init();
	new NorthExit32200;
	new SouthExit32200;
	intrface->show();
	theGame->handsOn();
}

/*********************
*
*	Exit Features 
*
**********************/
NorthExit32200::NorthExit32200()
{
	name = "NorthExit32200";
	setHotspot( DO_V );
	init(196, 142, 95, 183);
//	init(NORTH_EXIT_X, NORTH_EXIT_Y, NORTH_EXIT_WIDTH, NORTH_EXIT_HEIGHT);  
}

void NorthExit32200::respond()
{
	theGame->setCursor( northCursor );
	aniCursor();
}

int NorthExit32200::doVerb ( int theVerb )
{
	if( theVerb == DO_V ) {
    	theGame->newRoom( new Rm32300 );
    	return true;
	}
	return false;
}

SouthExit32200::SouthExit32200()
{
	name = "SouthExit32200";
	setHotspot( DO_V );
	init( SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT );  
}

void SouthExit32200::respond()
{
	theGame->setCursor( southCursor );
	aniCursor();
}

int SouthExit32200::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
    	theGame->newRoom( new Rm32100 );
    	return true;
	}
	return false;
}
 
} // namespace Spycraft 
