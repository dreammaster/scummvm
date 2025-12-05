/*************************************
 *		Quit Room
 *		room 94000
 *		finished by Linus Chen
 *************************************/ 	

#include "globals.h"
#include "game.h"
#include "verbs.h"
#include "views.h"
#include "94000.h"
#include "web.h"
#include "target.h"
#include "intrpuzz.h"					 
#include "movie.h"
#include "gamebox.h"
#include "save.h"

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
extern "C" HWND hGameWnd;
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
	return FALSE;
}

/*******************************
 *
 *	Scripts
 *
 *******************************/
void Script94000::changeState ( int newState )
{
	switchTo
		user->canInput(FALSE);
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
	  	quitting = TRUE;
	  	sfxQuit();
	END	
}

