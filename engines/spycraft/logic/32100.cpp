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
#include "32100.h"
#include "32200.h"
#include "32000.h"

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

Rm32100::Rm32100()
{
	name = "Rm32100";
	show_style = VE_FLIP;
}

void Rm32100::init()
{
	drawPic( 32100 );
	Room::init();
	new SouthExit32100;
	new NorthExit32100;
	intrface->show();
	theGame->handsOn();
}

/*********************
*
*	Exit Features 
*
**********************/

NorthExit32100::NorthExit32100()
{
	name = "NorthExit32100";
	setHotspot( DO_V );
	init(185, 101, 92, 193);
//	init(NORTH_EXIT_X, NORTH_EXIT_Y, NORTH_EXIT_WIDTH, NORTH_EXIT_HEIGHT);  
}

void NorthExit32100::respond()
{
	theGame->setCursor( northCursor );
	aniCursor();
}

int NorthExit32100::doVerb ( int theVerb )
{
	if( theVerb == DO_V ) {
    	theGame->newRoom( new Rm32200 );
    	return TRUE;
	}
	return FALSE;
}

SouthExit32100::SouthExit32100()
{
	name = "SouthExit32100";
	setHotspot( DO_V );
	init( SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT );  
}

void SouthExit32100::respond()
{
	theGame->setCursor( southCursor );
	aniCursor();
}

int SouthExit32100::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
    	theGame->newRoom( new Rm32000 );
    	return TRUE;
	}
	return FALSE;
}
