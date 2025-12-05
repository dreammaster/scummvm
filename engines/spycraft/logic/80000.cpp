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
#include "invent.h"
#include "movie.h"
#include "spycraft/logic/80000.h"
#include "spycraft/logic/1000.h"
#include "flag.h"												   
#include "vlink.h"
#include "roomsnd.h"
#include "advcurs.h"
//#include "timer.h"

namespace Spycraft {

/*********************
*
*		Room Code 
*
**********************/

#define CRSBLINK_SPEED	5
#define MAX_TOUCHES		2
#define BLAKEGONETIME	15

// these enums correspond to the lines in 80000.msg
enum{	WHAT_PRECAUTION1,
		WHAT_PRECAUTION2,
		YOU_KILL_M1,
		YOU_KILL_M2,
		THANK_YOU1,
		THANK_YOU2,
		WHO_RUN_P,
		WHY_HIDE_YELLOW
	};


extern Intrface* intrface;
extern NorthCursor* northCursor;
extern SouthCursor* southCursor;
extern BlinkCursor* normalCursor;

extern short	drawBackground;	// Travelink

extern DialTree* dialogTree;

static WaitingTimer80000*	waitingTimer;
static AnyElseObj80000*		anyElseObj;
static int	blakeReturning;	// flag: Blake footsteps playing
static int	firstTimeIn = TRUE;

Patio80000*	patio80000;

Dialog* 	BlakeDialog3;
Dialog* 	BlakeDialog4;
Dialog* 	BlakeDialog5;
Dialog* 	BlakeDialog6;
Dialog* 	BlakeDialog7;
Dialog* 	BlakeDialog8;

DItem*		whatPrecaution_3;	// What do you mean ...
DItem*		whatPrecaution_6;	// What did you mean ...
DItem*		youKillM_3_5;
DItem*		youKillM_4;
DItem*		thankYou_4_5_6;
DItem*		thankYou_3;
DItem*		whoRunP_6_4;
DItem*		whyHideYellow_5;

void	touchObject( void );	// local function

Rm80000::Rm80000()
{
	name = "Rm80000";
	show_style = VE_FLIP;
}

void Rm80000::init()
{
  	//drawPic( 80000 );
    Room::init();
	if( !blakeReturning && GameFlag.test( fBlakeGone ) )
	{						  
		new NorthExit80000;	// to mid view
  		//new SouthExit80000;	
		new Table80000; // back table
	}

  	intrface->show();
	theGame->handsOn();

	if( !GameFlag.test( fMetBlake ) )	{	// first time in: play intro movie w/o drawPic
		#pragma message( "Temp removed to cover hmi bug" )
			//sfxSystemCursor( SYS_WAIT );
			//	sfxLoadRes( SND_BLAKELEAVES, RES_WAVE );
		#if BKSOUND == ON
			sound1->stop();
		#endif
   		setScript( new IntroScript80000 ); // talk to Blake
		GameFlag.set( fMetBlake );
	}
	else   // (doing a new room in above case to draw pic)
	{
		if( GameFlag.test( fBlakeGone ) || blakeReturning )
		{
			if( !firstTimeIn )
			{
				sfxPurgeRes( 80000, RES_PIC );
				sfxPurgeRes( 80100, RES_PIC );
				sfxPurgeRes( 80200, RES_PIC );
			}
			firstTimeIn = FALSE;
			drawPic( 80000 );
		}
		if( !GameFlag.test( fDCK6/*fBlakePatioStarted*/ ) )	// setup Blake gone--snoop time
		{
			patio80000 = new Patio80000;  // handles the first return movie, and object touches
			theGame->setScript( new BlakeReturnsScript80000 ); // timer, talk to Blake
		} //note: delete patio on any exit from Blake's patio.
	}
}

Rm80100::Rm80100()
{
	name = "Rm80100";
	show_style = VE_FLIP;
}

void Rm80100::init()
{
  	drawPic( 80100 );
    Room::init();
  	new SouthExit80100;
	new Parker_CD80100;
	new Table80100; // back table

  	intrface->show();
  	theGame->handsOn();
}

Rm80200::Rm80200()
{
	name = "Rm80200";
	show_style = VE_FLIP;
}

void Rm80200::init()
{
  	drawPic( 80200 );
    Room::init();
  	new SouthExit80200;
	blakeReturning = FALSE;
	if( GameFlag.test( fBlakeGone ) )
	{
		new Powerbook80200;
		new Modem80200;
		new CDPlayer80200;
		//new Parker_CD80200;
		new Hound80200; // book
	}

  	intrface->show();
  	theGame->handsOn();
}

/*********************
*
*	Exit Features 
*
**********************/

NorthExit80000::NorthExit80000()
{
	name = "NorthExit80000";
	setHotspot( DO_V );
	init( NORTH_EXIT_X, NORTH_EXIT_Y, NORTH_EXIT_WIDTH, NORTH_EXIT_HEIGHT );  
}

void NorthExit80000::respond()
{
	theGame->setCursor( northCursor );
	aniCursor();
}

int NorthExit80000::doVerb ( int theVerb )
{
	if( theVerb == DO_V ) {
    	theGame->newRoom( new Rm80100 );
    	return TRUE;
	}
	return FALSE;
}

// not used
SouthExit80000::SouthExit80000()
{
	name = "SouthExit80000";
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
 	setHotspot ( DO_V );
}

// not used
void SouthExit80000::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

// not used
int SouthExit80000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		if( patio80000 != NULL ) {
			delete patio80000;
			patio80000 = NULL;
		}
		//theMovie->play("80018.avi", 0, 0, 640, 480, NULL);
		//sfxPrintf("See you Later (8a/80018) Best of luck...");
    	//sfxPrintf( "new room: 1000, the map" );
    	theGame->newRoom(new Rm1000);
    	return TRUE;
	}
	return FALSE;
}

SouthExit80100::SouthExit80100()
{
	name = "SouthExit80100";
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
 	setHotspot ( DO_V );
}

void SouthExit80100::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit80100::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		theGame->newRoom( new Rm80000 );
    	return TRUE;
	}
	return FALSE;
}

SouthExit80200::SouthExit80200()
{
	name = "SouthExit80200";
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
 	setHotspot ( DO_V );
}

void SouthExit80200::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit80200::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		theGame->newRoom( new Rm80000 );
    	return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Features
 *
/*******************************/

Blake80000::Blake80000()
{
 	name = "Blake80000";
	setHotspot(GUN_V);
	init(324, 36, 119, 150);
}
		
int Blake80000::doVerb ( int theVerb )
{
	if (theVerb == GUN_V)	{
		sfxPrintf("Ouch");
		return TRUE;
	}
	else if (theVerb == LINT_V)	{
		sfxPrintf( "Hmm...lint!" );
		return TRUE;
	}
	return FALSE;
}

Table80000::Table80000()
{
	name = "Patio Table"; // back table
	setHotspot ( DO_V );
	init ( 30, 140, 335, 110 );
}

void Table80000::respond()
{
	theGame->setCursor( northCursor );
}

int Table80000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom( new Rm80200 );
    	return TRUE;
	}
	return FALSE;
}

Table80100::Table80100()
{
	name = "Table80100"; // back table
	setHotspot ( DO_V );
	init ( 60, 65, 255, 100 );
}

void Table80100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Table80100::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom( new Rm80200 );
    	return TRUE;
	}
	return FALSE;
}

Parker_CD80100::Parker_CD80100()
{
	name = "CD: Charlie Parker";
	setHotspot ( DO_V );
	init ( 6, 175, 40, 60 );
}

void Parker_CD80100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Parker_CD80100::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	new Parker_CDInset80100;
		//GameFlag.set(fPlayerExaminingCD);
    	return TRUE;
	}
	return FALSE;
}

Powerbook80200::Powerbook80200()
{
	name = "Powerbook80200";
	setHotspot ( DO_V );
	init(301, 25, 185, 300);
}

void Powerbook80200::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Powerbook80200::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		new PowerbookInset80200;
		return TRUE;
	}
	return FALSE;
}

Modem80200::Modem80200()
{
	name = "Modem80200";
	setHotspot ( DO_V );
	init(206, 185, 70, 60);
}

void Modem80200::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Modem80200::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
	//note: fModemConnected used to be set and checked here (toggle)
		new ModemInset80200;
		return TRUE;
	}
	return FALSE;
}

CDPlayer80200::CDPlayer80200()
{
	name = "CDPlayer80200";
	setHotspot ( DO_V );
	init ( 6, 45, 185, 110 );
}

void CDPlayer80200::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int CDPlayer80200::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	new CDPlayerInset80200;
		//GameFlag.set( fPlayerExaminingCD );
    	return TRUE;
	}
	return FALSE;
}

Hound80200::Hound80200()
{
	name = "Hound of the Baskervilles";
	setHotspot ( DO_V );
	init ( 66, 200, 90, 90 );
}

void Hound80200::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Hound80200::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	new HoundInset80200;
		//GameFlag.set( fPlayerExaminingBook );
    	return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Insets
 *
 *******************************/

PowerbookInset80200::PowerbookInset80200()
{
	init( 80210, 0, 16, 512, 384 );
	normalCursor->rest();	// stops respond blink
	//GameFlag.set( fPlayerExaminingComputer );	
	patio80000->touchObject( COMPUTER80000 );
}

int PowerbookInset80200::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
		//sfxPrintf( "Power Book" );
		//Do something . . .
    	return TRUE;
	}
	return FALSE;
}

//void PowerbookInset80200::dispose( void )
//{
// 	GameFlag.clear( fPlayerExaminingComputer );
//	Inset::dispose();
//}
CDPlayerInset80200::CDPlayerInset80200()
{
	init( 80230, 0, 16, 512, 384 );
	normalCursor->rest();	// stops respond blink
}

int CDPlayerInset80200::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
		//Do something . . .
    	return TRUE;
	}
	return FALSE;
}

ModemInset80200::ModemInset80200()
{
	init( 80220, 0, 16, 512, 384 );
	normalCursor->rest();	// stops respond blink
}

int ModemInset80200::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
		//Do something . . .
    	return TRUE;
	}
	return FALSE;
}

Parker_CDInset80100::Parker_CDInset80100()
{
	init(80240, 0, 16, 512, 384 );
	normalCursor->rest();	// stops respond blink
	//GameFlag.set( fPlayerExaminingCD );
	patio80000->touchObject( CD80000 );
}

int Parker_CDInset80100::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	return TRUE;		   
    }
	return FALSE;
}

//void Parker_CDInset80100::dispose( void )
//{
// 	GameFlag.clear( fPlayerExaminingCD );
//	Inset::dispose();
//}

HoundInset80200::HoundInset80200()
{
	init(80250, 0, 16, 512, 384);
	normalCursor->rest();	// stops respond blink
	//GameFlag.set( fPlayerExaminingBook );
	patio80000->touchObject( BOOK80000 );
}

void HoundInset80200::respond()
{
}

int HoundInset80200::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		// nothing
    	return TRUE;		   
    }
	return FALSE;
}

//void HoundInset80200::dispose( void )
//{
// 	GameFlag.clear( fPlayerExaminingBook );
//	Inset::dispose();
//}

/*******************************
 *
 *	Scripts
 *
 *******************************/

void IntroScript80000::changeState ( int newState )
{
	switchTo
		theGame->handsOff();
		theMovie->fromTo( 0, 717 );
		theMovie->play( "80010.avi", this, FALSE, TRUE, FALSE );
	END
   
    BEG	
 		GameFlag.set( fBlakeGone ); 
		theGame->newRoom( new Rm80000 );
	END
}

void BlakeReturnsScript80000::changeState( int newState )
{
	char *rmName;

	switchTo
		soundptr = new Sound;
		soundptr->play( SND_BLAKELEAVES, this );
		#if BKSOUND == ON
			sound1->loop = SND_LOOP_INFINITE;
			sound1->playBuffered( 80000, NULL );
		#endif	
	END

	BEG
		if( pass == 0 )
 			seconds = BLAKEGONETIME;	// time out--Blake returns
		else
			seconds = 4;
	END

	BEG
		if( currentFocus == PUZZLE_FOCUS  || currentFocus == DIALOG_FOCUS )	// PDA is up
		{
			seconds = 0;
			state = 0;		// starts at state 1, skipping zero
			client->script = NULL;
			client->setScript( this, caller );	// reset	
		}
		else
		{
			seconds = 0;	// in case cued externally
			GameFlag.clear( fBlakeGone );
			soundptr->play( SND_BLAKERETURNS, this );
			blakeReturning = TRUE;
			intrface->disable();
		}
	END

	BEG
		soundptr->dispose();
		blakeReturning = FALSE;
		intrface->enable();
		/* make sure still at Blake's house  (can't leave anymore) */
		rmName = curRoom->name;
		if( strcmp( rmName, "Rm80000" ) && strcmp( rmName, "Rm80100" )
		    && strcmp( rmName, "Rm80200" ) )	 // if all three different
		{
			dispose();	// never mind, player left patio.
		}
		else
		{
			theGame->handsOff();
			#if BKSOUND == ON
				sound1->stop(); 	// currently no midi for this room
			#endif
			ticks = 2;	// hmi kludge--else movie restarts sound
		}
	END
		
	BEG	
		if( theInset != NULL)
			theInset->dispose(); // we were looking at something
		else
			patio80000->objectTouched = NO_OBJECT80000;	// not looking at anything
		rmName = curRoom->name;
		if( strcmp( rmName, "Rm80000" )!= 0 )
			theGame->newRoom( new Rm80000 );
		curRoom->setScript( new BlakeDialogScript80000 );
		dispose();
	END	   

	BEG
		assert( FALSE );  // debug--just in case dispose, then cue.  possible? let's see.
	END
}

void BlakeDialogScript80000::changeState( int newState )
{
	sfxLoadRes( 960, RES_ATS );
	sfxLockRes( 960, RES_ATS );
 	switchTo		
		switch( patio80000->objectTouched )
		{
		 case COMPUTER80000:
			//sfxPrintf( "movie: Blake returns <589> computer", this );
			theMovie->fromTo( 720, 974 );  // computer, #2
			break;
		 case CD80000:
			//sfxPrintf( "movie: Blake returns <590> CD", this );
			theMovie->fromTo( 2910, 3242 );	// cd, #10
			break;
		 case BOOK80000:
			//sfxPrintf( "movie: Blake returns <591> book", this );
			theMovie->fromTo( 2580, 2899 );	 // book, #9
			break;
		 default:	// player picking nose, or not looking at anything now
			NULL;
		}
		user->canInput(FALSE);
		if( patio80000->objectTouched != NO_OBJECT80000 )
			theMovie->play( "80010.avi", this, FALSE, FALSE, FALSE );
		else
			cue();
	END

	BEG
		theMovie->fromTo( 990, 1050 -1 ); // here's file, #3 
		theMovie->play( "80010.avi", this, FALSE, FALSE, FALSE );
		inventry->get( iPROCATFILE ); // Blake gives you yellow pages
	END

	BEG
		inventry->items[iPROCATFILE]->activate();
		theMovie->fromTo( 1050, 1479 ); // here's file, #3 cont 
		theMovie->play( "80010.avi", this, FALSE, FALSE, FALSE );	
	END
	
	BEG	
		//patio80000->cue();
		// DIALOG
		user->canInput(TRUE);
		anyElseObj = new AnyElseObj80000;		
		dialogTree = new DialTree;
		BlakeDialog3 = new BlakePatioDialog80000;
		BlakeDialog4 = new BlakePatioDialog80000;
		BlakeDialog5 = new BlakePatioDialog80000;
 		BlakeDialog6 = new BlakePatioDialog80000;
 		BlakeDialog7 = new BlakePatioDialog80000;
		BlakeDialog8 = new BlakePatioDialog80000;

		whatPrecaution_3	= new WhatPrecaution_3; // What do you mean ...
		whatPrecaution_3->addInfo(	80000, WHAT_PRECAUTION1, BlakeDialog4 );

		whatPrecaution_6	= new WhatPrecaution_6; // What did you mean ...
		whatPrecaution_6->addInfo(	80000, WHAT_PRECAUTION2, BlakeDialog4 );

		youKillM_3_5	= new YouKillM_3_5; // Did you kill ...
		youKillM_3_5->addInfo(			80000, YOU_KILL_M1, BlakeDialog6 );

		youKillM_4		= new YouKillM_4; // But, didn't you kill ...
		youKillM_4->addInfo(		80000, YOU_KILL_M2, BlakeDialog6 );

		thankYou_4_5_6	= new BlakePatioDItem80000; // ...I appreciate your help.
		thankYou_4_5_6->addInfo(			80000, THANK_YOU1, BlakeDialog7 );

		thankYou_3		= new BlakePatioDItem80000; // ...I appreciate all your help.
		thankYou_3->addInfo(			80000, THANK_YOU2, BlakeDialog7 );

		whoRunP_6_4		= new BlakePatioDItem80000; // So, who do you think is running Procat?
		whoRunP_6_4->addInfo(			80000, WHO_RUN_P, BlakeDialog5 );

		whyHideYellow_5	= new BlakePatioDItem80000; // Why do you take the precaution to hide...
		whyHideYellow_5->addInfo(		80000, WHY_HIDE_YELLOW, BlakeDialog4 );

		BlakeDialog3->addMovieInfo( 990,	1479, 0, 0);   // used?
		BlakeDialog4->addMovieInfo( 1485,	1790, 0, 0);
		BlakeDialog5->addMovieInfo( 1800,	2153, 0, 0);
 		BlakeDialog6->addMovieInfo( 2160,	2429, 0, 0);
 		BlakeDialog7->addMovieInfo( 2445,	2515, 0, 0);
		BlakeDialog8->addMovieInfo( 2520,	2577, 0, 0);	// used?

		BlakeDialog3->add(whatPrecaution_3);
		BlakeDialog3->add(youKillM_3_5);
		BlakeDialog3->add(thankYou_3);

		BlakeDialog4->add(youKillM_4);
		BlakeDialog4->add(whoRunP_6_4);
		BlakeDialog4->add(thankYou_4_5_6);
		
		BlakeDialog5->add(youKillM_3_5);
		BlakeDialog5->add(thankYou_4_5_6);
		BlakeDialog5->add(whyHideYellow_5);
 		
 		BlakeDialog6->add(whatPrecaution_6);
		BlakeDialog6->add(whoRunP_6_4);
		BlakeDialog6->add(thankYou_4_5_6);

		// set up timer, normally done in dialog cue()
		waitingTimer = new WaitingTimer80000;
		waitingTimer->setReal( 10, anyElseObj );
		permList->add( waitingTimer );// this is where the timer will get its pulses.
		
		dialogTree->rootNode = BlakeDialog3;
		dialogTree->resourceID = "80010.avi";
		dialogTree->show(this);	
	END

	BEG
		ticks = 2; // I'm not sure why I'm doing this --dck
	END

	BEG
		theMovie->caller = NULL;
		theMovie->stop();
		intrface->enable();	// movie won't restore since played w/o close requested
		BlakeDialog3->dispose();
		BlakeDialog4->dispose();
		BlakeDialog5->dispose();
		BlakeDialog6->dispose();
		BlakeDialog7->dispose();
		BlakeDialog8->dispose();
		if( patio80000 != NULL )
			patio80000->dispose;
		anyElseObj->dispose();
		sfxUnlockRes( 960, RES_ATS );
		ASSERT( waitingTimer == NULL, 0 );
		GameFlag.set( fBlakePatioTalkDone );
		inventry->clearCurrentItem();
		GameFlag.clear( fLondonAvailable );
		new EMail( "Trust no one: Colby, William", 98110, 0, 4 );
		drawBackground = FALSE;
		theGame->newRoom( new Rm1000 );
	END
}

/*******************************
 *
 *	Dialogs
 *
 *******************************/
 
 void BlakePatioDialog80000::cue( void )
 {
 	Node*	curNode = head;
	int		foundOne; // flag

	// see if there's a DItem up
	foundOne = FALSE;
 	while (curNode)
 	{
		if( !((DItem*)(curNode->data))->hasBeenChosen )
		{
			foundOne = TRUE;
			break;
		}
		curNode = curNode->next;
	}
	//set timer to hang up (if no DItem then we're on the last segment and don't want a timer)
	if( foundOne )
	{
		ASSERT( waitingTimer == NULL, 0 );
		waitingTimer = new WaitingTimer80000;
		waitingTimer->setReal( 10, anyElseObj );
		permList->add( waitingTimer );// this is where the timer will get its pulses.	
	}

 	Dialog::cue();	
 }

 /*******************************
 *
 *	DItems
 *
 *******************************/

 void BlakePatioDItem80000::doit( void )
 {
	if( waitingTimer != NULL )
	{
		//permList->release( waitingTimer );
	 	//waitingTimer->pause( TRUE );
		//waitingTimer->dispose();
		delete waitingTimer;
		waitingTimer = NULL;
	}
 	DItem::doit();
 }

/*******************************
 *
 *	Dialog DItem Doits
 *
 *******************************/

void WhatPrecaution_3::doit()
{
  	whatPrecaution_6->hasBeenChosen = TRUE;
	whyHideYellow_5->hasBeenChosen = TRUE;
	BlakePatioDItem80000::doit();
}

void WhatPrecaution_6::doit()
{
  	whatPrecaution_3->hasBeenChosen = TRUE;
	whyHideYellow_5->hasBeenChosen = TRUE;
	BlakePatioDItem80000::doit();
}
 
void YouKillM_3_5::doit()
{
  	youKillM_4->hasBeenChosen = TRUE;
	BlakePatioDItem80000::doit();
}

void YouKillM_4::doit()
{
  	youKillM_3_5->hasBeenChosen = TRUE;
	BlakePatioDItem80000::doit();
}

/*******************************
 *
 *	Patio
 *
 *******************************/

Patio80000::Patio80000()
{
 	name = "Patio80000";
	GameFlag.set( fDCK6/*fBlakePatioStarted*/ );
	objectTouched = NO_OBJECT80000;
	objectTouchCount = 0;
};

Patio80000::~Patio80000()
{
	GameFlag.clear( fDCK6/*fBlakePatioStarted*/ );
	patio80000 = NULL;
}
 
void	Patio80000::touchObject( int whichObject )
{
 // looked at object in one of the rooms, probably all in 80200  	
	if( GameFlag.test( fBlakeGone ) )
	{
		++objectTouchCount;
 		objectTouched = whichObject;	// remembers last one	
		if( objectTouchCount == MAX_TOUCHES  && GameFlag.test( fBlakeGone ) )
		{
			assert( theGame->script != NULL );
			theGame->script->cue();
		}
	}
}

/*******************************
 *
 *	Misc
 *
 *******************************/

 WaitingTimer80000::~WaitingTimer80000()
 {
  	permList->release( this );
 }

 void AnyElseObj80000::cue( void )
 {
 	static int	firstPass = TRUE; // first cue from timer, second from this movie

	if( firstPass )
	{
		user->canInput( FALSE );
		firstPass = FALSE;
		//permList->release( waitingTimer );
		waitingTimer = NULL;
  		theMovie->fromTo( 2520, 2577 );	// "Anything else?"
		theMovie->play( "80010.avi", this, FALSE, FALSE, FALSE );
	}
	else
	{
		user->canInput( TRUE );
		firstPass = TRUE;
	}
 }
 
} // namespace Spycraft 
