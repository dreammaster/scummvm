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
#include "38000.h"
#include "1000.h"
#include "flag.h"
#include "invent.h"
#include "roomsnd.h"
#include "60000.h"
#include "96000.h"

namespace Spycraft {

#define CRSBLINK_SPEED 5

/*********************
*
*		Room Code 
*
**********************/

extern BlinkCursor* normalCursor;
extern Intrface* intrface;
extern NorthCursor* northCursor;
extern SouthCursor* southCursor;
extern Flag GameFlag;
extern Sound* sound1;

Sound* snd38000;


Rm38000::Rm38000()
{
	name = "Rm38000";
	show_style = VE_FLIP;
}

Rm38000::~Rm38000()
{
}

void Rm38000::init()
{
  	drawPic( 38000 );
    Room::init();  

	snd38000 = new Sound;
  	
  	//new(SouthExit38000);	--no turning back
	new(InsideVan38000);
	
  	intrface->show();
  	theGame->handsOn();

	if(!GameFlag.test(fWahKa2) && (snd38000->number != 38001 || snd38000->isPlaying()) ) {
		GameFlag.set(fWahKa2);
//		snd38000->number = 38001; 
//		snd38000->play();
		snd38000->playBuffered(38001, 0);

	}
}

int Rm38000::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}

SouthExit38000::SouthExit38000()
{
	name = "SouthExit38000";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
}

void SouthExit38000::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit38000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
 		theGame->newRoom(new Rm1000);
    	return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Features
 *
/*******************************/

InsideVan38000::InsideVan38000()
{
  	name = "InsideVan38000";
	setHotspot( DO_V );
//	init(210, 47, 128, 205);   
	init(74, 66, 369, 231);
}

void InsideVan38000::respond()
{
	theGame->setCursor( northCursor );
}

int InsideVan38000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		curMap = MAP_ASSASS;		//make assassnation window appear in travelink.
		sfxSystemCursor( SYS_WAIT );
		sfxLoadRes(38000, RES_WAVE);
		theGame->newRoom(new Rm38100);
    	return TRUE;
	}
	return FALSE;
} 

Rm38100::Rm38100()
{
	name = "Rm38100";
	show_style = VE_FLIP;
}

Rm38100::~Rm38100()
{
// 	snd38000->dispose();
//	sound1->stop();
}

void Rm38100::init()
{
  	drawPic( 38100 );
    Room::init();

  	GameFlag.set(fEnteredProcatVan);
	
//	new(SouthExit38100);
//	new(VanHole38100);
	new(FolderHole38100);
	
  	intrface->show();
 	theGame->handsOn();

	if( sound1->number != 38000  || !sound1->isPlaying() )	{
		sound1->loop = -1; 
		sound1->play(38000, 0);
//		sound1->playBuffered(38000, 0);
	}
	
	setScript( new HandOverFileScript38100 );						
/*	
	if(!GameFlag.test(fWahKa3) && (snd38000->number != 38001 || snd38000->isPlaying()) ) {
		snd38000->stop();
		if(snd38000->number != 38002 || snd38000->isPlaying()) {
			GameFlag.set(fWahKa3);
			snd38000->number = SND_VO_CONTROLLER_WANT; 		 //38002.wav
			snd38000->play();
//			snd38000->playBuffered(38002, 0);
		}
	}
*/
}

int Rm38100::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}

SouthExit38100::SouthExit38100()
{
	name = "SouthExit38100";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
}

void SouthExit38100::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit38100::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		if(GameFlag.test(fWahKa4)) {
			curRoom->setScript(new ProcatTeamScript38100);
		}
		else  { 		//sound of hiss of gas......Thorn dies.
			snd38000->stop();				//stop
			theGame->newRoom(new Rm60000(END_DEAD));
		}
    	return TRUE;
	}
	return FALSE;
} 

FolderHole38100::FolderHole38100()
{
	name = "FolderHole38100";
	setHotspot ( DO_V );
	init (146, 93, 195, 135); 
	hotspot->addRespondVerb(ALL_V);}

void FolderHole38100::respond()
{
	normalCursor->blink( CRSBLINK_SPEED );
}

int FolderHole38100::doVerb ( int theVerb )
{
	if ( invVerb == FOLDERPAYMENTS_V) {
		if(!GameFlag.test(fWahKa4)&& (snd38000->number != 38002 || snd38000->isPlaying()) )	{
			snd38000->stop();
			if(snd38000->number != 38003 || snd38000->isPlaying()) {
				inventry->clearCurrentItem();
				GameFlag.set(fWahKa4);
//				snd38000->number = SND_VO_CORRECTFILES;   //38003.wav
//				snd38000->play();
				snd38000->playBuffered(38003, 0);
				inventry->put(iFOLDER_PAYMENTS);
				theGame->handsOff();
				curRoom->setScript(new ProcatTeamScript38100);
			}
		}
		return TRUE;
	}
	else if( invVerb != FOLDERPAYMENTS_V ) {
		inventry->clearCurrentItem();
		theGame->handsOff();
		curRoom->setScript(new StupidThornScript38100);
		return TRUE;
	}
	else if( theVerb == DO_V ) {
		return TRUE;
	}
	return FALSE;
}

HandOverFileScript38100::HandOverFileScript38100()
{
	name = "HandOverFileScript38100"; 
}

void HandOverFileScript38100::changeState( int newState )
{
	switchTo
		if(!GameFlag.test(fWahKa3) && (snd38000->number != 38001 || snd38000->isPlaying()) ) {
			snd38000->stop();
			if(snd38000->number != 38002 || snd38000->isPlaying()) {
				GameFlag.set(fWahKa3);
//				snd38000->number = SND_VO_CONTROLLER_WANT; 		 //38002.wav
//				snd38000->play(38002, this);
				snd38000->playBuffered(38002, this);
			}
		}
	END
	BEG
		ticks = 2;
	END
	BEG
		theGame->handsOff();
		sfxLoadRes(38200, RES_PIC);
		theGame->newRoom(new Rm38200);
	END
}

Rm38200::Rm38200()
{
	name = "Rm38200";
	show_style = VE_FLIP;
}

Rm38200::~Rm38200()
{
	snd38000->dispose();
}

void Rm38200::init()
{
  	drawPic( 38200 );
    Room::init();  

	new(VanHole38100);
	
  	intrface->show();
  	theGame->handsOn();
}

int Rm38200::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}

/*******************************
 *
 *	Features
 *
/*******************************/
    
VanHole38100::VanHole38100()
{
  	name = "VanHole38100";
	setHotspot( DO_V );
//	hotspot->addRespondVerb(FOLDERPAYMENTS_V); 
//	init (210, 47, 128, 205); 
	init (146, 93, 195, 135); 
	hotspot->addRespondVerb(ALL_V);
}

void VanHole38100::respond()
{
	normalCursor->blink( CRSBLINK_SPEED );
}

int VanHole38100::doVerb ( int theVerb )
{
/*
	if ( theVerb == FOLDERPAYMENTS_V) {
		if(!GameFlag.test(fWahKa4)&& (snd38000->number != 38002 || snd38000->isPlaying()) )	{
			snd38000->stop();
			if(snd38000->number != 38003 || snd38000->isPlaying()) {
				inventry->clearCurrentItem();
				GameFlag.set(fWahKa4);
//				snd38000->number = SND_VO_CORRECTFILES;   //38003.wav
//				snd38000->play();
				snd38000->playBuffered(38003, 0);
				
				theGame->handsOff();
				curRoom->setScript(new ProcatTeamScript38100);
			}
		}
		return TRUE;
	}
	else if ( theVerb == FOLDERCITATION_V || theVerb == DOSSIERZHIRONEV_V || theVerb == DOSSIERSHUSTAK_V || theVerb == FOLDERORDERS_V) {
		inventry->clearCurrentItem();
		theGame->handsOff();
		curRoom->setScript(new StupidThornScript38100);
		return TRUE;
	}
	else { 		//nothing will happen....must be a folder file from Max's drawer.
	} 
	return FALSE;
*/
	if ( invVerb == FOLDERPAYMENTS_V) {
		if(!GameFlag.test(fWahKa4)&& (snd38000->number != 38002 || snd38000->isPlaying()) )	{
			snd38000->stop();
			if(snd38000->number != 38003 || snd38000->isPlaying()) {
				inventry->clearCurrentItem();
				GameFlag.set(fWahKa4);
//				snd38000->number = SND_VO_CORRECTFILES;   //38003.wav
//				snd38000->play();
				snd38000->playBuffered(38003, 0);
				inventry->put(iFOLDER_PAYMENTS);
				theGame->handsOff();
				curRoom->setScript(new ProcatTeamScript38100);
			}
		}
		return TRUE;
	}
	else if( invVerb != FOLDERPAYMENTS_V ) {
		inventry->clearCurrentItem();
		theGame->handsOff();
		curRoom->setScript(new StupidThornScript38100);
		return TRUE;
	}
	else if( theVerb == DO_V ) {
		return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Scripts
 *
/*******************************/

ProcatTeamScript38100::ProcatTeamScript38100()
{
	name = "ProcatTeamScript38100"; 
}

void ProcatTeamScript38100::changeState( int newState )
{
	switchTo
		ticks = 2;
		theGame->handsOff();
		sound1->stop();
		snd38000->stop();
	END
	BEG
//		snd38000->play(38005, this);
		snd38000->playBuffered(38005, this);
	END
	BEG
		ticks = 2;
	END
	BEG
//		snd38000->play(38006, this);
		snd38000->playBuffered(38006, this);
	END
	BEG
		ticks = 2;
	END
	BEG
//		theGame->newRoom(new Rm1000);
//		theGame->newRoom(new Rm96000);
		snd38000->stop();					//stop
		theGame->newRoom(new Rm38300);
	END
}

StupidThornScript38100::StupidThornScript38100()
{
	name = "StupidThornScript38100"; 
}

void StupidThornScript38100::changeState( int newState )
{
	switchTo
		if(!GameFlag.test(fWahKa5) && (snd38000->number != 38002 || snd38000->isPlaying()) ) {
			snd38000->stop();
			if(snd38000->number != 38004 || snd38000->isPlaying()) {
				GameFlag.set(fWahKa5);
//				snd38000->number = SND_VO_INCORRECTFILES;  //38004.wav
//				snd38000->play(38004, this);
				snd38000->playBuffered(38004, this);
			}
		}
	END
	BEG
	   	theMovie->fromTo(0, 100);
		theMovie->play("38100.avi", this, 0, 1, 0);
	END
	BEG
		ticks = 2;
	END
	BEG										//stop
		snd38000->stop();
		theGame->newRoom(new Rm60000(END_DEAD));
	END
}

Rm38300::Rm38300()
{
	name = "Rm38300";
	show_style = VE_FLIP;
}

Rm38300::~Rm38300()
{
}

void Rm38300::init()
{
  	drawPic( 38200 );
    Room::init();  

	new(SouthExit38300);

	if(!GameFlag.test(fWahKa116)) {
		sfxSystemCursor( SYS_WAIT );
		sfxLoadRes ( 38160, RES_ATS );
		sfxLockRes ( 38160, RES_ATS );
		new(PegBullet38300);
	}
	
  	intrface->show();
  	theGame->handsOn();
		
	if( sound1->number != 38007  || !sound1->isPlaying() )	{
		sound1->loop = -1; 
		sound1->playBuffered(38007, 0);
	}
}

int Rm38300::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}

SouthExit38300::SouthExit38300()
{
	name = "SouthExit38300";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
}

void SouthExit38300::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit38300::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
 		theGame->newRoom(new Rm38400);
    	return TRUE;
	}
	return FALSE;
}

PegBullet38300::PegBullet38300()
{
	name = "PegBullet38300";
	setHotspot ( DO_V );
	setView(38120);
	init ( 200, 190 );
}

void PegBullet38300::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink(CRSBLINK_SPEED);
}

int PegBullet38300::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		GameFlag.set(fWahKa116);
		sfxUnlockRes(38160, RES_ATS);
		inventry->get(iNEEDLEPACK);
		inventry->items[iNEEDLEPACK]->activate();
		new PegBulletInset38300();	
		dispose();
    	return TRUE;
	}
	return FALSE;
}

PegBulletInset38300::PegBulletInset38300()
{
 	normalCursor->rest();
 	init(38160, 0, 15, 512, 385);	
}

Rm38400::Rm38400()
{
	name = "Rm38400";
	show_style = VE_FLIP;
}

Rm38400::~Rm38400()
{
}

void Rm38400::init()
{
  	drawPic( 38300 );
    Room::init();  
	
	new(SouthExit38400);
	new(NorthExit38400);

  	intrface->show();
  	theGame->handsOn();
}

int Rm38400::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}

SouthExit38400::SouthExit38400()
{
	name = "SouthExit38400";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
}

void SouthExit38400::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit38400::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		sound1->stop();
// 		theGame->newRoom(new Rm96000);
		theGame->newRoom(new Rm1000);
    	return TRUE;
	}
	return FALSE;
}

NorthExit38400::NorthExit38400()
{
	name = "NorthExit38400";
	setHotspot ( DO_V );
	init(74, 66, 369, 231);
}

void NorthExit38400::respond()
{
	theGame->setCursor(northCursor);
	aniCursor();
}

int NorthExit38400::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		theGame->newRoom(new Rm38300);
    	return TRUE;
	}
	return FALSE;
}

Rm38500::Rm38500()
{
	name = "Rm38500";
	show_style = VE_FULL_DISSOLVE;
}

Rm38500::~Rm38500()
{
}

void Rm38500::init()
{
  	drawPic( 1 );
    Room::init();  
	

  	intrface->show();
  	theGame->handsOn();
}

int Rm38500::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}
 
} // namespace Spycraft 
