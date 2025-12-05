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
#include "spycraft/logic/36400.h"
#include "spycraft/logic/1000.h"
#include "flag.h"
#include "invent.h"
#include "movie.h"

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

Dialog* 	maxwoKatDialog1;
Dialog*		maxwoKatDialog2;
Dialog*		maxwoKatDialog3;
Dialog*		maxwoKatDialog7;
Dialog*		maxwoKatDialog8;
Dialog*		maxwoKatDialog15;
Dialog*		maxwoKatDialog16;
Dialog*		maxwoKatDialog17;
Dialog*		maxwoKatDialog19;

DItem* aboutBird2;
DItem* whatNoise2; 
DItem* whereEveryone2; 
DItem* hangHat2; 
DItem* canDeed2; 
DItem* whatLoyalty2; 
DItem* meetBird2; 
DItem* weDo2; 
DItem* thanksHer2;

enum	
{
	ABOUT_BIRD2,
	WHAT_NOISE2,
	WHERE_EVERYONE2,
	HANG_HAT2,
	CAN_DEED2,
	WHAT_LOYALTY2,
	MEET_BIRD2,
	WE_DO2,
	THANKS_HER2
};

Rm36400::Rm36400()
{
	name = "Rm36400";
	show_style = VE_FLIP;
}

void Rm36400::init()
{
    Room::init();
  	intrface->show();

	new(SouthExit36400);
	
	if( !GameFlag.test( fMORoomEnter )){
//		drawPic( 1 );
   		setScript( new EnterScript36400 );
	}
	else{
		drawPic( 36000 );
		theGame->handsOn();
	}
}

void Rm36400::cue( void )
{
///	theMovie->fromTo(1369, 1370);
///	theMovie->play("36001.avi", this, 0, 0, 0);
}

Rm36400::~Rm36400( void )
{
	if( GameFlag.test( fDisposeDialog36400 )) {
		maxwoKatDialog1->dispose();
		maxwoKatDialog2->dispose();
		maxwoKatDialog3->dispose();
		maxwoKatDialog7->dispose();
		maxwoKatDialog8->dispose();
		maxwoKatDialog15->dispose();
		maxwoKatDialog16->dispose();
		maxwoKatDialog17->dispose();
		maxwoKatDialog19->dispose();
	
		GameFlag.clear( fDisposeDialog36400 );
	} //endif
}	

int Rm36400::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}

SouthExit36400::SouthExit36400()
{
	name = "SouthExit36400";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
}

void SouthExit36400::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit36400::doVerb ( int theVerb )
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

EnterScript36400::EnterScript36400()
{
	name = "EnterScript36400"; 
}

void EnterScript36400::changeState( int newState )
{
	switchTo
//		GameFlag.set(fMORoomEnter);
//		GameFlag.set(fBSMeetingSet);   //DEBUG change flag later...also in 1000.cpp
		GameFlag.set(fDisposeDialog36400);
//		GameFlag.set(fWahKa42);
//		GameFlag.set(fMetMaxFoster);

///		intrface->disable();
///		theGame->handsOff();
//		if( GameFlag.test(fKnowYingInMoscow )) {			  //wahka
//			theMovie->fromTo(987, 1370);
////		theMovie->fromTo(1005, 1367);
////		theMovie->play("36001.avi", this, 0, 0, 0);	
//		}
//		else{
//			theMovie->fromTo(987, 1523);
//			theMovie->play("36001.avi", this, 0, 0, 0);
//		}
		theMovie->fromTo(3570, 4007);
		theMovie->play("36001.avi", this, 0, 0, 0);	
	END

	BEG
///		curRoom->cue();
///		theGame->handsOn();
		
		dialogTree = new DialTree;
		maxwoKatDialog1 = new Dialog;
		maxwoKatDialog2 = new Dialog;
		maxwoKatDialog3 = new Dialog;
		maxwoKatDialog7 = new Dialog;
		maxwoKatDialog8 = new Dialog;
		maxwoKatDialog15 = new Dialog;
		maxwoKatDialog16 = new Dialog;
		maxwoKatDialog17 = new Dialog;
		maxwoKatDialog19 = new Dialog;

		aboutBird2 = new DItem;
		aboutBird2->addInfo(36400, ABOUT_BIRD2, maxwoKatDialog16 );
//		whatNoise2 = new DItem;
		whatNoise2 = new AddWeDo2;
		whatNoise2->addInfo(36400, WHAT_NOISE2, maxwoKatDialog3 );
//		whereEveryone2 = new DItem;
		whereEveryone2 = new AddWeDo2;
		whereEveryone2->addInfo(36400, WHERE_EVERYONE2, maxwoKatDialog1 );

if(!GameFlag.test(fWahKa31)) {
//		hangHat2 = new DItem;
		hangHat2 = new DoneHaveHat2;
		hangHat2->addInfo(36400, HANG_HAT2, maxwoKatDialog2 );
//	maxwoKatDialog8->add(hangHat2);
//	maxwoKatDialog15->add(hangHat2);
//	maxwoKatDialog19->add(hangHat2);
}
//		canDeed2 = new DItem;
//		canDeed2->addInfo(36400, CAN_DEED2, maxwoKatDialog19);
//		whatLoyalty2 = new DItem;
		whatLoyalty2 = new ProveDeed;
		whatLoyalty2->addInfo(36400, WHAT_LOYALTY2, maxwoKatDialog17);
		meetBird2 = new DItem;
		meetBird2->addInfo(36400, MEET_BIRD2, maxwoKatDialog8);
//		weDo2 = new DItem;
//		weDo2->addInfo(36400, WE_DO2, maxwoKatDialog15);
		thanksHer2 = new DItem;
//		thanksHer2->addInfo(36400, THANKS_HER2, maxwoKatDialog15);
		thanksHer2->addInfo(36400, THANKS_HER2, NULL);
/*
		maxwoKatDialog1->addMovieInfo(7, 125, 0, 0);
		maxwoKatDialog2->addMovieInfo(132, 263, 0, 0);
		maxwoKatDialog3->addMovieInfo(275, 513, 0, 0);
		maxwoKatDialog7->addMovieInfo(987, 1370, 0, 0);
		maxwoKatDialog8->addMovieInfo(1775, 1925, 0, 0);
		maxwoKatDialog15->addMovieInfo(2798, 2963, 0, 0);
		maxwoKatDialog16->addMovieInfo(2985, 3308, 0, 0);
		maxwoKatDialog17->addMovieInfo(3311, 3443, 0, 0);
		maxwoKatDialog19->addMovieInfo(1683, 1750, 0, 0);
*/		
		maxwoKatDialog1->addMovieInfo(0, 124, 0, 0);
		maxwoKatDialog2->addMovieInfo(135, 274, 0, 0);
		maxwoKatDialog3->addMovieInfo(285, 539, 0, 0);
//		maxwoKatDialog7->addMovieInfo(1005, 1367, 0, 0);
		maxwoKatDialog7->addMovieInfo(1005, 1046, 0, 0);
		maxwoKatDialog8->addMovieInfo(1785, 1971, 0, 0);
		maxwoKatDialog15->addMovieInfo(2895, 3069, 0, 0);
		maxwoKatDialog16->addMovieInfo(3075, 3403, 0, 0); 
		maxwoKatDialog17->addMovieInfo(3405, 3559, 0, 0);
		maxwoKatDialog19->addMovieInfo(1695, 1772, 0, 0);

		maxwoKatDialog7->add(aboutBird2);
////	maxwoKatDialog1->add(thanksHer2);
		maxwoKatDialog1->add(whatNoise2);
		maxwoKatDialog1->add(meetBird2);
		maxwoKatDialog2->add(whereEveryone2);
		maxwoKatDialog2->add(whatNoise2);
		maxwoKatDialog2->add(meetBird2);
//		maxwoKatDialog2->add(thanksHer2);
		maxwoKatDialog3->add(whereEveryone2);
////	maxwoKatDialog3->add(thanksHer2);
		maxwoKatDialog3->add(meetBird2);
		maxwoKatDialog8->add(whatLoyalty2);
//		maxwoKatDialog8->add(canDeed2);
//		maxwoKatDialog8->add(weDo2);
		maxwoKatDialog8->add(hangHat2);
//		maxwoKatDialog8->add(thanksHer2);
		maxwoKatDialog15->add(hangHat2);
		maxwoKatDialog15->add(meetBird2);
		maxwoKatDialog16->add(whatLoyalty2);
////	maxwoKatDialog16->add(meetBird2);
//		maxwoKatDialog17->add(canDeed2);
		maxwoKatDialog17->add(meetBird2);
//		maxwoKatDialog19->add(weDo2);
		maxwoKatDialog19->add(meetBird2);
		maxwoKatDialog19->add(hangHat2);

		dialogTree->rootNode = maxwoKatDialog7;
		dialogTree->resourceID = "36001.avi";
		dialogTree->show(this);
		
	END

	BEG
		GameFlag.set(fMORoomEnter);
		GameFlag.set(fBSMeetingSet);   //DEBUG change flag later...also in 1000.cpp
		GameFlag.set(fWahKa42);
		GameFlag.set(fMetMaxFoster);
		GameFlag.set(fDCK1);		   //make BS apt available in travelink

		intrface->enable();
		theMovie->caller = 0;
		theMovie->stop();
		theGame->newRoom( new Rm1000 );
	END
}

/*******************************
 *
 *	Dialog
 *
/*******************************/

ProveDeed::ProveDeed()
{
	name = "ProveDeed";
}

void ProveDeed::doit()
{
//	make information deed text appears
//	canDeed2 = new DItem;
	canDeed2 = new AddMeetBird2;
	canDeed2->addInfo(36400, CAN_DEED2, maxwoKatDialog19);
//	weDo2 = new DItem;
	weDo2 = new AddMeetBird2;
	weDo2->addInfo(36400, WE_DO2, maxwoKatDialog15);
			
	maxwoKatDialog8->add(canDeed2);
	maxwoKatDialog8->add(weDo2);
	maxwoKatDialog17->add(canDeed2);
	maxwoKatDialog19->add(weDo2);

	DItem::doit();
}

DoneHaveHat2::DoneHaveHat2()
{
	name = "DoneHaveHat2";
}

void DoneHaveHat2::doit()
{
	GameFlag.set(fWahKa31);
	maxwoKatDialog8->add(thanksHer2);
	DItem::doit();
}

AddMeetBird2::AddMeetBird2()
{
	name = "AddMeetBird2";
}

void AddMeetBird2::doit()
{
	if((canDeed2->hasBeenChosen) || (weDo2->hasBeenChosen))
		maxwoKatDialog8->add(meetBird2);

	if(((meetBird2->hasBeenChosen) && (hangHat2->hasBeenChosen)) && (whereEveryone2->hasBeenChosen || whatNoise2->hasBeenChosen)) {
		maxwoKatDialog15->add(whereEveryone2);
		maxwoKatDialog15->add(whatNoise2);
		maxwoKatDialog15->add(thanksHer2);
	}
	
	if((meetBird2->hasBeenChosen) || (hangHat2->hasBeenChosen))
		maxwoKatDialog15->add(thanksHer2);
	
	DItem::doit();
}  

AddWeDo2::AddWeDo2()
{
	name = "AddWeDo2";
}

void AddWeDo2::doit()
{
	if((!canDeed2->hasBeenChosen) || (!weDo2->hasBeenChosen)) {
		maxwoKatDialog1->add(weDo2);
		maxwoKatDialog1->add(canDeed2);
		maxwoKatDialog3->add(weDo2);
		maxwoKatDialog3->add(canDeed2);
	}
	
//wahka - trying to put thanks when whereeveryone and whatNoise	is at the end.
//	if(((canDeed2->hasBeenChosen) && (weDo2->hasBeenChosen)) && ((whereEveryone2->hasBeenChosen) || (whatNoise2->hasBeenChosen))) {
//		maxwoKatDialog1->add(thanksHer2);
//		maxwoKatDialog3->add(thanksHer2);
//	}

	DItem::doit();
}  
 
} // namespace Spycraft 
