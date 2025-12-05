//Started by Ka Wah
//Foster's office, Ying
//Copyright Activision 1995
//Finished by Ka Wah

#include "invent.h"
#include "globals.h"
#include "game.h"
#include "verbs.h"
#include "views.h"
#include "36700.h"
#include "1000.h"
//#include "23000.h"
//#include "25000.h"
#include "flag.h"
#include "invent.h"
#include "movie.h"

#define CRSBLINK_SPEED 5

/*********************
*
*		Room Code 
*
**********************/

extern DialTree *dialogTree;
extern Invent *inventry;
extern Intrface *intrface;
extern Flag GameFlag;
extern BlinkCursor *normalCursor;

Dialog*		maxDialog9;
Dialog*		maxDialog10;
Dialog*		maxDialog11;
Dialog*		maxDialog12;
Dialog*		maxDialog13;
Dialog*		maxDialog14;
Dialog*		maxDialog18;

DItem* thanksHer;
DItem* moveBullpen;
DItem* wantBullpen;
DItem* fairinterrogation;


enum	
{
	THANKSHER,
	MOVEBULLPEN,
	WANTBULLPEN,
	FAIRINTERROGATION,
};

Rm36700::Rm36700()
{
	name = "Rm36700";
	show_style = VE_FLIP;
}

void Rm36700::init()
{
    Room::init();
  	intrface->show();
	
//	if( GameFlag.test( fMORoomEnter )){
//		drawPic( 1 );
   		setScript( new EnterScript36700 );
//	}
//	else{
//		drawPic( 1 );
//		theGame->handsOn();
//	}
}

void Rm36700::cue( void )
{
///	theMovie->fromTo(1522, 1523);
///	theMovie->play("36001.avi", this, 0, 0, 0);

//	theMovie->fromTo(2648, 2787);
///	theMovie->fromTo(2750, 2887);
	theMovie->fromTo(2750, 2847);
	theMovie->play("36001.avi", curRoom->script, 0, 0, 0);
}

Rm36700::~Rm36700( void )
{
	if( GameFlag.test( fDisposeDialog36700 )) {
		maxDialog9->dispose();
		maxDialog10->dispose();
		maxDialog11->dispose();
		maxDialog12->dispose();
		maxDialog13->dispose();
		maxDialog14->dispose();
		maxDialog18->dispose();
	
		GameFlag.clear( fDisposeDialog36700 );
	} //endif
}	

int Rm36700::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}

/*******************************
 *
 *	Scripts
 *
 *******************************/

EnterScript36700::EnterScript36700()
{
	name = "EnterScript36700"; 
}

void EnterScript36700::changeState( int newState )
{
	switchTo
		GameFlag.set(fDisposeDialog36700);
//		GameFlag.set(fWahKa40);
//		GameFlag.set(fMetMaxFoster);	

///		intrface->disable();
///		theGame->handsOff();
///		if( GameFlag.test(fKnowYingInMoscow )) {
		if( GameFlag.test(fMORoomEnter)) {
//			theMovie->fromTo(987, 1100);   //met Max
///			theMovie->fromTo(1005, 1112);   //met Max
////		theMovie->fromTo(1005, 1017);   //met Max
////		theMovie->play("36001.avi", curRoom, 0, 0, 0);
			theMovie->fromTo(2750, 2847);
			theMovie->play("36001.avi", this, 0, 0, 0);
		}
		else {
//			theMovie->fromTo(987, 1120);   //not met Max
///			theMovie->fromTo(1005, 1140);   //not met Max
			theMovie->fromTo(1005, 1052);   //not met Max
			theMovie->play("36001.avi", curRoom, 0, 0, 0);	
		}
			
	END

	BEG
///		curRoom->cue();
///		theGame->handsOn();

		dialogTree = new DialTree;
		maxDialog9 = new Dialog;
		maxDialog10 = new Dialog;
		maxDialog11 = new Dialog;
		maxDialog12 = new Dialog;
		maxDialog13 = new Dialog;
		maxDialog14 = new Dialog;
		maxDialog18 = new Dialog;

//		thanksHer = new DItem;
		thanksHer = new DowntoInterrogation;
		thanksHer->addInfo(36700, THANKSHER, maxDialog9);
if(!GameFlag.test(fLockOut)) {		
		moveBullpen = new DItem;
		moveBullpen->addInfo(36700, MOVEBULLPEN, maxDialog10);
}
//		wantBullpen = new DItem;
		wantBullpen = new DowntoBullpen;
		wantBullpen->addInfo(36700, WANTBULLPEN, maxDialog11);
//		fairinterrogation = new DItem;
		fairinterrogation = new DowntoInterrogation;
		fairinterrogation->addInfo(36700, FAIRINTERROGATION, maxDialog12);
/*
		maxDialog9->addMovieInfo(1947, 2030, 2029, 2030);
		maxDialog10->addMovieInfo(2048, 2288, 2287, 2288);
		maxDialog11->addMovieInfo(2298, 2455, 2454, 2455);
		maxDialog12->addMovieInfo(2475, 2628, 2627, 2628);
		maxDialog13->addMovieInfo(987, 1120, 1119, 1120);
		maxDialog14->addMovieInfo(2648, 2787, 2786, 2787);
		maxDialog18->addMovieInfo(987, 1100, 1099, 1100);
*/
		maxDialog9->addMovieInfo(1980, 2085, 0, 0);
		maxDialog10->addMovieInfo(2100, 2352, 0, 0);
		maxDialog11->addMovieInfo(2355, 2543, 0, 0);
		maxDialog12->addMovieInfo(2550, 2740, 0, 0);
///		maxDialog13->addMovieInfo(1005, 1140, 0, 0);
		maxDialog13->addMovieInfo(1005, 1052, 0, 0);
		maxDialog14->addMovieInfo(2840, 2887, 0, 0);
///		maxDialog18->addMovieInfo(1005, 1112, 0, 0);
		maxDialog18->addMovieInfo(1005, 1017, 0, 0);
											   
		maxDialog10->add(wantBullpen);
		maxDialog10->add(fairinterrogation);
		maxDialog14->add(thanksHer);
if(!GameFlag.test(fLockOut))
		maxDialog14->add(moveBullpen);
		
		dialogTree->rootNode = maxDialog14;
		dialogTree->resourceID = "36001.avi";
		dialogTree->show(this);
	END

	BEG
		GameFlag.set(fWahKa40);
		GameFlag.set(fMetMaxFoster);	

		intrface->enable();
		theMovie->caller = 0;
		theMovie->stop();
		GameFlag.set(fMORoomEnter);
//		if(GameFlag.test(fStartBullpen))
//			theGame->newRoom( new Rm25000 );
//		else if(GameFlag.test(fStartInterrogation))									  
//			theGame->newRoom( new Rm23000 );
//		else
			theGame->newRoom( new Rm1000 );
	END
}

/*******************************
 *
 *	Dialog
 *
/*******************************/

DowntoBullpen::DowntoBullpen()
{
	name = "DowntoBullpen";
}

void DowntoBullpen::doit()
{
	GameFlag.set(fStartBullpen);			//Start with the Bullpen first
	DItem::doit();
}

DowntoInterrogation::DowntoInterrogation()
{
	name = "DowntoInterrogation";
}

void DowntoInterrogation::doit()
{
	GameFlag.set(fStartInterrogation);		//Start with the Interrogation first
	DItem::doit();
}
 
} // namespace Spycraft 
