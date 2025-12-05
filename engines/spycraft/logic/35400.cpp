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
#include "spycraft/logic/35000.h"
#include "spycraft/logic/35400.h"
#include "spycraft/game/flag.h"
#include "spycraft/game/vlink.h"
#include "invent.h"
#include "spycraft/game/sound.h"

namespace Spycraft {

#define CRSBLINK_SPEED 5

/*********************
*
*		Room Code 
*
**********************/

extern Intrface* intrface;
extern BlinkCursor* normalCursor;
extern SouthCursor* southCursor;
extern EastCursor* eastCursor;
extern Sound* sound2;

BlankPage35600* blankpage = 0;
PageOne35600* pageone = 0;
PageTwo35600* pagetwo = 0;
PageThree35600* pagethree = 0;
PageFour35600* pagefour = 0;
PageFive35600* pagefive = 0;
PageSix35600* pagesix = 0;

Sound* snd35400;
Sound* snd35500;
Sound* snd35600;
Sound* snd35701;

Rm35400::Rm35400()
{
	name = "Rm35400";
	show_style = VE_FLIP;
}

Rm35400::~Rm35400()
{
	snd35400->dispose();
//	sound2->stop();
	GameFlag.clear(fWahKa78);
}

void Rm35400::init()
{
  	drawPic( 35160 );
    Room::init();

	snd35400 = new Sound;
  
	new(SouthExit35400);
	new(EastExit35400);
	new(ClosedFile35400);
	
  	intrface->show();
  	theGame->handsOn();
}

void Rm35400::doit()
{
//	if((GameFlag.test(fYORoomEnter) && !GameFlag.test(fVCRoomEnter) && !GameFlag.test(fWahKa51)) ||		//Yuri about VC
//	if((GameFlag.test(fVCRoomEnter) && GameFlag.test(fDoneSoundAnal) && !GameFlag.test(fWahKa52)) ||	//Lean
	if((GameFlag.test(fOnyxHunt1) && !GameFlag.test(fWahKa53) && !GameFlag.test(fPlayerTurnsProcat) && GameFlag.test(fMetBlake)) ||					//Yuri about Onyx
	   (GameFlag.test(fBrokenYingInterrogation) && GameFlag.test(fOnyxHunt2) && !GameFlag.test(fKilledGrendel) && !GameFlag.test(fWahKa38) && !GameFlag.test(fWahKa54))) {  	//Harmonica
		if(!GameFlag.test(fWahKa78) && (snd35400->number != 35127 || snd35400->isPlaying()) ) {
			snd35400->stop();
			if( snd35400->number != 35127  || !snd35400->isPlaying() )	{
				GameFlag.set(fWahKa78);
				snd35400->loop = -1; 
				snd35400->play(35127, 0);			//phone ringing
////			GameFlag.clear(fWahKa78);
			}
		}
	}
	Room::doit();
}

int Rm35400::handleEvent ( MADEEventStamp *event )
{
	return false;
}

SouthExit35400::SouthExit35400()
{
	name = "SouthExit35400";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
}

void SouthExit35400::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit35400::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm35000);
    	return true;
	}
	return false;
}

EastExit35400::EastExit35400()
{
	name = "EastExit35400";
	setHotspot ( DO_V );
	init(EAST_EXIT_X, EAST_EXIT_Y, EAST_EXIT_WIDTH, EAST_EXIT_HEIGHT);
}

void EastExit35400::respond()
{
	theGame->setCursor(eastCursor);
	aniCursor();
}

int EastExit35400::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm35100);
    	return true;
	}
	return false;
}

/*******************************
 *
 *	Features
 *
/*******************************/

ClosedFile35400::ClosedFile35400()
{
	name = "ClosedFile35400";
	setHotspot( DO_V );
	init( 66, 144, 371, 196 );
}

void ClosedFile35400::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int ClosedFile35400::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
//		curRoom->setScript(new OpenFileScript35400);
//		new FileCabinetInset35001;
		theGame->handsOff();
		sfxLoadRes(35170, RES_PIC);

		if(!GameFlag.test(fWahKa76)) {
			sound2->stop();
			if( sound2->number != 35121  || !sound2->isPlaying()) {
				GameFlag.set(fWahKa76);
				sound2->number = 35121; 
				sound2->play();
				GameFlag.clear(fWahKa76);
			}
		}
		
		theGame->newRoom( new Rm35500);
		return true;
	}
	return false;
}

OpenFileScript35400::OpenFileScript35400()
{
	name = "OpenFileScript35400"; 
}

void OpenFileScript35400::changeState( int newState )
{	   
	switchTo
//		snd35400->number = 35121; 
//		snd35400->play();
	END
}

/* Room 35500 */
Rm35500::Rm35500()
{
	name = "Rm35500";
	show_style = VE_FLIP;
}

Rm35500::~Rm35500()
{
	snd35500->dispose();
	GameFlag.clear(fWahKa78);
}

void Rm35500::init()
{
  	drawPic( 35170 );
    Room::init();
  	
  	snd35500 = new Sound;

//	new(SouthExit35500);
	new(FileHandle35500);
	new(Hacker35500);
	new(Folder35500);
	
  	intrface->show();
  	theGame->handsOn();
}

void Rm35500::doit()
{
//	if((GameFlag.test(fYORoomEnter) && !GameFlag.test(fVCRoomEnter) && !GameFlag.test(fWahKa51)) ||		//Yuri about VC
//	if((GameFlag.test(fVCRoomEnter) && GameFlag.test(fDoneSoundAnal) && !GameFlag.test(fWahKa52)) ||	//Lean
	if((GameFlag.test(fOnyxHunt1) && !GameFlag.test(fWahKa53) && !GameFlag.test(fPlayerTurnsProcat) && GameFlag.test(fMetBlake)) ||					//Yuri about Onyx
	   (GameFlag.test(fBrokenYingInterrogation) && GameFlag.test(fOnyxHunt2) && !GameFlag.test(fKilledGrendel) && !GameFlag.test(fWahKa38) && !GameFlag.test(fWahKa54))) {  	//Harmonica
		if(!GameFlag.test(fWahKa78) && (snd35500->number != 35127 || snd35500->isPlaying()) ) {
			snd35500->stop();
			if( snd35500->number != 35127  || !snd35500->isPlaying() )	{
				GameFlag.set(fWahKa78);
				snd35500->loop = -1; 
				snd35500->play(35127, 0);			//phone ringing
////			GameFlag.clear(fWahKa78);
			}
		}
	}
	Room::doit();
}

int Rm35500::handleEvent ( MADEEventStamp *event )
{
	return false;
}
/*	  
SouthExit35500::SouthExit35500()
{
	name = "SouthExit35500";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
}

void SouthExit35500::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit35500::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
//		curRoom->setScript(new OpenFileScript35400);
		theGame->handsOff();
		sfxLoadRes(35160, RES_PIC);

		if( sound2->number != 35012  || !sound2->isPlaying()) {
			sound2->number = 35012; 
			sound2->play();
		}
    	theGame->newRoom(new Rm35400);
    	return true;
	}
	return false;
}  
*/
FileHandle35500::FileHandle35500()
{
	name = "FileHandle35500";
	setHotspot ( DO_V );
////init(87, 238, 290, 121);
	init(87, 238, 290, 161);
}

void FileHandle35500::respond()
{
	theGame->setCursor(normalCursor);
	normalCursor->blink( CRSBLINK_SPEED );
}

int FileHandle35500::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		theGame->handsOff();
		sfxLoadRes(35160, RES_PIC);

		if(!GameFlag.test(fWahKa77)) {
			sound2->stop();
			if( sound2->number != 35012  || !sound2->isPlaying()) {
				GameFlag.set(fWahKa77);
				sound2->number = 35012; 
				sound2->play();
				GameFlag.clear(fWahKa77);
			}
		}
    	theGame->newRoom(new Rm35400);
    	return true;
	}
	return false;
} 

Hacker35500::Hacker35500()
{
	name = "Hacker35500";
	setHotspot ( DO_V );
//	init(123, 189, 250, 50);
	init(174, 196, 123, 225);
}

void Hacker35500::respond()
{
	theGame->setCursor(normalCursor);
	normalCursor->blink( CRSBLINK_SPEED );
}

int Hacker35500::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
//		inventry->get(iHACKERDISK);
//		new HackerInset35520;
		theGame->newRoom(new Rm35701);
    	return true;
	}
	return false;
}  

HackerInset35520::HackerInset35520()
{
	normalCursor->rest();
 	init(35520, 0, 15, 512, 385);	
}

Folder35500::Folder35500()
{
	name = "Folder35500";
	setHotspot ( DO_V );
	init(185, 166, 114, 25);
}

void Folder35500::respond()
{
	theGame->setCursor(normalCursor);
	normalCursor->blink( CRSBLINK_SPEED );
}

int Folder35500::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		theGame->newRoom(new Rm35600);
    	return true;
	}
	return false;
}

/* Room 35600 */
Rm35600::Rm35600()
{
	name = "Rm35600";
	show_style = VE_FLIP;
}

Rm35600::~Rm35600()
{
	snd35600->dispose();
	GameFlag.clear(fWahKa78);
}

void Rm35600::init()
{
  	drawPic( 35600 );
    Room::init();
  
	snd35600 = new Sound;

 	new(SouthExit35600);
	new(LeftPage35600);
	new(RightPage35600);
	

	if(!GameFlag.test(fWahKa49)) {
		pageone = new(PageOne35600);
	}
	else if(GameFlag.test(fWahKa45)) {
		blankpage = new(BlankPage35600);
		pagetwo = new(PageTwo35600);
	}
	else if(GameFlag.test(fWahKa46)) {
		blankpage = new(BlankPage35600);
		pagethree = new(PageThree35600);
	}
	else if(GameFlag.test(fWahKa47)) {
		blankpage = new(BlankPage35600);
		pagefour = new(PageFour35600);
	}
	else if(GameFlag.test(fWahKa48)) {
		blankpage = new(BlankPage35600);
		pagefive = new(PageFive35600);
	}
	else if(GameFlag.test(fWahKa58)) {
		blankpage = new(BlankPage35600);
		pagesix = new(PageSix35600);
	}

  	intrface->show();
  	theGame->handsOn();
}

void Rm35600::doit()
{
//	if((GameFlag.test(fYORoomEnter) && !GameFlag.test(fVCRoomEnter) && !GameFlag.test(fWahKa51)) ||		//Yuri about VC
//	if((GameFlag.test(fVCRoomEnter) && GameFlag.test(fDoneSoundAnal) && !GameFlag.test(fWahKa52)) ||	//Lean
	if((GameFlag.test(fOnyxHunt1) && !GameFlag.test(fWahKa53) && !GameFlag.test(fPlayerTurnsProcat) && GameFlag.test(fMetBlake)) ||					//Yuri about Onyx
	   (GameFlag.test(fBrokenYingInterrogation) && GameFlag.test(fOnyxHunt2) && !GameFlag.test(fKilledGrendel) && !GameFlag.test(fWahKa38) && !GameFlag.test(fWahKa54))) {  	//Harmonica
		if(!GameFlag.test(fWahKa78) && (snd35600->number != 35127 || snd35600->isPlaying()) ) {
			snd35600->stop();
			if( snd35600->number != 35127  || !snd35600->isPlaying() )	{
				GameFlag.set(fWahKa78);
				snd35600->loop = -1; 
				snd35600->play(35127, 0);			//phone ringing
////			GameFlag.clear(fWahKa78);
			}
		}
	}
	Room::doit();
}

int Rm35600::handleEvent ( MADEEventStamp *event )
{
	return false;
}

SouthExit35600::SouthExit35600()
{
	name = "SouthExit35600";
	setHotspot ( DO_V );
//	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
	init(80, 360, 345, 50);
}

void SouthExit35600::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit35600::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm35500);
    	return true;
	}
	return false;
}    

LeftPage35600::LeftPage35600()
{
	name = "LeftPage35600";
	setHotspot ( DO_V );
	init(19, 335, 60, 65);
//	setView(35610);
//	init(24, 25);
//	noSkip();
}

void LeftPage35600::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink(CRSBLINK_SPEED);
}

int LeftPage35600::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		if(GameFlag.test(fWahKa58)) {
			if(pagesix) {
				pagesix->dispose();
				pagesix = NULL;
			}
			curRoom->setScript( new TurnPageScript35600);
			GameFlag.clear(fWahKa58);
			pagefive = new(PageFive35600);
		}
		else if(GameFlag.test(fWahKa48)) {
			if(pagefive) {
				pagefive->dispose();
				pagefive = NULL;
			}
			curRoom->setScript( new TurnPageScript35600);
			GameFlag.clear(fWahKa48);
			pagefour = new(PageFour35600);
		}
		else if(GameFlag.test(fWahKa47)) {
			if(pagefour) {
				pagefour->dispose();
				pagefour = NULL;
			}
			curRoom->setScript( new TurnPageScript35600);
			GameFlag.clear(fWahKa47);
			pagethree = new(PageThree35600);
		}
		else if(GameFlag.test(fWahKa46)) {
			if(pagethree) {
				pagethree->dispose();
				pagethree = NULL;
			}
			curRoom->setScript( new TurnPageScript35600);
			GameFlag.clear(fWahKa46);
			pagetwo = new(PageTwo35600);
		}
		else if(GameFlag.test(fWahKa45)) {
			if(pagetwo) {
				pagetwo->dispose();
				pagetwo = NULL;
			}
			if(blankpage) {
				blankpage->dispose();
				blankpage = NULL;
			}
			curRoom->setScript( new TurnPageScript35600);
			GameFlag.clear(fWahKa45);
			GameFlag.clear(fWahKa49);
			pageone = new(PageOne35600);
		}
		else {
			theGame->newRoom(new Rm35500);
		}
/*
		else if(GameFlag.test(fWahKa49) && GameFlag.test(fWahKa44)) {
			if(blankpage) {
				blankpage->dispose();
				blankpage = NULL;
			}
			GameFlag.clear(fWahKa44);
			GameFlag.clear(fWahKa49);
			pageone = new(PageOne35600);
		}
		else if(GameFlag.test(fWahKa44)) {
			if(blankpage) {
				blankpage->dispose();
				blankpage = NULL;
			}
			GameFlag.clear(fWahKa49);
//			pageone = new(PageOne35600);
		}
*/
    	return true;
	}
	return false;
}

RightPage35600::RightPage35600()
{
	name = "RightPage35600";
	setHotspot ( DO_V );
	init(455, 335, 56, 69);
//	setView(35620);
//	init(261, 25);
//	noSkip();
}

void RightPage35600::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink(CRSBLINK_SPEED);
}

int RightPage35600::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		if(!GameFlag.test(fWahKa49)) {
			if(pageone) {
				pageone->dispose();
				pageone = NULL;
			}
			curRoom->setScript( new TurnPageScript35600);
			blankpage = new(BlankPage35600);
			pagetwo = new(PageTwo35600);
			GameFlag.clear(fWahKa44);
		}
		else if(GameFlag.test(fWahKa45)) {
			if(pagetwo) {
				pagetwo->dispose();
				pagetwo = NULL;
			}
			curRoom->setScript( new TurnPageScript35600); 
			GameFlag.clear(fWahKa45);
			pagethree = new(PageThree35600);
		}
		else if(GameFlag.test(fWahKa46)) {
			if(pagethree) {
				pagethree->dispose();
				pagethree = NULL;
			}
			curRoom->setScript( new TurnPageScript35600); 
			GameFlag.clear(fWahKa46);
			pagefour = new(PageFour35600);
		}
		else if(GameFlag.test(fWahKa47)) {
			if(pagefour) {
				pagefour->dispose();
				pagefour = NULL;
			}
			curRoom->setScript( new TurnPageScript35600); 
			GameFlag.clear(fWahKa47);
			pagefive = new(PageFive35600);
		}
		else if(GameFlag.test(fWahKa48)) {
			if(pagefive) {
				pagefive->dispose();
				pagefive = NULL;
			}
			curRoom->setScript( new TurnPageScript35600); 
			GameFlag.clear(fWahKa48);
			pagesix = new(PageSix35600);
		}
    	return true;
	}
	return false;
}

BlankPage35600::BlankPage35600()
{
	name = "BlankPage35600";
//	setHotspot ( DO_V );
	setView(35610);
	init(24, 25);
	noSkip();
	GameFlag.set(fWahKa49);
}

void BlankPage35600::respond()
{
//	theGame->setCursor( normalCursor );
//	normalCursor->blink(CRSBLINK_SPEED);
}

int BlankPage35600::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	return false;
	}
	return false;
}

PageOne35600::PageOne35600()
{
	name = "PageOne35600";
//	setHotspot ( DO_V );
	setView(35620);
	init(261, 25);
	noSkip();
	GameFlag.set(fWahKa44);
}

void PageOne35600::respond()
{
//	theGame->setCursor( normalCursor );
//	normalCursor->blink(CRSBLINK_SPEED);
}

int PageOne35600::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
//		new(BlankPage35600);
//		new(PageTwo35600);
//		dispose();
    	return false;
	}
	return false;
}

PageTwo35600::PageTwo35600()
{
	name = "PageTwo35600";
//	setHotspot ( DO_V );
	setView(35630);
	init(261, 25);
	noSkip();
	GameFlag.set(fWahKa45);
}

void PageTwo35600::respond()
{
//	theGame->setCursor( normalCursor );
//	normalCursor->blink(CRSBLINK_SPEED);
}

int PageTwo35600::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
//		new(PageThree35600);
//		dispose();
    	return false;
	}
	return false;
}

PageThree35600::PageThree35600()
{
	name = "PageThree35600";
//	setHotspot ( DO_V );
	setView(35640);
	init(261, 25);
	noSkip();
	GameFlag.set(fWahKa46);
}

void PageThree35600::respond()
{
//	theGame->setCursor( normalCursor );
//	normalCursor->blink(CRSBLINK_SPEED);
}

int PageThree35600::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
//		new(PageFour35600);
//		dispose();
    	return false;
	}
	return false;
}

PageFour35600::PageFour35600()
{
	name = "PageFour35600";
//	setHotspot ( DO_V );
	setView(35650);
	init(261, 25);
	noSkip();
	GameFlag.set(fWahKa47);
}

void PageFour35600::respond()
{
//	theGame->setCursor( normalCursor );
//	normalCursor->blink(CRSBLINK_SPEED);
}

int PageFour35600::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
//		new(PageFive35600);
//		dispose();
    	return false;
	}
	return false;
}

PageFive35600::PageFive35600()
{
	name = "PageFive35600";
//	setHotspot ( DO_V );
	setView(35660);
	init(261, 25);
	noSkip();
	GameFlag.set(fWahKa48);
}

void PageFive35600::respond()
{
//	theGame->setCursor( normalCursor );
//	normalCursor->blink(CRSBLINK_SPEED);
}

int PageFive35600::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	return false;
	}
	return false;
}

PageSix35600::PageSix35600()
{
	name = "PageFive35600";
//	setHotspot ( DO_V );
	setView(35670);
	init(261, 25);
	noSkip();
	GameFlag.set(fWahKa58);
	GameFlag.set(fKneeAddressAvailable);
}

void PageSix35600::respond()
{
//	theGame->setCursor( normalCursor );
//	normalCursor->blink(CRSBLINK_SPEED);
}

int PageSix35600::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	return false;
	}
	return false;
}

TurnPageScript35600::TurnPageScript35600()
{
	name = "TurnPageScript35600"; 
}

void TurnPageScript35600::changeState( int newState )
{	   
	switchTo
//		snd35600->play(36023, this);
		if( sound2->number != 36023  || !sound2->isPlaying()) {
			sound2->number = 36023; 
			sound2->play();		
		}
	END
}

Rm35701::Rm35701()
{
	name = "Rm35701";
	show_style = VE_FLIP;
}

Rm35701::~Rm35701()
{
	snd35701->dispose();
	GameFlag.clear(fWahKa78);
}

void Rm35701::init()
{
  	drawPic( 35700 );
    Room::init();

	snd35701 = new Sound;
  
	new(SouthExit35701);
	
	if( GameFlag.test(fShowSwanTwo) && GameFlag.test(fBrokenYingInterrogation) && !GameFlag.test(fWahKa111)) {
		new(GogDisk35701);
	}
		
  	intrface->show();
  	theGame->handsOn();
}

void Rm35701::doit()
{
//	if((GameFlag.test(fYORoomEnter) && !GameFlag.test(fVCRoomEnter) && !GameFlag.test(fWahKa51)) ||		//Yuri about VC
//	if((GameFlag.test(fVCRoomEnter) && GameFlag.test(fDoneSoundAnal) && !GameFlag.test(fWahKa52)) ||	//Lean
	if((GameFlag.test(fOnyxHunt1) && !GameFlag.test(fWahKa53) && !GameFlag.test(fPlayerTurnsProcat) && GameFlag.test(fMetBlake)) ||					//Yuri about Onyx
	   (GameFlag.test(fBrokenYingInterrogation) && GameFlag.test(fOnyxHunt2) && !GameFlag.test(fKilledGrendel) && !GameFlag.test(fWahKa38) && !GameFlag.test(fWahKa54))) {  	//Harmonica
		if(!GameFlag.test(fWahKa78) && (snd35701->number != 35127 || snd35701->isPlaying()) ) {
			snd35701->stop();
			if( snd35701->number != 35127  || !snd35701->isPlaying() )	{
				GameFlag.set(fWahKa78);
				snd35701->loop = -1; 
				snd35701->play(35127, 0);			//phone ringing
////			GameFlag.clear(fWahKa78);
			}
		}
	}
	Room::doit();
}

int Rm35701::handleEvent ( MADEEventStamp *event )
{
	return false;
}

SouthExit35701::SouthExit35701()
{
	name = "SouthExit35701";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
}

void SouthExit35701::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit35701::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm35500);
    	return true;
	}
	return false;
}

GogDisk35701::GogDisk35701()
{
	name = "GogDisk35701";
	setHotspot ( DO_V );
	setView(35720);
	init ( 82, 94 );
	noSkip();
}

void GogDisk35701::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink(CRSBLINK_SPEED);
}

int GogDisk35701::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		GameFlag.set(fWahKa111);
		if( sound2->number != 50011  || !sound2->isPlaying()) {
			sound2->number = 50011;
			sound2->play();
		}
		inventry->get(iHACKERDISK);
		inventry->items[iHACKERDISK]->activate();
		new GogDiskInset35701();	
		dispose();
    	return true;
	}
	return false;
}

GogDiskInset35701::GogDiskInset35701()
{
	normalCursor->rest();
 	init(980, 0, 15, 512, 385);	
}
 
} // namespace Spycraft 
