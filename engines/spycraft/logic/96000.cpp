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
#include "spycraft/game/sound.h"
#include "spycraft/game/invent.h"
#include "spycraft/logic/60000.h"
#include "spycraft/logic/96000.h"

namespace Spycraft {

#define CRSBLINK_SPEED 5
#define GUN_X96000  430
#define GUN_Y96000  420

enum{	DROPGUNLOOP,
		SHOOTGUNLOOP,
		GETGUNLOOP
	};
#define MAXPASSES	4

extern Intrface*	intrface;
extern Sound*	sound1;
extern NorthCursor*	northCursor;
extern SouthCursor*	southCursor;
extern EastCursor*	eastCursor;
extern WestCursor*	westCursor;
extern NortheastCursor* northeastCursor;
extern NorthwestCursor* northwestCursor;
extern SoutheastCursor* southeastCursor;
extern SouthwestCursor* southwestCursor;
extern BlinkCursor*	normalCursor;
extern DoorCursor*	doorCursor;

enum{ NOATTEMPT, SHOOTBUTMISS, SHOOTANDHIT };

short state96000 = COVEREDFLOOR96300; // initial state
short attempt96000;			// one of emum--a result or no attempt

//static int movedForward;	// flag--once at down/floor, don't get back/stand view anymore 
//static int doDissolve;		// flag--dissolve into room 96300
//static int leaveAttempts;	// count

/*********************
*
*	Sound 
*
**********************/
enum{ PLAYWAVE, PLAYMIDI, PLAYWAVEMIDI };

static void startSound( int number, int which )
{
	if( which == PLAYWAVEMIDI )
	{
		// detect CPU
		which = PLAYWAVE;	// temp
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
				theMusic->loop = SND_LOOP_INFINITE;
				theMusic->play( number );
			}
		}
	#endif
}

static void stopSound( void )
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
};	// defined in 2000.cpp

/*********************
*
*	Rooms 
*
**********************/
// 96000 -- Exterior establishing
Rm96000::Rm96000()
{
	name = "Rm96000";
	show_style = VE_FLIP;
}

void Rm96000::init()
{
	Room::init();
	sfxLoadRes( 96100, RES_PIC );
	sfxLoadRes( 96120, RES_ATS ); 
	intrface->show();

	//new NorthExit96000;

	#if BKSOUND == ON
			sound1->stop();
	#endif

#pragma message( "remove this test code" )
inventry->get( iNEEDLEPACK );

	setScript( new GrendelStairsScript96000 );
}

// 96100 -- interior, facing window
Rm96100::Rm96100()
{
	name = "Rm96100";
	show_style = VE_FLIP;
}

void Rm96100::init()
{
	drawPic( 96100 );
	Room::init();
	new NorthExit96100;
	new WestExit96100;
	//new WindowProp96100;
	intrface->show();
	theGame->handsOn();

	startSound( 96100, PLAYWAVE ); //play End Treaty sound
}

// 96200 -- room exit
Rm96200::Rm96200()
{
	name = "Rm96100";
	show_style = VE_FLIP;
}

void Rm96200::init()
{
	drawPic( 96200 );
	Room::init();
	new NorthExit96200;
	new EastExit96200;
	intrface->show();
	theGame->handsOn();
}

// 96300 -- floor view
Rm96300::Rm96300()
{
	name = "Rm96300";
	show_style = VE_FLIP;
}

Rm96300::~Rm96300()
{
 	sfxUnlockRes( 911, RES_ATS );
}

void Rm96300::init()
{
	Room::init();

	update();
	//new NorthExit96300; // up window
	new WestExit96300;
	new EastExit96300;
	
	sfxLoadRes( 911, RES_ATS );
	sfxLockRes( 911, RES_ATS );	
	 
	intrface->show();
	theGame->handsOn();
}


void Rm96300::update( void )
{
	switch( state96000 )
	{
	 case COVEREDFLOOR96300:
	 	drawPic( 96300 );
		new Floor96300;
		new CoveredFloorView96300;
		new OutWindow96300;
		//new WindowProp96300;	no longer fits
	 	break;
	 case OPENEDFLOOR96300:
	 	drawPic( 96300 );
		//new WindowProp96300;	no longer fits
		new OutWindow96300;
		new Floor96300; 
	 	break;
	 case CASECLOSED96300:
	 	drawPic( 96400 );
	 	new ClosedCase96300;
		new OutWindow96300;
	 	break;
	 case CASEOPENED96300: // skipping this one
	 	if( GameFlag.test( fDoDissolve96000 ) )
		{
			show_style = VE_FULL_DISSOLVE;
			GameFlag.clear( fDoDissolve96000 );
		}
	 	drawPic( 96500 );
	 	new OpenedCase96300;
		new OutWindow96300;
	 	break;
	 case PEGOUT96300:
	 	drawPic( 96600 );
	 	new PEG96300;
		new OutWindow96300;
	 	break;
	 default:
		//assert( false );
		//use COVEREDFLOOR96300
		//sfxPrintf( "this woulda caused assert" );
		drawPic( 96300 );	
		new Floor96300;
		new CoveredFloorView96300;
		new OutWindow96300;
	}
}

// 96700 --black, with sound effects --not used
Rm96700::Rm96700()
{
	name = "Rm96700";
	show_style = VE_FULL_DISSOLVE;
}

void Rm96700::init()
{
	drawPic( 1 );
	Room::init();
	intrface->show();
	theGame->handsOn();

	setScript( new AssemblePEGScript96700 );
}

/*********************
*
*	Exit Features 
*
**********************/
// 96000
// no longer used
NorthExit96000::NorthExit96000()  // forward to movie
{
	name = "NorthExit96000";
	setHotspot( DO_V );
	init(100, 100, 350, 250 );  
}

// no longer used
void NorthExit96000::respond()
{
	theGame->setCursor( northCursor );
	aniCursor();
}

// no longer used
int NorthExit96000::doVerb ( int theVerb )
{
	if( theVerb == DO_V ) {
    	setScript( new GrendelStairsScript96000 );
    	return true;
	}
	return false;
}

// 96100
NorthExit96100::NorthExit96100()
{
	name = "NorthExit96100";
	setHotspot( DO_V );
	init(100, 100, 350, 250 );  
}

void NorthExit96100::respond()
{
	theGame->setCursor( northCursor );
	aniCursor();
}

int NorthExit96100::doVerb ( int theVerb )
{
	if( theVerb == DO_V ) {
    	theGame->newRoom( new Rm96300 );
		GameFlag.set( fMovedForward96000 );
    	return true;
	}
	return false;
}

WestExit96100::WestExit96100()
{
	name = "WestExit96100";
	setHotspot( DO_V );
	init(WEST_EXIT_X, WEST_EXIT_Y, WEST_EXIT_WIDTH, WEST_EXIT_HEIGHT);  
}

void WestExit96100::respond()
{
	theGame->setCursor(westCursor);
	aniCursor();
}

int WestExit96100::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
    	theGame->newRoom( new Rm96200 );
    	return true;
	}
	return false;
}

// 96200
NorthExit96200::NorthExit96200()  // forward to movie
{
	name = "NorthExit96200";
	setHotspot( DO_V );
	init(100, 100, 350, 250 );  
}

void NorthExit96200::respond()
{
	theGame->setCursor( northCursor );
	aniCursor();
}

int NorthExit96200::doVerb ( int theVerb )
{
	if( theVerb == DO_V ) {
    	curRoom->setScript( new ExitScript96200 );
    	return true;
	}
	return false;
}

EastExit96200::EastExit96200()
{
	name = "EastExit96200";
	setHotspot( DO_V );
	init( EAST_EXIT_X, EAST_EXIT_Y, EAST_EXIT_WIDTH, EAST_EXIT_HEIGHT );  
}

void EastExit96200::respond()
{
	theGame->setCursor( eastCursor );
	aniCursor();
}

int EastExit96200::doVerb( int theVerb )
{
	if ( theVerb == DO_V ) {
    	if( GameFlag.test( fMovedForward96000 ) )
			theGame->newRoom( new Rm96300 );	// floor view
		else
    		theGame->newRoom( new Rm96100 );	// standing window view
    	return true;
	}
	return false;
}

// 96300
WestExit96300::WestExit96300()
{
	name = "WestExit96300";
	setHotspot( DO_V );
	init(WEST_EXIT_X, WEST_EXIT_Y, WEST_EXIT_WIDTH, WEST_EXIT_HEIGHT); 
}

void WestExit96300::respond()
{
	theGame->setCursor( westCursor );
	aniCursor();
}

int WestExit96300::doVerb ( int theVerb )
{
	if( theVerb == DO_V ) {
    	theGame->newRoom( new Rm96200 );	// door (exit)
    	return true;
	}
	return false;
}

EastExit96300::EastExit96300()	// Same as west
{
	name = "EastExit96300";
	setHotspot( DO_V );
	init( EAST_EXIT_X, EAST_EXIT_Y, EAST_EXIT_WIDTH, EAST_EXIT_HEIGHT );  
}

void EastExit96300::respond()
{
	theGame->setCursor( eastCursor );
	aniCursor();
}

int EastExit96300::doVerb ( int theVerb )
{
	if( theVerb == DO_V ) {
    	theGame->newRoom( new Rm96200 );	// door (exit)
    	return true;
	}
	return false;
}

/*******************************
 *
 *	Features
 *
/*******************************/

// (click to remove boards)  --now click to remove case
//	Yeah, I could do some cleanup here...always in a hurry with this room
Floor96300::Floor96300()
{
	name = "Floor96300";
	setHotspot( DO_V );
	init( 200, 250, 150, 150 );
}

void Floor96300::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Floor96300::doVerb( int theVerb )  // this has become the floor removed--case exposed
{
	// NB: trying to use this feature for both the floor and the case in the hole.
	if( theVerb == DO_V )	{
		//if( state96000 == COVEREDFLOOR96300 ) {	// this one not used
		//	state96000 = OPENEDFLOOR96300; 
		//	theGame->newRoom( new Rm96300 );
		//}
		//else ...
		 if( state96000 == OPENEDFLOOR96300 ) {
			state96000 = CASECLOSED96300;
			new PlayASound( 96111 );
			theGame->newRoom( new Rm96300 );
		}
		else {
			//ASSERT( false, 0 );
			state96000 = OPENEDFLOOR96300;
			NULL;
		}
		return true;
	}
	return false;
}

// click on this to open case
ClosedCase96300::ClosedCase96300()
{
	name = "ClosedCase96300";
	setHotspot( DO_V );
	init( 200, 250, 150, 150 );
}

void ClosedCase96300::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int ClosedCase96300::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		state96000 = PEGOUT96300;
		new PlayASound( 96113 );
		theGame->newRoom( new Rm96300 );
		return true;
	}
	return false;
}

// click on this to remove PEG from case, case disappears
// currently not used
OpenedCase96300::OpenedCase96300()
{
	name = "OpenedCase96300";
	setHotspot( DO_V );
	init( 200, 250, 150, 150 );
}

// currently not used
void OpenedCase96300::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

// currently not used
int OpenedCase96300::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		state96000 = PEGOUT96300;
		theGame->newRoom( new Rm96700 );
		return true;
	}
	return false;
}

PEG96300::PEG96300()
{
	name = "PEG96300";
	setHotspot( DO_V );
	hotspot->addRespondVerb(NEEDLEPACK_V);
	init( 200, 150, 150, 150 );
}

void PEG96300::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int PEG96300::doVerb( int theVerb )
{
	if( invVerb == NEEDLEPACK_V )
	{	
		inventry->clearCurrentItem();
		inventry->put( iNEEDLEPACK );
		GameFlag.set( fRoundInPEG );
		return true;
	}
	else if( theVerb == DO_V )	{
		setScript( new PEGUpWindowScript96300 );
		return true;
	}
	return false;
}

OutWindow96300::OutWindow96300()
{
	name = "OutWindow96300";
	setHotspot( DO_V );
	init( 60, 25, 356, 43 );
}

void OutWindow96300::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int OutWindow96300::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		setScript( new OutWindowScript96300 );
		return true;
	}
	return false;
}				  

/*******************************
 *
 *	Views
 *
/*******************************/

CoveredFloorView96300::CoveredFloorView96300()
{
	name = "CoveredFloorView96300";
	setHotspot( DO_V );				
	setView( 96301 );
	init ( 0, 189 );
	normalCursor->rest();	// stops respond blink
}

void CoveredFloorView96300::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int CoveredFloorView96300::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
		state96000 = OPENEDFLOOR96300;
		new PlayASound( 96110 );
		theGame->newRoom( new Rm96300 );		
    	return true;
	}
	return false;
}

PEGTriggerView96300::PEGTriggerView96300()
{
	name = "PEGTriggerView96300";
	count = 0;
	setView( 911 );
	setLoop( SHOOTGUNLOOP );
	init ( GUN_X96000, GUN_Y96000 );
	normalCursor->rest();	// stops respond blink
	permList->add( this );
}

PEGTriggerView96300::~PEGTriggerView96300()
{
	permList->release( this );
}

void PEGTriggerView96300::cue()
{
	int	passes;

	++count;
	passes = count / 2;
	if( passes > MAXPASSES )
	{
		attempt96000 = SHOOTBUTMISS;	// didn't really shoot, but same outcome	
		curRoom->setScript( new ExitScript96200 );
		dispose();
	}
	else
	{
		if( (count % 2) != 0 ) // note: black frames in middle of this . . .
			theMovie->fromTo( 1080, 1184 );	// aim at president
		else
			theMovie->fromTo( 1200, 1293 );	// aim at president
		theMovie->play( "96000.avi", this, false, false, false );
	}
}

int PEGTriggerView96300::handleEvent( MADEEventStamp *event )
{
	if( onMe( event ) )
	{
		if ( event_type & USER_MOUSE_UP ) 
    		return ( doVerb ( user->message ) );
	}
 	return false;
}

int PEGTriggerView96300::doVerb( int theVerb )
{
	int		frame;

	if( theVerb == DO_V && GameFlag.test( fRoundInPEG ) ) {
		GameFlag.set( fShotPEG );
		GameFlag.clear( fRoundInPEG );
		frame = theMovie->getPosn();
		theMovie->pause( false );
		if( 	frame > 1092  && frame < 1120 ||
				frame > 1170  && frame < 1200 ||
				frame > 1260  && frame < 1294	 )	// Hit
			curRoom->setScript( new ShootPresidentScript96300 );
		else	// miss
		{
			attempt96000 = SHOOTBUTMISS;	
			curRoom->setScript( new ExitScript96200 );	
		}
		dispose();
    	return true;
	}
	return false;
}

/*******************************
 *
 *	Props
 *
/*******************************/

// 96100
WindowProp96100::WindowProp96100()
{
	name = "WindowProp96100";
	setView( 96120);
 	init( 222, 42 );
	setHotspot( DO_V );
	cycleSpeed = 4;
	//setCycle( new Forward( this ) );
}

void WindowProp96100::cue( void )
{
 	setCycle( new Forward( this ) );	
}

void WindowProp96100::respond( void )
{
 	theGame->setCursor( northCursor );
}

int WindowProp96100::doVerb( int theVerb )
{
	//if( theVerb == DO_V )
	//{
		//sfxPrintf( "look out window" );
	//	return true;
	//}
	return false;
}

// 96300
WindowProp96300::WindowProp96300()
{
	name = "WindowProp96300";
	setView( 903 );
 	init( 100, 20 );
	setHotspot( DO_V );
	cycleSpeed = 4;
	setCycle( new Forward( this ) );
}

void WindowProp96300::cue( void )
{
 	setCycle( new Forward( this ) );	
}

void WindowProp96300::respond( void )
{
	theGame->setCursor( northCursor );
}

int WindowProp96300::doVerb( int theVerb )
{
	//if( theVerb == DO_V )	   --we have outWindow for this
	//{
	//	sfxPrintf( "look out window" );
	//	return true;
	//}
	return false;
}

/*******************************
 *
 *	Scripts
 *
/*******************************/

// 96000
void GrendelStairsScript96000::changeState( int newState )
{
	switchTo
		theGame->handsOff();
		theMovie->fromTo( 0, 750 );
		theMovie->play( "96000.avi", this, false, true, false );
	END

	BEG
		theGame->newRoom( new Rm96100 );
	END
}

// 96200
void ExitScript96200::changeState( int newState )
{
 	switchTo
		theGame->handsOff();
		if( attempt96000 == NOATTEMPT )
		{
		 	if( /*leaveAttempts == 0*/ !GameFlag.test( f1LeaveAttempt96000 ) )
			{
				theMovie->play( "96200.avi", this );
				//theMovie->fromTo(  );
				//theMovie->play( "96000.avi", this );
				GameFlag.set( f1LeaveAttempt96000 );	
			}
			else
			{
				//theMovie->play( "96201.avi", this );
				theMovie->fromTo( 1305, 1581 );
				theMovie->play( "96000.avi", this, false, true, false );
			 	GameFlag.set( f2LeaveAttempts96000 );
			}
			//++leaveAttempts;
		}
		else if( attempt96000 == SHOOTBUTMISS )
		{
			//theMovie->play( "96201.avi", this );
			theMovie->fromTo( 1305, 1581 );
			theMovie->play( "96000.avi", this, false, true, false );
		}
		else if( attempt96000 == SHOOTANDHIT )
		{
			//theMovie->play( "96202.avi", this );
			theMovie->fromTo( 1800, 2169 );
			theMovie->play( "96000.avi", this, false, true, false );
		}
		else
		{
			//ASSERT( false, 0 );
			//sfxPrintf( "this woulda caused assert" );
			attempt96000 = NOATTEMPT;
		}
	END

	BEG
		ticks = 2;	// hmi pause
	END

	BEG
		sound1->stop();		// may or may not be playing
		theGame->handsOn();
		if( attempt96000 != NOATTEMPT  || GameFlag.test( f2LeaveAttempts96000 ) )
			theGame->newRoom( /*new Rm60000( END_DEAD )*/ new Rm60500 );
		else
			dispose();
	END
}

// 96300
void PEGUpWindowScript96300::changeState( int newState )
{
	PEGTriggerView96300*	trigger;

 	switchTo
		theGame->handsOff();
		sfxSystemCursor( SYS_ARROW );
		stopSound();	// you can't get back to room after this--don't restart sound
		//features->dispose();
		theMovie->fromTo( 960, 1079 );
		theMovie->play( "96000.avi", this, false, false, false );
	END

	BEG
		trigger = new PEGTriggerView96300;
		trigger->cue();
		dispose();
	END
}

void OutWindowScript96300::changeState( int newState )
{
 	switchTo
		theGame->handsOff();
		theMovie->fromTo( 1605, 1785 );
		theMovie->play( "96000.avi", this );
	END

	BEG
		theGame->handsOn();
		dispose();
	END
}

void ShootPresidentScript96300::changeState( int newState )
{
 	switchTo
		theMovie->fromTo( 765, 938 );
		theMovie->play( "96000.avi", this, false, false, false );
	END

	BEG
		attempt96000 = SHOOTANDHIT;
		curRoom->setScript( new ExitScript96200 );
	END
}

// 96700
// not used
void AssemblePEGScript96700::changeState( int newState )
{
	static Sound*	soundptr;

 	switchTo
		soundptr = new Sound;
		//soundptr->play( SND_ASSEMBLE_PEG, this );
		//sfxPrintf( "Assemble PEG sounds" );
		cue();//temp
	END

	BEG
		soundptr->dispose();
		GameFlag.set( fDoDissolve96000 );
		theGame->newRoom( new Rm96300 );
	END
}
 
} // namespace Spycraft 
