//Started by Linus Chen
//Helicopter after Blake
//Copyright Activision 1995

//flag: fLinus1 used to show state after powerbook thrown out
//flag: fLinus2 used to show state after powerbook removed from backpack
//flga: fLinus3 used to show state after powerbook openned / turned on

//flag: fLinus13 used for drawpic in room 90000 or 90001
//flag: fLinus14 used for one time check on heli timer script
//flag: fLinus36 : lock out player for south exit


#include "globals.h"
#include "game.h"
#include "verbs.h"
#include "views.h"
#include "feature.h"
#include "90000.h"
#include "1000.h"
#include "flag.h"
#include "web.h"
#include "html.h"
#include "plistobj.h"
#include "email.h"
#include "target.h"
#include "intrpuzz.h"
#include "60000.h"
#include "91000.h"
#include "save.h"

//DIRECT #include "mcimovie.h"
#ifdef DIRECTX
#include "advmovie.h"
#else
#include "mcimovie.h"
#endif

#define CRSBLINK_SPEED 5

/*********************
*
*		Room Code 
*
**********************/

extern MADEEnvirons *colbyInfo;
extern Intrface* intrface;
extern WestCursor* westCursor;
extern EastCursor* eastCursor;

extern BlinkCursor* normalCursor;
extern SouthCursor* southCursor;
extern IntrPuzz* intrpuzz;
extern int canSaveColby; 
int ExitPermission = TRUE;
int NoTimeLeft = FALSE;

extern short restoring; 
extern SaveGame* saveGame;

PDACursor 		*pdaCursor;
PBookCursor 	*pbkCursor;
PermListObject 	*pobj; 

Sound			*countSound;
OkButton 		*okButton;
CancelButton 	*cancelButton;
StopButton 		*stopButton;
DownloadMenu 	*downloadMenu;
PercentBar 		*percentBar;
OkFeature 		*okFeature;
CancelFeature 	*cancelFeature;
//StopFeature 	*stopFeature;
StatusBar		*statusBar;
StartScreen		*startScreen;
ClosedPowerBook *closedPowerBook;
DownloadGlobe	*downloadGlobe;

PowerBookInv *pbkInv;


//Room 90000 : Helicopter Cockpit shot
Rm90000::Rm90000()
{
	name = "Rm90000";
	show_style = VE_FLIP;
}

Rm90000::~Rm90000()
{
	canSaveColby = TRUE;
}

void Rm90000::init()
{
  	drawPic( 90000 );
    Room::init();

	
	if (!restoring && (strcmp(theGame->prevRoomName, "Rm90001")))
	{
		sfxPrintf("Autosaving in Chopper");
		#ifndef DOS4GW
		saveGame->save("Chopper.sgm");
		#else
			char* tmpSave;
			SaveDlg("sgm");
			tmpSave = sfxGetSaveFile();
			if (strcmp(tmpSave, ""))
				saveGame->save(tmpSave);
		#endif
	}
	canSaveColby = FALSE;	// disable save!

	if (GameFlag.test(fLinus1))
	{
  		intrface->enable();
	 	canSaveColby = TRUE;		// Save Turned on 2/23/96
	}else intrface->disable();		// added on 2/25/96

  	theGame->handsOn();
	GameFlag.set(fLinus13);

// Preload assets for download and backpack
	sfxLoadRes ( 90002, RES_PIC );
	
	if( sound1->number != 90100  || !sound1->isPlaying() )
	{	
		sound1->stop();
		sound1->loop = SND_LOOP_INFINITE;
		sound1->play( 90100, NULL );
	}
  
	if(GameFlag.test(fGotPowerBook) && !GameFlag.test(fLinus1)) 
		pbkInv = new(PowerBookInv);

	if (!GameFlag.test(fLinus14))
	{
		GameFlag.set(fLinus36);							// disallow south exit
		curRoom->setScript ( new HeliTimerStart );	 	// Pilot vo
	 	GameFlag.set(fLinus14);							// allow only once
	}

	new(SouthExit90000);
//	new(HeliDebug);
}

int Rm90000::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}

SouthExit90000::SouthExit90000()
{
	name = "SouthExit90000";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
}

void SouthExit90000::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit90000::doVerb ( int theVerb )
{
	if (( theVerb == DO_V ) && !GameFlag.test(fLinus36) && (ExitPermission == TRUE)) {
    	theGame->newRoom(new Rm90001);
    	return TRUE;
	}
	return FALSE;
}

//Room 90001 : Helicopter Seat / Door shot
Rm90001::Rm90001()
{
	name = "Rm90001";
	show_style = VE_FLIP;
}

Rm90001::~Rm90001()
{
	canSaveColby = TRUE;
}

void Rm90001::init()
{
  	drawPic( 90001 );
    Room::init();
	canSaveColby = FALSE;

	if (GameFlag.test(fLinus1))
	{
  		intrface->enable();
	 	canSaveColby = TRUE;	 		// Save Turned on 2/23/96
	}else intrface->disable();			// added on 2/25/96

	if( sound1->number != 90100  || !sound1->isPlaying() )
	{	
		sound1->stop();
		sound1->loop = SND_LOOP_INFINITE;
		sound1->play( 90100, NULL );
	}

  	theGame->handsOn();
	GameFlag.clear(fLinus13);			// clear in room 90000 flag

	if (!GameFlag.test(fLinus2))		// check for state before pbook out of pack
		new (Backpack90001);
	else new (EmptyBackpack);
										// check for state during countdown
	if (GameFlag.test(fGotPowerBook) && !GameFlag.test(fLinus1))
		pbkInv = new(PowerBookInv);
	else if (!GameFlag.test(fGotPowerBook) && GameFlag.test(fLinus2))	 
			if (GameFlag.test(fLinus3))				   // pbook open
			{
				new(PowerBook90001);
				startScreen = new ( StartScreen );
			}
			else new(ClosedPowerBook);

	new(SouthExit90001);
	new(WestExit90001);
	new(Window90001);
}

int Rm90001::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}

SouthExit90001::SouthExit90001()
{
	name = "SouthExit90001";
	setHotspot ( DO_V );
	init(60, SOUTH_EXIT_Y, 392, SOUTH_EXIT_HEIGHT);
}

void SouthExit90001::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit90001::doVerb ( int theVerb )
{
	if (( theVerb == DO_V ) && (ExitPermission == TRUE)){
    	theGame->newRoom(new Rm90000);
    	return TRUE;
	}
	return FALSE;
}

WestExit90001::WestExit90001()
{
	name = "WestExit90001";
	pbkCursor = new (PBookCursor);
	setHotspot ( DO_V );
	init(WEST_EXIT_X, WEST_EXIT_Y, WEST_EXIT_WIDTH, (WEST_EXIT_HEIGHT + 20));
}

void WestExit90001::respond()
{
	if ( GameFlag.test (fGotPowerBook) && !GameFlag.test (fLinus1))
		theGame->setCursor(pbkCursor);
}

int WestExit90001::doVerb ( int theVerb )
{
	if ( GameFlag.test (fGotPowerBook) && !GameFlag.test (fLinus1))
	{
		if ( theVerb == DO_V ) {
			GameFlag.set (fLinus1);
			canSaveColby = TRUE;		// Save Turned on 2/23/96
    		pbkInv->dispose();
			countSound->stop();
			pobj->script->dispose();
			pobj->dispose();
			theGame->setCursor( normalCursor );
			curRoom->setScript( new ExplodeSound ); 
	  	 	return TRUE;
		}
	}
	return FALSE;
}

//DownLoad "Room" 

Rm90002::Rm90002()
{
	name = "Rm90002";
	show_style = VE_FLIP;
}

Rm90002::~Rm90002()
{
	canSaveColby = TRUE;
}

void Rm90002::init()
{
  	drawPic( 90002 );
//  sfxLoadRes ( 90004, RES_ATS );

	canSaveColby = FALSE;
    Room::init();

	sound1->stop();
	theGame->handsOn();
	intrface->disable();
	
	downloadGlobe 	= new ( DownloadGlobe );
	downloadMenu 	= new ( DownloadMenu );
	okFeature 		= new ( OkFeature );
	cancelFeature 	= new ( CancelFeature );  
}

int Rm90002::handleEvent ( MADEEventStamp *event )
{

	return FALSE;
}

// room right before transfer movie plays from heli to carrier

Rm90003::Rm90003()
{
	name = "Rm90003";
	show_style = VE_FLIP;
}

Rm90003::~Rm90003()
{
	canSaveColby = TRUE;
}

void Rm90003::init()
{
	if (GameFlag.test(fLinus13))
  		drawPic( 90000 );
    else 
    {
    	drawPic ( 90001 );
	    new ( EmptyBackpack );
	}

    Room::init();
	canSaveColby = FALSE;

	theGame->handsOn();
	intrface->disable();

	curRoom->setScript ( new HeliTransferScript );
	
	if( sound1->number != 90100  || !sound1->isPlaying() )
	{	
		sound1->stop();
		sound1->loop = SND_LOOP_INFINITE;
		sound1->play( 90100, NULL );
	}
}

int Rm90003::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}


// Blow up room
Rm90004::Rm90004()
{
	name = "Rm90004";
	show_style = VE_FLIP;
}

Rm90004::~Rm90004()
{
	canSaveColby = TRUE;
}

void Rm90004::init()
{
	drawPic( -1 );
    Room::init();

	canSaveColby = FALSE;	// disable save!

	curRoom->setScript ( new HeliBlowupScript );
}

int Rm90004::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}


/*******************************
 *
 *	Features
 *
/*******************************/
OkFeature::OkFeature()
{
	name = "OkFeature";
	setHotspot( DO_V );
 	init( 333, 174, 48, 30 );
}

void OkFeature::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink(CRSBLINK_SPEED);
}

int OkFeature::doVerb( int theVerb )
{
	if ( theVerb == DO_V ) {
		theGame->handsOff();
		curRoom->setScript ( new OkButtonClick );
		GameFlag.set(fStartDownLoad);
		return TRUE;
	}
	return FALSE;
}

CancelFeature::CancelFeature()
{
	name = "CancelFeature";
	setHotspot( DO_V );
 	init( 387, 174, 102, 30 );
}

void CancelFeature::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink(CRSBLINK_SPEED);
}

int CancelFeature::doVerb( int theVerb )
{
	if ( theVerb == DO_V ) {
		curRoom->setScript ( new CancelButtonClick );
		return TRUE;
	}
	return FALSE;
}

StopFeature::StopFeature()
{
	name = "StopFeature";
	setHotspot( DO_V );
 	init( 179, 147, 46, 146 );
}

void StopFeature::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink(CRSBLINK_SPEED);
}

int StopFeature::doVerb( int theVerb )
{
	if ( theVerb == DO_V ) {
		theGame->handsOff();
		curRoom->setScript ( new StopButtonClick );
		dispose();
		return TRUE;
	}
	return FALSE;
}

Backpack90001::Backpack90001()
{
	name = "Backpack90001";
	setHotspot( DO_V );
// 	init( 310, 330, 20, 20 );
	init( 305, 325, 35, 25 );
}

void Backpack90001::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink(CRSBLINK_SPEED);
}

int Backpack90001::doVerb( int theVerb )
{
	if ( theVerb == DO_V ) {
		GameFlag.set (fLinus2);
//		closedPowerBook = new (ClosedPowerBook);
//		new (EmptyBackpack);
		curRoom->setScript ( new BackpackSound );
		dispose();
		return TRUE;
	}
	return FALSE;
}

Window90001::Window90001()
{
	name = "Window90001";
	pbkCursor = new (PBookCursor);
	setHotspot( DO_V );
	init( 415, 125, 85, 150 );
}

void Window90001::respond()
{
	if ( GameFlag.test (fGotPowerBook) && !GameFlag.test (fLinus1))
		theGame->setCursor(pbkCursor);
}

int Window90001::doVerb( int theVerb )
{
	if ( GameFlag.test (fGotPowerBook) && !GameFlag.test (fLinus1))
	{
		if ( theVerb == DO_V ) {
    		GameFlag.set (fLinus1);
			canSaveColby = TRUE;		// Save Turned on 2/23/96
    		pbkInv->dispose();
			countSound->stop();
			pobj->script->dispose();	
			pobj->dispose();
			theGame->setCursor( normalCursor );
			curRoom->setScript( new ExplodeSound );
			
			return TRUE;
		}
	}
	return FALSE;
}

/*******************************
 *
 *	Scripts
 *
 *******************************/
void HeliBlowupScript::changeState ( int newState )
{
 	switchTo
		theMovie->play("90001.avi",this, 0, 1, 0);
	END

	BEG
	   	ticks = 2;
	END

	BEG
		theGame->newRoom ( new Rm60000(END_DEAD));
	END
}

void goTo91000::changeState ( int newState )
{
 	switchTo
	   	theGame->newRoom ( new Rm90003 );
	END
}

void HeliTimerStart::changeState ( int newState )
{
 	switchTo
	    pilotVo1->play ( 90025, this );
	END

	BEG
	   	ticks = 2;
	END

	BEG
		pilotVo1->dispose();
	   	cue();
	END

	BEG
		GameFlag.clear(fLinus36);
		dispose();
	END
}

void HeliTransferScript::changeState( int newState )
{
	switchTo
	   	seconds = 5;
	END

	BEG
		pilotVo2->play ( 90026, this );
	END

	BEG
	   	ticks = 2;
	END

	BEG
		pilotVo2->dispose();
		sound1->stop();
		cue();
	END
	
	BEG
		theGame->newRoom ( new Rm91000 ); 		// tarmac room
	END
}

void ExplodeSound::changeState( int newState )
{
	switchTo
		ExitPermission = FALSE;
		seconds = 2;
	END

	BEG
	   	explode->play( 90020, NULL );
		seconds = 5;
	END

	BEG
	   	pilotVo3->play ( 90019, this );
	END

	BEG
		seconds = 2;
	END

	BEG
	   	pilotVo3->dispose();
	   	explode->dispose();
		intrface->enable();
	
		if (GameFlag.test(fPBMaxDownload))					 // email due to status
			new EMail ( "INTELINK ERROR: Data Exception", 98320 );
		else if (GameFlag.test(fPBLongDownload))
			new EMail ( "INTELINK ERROR: Data Exception", 98323 );
		else if (GameFlag.test(fPBMedDownload))
			new EMail ( "INTELINK ERROR: Data Exception", 98322 );
		else if (GameFlag.test(fPBShortDownload))
			new EMail ( "INTELINK ERROR: Data Exception", 98321 );
		ExitPermission = TRUE;
		dispose();
	END
}

PowerBookSnd::PowerBookSnd()
{
	name = "PowerBookSnd";
	snd1 = new (Sound);
	pobj = new ( PermListObject );
}

void PowerBookSnd::changeState( int newState )
{
	switchTo
		theGame->handsOff();
		intrface->disable();
		snd1->play(90011, this);	//click
	END
	
	BEG
	   	ticks = 15;
	END

	BEG
		startScreen = new ( StartScreen );
		snd1->play(90033, this);	//ding
	END
	
	BEG
		ticks = 15;
	END

	BEG
	   	snd1->play(90015, this);	//whoever you are....
	END

	BEG
	 	snd1->dispose();
		ticks = 5;
	END
	
	BEG
		pobj->setScript ( new CountDownScript90000 );	
	END
}

CountDownScript90000::CountDownScript90000()
{
 	name = "CountDownScript90000";
	countSound = new Sound;
}

void CountDownScript90000::changeState( int newState )
{	
	switchTo
		theGame->handsOn();
		if( colbyInfo->slow_system == TRUE )
			countSound->play ( 90028, this );	// slow computer long count
		else 
			countSound->play ( 45058, this );  	// fast computer short count
	END

	BEG
	   	if (GameFlag.test(fLinus1))
			countSound->dispose();
		else cue();
	END

	BEG
		sound1->stop();
		countSound->dispose();
		theGame->newRoom ( new Rm90004 );
	END
}

void OkButtonClick::changeState ( int newState )
{
 	switchTo
		okButton = new ( OkButton );
		ticks = 5;
	END

	BEG
	   	okButton->dispose();
		ticks = 2;
	END

	BEG
		cancelFeature->dispose();
		okFeature->dispose();
		downloadMenu->dispose();

	   	percentBar = new ( PercentBar );
	   	new ( StopFeature );	

		oksound->play(45059,this);
	END

	BEG
	   	ticks = 2;
	END

	BEG
		oksound->dispose();
		curRoom->setScript ( new StatusBarStart );
	END
}

void CancelButtonClick::changeState ( int newState )
{
 	switchTo
		cancelButton = new ( CancelButton );	
		ticks = 5;
	END

	BEG
		cancelButton->dispose();
		cue();
	END

	BEG
	   	theGame->newRoom ( new Rm90001 );
	END
}

void StopButtonClick::changeState ( int newState )
{
 	switchTo
		stopButton = new ( StopButton );
		ticks = 5;	
	END

	BEG
		stopButton->dispose();
		cue();
	END

	BEG
		snd5->play (45060, this);
	END

	BEG
	   	ticks = 2;
	END
	
	BEG
		snd5->dispose();
		downloadGlobe->dispose();
		percentBar->dispose();
		statusBar->dispose();
		theGame->newRoom( new Rm90001 );
	END
}

void StatusBarStart::changeState ( int newState )
{
 	switchTo
		statusBar = new ( StatusBar );
		ticks = 30;
	END

	BEG
		statusBar->setView(90021);
		GameFlag.set (fPBShortDownload);			 	// No flag set
		GameFlag.set (fPowerBook7Available);
		GameFlag.set (fPowerBook6Available);
		seconds = 1;
	END

	BEG
		theGame->handsOn();
	   	statusBar->setView(90022);
		seconds = 1;
	END

	BEG
	   	statusBar->setView(90023);
		GameFlag.clear (fPBShortDownload);				// allow GPS flag set
		GameFlag.set (fPBMedDownload);
		GameFlag.set (fPowerBook5Available);
		GameFlag.set (fPowerBook4Available);
		GameFlag.set (fPowerBook3Available);
		seconds = 2;
	END

	BEG
	   	statusBar->setView(90024);
		seconds = 1;
	END

	BEG
	   	statusBar->setView(90025);			  			// allow Freq set
		GameFlag.clear (fPBMedDownload);
		GameFlag.set (fPBLongDownload);
		GameFlag.set (fPowerBook2Available);
		seconds = 3;
	END

	BEG
	   	statusBar->setView(90026);
		seconds = 4;
	END

	BEG
	   	statusBar->setView(90027);
		GameFlag.clear (fPBLongDownload);
		GameFlag.set (fPBMaxDownload);					// all flags allowed
		GameFlag.set (fPowerBook1Available);
		seconds = 3;
	END

	BEG
	   	statusBar->setView(90028);
		seconds = 6;
	END
}

void BackpackSound::changeState ( int newState )
{
 	switchTo
		theGame->handsOff();		// added on 2/25/96
		new (EmptyBackpack);
	   	snd6->play ( 90010, this );
	END

 	BEG
	   	ticks = 2;
	END

	BEG
		closedPowerBook = new (ClosedPowerBook);
		snd6->play ( 90009, this );
	END

	BEG
	   	ticks = 2;
	END

	BEG
		theGame->handsOn();				// added on 2/25/96
	   	snd6->dispose();
		dispose();
	END
}

/*********
 * views *
 *********/

// Screen Items
StatusBar::StatusBar()
{
	name = "StatusBar";
	setLoop(0);
	setView(90020);
	init(238, 88);
	noSkip();
}

OkButton::OkButton()
{
	name = "OkButton";
	setLoop(0);
	setView(90006);
	init(328, 169);
	noSkip();
}

CancelButton::CancelButton()
{
	name = "CancelButton";
	setLoop(0);
	setView(90007);
	init(382, 169);
	noSkip();
}

StartScreen::StartScreen()
{
	name = "StartScreen";
	setLoop(0);
	setView(90012);
	init(243,222);
	noSkip();
}

DownloadMenu::DownloadMenu()
{
	name = "DownloadMenu";
	setLoop(0);
	setView(90008);
	init(10, 160);
	noSkip();
}

PercentBar::PercentBar()
{
	name = "PercentBar";
	setLoop(0);
	setView(90009);
	init(159, 41);
	noSkip();
}

StopButton::StopButton()
{
	name = "StopButton";
	setLoop(0);
	setView(90010);
	init(170, 139);
	noSkip();
}

// Room items
ClosedPowerBook::ClosedPowerBook()
{
	name = "ClosedPowerBook";
	setHotspot ( DO_V );
	setLoop(0);
	setView(90001);
	init(202, 203);
	noSkip();
}

void ClosedPowerBook::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink(CRSBLINK_SPEED);
}

int ClosedPowerBook::doVerb ( int theVerb )
{
 	if ( theVerb == DO_V ){
		GameFlag.set(fLinus3);
		new(PowerBook90001);
		curRoom->setScript(new PowerBookSnd);
		dispose();
		return TRUE;
	}
	return FALSE;
}

PowerBook90001::PowerBook90001()
{
	name = "PowerBook90001";
	pdaCursor = new (PDACursor);
	setHotspot ( DO_V );
	setLoop(0);
	setView(90003);
	init(202, 203);
	noSkip();
}

void PowerBook90001::respond()
{
	if (!GameFlag.test (fStartDownLoad))
		theGame->setCursor( pdaCursor );
	else
	{
		theGame->setCursor( normalCursor );
		normalCursor->blink(CRSBLINK_SPEED);
	}
}

int PowerBook90001::doVerb ( int theVerb )
{
	if( GameFlag.test(fStartDownLoad))
	{
		GameFlag.set(fGotPowerBook);
		pbkInv = new (PowerBookInv);
		startScreen->dispose();
		dispose();
		return TRUE;
	}else 
	if ( theVerb == DO_V )	{
		theGame->newRoom ( new Rm90002 );
		return TRUE;
	}
	return FALSE;
}				 

HeliDebug::HeliDebug()
{
 	name = "HeliDebug";
	setHotspot ( DO_V );
	setLoop(0);
	setView(90002);
	init(50,50);
	noSkip();
}

void HeliDebug::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink(CRSBLINK_SPEED);
}

int HeliDebug::doVerb ( int theVerb )
{
 	if ( theVerb == DO_V ){
		GameFlag.clear ( fLinus1 );
		GameFlag.clear ( fLinus2 );
		GameFlag.clear ( fLinus3 );
		GameFlag.clear ( fLinus14 );
		GameFlag.clear ( fGotPowerBook );
		GameFlag.clear ( fStartDownLoad );
		GameFlag.clear ( fPBShortDownload );
		GameFlag.clear ( fPBMedDownload );
		GameFlag.clear ( fPBLongDownload );
		GameFlag.clear ( fPBMaxDownload );
		GameFlag.clear ( fPowerBook1Available );
		GameFlag.clear ( fPowerBook2Available );
		GameFlag.clear ( fPowerBook3Available );
  		GameFlag.clear ( fPowerBook4Available );
		GameFlag.clear ( fPowerBook5Available );
		GameFlag.clear ( fPowerBook6Available );
		GameFlag.clear ( fPowerBook7Available );
		return TRUE;
	}
	return FALSE;
}

PowerBookInv::PowerBookInv()
{
	name = "PowerBookInv";
	setLoop(0);
	setView(90002);
	init(256, 440);
//	noSkip();
}

EmptyBackpack::EmptyBackpack()
{
	name = "EmptyBackpack";
	setLoop(0);
	setView(90004);
	init(258, 290);
	noSkip();
}

/* cursor */
PDACursor::PDACursor()
{
	name = "PDACurosr";
	view = 90015;
	loop = 0;
	cel = 0;
	orig_pt.x = 0;
	orig_pt.y = 0;
	verb = DO_V;
}

PBookCursor::PBookCursor()
{
	name = "PBookCurosr";
	view = 90002;
	loop = 0;
	cel = 0;
	orig_pt.x = 0;
	orig_pt.y = 0;
	verb = DO_V;
}

// Props

DownloadGlobe::DownloadGlobe()
{
 	name = "DownloadGlobe";
	
	cycleSpeed = 4;	
	view = 604;
	loop = 0;
	cel = 0;
	init(465,20);

	setCycle(new EndLoop((Prop*)this, this));
}

void DownloadGlobe::cue(void)
{
	setCycle(new EndLoop((Prop*)this, this));
}
