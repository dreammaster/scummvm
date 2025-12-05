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
#include "spycraft/logic/1075.h"
#include "spycraft/logic/1000.h"
#include "spycraft/game/flag.h"
#include "spycraft/game/movie.h"
#include "spycraft/game/vlink.h"
#include "spycraft/logic/60000.h"

namespace Spycraft {

/*********************
*
*		Room Code 
*
**********************/
extern Intrface* intrface;
extern NorthCursor* northCursor;
//extern Print* testPrint;
extern DialTree* dialogTree;

Dialog* 	grendelDialog1;
Dialog*		grendelDialog2;
Dialog*		grendelDialog3;
Dialog*		grendelDialog4;
Dialog*		grendelDialog5;

DItem* onyxGoods;
DItem* onyxPit;
DItem* blackjackAce; 
DItem* fruitLoops; 
DItem* frenchVanilla; 

enum	
{
	ONYXGOODS,
	ONYXPIT,
	BLACKJACKACE,
	FRUITLOOPS,
	FRENCHVANILLA
};

Rm1075::Rm1075()
{
	name = "Rm1075";
	show_style = VE_FLIP;
}

Rm1075::~Rm1075()
{
	
if( GameFlag.test( fDisposeDialog1075 )) {
	grendelDialog1->dispose();
	grendelDialog2->dispose();
	grendelDialog3->dispose();
	grendelDialog4->dispose();
	grendelDialog5->dispose();

	GameFlag.clear( fDisposeDialog1075 );
}	//endif

}

void Rm1075::init()
{
    Room::init();

  	intrface->disable();
///	theGame->handsOn();

//	if( !GameFlag.test( fGrendelGripe )){		 

//		drawPic( 1 );
   	setScript( new EnterScript1075 );

//	}
//	else{							//  	drawPic( 1075 );
//  		drawPic( 1 );
//  		theGame->handsOn();							 
//	}
//  	new(NorthExit1075);
	
//  	intrface->show();
//  	theGame->handsOn();

//   testPrint = new(Print); 
//   testPrint->init("Est. Castle");
}

void Rm1075::cue ( void )
{
///	theMovie->fromTo(743, 747);
///	theMovie->play("1079.avi", this, 0, 0, 0);
}

int Rm1075::handleEvent ( MADEEventStamp *event )
{
	return false;
}
/*
NorthExit1075::NorthExit1075()
{
	name = "NorthExit1075";
	setHotspot ( DO_V );
	init ( 0, 0, 640, 60 );
}

void NorthExit1075::respond()
{
	theGame->setCursor(northCursor);
}

int NorthExit1075::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm76000);
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

EnterScript1075::EnterScript1075()
{
	name = "EnterScript1075"; 
}

void EnterScript1075::changeState( int newState )
{
	switchTo
		ticks = 2;
	END

	BEG
		GameFlag.set(fGrendelGripe);		
		GameFlag.set(fDisposeDialog1075);
		intrface->disable();
		theMovie->fromTo(0, 747);
		theMovie->play("1079.avi", this, 0, 0, 0);
	END

	BEG

		dialogTree = new DialTree;
		grendelDialog1 = new Dialog;
		grendelDialog2 = new Dialog;
		grendelDialog3 = new Dialog;
		grendelDialog4 = new Dialog;
		grendelDialog5 = new Dialog;


		if (GameFlag.test(fMovedSuit))
		{
			onyxGoods = new DItem;
			onyxGoods->addInfo(1075, ONYXGOODS, grendelDialog2);		 // ok
			onyxPit = new DItem;
			onyxPit->addInfo(1075, ONYXPIT, grendelDialog2 );			 // ok
		}else
		{
			onyxGoods = new DItem;
			onyxGoods->addInfo(1075, ONYXGOODS, grendelDialog3);		 // onyx reveals you
			onyxPit = new DItem;
			onyxPit->addInfo(1075, ONYXPIT, grendelDialog3);			 // onyx reveals you
		}
		blackjackAce = new DItem;
		blackjackAce->addInfo(1075, BLACKJACKACE, grendelDialog4 );		 // loser
		fruitLoops = new DItem;
		fruitLoops->addInfo(1075, FRUITLOOPS, grendelDialog4 );			 // loser
		frenchVanilla = new PitSaleGood;
		frenchVanilla->addInfo(1075, FRENCHVANILLA, grendelDialog5 );	 // winner
		
		grendelDialog1->addMovieInfo(0, 747, 743, 747);
		grendelDialog2->addMovieInfo(750, 865, 862, 865);
		grendelDialog3->addMovieInfo(900, 1115, 1112, 1115);
		grendelDialog4->addMovieInfo(1125, 1255, 1252, 1255);
		grendelDialog5->addMovieInfo(1275, 1575, 1572, 1575); 

//		grendelDialog1->add(onyxGoods);
		grendelDialog1->add(onyxPit);
		grendelDialog2->add(blackjackAce);
		grendelDialog2->add(fruitLoops);
		grendelDialog2->add(frenchVanilla);

		dialogTree->rootNode = grendelDialog1;
		dialogTree->resourceID = "1079.avi";
		dialogTree->show(this);
	
	END

	BEG
		intrface->enable();
		theMovie->caller = 0;
		theMovie->stop();
		if (GameFlag.test(fEndgamePitStopped))
		{
			new VLink( "You've saved us all: DCI Sterling, Peter R.","1077.avi", 0, 0);			//vlink 616
			new VLink( "Red line communication: Oval Office","1078.avi", 0, 0);			//vlink 407
			GameFlag.clear(fHeidelbergAvailable);
			curMap = MAP_HEIDELBERG;
	   		theGame->newRoom( new Rm1000 );
		}
		else 
		{
		 	theGame->newRoom( new Rm60000(END_DEAD));
  		}
	END
}

PitSaleGood::PitSaleGood()
{
	name = "PitSaleGood";
}

void PitSaleGood::doit()
{
	GameFlag.set(fEndgamePitStopped);		//Pit sold successfully
	DItem::doit();
}
 
} // namespace Spycraft 
