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
#include "spycraft/logic/50000.h"
#include "spycraft/logic/1000.h"
#include "spycraft/game/flag.h"
#include "spycraft/game/movie.h"
#include "spycraft/game/vlink.h"
#include "invent.h"

namespace Spycraft {

#define CRSBLINK_SPEED 5

/*********************
*
*		Room Code 
*
**********************/

extern Intrface* intrface;
extern NorthCursor* northCursor;
extern SouthCursor* southCursor;
extern BlinkCursor* normalCursor;
extern Sound* sound1;
extern Sound* sound2;
extern MADEEnvirons *colbyInfo;

//Sound* snd50001;

Rm50000::Rm50000()
{
	name = "Rm50000";
	show_style = VE_FLIP;
//	show_style = VE_FULL_DISSOLVE;
}

Rm50000::~Rm50000()
{
//	sound1->stop(); 
//	theMusic->stop();
}

void Rm50000::init()
{
    Room::init();

  	new(SouthExit50000);
	new(BloodyCamera50000);
	new(Blanket50000);
	new(Garbage50000);
	new(JunkyWall50000);

  	intrface->show();

	if( !GameFlag.test( fMaxDead )){
//		drawPic( 1 );
		sfxSystemCursor( SYS_WAIT );
		sfxLoadRes ( 50001, RES_PIC );
		setScript( new EnterScript50000 );
	}
	else{
		drawPic( 50000 );		   //wide shot with video camera
		theGame->handsOn();

		if( colbyInfo->slow_system == true )
			theMusic->play ( 50000 );
		else {
			if( sound1->number != 50000  || !sound1->isPlaying() )	{
				sound1->loop = -1; 
				sound1->playBuffered(50000, 0);
			}
		}
	}
}

int Rm50000::handleEvent ( MADEEventStamp *event )
{
	return false;
}

SouthExit50000::SouthExit50000()
{
	name = "SouthExit50000";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
}

void SouthExit50000::respond()
{
	theGame->setCursor(southCursor);
}

int SouthExit50000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		sound1->stop(); 
		theMusic->stop();

////	GameFlag.clear(fDisplayStakout);       		//do not display Stakeout on map
		GameFlag.clear(fCanDisplayStakeOut);       	//do not display Stakeout on map

		if (!GameFlag.test(fPickedUpCamera))
			new VLink( "Check out the camera: Seaton, Jaimie A.", "50002.avi", 0, 10);    //vlink685, Jaimie help about video camer.

    	theGame->newRoom(new Rm1000);
    	return true;
	}
	return false;
}

/*******************************
 *
 *	Features
 *
/*******************************/
BloodyCamera50000::BloodyCamera50000()
{
	name = "BloodyCamera50000";
	setHotspot( DO_V );
	init( 199, 276, 144, 89 );
}

void BloodyCamera50000::respond()
{
	theGame->setCursor( northCursor );
//	normalCursor->blink( CRSBLINK_SPEED );
}

int BloodyCamera50000::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
//		new BloodyCameraInset50000();
		theGame->newRoom(new Rm50001);
		return true;
	}
	return false;
}

Blanket50000::Blanket50000()
{
	name = "Blanket50000";
	setHotspot( DO_V );
	init( 370, 325, 117, 42);
}

void Blanket50000::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Blanket50000::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		new BlanketInset50000();
		return true;
	}
	return false;
}

Garbage50000::Garbage50000()
{
	name = "Garbage50000";
	setHotspot( DO_V );
	init( 321, 245, 158, 65 );
}

void Garbage50000::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Garbage50000::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		new GarbageInset50000();
		return true;
	}
	return false;
}

JunkyWall50000::JunkyWall50000()
{
	name = "JunkyWall50000";
	setHotspot( DO_V );
	init( 427, 107, 74, 85 );
}

void JunkyWall50000::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int JunkyWall50000::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		new JunkyWallInset50000();
		return true;
	}
	return false;
}

/*******************************
 *
 *	Insets
 *
 *******************************/

BloodyCameraInset50000::BloodyCameraInset50000()
{
	normalCursor->rest();
 	init(50020, 0, 15, 512, 385);
}

BlanketInset50000::BlanketInset50000()
{
	normalCursor->rest();
 	init(50040, 0, 15, 512, 385);
}

GarbageInset50000::GarbageInset50000()
{
	normalCursor->rest();
 	init(50060, 0, 15, 512, 385);
}

JunkyWallInset50000::JunkyWallInset50000()
{
	normalCursor->rest();
 	init(50080, 0, 15, 512, 385);
}

/*******************************
 *
 *	Script
 *
 *******************************/

EnterScript50000::EnterScript50000()
{
	name = "EnterScript50000"; 
}

void EnterScript50000::changeState( int newState )
{
	switchTo
///		GameFlag.set(fMaxDead);		   
///		theGame->handsOff();
//		theMovie->fromTo(0, 237);
//		theMovie->play("50000.avi", this, 0, 1, 1);
		theMovie->fromTo(0, 307);
		theMovie->play("50000.avi", this, 0, 1, 0);
	END
//	BEG
//		new VLink( "Stunned: Warhurst, Eugene J.", "50001.avi", 0, 60 );	//vlink684, Warhurst is pissed about Max.
//		theGame->handsOn();
//		theGame->newRoom( new Rm50000 );
//	END
	BEG
		new VLink( "Stunned: Warhurst, Eugene J.", "50001.avi", 0, 60 );	//vlink684, Warhurst is pissed about Max.
		theGame->handsOn();
		theGame->newRoom( new Rm50300 );
	END
}

EnterBlackScreenScript50300::EnterBlackScreenScript50300()
{
	name = "EnterBlackScreenScript50300"; 
}

void EnterBlackScreenScript50300::changeState( int newState )
{
	switchTo
		theMovie->fromTo(308, 450);
		theMovie->play("50000.avi", this, 0, 1, 0);
	END
	BEG
		theGame->newRoom( new Rm50000 );
	END
}

/* Room 50001 */
Rm50001::Rm50001()
{
	name = "Rm50001";
	show_style = VE_FLIP;
}

Rm50001::~Rm50001()
{
//	snd50001->dispose();
}

void Rm50001::init()
{
	drawPic( 50020 );			//close-up shot with video camera
    Room::init();

//	snd50001 = new Sound;
  	
  	new(SouthExit50001);
	new(CameraChip50001);

  	intrface->show();
  	theGame->handsOn();

	sfxLoadRes ( 50011, RES_WAVE );
}

int Rm50001::handleEvent ( MADEEventStamp *event )
{
	return false;
}

SouthExit50001::SouthExit50001()
{
	name = "SouthExit50001";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
}

void SouthExit50001::respond()
{
	theGame->setCursor(southCursor);
}

int SouthExit50001::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm50000);
    	return true;
	}
	return false;
} 

/*******************************
 *
 *	Views
 *
 *******************************/

CameraChip50001::CameraChip50001()
{
	name = "CameraChip50001";
	setHotspot ( DO_V );
	setView(50035);		   
///	init(39, 118);
	init(13, 108);
	noSkip();
}

void CameraChip50001::respond()
{				  
	theGame->setCursor( normalCursor );
	normalCursor->blink(CRSBLINK_SPEED);
}

int CameraChip50001::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
//		sfxPrintf( "camera chip" );
//		curRoom->setScript(new GetCamera50000);
///		snd50001->number = 50011; 
//		snd50001->play();
		if( sound2->number != 50011  || !sound2->isPlaying()) {
			sound2->number = 50011;
			sound2->play();
		}
		
		GameFlag.set(fPickedUpCamera);
		inventry->get(iVIDEO_CAMERA);		   
		
		theGame->newRoom(new Rm50100);
    	return true;
	}
	return false;
}

/*********************
*
*		Room Code 
*
**********************/

Rm50100::Rm50100()
{
	name = "Rm50100";
	show_style = VE_FLIP;
}

Rm50100::~Rm50100()
{
}

void Rm50100::init()
{
  	drawPic( 50025 );		   //without video camera, close-up shot
    Room::init();
  
	new(SouthExit50100);
	
  	intrface->show();
  	theGame->handsOn();
}

int Rm50100::handleEvent ( MADEEventStamp *event )
{
	return false;
}

SouthExit50100::SouthExit50100()
{
	name = "SouthExit50100";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
}

void SouthExit50100::respond()
{
	theGame->setCursor(southCursor);
}

int SouthExit50100::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm50200);
    	return true;
	}
	return false;
}

/* Room 50200 */
Rm50200::Rm50200()
{
	name = "Rm50200";
	show_style = VE_FLIP;
}

Rm50200::~Rm50200()
{
}

void Rm50200::init()
{
  	drawPic( 50100 );		   //without video camera, wide shot
    Room::init();
  
	new(SouthExit50200);
	new(Blanket50000);
	new(Garbage50000);
	new(JunkyWall50000);
	
  	intrface->show();
  	theGame->handsOn();
}

int Rm50200::handleEvent ( MADEEventStamp *event )
{
	return false;
}

SouthExit50200::SouthExit50200()
{
	name = "SouthExit50200";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
}

void SouthExit50200::respond()
{
	theGame->setCursor(southCursor);
}

int SouthExit50200::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
////	GameFlag.clear(fDisplayStakout);
		GameFlag.clear(fCanDisplayStakeOut);       	//do not display Stakeout on map

    	theGame->newRoom(new Rm1000);
    	return true;
	}
	return false;
}

/* Room 50300 */
Rm50300::Rm50300()
{
	name = "Rm50300";
	show_style = VE_FLIP;
}

Rm50300::~Rm50300()
{
}

void Rm50300::init()
{
  	drawPic( 50001 );		   
    Room::init();

  	intrface->show();
  	theGame->handsOn();

	new(SouthExit50300);
	new(NorthExit50300);

	GameFlag.set(fMaxDead);	
}

int Rm50300::handleEvent ( MADEEventStamp *event )
{
	return false;
}

GetCamera50000::GetCamera50000()
{
	name = "GetCamera50000"; 
}

void GetCamera50000::changeState( int newState )
{	   
	switchTo
//		snd50001->number = 50011; 
//		snd50001->play();
	END
}

SouthExit50300::SouthExit50300()
{
	name = "SouthExit50300";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
}

void SouthExit50300::respond()
{
	theGame->setCursor(southCursor);
}

int SouthExit50300::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm50000);
    	return true;
	}
	return false;
} 

NorthExit50300::NorthExit50300()
{
	name = "NorthExit50300";
	setHotspot ( DO_V );
	init(142, 54, 353, 277);
}

void NorthExit50300::respond()
{
	theGame->setCursor(northCursor);
}

int NorthExit50300::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		curRoom->setScript( new EnterBlackScreenScript50300 );
    	return true;
	}
	return false;
} 
 
} // namespace Spycraft 
