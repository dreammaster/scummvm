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

#include "invent.h"
#include "globals.h"
#include "game.h"
#include "verbs.h"
#include "views.h"
#include "36500.h"
#include "1000.h"
#include "flag.h"
#include "movie.h"
#include "vlink.h"

namespace Spycraft {

#define CRSBLINK_SPEED 5

/*********************
*
*		Room Code 
*
**********************/

extern DialTree* dialogTree;
extern Invent* inventry;
extern Intrface* intrface;
extern Flag GameFlag;
extern SouthCursor* southCursor;
extern BlinkCursor* normalCursor;

Dialog* 	maxwKatDialog1;
Dialog*		maxwKatDialog2;
Dialog*		maxwKatDialog3;
Dialog*		maxwKatDialog4;
Dialog*		maxwKatDialog5;
Dialog*		maxwKatDialog6;
Dialog*		maxwKatDialog7;

DItem* whoYuri2;
DItem* whatNoise3; 
DItem* whereEveryone3; 
DItem* hangHat3; 
DItem* whatSVR2; 
DItem* howContact2; 
DItem* thanksHer3;

enum	
{
	WHO_YURI2,
	WHAT_NOISE3,
	WHERE_EVERYONE3,
	HANG_HAT3,
	WHAT_SVR2,
	HOW_CONTACT2,
	THANKS_HER3,
};

Rm36500::Rm36500()
{
	name = "Rm36500";
	show_style = VE_FLIP;
}

void Rm36500::init()
{
    Room::init();
  	intrface->show();

	new(SouthExit36500);
	
	if( GameFlag.test( fMORoomEnter ) && GameFlag.test(fWahKa84)){
//		drawPic( 1 );
   		setScript( new EnterScript36500 );
	}
	else{
		drawPic( 36000 );
		theGame->handsOn();
	}
}

void Rm36500::cue( void )
{
///	theMovie->fromTo(1522, 1523);
///	theMovie->play("36001.avi", this, 0, 0, 0);
}

Rm36500::~Rm36500( void )
{
	if( GameFlag.test( fDisposeDialog36500 )) {
		maxwKatDialog1->dispose();
		maxwKatDialog2->dispose();
		maxwKatDialog3->dispose();
		maxwKatDialog4->dispose();
		maxwKatDialog5->dispose();
		maxwKatDialog6->dispose();
		maxwKatDialog7->dispose();
	
		GameFlag.clear( fDisposeDialog36500 );
	} //endif
}	

int Rm36500::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}

SouthExit36500::SouthExit36500()
{
	name = "SouthExit36500";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
}

void SouthExit36500::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit36500::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm1000);
    	return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Scripts
 *
 *******************************/

EnterScript36500::EnterScript36500()
{
	name = "EnterScript36500"; 
}

void EnterScript36500::changeState( int newState )
{
	switchTo
//		GameFlag.set(fMORoomEnter);
//		GameFlag.set(fBSMeetingSet);   //DEBUG change flag later...also in 1000.cpp
		GameFlag.set(fDisposeDialog36500);
//		GameFlag.set(fWahKa41);
//		GameFlag.set(fMetMaxFoster);

///		intrface->disable();
///		theGame->handsOff();
//		theMovie->fromTo(1370, 1523);
///		theMovie->fromTo(1365, 1521); 
		theMovie->fromTo(1282, 1446); 
		theMovie->play("36001.avi", this, 0, 0, 0);	
	END

	BEG
///		curRoom->cue();
///		theGame->handsOn();
		
		dialogTree = new DialTree;
		maxwKatDialog1 = new Dialog;
		maxwKatDialog2 = new Dialog;
		maxwKatDialog3 = new Dialog;
		maxwKatDialog4 = new Dialog;
		maxwKatDialog5 = new Dialog;
		maxwKatDialog6 = new Dialog;
		maxwKatDialog7 = new Dialog;

		whoYuri2= new DItem;
		whoYuri2->addInfo(36500, WHO_YURI2, maxwKatDialog4 );
		whatNoise3 = new DItem;
		whatNoise3->addInfo(36500, WHAT_NOISE3, maxwKatDialog3 );
		whereEveryone3 = new DItem;
		whereEveryone3->addInfo(36500, WHERE_EVERYONE3, maxwKatDialog1 );

if(!GameFlag.test(fWahKa31)) {
//		hangHat3 = new DItem;
		hangHat3 = new DoneHaveHat3;
		hangHat3->addInfo(36500, HANG_HAT3, maxwKatDialog2 );
		
		maxwKatDialog5->add(hangHat3);
		maxwKatDialog6->add(hangHat3);	
}
//		whatSVR2 = new DItem;
		whatSVR2 = new AgainThanks3;
		whatSVR2->addInfo(36500, WHAT_SVR2, maxwKatDialog6);
//		howContact2 = new DItem;
		howContact2 = new NumberofYuri3;
		howContact2->addInfo(36500, HOW_CONTACT2, maxwKatDialog5);
		thanksHer3 = new DItem;
//		thanksHer3->addInfo(36500, THANKS_HER3, maxwKatDialog7);  //temp to test
		thanksHer3->addInfo(36500, THANKS_HER3, NULL);
/*
		maxwKatDialog1->addMovieInfo(7, 125, 124, 125);
		maxwKatDialog2->addMovieInfo(132, 263, 262, 263);
		maxwKatDialog3->addMovieInfo(275, 513, 512, 513);
		maxwKatDialog4->addMovieInfo(537, 747, 746, 747);
		maxwKatDialog5->addMovieInfo(758, 836, 835, 836);
		maxwKatDialog6->addMovieInfo(863, 972, 971, 972);
		maxwKatDialog7->addMovieInfo(1370, 1523, 1522, 1523);
*/	
		maxwKatDialog1->addMovieInfo(0, 124, 0, 0);
		maxwKatDialog2->addMovieInfo(135, 274, 0, 0);
		maxwKatDialog3->addMovieInfo(285, 539, 0, 0);
		maxwKatDialog4->addMovieInfo(540, 759, 0, 0);
		maxwKatDialog5->addMovieInfo(765, 867, 0, 0); 
		maxwKatDialog6->addMovieInfo(870, 996, 0, 0);
///		maxwKatDialog7->addMovieInfo(1365, 1521, 0, 0);
		maxwKatDialog7->addMovieInfo(1282, 1446, 0, 0);

		maxwKatDialog7->add(whoYuri2);
		maxwKatDialog4->add(howContact2);
		maxwKatDialog4->add(whatSVR2);
		maxwKatDialog5->add(whatSVR2);
//		maxwKatDialog5->add(hangHat3);
		maxwKatDialog5->add(thanksHer3);
		maxwKatDialog6->add(howContact2);
//		maxwKatDialog6->add(hangHat3);
//		maxwKatDialog6->add(thanksHer3);
		maxwKatDialog2->add(whatNoise3);
		maxwKatDialog2->add(whereEveryone3);
		maxwKatDialog2->add(thanksHer3);
		maxwKatDialog3->add(whereEveryone3);
		maxwKatDialog3->add(thanksHer3);
		maxwKatDialog1->add(whatNoise3);
		maxwKatDialog1->add(thanksHer3);

		dialogTree->rootNode = maxwKatDialog7;
		dialogTree->resourceID = "36001.avi";
		dialogTree->show(this);
	END

	BEG
		GameFlag.set(fMORoomEnter);
		GameFlag.set(fBSMeetingSet);   //DEBUG change flag later...also in 1000.cpp
		GameFlag.set(fWahKa41);
		GameFlag.set(fMetMaxFoster);
		if(GameFlag.test(fWahKa43))
			new EMail( "Yuri's Number: Foster, Maxine M.", 98090);

		intrface->enable();
		theMovie->caller = 0;
		theMovie->stop();
		theGame->newRoom( new Rm1000);
	END
}

/*******************************
 *
 *	Dialog
 *
/*******************************/

DoneHaveHat3::DoneHaveHat3()
{
	name = "DoneHaveHat3";
}

void DoneHaveHat3::doit()
{
	GameFlag.set(fWahKa31);
	DItem::doit();
}

NumberofYuri3::NumberofYuri3()
{
	name = "NumberofYuri3";
}

void NumberofYuri3::doit()
{
	GameFlag.set(fWahKa43);
//	inventry->get(iNUMBERTOYURI);
//	new EMail( "Yuri's Number: Foster, Maxine M.", 98090);
	DItem::doit();
}

AgainThanks3::AgainThanks3()
{
	name = "AgainThanks3";
}

void AgainThanks3::doit()
{
	if(howContact2->hasBeenChosen == TRUE)	
		maxwKatDialog6->add(thanksHer3);
	DItem::doit();
}
 
} // namespace Spycraft 
