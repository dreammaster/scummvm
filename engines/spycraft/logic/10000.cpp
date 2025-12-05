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
#include "flag.h"
#include "roomsnd.h"
#include "10000.h"
#include "12000.h"
#include "1000.h"

namespace Spycraft {

#define CRSBLINK_SPEED 5

extern Intrface* intrface;

extern SouthCursor* southCursor;
extern NorthCursor* northCursor;
extern DoorCursor* doorCursor;
extern BlinkCursor* normalCursor;

static View*	bkgndView;
static int		knockCount;	//currently irrelevant

/*********************
*
*		Room Code 
*
**********************/

Rm10000::Rm10000()
{
	name = "Rm10000";
	show_style = VE_FLIP;
}

void Rm10000::init()
{
	#if BKSOUND == ON
		if( sound1->number != 10000 )
		{
			sound1->stop();
			sound1->loop = SND_LOOP_INFINITE;
			sound1->playBuffered( 10000, NULL );
		}
	#endif
	#if BKMIDI == ON
		if( theMusic->number != 10000 )
		{
		 	theMusic->stop();
			//theMusic->loop = SND_LOOP_INFINITE;
			//theMusic->play( 10000 ); no such thing
		}
	#endif

  	drawPic( 10000 );
    Room::init();
  	new SouthExit10000;
	new House10000;
	
  	intrface->show();
  	theGame->handsOn();
}

Rm10100::Rm10100()
{
	name = "Rm10100";
	show_style = VE_FLIP;
}

void Rm10100::init()
{
	drawPic( -1 );
	Room::init();
	new SouthExit10100;
	new HouseView10100;
	new Door10100;
	intrface->show();
	theGame->handsOn();
}

Rm10200::Rm10200()
{
	name = "Rm10200";
	show_style = VE_FLIP;
}

void Rm10200::init()
{
	drawPic( 10200 );
	Room::init();
	new SouthExit10200;
	//new DoorView10200;
	new Knocker10200;
	knockCount = 0;
	intrface->show();
	theGame->handsOn();
}

/*********************
*
*	Exit Features 
*
**********************/

// 10000
SouthExit10000::SouthExit10000()
{
	name = "SouthExit10000";
	setHotspot ( DO_V );
	init( SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT );  
}

void SouthExit10000::respond(void)
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit10000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm1000);
    	return TRUE;
	}
	return FALSE;
}

// 10100
SouthExit10100::SouthExit10100()
{
	name = "SouthExit10000";
	setHotspot ( DO_V );
	init( SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT );  
}

void SouthExit10100::respond(void)
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit10100::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm1000);
    	return TRUE;
	}
	return FALSE;
}

// 10200
SouthExit10200::SouthExit10200()
{
	name = "SouthExit10200";
	setHotspot ( DO_V );
	init( SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT );  
}

void SouthExit10200::respond(void)
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit10200::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm10100);
    	return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Features
 *
/*******************************/

House10000::House10000()
{
	name = "House10100";
	setHotspot( DO_V );
	init( 431, 140, 75, 35 );
}

void House10000::respond()
{
	theGame->setCursor( northCursor );
}

int House10000::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		theGame->newRoom( new Rm10100 );
		return TRUE;
	}
	return FALSE;
}

Door10100::Door10100()
{
	name = "Door10100";
	setHotspot( DO_V );
	init( 230, 215, 35, 90 );
}

void Door10100::respond()
{
	theGame->setCursor( doorCursor );
}

int Door10100::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		theGame->newRoom( new Rm10200 );
		return TRUE;
	}
	return FALSE;
}

Knocker10200::Knocker10200()
{
	name = "Knocker10200";
	setHotspot( DO_V );
	init( 115, 50, 100, 140 );
}

void Knocker10200::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Knocker10200::doVerb( int theVerb )
{
	Script*	s_ptr;

	if( theVerb == DO_V )	{
		++knockCount;
		s_ptr = new KnockScript10200;
		s_ptr->init( s_ptr, NULL, NULL );	// bogus client: this, needed for dispose()	
		return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Views
 *
/*******************************/

HouseView10100::HouseView10100() // full screen
{
	name = "HouseView10100";
	//setHotspot( DO_V );
	setView( 10020 );
	init ( 0, 0 );
	noSkip();
	normalCursor->rest();	// stops respond blink
}

// not used
DoorView10200::DoorView10200()  // full screen
{
	name = "DoorView10200";
	//setHotspot( DO_V );
	setView( 10030 );
	init ( 0, 0 );
	noSkip();
	normalCursor->rest();	// stops respond blink
}


/*******************************
 *
 *	Scripts
 *
 *******************************/
  
void KnockScript10200::changeState( int newState )
{
	switchTo
		soundptr = new Sound;
		soundptr->play( SND_COLBYSDOORKNOCK, this );
		user->canInput( FALSE );	// this wipes out multiknock feature
	END

	BEG
		soundptr->dispose();
		if( !GameFlag.test( fColbyMoleBriefing ) && knockCount == 1 )
			theGame->newRoom( new Rm12000);
		--knockCount;
		user->canInput( TRUE );
		dispose();
	END
}
 
} // namespace Spycraft 
