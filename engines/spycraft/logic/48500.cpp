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
#include "spycraft/logic/48500.h"
#include "spycraft/logic/48000.h"
#include "spycraft/logic/1000.h"
//#include "spycraft/logic/45000.h"
#include "spycraft/game/flag.h"
#include "spycraft/game/movie.h"
#include "spycraft/game/vlink.h"
#include "spycraft/game/invent.h"

namespace Spycraft {

Dialog* 	yuriaboutonyxDialog1;
Dialog*		yuriaboutonyxDialog2;
Dialog*		yuriaboutonyxDialog3;

DItem* ciafileHowit;
DItem* knowWherethis;

enum	
{
	CIAFILEHOWIT,
	KNOWWHERETHIS,
};

/*********************
*
*		Room Code 
*
**********************/

extern Intrface* intrface;
extern DialTree* dialogTree;

Rm48500::Rm48500()
{
	name = "Rm48500";
	show_style = VE_FLIP;
}

void Rm48500::cue( void )
{
//	theMovie->fromTo(199, 200);				
//	theMovie->play("48004.avi", this, 0, 0, 0);
}

Rm48500::~Rm48500()
{ 
	if( GameFlag.test( fDisposeDialog48500 )) {
		yuriaboutonyxDialog1->dispose();
		yuriaboutonyxDialog2->dispose();
		yuriaboutonyxDialog3->dispose();

		GameFlag.clear( fDisposeDialog48500 );
	}	//endif
}
	
void Rm48500::init()
{
///	drawPic( 1 );
    Room::init();
  
  	intrface->show();
///	theGame->handsOn();

	setScript( new EnterScript48500 );
}

int Rm48500::handleEvent ( MADEEventStamp *event )
{
	return false;
}


/*******************************
 *
 *	Scripts
 *
/*******************************/

EnterScript48500::EnterScript48500()
{
	name = "EnterScript48500"; 
}

void EnterScript48500::changeState( int newState )
{
	switchTo
		GameFlag.set(fDisposeDialog48500);
///		theGame->handsOff();
		intrface->disable();
		theMovie->fromTo(0, 200);				
		theMovie->play("48004.avi", this, 0, 1, 0);
		inventry->get(iONYX_DOSSIER);
	END
	BEG
		curRoom->drawPic( -1 );
		new Yellow(98638);
		ticks = 2;
	END
	BEG
		theMovie->fromTo(201, 203);				
		theMovie->play("48004.avi", this, 0, 0, 0);
	END
	BEG
///		curRoom->cue();
///		theGame->handsOn();

		dialogTree = new DialTree;
		yuriaboutonyxDialog1 = new Dialog;
		yuriaboutonyxDialog2 = new Dialog;
		yuriaboutonyxDialog3 = new Dialog;
  
		ciafileHowit = new DItem;
		ciafileHowit->addInfo(48500, CIAFILEHOWIT, yuriaboutonyxDialog2);
		knowWherethis = new DItem;
		knowWherethis->addInfo(48500, KNOWWHERETHIS, yuriaboutonyxDialog3 );

		yuriaboutonyxDialog1->addMovieInfo(0, 200, 199, 200);
		yuriaboutonyxDialog2->addMovieInfo(201, 427, 426, 427);
		yuriaboutonyxDialog3->addMovieInfo(428, 635, 634, 635);

		yuriaboutonyxDialog1->add(ciafileHowit);
		yuriaboutonyxDialog2->add(knowWherethis);

		dialogTree->rootNode = yuriaboutonyxDialog1;
		dialogTree->resourceID = "48004.avi";
		dialogTree->show(this);
	END

	BEG
		GameFlag.set(fOnceOnyx48000);
		GameFlag.clear(fDCK2);					//make Yasevno not available in travelink			 
		intrface->enable();
		theMovie->caller = 0;
		theMovie->stop();
////	new VLink( "48005.avi", fHeidelbergAvailable, 60 );    //DEBUG, vlink681, Holt finds Onyx
//		theGame->newRoom( new Rm45000 );
		theGame->newRoom( new Rm1000 );
	END
}
	
 
} // namespace Spycraft 
