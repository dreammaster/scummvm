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
#include "56000.h"
#include "56100.h"
#include "57000.h"
#include "1000.h"		 
#include "flag.h"
#include "invent.h"
#include "sound.h"

namespace Spycraft {

/*********************
*
*		Room Code 
*
**********************/

extern Intrface* intrface;
extern Invent* inventry;
extern Flag GameFlag;
extern NorthCursor* northCursor;
extern SouthCursor* southCursor;

Rm56000::Rm56000()
{
	name = "Rm56000";
	show_style = VE_FLIP;
}

Rm56000::~Rm56000()
{
}

void Rm56000::init()
{
////if( !GameFlag.test( fMetBirdSong) ) 
//// 		drawPic( 56300 );				  	//day time hallway
////else
		drawPic( 56000 );				  		//night time hallway
  	
    Room::init();

  	new(NorthExit56000);
	new(SouthExit56000);


  	intrface->show();
  	theGame->handsOn();

	sfxLoadRes( 56100, RES_PIC );
	sfxLoadRes( 56120, RES_PIC );
}

int Rm56000::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}

NorthExit56000::NorthExit56000()
{
	name = "NorthExit56000";
	setHotspot ( DO_V );
	if( !GameFlag.test( fMetBirdSong) ) 
  		init ( 140, 40, 172, 316 );
	else
		init ( 214, 134, 71, 161 );
}

void NorthExit56000::respond()
{
	theGame->setCursor(northCursor);
	aniCursor();
}

int NorthExit56000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
//    	theGame->newRoom(new Rm56100);
		theGame->handsOff();
		theGame->newRoom(new Rm56001);
    	return TRUE;
	}
	return FALSE;
}

SouthExit56000::SouthExit56000()
{
	name = "SouthExit56000";
	setHotspot( DO_V );
	init( SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT );  
}

void SouthExit56000::respond()
{
	theGame->setCursor( southCursor );
	aniCursor();
}

int SouthExit56000::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
    	theGame->newRoom( new Rm1000 );
    	return TRUE;
	}
	return FALSE;
}

/* Room 56001 */
Rm56001::Rm56001()
{
	name = "Rm56001";
	show_style = VE_FLIP;
}

Rm56001::~Rm56001()
{
	sound1->stop();
}

void Rm56001::init()
{
// 	drawPic( 1 );
	if( !GameFlag.test( fMetBirdSong) ) 
		drawPic( 56300 );
	
	Room::init();

  	intrface->show();
// 	theGame->handsOn();

	setScript(new EnterBlackScreenScript56001);
}

int Rm56001::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}

EnterBlackScreenScript56001::EnterBlackScreenScript56001()
{
	name = "EnterBlackScreenScript56001"; 
}

void EnterBlackScreenScript56001::changeState( int newState )
{
//  GameFlag.set(fBSDoorUnlocked);	
//	theGame->newRoom( new Rm57000 );
	switchTo
		if( !GameFlag.test( fMetBirdSong) ) {
			GameFlag.set( fBSDoorUnlocked);

			if( sound1->number != 56100  || !sound1->isPlaying()) {
//				sound1->number = 56100;			//knock knock
				sound1->play(56100, this);
			}
		}
		else {
			GameFlag.clear( fBSDoorUnlocked );
			theGame->newRoom( new Rm56100 );
		}
	END
	BEG
		ticks = 2;
	END
	BEG

		theGame->newRoom( new Rm57000 );
	END
}
 
} // namespace Spycraft 
