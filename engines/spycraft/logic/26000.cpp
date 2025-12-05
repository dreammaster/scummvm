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
#include "roomsnd.h"
#include "26000.h"
#include "1000.h"
#include "web.h"

namespace Spycraft {

#define CRSBLINK_SPEED 5

extern Intrface* intrface;
extern SouthCursor* southCursor;
extern BlinkCursor* normalCursor;

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

/*********************
*
*	Room 
*
**********************/

Rm26000::Rm26000()
{
	name = "Rm26000";
	show_style = VE_FLIP;
}

void Rm26000::init()
{
	drawPic( 26000 );
	Room::init();
	startSound( 26000, PLAYWAVEMIDI );
	new SouthExit26000;
	new Desk26000;
	new Photos26000;
	new Map26000;
	intrface->show();
	theGame->handsOn();
}

Rm26100::Rm26100()
{
	name = "Rm26100";
	show_style = VE_FLIP;
}

void Rm26100::init()
{
	drawPic( 26100 );
	Room::init();
	startSound( 26000, PLAYWAVEMIDI );
	new SouthExit26100;
	new TravelLetter26100;
	new Folders26100;
	intrface->show();
	theGame->handsOn();
}

/*********************
*
*	Exit Features 
*
**********************/

SouthExit26000::SouthExit26000()
{
	name = "SouthExit26000";
	setHotspot( DO_V );
	init( SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT );  
}

void SouthExit26000::respond()
{
	theGame->setCursor( southCursor );
	aniCursor();
}

int SouthExit26000::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
    	theGame->newRoom( new Rm1000 );
    	return TRUE;								 
	}
	return FALSE;
}

SouthExit26100::SouthExit26100()
{
	name = "SouthExit26100";
	setHotspot( DO_V );
	init( SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT );  
}

void SouthExit26100::respond()
{
	theGame->setCursor( southCursor );
	aniCursor();
}

int SouthExit26100::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
    	theGame->newRoom( new Rm26000 );
    	return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Features
 *
/*******************************/

Desk26000::Desk26000()
{
	name = "Desk26000";
	setHotspot( DO_V );
	init( 166, 216, 200, 115 );
}

void Desk26000::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Desk26000::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		theGame->newRoom( new Rm26100 );
		return TRUE;
	}
	return FALSE;
}

Photos26000::Photos26000()
{
	name = "Photos26000";
	setHotspot( DO_V );
	init( 401, 80, 60, 75 );
}

void Photos26000::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Photos26000::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		new PhotosInset26000;
		return TRUE;
	}
	return FALSE;
}

Map26000::Map26000()
{
	name = "Map26000";
	setHotspot( DO_V );
	init( 246, 75, 130, 95 );
}

void Map26000::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Map26000::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		new MapInset26000;
		return TRUE;
	}
	return FALSE;
}

TravelLetter26100::TravelLetter26100()
{
	name = "TravelLetter26100";
	setHotspot( DO_V );
	init ( 2, 75, 130, 75 );
}

void TravelLetter26100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int TravelLetter26100::doVerb( int theVerb )
{
	if ( theVerb == DO_V ) {
		new TravelLetterInset26000;
		return TRUE;
	}
	return FALSE;
}

Folders26100::Folders26100()
{
	name = "Folders26100";
	setHotspot( DO_V );
	init ( 220, 255, 108, 75 );
}

void Folders26100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Folders26100::doVerb( int theVerb )
{
	if ( theVerb == DO_V ) {
		curRoom->setScript( new FolderSndScript26100 );
		new Yellow( 98635 );	// may want to move this into script
		return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Insets
 *
/*******************************/

MapInset26000::MapInset26000()
{
	init( 26040, 0, 16, 512, 384 );
	normalCursor->rest();	// stops respond blink
}

PhotosInset26000::PhotosInset26000()
{
	init( 26020, 0, 16, 512, 384 );
	normalCursor->rest();	// stops respond blink
	new MelnachovIFeature26000;
}

MelnachovInset26000::MelnachovInset26000()
{
	init( 26060, 0, 16, 512, 384 );
	normalCursor->rest();	// stops respond blink
}

void MelnachovInset26000::dispose( void )
{
 	Inset::dispose();
	new PhotosInset26000;
}

TravelLetterInset26000::TravelLetterInset26000()
{
	init( 26120, 0, 16, 512, 384 );
	normalCursor->rest();	// stops respond blink
}

/*******************************
 *
 *	Inset Features
 *
/*******************************/

MelnachovIFeature26000::MelnachovIFeature26000()
{
 	name = "MelnachovIFeature26000";
	setHotspot( DO_V );
	init( 218, 160 +16, 72, 88 );
}

void MelnachovIFeature26000::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int MelnachovIFeature26000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		theInset->dispose();
		new MelnachovInset26000;
		return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Scripts
 *
/*******************************/

void FolderSndScript26100::changeState( int newState )
{
	static Sound*	sndptr;

 	switchTo
		sndptr = new Sound;
		sndptr->play( 26010 , this );	// picking up and opening folders
	END

	BEG
		sndptr->dispose();
	END
}
 
} // namespace Spycraft 
