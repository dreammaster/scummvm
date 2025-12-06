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
#include "spycraft/logic/53000.h"
#include "spycraft/game/flag.h"
#include "spycraft/game/movie.h"
#include "spycraft/game/vlink.h"
#include "spycraft/logic/1000.h"

namespace Spycraft {

#define CRSBLINK_SPEED 5

/*********************
*
*		Room Code 
*
**********************/

extern Intrface* intrface;
extern BlinkCursor* normalCursor;

Rm53000::Rm53000()
{
	name = "Rm53000";
	show_style = VE_FLIP;
}

Rm53000::~Rm53000()
{
}

void Rm53000::init()
{
  	drawPic( 1 );
    Room::init();

  	intrface->show();
///	theGame->handsOn();
  
	if( !GameFlag.test( fVCRoomEnter ))	   
		setScript( new EnterScript53000 );
}

int Rm53000::handleEvent ( MADEEventStamp *event )
{
	return false;
}

/*******************************
 *
 *	Scripts
 *
/*******************************/
EnterScript53000::EnterScript53000()
{
	name = "EnterScript53000"; 
}

void EnterScript53000::changeState( int newState )
{
	switchTo
		GameFlag.set(fVCRoomEnter);
///		intrface->disable();
///		theGame->handsOff();
		theMovie->play( "53000.avi", this, 0, 1, 1);
	END
	BEG
///		theGame->handsOn();
///		dispose();
		theGame->newRoom(new Rm1000);
//		new VLink( "Birdsong blew the cage: Foster, Maxine M.", "53001.avi", 0, this );	
	END
}
 
} // namespace Spycraft 
