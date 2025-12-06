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
#include "spycraft/logic/69000.h"
#include "spycraft/logic/69100.h"
#include "spycraft/logic/69200.h"
#include "spycraft/logic/69300.h"
#include "spycraft/logic/69400.h"
#include "spycraft/logic/69500.h"
#include "spycraft/logic/1000.h"
#include "spycraft/game/movie.h"
#include "spycraft/game/flag.h"
#include "spycraft/dmade/advsound.h"
#include "spycraft/dmade/mcimovie.h"
#include "spycraft/game/roomsnd.h"
#include "spycraft/game/sound.h"

namespace Spycraft {

#define CRSBLINK_SPEED 5

/*********************
*
*		Room Code 
*
**********************/

extern MADEEnvirons *colbyInfo;
extern Intrface* intrface;
extern SouthCursor* southCursor;
extern WestCursor* westCursor;
extern EastCursor* eastCursor;
extern BlinkCursor* normalCursor;
extern DialTree* dialogTree;
CuffCursor* cuffCursor; 

Dialog* 	onyxDialog1;
Dialog*		onyxDialog2;
Dialog*		onyxDialog3;
Dialog*		onyxDialog4;
Dialog*		onyxDialog5;
Dialog*		onyxDialog6;
Dialog*		onyxDialog7;
Dialog* 	onyxDialog8;
Dialog*		onyxDialog9;
Dialog*		onyxDialog10;
Dialog*		onyxDialog11;
Dialog*		onyxDialog12;
Dialog*		onyxDialog13;
Dialog*		onyxDialog14;

DItem* nameThornCIA;
DItem* wellWorkFor;
DItem* hearPitDeal; 
DItem* cooperateShadow; 
DItem* cooperateCompany; 
DItem* tellPitCIA; 
DItem* okayPitDeal;
DItem* whenDealDown;
DItem* betterDealDown;
DItem* whatPassword;
DItem* whatPassword2;
DItem* stayPut;

DialogFlag *thanksForInfo;

enum	
{
	NAMETHORNCIA,
	WELLWORKFOR,
	HEARPITDEAL,
	COOPERATESHADOW,
	COOPERATECOMPANY,
	TELLPITCIA,
	OKAYPITDEAL,
	WHENDEALDOWN,
	BETTERDEALDOWN,
	WHATPASSWORD,
	THANKSFORINFO,
	STAYPUT,
};

Rm69000::Rm69000()
{
	name = "Rm69000";
	show_style = VE_FLIP;	 //VE_FULL_DISSOLVE for doors
	sound1->stop();
}

void Rm69000::cue( void )
{
//	theMovie->fromTo(372, 375);
//	theMovie->play("69001.avi", this, 0, 0, 0);
}

Rm69000::~Rm69000()
{
	if( GameFlag.test( fDisposeDialog69000 )) {
		onyxDialog1->dispose();
		onyxDialog2->dispose();
		onyxDialog3->dispose();
		onyxDialog4->dispose();
		onyxDialog5->dispose();
		onyxDialog6->dispose();
		onyxDialog7->dispose();
		onyxDialog8->dispose();
		onyxDialog9->dispose();
		onyxDialog10->dispose();
		onyxDialog11->dispose();
		onyxDialog12->dispose();
		onyxDialog13->dispose();
		onyxDialog14->dispose();

		GameFlag.clear( fDisposeDialog69000 );
	}
}

void Rm69000::init()
{
//	drawPic( 1 );			
    Room::init();

  	intrface->disable();
  	theGame->handsOn();

	curRoom->setScript( new EnterScript69000 );
}

int Rm69000::handleEvent ( MADEEventStamp *event )
{
	return false;
}

Rm69002::Rm69002()
{
	name = "Rm69002";
	show_style = VE_FLIP;
}

Rm69002::~Rm69002()
{
}

void Rm69002::cue( void )
{
}

void Rm69002::init()
{
	drawPic( 69002 );			
    Room::init();

  	intrface->enable();
 	theGame->handsOn();
	
	if( colbyInfo->slow_system == true )
		theMusic->play ( 69000 );
	else 
	{
		if( sound1->number != 69000  || !sound1->isPlaying() )
		{	
			sound1->stop();
			sound1->loop = SND_LOOP_INFINITE;
			sound1->playBuffered( 69000, NULL );
		}
	}

	new(EastExit69000);
	new(WestExit69000);
	new (SouthExit69000);	// south exit 

	if (GameFlag.test(fGotHandcuffs)) 		//check to see if handcuffs in inventory
	{  	
		new ( Onyx69000 );
		new ( Cuffs69000 );		// activate view for cuffing onyx
	}
}

int Rm69002::handleEvent ( MADEEventStamp *event )
{
	return false;
}

Rm69003::Rm69003()
{
	name = "Rm69003";
	show_style = VE_FLIP;
}

Rm69003::~Rm69003()
{
}

void Rm69003::cue( void )
{
}

void Rm69003::init()
{
//	drawPic( 1 );		
    Room::init();
	intrface->disable();
	GameFlag.set(fMovedSuit);	// use fMovedSuit as cuff onyx flag!!
	curRoom->setScript ( new OnyxScript69000 );
}			    

int Rm69003::handleEvent ( MADEEventStamp *event )
{
	return false;
}

/* Exit Features */

SouthExit69000::SouthExit69000()
{
	name = "SouthExit69000";
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
    setHotspot ( DO_V );
}

void SouthExit69000::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit69000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
	//	if (!GameFlag.test(fMovedSuit))			// fMovedSuit temp for flag of cuffing onyx
		sound1->stop();
		theMusic->stop();
	//	theGame->setCursor( normalCursor );
		theGame->handsOn();
		theGame->newRoom(new Rm1000);
    	return true;
	}
	return false;
}

WestExit69000::WestExit69000()
{
	name = "WestExit69000";
	setHotspot ( DO_V );
	init(WEST_EXIT_X, WEST_EXIT_Y, WEST_EXIT_WIDTH, WEST_EXIT_HEIGHT);
}

void WestExit69000::respond()
{
	theGame->setCursor(westCursor);
	aniCursor();
}

int WestExit69000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V )
	{
		if (GameFlag.test(fClosetOpen69100))
  			theGame->newRoom(new Rm69101);
		else theGame->newRoom(new Rm69100);
    	return true;
	}
	return false;
}

EastExit69000::EastExit69000()
{
	name = "EastExit69000";
	setHotspot ( DO_V );
	init(EAST_EXIT_X, EAST_EXIT_Y, EAST_EXIT_WIDTH, EAST_EXIT_HEIGHT);
}

void EastExit69000::respond()
{
	theGame->setCursor(eastCursor);
	aniCursor();
}

int EastExit69000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V )
	{
		theGame->newRoom(new Rm69200);
    	return true;
	}
	return false;
}

CuffCursor::CuffCursor()
{
	name = "CuffCurosr";
	view = 69110;
	loop = 0;
	cel = 0;
	orig_pt.x = 0;
	orig_pt.y = 0;
	verb = DO_V;
}

/*******************************
 *
 *	Features
 *
/*******************************/

// Onyx Feature for handcuffing
Onyx69000::Onyx69000()
{
	name = "Onyx69000";
	cuffCursor = new (CuffCursor);
	setHotspot ( DO_V );
//	init ( 150, 140, 200, 160);
	init ( 90, 100, 360, 270);
}

void Onyx69000::respond()
{
	theGame->setCursor( cuffCursor );
}

int Onyx69000::doVerb ( int theVerb )
{
  	if ( theVerb == DO_V ) {
		theGame->newRoom ( new Rm69003 );	
	   	return true;
	}
	return false;
}

/*******************************
 *
 *	Scripts
 *
/*******************************/
OnyxScript69000::OnyxScript69000()
{
	name = "OnyxScript69000";
	onyxSound = new Sound;
}

void OnyxScript69000::changeState( int newState )
{	
 	switchTo
		sound1->stop();
		theMusic->stop();
		onyxSound->play ( 69017, this );
	END

	BEG
	   	ticks = 2;
	END

	BEG
		onyxSound->dispose();
//		sfxSystemCursor( 10 );
		theMovie->fromTo(2115, 2285);
		theMovie->play("69001.avi", this, 0, 1, 0);	
    END	

	BEG
	   	ticks = 2;
	END
    
    BEG	
		intrface->enable();
		theGame->handsOn();
		theGame->newRoom( new Rm1000 );
	END		
}

EnterScript69000::EnterScript69000()
{
	name = "EnterScript69000"; 
}

void EnterScript69000::changeState( int newState )
{
	switchTo
		ticks = 2;
	END

	BEG
		GameFlag.set(fHKRoomEnter);
		GameFlag.set(fDisposeDialog69000);
		theMovie->fromTo(0, 375);
		theMovie->play("69001.avi", this, 0, 0, 0);
	END

	BEG

		dialogTree = new DialTree;
		onyxDialog1 = new Dialog;
		onyxDialog2 = new Dialog;
		onyxDialog3 = new Dialog;
		onyxDialog4 = new Dialog;
		onyxDialog5 = new Dialog;
		onyxDialog6 = new Dialog;
		onyxDialog7 = new Dialog;
		onyxDialog8 = new Dialog;
		onyxDialog9 = new Dialog;
		onyxDialog10 = new Dialog;
		onyxDialog11 = new Dialog;
		onyxDialog12 = new Dialog;
		onyxDialog13 = new Dialog;
		onyxDialog14 = new Dialog;

		nameThornCIA = new DItem;
		nameThornCIA->addInfo(69000, NAMETHORNCIA, onyxDialog2);
		wellWorkFor= new DItem;
		wellWorkFor->addInfo(69000, WELLWORKFOR, onyxDialog4 );
		hearPitDeal = new DItem;
		hearPitDeal->addInfo(69000, HEARPITDEAL, onyxDialog3 );
		cooperateShadow = new DItem;
		cooperateShadow->addInfo(69000, COOPERATESHADOW, onyxDialog6 );
		cooperateCompany = new DItem;
		cooperateCompany->addInfo(69000, COOPERATECOMPANY, onyxDialog5 );
		tellPitCIA = new DItem;
		tellPitCIA->addInfo(69000, TELLPITCIA, onyxDialog7 );
		okayPitDeal = new DItem;
		okayPitDeal->addInfo(69000, OKAYPITDEAL, onyxDialog8 );
		whenDealDown = new DItem;
		whenDealDown->addInfo(69000, WHENDEALDOWN, onyxDialog8 );
		betterDealDown = new DItem;
		betterDealDown->addInfo(69000, BETTERDEALDOWN, onyxDialog7 );
	// Black Jack Ace Password - wrong
		whatPassword= new DItem;
		whatPassword->addInfo(69000, WHATPASSWORD, onyxDialog9 );
	// French Vanilla Password - correct
		whatPassword2= new DItem;
		whatPassword2->addInfo(69000, WHATPASSWORD, onyxDialog10);
	// Thanks for info, set flag
		thanksForInfo = new DialogFlag;
		thanksForInfo->addInfo(69000, THANKSFORINFO, NULL);
	// Stay Put or die
		stayPut = new DItem;
		stayPut->addInfo(69000, STAYPUT, NULL);
/*
		onyxDialog1->addMovieInfo(0, 375, 372, 375);
		onyxDialog2->addMovieInfo(390, 462, 459, 462);
		onyxDialog3->addMovieInfo(466, 530, 527, 530);
		onyxDialog4->addMovieInfo(555, 640, 637, 640);
		onyxDialog5->addMovieInfo(662, 760, 757, 760); 
		onyxDialog6->addMovieInfo(765, 875, 872, 875);
		onyxDialog7->addMovieInfo(886, 1125, 1122, 1125);
		onyxDialog8->addMovieInfo(1129, 1338, 1335, 1338);
		onyxDialog9->addMovieInfo(1350, 1400, 1397, 1400);
		onyxDialog10->addMovieInfo(1413, 1660, 1657, 1660);	 
		onyxDialog11->addMovieInfo(1670, 1775, 1772, 1775);
		onyxDialog12->addMovieInfo(1790, 1925, 1922, 1925); 
		onyxDialog13->addMovieInfo(1937, 2200, 2197, 2200);
		onyxDialog14->addMovieInfo(600, 640, 637, 640);
*/
		onyxDialog1->addMovieInfo(0, 386, 0, 0);
		onyxDialog2->addMovieInfo(390, 469, 0, 0);
		onyxDialog3->addMovieInfo(480, 564, 0, 0);
		onyxDialog4->addMovieInfo(570, 681, 0, 0);
		onyxDialog5->addMovieInfo(690, 792, 0, 0); 
		onyxDialog6->addMovieInfo(795, 917, 0, 0);
		onyxDialog7->addMovieInfo(930, 1163, 0, 0);
		onyxDialog8->addMovieInfo(1170, 1386, 0, 0);
		onyxDialog9->addMovieInfo(1395, 1458, 0, 0);
		onyxDialog10->addMovieInfo(1470, 1726, 0, 0);	 
		onyxDialog11->addMovieInfo(1740, 1861, 0, 0);
		onyxDialog12->addMovieInfo(1875, 2013, 0, 0); 
		onyxDialog13->addMovieInfo(2025, 2285, 0, 0);
		onyxDialog14->addMovieInfo(600, 640, 637, 640);

		onyxDialog1->add(nameThornCIA);
	//	onyxDialog2->add(wellWorkFor);
		onyxDialog2->add(hearPitDeal);
		onyxDialog3->add(cooperateShadow);
		onyxDialog3->add(cooperateCompany);
		onyxDialog5->add(tellPitCIA);
		//onyxDialog5->add(okayPitDeal);
		onyxDialog6->add(whenDealDown);
		//onyxDialog6->add(betterDealDown);
		onyxDialog7->add(whatPassword);
		onyxDialog8->add(whatPassword2);
		onyxDialog9->add(thanksForInfo);
		onyxDialog9->add(stayPut);
		onyxDialog10->add(thanksForInfo);
		onyxDialog10->add(stayPut);

		dialogTree->rootNode = onyxDialog1;
		dialogTree->resourceID = "69001.avi";
		dialogTree->show(this);
	END
		
	BEG
	//	intrface->enable();
		theMovie->caller = 0;
		theMovie->stop();
		if (GameFlag.test(fLinus20))
		{
			intrface->enable();
			theGame->newRoom( new Rm1000 );
		}
		else theGame->newRoom( new Rm69002 );
		
	END
}

///////////
// Views //
///////////

Cuffs69000::Cuffs69000()
{
	name = "Cuffs69000";
	setHotspot ( DO_V );
	setView ( 69110 );
	init ( 239, 420 );
//	noSkip ();
}

void Cuffs69000::respond()
{
//		theGame->setCursor( normalCursor );
//		normalCursor->blink(CRSBLINK_SPEED);
}

int Cuffs69000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
	   	return true;
	}
	return false;
}

void DialogFlag::doit()
{
	GameFlag.set(fLinus20);
	
	DItem::doit();
}	
 
} // namespace Spycraft 
