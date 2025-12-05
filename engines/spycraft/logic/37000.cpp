//Started by Tom DeSalvo
//Ext. Street Kneecap
//Copyright Activision 1995
//Finished by Ka Wah

#include "globals.h"
#include "game.h"
#include "verbs.h"
#include "views.h"
#include "37000.h"
#include "1000.h"
//#include "38000.h"
//#include "41000.h"
//#include "39000.h"
//#include "37100.h"
#include "41100.h"   //rm41300
#include "flag.h"
#include "movie.h"

#define CRSBLINK_SPEED 5

/*********************
*
*		Room Code 
*
**********************/

extern Intrface* intrface;
extern BlinkCursor* normalCursor;
extern SouthCursor* southCursor;
extern NorthCursor* northCursor;

Rm37000::Rm37000()
{
	name = "Rm37000";
	show_style = VE_FLIP;
}

Rm37000::~Rm37000()
{
}

void Rm37000::init()
{
///	drawPic( 37000 );
////drawPic ( 1 );
    Room::init();

  	new(SouthExit37000);
//	new(NorthExit37000);
///	new(PanelVan37000);
///	new(PanelSign37000);
	
  	intrface->show();
// 	theGame->handsOn();

	if (!GameFlag.test(fEnterBadmanVan))
		curRoom->setScript(new EnterBadmanVanScript37000);
	else
		theGame->handsOn();
}

int Rm37000::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}

SouthExit37000::SouthExit37000()
{
	name = "SouthExit37000";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
}

void SouthExit37000::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit37000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm1000);
    	return TRUE;
	}
	return FALSE;
}
/*
NorthExit37000::NorthExit37000()
{
	name = "NorthExit37000";
	setHotspot ( DO_V );
	init(NORTH_EXIT_X, NORTH_EXIT_Y, NORTH_EXIT_WIDTH, NORTH_EXIT_HEIGHT);
}

void NorthExit37000::respond()
{
	theGame->setCursor(northCursor);
}

int NorthExit37000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm37100);
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

/*
KneeRoof37000::KneeRoof37000()
{
  	name = "KneeRoof";
	init (200, 150, 50, 50);   //bottom left
}

int KneeRoof37000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
   		theGame->newRoom(new Rm38000);
    	return TRUE;
	}
	return FALSE;
}
*/

PanelVan37000::PanelVan37000()
{
  	name = "PanelVan";
	setHotspot( DO_V );
	init (115, 203, 172, 135);   
}

void PanelVan37000::respond()
{
//	theGame->setCursor( normalCursor );
//	normalCursor->blink( CRSBLINK_SPEED );
	theGame->setCursor( northCursor );
}

int PanelVan37000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
//		theGame->newRoom(new Rm41000);
   		theGame->newRoom(new Rm41300);
	
    	return TRUE;
	}
	return FALSE;
}

PanelSign37000::PanelSign37000()
{
  	name = "PanelSign37000";
	setHotspot( DO_V );
	init (310, 213, 54, 43);   
}

void PanelSign37000::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int PanelSign37000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
   		new PanelSignInset37000();
    	return TRUE;
	}
	return FALSE;
}
/*
Tenement37000::Tenement37000()
{
  	name = "Tenement";
	init (400, 150, 50, 50);   //bottom left
}

int Tenement37000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
   		theGame->newRoom(new Rm39000);
    	return TRUE;
	}
	return FALSE;
}
*/
/*******************************
 *
 *	Insets
 *
 *******************************/

PanelSignInset37000::PanelSignInset37000()
{
	normalCursor->rest();
 	init(37020, 0, 15, 512, 385);
}

/*******************************
 *
 *	Scripts
 *
/*******************************/
EnterBadmanVanScript37000::EnterBadmanVanScript37000()
{
	name = "EnterBadmanVanScript37000"; 
}

void EnterBadmanVanScript37000::changeState( int newState )
{
	switchTo
		GameFlag.set(fEnterBadmanVan);
		theGame->handsOff();
		theMovie->play("41501.avi", this, 0, 1, 0);
	END
	BEG
///		theGame->handsOn();
//		dispose();
		theGame->newRoom(new Rm41500);
	END
}
