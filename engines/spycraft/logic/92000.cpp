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
#include "spycraft/logic/92000.h"

namespace Spycraft {

/*********************
*
*		Room Code 
*
**********************/

extern Intrface* intrface;

//extern SouthCursor* southCursor;

extern Print* testPrint;

Rm92000::Rm92000()
{
	name = "Rm92000";
	show_style = VE_FLIP;
}

Rm92000::~Rm92000()
{
   testPrint->erase();
}

void Rm92000::init()
{
  	drawPic( 1 );
    Room::init();

 	intrface->show();
  	theGame->handsOn();
	testPrint = new(Print); 
	testPrint->init("Ext. Tropical Desert ");
	curRoom->setScript(new Script92000);
}

int Rm92000::handleEvent ( MADEEventStamp *event )
{
	return false;
}

/*******************************
 *
 *	Scripts
 *
 *******************************/
void Script92000::changeState ( int newState )
{
	switchTo
		seconds = 2;
	END
   
    BEG
//		theMovie->play("?????.avi", 0, 0, 640, 480, this);
		sfxPrintf("Welcome to the jungle (j4/?????) Hello, thorn...");
		seconds = 2;
	END

	BEG
		sfxPrintf("crisis orienteering");	//pat interactive
 		dispose();
	END

}

 
} // namespace Spycraft 
