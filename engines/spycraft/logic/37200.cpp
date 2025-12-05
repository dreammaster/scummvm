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
#include "spycraft/logic/37000.h"
#include "spycraft/logic/1000.h"
#include "spycraft/logic/37200.h"
#include "spycraft/logic/37300.h"
#include "flag.h"
#include "movie.h"

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
extern WestCursor* westCursor;

Rm37200::Rm37200()
{
	name = "Rm37200";
	show_style = VE_FLIP;
}

Rm37200::~Rm37200()
{
}

void Rm37200::init()
{  
    Room::init();
  
	new(SouthExit37200);
	new(WestExit37200);
	
  	intrface->show();
///	theGame->handsOn();

	if( !GameFlag.test( fBSTellStory )) {	   
//		drawPic( 1 );                          
//		drawPic( 37200 );
// 		setScript( new EnterScript37200 );
///		setScript( new EnterBlackScreenScript37201);
		theGame->handsOff();
		if (GameFlag.test(fStickPath))
			setScript( new EnterStickScript37200 );
//		else if (GameFlag.test(fCarrotPath))
		else
			setScript( new EnterCarrotScript37200 );
	}
	else{
 	  	drawPic( 37200 );
		theGame->handsOn();
  	}
}

int Rm37200::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}

SouthExit37200::SouthExit37200()
{
	name = "SouthExit37200";
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
    setHotspot ( DO_V );
}

void SouthExit37200::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit37200::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
//		theGame->newRoom(new Rm37000);
		theGame->newRoom(new Rm1000);
    	return TRUE;
	}
	return FALSE;
}

WestExit37200::WestExit37200()
{
	name = "WestExit37200";
	init(WEST_EXIT_X, WEST_EXIT_Y, WEST_EXIT_WIDTH, WEST_EXIT_HEIGHT);
    setHotspot ( DO_V );
}

void WestExit37200::respond()
{
	theGame->setCursor(westCursor);
	aniCursor();
}

int WestExit37200::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm37300);
    	return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Scripts
 *
 *******************************/
EnterStickScript37200::EnterStickScript37200()
{
	name = "EnterStickScript37200"; 
}

void EnterStickScript37200::changeState( int newState )
{
	switchTo
		GameFlag.set(fBSTellStory);  
//		GameFlag.clear(fDisplayKnee); 
		ticks = 2;
	END
	BEG
		theMovie->play("37200.avi", this, 0, 1, 0);	  //stick - betray Max
	END
	BEG
		ticks = 2;
	END
	BEG
		theGame->newRoom( new Rm1000 );
	END
}

EnterCarrotScript37200::EnterCarrotScript37200()
{
	name = "EnterCarrotScript37200"; 
}

void EnterCarrotScript37200::changeState( int newState )
{
	switchTo
		GameFlag.set(fBSTellStory);
		ticks = 2;
	END
	BEG
		theMovie->play("37201.avi", this, 0, 1, 0);	  //carrot path
	END
	BEG
		ticks = 2;
	END
	BEG
		theGame->newRoom( new Rm1000);
	END
}

/* Room 37201 */
Rm37201::Rm37201()
{
	name = "Rm37201";
	show_style = VE_FLIP;
}

Rm37201::~Rm37201()
{
}

void Rm37201::init()
{
	drawPic ( 1 );
    Room::init();
  
  	intrface->show();
  	theGame->handsOn();

	if (GameFlag.test(fStickPath))
		setScript( new EnterStickScript37200 );
	else if (GameFlag.test(fCarrotPath))
		setScript( new EnterCarrotScript37200 );

}

int Rm37201::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}

EnterBlackScreenScript37201::EnterBlackScreenScript37201()
{
	name = "EnterBlackScreenScript37201"; 
}

void EnterBlackScreenScript37201::changeState( int newState )
{
   	GameFlag.set( fBSTellStory );		
	theGame->newRoom( new Rm37201 );
}
 
} // namespace Spycraft 
