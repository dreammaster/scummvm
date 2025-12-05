//Started by Ka Wah
//Dialog - Warn Max about Harmonica in the alley
//Copyright Activision 1995
//Finished by Ka Wah

#include "globals.h"
#include "game.h"
#include "verbs.h"
#include "views.h"
#include "35700.h"
#include "1000.h"
#include "flag.h"
#include "movie.h"
#include "invent.h"
#include "32000.h"
  
Dialog* 	maxtrapharmDialog1;
Dialog* 	maxtrapharmDialog2;

DItem* talktoMax;
DItem* gomeetHarm;

enum	
{
	TALKTOMAX,
	GOMEETHARM,
};

/*********************
*
*		Room Code 
*
**********************/

extern Intrface* intrface;
extern DialTree* dialogTree;
extern Invent* inventry;
extern Flag GameFlag;

Rm35700::Rm35700()
{
	name = "Rm35700";
	show_style = VE_FLIP;
}

void Rm35700::cue( void )
{
//	theMovie->fromTo(3, 6);
//	theMovie->play("33000.avi", this, 0, 0, 0);
}

void Rm35700::init()
{
	drawPic( 1 );
    Room::init();
  
  	intrface->show();
// 	theGame->handsOn();

	setScript( new ScriptMaxSetsTrap35700 );
}

Rm35700::~Rm35700()
{ 
	if( GameFlag.test( fDisposeDialog35700 )) {
		maxtrapharmDialog1->dispose();
		maxtrapharmDialog2->dispose();

		GameFlag.clear( fDisposeDialog35700 );
	}	//endif
}

int Rm35700::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}

/*******************************
 *
 *	Scripts
 *
 *******************************/

ScriptMaxSetsTrap35700::ScriptMaxSetsTrap35700()
{
	name = "ScriptMaxSetsTrap35700"; 
}

void ScriptMaxSetsTrap35700::changeState( int newState )
{
	switchTo
		GameFlag.set(fDisposeDialog35700);
//		GameFlag.set(fWahKa62);
		intrface->disable();
////	theMovie->fromTo(0, 3);
////	theMovie->play("33000.avi", this, 0, 0, 0);
		theMovie->fromTo(655, 665);
		theMovie->play("1001.avi", this, 0, 1, 0);
	END

	BEG
		dialogTree = new DialTree;
		maxtrapharmDialog1 = new Dialog;
		maxtrapharmDialog2 = new Dialog;

//		talktoMax = new DItem;
		talktoMax = new MaxSetTrap;
		talktoMax->addInfo(33000, TALKTOMAX, maxtrapharmDialog2);
//		gomeetHarm= new DItem;
		gomeetHarm = new MeetHarmonica;
//		gomeetHarm->addInfo(33000, GOMEETHARM, maxtrapharmDialog2);
		gomeetHarm->addInfo(33000, GOMEETHARM, NULL);

//		maxtrapharmDialog1->addMovieInfo(3, 228, 0, 0);
		maxtrapharmDialog2->addMovieInfo(0, 228, 0, 0);	
		
		maxtrapharmDialog1->add(talktoMax);
		maxtrapharmDialog1->add(gomeetHarm);

		dialogTree->rootNode = maxtrapharmDialog1;
		dialogTree->resourceID = "33000.avi";
		dialogTree->show(this);
	END

	BEG
		GameFlag.set(fWahKa62);
		intrface->enable();
		theMovie->caller = 0;
		theMovie->stop();
/*
		if(GameFlag.test(fWahKa61))
			theGame->newRoom(new Rm32000);	
		else
//			theGame->newRoom(new Rm1000);
			theGame->newRoom(new Rm32000);
*/
		if(GameFlag.test(fWahKa61)) {
//			theGame->newRoom(new Rm32000);	
			theGame->setScript( new AnotherScript35700 );
			dispose();
		}
		else
			theGame->newRoom(new Rm32000);
	END
}

AnotherScript35700::AnotherScript35700()
{
	name = "AnotherScript35700"; 
}

void AnotherScript35700::changeState( int newState )
{	   
	switchTo
		ticks = 2;
	END
	BEG
		theMovie->fromTo(670, 698);
		theMovie->play("1001.avi", this, 0, 1, 0);
	END
	BEG
		theGame->newRoom(new Rm32000);
	END
}

/*******************************
 *
 *	Dialog
 *
/*******************************/
MaxSetTrap::MaxSetTrap()
{
	name = "MaxSetTrap";
}

void MaxSetTrap::doit()
{
	GameFlag.set(fWahKa60);	   //Max sets trap for Harmonica
	DItem::doit();
}

MeetHarmonica::MeetHarmonica()
{
	name = "MeetHarmonica";
}

void MeetHarmonica::doit()
{
	GameFlag.set(fWahKa61);	   //didn't warn Max about Harmonica's call
	DItem::doit();
}
  
