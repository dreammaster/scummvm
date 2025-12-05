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
#include "32200.h"
#include "32300.h"
#include "32100.h"

#define CRSBLINK_SPEED 5

extern Intrface* intrface;
extern NorthCursor* northCursor;
extern SouthCursor* southCursor;
extern BlinkCursor* normalCursor;

/*********************
*
*	Room 
*
**********************/

Rm32200::Rm32200()
{
	name = "Rm32200";
	show_style = VE_FLIP;
}

void Rm32200::init()
{
	drawPic( 32200 );
	Room::init();
	new NorthExit32200;
	new SouthExit32200;
	intrface->show();
	theGame->handsOn();
}

/*********************
*
*	Exit Features 
*
**********************/
NorthExit32200::NorthExit32200()
{
	name = "NorthExit32200";
	setHotspot( DO_V );
	init(196, 142, 95, 183);
//	init(NORTH_EXIT_X, NORTH_EXIT_Y, NORTH_EXIT_WIDTH, NORTH_EXIT_HEIGHT);  
}

void NorthExit32200::respond()
{
	theGame->setCursor( northCursor );
	aniCursor();
}

int NorthExit32200::doVerb ( int theVerb )
{
	if( theVerb == DO_V ) {
    	theGame->newRoom( new Rm32300 );
    	return TRUE;
	}
	return FALSE;
}

SouthExit32200::SouthExit32200()
{
	name = "SouthExit32200";
	setHotspot( DO_V );
	init( SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT );  
}

void SouthExit32200::respond()
{
	theGame->setCursor( southCursor );
	aniCursor();
}

int SouthExit32200::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
    	theGame->newRoom( new Rm32100 );
    	return TRUE;
	}
	return FALSE;
}
