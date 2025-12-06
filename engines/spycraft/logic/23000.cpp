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
#include "spycraft/game/views.h"
#include "spycraft/logic/23000.h"
//#include "spycraft/logic/23100.h"
//#include "spycraft/logic/22300.h"
#include "spycraft/logic/1000.h"
#include "spycraft/game/flag.h"
#include "spycraft/game/movie.h"
#include "spycraft/game/vlink.h"

namespace Spycraft {

/*********************
*
*		Room Code 
*
**********************/

extern Intrface* intrface;
extern SouthCursor* southCursor;
//extern EastCursor* eastCursor;
//extern NorthCursor* northCursor;
extern DialTree* dialogTree;
extern Sound* sound1;
extern MADEEnvirons* colbyInfo;

Dialog* 	yingDialog1;
Dialog*		yingDialog2;
//Dialog*		yingDialog4;
Dialog4_23000*	yingDialog4;
Dialog*		yingDialog5;
Dialog*		yingDialog6;
Dialog*		yingDialog9;
Dialog*		yingDialog10;
Dialog*		yingDialog11;
Dialog*		yingDialog12;
Dialog* 	yingDialog15;
Dialog*		yingDialog18;
Dialog*		yingDialog19;
Dialog*		yingDialog23;
Dialog*		yingDialog24;
Dialog*		yingDialog25;
Dialog*		yingDialog28;
//Dialog* 	yingDialog31;
Dialog31_23000*	yingDialog31;
Dialog*		yingDialog32;
Dialog*		yingDialog33;
Dialog*		yingDialog35;
Dialog*		yingDialog36;
Dialog*		yingDialog37;
Dialog*		yingDialog40;
Dialog*		yingDialog41;
Dialog*		yingDialog42;
Dialog*		yingDialog43;

DItem* caughtGrendel;
DItem* scratchHere;
DItem* lookTalk;
DItem* thinkWayne;
DItem* lockedBoyfriend;
DItem* listenRussia;
DItem* intercomBullpen;
DItem* leaveWayne;
DItem* gettoughterYou;
DItem* notcoopGrendel;
DItem* dealtwoChoices;
DItem* iftalkFree; 
DItem* iftalkTen; 
DItem* iftalkLife; 
DItem* leavethinkDeal;
DItem* rolepegTheft; 
DItem* whoworkFor;
DItem* whattellSkodi;
DItem* whattellProcat;
DItem* firstrolePeg;
DItem* gotimeNow;
DItem* wipefaceCooperate;
DItem* liesoneLife; 
DItem* liesTen; 
DItem* liesbothLife; 
DItem* truthFree;
DItem* truthTen;
DItem* truthLife;
DItem* talkaboutSkodi;
DItem* goingspendMax;
DItem* firstwhoworkFor;
DItem* goTo28Ying;
DItem* goTo31Ying;
DItem* comeonKnow;
DItem* justintercom;

enum	
{
	CAUGHTGRENDEL,
	SCRATCHHERE,
	LOOKTALK,
	THINKWAYNE,
	LOCKEDBOYFRIEND,
	LISTENRUSSIA,
	INTERCOMBULLPEN,
	LEAVEWAYNE,
	GETTOUGHTERYOU,
	NOTCOOPGRENDEL,
	DEALTWOCHOICES,	
	IFTALKFREE,
	IFTALKTEN,
	IFTALKLIFE,
	LEAVETHINKDEAL,
	ROLEPEGTHEFT,
	WHOWORKFOR,
	WHATTELLSKODI,
	WHATTELLPROCAT,
	FIRSTROLEPEG,
	GOTTIMENOW,					
	WIPEFACECOOPERATE,
	LIESONELIFE,
	LIESTEN,
	LIESBOTHLIFE,
	TRUTHFREE,
	TRUTHTEN,
	TRUTHLIFE,
	TALKABOUTSKODI,
	GOINGSPENDMAX,
	FIRSTWHOWORKFOR,
	BLANK23000,
	COMEONKNOW,
	JUSTINTERCOM
};


Dialog* 	maxonintercomDialog1;
Dialog* 	maxonintercomDialog2;
Dialog* 	maxonintercomDialog3;
Dialog* 	maxonintercomDialog4;

DItem* doneyingNow;
DItem* moveyingBullpen;
DItem* tryagain;

enum	
{
	DONEYINGNOW,
	MOVEYINGBULLPEN,
	TRYAGAIN
};

Rm23000::Rm23000()
{
	name = "Rm23000";
	show_style = VE_FLIP;
//	show_style = VE_FULL_DISSOLVE;
}

void Rm23000::cue( void )
{
///	theMovie->fromTo(110, 113);
///	theMovie->play("23000.avi", this, 0, 0, 0);
}

Rm23000::~Rm23000()
{
	sound1->stop();  
	theMusic->stop();
	if( GameFlag.test( fDisposeDialog23000 )) {
		yingDialog1->dispose();
		yingDialog2->dispose();
		yingDialog4->dispose();
		yingDialog5->dispose();
		yingDialog6->dispose();
		yingDialog9->dispose();
		yingDialog10->dispose();
		yingDialog11->dispose();
		yingDialog12->dispose();
		yingDialog15->dispose();
		yingDialog18->dispose();
		yingDialog19->dispose();
		yingDialog23->dispose();
		yingDialog24->dispose();
		yingDialog25->dispose();
		yingDialog28->dispose();
		yingDialog31->dispose();
		yingDialog32->dispose();
		yingDialog33->dispose();
		yingDialog35->dispose();
		yingDialog36->dispose();
		yingDialog37->dispose();
		yingDialog40->dispose();
		yingDialog41->dispose();
		yingDialog42->dispose();
		yingDialog43->dispose();

		GameFlag.clear( fDisposeDialog23000 );
	}	//endif	for Dialog23000

	if( GameFlag.test( fDisposeDialog23002 )) {
		maxonintercomDialog1->dispose();
		maxonintercomDialog2->dispose();
		maxonintercomDialog3->dispose();
		maxonintercomDialog4->dispose();
		GameFlag.clear( fDisposeDialog23002 );
	}
}

void Rm23000::init()
{
    Room::init();
  
  	new(SouthExit23000);
// 	new(EastExit23000);
//	new(NorthExit23000);

  	intrface->show();
	
  	if( GameFlag.test(fStartInterrogation) && !GameFlag.test(fDoneInterrogation)){	
//		drawPic( 1 );
	   	setScript( new EnterScript23000 );
//		setScript( new EnterBlackScreenScript23001 );
	}
	else{
		drawPic( 23000 );
		theGame->handsOn();
	
		if( colbyInfo->slow_system == true )
			theMusic->play ( 23000 );
		else {
			if( sound1->number != 23000  || !sound1->isPlaying() )	{
				sound1->loop = -1; 
				sound1->playBuffered(23000, 0);
			}
		}
	}	//end else
}

int Rm23000::handleEvent ( MADEEventStamp *event )
{
	return false;
}

SouthExit23000::SouthExit23000()
{
	name = "SouthExit23000";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);  
   //init (46, 460, 495, 20);
}

void SouthExit23000::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit23000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
//		theGame->newRoom(new Rm22300);
		theGame->newRoom(new Rm1000);
    	return true;
	}
	return false;
}
/*
EastExit23000::EastExit23000()
{
	name = "EastExit23000";
	setHotspot ( DO_V );
	init ( 0, 0, 30, 330 );
}

void EastExit23000::respond()
{
  extern EastCursor* eastCursor;
	
  theGame->setCursor(eastCursor);
}

int EastExit23000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm22300);
    	return true;
	}
	return false;
}

NorthExit23000::NorthExit23000()
{
	name = "NorthExit23000";
	setHotspot ( DO_V );
//	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);  
    init (0, 0, 600, 40);
}

void NorthExit23000::respond()
{
	theGame->setCursor(northCursor);
}

int NorthExit23000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm23100);
    	return true;
	}
	return false;
}
*/
/*******************************
 *
 *	Scripts
 *
/*******************************/
EnterScript23000::EnterScript23000()
{
	name = "EnterScript23000"; 
}

void EnterScript23000::changeState( int newState )
{
	switchTo
		GameFlag.set(fDoneInterrogation);		
		GameFlag.set(fDisposeDialog23000);
		theMovie->fromTo(0, 113);
		theMovie->play("23000.avi", this, 0, 0, 0);
	END

	BEG
		dialogTree = new DialTree;
		yingDialog1 = new Dialog;
		yingDialog2 = new Dialog;
//		yingDialog4 = new Dialog;
		yingDialog4 = new Dialog4_23000;
		yingDialog5 = new Dialog;
		yingDialog6 = new Dialog;
		yingDialog9 = new Dialog;
		yingDialog10 = new Dialog;
		yingDialog11 = new Dialog;
		yingDialog12 = new Dialog;
		yingDialog15 = new Dialog;
		yingDialog18 = new Dialog;
		yingDialog19 = new Dialog;
		yingDialog23 = new Dialog;
		yingDialog24 = new Dialog;
		yingDialog25 = new Dialog;
		yingDialog28 = new Dialog;
//		yingDialog31 = new Dialog;
		yingDialog31 = new Dialog31_23000;
		yingDialog32 = new Dialog;
		yingDialog33 = new Dialog;
		yingDialog35 = new Dialog;
		yingDialog36 = new Dialog;
		yingDialog37 = new Dialog;
		yingDialog40 = new Dialog;
		yingDialog41 = new Dialog;
		yingDialog42 = new Dialog;
		yingDialog43 = new Dialog;

//		caughtGrendel = new DItem;
//		caughtGrendel->addInfo(23000, CAUGHTGRENDEL, yingDialog2);
		if(GameFlag.test(fWahKa115)) {
 			scratchHere = new DidPhotoAgain2;
			scratchHere->addInfo(23000, SCRATCHHERE, yingDialog43 );
		}
		else {
//			scratchHere = new DItem;
			scratchHere = new DidPhotoAgain;
			scratchHere->addInfo(23000, SCRATCHHERE, yingDialog6 );
		}
		
		if(GameFlag.test(fDidPhotoDoc)) {
			lookTalk = new DItem;
			lookTalk->addInfo(23000, LOOKTALK, yingDialog31 );			
		}
//		else if(GameFlag.test(fWahKa99)) {
		else if(GameFlag.test(fPrintedPhoto)) {
			if(GameFlag.test(fWahKa115)) {
				lookTalk = new DidPhotoAgain2;
				lookTalk->addInfo(23000, LOOKTALK, yingDialog42 );
			}
			else {
//				lookTalk = new DItem;
				lookTalk = new DidPhotoAgain;
				lookTalk->addInfo(23000, LOOKTALK, yingDialog41 );
			}
		}
		else {
			caughtGrendel = new DItem;
			caughtGrendel->addInfo(23000, CAUGHTGRENDEL, yingDialog2);
		}
		
		thinkWayne = new DItem;
		thinkWayne->addInfo(23000, THINKWAYNE, yingDialog5 );
		lockedBoyfriend = new DItem;
		lockedBoyfriend->addInfo(23000, LOCKEDBOYFRIEND, yingDialog4 );
		listenRussia = new DItem;
		listenRussia->addInfo(23000, LISTENRUSSIA, yingDialog10 );
if(!GameFlag.test(fLockOut)) {
//		intercomBullpen = new DItem;
		intercomBullpen = new VlinkMaxConversation;
//		intercomBullpen->addInfo(23000, INTERCOMBULLPEN, yingDialog36 );
		intercomBullpen->addInfo(23000, INTERCOMBULLPEN, NULL );
}
else {
		justintercom = new VlinkMaxConversation;
		justintercom->addInfo(23000, JUSTINTERCOM, NULL );
}
//		leaveWayne = new DItem;
		leaveWayne = new RestartInterrogation;
		leaveWayne->addInfo(23000, LEAVEWAYNE, NULL );
	
		gettoughterYou = new DItem;
		gettoughterYou->addInfo(23000, GETTOUGHTERYOU, yingDialog9);
		notcoopGrendel= new DItem;
		notcoopGrendel->addInfo(23000, NOTCOOPGRENDEL, yingDialog12 );	//topic135, use 35 for now, until next dialogue ready.
		dealtwoChoices = new DItem;
		dealtwoChoices->addInfo(23000, DEALTWOCHOICES, yingDialog15 );
		iftalkFree = new DItem;
		iftalkFree->addInfo(23000, IFTALKFREE, yingDialog18 );
		iftalkTen = new DItem;
		iftalkTen->addInfo(23000, IFTALKTEN, yingDialog32 );
		iftalkLife = new DItem;
		iftalkLife->addInfo(23000, IFTALKLIFE, yingDialog18 );
//		leavethinkDeal = new DItem;
		leavethinkDeal = new RestartInterrogation;
		leavethinkDeal->addInfo(23000, LEAVETHINKDEAL, NULL );
	
//		rolepegTheft = new DItem;
		rolepegTheft = new PegInfo;
		rolepegTheft->addInfo(23000, ROLEPEGTHEFT, yingDialog23);
//		whoworkFor= new DItem;
		whoworkFor = new PayCheck;
		whoworkFor->addInfo(23000, WHOWORKFOR, yingDialog24);
		whattellSkodi = new DItem;
		whattellSkodi->addInfo(23000, WHATTELLSKODI, yingDialog35);
		whattellProcat = new DItem;
////	whattellProcat->addInfo(23000, WHATTELLPROCAT, yingDialog25);
		whattellProcat->addInfo(23000, WHATTELLPROCAT, yingDialog40);
//		firstrolePeg = new DItem;
		firstrolePeg = new PegInfo;
		firstrolePeg->addInfo(23000, FIRSTROLEPEG, yingDialog23);	
		gotimeNow = new DItem;
		gotimeNow->addInfo(23000, GOTTIMENOW, NULL);
//		wipefaceCooperate = new DItem;
		wipefaceCooperate = new RestartInterrogation;
		wipefaceCooperate->addInfo(23000, WIPEFACECOOPERATE, NULL);		 
		liesoneLife = new DItem;
		liesoneLife->addInfo(23000, LIESONELIFE, yingDialog33);
		liesTen = new DItem;
		liesTen->addInfo(23000, LIESTEN, yingDialog18);
		liesbothLife= new DItem;
		liesbothLife->addInfo(23000, LIESBOTHLIFE, yingDialog18);
		truthFree = new DItem;
		truthFree->addInfo(23000, TRUTHFREE, yingDialog18);
//		truthTen = new DItem;
		truthTen = new BrokenYingInterrogation;
		truthTen->addInfo(23000, TRUTHTEN, yingDialog19);
		truthLife = new DItem;
		truthLife->addInfo(23000, TRUTHLIFE, yingDialog18);
////	talkaboutSkodi = new DItem;
		talkaboutSkodi = new EndBrokenYing;
		talkaboutSkodi->addInfo(23000, TALKABOUTSKODI, yingDialog37);
		goingspendMax = new DItem;
		goingspendMax->addInfo(23000, GOINGSPENDMAX, NULL);
//		firstwhoworkFor = new DItem;
		firstwhoworkFor = new PayCheck;
		firstwhoworkFor->addInfo(23000, FIRSTWHOWORKFOR, yingDialog24);
		goTo28Ying = new DItem;
		goTo28Ying->addInfo(23000, BLANK23000, yingDialog28 );
		goTo31Ying = new DItem;
//		goTo31Ying->addInfo(23000, BLANK23000, yingDialog31 );
		goTo31Ying->addInfo(23000, BLANK23000, yingDialog11 );
////	comeonKnow = new DItem;
		comeonKnow = new EndBrokenYing;
		comeonKnow->addInfo(23000, COMEONKNOW, yingDialog25 );
/*
		yingDialog1->addMovieInfo(0, 113, 0, 0);
		yingDialog2->addMovieInfo(120, 200, 0, 0);
		yingDialog4->addMovieInfo(263, 295, 0, 0);
		yingDialog5->addMovieInfo(300, 342, 0, 0); 
		yingDialog6->addMovieInfo(350, 387, 0, 0);
//ask about correct frame numbers.
//		yingDialog9->addMovieInfo(360, 750, 0, 0);
		yingDialog9->addMovieInfo(360, 391, 0, 0);
		yingDialog10->addMovieInfo(762, 888, 0, 0);
		yingDialog11->addMovieInfo(895, 942, 0, 0);
		yingDialog12->addMovieInfo(950, 1055, 0, 0); 
		yingDialog15->addMovieInfo(1184, 1353, 0, 0);
		yingDialog18->addMovieInfo(1625, 1685, 0, 0);
		yingDialog19->addMovieInfo(1690, 1760, 0, 0); 
		yingDialog23->addMovieInfo(2050, 2268, 0, 0);
//		yingDialog24->addMovieInfo(2273, 2450, 0, 0);
		yingDialog24->addMovieInfo(2273, 2409, 0, 0);
		yingDialog25->addMovieInfo(2457, 2553, 0, 0);
		yingDialog28->addMovieInfo(2656, 2697, 0, 0);
		yingDialog31->addMovieInfo(443, 525, 0, 0);
		yingDialog32->addMovieInfo(895, 945, 0, 0);
		yingDialog33->addMovieInfo(895, 945, 0, 0);
		yingDialog35->addMovieInfo(2400, 2445, 0, 0);
//ask about 36, wrong frame numbers.
		yingDialog36->addMovieInfo(2288, 2460, 0, 0);
		yingDialog37->addMovieInfo(2457, 2550, 0, 0);
		yingDialog40->addMovieInfo(2410, 2450, 0, 0);
		yingDialog41->addMovieInfo(443, 608, 0, 0);
		yingDialog42->addMovieInfo(1456, 1620, 0, 0);
		yingDialog43->addMovieInfo(2608, 2654, 0, 0);
*/
		yingDialog1->addMovieInfo(0, 117, 0, 0);
		yingDialog2->addMovieInfo(120, 207, 0, 0);
		yingDialog4->addMovieInfo(270, 307, 0, 0);
		yingDialog5->addMovieInfo(315, 362, 0, 0); 
		yingDialog6->addMovieInfo(375, 420, 0, 0);
		yingDialog9->addMovieInfo(375, 420, 0, 0);	  
		yingDialog10->addMovieInfo(825, 957, 0, 0);
		yingDialog11->addMovieInfo(960, 1013, 0, 0);
		yingDialog12->addMovieInfo(1020, 1129, 0, 0); 
		yingDialog15->addMovieInfo(1275, 1448, 0, 0);
		yingDialog18->addMovieInfo(1740, 1807, 0, 0);
		yingDialog19->addMovieInfo(1815, 1888, 0, 0); 
		yingDialog23->addMovieInfo(2205, 2424, 0, 0);
		yingDialog24->addMovieInfo(2430, 2561, 0, 0);
		yingDialog25->addMovieInfo(2625, 2721, 0, 0);
		yingDialog28->addMovieInfo(110, 113, 0, 0);
		yingDialog31->addMovieInfo(495, 580, 0, 0);
		yingDialog32->addMovieInfo(960, 1013, 0, 0);
		yingDialog33->addMovieInfo(960, 1013, 0, 0);
		yingDialog35->addMovieInfo(2555, 2614, 0, 0);
		yingDialog36->addMovieInfo(2430, 2561, 0, 0);
		yingDialog37->addMovieInfo(2625, 2720, 0, 0);
		yingDialog40->addMovieInfo(2555, 2614, 0, 0);
		yingDialog41->addMovieInfo(495, 661, 0, 0);
		yingDialog42->addMovieInfo(1560, 1724, 0, 0);
		yingDialog43->addMovieInfo(2790, 2836, 0, 0);


//		yingDialog1->add(caughtGrendel);
		yingDialog1->add(scratchHere);
		
//		if(GameFlag.test(fDidPhotoDoc) || GameFlag.test(fWahKa99)) {
		if(GameFlag.test(fDidPhotoDoc) || GameFlag.test(fPrintedPhoto)) {
			yingDialog1->add(lookTalk);
			yingDialog2->add(lookTalk);
			yingDialog5->add(lookTalk);
			yingDialog6->add(lookTalk);
			yingDialog10->add(lookTalk);
		}
		else {
			yingDialog1->add(caughtGrendel);
			yingDialog6->add(caughtGrendel);
			yingDialog10->add(caughtGrendel);
		}
		
		yingDialog2->add(thinkWayne);
		yingDialog2->add(scratchHere);
//		yingDialog2->add(lookTalk);
		yingDialog5->add(lockedBoyfriend);
		yingDialog5->add(scratchHere);
//		yingDialog5->add(lookTalk);
//		yingDialog6->add(caughtGrendel);
		yingDialog6->add(listenRussia);
//		yingDialog6->add(lookTalk);
if(!GameFlag.test(fLockOut))
		yingDialog9->add(intercomBullpen);
else
		yingDialog9->add(justintercom);
		yingDialog9->add(leaveWayne);
//		yingDialog9->add();					
//	   	yingDialog10->add(caughtGrendel);
		yingDialog10->add(gettoughterYou);
//		yingDialog10->add(lookTalk);
		yingDialog11->add(notcoopGrendel);
		yingDialog11->add(dealtwoChoices);
if(!GameFlag.test(fLockOut))
		yingDialog12->add(intercomBullpen);
else
		yingDialog12->add(justintercom);
		yingDialog12->add(dealtwoChoices);
		yingDialog15->add(iftalkFree);
		yingDialog15->add(iftalkTen);
		yingDialog15->add(iftalkLife);
if(!GameFlag.test(fLockOut))
		yingDialog18->add(intercomBullpen);
else
		yingDialog18->add(justintercom);
		yingDialog18->add(leavethinkDeal);
		yingDialog19->add(rolepegTheft);
		yingDialog19->add(whoworkFor);
		yingDialog23->add(whattellSkodi);
		yingDialog23->add(whoworkFor);
		yingDialog24->add(rolepegTheft);
		yingDialog24->add(whattellProcat);
		yingDialog25->add(firstrolePeg);
//		yingDialog25->add(gotimeNow);
		yingDialog28->add(gettoughterYou);
		yingDialog28->add(scratchHere);
		yingDialog28->add(wipefaceCooperate);
		yingDialog32->add(liesoneLife);
		yingDialog32->add(liesTen);
		yingDialog32->add(liesbothLife);
		yingDialog33->add(truthFree);
		yingDialog33->add(truthTen);
		yingDialog33->add(truthLife);
		yingDialog35->add(talkaboutSkodi);
		yingDialog35->add(whoworkFor);
//		yingDialog37->add(goingspendMax);
		yingDialog37->add(firstwhoworkFor);
		yingDialog40->add(rolepegTheft);
		yingDialog40->add(comeonKnow);
		yingDialog41->add(thinkWayne);
		yingDialog41->add(scratchHere);
		yingDialog42->add(thinkWayne);
		yingDialog42->add(scratchHere);
		yingDialog43->add(listenRussia);

		dialogTree->rootNode = yingDialog1;
		dialogTree->resourceID = "23000.avi";
		dialogTree->show(this);

	END

	BEG
//		GameFlag.set(fDoneInterrogation);	
		intrface->enable();
		theMovie->caller = 0;
		theMovie->stop();
//		new VLink( "25004.avi", 0, 10);
//		new VLink( "25007.avi", 0, 20);
//		new EMail( "Kipling", 98050);
//		theGame->newRoom(new Rm23000);

		if(GameFlag.test(fWahKa114)) {
			GameFlag.clear(fWahKa114);			//clear because otherwise, each time the player
			theGame->newRoom(new Rm23002);		//comes back, intercom is active
		}
		else {
			if(GameFlag.test(fBrokenYingInterrogation)) {
				new EMail( "Killers for hire: Seaton, Jaimie A.", 98070, fWahKa113, 0);		//Jaimie about the fake Procat file (fake?)
			}
			theGame->newRoom(new Rm1000);
		}
	END
}

/*******************************
 *
 *	Dialogue
 *
/*******************************/

void Dialog4_23000::cue( void )
{
	goTo28Ying->cue();
}

void Dialog31_23000::cue( void )
{
	goTo31Ying->cue();
}

VlinkMaxConversation::VlinkMaxConversation()
{
	name = "VlinkMaxConversation";
}

void VlinkMaxConversation::doit()
{
//	sfxPrintf("Topic 135 - Ask about Procat");
///	curRoom->setScript( new EnterIntercomScript23000 );
	GameFlag.set(fWahKa114);
	DItem::doit();
}

BrokenYingInterrogation::BrokenYingInterrogation()
{
	name = "BrokenYingInterrogation";
}

void BrokenYingInterrogation::doit()
{
	GameFlag.set(fBrokenYingInterrogation);
	GameFlag.set(fPyramidInfoAvailable);

	GameFlag.set(fWahKa32);					//link to vlink 657, Jaimie
///	new EMail( "Killers for hire: Seaton, Jaimie A.", 98070, fWahKa113, 0);//Jaimie about the fake Procat file (fake?)

//	new VLink( "25003.avi", 0, 0);			//vlink 654, Look into other thefts
//	new VLink( "25004.avi", 0, 10);			//vlink 655, Skodi details
//MOVED TO FLAG 12/16 new VLink( "Dangerous new weapon: Warhurst, Eugene J.", "25014.avi", 0, 15);			//vlink new, Skodi
//	GameFlag.set(fWahKa19);					//link to vlink 205, Warhurst
//	new VoiceMail("98072.wav", 98072, 0, 0);//Jaimie
//	new VLink( "Brooks cuts peace trip short: Newscast", "25015.avi", 0, 60);			//vlink 540 
//	new VLink( "25005.avi", 0, 60);			//vlink 205, Sterling heras about Procat
///	new VLink( "25006.avi", 0, 60);			//vlink 596, Warhurst commends
//	new VLink( "25008.avi", 0, 30);			//vlink 657, Jaimie implicates Holt
//	new VLink( "25009.avi", 0, 60);			//vlink 27, I didn't do it
//	new EMail( "Kipling", 98050);
	DItem::doit();
}

PegInfo::PegInfo()
{
	name = "PegInfo";
}

void PegInfo::doit()
{
	rolepegTheft->hasBeenChosen = true;
	firstrolePeg->hasBeenChosen = true;
	DItem::doit();
}
		   
PayCheck::PayCheck()
{
	name = "PayCheck";
}

void PayCheck::doit()
{
	whoworkFor->hasBeenChosen = true;
	firstwhoworkFor->hasBeenChosen = true;
	DItem::doit();
}
		   
RestartInterrogation::RestartInterrogation()
{
	name = "RestartInterrogation";
}

void RestartInterrogation::doit()
{
	GameFlag.clear(fDoneInterrogation);
	DItem::doit();
}

DidPhotoAgain::DidPhotoAgain()
{
	name = "DidPhotoAgain";
}

void DidPhotoAgain::doit()
{
	GameFlag.set(fWahKa115);
	DItem::doit();
}

DidPhotoAgain2::DidPhotoAgain2()
{
	name = "DidPhotoAgain2";
}

void DidPhotoAgain2::doit()
{
	GameFlag.clear(fWahKa115);
	DItem::doit();
}

EndBrokenYing::EndBrokenYing()
{
	name = "EndBrokenYing";
}

void EndBrokenYing::doit()
{
	if(rolepegTheft->hasBeenChosen && whoworkFor->hasBeenChosen) {
		yingDialog37->add(goingspendMax);
		yingDialog25->add(gotimeNow);
	}
	DItem::doit();
}

/* Room 23001 */
Rm23001::Rm23001()
{
	name = "Rm23001";
	show_style = VE_FLIP;
}

Rm23001::~Rm23001()
{
}

void Rm23001::init()
{
  	drawPic( 1 );
    Room::init();

  	intrface->show();
  	theGame->handsOn();

	setScript( new EnterScript23000 );
}

void Rm23001::cue( void )
{
	theMovie->fromTo(110, 113);
	theMovie->play("23000.avi", this, 0, 0, 0);
}

int Rm23001::handleEvent ( MADEEventStamp *event )
{
	return false;
}

EnterBlackScreenScript23001::EnterBlackScreenScript23001()
{
	name = "EnterBlackScreenScript23001"; 
}

void EnterBlackScreenScript23001::changeState( int newState )
{
//	GameFlag.set(fDoneInterrogation);			 
//	theGame->newRoom( new Rm23001 );
}
 

EnterIntercomScript23000::EnterIntercomScript23000()
{
	name = "EnterIntercomScript23000"; 
}

void EnterIntercomScript23000::changeState( int newState )
{
	switchTo
///		theMovie->caller = 0;
///		theMovie->stop();

		GameFlag.set(fDisposeDialog23002);
//		theMovie->fromTo(0, 2260);
///		theMovie->fromTo(0, 40);
		theMovie->fromTo(0, 31);
#ifdef DIRECTX
		theMovie->play("23002.avi", this, 0, -1, 0);
#else	
		theMovie->play("23002.avi", this, 0, 0, 0);
#endif
	END

	BEG
		dialogTree = new DialTree;
		dialogTree->negativeParam = true;
		maxonintercomDialog1 = new Dialog;
		maxonintercomDialog2 = new Dialog;
		maxonintercomDialog3 = new Dialog;
		maxonintercomDialog4 = new Dialog;
	
//		doneyingNow = new DItem;
		doneyingNow = new CheckBroken;
		doneyingNow->addInfo(23002, DONEYINGNOW, maxonintercomDialog3);
if(!GameFlag.test(fLockOut)) {
//		moveyingBullpen = new DItem;
		moveyingBullpen = new MovetoBullpen;
		moveyingBullpen->addInfo(23002, MOVEYINGBULLPEN, maxonintercomDialog2);
}
else {
		tryagain = new RestartInterrogation;
		tryagain->addInfo(23002, TRYAGAIN, maxonintercomDialog4);
}

//		maxonintercomDialog1->addMovieInfo(0, 2260, 0, 0);
//		maxonintercomDialog2->addMovieInfo(3300, 18310, 0, 0);
//		maxonintercomDialog3->addMovieInfo(19000, 26749, 0, 0);
		
///		maxonintercomDialog1->addMovieInfo(0, 40, 0, 0);
///		maxonintercomDialog2->addMovieInfo(41, 280, 0, 0);
///		maxonintercomDialog3->addMovieInfo(281, 401, 0, 0);
///		maxonintercomDialog4->addMovieInfo(281, 345, 0, 0);
		
		maxonintercomDialog1->addMovieInfo(0, 31, 0, 0);
		maxonintercomDialog2->addMovieInfo(45, 286, 0, 0);
		maxonintercomDialog3->addMovieInfo(300, 417, 0, 0);
		maxonintercomDialog4->addMovieInfo(300, 372, 0, 0);

		maxonintercomDialog1->add(doneyingNow);
if(!GameFlag.test(fLockOut))
		maxonintercomDialog1->add(moveyingBullpen);
else	
		maxonintercomDialog1->add(tryagain);
		

		dialogTree->rootNode = maxonintercomDialog1;
		dialogTree->resourceID = "23002.avi";
		dialogTree->show(this);
	END

	BEG
		intrface->enable();
		theMovie->caller = 0;
		theMovie->stop();
		theGame->newRoom(new Rm1000);
	END
}

/*******************************
 *
 *	Dialog
 *
/*******************************/

MovetoBullpen::MovetoBullpen()
{
	name = "MovetoBullpen";
}

void MovetoBullpen::doit()
{
	GameFlag.set(fStartBullpen);			//Start with the Bullpen first
	GameFlag.set(fDoneInterrogation);
	DItem::doit();
}

CheckBroken::CheckBroken()
{
	name = "CheckBroken";
}

void CheckBroken::doit()
{
	if(!GameFlag.test(fBrokenYingInterrogation)) {
		GameFlag.clear(fDoneInterrogation);
	}
	DItem::doit();
}

/* Room 23002 */
Rm23002::Rm23002()
{
	name = "Rm23002";
	show_style = VE_FLIP;
}

Rm23002::~Rm23002()
{
	if( GameFlag.test( fDisposeDialog23002 )) {
		maxonintercomDialog1->dispose();
		maxonintercomDialog2->dispose();
		maxonintercomDialog3->dispose();
		maxonintercomDialog4->dispose();
		GameFlag.clear( fDisposeDialog23002 );
	}
}

void Rm23002::init()
{
// 	drawPic( 1 );
    Room::init();

  	intrface->show();
// 	theGame->handsOn();

	setScript( new EnterIntercomScript23000 );
}

void Rm23002::cue( void )
{
}

int Rm23002::handleEvent ( MADEEventStamp *event )
{
	return false;
}

 
} // namespace Spycraft 
