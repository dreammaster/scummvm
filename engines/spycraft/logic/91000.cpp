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
#include "spycraft/logic/91000.h"
#include "web.h"
#include "target.h"
#include "intrpuzz.h"					 
#include "spycraft/game/movie.h"
#include "spycraft/logic/1000.h"
#include "spycraft/logic/60000.h"
#include "save.h"

namespace Spycraft {

/*********************
*
*		Room Code 
*
**********************/

extern Intrface* intrface;
extern SaveGame* saveGame;
extern short restoring; 

int didRestore;

Rm91000::Rm91000()
{
	name = "Rm91000";
	show_style = VE_FLIP;
}

Rm91000::~Rm91000()
{
}

void Rm91000::init()
{
  	drawPic(-1);
    Room::init();
	user->canInput(false);
	
	if (!restoring)	{
		sfxPrintf("Autosaving in Red Square");
		#ifndef DOS4GW
			saveGame->save("Target Harmonica.sgm");
		#else
			char* tmpSave;
			SaveDlg("sgm");
			tmpSave = sfxGetSaveFile();
			if (strcmp(tmpSave, ""))
				saveGame->save(tmpSave);
		#endif
	}
	else
		didRestore = true;
	curRoom->setScript(new Script91000);
}

void Rm91000::cue()
{
//	theMovie->fromTo(5490,5600);
// 	theMovie->play("250.avi",this, 0, 0, 0);	   	// square loop
	theMovie->fromTo(653,763);
   	theMovie->play("253.avi",this, 0, 0, 0);	   	// square loop
}

int Rm91000::handleEvent ( MADEEventStamp *event )
{
	return false;
}

/*******************************
 *
 *	Scripts
 *
 *******************************/
void Script91000::changeState ( int newState )
{
	switchTo
		sfxSystemCursor( 10 );
		if (!didRestore)
			theMovie->play("90000.avi", this, 0, 1, 0);		// Transfer
		else
			cue();
	END

	BEG
		if (!didRestore)	{
			theMovie->stop();
	   		theMovie->play("91001.avi", this, 0, 1, 0);	   	// Sterling on tarmac
		}
		else
			cue();
	END

	BEG
		if (!didRestore)	{
			theMovie->stop();
	   		theMovie->play("91000.avi", this, 0, 1, 0);	   	// Airforce One
		}
		else
			cue();
	END

	BEG
		didRestore = false;
		theMovie->stop();
//		theMovie->fromTo(5250,5487);
//	   	theMovie->play("250.avi", this, 0, 0, 0);	   	// square flyby
		theMovie->fromTo(413,649);
	   	theMovie->play("253.avi", this, 0, 0, 0);	   	// square flyby
	END

	BEG
		user->canInput(true);
		theGame->enableIntrDuringMovie = true;
		intrface->enable();
		intrface->blink();
//		theMovie->fromTo(5490,5600);
//	   	theMovie->play("250.avi",curRoom, 0, 0, 0);	   	// square loop
		theMovie->fromTo(653,763);
	   	theMovie->play("253.avi",curRoom, 0, 0, 0);	   	// square loop
	END
}

void ArrestedScript91000::changeState ( int newState )
{
 	switchTo
		theMovie->stop();
		theMovie->play("91002.avi", this, 0, 1, 0);
	END

	BEG
		theMovie->stop();
	   	theGame->newRoom ( new Rm60000(END_JAIL));	
	END
}

void GoTargetScript::changeState ( int newState )
{
 	switchTo
		intrface->blinking = false;
		if (pdaProp)
		{
			intrface->release(pdaProp);
			pdaProp->setCycle(0);
			pdaProp->setHotspot(NULL);
			delete pdaProp;
			pdaProp = NULL;
		}
		theGame->enableIntrDuringMovie = false;
		theMovie->stop();
		theMovie->fromTo(2108,2223);
		theMovie->play("253.avi", this,false,true,false);
//		theMovie->fromTo(6945,7060);
//		theMovie->play("250.avi", this,false,true,false);
	END

	BEG
		theMovie->stop();
		theMovie->fromTo(2753,3095);
		theMovie->play("253.avi", this,false,true,false);
//		theMovie->fromTo(7590,7932);
//		theMovie->play("250.avi", this,false,true,false);
	END
	BEG
//		if (GameFlag.test(fTargetFreqAvailable))
//		{
			theMovie->stop();
			theMovie->fromTo(2243,2746);
			theMovie->play("253.avi", this,false,true,false);
//			theMovie->fromTo(7080,7584);
//			theMovie->play("250.avi", this,false,true,false);
//		}	
//		else
//			cue();
	END
	BEG
		GameFlag.set(fPatsHTM_127);
		new Web;
		new Target;
	END
}

 
} // namespace Spycraft 
