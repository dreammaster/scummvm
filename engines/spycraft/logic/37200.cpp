//Started by Ka Wah
//Kneecaps Pad - Master of room without Birdsong
//Copyright Activision 1995
//Finished by Ka Wah

#include "globals.h"
#include "game.h"
#include "verbs.h"
#include "views.h"
#include "37000.h"
#include "1000.h"
#include "37200.h"
#include "37300.h"
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
extern WestCursor* westCursor;

Rm37200::Rm37200()
{
	name = "Rm37200";
	show_style = VE_FLIP;
}

Rm37200::~Rm37200()
{
}

void Rm37200::init()
{  
    Room::init();
  
	new(SouthExit37200);
	new(WestExit37200);
	
  	intrface->show();
///	theGame->handsOn();

	if( !GameFlag.test( fBSTellStory )) {	   
//		drawPic( 1 );                          
//		drawPic( 37200 );
// 		setScript( new EnterScript37200 );
///		setScript( new EnterBlackScreenScript37201);
		theGame->handsOff();
		if (GameFlag.test(fStickPath))
			setScript( new EnterStickScript37200 );
//		else if (GameFlag.test(fCarrotPath))
		else
			setScript( new EnterCarrotScript37200 );
	}
	else{
 	  	drawPic( 37200 );
		theGame->handsOn();
  	}
}

int Rm37200::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}

SouthExit37200::SouthExit37200()
{
	name = "SouthExit37200";
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
    setHotspot ( DO_V );
}

void SouthExit37200::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit37200::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
//		theGame->newRoom(new Rm37000);
		theGame->newRoom(new Rm1000);
    	return TRUE;
	}
	return FALSE;
}

WestExit37200::WestExit37200()
{
	name = "WestExit37200";
	init(WEST_EXIT_X, WEST_EXIT_Y, WEST_EXIT_WIDTH, WEST_EXIT_HEIGHT);
    setHotspot ( DO_V );
}

void WestExit37200::respond()
{
	theGame->setCursor(westCursor);
	aniCursor();
}

int WestExit37200::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm37300);
    	return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Scripts
 *
 *******************************/
EnterStickScript37200::EnterStickScript37200()
{
	name = "EnterStickScript37200"; 
}

void EnterStickScript37200::changeState( int newState )
{
	switchTo
		GameFlag.set(fBSTellStory);  
//		GameFlag.clear(fDisplayKnee); 
		ticks = 2;
	END
	BEG
		theMovie->play("37200.avi", this, 0, 1, 0);	  //stick - betray Max
	END
	BEG
		ticks = 2;
	END
	BEG
		theGame->newRoom( new Rm1000 );
	END
}

EnterCarrotScript37200::EnterCarrotScript37200()
{
	name = "EnterCarrotScript37200"; 
}

void EnterCarrotScript37200::changeState( int newState )
{
	switchTo
		GameFlag.set(fBSTellStory);
		ticks = 2;
	END
	BEG
		theMovie->play("37201.avi", this, 0, 1, 0);	  //carrot path
	END
	BEG
		ticks = 2;
	END
	BEG
		theGame->newRoom( new Rm1000);
	END
}

/* Room 37201 */
Rm37201::Rm37201()
{
	name = "Rm37201";
	show_style = VE_FLIP;
}

Rm37201::~Rm37201()
{
}

void Rm37201::init()
{
	drawPic ( 1 );
    Room::init();
  
  	intrface->show();
  	theGame->handsOn();

	if (GameFlag.test(fStickPath))
		setScript( new EnterStickScript37200 );
	else if (GameFlag.test(fCarrotPath))
		setScript( new EnterCarrotScript37200 );

}

int Rm37201::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}

EnterBlackScreenScript37201::EnterBlackScreenScript37201()
{
	name = "EnterBlackScreenScript37201"; 
}

void EnterBlackScreenScript37201::changeState( int newState )
{
   	GameFlag.set( fBSTellStory );		
	theGame->newRoom( new Rm37201 );
}
