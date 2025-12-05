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
#include "spycraft/logic/48000.h"
#include "spycraft/logic/48400.h"
#include "spycraft/logic/1000.h"
//#include "spycraft/logic/45000.h"
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

Rm48400::Rm48400()
{
	name = "Rm48400";
	show_style = VE_FLIP;
}

Rm48400::~Rm48400()
{
}

void Rm48400::init()
{
  	drawPic( 48400 );
    Room::init();
  
	new(SouthExit48400);
	
  	intrface->show();
  	theGame->handsOn();
}

int Rm48400::handleEvent ( MADEEventStamp *event )
{
	return false;
}

SouthExit48400::SouthExit48400()
{
	name = "SouthExit48400";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
}

void SouthExit48400::respond()
{
	theGame->setCursor(southCursor);
}

int SouthExit48400::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
//    	theGame->newRoom(new Rm45000);
		theGame->newRoom(new Rm1000);
    	return true;
	}
	return false;
}
 
 
} // namespace Spycraft 
