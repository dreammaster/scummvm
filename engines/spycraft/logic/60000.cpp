/*********************************************
* Cpp source file.
* Start menu and death screen.
* Copyright Activision 1995
* Started by Ka Wah 
* Continued by:  Daniel Kinzek   November, 1995
**********************************************/

#include "globals.h"
#include "game.h"
#include "verbs.h"
#include "views.h"
#include "movie.h"
#include "60000.h"
#include "flag.h"
#include "roomsnd.h"
#include "advfile.h"
#include "save.h"								  	
#include "1270.h"
#include "gamebox.h"

#define CRSBLINK_SPEED 5

extern Intrface*	intrface;
extern SouthCursor*	southCursor;
extern BlinkCursor*	normalCursor;

extern int quitting;

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

/*********************
*
*		Room Code 
*
**********************/

/* DEATH SCREEN */
Rm60000::Rm60000()
{
	name = "Rm60000";
	show_style = VE_FLIP;
	theEnd = END_NONE;
}

Rm60000::Rm60000( int endType )
{
	name = "Rm60000";
	show_style = VE_FLIP;
	theEnd = endType;		// one of enum
}

Rm60000::~Rm60000()
{
	intrface->enable();
}

void Rm60000::init()
{
	intrface->disable();

	switch( theEnd )
	{
	 case END_JAIL:
	 	drawPic( -1 );
		Room::init();
		setScript( new JailScript60000 );
		break;
	 case END_BLACKDEAD:
	 	drawPic( -1 );
		// continue to next case
	 case END_DEAD:
		Room::init();
		setScript( new DeadScript60000 );
		break;
	 case END_FIRED:
		//drawPic( 60100 );
		Room::init();
		setScript( new FiredScript60000 );
		break;
	 case END_GAME:
	 	Room::init();
		setScript( new EndGameScript60000 );
		break;
	 case END_NONE:
		drawPic( 60000 );
		Room::init();
		#ifdef _DEBUG
			sfxPrintf( "No end type set" );
		#endif
		theGame->handsOn();
		new RestoreView60000;
		new QuitView60000;
		break;
	 default:
	 	drawPic( 60000 );
		Room::init();
		theGame->handsOn();
		#ifdef _DEBUG
			sfxPrintf( "Unknown end type set" );
		#endif
	}

}

/* STARTUP MENU SCREEN */

Rm60500::Rm60500()
{
	name = "Rm60500";
	show_style = VE_FLIP;
}

Rm60500::~Rm60500()
{
	intrface->enable();
}

void Rm60500::init()
{
	drawPic( 60500 );
	Room::init();
	stopSound();
	theGame->handsOn();
	sfxEnableSaveGame( FALSE );
	
	if( !GameFlag.test( fInitialBriefing ) )
		new PlayBt60500;
	new RestoreBt60500;
	new QuitBt60500;

	sfxLoadRes(904, RES_ATS);
	sfxLockRes(904, RES_ATS);
	sfxLoadRes(960, RES_ATS);
	sfxLockRes(960, RES_ATS);
	sfxLoadRes(40002, RES_WAVE);
	sfxLockRes(40002, RES_WAVE);
	sfxLoadRes(103, RES_WAVE);
	sfxLockRes(103, RES_WAVE);
	sfxLoadRes(136, RES_WAVE);
	sfxLockRes(136, RES_WAVE);

	intrface->disable();
}

/*******************************
 *
 *	Views
 *
/*******************************/

ButtonView60000::ButtonView60000()
{
	name = "ButtonView60000";
}

int ButtonView60000::handleEvent( MADEEventStamp * event )
{
	if ( user->input && onMe( event ) )
	{
		if ( event_type == USER_LEFT_DOWN )
		{
			if( cel != 1 )
				setCel( 1 );
		}
		else if ( event_type == USER_LEFT_UP )
		{
			if( cel != 0 );
				setCel( 0 );
			activate();		
		}
		return TRUE;
	}
	return FALSE;
}

// 60000

// not used
RestoreView60000::RestoreView60000()
{
	name = "RestoreView60000";
	setHotspot( DO_V );
	setView( 60001 );
	init ( 200, 250 );
	noSkip();
	normalCursor->rest();	// stops respond blink
}

// not used
void RestoreView60000::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

// not used
void RestoreView60000::activate( void )
{
	char* tmpSave;

		setCel(1);
		init( 200, 250 );

		sfxSystemCursor(SYS_ARROW);
		OpenDlg ("sgm");
		tmpSave = sfxGetRestoreFile();
		if (strcmp(tmpSave, ""))	
			theGame->newRoom(new RestRoom(tmpSave));
		if (strcmp(curRoom->name, "Rm91000"))
			sfxMadeCursor();
}

// not used
QuitView60000::QuitView60000()
{
	name = "QuitView60000";
	setHotspot( DO_V );
	setView( 60002 );
	init ( 200, 315 );
	noSkip();
	normalCursor->rest();	// stops respond blink
}

// not used
void QuitView60000::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

// not used
void QuitView60000::activate( void )
{
	setCel(1);
	init( 200, 315 );
	quitting = TRUE;
	sfxQuit();
			
}

// 60500

PlayBt60500::PlayBt60500()
{
	name = "PlayBt60500";
	setHotspot( DO_V );
	setView( 60501 );
	init ( 187, 321 );
	normalCursor->rest();	// stops respond blink
}

void PlayBt60500::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

void PlayBt60500::activate( void )
{
	if (!GameFlag.test(fInitialBriefing))
		theGame->newRoom( new Rm1270 );		
}

RestoreBt60500::RestoreBt60500()
{
	name = "RestoreBt60500";
	setHotspot( DO_V );
	setView( 60502 );
	init ( 28, 325 );
	noSkip();
	normalCursor->rest();	// stops respond blink
}

void RestoreBt60500::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

void RestoreBt60500::activate( void )
{
	char* tmpSave;

	sfxSystemCursor(SYS_ARROW);
	OpenDlg ("sgm");
	tmpSave = sfxGetRestoreFile();
	if (strcmp(tmpSave, ""))	
		theGame->newRoom(new RestRoom(tmpSave));
	if (strcmp(curRoom->name, "Rm91000"))
		sfxMadeCursor();
}

QuitBt60500::QuitBt60500()
{
	name = "QuitBt60500";
	setHotspot( DO_V );
	setView( 60503 );
	init ( 369, 325 );
	noSkip();
	normalCursor->rest();	// stops respond blink
}

void QuitBt60500::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

void QuitBt60500::activate( void )
{
	quitting = TRUE;
	sfxQuit();
}

/*******************************
 *
 *	Scripts
 *
/*******************************/

void JailScript60000::changeState( int newState )
{
	switchTo
		theGame->handsOff();
		ticks = 120;
	END
	BEG
 		theMovie->play( "60000.avi", this, FALSE, TRUE, FALSE );
	END

	BEG
		theGame->newRoom( new Rm60500 );
	END
}

void FiredScript60000::changeState( int newState )
{
	switchTo
		theGame->handsOff();
		ticks = 120;	// allows for dispose
	END

	BEG
		theMovie->play( "60100.avi", this, FALSE, TRUE, FALSE );
	END

	BEG
		theGame->newRoom( new Rm60500 );
	END
}

void DeadScript60000::changeState( int newState )
{
	switchTo
		theGame->handsOff();
		ticks = 120;
	END

	BEG
		theMovie->play( "60050.avi", this, FALSE, TRUE, FALSE );
	END

	BEG
		theGame->newRoom( new Rm60500 );
	END
}

void EndGameScript60000::changeState( int newState )
{
	switchTo
		theGame->handsOff();
		ticks = 120;
	END
	BEG
		// Warhurst caught news
		theMovie->fromTo( 60, 629 );
		theMovie->play( "60010.avi", this, 	FALSE, FALSE, FALSE );
	END

	BEG
		// Goto Langley seque
		theMovie->fromTo( 0, 56 );
		theMovie->play( "60010.avi", this, FALSE, FALSE, FALSE );
	END

	BEG
		// Sterling
		if( GameFlag.test( fEndgameYuriShot ) )
		{
			theMovie->fromTo( 645, 1211 );
			theMovie->play("60010.avi", this, FALSE, FALSE, FALSE );		// medal
		}
		else
		{
			theMovie->fromTo( 1275, 2094 );
			theMovie->play("60010.avi", this, FALSE, FALSE, FALSE );		// fired
		}
	END

	BEG
		// News
		if( GameFlag.test( fEndgameYuriShot ) )
		{
			theMovie->fromTo( 2130, 2660 );
			theMovie->play("60010.avi", this, FALSE, FALSE, FALSE );		// Churby wins election
		}
		else
		{
			theMovie->fromTo( 2670, 3111 );
			theMovie->play("60010.avi", this, FALSE, FALSE, FALSE );		// Chaos in Russia
		}
	END
	
	BEG
		// Washington gets NUKED 
		if( !GameFlag.test( fEndgamePitStopped ) )
		{
			theMovie->fromTo( 3180, 3952 );
			theMovie->play("60010.avi", this, FALSE, TRUE, FALSE );		// Sterling: Oh my God!
		}
		else
		{
			theMovie->caller = NULL;
			theMovie->stop();
			//intrface->enable();
			cue();
		}
	END

	BEG
		// credits
		theMovie->play("200.avi", this, FALSE, 2, FALSE );
	END

	BEG
		ticks = 1;		// let stack unwind before new room
	END

	BEG
		theGame->newRoom( new Rm60500 );
	END
}

 
