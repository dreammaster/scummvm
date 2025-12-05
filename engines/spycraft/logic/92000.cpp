/******************************
 *		Ext. Tropical Desert 
 *		room 92000
 *		created by: Greg Scharf
 *****************************/ 	

#include "globals.h"
#include "game.h"
#include "verbs.h"
#include "views.h"
#include "92000.h"

/*********************
*
*		Room Code 
*
**********************/

extern Intrface* intrface;

//extern SouthCursor* southCursor;

extern Print* testPrint;

Rm92000::Rm92000()
{
	name = "Rm92000";
	show_style = VE_FLIP;
}

Rm92000::~Rm92000()
{
   testPrint->erase();
}

void Rm92000::init()
{
  	drawPic( 1 );
    Room::init();

 	intrface->show();
  	theGame->handsOn();
	testPrint = new(Print); 
	testPrint->init("Ext. Tropical Desert ");
	curRoom->setScript(new Script92000);
}

int Rm92000::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}

/*******************************
 *
 *	Scripts
 *
 *******************************/
void Script92000::changeState ( int newState )
{
	switchTo
		seconds = 2;
	END
   
    BEG
//		theMovie->play("?????.avi", 0, 0, 640, 480, this);
		sfxPrintf("Welcome to the jungle (j4/?????) Hello, thorn...");
		seconds = 2;
	END

	BEG
		sfxPrintf("crisis orienteering");	//pat interactive
 		dispose();
	END

}

