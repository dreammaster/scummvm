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
#include "spycraft/logic/35000.h"
#include "spycraft/logic/35300.h"
#include "invent.h"
#include "flag.h"
#include "vlink.h"
#include "phone.h"
#include "advcurs.h"

namespace Spycraft {

#define CRSBLINK_SPEED 5

extern Flag GameFlag;
extern Invent* inventry;
extern DialTree* dialogTree;
extern Intrface* intrface;

//Print* showDigit;

Dialog* 	yuriphoneDialog1;
Dialog* 	yuriphoneDialog2;
Dialog* 	yuriphoneDialog3;
Dialog* 	yuriphoneDialog4;
Dialog* 	yuriphoneDialog5;

DItem* thornCall;
DItem* investigatingMafia;
DItem* goodMeet;
DItem* thankGromchevsky;

enum	
{
	THORNCALL,
	INVESTIGATINGMAFIA,
	GOODMEET,
	THANKGROMCHEVSKY,
};

/*********************
*
*		Room Code 
*
**********************/

extern Intrface* intrface;
extern BlinkCursor* normalCursor;
extern SouthCursor* southCursor;

Rm35300::Rm35300()
{
	name = "Rm35300";
	show_style = VE_FLIP;
}

Rm35300::~Rm35300()
{
//	showDigit->dispose();

	if( GameFlag.test( fDisposeDialog35000 )) {
		yuriphoneDialog1->dispose();
		yuriphoneDialog2->dispose();
		yuriphoneDialog3->dispose();
		yuriphoneDialog4->dispose();
		yuriphoneDialog5->dispose();
		GameFlag.clear( fDisposeDialog35000 );
	}  //endif
}

void Rm35300::init()
{
  	drawPic( 35195 );
    Room::init();
  	
// 	showDigit = new Print;

	new(SouthExit35300);
	new(Light35300);
//	new(DigitOne35300);
	
  	intrface->show();
  	theGame->handsOn();
}

int Rm35300::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}

SouthExit35300::SouthExit35300()
{
	name = "SouthExit35300";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
}

void SouthExit35300::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit35300::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm35100);
    	return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Features
 *
/*******************************/

Light35300::Light35300()
{
	name = "Light35300";
	setHotspot( DO_V );
//	init( 374, 210, 84, 75 );		//number pad
	init( 270, 125, 187, 187);
}

void Light35300::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Light35300::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		theGame->newRoom(new PhoneKeyPad(FROM_RUSSIA));
		return TRUE;
	}
/*
		if( GameFlag.test( fMORoomEnter ) && !GameFlag.test(fOnce35001)) {
			GameFlag.set(fOnce35001);
//			sfxPrintf("Yuri's phone dialogue, #282 - see you in my office");
			setScript( new EnterScriptYuriVO35000 );
		}
		else {
			sfxPrintf("All phone lines are busy, please try again later.");
		}
		return TRUE;
	}
*/
	return FALSE;
}

DigitOne35300::DigitOne35300()
{
	name = "DigitOne35300";
	setHotspot( DO_V );
	init( 371, 212, 28, 19 );
}

void DigitOne35300::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int DigitOne35300::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
//		showDigit->x = 265;
//		showDigit->y = 145;
//		showDigit->size = 24;
//		showDigit->fore_color = madeRGB(230, 230,230);
//		showDigit->init( "1");
		return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Scripts
 *
/*******************************/

EnterScriptYuriVO35000::EnterScriptYuriVO35000()
{
	name = "EnterScriptYuriVO35000"; 
}

void EnterScriptYuriVO35000::changeState( int newState )
{
	switchTo
		theGame->handsOff();
		sfxSystemCursor( 10 );
		GameFlag.set(fDisposeDialog35000);
		GameFlag.set(fOnce35001);
  		theMovie->fromTo(0, 23);

//		theMovie->fromTo(0, 20);
//		theMovie->play("35105.avi", this, 0, 0, 0);

#ifdef DIRECTX
		theMovie->play("35105.avi", this, 0, -1, 1);
#else	
		theMovie->play("35105.avi", this, 0, 0, 1);
#endif

	END

	BEG
		theGame->handsOn();
		sfxSystemCursor( 10 );		
		dialogTree = new DialTree;
		dialogTree->negativeParam = TRUE;
		yuriphoneDialog1 = new Dialog;
		yuriphoneDialog2 = new Dialog;
		yuriphoneDialog3 = new Dialog;
		yuriphoneDialog4 = new Dialog;
	   	yuriphoneDialog5 = new Dialog;

		thornCall = new DItem;
		thornCall->addInfo(35005, THORNCALL, yuriphoneDialog2);
		investigatingMafia = new DItem;
		investigatingMafia->addInfo(35005, INVESTIGATINGMAFIA, yuriphoneDialog3);
		goodMeet = new DItem;
		goodMeet->addInfo(35005, GOODMEET, yuriphoneDialog4);
		thankGromchevsky = new DItem;
		thankGromchevsky->addInfo(35005, THANKGROMCHEVSKY, yuriphoneDialog5);

//		yuriphoneDialog1->addMovieInfo(0, 20, 0, 0);
//		yuriphoneDialog2->addMovieInfo(21, 69, 0, 0);
//		yuriphoneDialog3->addMovieInfo(70, 112, 0, 0);
//		yuriphoneDialog4->addMovieInfo(113, 200, 0, 0);
//	   	yuriphoneDialog1->addMovieInfo(201, 289, 0, 0);

		yuriphoneDialog1->addMovieInfo(0, 23, 0, 0);
		yuriphoneDialog2->addMovieInfo(30, 79, 0, 0);
		yuriphoneDialog3->addMovieInfo(90, 136, 0, 0);
		yuriphoneDialog4->addMovieInfo(150, 231, 0, 0);
	   	yuriphoneDialog1->addMovieInfo(240, 276, 0, 0);


		yuriphoneDialog1->add(thornCall);
		yuriphoneDialog2->add(investigatingMafia);
		yuriphoneDialog3->add(goodMeet);
		yuriphoneDialog4->add(goodMeet);
		yuriphoneDialog5->add(thankGromchevsky);


		dialogTree->rootNode = yuriphoneDialog1;
		dialogTree->resourceID = "35105.avi";
		dialogTree->show(this);
	END

	BEG
		theGame->handsOff();
	   	theMovie->fromTo ( 275, 276 );

#ifdef DIRECTX
		theMovie->play("35105.avi", this, 0, 2, 1);
#else	
		theMovie->play("35105.avi", this, 0, 1, 1);
#endif

	END

	BEG
		theGame->handsOn();
//		intrface->enable();
		theMovie->caller = 0;
		theMovie->stop();
//		theGame->newRoom(new Rm35100);
	END
}

CallScript35100::CallScript35100()
{
	name = "CallScript35100"; 
}

void CallScript35100::changeState( int newState )
{	   
	Sound* snd;

	switchTo
		snd = new(Sound);
		snd->number = 35122; 
		snd->play();
	END
}
	  
 
} // namespace Spycraft 
