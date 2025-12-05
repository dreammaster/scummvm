/*********************************************
* Cpp source file.
* Alley near unsafe house.        
* Copyright Activision 1995
* Started by Daniel Kinzek   June, 1995
**********************************************/
//Finished by Ka Wah

#include "globals.h"
#include "game.h"
#include "verbs.h"
#include "views.h"
#include "32300.h"
#include "32200.h"
//#include "movie.h"
//#include "vlink.h"

#define CRSBLINK_SPEED 5

extern Intrface* intrface;
extern SouthCursor* southCursor;
//extern NorthwestCursor* northwestCursor;
extern BlinkCursor* normalCursor;

/*********************
*
*	Room 
*
**********************/

Rm32300::Rm32300()
{
	name = "Rm32300";
	show_style = VE_FLIP;
}

void Rm32300::init()
{
	drawPic( 32300 );
	Room::init();
	new SouthExit32300;
//	new NorthWestExit32300;
	intrface->show();
	theGame->handsOn();
}

/*********************
*
*	Exit Features 
*
**********************/

SouthExit32300::SouthExit32300()
{
	name = "SouthExit32300";
	setHotspot( DO_V );
	init( SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT );  
}

void SouthExit32300::respond()
{
	theGame->setCursor( southCursor );
	aniCursor();
}

int SouthExit32300::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
    	theGame->newRoom( new Rm32200 );
    	return TRUE;
	}
	return FALSE;
}
/*
NorthWestExit32300::NorthWestExit32300()
{
	name = "NorthWestExit32300";
	setHotspot( DO_V );
	init(155, 63, 69, 123);
}

void NorthWestExit32300::respond()
{
	theGame->setCursor( northwestCursor );
}

int NorthWestExit32300::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
//    	theGame->newRoom( new Rm32200 );
			if( !GameFlag.test( fVCRoomEnter ))	   
				setScript( new EnterScript32300 );
    	return TRUE;
	}
	return FALSE;
}
*/
/*******************************
 *
 *	Scripts
 *
/*******************************/
/*
EnterScript32300::EnterScript32300()
{
	name = "EnterScript32300"; 
}

void EnterScript32300::changeState( int newState )
{
	switchTo
		GameFlag.set(fVCRoomEnter);
		theGame->handsOff();
		theMovie->play("32300.avi", this, 0, 0, 1);
	END
	BEG
		theGame->handsOn();
		dispose();
		new VLink( "32301.avi", 0, this );	
	END
}
*/
