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
#include "1090.h"
#include "80000.h"
#include "1000.h"

namespace Spycraft {

/*********************
*
*		Room Code 
*
**********************/

#define CRSBLINK_SPEED 5

extern Intrface* intrface;
extern NorthCursor* northCursor;
extern SouthCursor* southCursor;
extern EastCursor* eastCursor;
extern BlinkCursor* normalCursor;

static int	playingVoice;	// Flag to prevent room change while Blake is talking.
static int	playingDoorbell;	// Flag to prevent room change while doorbell is ringing.
static int	blakeHasSpoken;	// Used to distinguish which Blake response to use.

/*********************
*
*	Background Sound 
*
**********************/

extern MADEEnvirons* colbyInfo;

enum{ PLAYWAVE, PLAYMIDI, PLAYWAVEMIDI };

static void startSound( int number, int which )
{
	if( which == PLAYWAVEMIDI )
	{
		if( colbyInfo->slow_system )
			which = PLAYMIDI;
		else
			which = PLAYWAVE;
	}
	#if BKSOUND == ON
		if( which == PLAYWAVE )
		{
			if( sound1->number != number  || !sound1->isPlaying() )
			{	
				sound1->stop();
				sound1->loop = SND_LOOP_INFINITE;
				sound1->playBuffered( number, NULL );
			}
		}
	#endif
	#if BKMIDI == ON
		if( which == PLAYMIDI )
		{
			if( theMusic->number != number  || !theMusic->isPlaying() )
			{
			 	theMusic->stop();
				theMusic->play( number );
			}
		}
	#endif
}

/******************
*
*	Room 
*
*******************/

Rm1090::Rm1090()
{
	name = "Rm1090";
	show_style = VE_FLIP;
}

void Rm1090::init()
{
  	drawPic( 1090 );
    Room::init();
	#pragma message( "preloading next room tga" )
	sfxSystemCursor( SYS_WAIT );
	sfxLoadRes( 80000, RES_PIC );
	theGame->handsOn(); 
	new NorthExit1090;
	new SouthExit1090;
	//if( blakeHasSpoken )
	//	new EastExit1090;
	blakeHasSpoken = FALSE;
	
  	intrface->show();
  	theGame->handsOn();

 	startSound( 1090, PLAYWAVEMIDI );
 }

Rm1091::Rm1091()
{
	name = "Rm1091";
	show_style = VE_FLIP;
}

void Rm1091::init()
{
	drawPic( 1091 );
	Room::init();
	new SouthExit1091;
	new DoorBell_1091;
	if( blakeHasSpoken )
		new EastExit1091;
	intrface->show();
	theGame->handsOn();
}

/*********************
*
*	Exit Features 
*
**********************/

// 1090
NorthExit1090::NorthExit1090()
{
	name = "NorthExit1090";
	setHotspot( DO_V );
	init( 220, 145, 40, 95 );  
}

void NorthExit1090::respond()
{
	theGame->setCursor( northCursor );
	aniCursor();
}

int NorthExit1090::doVerb ( int theVerb )
{
	if( theVerb == DO_V ) {
    	theGame->newRoom( new Rm1091 );
    	return TRUE;
	}
	return FALSE;
}

SouthExit1090::SouthExit1090()
{
	name = "SouthExit1090";
	setHotspot( DO_V );
	init( SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT );  
}

void SouthExit1090::respond()
{
	theGame->setCursor( southCursor );
	aniCursor();
}

int SouthExit1090::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
		theGame->newRoom( new Rm1000 );
    	return TRUE;
	}
	return FALSE;
}

// no longer used
EastExit1090::EastExit1090()
{
	name = "EastExit1090";
	setHotspot ( DO_V );
	init ( EAST_EXIT_X, EAST_EXIT_Y, EAST_EXIT_WIDTH, EAST_EXIT_HEIGHT );
}
// no longer used
void EastExit1090::respond()
{
	if( !playingVoice && !playingDoorbell )
		theGame->setCursor(eastCursor);
}
// no longer used
int EastExit1090::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		if( !playingVoice  && !playingDoorbell )
    		theGame->newRoom( new Rm80000 );
    	return TRUE;
	}
	return FALSE;
}

// 1091
SouthExit1091::SouthExit1091()
{
	name = "SouthExit1091";
	setHotspot( DO_V );
	init( SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT );  
}

void SouthExit1091::respond()
{
	if( !playingVoice && !playingDoorbell )	
	{
		theGame->setCursor( southCursor );
		aniCursor();
	}
}

int SouthExit1091::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
		if( !playingVoice && !playingDoorbell )
    		theGame->newRoom( new Rm1090 );
    	return TRUE;
	}
	return FALSE;
}

EastExit1091::EastExit1091()
{
	name = "EastExit1091";
	setHotspot ( DO_V );
	init ( 400, EAST_EXIT_Y, 111, EAST_EXIT_HEIGHT );
}

void EastExit1091::respond()
{
	if( !playingVoice && !playingDoorbell )
	{
		theGame->setCursor(eastCursor);
		aniCursor();
	}
}

int EastExit1091::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		if( !playingVoice && !playingDoorbell )
    		theGame->newRoom( new Rm80000 );
    	return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Features
 *
/*******************************/

DoorBell_1091::DoorBell_1091()
{
	name = "DoorBell_1091";
	setHotspot( DO_V );
	init( 115, 36, 200, 270 );
}

void DoorBell_1091::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int DoorBell_1091::doVerb( int theVerb )
{
	if( theVerb == DO_V )
	{
		if( !playingDoorbell )
			setScript( new DoorbellScript1091 );
		if( !playingVoice  && !GameFlag.test( fBlakePatioTalkDone ) )
		{
			if( !blakeHasSpoken )
			{
				curRoom->setScript( new HelloScript1091 );
				new EastExit1091;
			}
			else
				curRoom->setScript( new ISaidHelloScript1091 );
		}
		return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Scripts
 *
/*******************************/

void DoorbellScript1091::changeState( int newState )
{
	static Sound*	soundptr;

	switchTo
		playingDoorbell = TRUE;
		soundptr = new Sound;
		soundptr->play( SND_BLAKESDOORBELL, this );
	END

	BEG
		soundptr->dispose();
		dispose();
		playingDoorbell = FALSE;
	END
}

void HelloScript1091::changeState( int newState )
{
	static Sound*	soundptr;

	switchTo
		playingVoice = TRUE;
		ticks = 100;
	END

	BEG		
		soundptr = new Sound;
		soundptr->play( SND_VO_BLAKEAROUNDBACK1, this );
	END

	BEG
		soundptr->dispose;
		dispose();
		playingVoice = FALSE;
		blakeHasSpoken = TRUE;						 
	END
}

void ISaidHelloScript1091::changeState( int newState )
{
	static Sound*	soundptr;
	static int		annoyed;	// flag

	switchTo
		playingVoice = TRUE;
		ticks = 40;
	END

	BEG		
		soundptr = new Sound;
		if( !annoyed )
		{
			soundptr->play( SND_VO_BLAKEAROUNDBACK2, this );
			annoyed = TRUE;
		}
		else
			soundptr->play( SND_VO_BLAKEAROUNDBACK3, this );
	END

	BEG
		soundptr->dispose;
		dispose();
		playingVoice = FALSE;
	END
}

 
} // namespace Spycraft 
