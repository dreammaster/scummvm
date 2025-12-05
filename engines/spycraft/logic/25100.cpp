//Started by Ka Wah
//Bullpen - Bullpen from 25000
//Copyright Activision 1995
//Finished by Ka Wah

#include "globals.h"
#include "game.h"
#include "verbs.h"
#include "views.h"
#include "25000.h"
#include "25100.h"
#include "flag.h"

#define CRSBLINK_SPEED 5

/*********************
*
*		Room Code 
*
**********************/

extern Intrface* intrface;

extern BlinkCursor* normalCursor;
extern WestCursor* westCursor;

Rm25100::Rm25100()
{
	name = "Rm25100";
	show_style = VE_FLIP;
}

Rm25100::~Rm25100()
{
}

void Rm25100::init()
{
  	drawPic( 25100 );
    Room::init();
  
	new(WestExit25100);
	
  	intrface->show();
  	theGame->handsOn();
}

int Rm25100::handleEvent ( MADEEventStamp *event )
{

	return FALSE;
}

WestExit25100::WestExit25100()
{
	name = "WestExit25100";
    setHotspot ( DO_V );
   	init(0,0,30,330);
}

void WestExit25100::respond()
{
	theGame->setCursor(westCursor);
	aniCursor();
}

int WestExit25100::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm25000);
    	return TRUE;
	}
	return FALSE;
}
