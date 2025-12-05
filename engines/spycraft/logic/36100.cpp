/*********************************************
* Cpp source file.
* Foster's office, side desk view.        
* Copyright Activision 1995
* Started by Daniel Kinzek   June, 1995
**********************************************/
//Finished by Ka Wah

#include "globals.h"
#include "game.h"
#include "verbs.h"
#include "views.h"
#include "36100.h"
#include "36000.h"
#include "36300.h"
#include "flag.h"
#include "movie.h"

#define CRSBLINK_SPEED 5

extern Intrface* intrface;
extern SouthCursor* southCursor;
extern BlinkCursor* normalCursor;

/*********************
*
*	Room 
*
**********************/

Rm36100::Rm36100()
{
	name = "Rm36100";
	show_style = VE_FLIP;
}

void Rm36100::init()
{
	drawPic( 36100 );
	Room::init();

	new SouthExit36100;
	new	Desk36100;
//	new Photo36100;
	new Computer36100;

	intrface->show();
	theGame->handsOn();

	//if(GameFlag.test(fPlayerTurnsProcat) && !GameFlag.test(fGetProcatFile))  //1 Minute Max caught player
	//	setScript( new EnterScript36100 );	   //DEBUG
}

/*********************
*
*	Exit Features 
*
**********************/

SouthExit36100::SouthExit36100()
{
	name = "SouthExit36100";
	setHotspot( DO_V );
	init( SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT );  
}

void SouthExit36100::respond()
{
	theGame->setCursor( southCursor );
	aniCursor();
}

int SouthExit36100::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
	
    	theGame->newRoom( new Rm36000 );
    	return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Features
 *
/*******************************/

Desk36100::Desk36100()
{
	name = "Desk36100";
	setHotspot( DO_V );
	init( 261, 130, 230, 265 );
}

void Desk36100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Desk36100::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		GameFlag.set(fGetProcatFile);
		theGame->newRoom( new Rm36300 );
		return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Text Features
 *
/*******************************/
/*
Photo36100::Photo36100()
{
	name = "Photo36100";
	setHotspot( DO_V );
	init ( 100, 100, 200, 50 );
}

void Photo36100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Photo36100::doVerb( int theVerb )
{
	if ( theVerb == DO_V ) {
		sfxPrintf( "Photo");
		return TRUE;
	}
	return FALSE;
}
*/

Computer36100::Computer36100()
{
	name = "Computer36100";
	setHotspot( DO_V );
	init ( 414, 17, 166, 140 );
}

void Computer36100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Computer36100::doVerb( int theVerb )
{
	if ( theVerb == DO_V ) {
		new ComputerInset36100();
		return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Insets
 *
 *******************************/

ComputerInset36100::ComputerInset36100()
{
	normalCursor->rest();
 	init(36120, 0, 15, 512, 385);
}

/*******************************
 *
 *	Scripts
 *
 *******************************/
 
EnterScript36100::EnterScript36100()
{
	name = "EnterScript36100"; 
}

void EnterScript36100::changeState( int newState )
{
	switchTo
		seconds = 60;

	END
	BEG
		GameFlag.set(fGetProcatFile);	
		theGame->handsOff();
///		theMovie->fromTo(390, 630);
   		theMovie->play("36100.avi", this, 0, 1, 1);
	END
	BEG
		theGame->handsOn();
		dispose();
	END
}
