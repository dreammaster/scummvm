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
#include "spycraft/game/flag.h"	  
#include "spycraft/game/movie.h"
#include "spycraft/game/sound.h"
#include "spycraft/game/roomsnd.h"
#include "spycraft/game/timer.h"
#include "spycraft/game/vlink.h"
#include "spycraft/game/plistobj.h"
#include "spycraft/logic/2000.h"
#include "spycraft/logic/1000.h"

namespace Spycraft {

#define CRSBLINK_SPEED 5
#define MAX_TOUCHES 1 		/* triggers Frank's return */
#define SOUNDTYPE	PLAYWAVEMIDI

extern Intrface* intrface;
extern Sound*	sound1;
extern NorthCursor* northCursor;
extern SouthCursor* southCursor;
extern BlinkCursor* normalCursor;
extern DoorCursor*	doorCursor;		// door cursor?  archaic
extern short	drawBackground;		// Travelink

static LEDInsetView2100*	LEDViewPtr;
static Blinker2000*			blinker;	// LED blinker
static HurryUp2000*		hurryUp;	// object for cue of timer
static PermListObject*	bigPress;	// big phone button press
static PermListObject*	press1;		// phone button 1 press
static PermListObject*	fTimerObj;	// F's entrance. Ptr to host allows dispose on room exit 
static int	blinking;		// flag, LED is blinking.  set to false to stop blinking
static int	phoneUp;		// flag, phone inset is up
static int	heardPhoneWoman;// flag, played 'you have two messages...' 
static int	messagePlaying;	// flag, phone machine is playing a message
static int	frankSad;		// flag, in Frank sad sequence
static int	playingFrankEnter;	// flag
static int	bt1EggCt;		// counter to delay playing Adams message
static Sound*	walkSnd;		// sound of Frank coming.  May be stopped by inset.
static int		pictureTouchCt; // count of the times a picture was touched (could be same one)

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

void stopSound( void )
{
	#if BKSOUND == ON
	if(	sound1->isPlaying() )
		sound1->stop();
	#endif
	#if BKMIDI == ON
	if( theMusic->isPlaying() )
	 	theMusic->stop();
	#endif
}

class PlayASound : public Object
{
 public:
 	PlayASound( int soundNum );
	void cue( void );
	Sound*	soundptr;
};

PlayASound::PlayASound( int soundNum )
{
	soundptr = new Sound;
	soundptr->play( soundNum, this );	
}

void PlayASound::cue( void )
{
	soundptr->dispose();
	dispose();
}

/*********************
*
*		Rooms 
*
**********************/

// Intro movie room, segue from travelink
Rm2001::Rm2001() 
{
	name = "Rm2001";
	show_style = VE_FLIP;
}

void Rm2001::init()
{
	Room::init();
	setScript( new TravelInScript2001 );
}

// Main room
Rm2000::Rm2000()
{
	name = "Rm2000";
	show_style = VE_FLIP;
}

Rm2000::~Rm2000()
{
 	stopSound();
	if( fTimerObj != NULL )
	{
		fTimerObj->dispose();
		fTimerObj = NULL;
	}
}

void Rm2000::init()
{
	// frankSad: if finished shooters and not played Frank sad movie
	if( GameFlag.test( fFarmCODone )  && !GameFlag.test( fFarmDidFrankSad ) )
		frankSad = true;
	else
		frankSad = false;

	Room::init();
	if( frankSad )
	{
		//drawPic( 1 );
		setScript( new FrankSadScript2000 );
	}
	else if( !GameFlag.test( fEnteredFranksOffice ) )
	{
		setScript( new EnterScript2000 );
	}
	else
	{	
		if( !GameFlag.test( fFarmDidPhotoIntro ) )
			new FrankTimer2000;
 		drawPic( 2000 );
		startSound( 2000, SOUNDTYPE );
		furnishRoom();
	}
	pictureTouchCt = 0;
	bt1EggCt = 0;
	playingFrankEnter = false;			
}

void Rm2000::furnishRoom( void )
{
	if( GameFlag.test( fFarmDidPhotoIntro ) )
	{
	 	new SouthExit2000;
		new Door2000;
	}
	//new Desk2000;
	
	new Target2000;
	new GunRack2000;

	new MidDesk2100;
	new RightDesk2100;
	heardPhoneWoman = false;

	new	PictureColby2200;
	new PictureHarmonica2200;

	press1 = NULL;
	bigPress = NULL;
 	intrface->show();
 	theGame->handsOn(); 	
}

// no longer used
Rm2100::Rm2100()
{
	name = "Rm2100";
	//show_style = VE_FLIP;
	show_style = VE_FULL_DISSOLVE;
}

// no longer used
void Rm2100::init()
{
	drawPic( 2100 );
	Room::init();
	new SouthExit2100;
	//new LeftDesk2100;
	new MidDesk2100;
	new RightDesk2100;
	intrface->show();
	theGame->handsOn();
	heardPhoneWoman = false;
}

/*********************
*
*	Exit Features 
*
**********************/

SouthExit2000::SouthExit2000()
{
	name = "SouthExit2000";
	setHotspot( DO_V );
	init( SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT );  
}

void SouthExit2000::respond()
{
	theGame->setCursor( southCursor );
	aniCursor();
}

int SouthExit2000::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
    	theGame->newRoom( new Rm1000 );
    	return true;
	}
	return false;
}

// no longer used
SouthExit2100::SouthExit2100()
{
	name = "SouthExit2100";
	setHotspot( DO_V );
	init( SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT );  
}

// no longer used
void SouthExit2100::respond()
{
	theGame->setCursor( southCursor );
	aniCursor();
}

// no longer used
int SouthExit2100::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
    	theGame->newRoom( new Rm2000 );
    	return true;
	}
	return false;
}

// no longer used
SouthExit2300::SouthExit2300()
{
	name = "SouthExit2300";
	setHotspot( DO_V );
	init( SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT );  
}

// no longer used
void SouthExit2300::respond()
{
	theGame->setCursor( southCursor );
	aniCursor();
}

// no longer used
int SouthExit2300::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
    	theGame->newRoom( new Rm2000 );
    	return true;
	}
	return false;
}

/*******************************
 *
 *	Features
 *
/*******************************/

// no longer used
Desk2000::Desk2000()
{
	name = "Desk2000";
	setHotspot( DO_V );
	init( 121, 280, 260, 85 );
}

// no longer used
void Desk2000::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

// no longer used
int Desk2000::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		curRoom->zoomTo( 273, 311 );
		theGame->newRoom( new Rm2100 );
		return true;
	}
	return false;
}

Door2000::Door2000()
{
	name = "Door2000";
	setHotspot( DO_V );
	init( 91, 50, 60, 215 );
}

void Door2000::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Door2000::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		theGame->newRoom( new Rm1000 );
		return true;
	}
	return false;
}

Target2000::Target2000()
{
 	name = "Target2000";
	setHotspot( DO_V );
	init( 155, 135, 35, 65 );
}

void Target2000::respond( void )
{
 	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Target2000::doVerb( int theVerb )
{
 	if(theVerb == DO_V )
	{
		curRoom->zoomTo( 180, 150 );
		new TargetInset2000;		
		return true;
	}
	else
		return false;
}

GunRack2000::GunRack2000()
{
 	name = "GunRack2000";
	setHotspot( DO_V );
	init( 385, 160, 120, 155 );
}

void GunRack2000::respond( void )
{
 	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int GunRack2000::doVerb( int theVerb )
{
 	if(theVerb == DO_V )
	{
		curRoom->zoomTo( 450, 235 );
		new GunRackInset2000;		
		return true;
	}
	else
		return false;
}

// 2100 --desk

MidDesk2100::MidDesk2100() // book
{
	name = "MidDesk2100";
	setHotspot( DO_V );
	init( 182, 295, 50, 40 );
}

void MidDesk2100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int MidDesk2100::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		//curRoom->zoomTo( 153, 231 );
		new MidDeskInset2100;		
		return true;
	}
	return false;
}

RightDesk2100::RightDesk2100() 	// ----phone
{
	name = "RightDesk2100";
	setHotspot( DO_V );
	init( 310, 296, 46, 24 );
}

void RightDesk2100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int RightDesk2100::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		curRoom->zoomTo( 388, 220 );
		new RightDeskInset2100;		
		return true;
	}
	return false;
}

PictureColby2200::PictureColby2200()
{
	name = "PictureColby2200";
   	setHotspot ( DO_V );
	init ( 267, 220, 28, 36);
}

void PictureColby2200::respond( void )
{
 	theGame->setCursor( normalCursor );
	normalCursor->blink(CRSBLINK_SPEED);
}

int PictureColby2200::doVerb( int theVerb )
{
 	if ( theVerb == DO_V )
	{ 
		curRoom->zoomTo( 232, 268 );
		new PictureColbyInset2200;
		return true;
	}
	return false;
}

PictureHarmonica2200::PictureHarmonica2200()
{
	name = "PictureHarmonica2200";
   	setHotspot ( DO_V );
	init (235, 214, 25, 45);
}

void PictureHarmonica2200::respond( void )
{
 	theGame->setCursor( normalCursor );
	normalCursor->blink(CRSBLINK_SPEED);
}

int PictureHarmonica2200::doVerb( int theVerb )
{
 	if ( theVerb == DO_V )
	{ 
		curRoom->zoomTo( 172, 270 );
		new PictureHarmonicaInset2200;
		return true;
	}
	return false;
}

/*******************************
 *
 *	Inset Features
 *
/*******************************/

PhoneButton2100::PhoneButton2100()
{
	name = "PhoneButton2100";
	setHotspot( DO_V );
	init( 135, 230, 60, 75 );
}

void PhoneButton2100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int PhoneButton2100::doVerb( int theVerb )
{
	if( theVerb == DO_V  && bigPress == NULL)	{
		bigPress = new PermListObject;
		bigPress->setScript( new BigButtonPressScript2100 );
		return true;
	}
	return false;
}

Button1_2100::Button1_2100()	// phone button 1
{
	name = "Button1_2100";
	setHotspot( DO_V );
	init( 205, 112, 60, 75 );
}

void Button1_2100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Button1_2100::doVerb( int theVerb )
{
	if( theVerb == DO_V && press1 == NULL )	{
		press1 = new PermListObject;
		press1->setScript( new Button1PressScript2100 );
		return true;
	}
	return false;
}

CursorArrowizer2100::CursorArrowizer2100()	// Causes inset cursor to be an arrow
{
	name = "CursorArrowizer2100";
	setHotspot( DO_V );
	init( 20, 36, 472, 344 );
}

/*******************************
 *
 *	Inset Views
 *
/*******************************/

LEDInsetView2100::LEDInsetView2100()
{
	name = "LEDInsetView2100";
	setHotspot( DO_V );
	setView( 2102 );
	init ( /*147*/158, /*211*/228 );
	//setPri( 402 );
	normalCursor->rest();	// stops respond blink
}

BigButtonInsetView2100::BigButtonInsetView2100()
{
	name = "BigButtonInsetView2100";
	setHotspot( DO_V );
	setView( 2101 );
	init ( 138, 266 );
	//setPri( 403 );
	normalCursor->rest();	// stops respond blink
}


Button1InsetView2100::Button1InsetView2100()
{
	name = "Button1InsetView2100";
	setHotspot( DO_V );
	setView( 2103 );
	init ( 205, 128 );
	//setPri( 403 );
	normalCursor->rest();	// stops respond blink
}

/*******************************
 *
 *	Insets
 *
/*******************************/

TargetInset2000::TargetInset2000()
{
	init( 2280, 0, 16, 512, 384 );
	normalCursor->rest();	// stops respond blink
}

GunRackInset2000::GunRackInset2000()
{
	init( 2290, 0, 16, 512, 384 );
	normalCursor->rest();	// stops respond blink
}

//LeftDeskInset2100::LeftDeskInset2100()
//{
//	init( 2140, 0, 16, 512, 384 );
//	normalCursor->rest();	// stops respond blink
//}

RightDeskInset2100::RightDeskInset2100() // phone
{
	init( 2120, 0, 16, 512, 384 );
	normalCursor->rest();	// stops respond blink
	phoneUp = true;
	new PhoneButton2100;
	new Button1_2100;
	if( !GameFlag.test( fHeardFranksMessages ) )
	{
		blinker = new Blinker2000;
		blinker->init();	// starts blinking		
	}
}

void RightDeskInset2100::dispose( void )	// close phone, stop LED blink script
{
	phoneUp = false;
	if( blinker != NULL )
	{
		blinker->dispose();
		blinker = NULL;
	}
	Inset::dispose();
}

MidDeskInset2100::MidDeskInset2100()	// book
{
	init( 2160, 0, 16, 512, 384 );
	normalCursor->rest();	// stops respond blink
	new CursorArrowizer2100;
}

int MidDeskInset2100::doVerb( int theVerb )
{
	if( theVerb == DO_V )
	{
		dispose();
		new OpenManualInset2100;
		new PlayASound( 2211 );	
		return true;
	}
	return false ;
}

OpenManualInset2100::OpenManualInset2100()	// open book
{
	init( 2180, 0, 16, 512, 384 );
	normalCursor->rest();	// stops respond blink
	new CursorArrowizer2100;
}

int OpenManualInset2100::doVerb( int theVerb )
{
	if( theVerb == DO_V )
	{
		dispose();
		new MidDeskInset2100;
		return true;
	}
	return false;
}

PictureInset2200::PictureInset2200()
{
	name = "PictureInset2200";
	walkingSoundPlaying = false;
}

PictureColbyInset2200::PictureColbyInset2200()
{
 	init(2220, 0, 16, 512, 384);
	normalCursor->rest();
	++pictureTouchCt;
	if( !GameFlag.test( fFarmDidPhotoIntro ) && pictureTouchCt >= MAX_TOUCHES )
	{
		theGame->setScript( new FrankComingScript2200 );
		script = new ColbyPictScript2200;
	}
}

PictureHarmonicaInset2200::PictureHarmonicaInset2200()
{
 	init(2240, 0, 16, 512, 384);
	normalCursor->rest();
	++pictureTouchCt;
	if( !GameFlag.test(fFarmDidPhotoIntro ) && pictureTouchCt >= MAX_TOUCHES )
	{
		theGame->setScript( new FrankComingScript2200 );
		script = new HarmonicaPictScript2200;
	}
}

/*******************************
 *
 *	Scripts
 *
/*******************************/

// segue from Travelink (couldn't drawPic after movie play in same room)
void TravelInScript2001::changeState( int newState )
{
 	switchTo
		theMovie->play( "2204.avi", this, false, true, false );
	END

	BEG
		// note: shouldn't need ticks here before new room, movie is separate thread
		theGame->newRoom( new Rm2000 );
	END
}

void EnterScript2000::changeState( int newState )
{
//	Sound* soundPtr; -- sound not available

 	switchTo
		// note: tried playing TL movie here, failed
		curRoom->drawPic( 2000 );			 
//		soundPtr = new Sound;	//play phone VO
//		#define SND_VO_FRANKGUARD 1094
//		#pragma message( "replace this temp sound" )
//		soundPtr->play( SND_VO_FRANKGUARD, this );
		cue();// temp
	END

	BEG
//		soundPtr->dispose();
		GameFlag.set( fEnteredFranksOffice );
		startSound( 2000, SOUNDTYPE );
		new FrankTimer2000;
		((Rm2000*)curRoom)->furnishRoom();
		dispose();
	END
}

void LEDBlinkScript2100::changeState( int newState )
{
	if( !blinking ) // pressed phone button
	{
		blinker->dispose();	// Calling object, gets this one too
		blinker = NULL;
		return;
	}

 	switchTo
		if( currentFocus == INSET_FOCUS )
			LEDViewPtr->hide(); 	
		ticks = 20;
	END

	BEG
		if( currentFocus == INSET_FOCUS )
			LEDViewPtr->show();
		ticks = 40;
	END

	BEG
		dispose();	// have to disconnect before setScript to avoid permList release problem
		if( phoneUp )
			blinker->setScript( new LEDBlinkScript2100 ); // keep blinking
	END
}

void BigButtonPressScript2100::changeState( int newState )
{
	static BigButtonInsetView2100*	phoneButtonPtr;

 	switchTo
		phoneButtonPtr = new BigButtonInsetView2100; 	
		ticks = 20;
	END

	BEG
		if( !phoneUp )
			cue();
		phoneButtonPtr->dispose();
		ticks = 40;
		
	END

	BEG
		if( phoneUp  && !messagePlaying  && !GameFlag.test( fHeardFranksMessages ) )
			theGame->setScript( new PhoneWomanScript2100 );
	//	dispose();	// have to disconnect before setScript to avoid permList release problem
	//	bigPress->setScript( NULL );	// disposes of this script
		bigPress->dispose();
		bigPress = NULL;
	END
}

void Button1PressScript2100::changeState( int newState )
{
 	static Button1InsetView2100*	phoneButtonPtr;

 	switchTo
		phoneButtonPtr = new Button1InsetView2100; 	
		ticks = 20;
	END

	BEG
		if( !phoneUp )
			cue();
		phoneButtonPtr->dispose();
		ticks = 40;	
	END

	BEG
		if( phoneUp && !messagePlaying && !GameFlag.test( fHeardFranksMessages ) && heardPhoneWoman )
			theGame->setScript( new PhoneMessagesScript2100 );
		else if( phoneUp && !messagePlaying  && GameFlag.test( fHeardFranksMessages ) )
		{
			++bt1EggCt;
			if( bt1EggCt == 2 )
				theGame->setScript( new AdamsPhoneScript2100 );
		}
		dispose();	// have to disconnect before setScript to avoid permList release problem
		press1->setScript( NULL );
		press1->dispose();
		press1 = NULL;
	END
}

void PhoneWomanScript2100::changeState( int newState )
{
	static Sound*	soundptr;

 	switchTo
		stopSound();
		messagePlaying = true;
		theGame->handsOff(); 	// blunt fix
		#pragma message( "hmi ticks before buffered sounds" )
		ticks = 16;	
	END

	BEG
		soundptr = new Sound;
		soundptr->playBuffered( SND_VO_FRANKVOICEPHONE, this );		
	END

	BEG
		theGame->handsOn();
		messagePlaying = false;
		heardPhoneWoman = true;		
		if( phoneUp )
			new Button1_2100;
		startSound( 2000, SOUNDTYPE );
		theGame->setScript( NULL );
	END
}

void PhoneMessagesScript2100::changeState( int newState )
{
	static Sound*	soundptr;

 	switchTo
		stopSound();
		messagePlaying = true;
		theGame->handsOff(); 	// blunt fix
		ticks = 16;	// hmi breather	
	END

	BEG		
		soundptr = new Sound;
		soundptr->playBuffered( SND_VO_FRANKCOHENPHONE, this );		
	END

	BEG
	   ticks = 16;	// hmi breather
	END

	BEG
		soundptr->playBuffered( SND_VO_FRANKFRIENDPHONE, this );
	END

	BEG
		theGame->handsOn();
		messagePlaying = false;
		GameFlag.set( fHeardFranksMessages );	// finished listening to Frank's messages
		blinking = false;
		startSound( 2000, SOUNDTYPE );
		theGame->setScript( NULL );
	END
}

void AdamsPhoneScript2100::changeState( int newState )
{
	static Sound*	soundptr;

 	switchTo
		stopSound();
		theGame->handsOff(); 	// blunt fix
		ticks = 16;	// hmi breather
		
	END
		  
	BEG
		messagePlaying = true;
		soundptr = new Sound;
		soundptr->playBuffered( 2114, this );	
	END

	BEG
		theGame->handsOn();
		messagePlaying = false;
		startSound( 2000, SOUNDTYPE );
		theGame->setScript( NULL );
	END
}

void FrankSadScript2000::changeState( int newState )
{
	Timer*	timer;

 	switchTo
		// The following movie was incorporated into the Bruce bites it movie
		//theGame->handsOff();	
		//theMovie->play( "68.avi", this, false, true, false );  // Frank sad	
		ticks = 2;
	END
   
	BEG
		//theGame->handsOn();
		GameFlag.set( fFarmDidFrankSad );
		new VLink( "You're up, Thorn: DDO Warhurst, Eugene J.",
		           "69.avi", 0, NULL ); // Warhurst: "Get to Sterling's office!"
		GameFlag.set( fFarmDone );
		GameFlag.set( fDCIOfficeAvailable ); 
		hurryUp = new HurryUp2000;
		timer = new Timer;
		timer->setReal( 300, hurryUp );	// 5 minutes
		curMap = MAP_WASHINGTON;
		drawBackground = false;	// Travelink
		theGame->newRoom( new Rm1000 );
	END
}

void FrankComingScript2200::changeState( int newState )
{
 	switchTo
		playingFrankEnter = true;
		ticks = 10;
		theGame->handsOff();
	END

	BEG
		assert( theInset != NULL ); 	// I don't think this should be possible
		//walkSnd = new Sound;
		//walkSnd->number = SND_FRANKCOMING;
		//walkSnd->play( this );	
		ticks = 2;	// instead of sound cue
	END

	BEG
		//walkSnd->dispose();
		assert( theInset != NULL );
		//( (PictureInset2200*) theInset )->walkingSoundPlaying = false;
		theGame->setScript( theInset->script );
		theInset->script = NULL;
	END
}

void ColbyPictScript2200::changeState( int newState )
{
 	switchTo
		ticks = 2;
	END

	BEG
		theMovie->play( "2201.avi", this, false, true, false );   // formerly 37.avi
	END

	BEG
		theGame->setScript( new FrankIntroScript2200 );
	END
}

void HarmonicaPictScript2200::changeState( int newState )
{
 	switchTo
		ticks = 2;
	END

	BEG
		theMovie->play("2202.avi", this, false, true, false );	 // formerly 628.avi
	END

	BEG
		theGame->setScript( new FrankIntroScript2200 );
	END
}

void FrankIntroScript2200::changeState( int newState )
{
 	switchTo
		//theGame->handsOn();
		theInset->dispose();
		GameFlag.set( fFarmDidPhotoIntro );
		playingFrankEnter = false;
		//new SouthExit2000;
		//new Door2000;
		theGame->newRoom( new Rm2000 );
		theGame->setScript( NULL );
	END
}

/*******************************
 *
 *	Misc
 *
/*******************************/

FrankTimer2000::FrankTimer2000()
{
	name = "FrankTimer2000";
	firstTime = true;
	fTimerObj = new PermListObject;
	fTimerObj->setScript( this );	
}

void FrankTimer2000::changeState( int newState )
{
 	switchTo
 		if( firstTime )
		{
 			seconds = 120;
			firstTime = false;
		}
		else
			seconds = 10;
	END

	BEG
		if( GameFlag.test( fFarmDidPhotoIntro )  || playingFrankEnter )
		{
			client->dispose();	// perm list obj, gets this script too
			fTimerObj = NULL;
		}
		//else if( strcmp( curRoom->name, "Rm2000" ) != 0 )	// not in 2000 (only room left)
		//	client->dispose();	// perm list obj, gets this script too
		else if( currentFocus != 0 )	// inset, PDA, etc.
		{
			client->script = NULL;
			seconds = 0;
			state = -1;
			client->setScript( this );	// reset
		}
		else
		{
			playingFrankEnter = true;
			theGame->handsOff();
		 	//walkSnd = new Sound;
			//walkSnd->number = SND_FRANKCOMING;
			//walkSnd->play( this );	
			ticks = 2;	// instead of sound cue
		}
	END

	BEG
		//walkSnd->dispose();
		theMovie->play( "2203.avi", this);
	END

	BEG
		GameFlag.set( fFarmDidPhotoIntro );
		playingFrankEnter = false;
		theGame->handsOn();
		new SouthExit2000;
		new Door2000;
		fTimerObj = NULL;	// points to client
		client->dispose();	// perm list obj
	END
}

void HurryUp2000::cue( void )
{	
	if( strcmp( curRoom->name, "Rm18000" ) != 0  && !GameFlag.test( fFullBriefing ) )
		new VoiceMail( "Cat-1 Urgent: DDOWarhurst, Eugene J.", 981, WARHURST );	
	dispose();
}

// blinker 2000, blinks LED with an attached script
void Blinker2000::init( void )
{
 	name = "blinker";
 	LEDViewPtr = new LEDInsetView2100;	
	blinking = true; 	// has to come before setScript
	setScript( new LEDBlinkScript2100 );
}

void Blinker2000::dispose( void )
{
	LEDViewPtr->dispose();
	Object::dispose();
}

 
} // namespace Spycraft 
