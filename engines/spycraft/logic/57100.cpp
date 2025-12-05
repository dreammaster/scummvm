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
#include "spycraft/logic/57100.h"
#include "spycraft/logic/57000.h"
#include "spycraft/logic/1000.h"
#include "movie.h"
#include "flag.h"
#include "vlink.h"
#include "spycraft/logic/60000.h"

namespace Spycraft {

#define CRSBLINK_SPEED 5

extern Intrface* intrface;
extern SouthCursor* southCursor;
extern EastCursor* eastCursor;
extern BlinkCursor* normalCursor;
extern DialTree* dialogTree;

Dialog* 	LangeDialog1;
Dialog* 	LangeDialog2;
Dialog* 	LangeDialog3;

DItem* procatWeapon;
DItem* procatClues;
DItem* killLange;

enum
{	DROPGUNLOOP,
	SHOOTGUNLOOP,
	GETGUNLOOP,
	SHOOTWARHLOOP,
	SHOOTYURILOOP,
};

enum	
{
	PROCATWEAPON,
	PROCATCLUES,
	KILLLANGE,
};

/*********************
*
*	Room 
*
**********************/

Rm57100::Rm57100()
{
	name = "Rm57100";
	show_style = VE_FLIP;
}

void Rm57100::cue( void )
{
///	theMovie->fromTo(251, 252);
///	theMovie->play("57103.avi", this, 0, 0, 0);
}

Rm57100::~Rm57100()
{
	if( GameFlag.test( fDisposeDialog57100 )) {
		LangeDialog1->dispose();
		LangeDialog2->dispose();
		LangeDialog3->dispose();

		GameFlag.clear( fDisposeDialog57100 );
	}	//endif
}

void Rm57100::init()
{
	Room::init();
	intrface->show();

	if(GameFlag.test(fHaveSwanTwo) && !GameFlag.test(fBSLangeEnter) && !GameFlag.test(fOnce57100)) {
		drawPic( 1 );
///		setScript(new EnterBlackScreenScript57101);
		sfxSystemCursor( SYS_WAIT );
		sfxLoadRes ( 911, RES_ATS );
		sfxLockRes ( 911, RES_ATS );
		setScript(new LangeEnter57100);		
	}
	else {
		drawPic( 57100 );	
		theGame->handsOn();
		
		new(EastExit57100);
		new(Calendar57100);
	}
}

int Rm57100::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}

EastExit57100::EastExit57100()
{
	name = "EastExit57100";
	setHotspot( DO_V );
	init( EAST_EXIT_X, EAST_EXIT_Y, EAST_EXIT_WIDTH, EAST_EXIT_HEIGHT );  
}

void EastExit57100::respond()
{
	theGame->setCursor( eastCursor );
	aniCursor();
}

int EastExit57100::doVerb( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom( new Rm57000 );
    	return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Features
 *
/*******************************/

Calendar57100::Calendar57100()
{
	name = "Calendar57100";
	setHotspot( DO_V );
	init( 301, 150, 50, 55 );
}

void Calendar57100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Calendar57100::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		new CalendarInset57100;
		return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Insets
 *
/*******************************/

CalendarInset57100::CalendarInset57100()
{
	normalCursor->rest();	// stops respond blink
	init( 57120, 0, 16, 512, 384 );
}

int CalendarInset57100::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
    	return TRUE;
	}
	return FALSE;
}
 
/*******************************
 *
 *	Scripts
 *
/*******************************/

LangeEnter57100::LangeEnter57100()
{
	name = "LangeEnter57100";
}

void LangeEnter57100::changeState( int newState )
{
 		switchTo
//			GameFlag.set(fBSLangeEnter);
			GameFlag.set(fDisposeDialog57100);
			theMovie->fromTo(0, 252);
			theMovie->play("57103.avi", this, 0, 0, 0);
		END

		BEG
//			new(GunLange57100);
			
			dialogTree = new DialTree;
			LangeDialog1 = new Dialog;
			LangeDialog2 = new Dialog;
			LangeDialog3 = new Dialog;
	
//			procatWeapon = new DItem;
			procatWeapon = new LangeShootThorn;
			procatWeapon->addInfo(57100, PROCATWEAPON, LangeDialog3);
//			procatClues = new DItem;
			procatClues = new LangeShootThorn;
			procatClues->addInfo(57100, PROCATCLUES, LangeDialog3);
////		killLange = new DItem;
////		killLange->addInfo(57100, KILLLANGE, LangeDialog2);
			killLange = new ThornShootLange;
			killLange->addInfo(	911, SHOOTGUNLOOP, 0, 410, 400, LangeDialog2 );

			LangeDialog1->addMovieInfo(0, 252, 0, 0);
			LangeDialog2->addMovieInfo(254, 367, 0, 0);
			LangeDialog3->addMovieInfo(368, 456, 0, 0);
	
			LangeDialog1->add(procatWeapon);
			LangeDialog1->add(procatClues);
			LangeDialog1->add(killLange);

			dialogTree->rootNode = LangeDialog1;
			dialogTree->resourceID = "57103.avi";
			dialogTree->show(this);
	END

	BEG
		GameFlag.set(fBSLangeEnter);
		GameFlag.clear(fDCK1);
		
		intrface->enable();
		theMovie->caller = 0;
		theMovie->stop();
		sfxUnlockRes(911, RES_ATS);

//		if((GameFlag.test(fCanDoMaxIntrVLink)) && (!GameFlag.test(fWahKa73))) {
//			GameFlag.set(fWahKa73);
		//	new VLink( "101.avi", fWahKa18, 0);			//carrot and stick dialog
//			theGame->newRoom(new Rm1000);
//			theGame->newRoom(new Rm57000);
//  	}
		if(GameFlag.test(fPlayerKilled)) {
			//endType = END_DEAD;
			theGame->newRoom(new Rm60000(END_DEAD));	//game over
		}
		else {
//	    	theGame->newRoom(new Rm1000);
			new EMail( "Tough call: Holt, David M.", 98214, 0, 30);	
			theGame->newRoom(new Rm57000);
		}
	END
}

/*******************************
 *
 *	Dialog
 *
/*******************************/

LangeShootThorn::LangeShootThorn()
{
	name = "LangeShootThorn";
}

void LangeShootThorn::doit()
{
	GameFlag.set(fPlayerKilled);

	DItem::doit();
}

ThornShootLange::ThornShootLange()
{
	name = "ThornShootLange";
}

void ThornShootLange::doit()
{
	myDialog->hide();
	theMovie->pause(FALSE);
	//cue();
	DItem::doit();
}
/*******************************
 *
 *	Views
 *
 *******************************/

GunLange57100::GunLange57100()
{
	name = "GunLange57100";
	setHotspot ( DO_V );
	setView(911);
	init(410, 400);
	view = 911;
	loop = 2;
	cel = 0;
	noSkip();
}

void GunLange57100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink(CRSBLINK_SPEED);
}

int GunLange57100::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		killLange = new ThornShootLange;
		killLange->addInfo(57100, KILLLANGE, LangeDialog2);
		killLange->addInfo(	911, 1, 0, 410, 400, LangeDialog2 );

    	return TRUE;
	}
	return FALSE;
}

/* Room 57101 */
/*
Rm57101::Rm57101()
{
	name = "Rm57101";
	show_style = VE_FLIP;
}

Rm57101::~Rm57101()
{
}

void Rm57101::init()
{
  	drawPic( 1 );
    Room::init();

  	intrface->show();
  	theGame->handsOn();

	setScript( new LangeEnter57100 );
}

void Rm57101::cue( void )
{
	theMovie->fromTo(251, 252);
	theMovie->play("57103.avi", this, 0, 0, 0);
}

int Rm57101::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}

EnterBlackScreenScript57101::EnterBlackScreenScript57101()
{
	name = "EnterBlackScreenScript57101"; 
}

void EnterBlackScreenScript57101::changeState( int newState )
{
   	GameFlag.set(fOnce57100);		
	theGame->newRoom( new Rm57101 );
}
*/
 
} // namespace Spycraft 
