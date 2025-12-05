//Started by Ka Wah
//Yuri's room without gift box
//Copyright Activision 1995
//Finished by Ka Wah

#include "globals.h"
#include "game.h"
#include "verbs.h"
#include "views.h"
#include "48000.h"
#include "48400.h"
#include "1000.h"
//#include "45000.h"
#include "flag.h"

#define CRSBLINK_SPEED 5

/*********************
*
*		Room Code 
*
**********************/

extern Intrface* intrface;
extern BlinkCursor* normalCursor;
extern SouthCursor* southCursor;

Rm48400::Rm48400()
{
	name = "Rm48400";
	show_style = VE_FLIP;
}

Rm48400::~Rm48400()
{
}

void Rm48400::init()
{
  	drawPic( 48400 );
    Room::init();
  
	new(SouthExit48400);
	
  	intrface->show();
  	theGame->handsOn();
}

int Rm48400::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}

SouthExit48400::SouthExit48400()
{
	name = "SouthExit48400";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
}

void SouthExit48400::respond()
{
	theGame->setCursor(southCursor);
}

int SouthExit48400::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
//    	theGame->newRoom(new Rm45000);
		theGame->newRoom(new Rm1000);
    	return TRUE;
	}
	return FALSE;
}
 
