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
#include "spycraft/game/views.h"
#include "spycraft/logic/68000.h"
#include "spycraft/logic/1000.h"
#include "spycraft/logic/69000.h"
#include "spycraft/game/flag.h"

namespace Spycraft {

/*********************
*
*		Room Code 
*
**********************/

extern Flag GameFlag;
extern Intrface* intrface;
extern SouthCursor* southCursor;
//extern NorthCursor* northCursor;
extern NorthwestCursor* northwestCursor;
//extern Print* testPrint;

Rm68000::Rm68000()
{
	name = "Rm68000";
	show_style = VE_FLIP;
}

Rm68000::~Rm68000()
{
//   testPrint->erase();
}

void Rm68000::init()
{
  	drawPic( 68000 );
    Room::init();

  	new(SouthExit68000);
	new(NorthExit68000);
//	new(DisguiseKit68000);
//	new(Closet68000);
//	new(Suitcase68000);

  	intrface->show();
  	theGame->handsOn();
	
//	testPrint = new(Print); 
//	testPrint->init("Hotel Schlump Cluster.");
}

int Rm68000::handleEvent ( MADEEventStamp *event )
{
	return false;
}

SouthExit68000::SouthExit68000()
{
	name = "SouthExit68000";
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
    setHotspot ( DO_V );
	//init (46, 460, 495, 20);
}

void SouthExit68000::respond()
{
	theGame->setCursor(southCursor);
}

int SouthExit68000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm1000);
    	return true;
	}
	return false;
}

NorthExit68000::NorthExit68000()
{
	name = "NorthExit68000";
//	init(NORTH_EXIT_X, NORTH_EXIT_Y, NORTH_EXIT_WIDTH, NORTH_EXIT_HEIGHT);
	init(124, 97, 225, 240);
    setHotspot ( DO_V );
}

void NorthExit68000::respond()
{
//	theGame->setCursor(northCursor);
	theGame->setCursor(northwestCursor);
}

int NorthExit68000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm69000);
    	return true;
	}
	return false;
}


/*******************************
 *
 *	Views
 *
 *******************************/
/*
DisguiseKit68000::DisguiseKit68000()
{
	name = "Trunk";
	setHotspot ( DO_V );
	setView(5);
	init ( 100, 100, true);
}

void DisguiseKit68000::respond()
{
}

int DisguiseKit68000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	if(GameFlag.test(fDisguiseKitOpen))
		{
			GameFlag.clear(fDisguiseKitOpen);
			sfxPrintf("Disguise kit closed");
		}
		else
		{
			sfxPrintf("Disguise kit opened");
			GameFlag.set(fDisguiseKitOpen);
			GameFlag.set(fDisguiseKitFound);
		}			
    	return true;
	}
	return false;
}


Suitcase68000::Suitcase68000()
{
	name = "Suitcase";
	setHotspot ( DO_V );
	setView(5);
	init ( 250, 150, true);
}

void Suitcase68000::respond()
{
}

int Suitcase68000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	sfxPrintf("show close up of open suitcase");
    	new(SuitcaseInset);
	}
	return false;
}

Closet68000::Closet68000()
{
	name = "Closet";
	setHotspot ( DO_V );
	setView(5);
	init ( 250, 320, true);
}

void Closet68000::respond()
{
}

int Closet68000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	new(ClosetInset);
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
/*
SuitcaseInset::SuitcaseInset()
{
	init(68050, 0, 0, 100, 100);
	//create any features that are in the inset
	new(Boxers68000);
	new(Socks68000);
}

void SuitcaseInset::respond()
{
}

int SuitcaseInset::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		sfxPrintf("Inset do verb");
    	return true;
	}
	return false;
}

ClosetInset::ClosetInset()
{
	init(68150, 0, 0, 300, 300);
	//create any features that are in the inset
	new(Handcuffs68000);
	new(Casuals68000);
	new(Suit68000);
}

void ClosetInset::respond()
{
}

int ClosetInset::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	sfxPrintf("doVerb for Closet inset");
    	return true;
	}
	return false;
}
*/
/*******************************
 *
 *	Inset Features
 *
 *******************************/
/*
Boxers68000::Boxers68000()
{
	name = "Boxers";
//	setHotspot(DO_V);
	init(0, 0, 50, 50);
}

void Boxers68000::respond()
{
}

int Boxers68000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		sfxPrintf("move boxers");
	}
	return false;
}

Socks68000::Socks68000()
{
	name = "Socks";
//	setHotspot(DO_V);
	init(50, 50, 50, 50);
}

void Socks68000::respond()
{
}

int Socks68000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		sfxPrintf("Get Socks");
	}
	return false;
}

Handcuffs68000::Handcuffs68000()
{
	name = "Handcuffs";
//	setHotspot(DO_V);
	init(50, 300, 50, 50);
}

void Handcuffs68000::respond()
{
}

int Handcuffs68000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		sfxPrintf("Handcuffs are in inventory");
		GameFlag.set(fGotHandcuffs);
//		sfxPrintf("Onyx' Handcuffs (00384/????) ...really must protest..");
	}
	return false;
}

Casuals68000::Casuals68000()
{
	name = "Casuals";
//	setHotspot(DO_V);
	init(250, 200, 50, 50);
}

void Casuals68000::respond()
{
}

int Casuals68000::doVerb ( int theVerb )
{
	return false;
}

Suit68000::Suit68000()
{
	name = "Pinstripe suit";
//	setHotspot(DO_V);
	init(0, 10, 50, 50);
}

void Suit68000::respond()
{
}

int Suit68000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		if(!GameFlag.test(fMovedSuit)){
			sfxPrintf("suit moves");
			new(Bonds68000);
			GameFlag.set(fMovedSuit);
		}
	}
	return false;
}

Bonds68000::Bonds68000()
{
	name = "Bearer Bonds";
//	setHotspot(DO_V);
	init(250, 0, 50, 50);
}

void Bonds68000::respond()
{
}

int Bonds68000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		sfxPrintf("Player gets bonds");
		this->dispose;
	}
	return false;
}
*/
/*******************************
 *
 *	Scripts
 *
 *******************************/
/*
void Script68000::changeState ( int newState )
{
	switchTo
		seconds = 2;
	END
   
    BEG
		sfxPrintf("Introduction Movie 00360"); //pat interactive
//		GameFlag.set(fOnyxCalled);
		seconds = 2;			
    END

    BEG
		if(!GameFlag.test(fBeenTo68000))
		{
			sfxPrintf("Onyx Greeting (00362/????) who are you");
			GameFlag.set(fBeenTo68000);
		}
		seconds = 2;			
    END

    BEG
		sfxPrintf("Talk about pit sale"); //pat interactive
		dispose();
    END

}
*/
 
} // namespace Spycraft 
