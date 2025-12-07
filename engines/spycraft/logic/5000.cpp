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
#include "spycraft/game/movie.h"
#include "spycraft/game/flag.h"
#include "spycraft/game/roomsnd.h"
#include "spycraft/game/web.h"
#include "spycraft/game/image.h"
#include "spycraft/game/html.h"
#include "spycraft/game/pcscreen.h"
#include "spycraft/logic/5000.h"
#include "spycraft/logic/1000.h"

namespace Spycraft {

#define CRSBLINK_SPEED 10

enum{ NOPUZZLE, LICENSEPUZZLE, TANKSPUZZLE, AMESPUZZLE };

extern Intrface* intrface;

extern NorthCursor* northCursor;
extern SouthCursor* southCursor;
extern EastCursor* eastCursor;
extern WestCursor* westCursor;
extern BlinkCursor* normalCursor;

extern short  restoring;
extern short drawBackground;	// Travelink

extern List*	permList;

static int	showingNews;			// flag: needed to prevent room change during movie.
static int	lastPuzzle;				// one of enum
static int	timerRunning;			// flag: M's intro timer running (don't start another)
static int	repeatedPz;				// flag: second or more time thru the puzzle
static Feature*	computerPtr;		
static Prop*	backMonitorPtr;		
static Prop*	blankBackMonitorPtr;

/*********************
*
*		Sound 
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

class PlayASound : public Object
{
 public:
 	PlayASound( int soundNum );
	void cue( void );
	Sound*	soundptr;
}; // defined in 2000.cpp

/*********************
*
*		Room Code 
*
**********************/

/* 5000 */
Rm5000::Rm5000()
{
	name = "Rm5000";
	show_style = VE_FLIP;
	if( restoring == 1 )	// purge on game restore
		sfxPurgeRes( 5000, RES_PIC );
}

void Rm5000::init()
{
	PermListObject*	plobjPtr;
									
	drawPic( 5000 );
	Room::init();

	// temp debug
	#ifdef _DEBUG
		#pragma message( "flag message" )
		if( !GameFlag.test( fFarmDidPhotoIntro) )
			sfxPrintf( "photo intro not done, flag %d", fFarmDidPhotoIntro );
	#endif

	new SouthExit5000;
	new WestExit5000;
	computerPtr = new Computer5000;
	backMonitorPtr = new BackMonitorProp5000;
	blankBackMonitorPtr = new BlankBackMonitorProp5000;
	intrface->show();
	// position cursor
	if( !GameFlag.test( fFrankEnteredIA )  && GameFlag.test( fFarmIAAmesDone ) )
	{
		normalCursor->posn( (WEST_EXIT_X +WEST_EXIT_WIDTH)/2,
							(WEST_EXIT_Y +WEST_EXIT_HEIGHT)/2 );
	}
	// set up timer script
	if( !GameFlag.test( fFrankEnteredIA ) &&
		GameFlag.test( fFarmDidPhotoIntro )  &&
		!timerRunning  && GameFlag.test( fShowedIANews ) )
	{
		plobjPtr = new PermListObject;
		plobjPtr->setScript( new TimeoutScript5000 );
	}
	// News clip
	if( !GameFlag.test( fShowedIANews )  && GameFlag.test( fFarmDidPhotoIntro ) )
			setScript( new NewsScript5000Plus );
	else
	{
		theGame->handsOn();
		startSound( 5000, PLAYWAVEMIDI );
	}
}

/* 5100 */	 // computer, puzzles
Rm5100::Rm5100()
{
	name = "Rm5100";
	show_style = VE_FLIP;
}

void Rm5100::init()
{	
	//drawPic( 5100 );
	Room::init();

	intrface->show();
	//theGame->handsOn();
	repeatedPz = false;
	if( !GameFlag.test( fFarmIALicenseDone ) )
		setScript( new IntroScript5100 );
	else 
		playIA();	// this possible on saved game restore
}

void Rm5100::playIA()
{
	theGame->handsOn();
	/* do appropriate puzzle */	
	if( !GameFlag.test( fFarmIALicenseDone ) )
		setScript( new LicenseScript5100 );
	else				//if( !GameFlag.test( fFarmIATanksDone ) )
		setScript( new TanksScript5100 );	
	//else if( !GameFlag.test( fFarmIAAmesDone ) )  -- no longer used
	//	setScript( new AmesScript5100 );
}

/* 5200 */
Rm5200::Rm5200()
{
	name = "Rm5200";
	show_style = VE_FLIP;
}

void Rm5200::init()
{	
	drawPic( 5200 );
	Room::init();

	startSound( 5000, PLAYWAVEMIDI );
	intrface->show();
	theGame->handsOn();
	new EastExit5200;
	new SouthExit5200;
}

/*********************
*
*	Exit Features 
*
**********************/

/* 5000 */
SouthExit5000::SouthExit5000()
{
	name = "SouthExit5000";
	setHotspot( DO_V );
	init( SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT );  
}

void SouthExit5000::respond()
{
	theGame->setCursor( southCursor );
	aniCursor();
}

int SouthExit5000::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
    	theGame->newRoom( new Rm1000 );
    	return true;
	}
	return false;
}

WestExit5000::WestExit5000()
{
	name = "WestExit5000";
	setHotspot( DO_V );
	init(WEST_EXIT_X, WEST_EXIT_Y, WEST_EXIT_WIDTH, WEST_EXIT_HEIGHT);  
}

void WestExit5000::respond()
{
	theGame->setCursor(westCursor);
	aniCursor();
}

int WestExit5000::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
    	theGame->newRoom( new Rm5200 );
    	return true;
	}
	return false;
}

/* 5100 */
SouthExit5100::SouthExit5100()
{
	name = "SouthExit5100";
	setHotspot( DO_V );
	init( SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT );  
}

void SouthExit5100::respond()
{
	theGame->setCursor( southCursor );
	aniCursor();
}

int SouthExit5100::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
    	theGame->newRoom( new Rm5000 );
    	return true;
	}
	return false;
}

/* 5200 */
SouthExit5200::SouthExit5200()
{
	name = "SouthExit5200";
	setHotspot( DO_V );
	init( SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT );  
}

void SouthExit5200::respond()
{
	theGame->setCursor( southCursor );
	aniCursor();
}

int SouthExit5200::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
    	theGame->newRoom( new Rm1000 );
    	return true;
	}
	return false;
}

EastExit5200::EastExit5200()
{
	name = "EastExit5200";
	setHotspot( DO_V );
	init( EAST_EXIT_X, EAST_EXIT_Y, EAST_EXIT_WIDTH, EAST_EXIT_HEIGHT );  
}

void EastExit5200::respond()
{
	theGame->setCursor( eastCursor );
	aniCursor();
}

int EastExit5200::doVerb( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom( new Rm5000 );
    	return true;
	}
	return false;
}

/*******************************
 *
 *	Features
 *
/*******************************/

 Computer5000:: Computer5000()
{
	name = " Computer5000";
	setHotspot( DO_V );
 	init( 302, 272, 60, 60 );
}

void  Computer5000::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int  Computer5000::doVerb( int theVerb )
{
	if ( theVerb == DO_V) {
		pcLastUsed[PC_INTRLINK] = false;
		new PCScreen( 5000 );
		return true;
	}
	return false;
}

/*******************************
 *
 *	Props
 *
/*******************************/

// 5000
BackMonitorProp5000::BackMonitorProp5000()
{
	name = "BackMonitorProp5000";
	setView( 5120 );
 	init( 90, 69 );
 	noSkip();	
	//setHotspot( DO_V );
	cycleSpeed = 4;
	setCycle( new Forward( this ) );
}

void BackMonitorProp5000::respond()
{
	return;
}

void BackMonitorProp5000::cue( void )
{
 	setCycle( new Forward( this ) );	
}

int BackMonitorProp5000::doVerb( int theVerb )
{
	if ( theVerb == DO_V) {
		// happens automatically now
			//if( !GameFlag.test( fShowedIANews ) )
			//	setScript( new NewsScript5000Plus );
		return true;
	}
	return false;
}

BlankBackMonitorProp5000::BlankBackMonitorProp5000()
{
	name = "BlankBackMonitorProp5000";
	setView( 5130 );
 	init( 196, 69 );
 	noSkip();	
	//setHotspot( DO_V );
	cycleSpeed = 4;
	setCycle( new Forward( this ) );
}

void BlankBackMonitorProp5000::cue( void )
{
 	setCycle( new Forward( this ) );	
}

int BlankBackMonitorProp5000::doVerb( int theVerb )
{
	//if( theVerb == DO_V )
	//{
	//	// do stuff ...
	//	return true;
	//}
	return false;
}

/*******************************
 *
 *	Scripts
 *
/*******************************/

TimeoutScript5000::~TimeoutScript5000()
{
 	timerRunning = false;
}

void TimeoutScript5000::changeState( int newState )
{
	static Sound*	soundptr;
	static int	pass;			// passes thru this script

 	switchTo
		if( pass == 0 )
			seconds = 10;	
		else
			seconds = 6;
		++pass;
		soundptr = NULL;
		timerRunning = true;
	END

	BEG
		if( showingNews )	// player used to trigger news
		 	client->setScript( new TimeoutScript5000 ); // reset
		if( currentFocus != 0  && currentFocus != INSET_FOCUS )
			client->setScript( new TimeoutScript5000 ); // reset if PDA open
		else if( theInset != NULL  && pass < 4 )
			client->setScript( new TimeoutScript5000 ); // reset
		else if( strcmp( curRoom->name, "Rm5000")  && strcmp( curRoom->name, "Rm5200") )
			client->dispose();	// perm list object.  Player left IA
		else
		{	// start
			theGame->handsOff();	//this delay protects the PDA-up script
		 	ticks = 5;
		}
	END

	BEG
		if( theInset != NULL )
			theInset->dispose();
		//soundptr = new Sound;
		//soundptr->play( 5210, this );	// Frank coming	footsteps
		ticks = 2;	// instead of sound cue
		timerRunning = false;
	END

	BEG
		//soundptr->dispose();
		theGame->newRoom( new Rm5100 );	// computer
		client->dispose();	// perm list object
	END
}

void NewsScript5000Plus::changeState( int newState )
{
	PermListObject*	plobjPtr;

	switchTo
		#ifdef _DEBUG
		  #if BKSOUND == ON
			if( sound1->isPlaying() )
			{
				sfxPrintf( "stopping sound" );	// warn in case sound shouldn't be playing
				sound1->stop();	// in case you teleport in and then set flags.
			}
		  #endif
		#endif
		theGame->handsOff();
		ticks = 10;	// seems to need this between when skipping ('M') movie in Travelink
	END

	BEG
		ASSERT( !sound1->isPlaying(), 0 );
		computerPtr->dispose();
		backMonitorPtr->setCycle( NULL );
		blankBackMonitorPtr->setCycle( NULL );
		ticks = 2;	// let's see if this kills the ghost
	END

	BEG
		showingNews = true;
		theMovie->play( "5010.avi", this );	// news cast	
	END

	BEG
		GameFlag.set( fShowedIANews );
		theGame->handsOn();
		showingNews = false;
		// set Frank's timer
		plobjPtr = new PermListObject;
		plobjPtr->setScript( new TimeoutScript5000 );
		// set up room
		computerPtr = new Computer5000;
		backMonitorPtr->setCycle( new Forward( backMonitorPtr ) );
		blankBackMonitorPtr->setCycle( new Forward( blankBackMonitorPtr ) );
		startSound( 5000, PLAYWAVEMIDI );
		dispose();
	END
}

void IntroScript5100::changeState( int newState )
{
	switchTo
		#if BKSOUND == ON
			sound1->stop();
		#endif
		ticks = 1;
	END

	BEG
		theGame->handsOff();
		theMovie->play( "5000.avi", this, false, true, false );  //Image Analysis intro		
	END

	BEG
		((Rm5100*)curRoom)->playIA();
	END
}

void LicenseScript5100::changeState( int newState )
{
	switchTo
		ticks = 1; // allows some cues to unwind
	END

	BEG
		GameFlag.clear( fSentPuzzleMail );
		pcLastUsed[PC_IA] = WEB_IMAGEPLATE;
		pcLastUsed[PC_INTRLINK] = false;
		new PCScreen( 5100, this );
	END

	BEG
		theGame->handsOff();
		if( !GameFlag.test( fFarmIALicenseDone ) )
		{
			repeatedPz = true;
			if( GameFlag.test( fSentPuzzleMail ) )
				theMovie->play( "5001.avi", this );	 // absolute crap
			else
				theMovie->play( "5003.avi", this );	 // give it another go
		}
		else
		{
			if( repeatedPz )
				theMovie->play( "5005.avi", this );	 // So so, move on
			else
				theMovie->play( "5006.avi", this );	 // You smoked that, move on. (727)
		}
	END
 
	BEG
		theGame->handsOn();
		dispose();
		((Rm5100*)curRoom)->playIA();
	END
}

void TanksScript5100::changeState( int newState )
{
	switchTo
		ticks = 1; // allows some cues to unwind
	END

	BEG
		GameFlag.clear( fSentPuzzleMail );
		pcLastUsed[PC_IA] = WEB_IMAGETANK;
		pcLastUsed[PC_INTRLINK] = false;
		new PCScreen( 5100, this );
	END

	BEG
		theGame->handsOff();
		if( GameFlag.test( fFarmIATanksDone ) )	 // success
			theMovie->play( "5004.avi", this, false, true, false ); // takes your picture
		else  // failure
		{
			if( GameFlag.test( fSentPuzzleMail ) )
				theMovie->play( "5001.avi", this );	 // absolute crap
			else
				theMovie->play( "5003.avi", this );	 // give it another go
		}
	END

	BEG			
		if( GameFlag.test( fFarmIATanksDone ) )
		{
			GameFlag.set( fFarmIADone );
			drawBackground = false;	// Travelink
		 	theGame->newRoom( new Rm1000 );
			pcLastUsed[PC_IA] = false;
		}
		else
		{
			dispose();		
			((Rm5100*)curRoom)->playIA();
		}
	END
}

// no longer used
void AmesScript5100::changeState( int newState )
{
	switchTo
		ticks = 1;	// allows some cues to unwind
	END

	BEG
		theGame->handsOff();
		if( lastPuzzle != AMESPUZZLE )
			theMovie->play( "5002.avi", this );  // win: Ames puzzle intro
		else
			theMovie->play( "5003.avi", this );	// give it another go
	END

	BEG
		theGame->handsOn();
		lastPuzzle = AMESPUZZLE;
		pcLastUsed[PC_IA] = WEB_IMAGECOURIER;
		pcLastUsed[PC_INTRLINK] = false;	
		new PCScreen( 5100, this );
		//normalCursor->rest();
		//new Web;
		//new ImageAnalPlate;
		//new HTMLPage( 99171 );
		//ticks = 2;	   //web doesn't know how to cue
	END

	BEG
		theGame->handsOff();
		 //#pragma message( "Temp: always win" )
		 //GameFlag.set( fFarmIAAmesDone );
		if( GameFlag.test( fFarmIAAmesDone ) )
		{
			theMovie->play( "5004.avi", this, false, true, false ); // that's how we got him
			pcLastUsed[PC_IA] = false;
		}
		else
			cue();
	END

	BEG
		if( GameFlag.test( fFarmIAAmesDone ) )
			theGame->newRoom( new Rm1000 );
		else
		{
			dispose();
			((Rm5100*)curRoom)->playIA();        //theGame->newRoom( new Rm5100 );
		}
	END
}
 
} // namespace Spycraft 
