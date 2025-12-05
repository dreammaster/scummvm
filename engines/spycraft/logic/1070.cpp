//Started by Tom DeSalvo
//Tundra Est & Cluster

#include "globals.h"
#include "game.h"
#include "verbs.h"
#include "views.h"
#include "1070.h"
#include "95300.h"
#include "movie.h"
#include "save.h"
#include "1000.h"


/*********************
*
*		Room Code 
*
**********************/

//extern Intrface* intrface;

//extern SouthCursor* southCursor;

//extern Print* testPrint;

//static Tundra3D1070* theTundra3D;
//static JumpJet1070* theJumpJet;
extern short restoring; 
extern SaveGame* saveGame;

Rm1070::Rm1070()
{
	name = "Rm1070";
	show_style = VE_FLIP;
}

Rm1070::~Rm1070()
{
   //testPrint->erase();
   //theTundra3D->myPrint->erase();
   //theJumpJet->myPrint->erase();
}

void Rm1070::init()
{
 //	drawPic( 1 );
    Room::init();

	if (!restoring)
		sfxPrintf("Autosaving in Tunisia");
		#ifndef DOS4GW
		saveGame->save("Tunisia.sgm");

		#else
			char* tmpSave;
			SaveDlg("sgm");
			tmpSave = sfxGetSaveFile();
			if (strcmp(tmpSave, ""))
				saveGame->save(tmpSave);
		#endif


	curMap = MAP_MOSCOWPLATE;
	GameFlag.set(fBeenToTunisia);
	intrface->disable();


	setScript(new EnterScript1070);
  	//new(SouthExit1070);
	//theTundra3D = new Tundra3D1070;
//	theJumpJet = new JumpJet1070;
  	//intrface->show();
  //	theGame->handsOn();
   //	testPrint = new(Print); 
   //	testPrint->init("Tundra Est & Cluster ");
}

int Rm1070::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}

EnterScript1070::EnterScript1070()
{
	name = "EnterScript1070";
}

void EnterScript1070::changeState( int newState )
{
	switchTo
	   	ticks = 2;
	END

	BEG
		theMovie->play( "93002.avi", this, 0, 1, 0 );	   // helicopter lands
	END

	BEG
		GameFlag.set(fStartCombat);
		theGame->newRoom( new Rm95309);
	END
}

/*SouthExit1070::SouthExit1070()
{
	name = "SouthExit1070";
	setHotspot ( DO_V );
	init (46, 460, 495, 20);
}

void SouthExit1070::respond()
{
	theGame->setCursor(southCursor);
}

int SouthExit1070::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm1000);
    	return TRUE;
	}
	return FALSE;
}

Tundra3D1070::Tundra3D1070()
{
  	name = "Tundra";
	init (200, 150, 50, 50);   //bottom left
}

int Tundra3D1070::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
   		theGame->newRoom(new Rm93000);
    	return TRUE;
	}
	return FALSE;
}

JumpJet1070::JumpJet1070()
{
  	name = "JumpJet";
	init (300, 150, 50, 50);   //bottom left
}

int JumpJet1070::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
   	//	theGame->newRoom(new Rm70000);
    	return TRUE;
	}
	return FALSE;
}*/

