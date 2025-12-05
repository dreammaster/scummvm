//Started by Ka Wah
//Hotel Schlump - down to Disguise kit on floor
//Copyright Activision 1995
//Finished by Ka Wah

#include "globals.h"
#include "game.h"
#include "verbs.h"
#include "views.h"
#include "69000.h"
#include "69300.h"
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
extern BlinkCursor* normalCursor;
//extern Print* testPrint;

Rm69300::Rm69300()
{
	name = "Rm69300";
	show_style = VE_FLIP;
}

//Rm69300::~Rm69300()
//{
   //testPrint->erase();
//}

void Rm69300::init()
{
  	drawPic( 69300 );
    Room::init();
  
	new(SouthExit69300);
	new(DisguiseKit69300);
	
  	intrface->show();
  	theGame->handsOn();

   //testPrint = new(Print); 
   //testPrint->init("Hotel Schlump Int.");
}

int Rm69300::handleEvent ( MADEEventStamp *event )
{
//	if (event_type == USER_LEFT_UP)	{
//		new(DisguiseKitInset69320);
//	}	
	return FALSE;
}
/*
int Rm69300::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	new(DisguiseKitInset69320);
    	return TRUE;
	}
	return FALSE;
}
*/

/*******************************
 *
 *	Features
 *
/*******************************/

DisguiseKit69300::DisguiseKit69300()
{
	name = "DisguiseKit69300";
	setHotspot( DO_V );
	init( 150, 150, 211, 159 );
}

void DisguiseKit69300::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int DisguiseKit69300::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		new DisguiseKitInset69320();
		return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Insets
 *
 *******************************/

DisguiseKitInset69320::DisguiseKitInset69320()
{
	normalCursor->rest();
// switch places with 69320
 	init(69340, 0, 15, 512, 385);
}

void DisguiseKitInset69320::respond()
{

}

int DisguiseKitInset69320::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
		dispose();
		new DisguiseKitInset69340();
    	return TRUE;
	}
	return FALSE;
}

DisguiseKitInset69340::DisguiseKitInset69340()
{
	normalCursor->rest();
// switch places with 69340 
 	init(69320, 0, 15, 512, 385);
}

SouthExit69300::SouthExit69300()
{
	name = "SouthExit69300";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
    
}

void SouthExit69300::respond()
{
	theGame->setCursor(southCursor);
}

int SouthExit69300::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm69000);
    	return TRUE;
	}
	return FALSE;
}

