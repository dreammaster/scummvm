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
#include "spycraft/logic/37000.h"
#include "spycraft/logic/1000.h"
//#include "spycraft/logic/38000.h"
//#include "spycraft/logic/41000.h"
//#include "spycraft/logic/39000.h"
//#include "spycraft/logic/37100.h"
#include "spycraft/logic/41100.h"   //rm41300
#include "spycraft/game/flag.h"
#include "spycraft/game/movie.h"

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
extern NorthCursor* northCursor;

Rm37000::Rm37000()
{
	name = "Rm37000";
	show_style = VE_FLIP;
}

Rm37000::~Rm37000()
{
}

void Rm37000::init()
{
///	drawPic( 37000 );
////drawPic ( 1 );
    Room::init();

  	new(SouthExit37000);
//	new(NorthExit37000);
///	new(PanelVan37000);
///	new(PanelSign37000);
	
  	intrface->show();
// 	theGame->handsOn();

	if (!GameFlag.test(fEnterBadmanVan))
		curRoom->setScript(new EnterBadmanVanScript37000);
	else
		theGame->handsOn();
}

int Rm37000::handleEvent ( MADEEventStamp *event )
{
	return false;
}

SouthExit37000::SouthExit37000()
{
	name = "SouthExit37000";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
}

void SouthExit37000::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit37000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm1000);
    	return true;
	}
	return false;
}
/*
NorthExit37000::NorthExit37000()
{
	name = "NorthExit37000";
	setHotspot ( DO_V );
	init(NORTH_EXIT_X, NORTH_EXIT_Y, NORTH_EXIT_WIDTH, NORTH_EXIT_HEIGHT);
}

void NorthExit37000::respond()
{
	theGame->setCursor(northCursor);
}

int NorthExit37000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm37100);
    	return true;
	}
	return false;
}
*/

/*******************************
 *
 *	Features
 *
/*******************************/

/*
KneeRoof37000::KneeRoof37000()
{
  	name = "KneeRoof";
	init (200, 150, 50, 50);   //bottom left
}

int KneeRoof37000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
   		theGame->newRoom(new Rm38000);
    	return true;
	}
	return false;
}
*/

PanelVan37000::PanelVan37000()
{
  	name = "PanelVan";
	setHotspot( DO_V );
	init (115, 203, 172, 135);   
}

void PanelVan37000::respond()
{
//	theGame->setCursor( normalCursor );
//	normalCursor->blink( CRSBLINK_SPEED );
	theGame->setCursor( northCursor );
}

int PanelVan37000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
//		theGame->newRoom(new Rm41000);
   		theGame->newRoom(new Rm41300);
	
    	return true;
	}
	return false;
}

PanelSign37000::PanelSign37000()
{
  	name = "PanelSign37000";
	setHotspot( DO_V );
	init (310, 213, 54, 43);   
}

void PanelSign37000::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int PanelSign37000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
   		new PanelSignInset37000();
    	return true;
	}
	return false;
}
/*
Tenement37000::Tenement37000()
{
  	name = "Tenement";
	init (400, 150, 50, 50);   //bottom left
}

int Tenement37000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
   		theGame->newRoom(new Rm39000);
    	return true;
	}
	return false;
}
*/
/*******************************
 *
 *	Insets
 *
 *******************************/

PanelSignInset37000::PanelSignInset37000()
{
	normalCursor->rest();
 	init(37020, 0, 15, 512, 385);
}

/*******************************
 *
 *	Scripts
 *
/*******************************/
EnterBadmanVanScript37000::EnterBadmanVanScript37000()
{
	name = "EnterBadmanVanScript37000"; 
}

void EnterBadmanVanScript37000::changeState( int newState )
{
	switchTo
		GameFlag.set(fEnterBadmanVan);
		theGame->handsOff();
		theMovie->play("41501.avi", this, 0, 1, 0);
	END
	BEG
///		theGame->handsOn();
//		dispose();
		theGame->newRoom(new Rm41500);
	END
}
 
} // namespace Spycraft 
