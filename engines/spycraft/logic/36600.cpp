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
#include "spycraft/game/movie.h"
#include "spycraft/logic/36000.h"
#include "spycraft/logic/36600.h"
#include "spycraft/logic/1000.h"
#include "spycraft/game/flag.h"
#include "invent.h"
#include "spycraft/game/vlink.h"

namespace Spycraft {

extern Intrface* intrface;
extern Flag GameFlag;
extern DialTree* dialogTree;
extern SouthCursor* southCursor;

Dialog* 	maxDialog1;
Dialog*		maxDialog2;
Dialog*		maxDialog3;
Dialog*		maxDialog4;
Dialog*		maxDialog5;
Dialog*		maxDialog6;
Dialog*		maxDialog7;
Dialog*		maxDialog8;
Dialog*		maxDialog15;
Dialog*		maxDialog16;
Dialog*		maxDialog17;
Dialog*		maxDialog19;
Dialog*		maxDialog40;


DItem* whoYuri;
DItem* aboutBird;
DItem* whatBird; 	 
DItem* whatNoise; 
DItem* whereEveryone; 
DItem* howYuri; 
DItem* whatSVR; 
DItem* hangHat; 
DItem* howContact; 
///DItem* birdDo;       //same as whatLoyalty
DItem* canDeed; 
DItem* whatLoyalty; 
DItem* meetBird; 
DItem* weDo; 
DItem* thanksHer4;

enum	
{
	WHO_YURI,
	ABOUT_BIRD,
	WHAT_BIRD,
	WHAT_NOISE,
	WHERE_EVERYONE,
	HOW_YURI,
	WHAT_SVR,
	HANG_HAT,
	HOW_CONTACT,
	BIRD_DO,
	CAN_DEED,
	WHAT_LOYALTY,
	MEET_BIRD,
	WE_DO,
	THANKS_HER4
};

/*********************
*
*		Room Code 
*
**********************/

extern Intrface* intrface;

Rm36600::Rm36600()
{
	name = "Rm36600";
	show_style = VE_FLIP;
}

void Rm36600::cue()
{
///Ying's path
///	theMovie->fromTo(2648, 2787);
///	theMovie->play("36001.avi", curRoom->script, 0, 0, 0);
}

Rm36600::~Rm36600()
{
	if( GameFlag.test( fDisposeDialog36000 )) {		
		maxDialog1->dispose();
		maxDialog2->dispose();
		maxDialog3->dispose();
		maxDialog4->dispose();
		maxDialog5->dispose();
		maxDialog6->dispose();
		maxDialog8->dispose();
		maxDialog15->dispose();
		maxDialog16->dispose();
		maxDialog17->dispose();
		maxDialog19->dispose();
		maxDialog40->dispose();
	
		GameFlag.clear( fDisposeDialog36000 );		
	} //endif
}

void Rm36600::init()
{
////drawPic( 1 );
    Room::init();
	
  	intrface->show();

	new(SouthExit36600);

  	setScript( new EnterScript36600());
}

int Rm36600::handleEvent ( MADEEventStamp *event )
{
	return false;
}


SouthExit36600::SouthExit36600()
{
	name = "SouthExit36600";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
}

void SouthExit36600::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit36600::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm1000);
    	return true;
	}
	return false;
}
/*******************************
 *
 *	Scripts
 *
/*******************************/

EnterScript36600::EnterScript36600()
{
	name = "EnterScript36600"; 
}

void EnterScript36600::changeState( int newState )
{
	switchTo
//		GameFlag.set(fBSMeetingSet);   //DEBUG change flag later...also in 1000.cpp
		GameFlag.set(fDisposeDialog36000);
//		GameFlag.set(fMetMaxFoster);	  
		
		if(GameFlag.test(fMORoomEnter)) {    
//			theMovie->fromTo(1336, 1526);  //met Max
///			theMovie->fromTo(1365, 1521);  //met Max
			theMovie->fromTo(1282, 1446);  //met Max
			theMovie->play("36001.avi", this, 0, 0, 0);	
		}
		else {							  
//			theMovie->fromTo(987, 1523);   //ask about BS and Yuri, not met Max
///			theMovie->fromTo(1005, 1521);   //ask about BS and Yuri, not met Max
			theMovie->fromTo(1005, 1446);   //ask about BS and Yuri, not met Max
			theMovie->play("36001.avi", this, 0, 0, 0);
		}
	END

	BEG
		//Two dialogs here, 1)Ying 2)player not met max and did not do KAT
		dialogTree = new DialTree;
		maxDialog1 = new Dialog;
		maxDialog2 = new Dialog;
		maxDialog3 = new Dialog;
		maxDialog4 = new Dialog;
		maxDialog5 = new Dialog;
		maxDialog6 = new Dialog;
		maxDialog7 = new Dialog;
		maxDialog8 = new Dialog;
		maxDialog15 = new Dialog;
		maxDialog16 = new Dialog;
		maxDialog17 = new Dialog;
		maxDialog19 = new Dialog;
		maxDialog40 = new Dialog;


		whoYuri = new DItem;
		whoYuri->addInfo(36000, WHO_YURI, maxDialog4);
//		aboutBird= new DItem;
		aboutBird = new TellAboutBS;
		aboutBird->addInfo(36000, ABOUT_BIRD, maxDialog16 );
//		whatBird = new DItem;
		whatBird = new TellAboutBS;
		whatBird->addInfo(36000, WHAT_BIRD, maxDialog16 );
//		whatNoise = new DItem;
		whatNoise = new AddMeetBird;
		whatNoise->addInfo(36000, WHAT_NOISE, maxDialog3 );
//		whereEveryone = new DItem;
		whereEveryone = new AddMeetBird;
		whereEveryone->addInfo(36000, WHERE_EVERYONE, maxDialog1 );
//		howYuri = new DItem;
		howYuri = new NumberofYuri;
		howYuri->addInfo(36000, HOW_YURI, maxDialog5 );
//		whatSVR = new DItem;
		whatSVR = new AddMeetBird;
////	whatSVR->addInfo(36000, WHAT_SVR, maxDialog6 );
		whatSVR->addInfo(36000, WHAT_SVR, maxDialog40 );

if(!GameFlag.test(fWahKa31)) {
//		hangHat = new DItem;
		hangHat = new DoneHaveHat;
		hangHat->addInfo(36000, HANG_HAT, maxDialog2 );

		maxDialog5->add(hangHat);
		maxDialog6->add(hangHat);
		maxDialog15->add(hangHat);
}
		//howContact = new DItem;
		//howContact->addInfo(36000, HOW_CONTACT );
///		birdDo = new DItem;
///		birdDo = new InfoDeed;
///		birdDo->addInfo(36000, BIRD_DO, maxDialog17);
///		canDeed = new DItem;
///		canDeed->addInfo(36000, CAN_DEED, maxDialog19);
///		whatLoyalty = new DItem;
		whatLoyalty = new InfoDeed;
		whatLoyalty->addInfo(36000, WHAT_LOYALTY, maxDialog17);
		meetBird = new DItem;
		meetBird->addInfo(36000, MEET_BIRD, maxDialog8);
////	weDo = new DItem;
		weDo = new AddEveryoneNoise;
		weDo->addInfo(36000, WE_DO, maxDialog15);
		thanksHer4 = new DItem;
		thanksHer4->addInfo(36000, THANKS_HER4, NULL);
/*
		maxDialog1->addMovieInfo(7, 125, 124, 125);
		maxDialog2->addMovieInfo(132, 263, 262, 263);
		maxDialog3->addMovieInfo(275, 513, 512, 513);
		maxDialog4->addMovieInfo(537, 747, 746, 747);
		maxDialog5->addMovieInfo(758, 837, 836, 837); 
		maxDialog6->addMovieInfo(863, 972, 971, 972);
		maxDialog8->addMovieInfo(1775, 1925, 1924, 1925);
		maxDialog15->addMovieInfo(2798, 2963, 2962, 2963);
		maxDialog16->addMovieInfo(2985, 3310, 3309, 3310); 
		maxDialog17->addMovieInfo(3311, 3443, 3442, 3443);
		maxDialog19->addMovieInfo(1683, 1750, 1749, 1750);
*/		
		maxDialog1->addMovieInfo(0, 124, 0, 0);
		maxDialog2->addMovieInfo(135, 274, 0, 0);
		maxDialog3->addMovieInfo(285, 539, 0, 0);
		maxDialog4->addMovieInfo(540, 759, 0, 0);
		maxDialog5->addMovieInfo(765, 867, 0, 0); 
		maxDialog6->addMovieInfo(870, 996, 0, 0);
		maxDialog8->addMovieInfo(1785, 1971, 0, 0);
		maxDialog15->addMovieInfo(2895, 3069, 0, 0);
		maxDialog16->addMovieInfo(3075, 3403, 0, 0); 
		maxDialog17->addMovieInfo(3405, 3559, 0, 0);
		maxDialog19->addMovieInfo(1695, 1772, 0, 0);
		maxDialog40->addMovieInfo(870, 996, 0, 0);

		maxDialog7->add(whoYuri);
		maxDialog7->add(aboutBird);
		maxDialog1->add(whoYuri);
		maxDialog1->add(whatBird);
		maxDialog1->add(whatNoise);
		maxDialog2->add(whoYuri);
		maxDialog2->add(whatBird);
		maxDialog2->add(whatNoise);
		maxDialog3->add(whoYuri);
		maxDialog3->add(whatBird);
		maxDialog3->add(whereEveryone);
		maxDialog4->add(howYuri);
		maxDialog4->add(whatSVR);
//		maxDialog4->add(whatBird);
		maxDialog5->add(whatSVR);
		maxDialog5->add(whatBird);
//		maxDialog5->add(hangHat);
		maxDialog6->add(howYuri);
		maxDialog6->add(whatBird);
//		maxDialog6->add(hangHat);
///		maxDialog8->add(birdDo);
		maxDialog8->add(whatLoyalty);
///		maxDialog8->add(canDeed);
		maxDialog8->add(whoYuri);
////	maxDialog8->add(thanksHer4);
		maxDialog15->add(whoYuri);
//		maxDialog15->add(hangHat);
		maxDialog15->add(meetBird);
		maxDialog16->add(whatLoyalty);
////	maxDialog16->add(meetBird);
		maxDialog16->add(whoYuri);
		maxDialog17->add(meetBird);
///		maxDialog17->add(canDeed);
		maxDialog17->add(whoYuri);
		maxDialog19->add(weDo);
		maxDialog19->add(meetBird);
		maxDialog19->add(whoYuri);
		maxDialog40->add(howYuri);

		dialogTree->rootNode = maxDialog7;
		dialogTree->resourceID = "36001.avi";
		dialogTree->show(this);
	END

	BEG
		GameFlag.set(fBSMeetingSet);   //DEBUG change flag later...also in 1000.cpp
		GameFlag.set(fMetMaxFoster);	  
		GameFlag.set(fDCK1);		   //make BS apt available in travelink

		GameFlag.set(fMORoomEnter);
		if(GameFlag.test(fWahKa43))
			new EMail( "Yuri's Number: Foster, Maxine M.", 98090);
		
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
InfoDeed::InfoDeed()
{
	name = "InfoDeed";
}

void InfoDeed::doit()
{
//	make information deed text appears
	canDeed = new DItem;
	canDeed->addInfo(36000, CAN_DEED, maxDialog19);
	
	maxDialog8->add(canDeed);
	maxDialog17->add(canDeed);

	DItem::doit();
}

TellAboutBS::TellAboutBS()
{
	name = "TellAboutBS";
}

void TellAboutBS::doit()
{
//whatBird and aboutBird has different text but same movie
	whatBird->hasBeenChosen = true;
	
	DItem::doit();
}

DoneHaveHat::DoneHaveHat()
{
	name = "DoneHaveHat";
}

void DoneHaveHat::doit()
{
	GameFlag.set(fWahKa31);
	
	maxDialog2->add(meetBird);
	
	if(meetBird->hasBeenChosen && (!canDeed->hasBeenChosen)) {
		maxDialog2->add(canDeed);
	}

	DItem::doit();
}

NumberofYuri::NumberofYuri()
{
	name = "NumberofYuri";
}

void NumberofYuri::doit()
{
	GameFlag.set(fWahKa43);
//	inventry->get(iNUMBERTOYURI);
	if(hangHat->hasBeenChosen)
		maxDialog5->add(meetBird);

	if(meetBird->hasBeenChosen && (!canDeed->hasBeenChosen)) {
		maxDialog5->add(canDeed);
	}

//	new EMail( "Yuri's Number: Foster, Maxine M.", 98090);
	DItem::doit();
}  

AddMeetBird::AddMeetBird()
{
	name = "AddMeetBird";
}

void AddMeetBird::doit()
{
	maxDialog1->add(meetBird);
 	maxDialog2->add(meetBird);
	maxDialog3->add(meetBird);
	
	if(hangHat->hasBeenChosen)
		maxDialog6->add(meetBird);

	if(meetBird->hasBeenChosen && (!canDeed->hasBeenChosen)) {
		maxDialog1->add(canDeed);
		maxDialog3->add(canDeed);
	}

	if(howYuri->hasBeenChosen) {
		maxDialog40->add(hangHat);
		maxDialog40->add(whatBird);
	}
	
	DItem::doit();
}  

AddEveryoneNoise::AddEveryoneNoise()
{
	name = "AddEveryoneNoise";
}

void AddEveryoneNoise::doit()
{
	if(((meetBird->hasBeenChosen) && (hangHat->hasBeenChosen)) && (whereEveryone->hasBeenChosen || whatNoise->hasBeenChosen)) {
		maxDialog15->add(whereEveryone);
		maxDialog15->add(whatNoise);
		maxDialog15->add(thanksHer4);
	}			   

	if((meetBird->hasBeenChosen) && (whoYuri->hasBeenChosen)) {
		maxDialog15->add(whereEveryone);
		maxDialog15->add(whatNoise);
////	maxDialog15->add(thanksHer4);
	}

	DItem::doit();
}  
 
} // namespace Spycraft 
