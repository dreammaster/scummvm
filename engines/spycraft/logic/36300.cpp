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
#include "spycraft/logic/36300.h"
#include "spycraft/logic/36200.h"
#include "spycraft/logic/36100.h"
#include "spycraft/game/sound.h"

namespace Spycraft {

#define CRSBLINK_SPEED 5

extern Intrface* intrface;
extern SouthCursor* southCursor;
extern BlinkCursor* normalCursor;
extern Sound* sound2;

/*********************
*
*	Room 
*
**********************/

Rm36300::Rm36300()
{
	name = "Rm36300";
	show_style = VE_FLIP;
}

Rm36300::~Rm36300()
{
}

void Rm36300::init()
{
	GameFlag.set(fWahKa80);

	drawPic( 36300 );
	Room::init();

	new SouthExit36300;
	new	Drawer36300;
	
	intrface->show();
	theGame->handsOn();
}

int Rm36300::handleEvent ( MADEEventStamp *event )
{
	return false;
}

/*********************
*
*	Exit Features 
*
**********************/

SouthExit36300::SouthExit36300()
{
	name = "SouthExit36300";
	setHotspot( DO_V );
	init( SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT );  
}

void SouthExit36300::respond()
{
	theGame->setCursor( southCursor );
	aniCursor();
}

int SouthExit36300::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
    	GameFlag.clear(fWahKa80);
    	theGame->newRoom( new Rm36100 );
    	return true;
	}
	return false;
}

/*******************************
 *
 *	Features
 *
/*******************************/

Drawer36300::Drawer36300()
{
	name = "Drawer36300";
	setHotspot( DO_V );
	init( 116, 125, 322, 250 );
}

void Drawer36300::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Drawer36300::doVerb( int theVerb )
{
/*	if( theVerb == DO_V || 1 )	{
		if( !GameFlag.test( fFosterDeskDrawerUnlocked ))	 	// drawer unlocked
			theGame->newRoom( new Rm36200 );
//		else if( !playerHasAllFolders( inventry )) {				// any folders still in drawer?
		else if(!GameFlag.test(fWahKa64)) {
			curRoom->setScript(new OpenFileScript35400);
			new DrawerWFolderInset36300;
		}
		else													// drawer is empty
			new EmptyDrawerInset36300;
		theGame->newRoom( new Rm36301 );
		return true;
	}
	return false;
*/
	if( theVerb == DO_V )	{
		theGame->newRoom( new Rm36301 );
		return true;
	}
	return false;
}
/*
int	Drawer36300::playerHasAllFolders( Invent *inventory )
{
	return 	inventory->get(iFOLDER_PAYMENTS) &&
		inventory->get(iFOLDER_ORDERS) &&
		inventory->get(iFOLDER_CITATION) &&
		inventory->get(iDOSSIER_ZHIRONEV) &&
		inventory->get(iDOSSIER_SHUSTAK);
}
*/
/*******************************
 *
 *	Insets
 *
/*******************************/
/*
DrawerWFolderInset36300::DrawerWFolderInset36300()
{
	normalCursor->rest();	// stops respond blink
	init( 36320, 0, 15, 512, 384 );
	GameFlag.set(fWahKa64);
	new FolderInsetFeature36300;
}

EmptyDrawerInset36300::EmptyDrawerInset36300()
{
	normalCursor->rest();	// stops respond blink
	init( 36340, 0, 16, 512, 384 );

	if( !inventry->has( iFOLDER_PAYMENTS ) )
		new	PaymentsInsetFeature36300;
	if( !inventry->has( iFOLDER_ORDERS ) )
		new	OrdersInsetFeature36300;
	if( !inventry->has( iFOLDER_CITATION ) )
		new	CitationInsetFeature36300;
	if( !inventry->has( iDOSSIER_ZHIRONEV ) )
		new	ZhironevInsetFeature36300;
	if( !inventry->has( iDOSSIER_SHUSTAK ) )
		new	ShustakInsetFeature36300;
}
*/

/*******************************
*
*	Inset Features
*
*******************************/
/*
FolderInsetFeature36300::FolderInsetFeature36300()
{
	name = "FolderInsetFeature36300";
	setHotspot( DO_V );
	init(200, 200, 150, 150);
}

void FolderInsetFeature36300::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int FolderInsetFeature36300::doVerb ( int theVerb )
{
	if ( theVerb == DO_V || 1) {
		theInset->dispose();
//		sfxPrintf("opened folder");
		new EmptyDrawerInset36300;
		return true;
	}
	return false;
}

PaymentsInsetFeature36300::PaymentsInsetFeature36300()
{
	name = "Payments folder";
	setHotspot( DO_V );
	init(50, 50, 150, 40);
}

void PaymentsInsetFeature36300::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int PaymentsInsetFeature36300::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		inventry->get(iFOLDER_PAYMENTS);
//		sfxPrintf("got payments folder");
		dispose();
	}
	return false;
}

OrdersInsetFeature36300::OrdersInsetFeature36300()
{
	name = "Orders folder";
	setHotspot( DO_V );
	init(50, 100, 150, 40);
}

void OrdersInsetFeature36300::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int OrdersInsetFeature36300::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		inventry->get(iFOLDER_ORDERS);
//		sfxPrintf("got orders folder");
		dispose();
	}
	return false;
}

CitationInsetFeature36300::CitationInsetFeature36300()
{
	name = "Citation folder";
	setHotspot( DO_V );
	init(50, 150, 150, 40);
}

void CitationInsetFeature36300::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int CitationInsetFeature36300::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		inventry->get(iFOLDER_CITATION);
//		sfxPrintf("got citation folder");
		dispose();
	}
	return false;
}

ZhironevInsetFeature36300::ZhironevInsetFeature36300()
{
	name = "Zhironev Dossier";
	setHotspot( DO_V );
	init(50, 200, 150, 40);
}

void ZhironevInsetFeature36300::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int ZhironevInsetFeature36300::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		inventry->get(iDOSSIER_ZHIRONEV);
//		sfxPrintf("got Zhironev Dossier");
		dispose();
	}
	return false;
}

ShustakInsetFeature36300::ShustakInsetFeature36300()
{
	name = "Shustak Dossier";
	setHotspot( DO_V );
	init(50, 250, 150, 40);
}

void ShustakInsetFeature36300::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int ShustakInsetFeature36300::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		inventry->get(iDOSSIER_SHUSTAK);
//		sfxPrintf("got Shustak Dossier");
		dispose();
	}
	return false;
}
*/
/*********************
*
*		Room Code 
*
**********************/

Rm36301::Rm36301()
{
	name = "Rm36301";
	show_style = VE_FLIP;
}

Rm36301::~Rm36301()
{
}

void Rm36301::init()
{
    Room::init();
  
  	intrface->show();
  	theGame->handsOn();

	new SouthExit36301;

	if( !GameFlag.test( fFosterDeskDrawerUnlocked ))	 	
		theGame->newRoom( new Rm36200 );							//close up shot of the drawer
	else if(GameFlag.test(fWahKa64) && !GameFlag.test(fWahKa79)) {	//open drawer
//		curRoom->setScript(new OpenFileScript35400);
		theGame->handsOff();
		sfxLoadRes(36320, RES_PIC);
		if(!GameFlag.test(fWahKa90)) {
			sound2->stop();			
			if( sound2->number != 35121  || !sound2->isPlaying()) {
				GameFlag.set(fWahKa90);
				sound2->number = 35121; 
				sound2->play();
				GameFlag.clear(fWahKa90);
			}
		}
		theGame->newRoom( new Rm36302 );
	}
	else if(GameFlag.test(fWahKa64)) {								//have file, open drawer again
//		curRoom->setScript(new OpenFileScript35400);
		theGame->handsOff();
		sfxLoadRes(36320, RES_PIC);
		if(!GameFlag.test(fWahKa91)) {
			sound2->stop();	
			if( sound2->number != 35121  || !sound2->isPlaying()) {
				GameFlag.set(fWahKa91);
				sound2->number = 35121; 
				sound2->play();
				GameFlag.clear(fWahKa91);
			}
		}
		theGame->newRoom( new Rm36303 );	
	}
//	else															//drawer is empty
//		new EmptyDrawerInset36300;
//		theGame->newRoom( new Rm36303 );
}

int Rm36301::handleEvent ( MADEEventStamp *event )
{
	return false;
}

SouthExit36301::SouthExit36301()
{
	name = "SouthExit36301";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
}

void SouthExit36301::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit36301::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm36100);
    	return true;
	}
	return false;
}

Rm36302::Rm36302()
{
	name = "Rm36302";
	show_style = VE_FLIP;
}

Rm36302::~Rm36302()
{
}

void Rm36302::init()
{
	drawPic(36320);
    Room::init();
  
  	intrface->show();
  	theGame->handsOn();

	new OpenDrawer36300;
	new SouthExit36302;
}

int Rm36302::handleEvent ( MADEEventStamp *event )
{
	return false;
}

SouthExit36302::SouthExit36302()
{
	name = "SouthExit36302";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
}

void SouthExit36302::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit36302::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		theGame->handsOff();
		sfxLoadRes(36300, RES_PIC);
		if(!GameFlag.test(fWahKa93)) {
			sound2->stop();	
			if( sound2->number != 35012  || !sound2->isPlaying()) {
				GameFlag.set(fWahKa93);
				sound2->number = 35012; 
				sound2->play();
				GameFlag.clear(fWahKa93);
			}
		}
    	theGame->newRoom(new Rm36300);
    	return true;
	}
	return false;
}

OpenDrawer36300::OpenDrawer36300()
{
	name = "OpenDrawer36300";
	setHotspot( DO_V );
	init( 112, 47, 299, 317 );
}

void OpenDrawer36300::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int OpenDrawer36300::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		GameFlag.set(fWahKa79);
		if( sound2->number != 36022  || !sound2->isPlaying()) {
			sound2->number = 36022;
			sound2->play();
		}
		inventry->get(iFOLDER_PAYMENTS);
		inventry->get(iFOLDER_ORDERS);
		inventry->get(iFOLDER_CITATION);
//		inventry->get(iDOSSIER_ZHIRONEV);
//		inventry->get(iDOSSIER_SHUSTAK);

//		new EmptyDrawerInset36300;
		theGame->newRoom(new Rm36303);
		return true;
	}
	return false;
}

Rm36303::Rm36303()
{
	name = "Rm36303";
	show_style = VE_FLIP;
}

Rm36303::~Rm36303()
{
}

void Rm36303::init()
{
    drawPic(36340);
    Room::init();
  
  	intrface->show();
  	theGame->handsOn();

	new SouthExit36303;
}

int Rm36303::handleEvent ( MADEEventStamp *event )
{
	return false;
}

SouthExit36303::SouthExit36303()
{
	name = "SouthExit36303";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
}

void SouthExit36303::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit36303::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		theGame->handsOff();
		sfxLoadRes(36300, RES_PIC);
		if(!GameFlag.test(fWahKa92)) {
			sound2->stop();	
			if( sound2->number != 35012  || !sound2->isPlaying()) {
				GameFlag.set(fWahKa92);
				sound2->number = 35012; 
				sound2->play();
				GameFlag.clear(fWahKa92);
			}
		}

//    	theGame->newRoom(new Rm36301);
		theGame->newRoom(new Rm36300);
    	return true;
	}
	return false;
}
 
} // namespace Spycraft 
