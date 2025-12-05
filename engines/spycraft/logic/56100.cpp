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
#include "56100.h"
#include "57000.h"
#include "1000.h"
#include "invent.h"
#include "sound.h"

namespace Spycraft {

#define CRSBLINK_SPEED 5

extern Intrface* intrface;
extern NorthCursor* northCursor;
extern SouthCursor* southCursor;
extern EastCursor* eastCursor;
extern WestCursor* westCursor;
extern BlinkCursor* normalCursor;
//extern DoorCursor* doorCursor;

Sound* snd56100;

/*********************
*
*	Room 
*
**********************/
Rm56100::Rm56100()
{
	name = "Rm56100";
	show_style = VE_FLIP;
}

Rm56100::~Rm56100()
{
	snd56100->dispose();
}

void Rm56100::init()
{
	drawPic( 56100 );
	Room::init();

	intrface->show();
	theGame->handsOn();	
	
	new(SouthExit56100);
//	new(NorthExit56100);
	
	snd56100 = new Sound;

	if( !GameFlag.test( fMetBirdSong) )
		GameFlag.set( fBSDoorUnlocked);
	else
		GameFlag.clear( fBSDoorUnlocked );


	if( !GameFlag.test( fBSDoorUnlocked )) {
		new Doorknob56100;
		new KnockDoor56100;
//		sfxLoadRes ( 56120, RES_ATS );
	}
//	else {
//		new Door56100;
//	}
}

int Rm56100::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}

/*********************
*
*	Exit Features 
*
**********************/
/*
NorthExit56100::NorthExit56100()
{
	name = "NorthExit56100";
	setHotspot( DO_V );
//	init(NORTH_EXIT_X, NORTH_EXIT_Y, NORTH_EXIT_WIDTH, NORTH_EXIT_HEIGHT);  
//	init(206, 60, 205, 338);
}

void NorthExit56100::respond()
{
	if( GameFlag.test( fBSDoorUnlocked )) {
		theGame->setCursor( northCursor );
		aniCursor();
	}
}

int NorthExit56100::doVerb ( int theVerb )
{
	if( theVerb == DO_V || theVerb == LOCKPICK_V ) {
		if( GameFlag.test( fBSDoorUnlocked ))
//			setScript( new DoorScript56100 );       //DEBUG wahka
//			theGame->newRoom( new Rm56200 );		//try this
    	return TRUE;
	}
	return FALSE;
}
*/
SouthExit56100::SouthExit56100()
{
	name = "SouthExit56100";
	setHotspot( DO_V );
	init( SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT );  
}

void SouthExit56100::respond()
{
	theGame->setCursor( southCursor );
	aniCursor();
}

int SouthExit56100::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
    	theGame->newRoom( new Rm1000 );
    	return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Features
 *
/*******************************/

Doorknob56100::Doorknob56100()
{
	name = "Doorknob56100";
	setHotspot( DO_V );
//	init(321, 336, 11, 67);
	init(300, 275, 71, 61);
}									  

void Doorknob56100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Doorknob56100::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		inventry->clearCurrentItem();
		theGame->newRoom( new Rm56101 );	
////	new DoorknobInset56100;
		return TRUE;
	}
//	else if( theVerb == GUN_V ) {
//		GameFlag.set( fBSDoorUnlocked ); //bkdr
//		return TRUE;
//	}
	return FALSE;
}

KnockDoor56100::KnockDoor56100()
{
	name = "KnockDoor56100";
	setHotspot( DO_V );
	init(162, 72, 186, 174);
}									  

void KnockDoor56100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int KnockDoor56100::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
		if(!GameFlag.test(fWahKa71) && (snd56100->number != 56100 || snd56100->isPlaying()) ) {
		snd56100->stop();
			if(snd56100->number != 56100 || snd56100->isPlaying()) {
				GameFlag.set(fWahKa71);
				snd56100->number = 56100; 		 //knock knock
				snd56100->play();
				GameFlag.clear(fWahKa71);
			}
		}
		return TRUE;
	}
//	else if( theVerb == GUN_V ) {
//		GameFlag.set( fBSDoorUnlocked ); //bkdr
//		return TRUE;
//	}
	return FALSE;
}

/*
Door56100::Door56100()
{
	name = "Door56100";
	setHotspot( DO_V );
	init(142, 60, 205, 338);
}

void Door56100::respond()
{
//	if( GameFlag.test( fBSDoorUnlocked ))
//		theGame->setCursor( doorCursor );
}

int Door56100::doVerb( int theVerb )
{
	if( theVerb == DO_V || theVerb == LOCKPICK_V ) {
		if( GameFlag.test( fBSDoorUnlocked ))
//			setScript( new DoorScript56100 );
//			theGame->newRoom( new Rm56200 );		   
			theGame->newRoom( new Rm57000 );
    	return TRUE;
	}
	return FALSE;
}
*/
/*******************************
 *
 *	Insets
 *
/*******************************/

DoorknobInset56100::DoorknobInset56100()
{
	normalCursor->rest();	// stops respond blink
	init( 56120, 0, 16, 512, 384 );
	new LockInsetFeature56100;
}

void DoorknobInset56100::dispose()
{	
//	new Door56100;
	Inset::dispose();
}

/*******************************
 *
 *	Inset Features
 *
/*******************************/

LockInsetFeature56100::LockInsetFeature56100()
{
	name = "LockInsetFeature56100";
	init(211, 205, 55, 90);
	setHotspot( DO_V );
	hotspot->addRespondVerb(LOCKPICK_V);
	
}

void LockInsetFeature56100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int LockInsetFeature56100::doVerb ( int theVerb )
{
	if ( invVerb == LOCKPICK_V ) {
//		sfxPrintf( "Click" );
		GameFlag.set( fBSDoorUnlocked );
		inventry->clearCurrentItem();
		theGame->handsOff();
		curRoom->setScript(new PickLockScript56100);
		return TRUE;
	}
	else if ( theVerb == DO_V ) {
		return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Scripts
 *
/*******************************/
/*
void DoorScript56100::DoorScript56100()
{
	name = "DoorScript56100";
}

void DoorScript56100::changeState( int newState )
{
	switchTo
		theGame->handsOff();
		curRoom->drawPic( 56200 ); // opened door
		sfxPrintf( "-- opened door --" );
		cue();
	END
	BEG
		curRoom->show_style = VE_FULL_DISSOLVE;
		dispose();
		theGame->newRoom( new Rm57000 );
	END
}
*/
PickLockScript56100::PickLockScript56100()
{
	name = "PickLockScript56100"; 
}

void PickLockScript56100::changeState( int newState )
{
	switchTo
		snd56100->play(36011, this);
	END
	BEG
		GameFlag.set(fInvestigateBS);
////	theInset->dispose();
		theGame->newRoom( new Rm57000 );
	END
}

/*********************
*
*		Room Code 
*
**********************/
/*
Rm56200::Rm56200()
{
	name = "Rm56200";
	show_style = VE_FLIP;
}

Rm56200::~Rm56200()
{
}

void Rm56200::init()
{
// 	drawPic( 56200 );	//open door shot
	drawPic( 1 );
    Room::init();
  
  	intrface->show();
  	theGame->handsOn();

	setScript( new RoomEnter56200 );
}

int Rm56200::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}

RoomEnter56200::RoomEnter56200()
{
	name = "RoomEnter56200";
}

void RoomEnter56200::changeState ( int newState )
{
	switchTo
//		theGame->handsOff();
		seconds = 2;
//		__index = 0;
//		changeState ( state ++ );
	END
	BEG
//		curRoom->show_style = VE_FULL_DISSOLVE;
		dispose();
		theGame->newRoom( new Rm57000 );
	END
}
*/
Rm56101::Rm56101()
{
	name = "Rm56101";
	show_style = VE_FLIP;
}

Rm56101::~Rm56101()
{
	snd56100->dispose();
}

void Rm56101::init()
{
	drawPic( 56120 );
	Room::init();

	snd56100 = new Sound;
	
	intrface->show();
	theGame->handsOn();	
	
	new(OpenBSLock56101);
	new(SouthExit56101);
	new(WestExit56101);
	new(EastExit56101);
	
}

int Rm56101::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}

SouthExit56101::SouthExit56101()
{
	name = "SouthExit56101";
	setHotspot( DO_V );
	init( SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT );  
}

void SouthExit56101::respond()
{
	theGame->setCursor( southCursor );
	aniCursor();
}

int SouthExit56101::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
    	theGame->newRoom( new Rm56100 );
    	return TRUE;
	}
	return FALSE;
}

WestExit56101::WestExit56101()
{
	name = "WestExit56101";
	setHotspot( DO_V );
	init( WEST_EXIT_X, WEST_EXIT_Y, WEST_EXIT_WIDTH, WEST_EXIT_HEIGHT );  
}

void WestExit56101::respond()
{
	theGame->setCursor( westCursor );
	aniCursor();
}

int WestExit56101::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
    	theGame->newRoom( new Rm56100 );
    	return TRUE;
	}
	return FALSE;
}

EastExit56101::EastExit56101()
{
	name = "EastExit56101";
	setHotspot( DO_V );
	init( EAST_EXIT_X, EAST_EXIT_Y, EAST_EXIT_WIDTH, EAST_EXIT_HEIGHT );
}

void EastExit56101::respond()
{
	theGame->setCursor( eastCursor );
	aniCursor();
}

int EastExit56101::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
    	theGame->newRoom( new Rm56100 );
    	return TRUE;
	}
	return FALSE;
}

OpenBSLock56101::OpenBSLock56101()
{
	name = "OpenBSLock56101";
	init(211, 205, 55, 90);
	setHotspot( DO_V );
//	hotspot->addRespondVerb(LOCKPICK_V);
	hotspot->addRespondVerb(ALL_V);
}									  

void OpenBSLock56101::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int OpenBSLock56101::doVerb( int theVerb )
{
	if ( invVerb == LOCKPICK_V ) {
//		sfxPrintf( "Click" );
		GameFlag.set( fBSDoorUnlocked );
		inventry->clearCurrentItem();
		theGame->handsOff();
		curRoom->setScript(new PickLockScript56100);
		return TRUE;
	}
	else if ( invVerb != LOCKPICK_V ){
		if( snd56100->number != 36021  || !snd56100->isPlaying()) {
			snd56100->number = 36021;
			snd56100->play();
		}
	}
	else if ( theVerb == DO_V ) {
		return TRUE;
	}

	return FALSE;
}
 
} // namespace Spycraft 
