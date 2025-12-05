/*************************************
 *		Helicopter Transfer to Target 
 *		room 91000
 *		finished by Linus Chen
 *************************************/ 	

#include "globals.h"
#include "game.h"
#include "verbs.h"
#include "views.h"
#include "91000.h"
#include "web.h"
#include "target.h"
#include "intrpuzz.h"					 
#include "movie.h"
#include "1000.h"
#include "60000.h"
#include "save.h"

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
	user->canInput(FALSE);
	
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
		didRestore = TRUE;
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
	return FALSE;
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
		didRestore = FALSE;
		theMovie->stop();
//		theMovie->fromTo(5250,5487);
//	   	theMovie->play("250.avi", this, 0, 0, 0);	   	// square flyby
		theMovie->fromTo(413,649);
	   	theMovie->play("253.avi", this, 0, 0, 0);	   	// square flyby
	END

	BEG
		user->canInput(TRUE);
		theGame->enableIntrDuringMovie = TRUE;
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
		intrface->blinking = FALSE;
		if (pdaProp)
		{
			intrface->release(pdaProp);
			pdaProp->setCycle(0);
			pdaProp->setHotspot(NULL);
			delete pdaProp;
			pdaProp = NULL;
		}
		theGame->enableIntrDuringMovie = FALSE;
		theMovie->stop();
		theMovie->fromTo(2108,2223);
		theMovie->play("253.avi", this,FALSE,TRUE,FALSE);
//		theMovie->fromTo(6945,7060);
//		theMovie->play("250.avi", this,FALSE,TRUE,FALSE);
	END

	BEG
		theMovie->stop();
		theMovie->fromTo(2753,3095);
		theMovie->play("253.avi", this,FALSE,TRUE,FALSE);
//		theMovie->fromTo(7590,7932);
//		theMovie->play("250.avi", this,FALSE,TRUE,FALSE);
	END
	BEG
//		if (GameFlag.test(fTargetFreqAvailable))
//		{
			theMovie->stop();
			theMovie->fromTo(2243,2746);
			theMovie->play("253.avi", this,FALSE,TRUE,FALSE);
//			theMovie->fromTo(7080,7584);
//			theMovie->play("250.avi", this,FALSE,TRUE,FALSE);
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

