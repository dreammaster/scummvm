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
#include "movie.h"
#include "verbs.h"
#include "views.h"
#include "flag.h"
#include "vlink.h"
#include "roomsnd.h"
#include "1270.h"
#include "1000.h"
#include "intrface.h"
#include "pda.h"
#include "invent.h"

namespace Spycraft {

extern Intrface* intrface;

extern PDA* thePDA;
extern Briefcase* theCase;

/*********************				  
*
*		Room Code 
*
**********************/

Rm1270::Rm1270()
{
	name = "Rm1270";
	show_style = VE_FLIP;
}

Rm1270::~Rm1270()
{
	/*sfxUnlockRes(2, RES_PIC);
	sfxUnlockRes(666, RES_ATS);
	sfxUnlockRes(667, RES_ATS);*/
	if (theGame)
		theGame->flag = FALSE;
}
						  
 
void Rm1270::init()
{	

	drawPic( -1 );
    Room::init();
    intrface->show();	
  /*	sfxLoadRes(2, RES_PIC);
	sfxLockRes(2, RES_PIC);
	sfxLoadRes(666, RES_ATS);
	sfxLockRes(666, RES_ATS);
	sfxLoadRes(667, RES_ATS);
	sfxLockRes(667, RES_ATS);*/
	inventry->get(iLOCKPICK);
	
	setScript( new NewspaperScript1270 ); // first
}

void Rm1270::cue ( void )
{
	theMovie->fromTo(60, 185);
	theMovie->play("1271.avi", this, 0, 0, 0);
}

/*******************************
 *
 *	Scripts
 *
 *******************************/


void NewspaperScript1270::changeState ( int newState )
{
	switchTo
		//theGame->handsOff();
		ticks = 2;
	END
   
	BEG	
		#ifndef DIRECTX
			theMovie->play( "1270.avi", this, FALSE, TRUE, FALSE ); // Credo
		#else
			theMovie->play( "1270.avi", this, FALSE, 2, FALSE ); // Credo
		#endif
	END

	BEG
		ticks = 10;
	END

	BEG
		theMovie->fromTo(0, 56);
		theMovie->play( "1271.avi", this, FALSE, FALSE, FALSE ); // newspaper
	END

	BEG
		theMovie->fromTo(60, 185); 
		theMovie->play("1271.avi", curRoom, 0, 0, 0);

		
		intrface->isOpen = TRUE;
		theCase = new(Briefcase);
  		//theHolster = new(Holster);
  		thePDA = new(PDA);

		theCase->view = vINTERFACE;
		theCase->loop = 0;
		theCase->cel = 0;
  		theCase->init(10, 480); //74
		//theHolster->init(530,480);
  		thePDA->init(391,480);	//455
		intrface->enable();
		
		theGame->enableIntrDuringMovie = TRUE;
		setScript(new OpenInterface, this);
	END

	BEG
		user->canInput(TRUE);
		ticks = 10;
	END
	BEG
		
		new VLink( "Priority one: DDO Warhurst, Eugene J.","41.avi", fTravelLinkAvail, this );
		
	END
	BEG
		theMovie->stop();
		theMovie->paused = FALSE;
		ticks = 10;
	END
	BEG
		curMap = MAP_WASHINGTON;	// global in 1000.cpp
		drawBackground = FALSE;
		theGame->enableIntrDuringMovie = FALSE;
		theMovie->play("42.avi", this, 0, TRUE, 0);
	END
	BEG
		GameFlag.set(fInitialBriefing);
		theGame->newRoom(new Rm1000);
	END
}




 
} // namespace Spycraft 
