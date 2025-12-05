/*********************************************
* Cpp source file.
* Dacha.
* Rooms: 79000        
* Copyright Activision 1995
* Started by Tom DeSalvo 
* Continued by:  Daniel Kinzek   August, 1995
**********************************************/

#include "globals.h"
#include "verbs.h"
#include "views.h"
#include "flag.h"
#include "movie.h"
#include "roomsnd.h"
#include "79000.h"
#include "1000.h"
#include "18000.h"
//#include "1085.h"
#include "60000.h"

#define CANSHOOTW  973
#define	CANDROPGUN 1155

#define GUN_X79000  256
#define GUN_Y79000  400
#define GENERICGUN_X79000	432

enum{	DROPGUNLOOP,
		SHOOTGUNLOOP,
		GETGUNLOOP,
		SHOOTWARHLOOP,
		SHOOTYURILOOP,
	};

// these enums correspond to the lines in 79000.msg
enum{	WHY_TURN_TRAITOR,
		THATS_WHY_TEAM,
		ONE_QUESTION1,
		ONE_QUESTION2,
		THATS_WHY_HACKED,
		NEWS_FLASH,
		SHOOT_WARHURST,		// should not be used anymore
		DROP_GUN,			// should not be used anymore
		GET_GUN,
		SIT_LIKE_PUD,
		SHOOT_YURI,			// should not be used anymore
		BLANK
	};

enum{	PLAYED_NONE, PLAYED_FIRST, PLAYED_SECOND }; // values for playedLoop of DachaDialog


/*********************
*
*		Room Code 
*
**********************/

extern DialTree* dialogTree;

extern Intrface* intrface;

extern SouthCursor* southCursor;

static int	warhurstShootsYou;	// flag
static int	newsFlash;			// flag


// Dialog pointers
DachaD1_79000* 		DachaDialog1;
DachaD2_79000* 		DachaDialog2;  // exit
DachaD3_79000* 		DachaDialog3;
Dialog* 		DachaDialog4;
Dialog* 		DachaDialog5;
Dialog* 		DachaDialog6;
Dialog* 		DachaDialog7;
DachaD8_79000* 		DachaDialog8;
DachaD9_79000* 		DachaDialog9;
DachaD10_79000* 	DachaDialog10;
DachaD14_79000* 	DachaDialog14;
DachaD15_79000* 	DachaDialog15;	// exit
Dialog* 		DachaDialog16;
Dialog*			DachaDialog17;
DachaD18_79000*		DachaDialog18;
DachaD19_79000*		DachaDialog19;
DachaD20_79000*		DachaDialog20;
DachaD21_79000*		DachaDialog21;
DachaD22_79000*		DachaDialog22;

DachaDExit_79000*	DachaDialogExit;  // exit

// DItem pointers

DItem*	whyTurnTraitor_16_17;
DItem*	oneMoreQuestion_16;
DItem*	thatsWhyTeam_17;
DItem*	oneMoreQuestion_4;
DItem*	thatsWhyTeam_4_5;
DItem*	thatsWhyTeam_6;
DItem*	whyTurnTraitor_4_5;
DItem*	whyTurnTraitor_7;
DItem*	thatsWhyHacked_6_7;
NewsFlash_16_79000*	newsFlash_16;
CutDItem*		shootWarhurst_1;
CutDItem*		dropGun_1;
CutDItem*		getGun_9;
GrabShootDItem*		getGun_10;
CutDItem*		getGun_Generic4;
CutDItem*		getGun_Generic5;
CutDItem*		getGun_Generic6;
CutDItem*		getGun_Generic7;
CutDItem*		getGun_Generic16;
CutDItem*		getGun_Generic17;
CutDItem*		shootYuri_14;
DItem*		goTo2;
DItem*		goTo9;
DItem*		goTo10;
DItem*		goTo20;
DItem*		goToExit;


Rm79000::Rm79000()
{
	name = "Rm79000";
	show_style = VE_FLIP;
}

void Rm79000::init()
{
	//drawPic( 79000 );
    Room::init();
  	//new(SouthExit79000);

	#if BKSOUND == ON
		sound1->stop();
	#endif

	sfxSystemCursor( SYS_WAIT );
	sfxLoadRes( 911, RES_ATS );
	sfxLockRes( 911, RES_ATS );

  	intrface->show();
  	theGame->handsOn();
	// set flags for to support playing multiple times
	warhurstShootsYou = FALSE;	
	newsFlash = FALSE;			
	curRoom->setScript( new Script79000 );
}

int Rm79000::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}

SouthExit79000::SouthExit79000()
{
	name = "SouthExit79000";
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
	setHotspot ( DO_V );
	//init (46, 460, 495, 20);
}

void SouthExit79000::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit79000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm1000);
    	return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Scripts
 *
 *******************************/
void Script79000::changeState ( int newState )
{
	switchTo
		theMovie->fromTo( 0, CANSHOOTW -1 );
		theMovie->play( "79000.avi", this, FALSE, FALSE, FALSE );
	END

	BEG	
		dialogTree = new DialTree;

		DachaDialog1  = new DachaD1_79000;	// Warhurst enters, you drop gun
		DachaDialog2  = new DachaD2_79000;
		DachaDialog3  = new DachaD3_79000;
		DachaDialog4  = new Dialog;
		DachaDialog5  = new Dialog;
		DachaDialog6  = new Dialog;
		DachaDialog7  = new Dialog;
		DachaDialog8  = new DachaD8_79000;
		DachaDialog9  = new DachaD9_79000;
		DachaDialog10 = new DachaD10_79000; // Startrek shuffle, shoot option appears during.
		DachaDialog14 = new DachaD14_79000;	// Shoot W, shoot Y, spsd to be too late phase too. 
		DachaDialog15 = new DachaD15_79000;
		DachaDialog16 = new Dialog;
		DachaDialog17 = new Dialog;
		DachaDialog18 = new DachaD18_79000;
		DachaDialog19 = new DachaD19_79000;
		DachaDialog20 = new DachaD20_79000;
		DachaDialog21 = new DachaD21_79000;
		DachaDialog22 = new DachaD22_79000;

		DachaDialogExit = new DachaDExit_79000;
		

		dropGun_1 = new CutDItem;
		dropGun_1->addInfo(	911, DROPGUNLOOP, 0, GUN_X79000 -80, GUN_Y79000, DachaDialog16 );
	
		newsFlash_16 = new NewsFlash_16_79000;
		newsFlash_16->addInfo(	79000, NEWS_FLASH, DachaDialog4 );

		whyTurnTraitor_16_17 = new DItem;
		whyTurnTraitor_16_17->addInfo(	79000, WHY_TURN_TRAITOR, DachaDialog18 );
		
		oneMoreQuestion_16 = new DItem; // ...one question...
		oneMoreQuestion_16->addInfo(	79000, ONE_QUESTION1, DachaDialog17 ); // ...one question...

		thatsWhyTeam_17 = new DItem;
		thatsWhyTeam_17->addInfo(	79000, THATS_WHY_TEAM, DachaDialog19 );
		
		shootWarhurst_1 = new CutDItem;
		shootWarhurst_1->addInfo( 911, SHOOTWARHLOOP, 0, GUN_X79000, GUN_Y79000, DachaDialog2 );

		oneMoreQuestion_4 = new DItem; // ...one question...
		oneMoreQuestion_4->addInfo(	79000, ONE_QUESTION1, DachaDialog5 ); // ...one question...

		thatsWhyTeam_4_5 = new DItem;
		thatsWhyTeam_4_5->addInfo(	79000, THATS_WHY_TEAM, DachaDialog7 );
		
		thatsWhyTeam_6 = new DItem;
		thatsWhyTeam_6->addInfo(	79000, THATS_WHY_TEAM, DachaDialog22 );
		
		whyTurnTraitor_4_5 = new DItem;
		whyTurnTraitor_4_5->addInfo(	79000, WHY_TURN_TRAITOR, DachaDialog6 );
		
		whyTurnTraitor_7 = new DItem;
		whyTurnTraitor_7->addInfo(	79000, WHY_TURN_TRAITOR, DachaDialog21 );
		
		thatsWhyHacked_6_7 = new DItem;
		thatsWhyHacked_6_7->addInfo(	79000, THATS_WHY_HACKED, DachaDialog8 );

		getGun_9 = new CutDItem;
		getGun_9->addInfo( 911, GETGUNLOOP, 0, GUN_X79000, GUN_Y79000, DachaDialog2 );

		getGun_10 = new GrabShootDItem( SHOOTWARHLOOP );
		getGun_10->addInfo( 911, GETGUNLOOP, 0, GUN_X79000 +5, GUN_Y79000, DachaDialog14 );

		getGun_Generic4 = new CutDItem;
		getGun_Generic4->addInfo( 911, GETGUNLOOP, 0, GENERICGUN_X79000, GUN_Y79000, DachaDialog2 );

		getGun_Generic5 = new CutDItem;
		getGun_Generic5->addInfo( 911, GETGUNLOOP, 0, GENERICGUN_X79000, GUN_Y79000, DachaDialog2 );

		getGun_Generic6 = new CutDItem;
		getGun_Generic6->addInfo( 911, GETGUNLOOP, 0, GENERICGUN_X79000, GUN_Y79000, DachaDialog2 );

		getGun_Generic7 = new CutDItem;
		getGun_Generic7->addInfo( 911, GETGUNLOOP, 0, GENERICGUN_X79000, GUN_Y79000, DachaDialog2 );

		getGun_Generic16 = new CutDItem;
		getGun_Generic16->addInfo( 911, GETGUNLOOP, 0, GENERICGUN_X79000, GUN_Y79000, DachaDialog2 );

		getGun_Generic17 = new CutDItem;
		getGun_Generic17->addInfo( 911, GETGUNLOOP, 0, GENERICGUN_X79000, GUN_Y79000, DachaDialog2 );

		shootYuri_14 = new CutDItem;
		shootYuri_14->addInfo( 911, SHOOTYURILOOP, 0, GUN_X79000+15, GUN_Y79000, DachaDialog15 );

		
		goTo2 = new DItem;
		goTo2->addInfo(	79000, BLANK, DachaDialog2 );

		goTo9 = new DItem;
		goTo9->addInfo(	79000, BLANK, DachaDialog9 );

		goTo10 = new DItem;
		goTo10->addInfo(	79000, BLANK, DachaDialog10 );

		goTo20 = new DItem;
		goTo20->addInfo(	79000, BLANK, DachaDialog20 );

		goToExit = new DItem;
		goToExit->addInfo(	79000, BLANK, DachaDialogExit );

		DachaDialog1->addMovieInfo( CANSHOOTW, CANDROPGUN -1, CANDROPGUN, 1247 );
		DachaDialog2->addMovieInfo( 1275, 1363, 0, 0 );	 // exit
		DachaDialog3->addMovieInfo( 1380, 1433, 0, 0 );	 // not used: put PDA away
		DachaDialog4->addMovieInfo( 1455, 1926, 0, 0 );
		DachaDialog5->addMovieInfo( 1965, 2074, 0, 0 );
		DachaDialog6->addMovieInfo( 2100, 2433, 0, 0 );
		DachaDialog7->addMovieInfo( 2445, 2648, 0, 0 );
		DachaDialog8->addMovieInfo( 2655, 2729, 0, 0 );
		DachaDialog9->addMovieInfo( 2910, 2984, 2985,  3109 );	// may shoot W
		DachaDialog10->addMovieInfo( 2730, 2744, 2745, 2897 );
		DachaDialog14->addMovieInfo( 3120, 3566, 3567, 3804 ); // W shot, shoot Y, Y lives
		DachaDialog15->addMovieInfo( 3810, 3965, 0, 0 ); // exit
		DachaDialog16->addMovieInfo( 3975, 4156, 0, 0 );
		DachaDialog17->addMovieInfo( 4170, 4280, 0, 0 );
		DachaDialog18->addMovieInfo( 4290, 4598, 0, 0 );
		DachaDialog19->addMovieInfo( 4605, 4848, 0, 0 );
		DachaDialog20->addMovieInfo( 4860, 5058, 0, 0 );
		DachaDialog21->addMovieInfo( 4290, 4598, 0, 0 );
		DachaDialog22->addMovieInfo( 4605, 4848, 0, 0 );
		
		DachaDialogExit->addMovieInfo( /*3795*/2649, /*3797*/2654, 0, 0 );	// (can we get rid of this thing?)

		DachaDialog1->add( shootWarhurst_1 );
		//DachaDialog1->add( dropGun_1 ); // added in dialog

		DachaDialog4->add( whyTurnTraitor_4_5 );
		DachaDialog4->add( thatsWhyTeam_4_5 );
		DachaDialog4->add( oneMoreQuestion_4 );
		DachaDialog4->add( getGun_Generic4 );

	  
		DachaDialog5->add( whyTurnTraitor_4_5 );
		DachaDialog5->add( thatsWhyTeam_4_5 );
		DachaDialog5->add( getGun_Generic5 );

		DachaDialog6->add( thatsWhyHacked_6_7 );
		DachaDialog6->add( thatsWhyTeam_6 );
		DachaDialog6->add( getGun_Generic6 );

		DachaDialog7->add( thatsWhyHacked_6_7 );
		DachaDialog7->add( whyTurnTraitor_7 );
		DachaDialog7->add( getGun_Generic7 );

		DachaDialog9->add( getGun_9 );

		DachaDialog10->add( getGun_10 );

		DachaDialog14->add(	shootYuri_14 );

		DachaDialog16->add( whyTurnTraitor_16_17 );
		DachaDialog16->add( newsFlash_16 );
		DachaDialog16->add( oneMoreQuestion_16 );
		DachaDialog16->add( getGun_Generic16 );

		DachaDialog17->add( whyTurnTraitor_16_17 );
		DachaDialog17->add( thatsWhyTeam_17 );
		DachaDialog17->add( getGun_Generic17 );


		dialogTree->rootNode = DachaDialog1;
		dialogTree->resourceID = "79000.avi";
		dialogTree->show( this );
		DachaDialog1->cue();
	END

	BEG
		theMovie->caller = NULL;
		theMovie->stop();

		DachaDialog1->dispose();
		DachaDialog2->dispose();
		DachaDialog3->dispose();
		DachaDialog4->dispose();
		DachaDialog5->dispose();
		DachaDialog6->dispose();
		DachaDialog7->dispose();
		DachaDialog8->dispose();
		DachaDialog9->dispose();
		DachaDialog10->dispose();
		DachaDialog14->dispose();
		DachaDialog15->dispose();
		DachaDialog16->dispose();
		DachaDialogExit->dispose();
		sfxUnlockRes( 911, RES_ATS );
		ticks = 2;	// no particular reason, except gives time for dispose
    END

	BEG
		intrface->enable();	// needed after dialogs
		if( warhurstShootsYou )
			theGame->newRoom( new Rm60000( END_DEAD ) ); // death
		else
		{
			GameFlag.set( fEndgame );
			theGame->newRoom( new Rm60000( END_GAME ) );
		}
	END
}

/*******************************
 *
 *	Dialogs
 *
 *******************************/
 
 DachaDialog::DachaDialog()
 {
 	name = "DachaDialog";
 	playedLoop = FALSE;
 }

 void DachaDialog::cue( void )  // parent class, passes polymorphic cue() (unnecesary?)
 {
 	Dialog::cue();
 }

 void DachaD1_79000::cue( void )	// initial to shoot or drop cuts
 {
	switch( playedLoop ) // three segs: movie, shoot, shoot or drop gun
	{
	 case PLAYED_NONE:	// regular movie, shoot Warhurst option
	 	//Dialog::cue();
		theMovie->fromTo(from, to);
		theMovie->play(myTree->resourceID, this, FALSE, FALSE, FALSE);
		break;
	 case PLAYED_FIRST:	// option to drop gun added
	 	//hide(); -- it's a view now
	 	DachaDialog1->add( dropGun_1 );
		head = head->next;	// total kludge, skips gun
		show();
		head = head->prev;	// total kludge, restores skipped gun
		theMovie->fromTo( loopFrom, loopTo );	// not a loop
		theMovie->play(myTree->resourceID, this, FALSE, FALSE, FALSE);
		break;
	 case PLAYED_SECOND:	// you did nothing, Warhurst shoots you	
	 	hide();
		goTo2->cue();
		break;
	 default:
	 	assert( FALSE );
	}

	++playedLoop;
 }

 void DachaD2_79000::cue( void )   // you're dead
 {
	warhurstShootsYou = TRUE;
	Dialog::cue();
 }

 void DachaD3_79000::cue( void )	// PDA thing
 {	// note: this one isn't ready or used yet
 	//sfxPrintf( "How'd you get here? (dialog3)" );
	Dialog::cue();
 }

 // note: Dialogs 4, 5, 6 and 7 are normal!

 void DachaD8_79000::cue( void )
 {
	goTo10->cue();
 }

 void DachaD9_79000::cue( void )
 {
	if( playedLoop)
	{
		hide();
		goTo2->cue();
	}
	else  
	{
		playedLoop = TRUE;
		Dialog::cue();		// 'loop' finishes movie, gives action options
		theMovie->fromTo(loopFrom, loopTo);
		theMovie->play(myTree->resourceID, this, FALSE, FALSE, FALSE);		
	}
 }

 void DachaD10_79000::cue( void )
 {
	if( playedLoop)
	{
		hide();
		goTo2->cue();
	}
	else  
	{
		playedLoop = TRUE;
		Dialog::cue();		// 'loop' finishes movie, gives action options
		theMovie->fromTo(loopFrom, loopTo);
		theMovie->play(myTree->resourceID, this, FALSE, FALSE, FALSE);		
	}
 }

 void DachaD14_79000::cue( void )  // Yuri tries to arrest Cherbanov
 {
	switch( playedLoop ) // multiple change points ( 3 segs )
	{
	 case PLAYED_NONE:	// regular movie
	 	Dialog::cue();
		theMovie->fromTo(loopFrom, loopTo);
		theMovie->play(myTree->resourceID, this, FALSE, FALSE, FALSE);
		break;
	 case PLAYED_FIRST:	// option to shoot Yuri
	//	Dialog::cue();	// either DItem: shoot Yuri, or loop finishes movie
	//	theMovie->fromTo( 3780, 3795 );
	//	theMovie->play(myTree->resourceID, this, FALSE, FALSE, FALSE);
	//	break;
	// case PLAYED_SECOND:	// Yuri gone, end	
	 	hide();
		goToExit->cue();
		break;
	 default:
	 	assert( FALSE );
	}

	++playedLoop;
 }

 void DachaD15_79000::cue( void )
 {
	GameFlag.set( fEndgameYuriShot );
	Dialog::cue();
 }

 // dialogs 16 and 17 are normal

  void DachaD18_79000::cue( void )
 {
	goTo20->cue();
 }

  void DachaD19_79000::cue( void )
 {
	goTo2->cue();
 }

  void DachaD20_79000::cue( void )
 {
	goTo2->cue();
 }

  void DachaD21_79000::cue( void )
 {
	goTo9->cue();
 }

  void DachaD22_79000::cue( void )
 {
	goTo9->cue();
 }

 void DachaDExit_79000::cue( void )	 // yes, this is an extraneous method (and subclass) ?
 {
	Dialog::cue();	
 }

 /*******************************
 *
 *	Ditems
 *
 *******************************/
 
 void CutDItem::doit( void )
 {										 
	myDialog->hide();
	user->canInput( FALSE );
	theMovie->pause( FALSE );
	cue();
 }

  GrabShootDItem::GrabShootDItem( int whoToShootLoop )
 {
 	grabbedGun = FALSE;
	shootLoop = whoToShootLoop;
 }

 void GrabShootDItem::doit( void )
 {
	if( !grabbedGun )
	{
		grabbedGun = TRUE;
		// switch gun views
		myView->dispose();
		myView = new View;
		myView->view = 911;
		myView->loop = shootLoop;
		myView->x = GUN_X79000;
		myView->y = GUN_Y79000;
		myView->init(myView->x, myView->y);
		//myView->noSkip();
		setRect(myView->top, myView->left, myView->bottom, myView->right); // ?
	}
	else
		CutDItem::doit();	
 }
 
 // used but no longer needed
 void NewsFlash_16_79000::doit( void )
 {
  	newsFlash = TRUE;	
  	DItem::doit();	
 }
