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
#include "movie.h"
#include "flag.h"
#include "roomsnd.h"
#include "27000.h"
#include "1000.h"
#include "web.h"

namespace Spycraft {

#define CRSBLINK_SPEED 5

extern Intrface* intrface;

extern SouthCursor* southCursor;
extern EastCursor* eastCursor;
extern WestCursor* westCursor;
extern BlinkCursor* normalCursor;

/*********************
*
*	Background Sound 
*
**********************/

extern MADEEnvirons* colbyInfo;

enum{ PLAYWAVE, PLAYMIDI, PLAYWAVEMIDI };

static void startSound( int number, int which )
{
	if( which == PLAYWAVEMIDI )
	{
		if( colbyInfo->slow_system )
			which = PLAYMIDI;
		else
			which = PLAYWAVE;
	}
	#if BKSOUND == ON
		if( which == PLAYWAVE )
		{
			if( sound1->number != number  || !sound1->isPlaying() )
			{	
				sound1->stop();
				sound1->loop = SND_LOOP_INFINITE;
				sound1->playBuffered( number, NULL );
			}
		}
	#endif
	#if BKMIDI == ON
		if( which == PLAYMIDI )
		{
			if( theMusic->number != number  || !theMusic->isPlaying() )
			{
			 	theMusic->stop();
				theMusic->play( number );
			}
		}
	#endif
}

/*********************
*
*		Room Code 
*
**********************/

Rm27000::Rm27000()
{
	name = "Rm27000";
	show_style = VE_FLIP;
}

void Rm27000::init()
{
	// background sound
	startSound( 27000, PLAYWAVEMIDI );

   	drawPic( 27000 );
    Room::init();
  	new SouthExit27000;
	new EastExit27000;
	new WestExit27000;
	new Photo27000;
	new Phone27000;
	new Camera27000;
  	intrface->show();
  	theGame->handsOn();
}

// 27100
Rm27100::Rm27100()
{
	name = "Rm27100";
	show_style = VE_FLIP;
}

void Rm27100::init()
{
	drawPic( 27100 );
	Room::init();
	startSound( 27000, PLAYWAVEMIDI );
	new SouthExit27100;
	new EastExit27100;
	new Phone27100;
	new Camera27100;
	new Folder27100;
	intrface->show();
	theGame->handsOn();
}

// 27200
Rm27200::Rm27200()
{
	name = "Rm27200";
	show_style = VE_FLIP;
}

void Rm27200::init()
{
	drawPic( 27200 );
	Room::init();
	startSound( 27000, PLAYWAVEMIDI );
	new SouthExit27200;
	new WestExit27200;
	intrface->show();
	theGame->handsOn();
}

/*********************
*
*	Exit Features 
*
**********************/

SouthExit27000::SouthExit27000()
{
	name = "SouthExit27000";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);  
}

void SouthExit27000::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit27000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm1000);
    	return TRUE;
	}
	return FALSE;
}

EastExit27000::EastExit27000()
{
	name = "EastExit27000";
	setHotspot( DO_V );
	init( EAST_EXIT_X, EAST_EXIT_Y, EAST_EXIT_WIDTH, EAST_EXIT_HEIGHT );  
}

void EastExit27000::respond()
{
	theGame->setCursor( eastCursor );
	aniCursor();
}

int EastExit27000::doVerb( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom( new Rm27200 );
    	return TRUE;
	}
	return FALSE;
}

WestExit27000::WestExit27000()
{
	name = "WestExit27000";
	setHotspot( DO_V );
	init(WEST_EXIT_X, WEST_EXIT_Y +90, WEST_EXIT_WIDTH, WEST_EXIT_HEIGHT -90);  
}

void WestExit27000::respond()
{
	theGame->setCursor(westCursor);
	aniCursor();
}

int WestExit27000::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
    	theGame->newRoom( new Rm27100 );
    	return TRUE;
	}
	return FALSE;
}

//27100
SouthExit27100::SouthExit27100()
{
	name = "SouthExit27100";
	setHotspot( DO_V );
	init( SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT );  
}

void SouthExit27100::respond()
{
	theGame->setCursor( southCursor );
	aniCursor();
}

int SouthExit27100::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
    	theGame->newRoom( new Rm1000 );
    	return TRUE;
	}
	return FALSE;
}

EastExit27100::EastExit27100()
{
	name = "EastExit27100";
	setHotspot( DO_V );
	init( EAST_EXIT_X, EAST_EXIT_Y, EAST_EXIT_WIDTH, EAST_EXIT_HEIGHT );  
}

void EastExit27100::respond()
{
	theGame->setCursor( eastCursor );
	aniCursor();
}

int EastExit27100::doVerb( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom( new Rm27000 );
    	return TRUE;
	}
	return FALSE;
}

// 27200
SouthExit27200::SouthExit27200()
{
	name = "SouthExit27200";
	setHotspot( DO_V );
	init( SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT );  
}

void SouthExit27200::respond()
{
	theGame->setCursor( southCursor );
	aniCursor();
}

int SouthExit27200::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
    	theGame->newRoom( new Rm1000 );
    	return TRUE;
	}
	return FALSE;
}

WestExit27200::WestExit27200()
{
	name = "WestExit27200";
	setHotspot( DO_V );
	init(WEST_EXIT_X, WEST_EXIT_Y, WEST_EXIT_WIDTH, WEST_EXIT_HEIGHT);  
}

void WestExit27200::respond()
{
	theGame->setCursor(westCursor);
	aniCursor();
}

int WestExit27200::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
    	theGame->newRoom( new Rm27000 );
    	return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Features
 *
/*******************************/

Photo27000::Photo27000()
{
	name = "Photo27000";
	setHotspot( DO_V );
	init( 259, 55, 50, 45 );
}

void Photo27000::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Photo27000::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		new PhotoInset27000;
		return TRUE;
	}
	return FALSE;
}

Phone27000::Phone27000()
{
	name = "Phone5000";
	setHotspot( DO_V );
	init( 6, 210, 70, 75 );
}

void Phone27000::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Phone27000::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		new PhoneInset27000;
		return TRUE;
	}
	return FALSE;
}

Camera27000::Camera27000()
{
	name = "Camera27000";
	setHotspot( DO_V );
	init( 6, 15, 55, 75 );
}

void Camera27000::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Camera27000::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		new CameraInset27000;
		return TRUE;
	}
	return FALSE;
}

// 27100
Phone27100::Phone27100()
{
	name = "Phone27100";
	setHotspot( DO_V );
	init( 251, 215, 95, 60 );
}

void Phone27100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Phone27100::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		new PhoneInset27100;
		return TRUE;
	}
	return FALSE;
}	

Camera27100::Camera27100()
{
	name = "Camera27100";
	setHotspot( DO_V );
	init( 281, 45, 50, 50 );
}

void Camera27100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Camera27100::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		new CameraInset27100;
		return TRUE;
	}
	return FALSE;
}

Folder27100::Folder27100()
{
	name = "Folder27100";
	setHotspot( DO_V );
	init( 146, 220, 70, 35 );
}

void Folder27100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Folder27100::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		new FolderInset27100;
		return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Insets
 *
/*******************************/

PhotoInset27000::PhotoInset27000()
{
	init( 27020, 0, 16, 512, 384 );
	normalCursor->rest();	// stops respond blink
}

void PhotoInset27000::respond()
{

}

int PhotoInset27000::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
		//Do something . . .
    	return TRUE;
	}
	return FALSE;
}

PhoneInset27000::PhoneInset27000()
{
	init( 27140, 0, 16, 512, 384 );
	normalCursor->rest();	// stops respond blink
	new PhoneIFeature27000;
}

void PhoneInset27000::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int PhoneInset27000::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
		//new Web;
		//new HTMLPage( 98800 );
		//comlinkshort->hide();	// hides the receive button
		//homobutt->setCel( 2 );	// deactivates Home button in Web
		return TRUE;
	}
	return FALSE;
}

CameraInset27000::CameraInset27000()
{
	init( 27160, 0, 16, 512, 384 );
	normalCursor->rest();	// stops respond blink
}

void CameraInset27000::respond()
{

}

int CameraInset27000::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
		//Do something . . .
    	return TRUE;
	}
	return FALSE;
}

// 27100
PhoneInset27100::PhoneInset27100()
{
	init( 27140, 0, 16, 512, 384 );
	normalCursor->rest();	// stops respond blink
	new PhoneIFeature27100;
}

void PhoneInset27100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int PhoneInset27100::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
		//new Web;
		//new HTMLPage( 98800 );
		//comlinkshort->hide();	// hides the receive button
		//homobutt->setCel( 2 );	// deactivates Home button in Web
    	return TRUE;
	}
	return FALSE;
}

CameraInset27100::CameraInset27100()
{
	init( 27160, 0, 16, 512, 384 );
	normalCursor->rest();	// stops respond blink
}

void CameraInset27100::respond()
{

}

int CameraInset27100::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
		//Do something . . .
    	return TRUE;
	}
	return FALSE;
}

FolderInset27100::FolderInset27100()
{
	init( 27120, 0, 16, 512, 384 );
	normalCursor->rest();	// stops respond blink
	new CorrTabIFeature27100;
	new BSTabIFeature27100;
}

/*******************************
 *
 *	Inset Features
 *
/*******************************/

PhoneIFeature27000::PhoneIFeature27000()
{
	name = "PhoneIFeature27000";
	setHotspot( DO_V );
	init( 218, 98, 166, 116 );
}

void PhoneIFeature27000::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int PhoneIFeature27000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		new Web;
		new HTMLPage( 98800 );
		comlinkshort->hide();	// hides the receive button
		homobutt->setCel( 2 );	// deactivates Home button in Web
		return TRUE;
	}
	return FALSE;
}

PhoneIFeature27100::PhoneIFeature27100()
{
	name = "PhoneIFeature27100";
	setHotspot( DO_V );
	init( 218, 98, 166, 116 );
}

void PhoneIFeature27100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int PhoneIFeature27100::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		new Web;
		new HTMLPage( 98800 );
		comlinkshort->hide();	// hides the receive button
		homobutt->setCel( 2 );	// deactivates Home button in Web
		return TRUE;
	}
	return FALSE;
}

CorrTabIFeature27100::CorrTabIFeature27100()
{
	name = "CorrTabIFeature27100";
	setHotspot( DO_V );
	init( 105, 50 +16, 132, 25 );
}

void CorrTabIFeature27100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int CorrTabIFeature27100::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		new Yellow( 98634 );
		return TRUE;
	}
	return FALSE;
}

BSTabIFeature27100::BSTabIFeature27100()
{
	name = "BSTabIFeature27100";
	setHotspot( DO_V );
	init( 260, 50 +16, 125, 25 );
}

void BSTabIFeature27100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int BSTabIFeature27100::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		new Yellow( 98630 );
		return TRUE;
	}
	return FALSE;
}
 
} // namespace Spycraft 
