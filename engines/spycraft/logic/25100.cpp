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
#include "spycraft/logic/25000.h"
#include "spycraft/logic/25100.h"
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
extern WestCursor* westCursor;

Rm25100::Rm25100()
{
	name = "Rm25100";
	show_style = VE_FLIP;
}

Rm25100::~Rm25100()
{
}

void Rm25100::init()
{
  	drawPic( 25100 );
    Room::init();
  
	new(WestExit25100);
	
  	intrface->show();
  	theGame->handsOn();
}

int Rm25100::handleEvent ( MADEEventStamp *event )
{

	return false;
}

WestExit25100::WestExit25100()
{
	name = "WestExit25100";
    setHotspot ( DO_V );
   	init(0,0,30,330);
}

void WestExit25100::respond()
{
	theGame->setCursor(westCursor);
	aniCursor();
}

int WestExit25100::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm25000);
    	return true;
	}
	return false;
}
 
} // namespace Spycraft 
