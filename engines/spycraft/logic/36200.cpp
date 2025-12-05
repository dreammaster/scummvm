//Started by Tom DeSalvo
//Foster's desk drawer for lock pick puzzle.   
//Copyright Activision 1995
//Finished by Ka Wah

#include "globals.h"
#include "game.h"
#include "verbs.h"
#include "views.h"
#include "36200.h"
#include "36300.h"
#include "sound.h"

#define CRSBLINK_SPEED 5

extern Intrface* intrface;
extern SouthCursor* southCursor;
extern BlinkCursor* normalCursor;
extern Sound* sound2;

/*********************
*
*	Room 
*
**********************/

Rm36200::Rm36200()
{
	name = "Rm36200";
	show_style = VE_FLIP;
}

Rm36200::~Rm36200( void )
{
}

void Rm36200::init()
{							   
	drawPic( 36200 );
	Room::init();

	new SouthExit36200;
	new Lock36200;

	intrface->show();
	theGame->handsOn();

}

int Rm36200::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}

/*********************
*
*	Exit Features 
*
**********************/

SouthExit36200::SouthExit36200()
{
	name = "SouthExit36200";
	setHotspot( DO_V );
	init( SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT );  
}

void SouthExit36200::respond()
{
	theGame->setCursor( southCursor );
	aniCursor();
}

int SouthExit36200::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
    	theGame->newRoom( new Rm36300 );
    	return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Features
 *
/*******************************/

Lock36200::Lock36200()
{
	name = "Lock36200";
	setHotspot( DO_V );
//	hotspot->addRespondVerb(LOCKPICK_V);	
	init( 326, 125, 140, 165 );
	hotspot->addRespondVerb(ALL_V);
}

void Lock36200::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Lock36200::doVerb( int theVerb )
{
/*
	if( theVerb != LOCKPICK_V)	{
			if( sound2->number != 36021  || !sound2->isPlaying()) {
			sound2->number = 36021;
			sound2->play();
		}
//		sfxPrintf( "You can't pick a lock with that!" );	
		return TRUE;
	}
	else {
		theGame->handsOff();
		inventry->clearCurrentItem();

		curRoom->setScript(new LockPickScript36200);
		return TRUE;
	}
	return FALSE;
*/
	if ( invVerb == LOCKPICK_V ) {
//		sfxPrintf( "Click" );
		inventry->clearCurrentItem();
		theGame->handsOff();
		curRoom->setScript(new LockPickScript36200);
		return TRUE;
	}
	else if ( invVerb != LOCKPICK_V ){
		if( sound2->number != 36021  || !sound2->isPlaying()) {
			sound2->number = 36021;
			sound2->play();
		}
	}
	else if ( theVerb == DO_V ) {
		return TRUE;
	}
	return FALSE;
}

LockPickScript36200::LockPickScript36200()
{
	name = "LockPickScript36200"; 
}

void LockPickScript36200::changeState( int newState )
{	   
	switchTo
		GameFlag.set( fFosterDeskDrawerUnlocked );
		GameFlag.set( fWahKa64 );
//		GameFlag.set( fWahKa79 );
		sound2->play(36011, this);		 //picking lock
	END
	BEG
		ticks = 2;
	END
	BEG
		sound2->play(35121, this);		 //drawer opens
	END
	BEG
		ticks = 2;
	END
	BEG
		theGame->newRoom( new Rm36302 );
	END
}
