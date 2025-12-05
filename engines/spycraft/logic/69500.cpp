//Started by Ka Wah
//Hotel Schlump - round table to left and ahead
//Copyright Activision 1995
//Finished by Ka Wah

#include "globals.h"
#include "game.h"
#include "verbs.h"
#include "views.h"
#include "69000.h"
#include "69500.h"
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
//extern Print* testPrint;

Rm69500::Rm69500()
{
	name = "Rm69500";
	show_style = VE_FLIP;
}

//Rm69500::~Rm69500()
//{
   //testPrint->erase();
//}

void Rm69500::init()
{
  	drawPic( 69500 );
    Room::init();
  
	new(SouthExit69500);
	new(Menu69500);
	
  	intrface->show();
  	theGame->handsOn();
   //testPrint = new(Print); 
   //testPrint->init("Hotel Schlump Int.");
}

int Rm69500::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}

SouthExit69500::SouthExit69500()
{
	name = "SouthExit69500";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
}

void SouthExit69500::respond()
{
	theGame->setCursor(southCursor);
}

int SouthExit69500::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm69000);
    	return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Features
 *
/*******************************/

Menu69500::Menu69500()
{
	name = "Menu69500";
	setHotspot( DO_V );
	init( 282, 238, 107, 92 );
}

void Menu69500::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Menu69500::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		curRoom->zoomTo ( 302, 296 );
		new MenuInset69500();
		return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Insets
 *
 *******************************/

MenuInset69500::MenuInset69500()
{
	normalCursor->rest();
 	init(69520, 0, 15, 512, 385);
}
