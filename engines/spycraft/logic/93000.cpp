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
 
// fLinus4 : dropped gun	 
// fLinus5 : dropped goggles NOT USED!!
// fLinus6 : get gun
// fLinus7 : shoot blake
// fLinus8 : offers backpack
// fLinus9 : after backpack
// fLinus10 : killed blake
// fLinus11 : used for the colby and sterling vlink
// fLinus12 : used for sterling's vlink before transfer to heli
// fLinus23 : used for signaling the transfer to helicopter from intrpuzz
// fLinus25 : used for colby vlink for dacha message
// fLinus35 : used for checking Grendal killed

#include "spycraft/game/globals.h"
#include "spycraft/game/game.h"
#include "spycraft/game/verbs.h"
#include "spycraft/game/views.h"
#include "spycraft/game/flag.h"
#include "spycraft/game/invent.h"
#include "spycraft/logic/93000.h"
#include "spycraft/game/movie.h"
#include "spycraft/logic/90000.h"
#include "spycraft/logic/60000.h"
#include "spycraft/game/sound.h"
#include "spycraft/game/vlink.h"
#include "spycraft/logic/1000.h"
#include "spycraft/logic/95200.h"

namespace Spycraft {

#define CRSBLINK_SPEED 5

/*********************
*
*		Room Code 
*
**********************/
extern Colby colby;
extern Flag GameFlag;
extern Intrface* intrface;
extern Invent* inventry;
extern DialTree* dialogTree;
extern BlinkCursor* normalCursor;
extern int grendlPos;
extern int canSaveColby;

DropGunIcon 	*dropGunIcon;
DropGogglesIcon *dropGogglesIcon;
GetGunIcon 		*getGunIcon;
ShootBlakeIcon 	*shootBlakeIcon;
ShootBlakeIcon	*shootGrendIcon;
Backpack		*backpack;
BackpackOn		*backpackOn;
BackpackOff		*backpackOff;


Rm93000::Rm93000()
{
	name = "Rm93000";
	show_style = VE_FLIP;
}

Rm93000::~Rm93000()
{
	canSaveColby = true;
	sfxUnlockRes( 911, RES_ATS );
}

void Rm93000::init()
{
	drawPic( -1 );
    Room::init();
	canSaveColby = false;
	sfxLoadRes( 911, RES_ATS );
	sfxLockRes( 911, RES_ATS );

	GameFlag.clear(fTunisiaAvailable);
 	intrface->disable();
  //	theGame->handsOn();
	sound1->stop();
	sfxSystemCursor ( 10 );
	curRoom->setScript(new Script93000);
}

int Rm93000::handleEvent ( MADEEventStamp *event )
{
	return false;
}

Rm93010::Rm93010()
{
	name = "Rm93010";
	show_style = VE_FLIP;
}

Rm93010::~Rm93010()
{
	canSaveColby = true;
	if (!GameFlag.test(fLinus10))
		backpack->dispose();
	else
		backpackOff->dispose();
}

void Rm93010::init()
{
	GameFlag.set(fLinus12);
	canSaveColby = false;
	if (!GameFlag.test(fLinus10))
		drawPic( 93001 );
	else
		drawPic( 93002 );

//	else if (GameFlag.test(fLinus9))
//  		drawPic( 93002 );
//	else if (GameFlag.test(fLinus8))
//  		drawPic( 93002 );
//	else if (GameFlag.test(fLinus7))
// 		drawPic( 93003 );

    Room::init();

 	intrface->enable();
  	theGame->handsOn();

	if (!GameFlag.test(fLinus10))
		backpack = new ( Backpack );
	else 
		backpackOff = new ( BackpackOff );

//	{	
//		if (GameFlag.test(fLinus9))
// 			backpackOff = new ( BackpackOff );
//		else if (GameFlag.test(fLinus8))
// 			backpackOff = new ( BackpackOff );
//		else if (GameFlag.test(fLinus7))
//			backpackOn = new ( BackpackOn );	
//	}
						
}

int Rm93010::handleEvent ( MADEEventStamp *event )
{
	return false;
}

Rm93020::Rm93020()
{
	name = "Rm93020";
	show_style = VE_FLIP;
}

Rm93020::~Rm93020()
{
	canSaveColby = true;
}

void Rm93020::init()
{
	if (!GameFlag.test(fLinus10))
		drawPic( 93004 );
	else
		drawPic( 93005 );
	canSaveColby = false;
    Room::init();

 	intrface->enable();
  	theGame->handsOn();
	
	if (!GameFlag.test(fLinus11))
	{
		GameFlag.set(fLinus11);
		new VLink ( "Change Harmonica's tune: DCI Sterling, Peter R.",
		            "93100.avi", fLinus23, 0 );
	//	new VLink ( "Procat's rudder: Colby, William",
	//              "93110.avi", 0, 0 );
	}
}

int Rm93020::handleEvent ( MADEEventStamp *event )
{
	return false;
}

// Oil refinery place
Rm93500::Rm93500()
{
	name = "Rm93500";
	show_style = VE_FLIP;
}

Rm93500::~Rm93500()
{
	sfxUnlockRes( 911, RES_ATS );
}

void Rm93500::init()
{
	drawPic( -1 );
    Room::init();	 
	sfxLoadRes( 911, RES_ATS );
	sfxLockRes( 911, RES_ATS );

	intrface->disable();
  	//theGame->handsOn();
	sound1->stop();
	sfxSystemCursor ( 10 );
	curRoom->setScript(new Script93500);
}

int Rm93500::handleEvent ( MADEEventStamp *event )
{
	return false;
}


/*******************************
 *
 *	Features
 *
 *******************************/

Backpack::Backpack()
{
	name = "Backpack";
	setHotspot( DO_V );
 	init( 120, 70, 230, 180 );
}

void Backpack::respond()
{
	if (GameFlag.test(fLinus12))
	{
		theGame->setCursor( normalCursor );
		normalCursor->blink(CRSBLINK_SPEED);
	}
}

int Backpack::doVerb( int theVerb )
{
	if (( theVerb == DO_V ) && GameFlag.test(fLinus12))
	{
		theGame->newRoom ( new Rm93020 );
//		curRoom->setScript(new ToHeliTransfer);
		return true;
	}
	return false;
}

BackpackOn::BackpackOn()
{
	name = "BackpackOn";
	setHotspot( DO_V );
 	init( 20, 120, 140, 180 );
}

void BackpackOn::respond()
{
	if (GameFlag.test(fLinus12))
	{
		theGame->setCursor( normalCursor );
		normalCursor->blink(CRSBLINK_SPEED);
	}
}

int BackpackOn::doVerb( int theVerb )
{
	if (( theVerb == DO_V ) && GameFlag.test(fLinus12))
	{
		theGame->newRoom ( new Rm93020 );
//		curRoom->setScript(new ToHeliTransfer);
		return true;
	}
	return false;
}

BackpackOff::BackpackOff()
{
	name = "BackpackOff";
	setHotspot( DO_V );
 	init( 260, 150, 160, 150 );
}

void BackpackOff::respond()
{
	if (GameFlag.test(fLinus12))
	{
		theGame->setCursor( normalCursor );
		normalCursor->blink(CRSBLINK_SPEED);
	}
}

int BackpackOff::doVerb( int theVerb )
{
	if (( theVerb == DO_V ) && GameFlag.test(fLinus12))
	{
		theGame->newRoom ( new Rm93020 );
//		curRoom->setScript(new ToHeliTransfer);	
		return true;
	}
	return false;
}

/*******************************
 *
 *	Views
 *
 *******************************/

DropGunIcon::DropGunIcon()
{
	name = "DropGunIcon";
	setHotspot ( DO_V );
	setLoop(0);
	setView(911);
	init(206, 400);
	noSkip();
}

void DropGunIcon::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink(CRSBLINK_SPEED);
}

int DropGunIcon::doVerb ( int theVerb )
{
	if ( theVerb == DO_V )	{
		GameFlag.set (fLinus4);
		theMovie->stop();
		curRoom->setScript ( new DropGunScript );
		dispose();
		return true;
	}
	return false;
}	

DropGogglesIcon::DropGogglesIcon()
{
	name = "DropGogglesIcon";
	setHotspot ( DO_V );
	setLoop(0);
	setView(908);
	init(202, 203);
	noSkip();
}

void DropGogglesIcon::respond()
{
//	theGame->setCursor( normalCursor );
//	normalCursor->blink(CRSBLINK_SPEED);
}

int DropGogglesIcon::doVerb ( int theVerb )
{
	if ( theVerb == DO_V )	{
		GameFlag.set (fLinus5);
		theMovie->fromTo(10,10);
		theMovie->play("93000.avi", this, 0, 0, 0);
		return true;
	}
	return false;
}	

GetGunIcon::GetGunIcon()
{
	name = "GetGunIcon";
	setHotspot ( DO_V );
	setLoop(2);
	setView(911);
	init(206, 400);
	noSkip();
}

void GetGunIcon::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink(CRSBLINK_SPEED);
}

int GetGunIcon::doVerb ( int theVerb )
{
	if ( theVerb == DO_V )	{
		GameFlag.set (fLinus7);
		theMovie->stop();
		curRoom->setScript ( new BlakeCaughtScript );
		dispose();
		return true;
	}
	return false;
}	

ShootBlakeIcon::ShootBlakeIcon(int who)
{
	name = "ShootBlakeIcon";
	setHotspot ( DO_V );
	person = who;
	if(person == 1)	// shoot blake
		setLoop(5);
	else setLoop(7);	// shoot grendal
	setView(911);
	init(206, 400);
	noSkip();
}

void ShootBlakeIcon::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink(CRSBLINK_SPEED);
}

int ShootBlakeIcon::doVerb ( int theVerb )
{
	if ( theVerb == DO_V )	{
		if ( person == 1 )
		{
			GameFlag.set (fLinus10);
			theMovie->stop();
			curRoom->setScript ( new BlakeDeathScript );
		}
		else 
		{
			GameFlag.set (fLinus35);
			theMovie->stop();
			curRoom->setScript ( new ShootGrendal );
		}
		dispose();
		return true;
	}
	return false;
}	

/*******************************
 *
 *	Scripts
 *
 *******************************/
void Script93500::changeState ( int newState )
{
 	switchTo
	   	ticks = 2;
	END

	BEG
	   	theMovie->fromTo(0,29);
		theMovie->play("1082.avi", this, 0, 0, 0);
	END

	BEG
		theGame->handsOn();
		shootGrendIcon = new ShootBlakeIcon(0);			
		theMovie->fromTo(30,70);						// player sees grendel
		theMovie->play("1082.avi", this, 0, 0, 0);
	END

	BEG
	   	if (!GameFlag.test(fLinus35))
		{
			cue();
		}
		else
			dispose();
	END

	BEG
		shootGrendIcon->dispose();
		theMovie->fromTo(120,263);						// Grendel shoots you
		theMovie->play("1082.avi", this, 0, 0, 0);
	END

	BEG
		theMovie->stop();
		theGame->newRoom ( new Rm60000(END_DEAD));
	END
}

void ShootGrendal::changeState ( int newState )
{
 	switchTo
		theMovie->fromTo(270,394);
		theMovie->play("1082.avi", this, 0, 0, 0);		// kill grendel
	END
	
	BEG
		theMovie->stop();
/*
		GameFlag.set(fEndgamePitStopped);
		GameFlag.clear(fHeidelbergAvailable);
		curMap = MAP_HEIDELBERG;
	
		new VLink( "You've saved us all: DCI Sterling, Peter R.","1077.avi", 0, 0);
			//vlink congrats
		new VLink( "Red line communication: Oval Office", "1078.avi", 0, 0);
			//vlink congrats
*/
		GameFlag.set(fKilledGrendel);
		GameFlag.set(fStartCombat);
		switch (grendlPos)
		{
			case 95201:
			   	theGame->newRoom (new Rm95201);
				break;
			case 95203:
			   	theGame->newRoom (new Rm95203);
				break;
			case 95204:
			   	theGame->newRoom (new Rm95204);
				break;
			case 95206:
			   	theGame->newRoom (new Rm95206);
				break;
			case 95209:
			   	theGame->newRoom (new Rm95209);
				break;
		}
	END
}

void BlakeDeathScript::changeState ( int newState )
{
 	switchTo
		ticks = 180;
	END

	BEG
		if (GameFlag.test(fLinus9))
		{
			theMovie->fromTo(0,30);
			theMovie->play("93013.avi", this, 0, 0, 0);		// death with no back pack
		}else if (GameFlag.test(fLinus8))
		{
			theMovie->fromTo(0,42);
			theMovie->play("93012.avi", this, 0, 0, 0);	   	// death with back pack in hands
		}else if (GameFlag.test(fLinus7))
		{
			theMovie->fromTo(0,44);							// death with back pack on
			theMovie->play("93011.avi", this, 0, 0, 0);
		}
	END

	BEG
		theMovie->stop();
//		sfxPrintf("Showdown Finished - Killed Blake");
		intrface->enable();
		theGame->newRoom ( new Rm93010 );
	END
}

void DropGunScript::changeState ( int newState )
{
 	switchTo
		theMovie->fromTo(300,389);
		theMovie->play("93000.avi", this, 0, 0, 0);
	END

	BEG
		theMovie->stop();
		curRoom->setScript ( new Script93001 );
	END
}

void Script93000::changeState ( int newState )
{
	switchTo
	   	
	   	ticks = 180;
	END

	BEG
		theMovie->fromTo(0,216);							// start of sequence
		theMovie->play("93000.avi", this, 0, 0, 0);
	END
   
    BEG
		theGame->handsOn();
		new (DropGunIcon);
		theMovie->fromTo(217,270);						// Blake says drop gun
		theMovie->play("93000.avi", this, 0, 0, 0);
	END
/*	
	BEG
	   	if (!GameFlag.test(fLinus4))
			cue();
		else
			dispose();
	END
	
	BEG
	   	theMovie->fromTo(237,270);
		theMovie->play("93000.avi", this, 0, 0, 0);
	END
 */
	BEG
//		theGame->handsOff();
	   	if (!GameFlag.test(fLinus4))
			cue();
		else
			dispose();
	END

	BEG
		theMovie->stop();
		snd1->play ( 95001, this );
	END

	BEG
	   	ticks = 2;
	END

	BEG
		snd1->dispose();
//		theGame->handsOn();
		theGame->newRoom ( new Rm60000(END_DEAD));
	END
}

void Script93001::changeState ( int newState )
{
    switchTo
		theMovie->fromTo(390,704);						// Blake gun jams
		theMovie->play("93000.avi", this, 0, 0, 0);
	END

	BEG
		getGunIcon = new (GetGunIcon);					// possible frame 690, 705, 731<=last
		theMovie->fromTo(705,769);						// Blake gets his knife
		theMovie->play("93000.avi", this, 0, 0, 0);
	END

	BEG
		if (!GameFlag.test(fLinus7))
			cue();
		else
			dispose();
	END

	BEG
	   	getGunIcon->dispose();
		theMovie->fromTo(770,931);
		theMovie->play("93000.avi", this, 0, 0, 0);		// Blake kills you
	END

	BEG
	   	theMovie->stop();
		theGame->newRoom ( new Rm60000(END_DEAD));
	END
}

void BlakeCaughtScript::changeState ( int newState )
{
 	switchTo
		theMovie->fromTo(0,89);							// start of sequence
		theMovie->play("93001.avi", this, 0, 0, 0);
	END

	BEG
		shootBlakeIcon = new ShootBlakeIcon(1);				// 67 last, new 90 frame
// 		theMovie->fromTo(0,279);
		theMovie->fromTo(90,279);							// give option of kill blake
		theMovie->play("93001.avi", this, 0, 0, 0);
	END

	BEG
		if (!GameFlag.test(fLinus10))
			cue();
		else
			dispose();
	END

	BEG
		GameFlag.set (fLinus8);
		theMovie->fromTo(280,365);							// offer backpack
		theMovie->play("93001.avi", this, 0, 0, 0);
	END

	BEG
		if (!GameFlag.test(fLinus10))
			cue();
		else
			dispose();
	END

	BEG	
		GameFlag.set (fLinus9);
		theMovie->fromTo(366,584);							// after backpack
		theMovie->play("93001.avi", this, 0, 0, 0);
	END

	BEG
		if (!GameFlag.test(fLinus10))
			cue();
		else
			dispose();
	END

	BEG
		shootBlakeIcon->dispose();
	 	theMovie->fromTo(585,656);							// take away shoot option
		theMovie->play("93001.avi", this, 0, 0, 0);
	END
	
	BEG
		theMovie->stop();
		intrface->enable();
	//	sfxPrintf("End Showdown sequence - Blake free");
		theGame->newRoom ( new Rm93010 );					// Go to Helicopter room
	END
}

void ToHeliTransfer::changeState ( int newState )
{	
 	switchTo
		intrface->disable();
		sfxSystemCursor ( 10 );
//		if (!GameFlag.test(fLinus10))
//			backpack->dispose();
//		else if (GameFlag.test(fLinus9))
// 			backpackOff->dispose();
//		else if (GameFlag.test(fLinus8))
// 			backpackOff->dispose();
//		else if (GameFlag.test(fLinus7))
//			backpackOn->dispose();	
	
		theMovie->stop();
		theMovie->play ( "93003.avi", this, 0, 0, 0);

//		theMovie->fromTo ( 0, 45 );
//	   	theMovie->play ( "90000.avi", this, 0, 0, 0);

	END

	BEG
		theMovie->stop();
	   	theGame->newRoom ( new Rm90000 );
	END
}
 
} // namespace Spycraft 
