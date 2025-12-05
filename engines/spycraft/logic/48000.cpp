/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "spycraft/game/globals.h"
#include "spycraft/game/game.h"
#include "spycraft/game/verbs.h"
#include "views.h"
#include "spycraft/logic/1000.h"
#include "spycraft/logic/48000.h"
#include "spycraft/logic/48400.h"
#include "spycraft/logic/48500.h"
#include "spycraft/game/sound.h"
#include "spycraft/game/movie.h"
#include "invent.h"
#include "spycraft/game/vlink.h"

namespace Spycraft {

#define CRSBLINK_SPEED 5

extern BlinkCursor* normalCursor;
extern Invent* inventry;
extern Flag GameFlag;
extern DialTree* dialogTree;
extern Intrface* intrface;
extern SouthCursor* southCursor;
extern Sound* sound1;
extern MADEEnvirons *colbyInfo;

/*********************
*
*		Room Code 
*
**********************/

Dialog* 	yuriDialog1;
Dialog2_48000*		yuriDialog2;
Dialog*		yuriDialog3;
Dialog*		yuriDialog4;
Dialog*		yuriDialog5;
//Dialog*		yuriDialog6;
Dialog*		yuriDialog7;
Dialog* 	yuriDialog8;
Dialog*		yuriDialog9;
Dialog*		yuriDialog10;
Dialog*		yuriDialog11;

DItem* photoSend;
DItem* churbanovCandidate;
DItem* tellMafia; 
DItem* tellmoreChurbanov; 
DItem* tellmoreMafia; 
DItem* thanksmeetYou; 
DItem* whataboutMafia;
DItem* problemsRussia;
DItem* governmentControl;
DItem* corruptionGovernment;
DItem* goTo10Yuri;

enum	
{
	PHOTOSEND,
	CHURBANOVCANDIDATE,
	TELLMAFIA,
	TELLMORECHURBANOV,
	TELLMOREMAFIA,
	THANKSMEETYOU,
	WHATABOUTMAFIA,
	PROBLEMSRUSSIA,
	GOVERNMENTCONTROL,
	CORRUPTIONGOVERNMENT,
	BLANK48000
};

Rm48000::Rm48000()
{
	name = "Rm48000";
	show_style = VE_FLIP;
}

void Rm48000::cue( void )
{
///	theMovie->fromTo(599, 600);
//	theMovie->play("48002.avi", this, 0, 0, 0);
///	theMovie->play("48003.avi", this, 0, 0, 0);
}

Rm48000::~Rm48000()
{
	sound1->stop(); 
	theMusic->stop();
	if( GameFlag.test( fDisposeDialog48000 )) {
		yuriDialog1->dispose();
		yuriDialog2->dispose();
		yuriDialog3->dispose();
		yuriDialog4->dispose();
		yuriDialog5->dispose();
//		yuriDialog6->dispose();
		yuriDialog7->dispose();
		yuriDialog8->dispose();
		yuriDialog9->dispose();
		yuriDialog10->dispose();
		yuriDialog11->dispose();

		GameFlag.clear( fDisposeDialog48000 );
	}	//endif
}

void Rm48000::init()
{
    Room::init();
 
  	new(SouthExit48000);
//	new(Photo48000);
//	new(Shelf48000);
//	new(Booze48000);
//	new(Desk48000);
//	new(PhotoHead48000);

	intrface->show();
//	theGame->handsOn();

	if( !GameFlag.test( fYORoomEnter )){
////	drawPic( 1 );
   		setScript( new EnterScriptIntro48000 );
	}
	else{
		if( GameFlag.test(fYORoomEnter) && GameFlag.test(fYuriGift) && !GameFlag.test( fOnceGift48000 )) {
			//drawPic( 48300 );
// 			setScript( new EnterScriptHarmonica48000 );
		}
		else if ( GameFlag.test(fYuriAboutOnyx) && !GameFlag.test(fOnceOnyx48000)) {
			//drawPic( 1 );
			setScript( new EnterScriptOnyx48000 );
		}
		else {
//			drawPic( 48000);
			drawPic( 1 );
			theGame->handsOn();
			
			if( colbyInfo->slow_system == true )
				theMusic->play ( 48000 );
			else {
				if( sound1->number != 48000  || !sound1->isPlaying() )	{
					sound1->loop = -1; 
					sound1->playBuffered(48000, 0);
				} //end if
			}

		}	//end else
    }   //end else
}

int Rm48000::handleEvent ( MADEEventStamp *event )
{
	return false;
}

SouthExit48000::SouthExit48000()
{
	name = "SouthExit48000";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);  
}

void SouthExit48000::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit48000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		theGame->newRoom(new Rm1000);
    	return true;
	}
	return false;
}

/*******************************
 *
 * Features
 *
 *******************************/

Photo48000::Photo48000()
{
	name = "Yuri's Family Photo";
   	setHotspot ( DO_V );
	init (142, 163, 25, 31);
}

void Photo48000::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Photo48000::doVerb (int theVerb)
{
	if ( theVerb == DO_V ) {
  		new(PhotoInset48000);
		return true;
	}
	return false;		
}

PhotoHead48000::PhotoHead48000()
{
	name = "Photo of head";
   	setHotspot ( DO_V );
	init (213, 156, 21, 40);
}

void PhotoHead48000::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int PhotoHead48000::doVerb (int theVerb)
{
	if ( theVerb == DO_V ) {
  		new(PhotoHeadInset48000);
		return true;
	}
	return false;		
}

Booze48000::Booze48000()
{
	name = "Yuri's Booz";
   	setHotspot ( DO_V );
	init (87, 243, 13, 42);
}

void Booze48000::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Booze48000::doVerb (int theVerb)
{
	if ( theVerb == DO_V ) {
	  	new(BoozeInset48000);
		return true;
	}
	return false;		
}

Shelf48000::Shelf48000()
{
	name = "Yuri's Shelf";
   	setHotspot ( DO_V );
	init (249, 114, 92, 130);
}

void Shelf48000::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Shelf48000::doVerb (int theVerb)
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm48200);
		return true;
	}
	return false;		
}

Desk48000::Desk48000()
{
	name = "Desk";
   	setHotspot ( DO_V );
	init (122, 320, 295, 80);
}

int Desk48000::doVerb (int theVerb)
{
	if ( theVerb == DO_V ) {
//	    theGame->newRoom(new Rm48300);		//showing gift box
    	return true;
	}
	return false;
}

/*******************************
 *
 *	Views
 *
 *******************************/

GiftBox48000::GiftBox48000()
{
	name = "Gift Box";
	setHotspot ( DO_V );
	setView(902);
	init ( 0, 0);
}

void GiftBox48000::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int GiftBox48000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
//    	new(Harmonica48000);
    	return true;
	}
	return false;
}

/*******************************
 *
 *	Insets
 *
 *******************************/

PhotoInset48000::PhotoInset48000()
{
	init(48040, 0, 16, 512, 383);
	GameFlag.set(fSeenYuriPhoto);
//	sfxPrintf("Yuri's family (8n/48017)");
//	theMovie->play("48017.avi", 0, 0, 640, 480);
}
/*
void PhotoInset48000::respond()
{
}

int PhotoInset48000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
//    	sfxPrintf("doVerb for inset");
    	return true;
	}
	return false;
}
*/
PhotoHeadInset48000::PhotoHeadInset48000()
{
	init(48060, 0, 16, 512, 383);
}
/*
void PhotoHeadInset48000::respond()
{
}

int PhotoHeadInset48000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
//    	sfxPrintf("doVerb for inset");
    	return true;
	}
	return false;
}
*/
BoozeInset48000::BoozeInset48000()
{
	init(48020, 0, 16, 512, 383);
}
/*
void BoozeInset48000::respond()
{
}

int BoozeInset48000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
//    	sfxPrintf("doVerb for inset");
    	return true;
	}
	return false;
}
*/
/*******************************
 *
 *	Scripts
 *
 *******************************/

EnterScriptIntro48000::EnterScriptIntro48000()
{
	name = "EnterScriptIntro48000"; 
}

void EnterScriptIntro48000::changeState( int newState )
{
	switchTo
//		GameFlag.set(fYORoomEnter);
		GameFlag.set(fDisposeDialog48000);
///		intrface->disable();
///		theGame->handsOff();
//		theMovie->fromTo(0, 600);
//		theMovie->play("48000.avi", this, 0, 1, 1);
//		theMovie->play("48002.avi", this, 0, 0, 0);
		theMovie->fromTo(0, 612);
		theMovie->play("48003.avi", this, 0, 0, 0);
	END

	BEG
///		curRoom->cue();
///		theGame->handsOn();
		
		dialogTree = new DialTree;
		yuriDialog1 = new Dialog;
		yuriDialog2 = new Dialog2_48000;
		yuriDialog3 = new Dialog;
		yuriDialog4 = new Dialog;
		yuriDialog5 = new Dialog;
//		yuriDialog6 = new Dialog;
		yuriDialog7 = new Dialog;
		yuriDialog8 = new Dialog;
		yuriDialog9 = new Dialog;
		yuriDialog10 = new Dialog;
		yuriDialog11 = new Dialog;

		photoSend = new DItem;
//		photoSend = new PhotoMaxSend;
		photoSend->addInfo(48000, PHOTOSEND, yuriDialog2);
		churbanovCandidate= new DItem;
		churbanovCandidate->addInfo(48000, CHURBANOVCANDIDATE, yuriDialog4 );
//		tellMafia = new DItem;
		tellMafia = new MafiaInfo;
		tellMafia->addInfo(48000, TELLMAFIA, yuriDialog7 );
///		tellMafia->addInfo(48000, TELLMAFIA, yuriDialog8 );
		tellmoreChurbanov = new DItem;
		tellmoreChurbanov->addInfo(48000, TELLMORECHURBANOV, yuriDialog5 );
//		tellmoreMafia = new DItem;
		tellmoreMafia = new MafiaInfo;
		tellmoreMafia->addInfo(48000, TELLMOREMAFIA, yuriDialog7 );
		thanksmeetYou = new DItem;
		thanksmeetYou->addInfo(48000, THANKSMEETYOU, yuriDialog11 );
//		whataboutMafia = new DItem;
		whataboutMafia = new MafiaInfo;
		whataboutMafia->addInfo(48000, WHATABOUTMAFIA, yuriDialog7 );
//		problemsRussia = new DItem;
		problemsRussia = new ProblemsInfo;
		problemsRussia->addInfo(48000, PROBLEMSRUSSIA, yuriDialog3 );
		governmentControl = new DItem;
		governmentControl->addInfo(48000, GOVERNMENTCONTROL, yuriDialog8 );
//		corruptionGovernment = new DItem;
		corruptionGovernment = new ProblemsInfo;
		corruptionGovernment->addInfo(48000, CORRUPTIONGOVERNMENT, yuriDialog3 );
		goTo10Yuri = new DItem;
		goTo10Yuri->addInfo(48000, BLANK48000, yuriDialog10 );		//after yuriDialog2

		yuriDialog1->addMovieInfo(0, 612, 0, 0);
		yuriDialog2->addMovieInfo(615, 1872, 0, 0);
		yuriDialog3->addMovieInfo(1920, 2087, 0, 0);
		yuriDialog4->addMovieInfo(2100, 2253, 0, 0); 
//		yuriDialog5->addMovieInfo(2265, 2469, 0, 0);
		yuriDialog5->addMovieInfo(2265, 2400, 0, 0);
//		yuriDialog7->addMovieInfo(2475, 2686, 0, 0);
///		yuriDialog7->addMovieInfo(2475, 2600, 0, 0);
		yuriDialog7->addMovieInfo(2700, 3055, 0, 0);
///		yuriDialog8->addMovieInfo(2700, 3055, 0, 0);
		yuriDialog8->addMovieInfo(3066, 3437, 0, 0);
		yuriDialog9->addMovieInfo(3066, 3437, 0, 0);
		yuriDialog10->addMovieInfo(575, 600, 0, 0);
//		yuriDialog11->addMovieInfo(3450, 4075, 0, 0);
		yuriDialog11->addMovieInfo(2400, 2469, 0, 0);

		yuriDialog1->add(photoSend);
//		yuriDialog2->add(tellMafia);		   //changed movie 48003,
//		yuriDialog2->add(problemsRussia);	   //from 48002 to 48003
		yuriDialog3->add(churbanovCandidate);
		yuriDialog3->add(tellMafia);
		yuriDialog4->add(tellmoreChurbanov);
		yuriDialog4->add(tellmoreMafia);        //same as tellMafia
		yuriDialog5->add(thanksmeetYou);
		yuriDialog5->add(whataboutMafia);		//same as tellMafia
		yuriDialog7->add(problemsRussia);
		yuriDialog7->add(governmentControl);
///		yuriDialog7->add(churbanovCandidate);	//added Churbanov
		yuriDialog8->add(corruptionGovernment);	//same as problemsRussia
////	yuriDialog8->add(churbanovCandidate);	//added Churbanov
////	yuriDialog8->add(thanksmeetYou);
///		yuriDialog9->add(thanksmeetYou);
///		yuriDialog9->add(churbanovCandidate);	//added Churbanov
		yuriDialog10->add(tellMafia);
		yuriDialog10->add(problemsRussia);

		dialogTree->rootNode = yuriDialog1;
		dialogTree->resourceID = "48003.avi";
		dialogTree->show(this);
	END

	BEG
		GameFlag.set(fYORoomEnter);
		GameFlag.clear(fDCK2);					//make Yasevno not available in travelink
		theMovie->caller = 0;
		theMovie->stop();
		intrface->enable();
//		new VLink ( "Strongman Polyakov?: Newscast", "48006.avi", fWahKa68, 60 );//vlink 542 - BBC World Sofkinsky strong air
		theGame->newRoom( new Rm1000 );
	END
}

//Harmonica's gift
EnterScriptHarmonica48000::EnterScriptHarmonica48000()
{
	name = "EnterScriptHarmonica48000"; 
}

void EnterScriptHarmonica48000::changeState( int newState )
{
	switchTo
		GameFlag.set(fOnceGift48000);
		GameFlag.clear(fDCK2);				//make Yasevno available in travelink		
///		intrface->disable();
///		theGame->handsOff();
		theMovie->fromTo(0, 147);
		theMovie->play("48001.avi", this, 0, 1, 1);
	END
	BEG
		theGame->handsOn();
//		new(Harmonica48000);
		theGame->newRoom( new Rm48300 );
//		sfxPrintf("Topic #114, about Shock");

	END
}
 
/*******************************
 *
 *	Dialogue
 *
/*******************************/

void Dialog2_48000::cue( void )
{
	goTo10Yuri->cue();
}

MafiaInfo::MafiaInfo()
{
	name = "MafiaInfo";
}

void MafiaInfo::doit()
{
	tellMafia->hasBeenChosen = true;
	tellmoreMafia->hasBeenChosen = true;
	whataboutMafia->hasBeenChosen = true;
	
	if(problemsRussia->hasBeenChosen)
		yuriDialog7->add(churbanovCandidate);
	DItem::doit();				  
}

ProblemsInfo::ProblemsInfo()
{
	name = "ProblemsInfo";
}

void ProblemsInfo::doit()
{
	problemsRussia->hasBeenChosen = true;
	corruptionGovernment->hasBeenChosen = true;
	DItem::doit();				  
}
/*
PhotoMaxSend::PhotoMaxSend()
{
	name = "PhotoMaxSend";
}

void PhotoMaxSend::doit()
{
	DItem::doit();				  //must change, cut to movie6
}

void PhotoMaxSend::cue()
{
//	photoSend->addInfo(48000, PHOTOSEND, yuriDialog2);		
//	DItem::cue();
	photoSend->addInfo(48000, PHOTOSEND, yuriDialog10 );     
	DItem::cue();
}
*/

/*********************
*
*		Room Code 
*
**********************/

//closer to book shelf
Rm48200::Rm48200()
{
	name = "Rm48200";
	show_style = VE_FLIP;
}

void Rm48200::init()
{
  	drawPic( 48200);
    Room::init();
  	
	new Softball48200;
	new SouthExit48200;

	intrface->show();
  	theGame->handsOn();
}

SouthExit48200::SouthExit48200()
{
	name = "SouthExit48200";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);  
}

void SouthExit48200::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit48200::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm48000);
    	return true;
	}
	return false;
}

Softball48200::Softball48200()
{
	name = "Softball";
	setHotspot ( DO_V );
	init (239, 294, 60, 53);
}

int Softball48200::doVerb (int theVerb)
{
	if ( theVerb == DO_V ) { 
		GameFlag.set(fSeenSoftball);
		new BallInset48200;
    	return true;
	}
	return false;
}

BallInset48200::BallInset48200()
{
	init(48220, 0, 16, 512, 384);
}
/*
int BallInset48200::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
//    	sfxPrintf("doVerb for inset");
    	return true;
	}
	return false;
}
*/
/*********************
*
*		Room Code 
*
**********************/

//Closer to desk
Rm48300::Rm48300()
{
	name = "Rm48300";
	show_style = VE_FLIP;
}

void Rm48300::init()
{
//  drawPic( 48300 );
	drawPic( 48100 );
    Room::init();
  	
//	new Softball48200;
	new SouthExit48300;
	new Box48300;
//	if(!fHarmonicaTouche)
//		new Box48300;

	intrface->show();
  	theGame->handsOn();
}

SouthExit48300::SouthExit48300()
{
	name = "SouthExit48300";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);  
}

void SouthExit48300::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit48300::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		theGame->newRoom(new Rm1000);
    	return true;
	}
	return false;
}

Box48300::Box48300()
{
	name = "Box";
	setHotspot ( DO_V );
//	init (174, 321, 90, 50);
	init (169, 309, 101, 72);
}

void Box48300::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Box48300::doVerb (int theVerb)
{
	if ( theVerb == DO_V ) {
		if( !GameFlag.test( fOnceShocked48000 )) {	    
			setScript( new EnterScript48300 );
		}
    	return true;
	}
	return false;
}

HarmInset48300::HarmInset48300()
{
	init(48320, 0, 16, 512, 384);
}

int HarmInset48300::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		return true;
	}
	return false;
}

/*******************************
 *
 *	Scripts
 *
/*******************************/

EnterScript48300::EnterScript48300()
{
	name = "EnterScript48300"; 
}

void EnterScript48300::changeState( int newState )
{
	switchTo
		GameFlag.set(fOnceShocked48000);			 
		theGame->handsOff();
		theMovie->play("48300.avi", this, 0, 1, 1);
	END
	BEG
		theGame->newRoom(new Rm48400);
	END
}

EnterScriptOnyx48000::EnterScriptOnyx48000()
{
	name = "EnterScriptOnyx48000"; 
}

void EnterScriptOnyx48000::changeState( int newState )
{
	switchTo
//		GameFlag.set(fOnceOnyx48000);
//		GameFlag.clear(fDCK2);					//make Yasevno not available in travelink			 
		ticks = 2;
	END
	BEG
		theGame->newRoom(new Rm48500);
	END
}
 
 
} // namespace Spycraft 
