//Started by Ka Wah
//Vilnius Center
//Copyright Activision 1995
//Finished by Ka Wah

#include "globals.h"
#include "game.h"
#include "verbs.h"
#include "views.h"
#include "53000.h"
#include "flag.h"
#include "movie.h"
#include "vlink.h"
#include "1000.h"

#define CRSBLINK_SPEED 5

/*********************
*
*		Room Code 
*
**********************/

extern Intrface* intrface;
extern BlinkCursor* normalCursor;

Rm53000::Rm53000()
{
	name = "Rm53000";
	show_style = VE_FLIP;
}

Rm53000::~Rm53000()
{
}

void Rm53000::init()
{
  	drawPic( 1 );
    Room::init();

  	intrface->show();
///	theGame->handsOn();
  
	if( !GameFlag.test( fVCRoomEnter ))	   
		setScript( new EnterScript53000 );
}

int Rm53000::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}

/*******************************
 *
 *	Scripts
 *
/*******************************/
EnterScript53000::EnterScript53000()
{
	name = "EnterScript53000"; 
}

void EnterScript53000::changeState( int newState )
{
	switchTo
		GameFlag.set(fVCRoomEnter);
///		intrface->disable();
///		theGame->handsOff();
		theMovie->play( "53000.avi", this, 0, 1, 1);
	END
	BEG
///		theGame->handsOn();
///		dispose();
		theGame->newRoom(new Rm1000);
//		new VLink( "Birdsong blew the cage: Foster, Maxine M.", "53001.avi", 0, this );	
	END
}
