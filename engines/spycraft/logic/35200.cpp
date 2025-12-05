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

#include "globals.h"
#include "game.h"
#include "verbs.h"
#include "views.h"
#include "35200.h"
#include "1000.h"
#include "flag.h"
#include "movie.h"

namespace Spycraft {

Dialog* 	maxaboutbsDialog1;
Dialog*		maxaboutbsDialog2;
Dialog*		maxaboutbsDialog3;
Dialog*		maxaboutbsDialog4;
Dialog*		maxaboutbsDialog5;

DItem* montanaCooperation;
DItem* cooperateMurder;
DItem* ideaYou; 
DItem* toldDo; 
DItem* allMontana; 

enum	
{
	MONTANACOOPERATION,
	COOPERATEMURDER,
	IDEAYOU,
	TOLDDO,
	ALLMONTANA,
};

/*********************
*
*		Room Code 
*
**********************/

extern Intrface* intrface;
extern DialTree* dialogTree;

Rm35200::Rm35200()
{
	name = "Rm35200";
	show_style = VE_FLIP;
}

void Rm35200::cue( void )
{
///	theMovie->fromTo(123, 124);
///	theMovie->play("33001.avi", this, 0, 0, 0);
}

Rm35200::~Rm35200()
{ 
	if( GameFlag.test( fDisposeDialog35200 )) {
		maxaboutbsDialog1->dispose();
		maxaboutbsDialog2->dispose();
		maxaboutbsDialog3->dispose();
		maxaboutbsDialog4->dispose();
		maxaboutbsDialog5->dispose();

		GameFlag.clear( fDisposeDialog35200 );
	}	//endif
}
	
void Rm35200::init()
{
  	drawPic( 1 );
    Room::init();
  
  	intrface->show();
  	theGame->handsOn();

	setScript( new EnterScript35200 );
}

int Rm35200::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}

/*******************************
 *
 *	Scripts
 *
/*******************************/

EnterScript35200::EnterScript35200()
{
	name = "EnterScript35200"; 
}

void EnterScript35200::changeState( int newState )
{
	switchTo
		GameFlag.set(fDisposeDialog35200);
///		theGame->handsOff();
///		intrface->disable();
		theMovie->fromTo(0, 123);
//		theMovie->play("33001.avi", this, 0, 0, 0);
		theMovie->play("101.avi", this, 0, 0, 0);
	END

	BEG
///		curRoom->cue();
///		theGame->handsOn();

		dialogTree = new DialTree;
		maxaboutbsDialog1 = new Dialog;
		maxaboutbsDialog2 = new Dialog;
		maxaboutbsDialog3 = new Dialog;
		maxaboutbsDialog4 = new Dialog;
		maxaboutbsDialog5 = new Dialog;

//		montanaCooperation = new DItem;
		montanaCooperation = new CarrotPath;
		montanaCooperation->addInfo(35200, MONTANACOOPERATION, maxaboutbsDialog2);
//		cooperateMurder = new DItem;
		cooperateMurder = new StickPath;
		cooperateMurder->addInfo(35200, COOPERATEMURDER, maxaboutbsDialog4 );
		ideaYou = new DItem;
//not back to five, wahka
		ideaYou->addInfo(35200, IDEAYOU, maxaboutbsDialog5 );
		toldDo = new DItem;
		toldDo->addInfo(35200, TOLDDO, maxaboutbsDialog5 );
//		allMontana = new DItem;
		allMontana = new CarrotAfterStickPath;
		allMontana->addInfo(35200, ALLMONTANA, maxaboutbsDialog3 );

		maxaboutbsDialog1->addMovieInfo(0, 123, 0, 0);
		maxaboutbsDialog2->addMovieInfo(135, 221, 0, 0);
		maxaboutbsDialog3->addMovieInfo(225, 317, 0, 0);
		maxaboutbsDialog4->addMovieInfo(330, 458, 0, 0);
		maxaboutbsDialog5->addMovieInfo(465, 570, 0, 0); 

		maxaboutbsDialog1->add(montanaCooperation);
		maxaboutbsDialog1->add(cooperateMurder);
		maxaboutbsDialog1->add(ideaYou);
		maxaboutbsDialog4->add(toldDo);
		maxaboutbsDialog4->add(allMontana);

		dialogTree->rootNode = maxaboutbsDialog1;
//		dialogTree->resourceID = "33001.avi";
		dialogTree->resourceID = "101.avi";
		dialogTree->show(this);
	
	END

	BEG
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
StickPath::StickPath()
{
	name = "StickPath";
}

void StickPath::doit()
{
	GameFlag.set(fStickPath);	  	 	//do stick path
	GameFlag.set(fDisplayStakout);
	DItem::doit();
}

CarrotPath::CarrotPath()
{
	name = "CarrotPath";
}

void CarrotPath::doit()
{
	GameFlag.set(fCarrotPath);	  	 	//do carrot path
	DItem::doit();
}

CarrotAfterStickPath::CarrotAfterStickPath()
{
	name = "CarrotAfterStickPath";
}

void CarrotAfterStickPath::doit()
{
	GameFlag.clear(fStickPath);	
	GameFlag.set(fCarrotPath);	  	 	//do carrot after stick path
	GameFlag.clear(fDisplayStakout);
	DItem::doit();
}
 
} // namespace Spycraft 
