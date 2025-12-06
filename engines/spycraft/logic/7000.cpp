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
#include "spycraft/game/invent.h"
#include "spycraft/game/flag.h"
#include "spycraft/game/movie.h"
#include "spycraft/logic/7000.h"
#include "spycraft/logic/2000.h"
#include "spycraft/logic/95000.h"
#include "spycraft/logic/95100.h"
#include "spycraft/logic/1000.h"
#include "spycraft/game/invent.h"
#include "spycraft/game/roomsnd.h"

namespace Spycraft {

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
*		Room Code 
*
**********************/

extern Intrface* intrface;

extern SouthCursor* southCursor;
extern NorthCursor* northCursor;

Rm7000::Rm7000()
{
	name = "Rm7000";
	show_style = VE_FLIP;
}

void Rm7000::init()
{
	int	shootersville = false;	 // somewhere in the shooters, their intros or outros

	#ifdef _DEBUG
		if( !GameFlag.test( fFarmIADone ) )
			sfxPrintf( "image analysis not completed, flag %d", fFarmIADone );
	#endif
	// in shootersville
	if( GameFlag.test( fFarmIADone ) && !GameFlag.test( fFarmCODone ) )
		shootersville = true;

	if( shootersville )
		drawPic( -1 );
	else
	{
  		drawPic( 7000 );
		new SouthExit7000;
		#pragma message( "midi missing" )
		startSound( 7000, PLAYWAVE );
	}
	
    Room::init();

  	intrface->show();
  	theGame->handsOn();

	// do a shooter
	if( shootersville )
	{
		#if BKSOUND == ON
			sound1->stop();	// needed for teleport
		#endif
		//sfxPrintf( "Do scripts" );
		if(!GameFlag.test( fFarmWPDone )) 
			setScript( new WPScript7000 );	// do weapons proficiency
		else if(!GameFlag.test( fFarmCODone )) 
			setScript( new COScript7000 );	// do crisis orienteering
	}
}

/*********************
*
*	Exit Features 
*
**********************/

SouthExit7000::SouthExit7000()
{
	name = "SouthExit7000";
	setHotspot( DO_V );
	init( SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT );  
}

void SouthExit7000::respond()
{
	theGame->setCursor( southCursor );
	aniCursor();
}

int SouthExit7000::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
    	theGame->newRoom( new Rm1000 );
    	return true;
	}
	return false;
}

/*******************************
 *
 *	Scripts
 *
/*******************************/

void WPScript7000::changeState( int newState )
{
	switchTo
		theGame->handsOff();
		ticks = 120;
	END
	BEG

		// Combat Results
		if( GameFlag.test( fFarmDoingWP ) )
		{
			// Get Killed
			if( GameFlag.test( fGotKilled ) )
				theMovie->play( "54.avi", this );	//weapons proficiency failed
			// Succeed
			else
			{
				GameFlag.set( fFarmWPDone );
				if( GameFlag.test( fWPSqueekBy ) )
					theMovie->play( "52.avi", this );	//weapons proficiency squeak by
				else
					cue(); // the intro to CO
			}
			GameFlag.clear( fFarmDoingWP );
		}
		else // Intro
			theMovie->play( "50.avi", this );	//weapons proficiency intro
	END

	BEG	
		// go to Crisis Orienteering
		if( GameFlag.test( fFarmWPDone ) )
			curRoom->setScript( new COScript7000 );
		else // do Weapons Proficiency
		{
			GameFlag.set( fFarmDoingWP );
			GameFlag.set( fStartCombat );
			theGame->newRoom( new Rm95018 );
		}
	END
}

void COScript7000::changeState( int newState )
{
	switchTo
		theGame->handsOff();
		ticks = 120;
	END
	BEG
		// Combat results
		if( GameFlag.test( fFarmDoingCO ) )
		{
			// Failure
			if( GameFlag.test( fGotKilled) )
			{
				if( GameFlag.test( fGotBeacon ) )
					theMovie->play("66.avi", this);	// CO died w/beacon
				else
					theMovie->play("65.avi", this);	// CO no beacon
			}
			else // Success
			{
				GameFlag.set(fFarmCODone); // note: can't get out alive w/o beacon
				theMovie->play("67.avi", this);	 // CO win--Jeffries dies
			}
			GameFlag.clear( fFarmDoingCO );
		}
		else // WP success and CO intro
		{
			if( GameFlag.test( fWPSqueekBy ) )
			{
			 theMovie->fromTo( 60, 496 );	// skip you did well part
			 theMovie->play("5164.avi", this); // Crisis orienteering intro
			}
			else
				theMovie->play("5164.avi", this); // Crisis orienteering intro	
		}
	END

	BEG
		// finished shooters
		if(GameFlag.test( fFarmCODone ))
			theGame->newRoom( new Rm2000 );
		else  // do Crisis Orienteering
		{
			GameFlag.set( fFarmDoingCO );
			GameFlag.set( fStartCombat );
			theGame->newRoom( new Rm95118 );
		}
	END
}

 
} // namespace Spycraft 
