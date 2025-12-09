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
#include "spycraft/logic/94000.h"
#include "spycraft/game/web.h"
#include "spycraft/game/target.h"
#include "spycraft/game/interface_puzzle.h"					 
#include "spycraft/game/movie.h"
#include "spycraft/dmade/gamebox.h"
#include "spycraft/game/save.h"

namespace Spycraft {

/*********************
*
*		Room Code 
*
**********************/
extern Intrface* intrface;
#ifdef DOS4GW
extern "C" {
int dlgOpenProc(int id);
}
#define hGameWnd NULL
#else
extern HWND hGameWnd;
#endif
extern Intrface* intrface;

int SaveYes;

extern SaveGame* saveGame; 
extern int quitting;


Rm94000::Rm94000()
{
	name = curRoom->name;
}

Rm94000::~Rm94000()
{
}

void Rm94000::init()
{
  //	drawPic(-1);
    Room::init();
	intrface->disable();
	curRoom->setScript(new Script94000);
}

void Rm94000::cue()
{
}

int Rm94000::handleEvent ( MADEEventStamp *event )
{
	return false;
}

/*******************************
 *
 *	Scripts
 *
 *******************************/
void Script94000::changeState ( int newState )
{
	switchTo
		user->canInput(false);
		sfxSystemCursor(SYS_ARROW);
		SaveYes = MessageBox(hGameWnd, "Do you wish to Save before quitting?",
		                               "Leaving Spycraft", MB_ICONQUESTION | MB_SETFOREGROUND	| MB_YESNO);
	//	SaveYes = MessageBox( NULL, "Do you wish to Save before quitting?", "Leaving Spycraft", MB_YESNO );	
		if( SaveYes == IDYES ) {
			char* tmpSave;
			SaveDlg("sgm");
			tmpSave = sfxGetSaveFile();
			if (strcmp(tmpSave, ""))
				saveGame->save(tmpSave);
				
			ticks = 30;	
		}else cue();
	END
	
	BEG
	  	quitting = true;
	  	sfxQuit();
	END	
}

 
} // namespace Spycraft 
