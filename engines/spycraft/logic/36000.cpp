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
#include "spycraft/logic/36000.h"
#include "spycraft/logic/36100.h"
#include "spycraft/logic/36400.h"
#include "spycraft/logic/36500.h"
#include "spycraft/logic/36600.h"
#include "spycraft/logic/36700.h"
#include "spycraft/logic/1000.h"
#include "flag.h"
#include "invent.h"
#include "movie.h"
#include "sound.h"
#include "spycraft/logic/60000.h"
#include "intrpuzz.h"

namespace Spycraft {

#define CRSBLINK_SPEED 5

extern Invent* inventry;
extern Intrface* intrface;
extern Flag GameFlag;
extern BlinkCursor* normalCursor;
extern NorthCursor* northCursor;
extern SouthCursor* southCursor;
extern Sound* sound1;
extern int currentFocus;
extern IntrPuzz* intrpuzz;

static Prop* minMax;
PermListObject* procatTimer;

/*********************
*
*		Room Code 
*
**********************/

Rm36000::Rm36000()
{
	name = "Rm36000";
	show_style = VE_FLIP;
}

void Rm36000::init()
{
	Room::init();

int yuriCanPlay = FALSE;
int birdCanPlay = FALSE;
int yingCanPlay = FALSE;


	if((!GameFlag.test(fWahKa40)) && (GameFlag.test(fYingCaptured))) {
		yingCanPlay = TRUE;
	}
	else {
//		if((!GameFlag.test(fWahKa41)) && (GameFlag.test(fSolvedDubanskyKAT)))
		if((!GameFlag.test(fWahKa41)) && (GameFlag.test(fPEGBriefing)))
			yuriCanPlay = TRUE;
		if(!GameFlag.test(fWahKa42))
			birdCanPlay = TRUE;
	}


	if(yingCanPlay) {
		GameFlag.set(fWahKa40);
		theGame->newRoom( new Rm36700 );
	}
	else if(birdCanPlay && yuriCanPlay) {
		GameFlag.set(fWahKa41);
		GameFlag.set(fWahKa42);
		theGame->newRoom( new Rm36600 );
	}
	else if(birdCanPlay && !yuriCanPlay) {
		GameFlag.set(fWahKa42);
		theGame->newRoom( new Rm36400 );
	}
	else if(!birdCanPlay && yuriCanPlay) {
		GameFlag.set(fWahKa41);
		theGame->newRoom( new Rm36500 );
	}
	else {
		drawPic( 36000 );

		new(SouthExit36000);
		new(Camera36000);
		minMax = new MinuteMax36000;
		GameFlag.set(fWahKa57);
		setScript( new MinuteScript36000 );
		
		intrface->show();
		theGame->handsOn();
				
		if(GameFlag.test(fPlayerTurnsProcat)) {		//timer is only active when player turned Procat
			new(NorthExit36000);
		
			if(!GameFlag.test(fWahKa81)) {
				GameFlag.set(fWahKa81);
				procatTimer = new PermListObject;
				procatTimer->setScript( new BackgroundScript36000 ); 
			}
		}
	 
		if(sound1->number != 36000  || !sound1->isPlaying()) {
//			setScript( new BackgroundScript36000);
			GameFlag.set(fWahKa30);
			sound1->number = 36000;
			sound1->loop = SND_LOOP_INFINITE;
			sound1->playBuffered( 36000, NULL );
		}
	}   //end else
}

void Rm36000::cue( void )
{
//	theMovie->fromTo(1522, 1523);
//	theMovie->play("36001.avi", this, 0, 0, 0);
}

Rm36000::~Rm36000( void )
{
//	sound1->stop();

	if(GameFlag.test(fWahKa57)) {
		GameFlag.clear(fWahKa57);
		minMax->dispose();
	}
}	

int Rm36000::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}

NorthExit36000::NorthExit36000()
{
	name = "NorthExit36000";
	setHotspot ( DO_V );
//	init(NORTH_EXIT_X, NORTH_EXIT_Y, NORTH_EXIT_WIDTH, NORTH_EXIT_HEIGHT);
	init( 113, 165, 275, 210);  //desk
}

void NorthExit36000::respond()
{
	theGame->setCursor(northCursor);
	aniCursor();
}

int NorthExit36000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		theGame->newRoom( new Rm36100 );
		return TRUE;
	}
	return FALSE;
}

SouthExit36000::SouthExit36000()
{
	name = "SouthExit36000";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
}

void SouthExit36000::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit36000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		if(!GameFlag.test(fPlayerTurnsProcat)) {
			if(GameFlag.test(fWahKa81)) {
				procatTimer->dispose();
				//delete procatTimer;
				//procatTimer = NULL;
				GameFlag.clear(fWahKa81);
			}
			if(GameFlag.test(fWahKa30)) {
				GameFlag.clear(fWahKa30);
				sound1->stop();
			}
			theGame->newRoom(new Rm1000);
		}
		else {   //player must go directly to van and station house shot
			if(GameFlag.test(fWahKa81)) {
				procatTimer->dispose();
				//delete procatTimer;
				//procatTimer = NULL;
				GameFlag.clear(fWahKa81);
			}
			if(GameFlag.test(fWahKa30)) {
				GameFlag.clear(fWahKa30);
				sound1->stop();
			}
			curMap = MAP_STATION_EXT;
			theGame->newRoom(new Rm1000);		
		}
/*
		if(GameFlag.test(fProcatPlayerCaught)) {
		}
		else if(GameFlag.test(fWahKa30)) {
			GameFlag.clear(fWahKa30);
			sound1->stop();
			theGame->newRoom(new Rm1000);
		}
		else {
			theGame->newRoom(new Rm1000);
		}
*/
    	return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Features
 *
/*******************************/

Camera36000::Camera36000()
{
	name = "Camera36000";
	setHotspot( DO_V );
	init ( 343, 71, 47, 44 );
}

void Camera36000::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Camera36000::doVerb( int theVerb )
{
	if ( theVerb == DO_V ) {
		new CameraInset36000();
		return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Insets
 *
 *******************************/

CameraInset36000::CameraInset36000()
{
	normalCursor->rest();
 	init(36101, 0, 15, 512, 385);
}

/*******************************
 *
 *	Scripts
 *
 *******************************/

BackgroundScript36000::BackgroundScript36000()
{
	name = "BackgroundScript36000"; 
}

void BackgroundScript36000::changeState( int newState )
{	   	  
	switchTo
		ticks = 2;
	END
	BEG
		seconds = 75;
	END
	BEG
		if(currentFocus == PUZZLE_FOCUS) {		//determine if the pda is up
			intrpuzz->quitIntrface();
		}
		ticks = 15;
	END
	BEG
		sound1->stop();
		theGame->handsOff();
		features->dispose();
		if( GameFlag.test( fWahKa57 ) )
		{
			minMax->dispose();
			GameFlag.clear( fWahKa57 );
		}

		if(GameFlag.test(fWahKa80)) {
			theMovie->fromTo(0, 417);
   			theMovie->play("36100.avi", this, 0, 1, 0);
		}
		else {
			theMovie->fromTo(150, 417);
   			theMovie->play("36100.avi", this, 0, 1, 0);
		}		
	END
	BEG
		ticks = 15;
	END
	BEG
		theGame->newRoom(new Rm60000(END_JAIL));
		procatTimer->dispose();
//		if (procatTimer)	{
//			delete procatTimer;
//			procatTimer = NULL;
//		}
	END
}

MinuteScript36000::MinuteScript36000()
{
	name = "MinuteScript36000"; 
}

void MinuteScript36000::changeState( int newState )
{	   	  
	switchTo
		ticks = 2;
	END
	BEG
		minMax->setCycle(new EndLoop(minMax, this));
	END
	BEG
		minMax->setCycle(new BegLoop(minMax, this));
	END
	BEG
//		changeState(0);
		state = 0;
		ticks = 2;
	END
}
 	
MinuteMax36000::MinuteMax36000()
{
	name = "MinuteMax36000";
	setView( 36010 );
 	init( 350, 76 );	
	setHotspot( DO_V );
	cycleSpeed = 60;				//higher the number, slower it is.
//	setCycle( new Forward( this ) );
}

void MinuteMax36000::cue( void )
{
 	setCycle( new Forward( this ) );	
}

/*******************************
 *
 *	PermListObj
 *
 *******************************/
/*
ProcatTimer::ProcatTimer()
{
	name = "ProcatTimer";
	permList->add( this );
}

void ProcatTimer::dispose( void )
{

	//permList->release( this );
	
}
*/
 
} // namespace Spycraft 
