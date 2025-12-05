/*********************************************
* Player's office
* Copyright Activision 1995
* Started by Tom DeSalvo 
* Continued by:  Daniel C. Kinzek   June, 1995
* Notes:  Using theGame->setScript for Jaimie's SoundHack --removed
*		  
**********************************************/
			 
#include "globals.h"
#include "game.h"
#include "verbs.h"
#include "views.h"
#include "invent.h"
#include "flag.h"
#include "pda.h"
#include "vlink.h"
#include "roomsnd.h"
#include "24000.h"
#include "1000.h"
#include "advsound.h"
#include "advmusic.h"
#include "gamebox.h"
#include "verbs.h"
#include "advfile.h"
#include "60000.h"

#include "teletalk.h"
#include "PCScreen.h"
#include "phone.h"

//#pragma message( "remove this" )
//extern short curDisc;

#define CRSBLINK_SPEED 5

extern Intrface* intrface;
extern SouthCursor* southCursor;
extern BlinkCursor* normalCursor;
extern DoorCursor* doorCursor;
extern int isDemo;
//extern int invVerb;

#ifdef _DEBUG
	class YingStuffToggle : Feature
	{
	 public:
		YingStuffToggle();
		int doVerb( int );
	};
#endif

int	launchHacker( void );

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

class PlayASound : public Object
{
 public:
 	PlayASound( int soundNum );
	void cue( void );
	Sound*	soundptr;
};	// defined in 2000.cpp

/*********************
*
*		Room Code 
*
**********************/

Rm24000::Rm24000()
{
	name = "Rm24000";
	show_style = VE_FLIP;
}

void Rm24000::init()
{
	drawPic( 24000 );
    Room::init();
  	new SouthExit24000;
	new Desk24000;
	#ifdef _DEBUG
		new YingStuffToggle;	
	#endif				  

 //#pragma message( "remove this!!!" )
 //inventry->get( iHACKERDISK );
 //GameFlag.set( fPlayerTurnsProcat );
 //GameFlag.set( fMoscowAvailable );
 //curMap = MAP_ASSASS;
 //curDisc = 2;
 //inventry->get( iNEEDLEPACK );

	startSound( 24000, PLAYWAVEMIDI );
			          
	intrface->show();
	theGame->handsOn();  

	// Start office
	if( !GameFlag.test( fPlayerOfficeStarted ) )
		GameFlag.set( fPlayerOfficeStarted );

	// Desk Sprites
	if( GameFlag.test( fYingStuffArrived ) )	  
	{
		if(!inventry->has( iYINGPHOTO ))
			new DeskSprite24000( 24001, 231, 256 +16 );
		if(!inventry->has( iYINGBEALEPOSTIT ))
			new DeskSprite24000( 24006, 265, 259 +16 );
		if(!inventry->has( iYINGBEOWULF ))
			new DeskSprite24000( 24008, 181, 252 +16 );
		if(!inventry->has( iEBMCATALOG ))
			new DeskSprite24000( 24010, 183, 236 +16 );
		//if(!inventry->has( iYINGINTERNETGUIDE ))			--- removed
		//	new DeskSprite24000( 24009, 182, 235 +16 );
	}
	//if(!inventry->has( iLOCKPICK ))
	//	new	DeskSprite24000( 24007, 224, 237 +16 );
}

Rm24100::Rm24100()
{
	name = "Rm24100";
	show_style = VE_FLIP;
}

void Rm24100::init()
{
	drawPic (24100);
	Room::init();

	startSound( 24000, PLAYWAVEMIDI );
	new SouthExit24100;
	new Computer24100;
	new WallChart24100;
	if( GameFlag.test( fFullBriefing ) )
		new WallBox24100;
	//new ScreenSaverProp24100; 
		//new DeadParkerPhoto24100; // text feature --moved to Moscow
		//new PresidentPhotoView24100;			    --moved to Moscow
		//new EnvelopeH24100;	// text feature	    --moved to Moscow
	new Phone24100;
	//new Rolodex24100;
	//new PCDistantButton( PC_INTRLINKBUTTON, 92060, 426, 272 +16 );

	if( GameFlag.test( fYingStuffArrived ) )	  
	{
		if(!inventry->has( iYINGPHOTO ))
			new YingPhotoView24100;
		if(!inventry->has( iYINGBEALEPOSTIT ))
			new YingStickyNoteView24100;
		if(!inventry->has( iYINGBEOWULF ))
			new BeowulfView24100;
		//if(!inventry->has( iYINGINTERNETGUIDE ))	-- removed
		//	new InternetGuideView24100;
		if(!inventry->has( iEBMCATALOG ))
			new YingEBMCatView24100;
		#ifndef DOS4GW
		  if(!inventry->has( iSHANGHAIDISK ))
			  new ShanghaiView24100;
		#endif
	}
	//if(!inventry->has( iLOCKPICK ))
	//	new	LockPickView24100;

	intrface->show();
	theGame->handsOn();
}

/*********************
*
*	Exit Features 
*
**********************/

SouthExit24000::SouthExit24000()
{
	name = "SouthExit24000";
	setHotspot ( DO_V );
	init( SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT );  
}

void SouthExit24000::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit24000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm1000);
    	return TRUE;
	}
	return FALSE;
}

SouthExit24100::SouthExit24100()
{
	name = "SouthExit24100";
	setHotspot ( DO_V );
	init(100, SOUTH_EXIT_Y +35, 300, SOUTH_EXIT_HEIGHT);  
}

void SouthExit24100::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit24100::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		inventry->clearCurrentItem(); 	// get rid of EBMC
    	theGame->newRoom(new Rm24000);
    	return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Features
 *
/*******************************/

Desk24000::Desk24000()
{
	name = "Desk";
	setHotspot ( DO_V );
	init ( 120, 208, 250, 100 );
}

void Desk24000::respond()
{
	theGame->setCursor( doorCursor );
}

int Desk24000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom( new Rm24100 );
    	return TRUE;
	}
	return FALSE;
}

// not used
Rolodex24100::Rolodex24100()
{
	name = "Rolodex24100";
	setHotspot( DO_V );
	init(80, 283, 52, 25);
}

// not used
void Rolodex24100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink(CRSBLINK_SPEED);
}

// not used
int Rolodex24100::doVerb ( int theVerb )
{
	if(theVerb == DO_V) {
		new RolodexInset24100;
		return TRUE;
	}
	return FALSE;
}

Phone24100::Phone24100()
{
	name = "Phone24100";
	setHotspot( DO_V );
	hotspot->addRespondVerb(NUMBERTOBLAKE_V);
	init(25, 325, 80, 35);
}

void Phone24100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink(CRSBLINK_SPEED);
}

int Phone24100::doVerb ( int theVerb )
{
	if(theVerb == DO_V) {
		theGame->newRoom( new PhoneKeyPad( FROM_USA ) );
		return TRUE;
	}
	return FALSE;
}

void Phone24100::cue( void )
{
	curRoom->setScript(	new BlakeTeletalkScript ); 	
}

Computer24100::Computer24100()
{
	name = "Computer24100";
	setHotspot ( DO_V );
	hotspot->addRespondVerb(SHANGHAIDISK_V);
	hotspot->addRespondVerb(HACKERDISK_V);
	hotspot->addRespondVerb(YINGEBMCATALOG_V);
	hotspot->addRespondVerb(YINGBEALEPOSTIT_V);
	init ( 403, 220, 80, 165 );
}

void Computer24100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink(CRSBLINK_SPEED);
}

int Computer24100::doVerb ( int theVerb )
{
	
	if( invVerb == SHANGHAIDISK_V )
	{	
		if( curRoom->script == NULL )
		{
			inventry->clearCurrentItem();
			curRoom->setScript( new launchShanghai );
		}
		return TRUE;
	}
	if( invVerb == HACKERDISK_V )
	{	
		//inventry->clearCurrentItem();
		//launchHacker();
		return TRUE;
	}
	else if( invVerb == YINGEBMCATALOG_V )
	{
		if( curRoom->script == NULL )
		{
			inventry->clearCurrentItem();
			curRoom->setScript( new EBMCatScript24100 );			
		}
		return TRUE;
	}
	else if( invVerb == YINGBEALEPOSTIT_V )
	{
		if( curRoom->script == NULL )
		{
			inventry->clearCurrentItem();
			curRoom->setScript( new YingBealeScript24100 );	
		}
		//GameFlag.set( fYingCipherLoaded ); // HTM flag 203
		//new PCScreen( 24100 );
		return TRUE;
	}
	else if( theVerb == DO_V )
	{
		if( curRoom->script == NULL )
		{
			pcLastUsed[PC_INTRLINK] = TRUE;
			if( isDemo )
			{
				pcLastUsed[PC_KAT] = TRUE;
				pcLastUsed[PC_IA] = WEB_IMAGEPEG;
				pcLastUsed[PC_INFOSCI] = TRUE;
				pcLastUsed[PC_PHOTODOC] = TRUE;
				pcLastUsed[PC_SNDANAL] = WEB_SOUND_BIRD;
				pcLastUsed[PC_CIPHER] = WEB_YINGCYPHER;
				pcLastUsed[PC_PEGID] = TRUE;
				//GameFlag.set( fSecurityModelAvailable );
			}
			new PCScreen( 24100 );
		}
		return TRUE;
	}						
	
	return FALSE;
}

WallChart24100::WallChart24100()
{
	name = "WallChart24100";
	setHotspot ( DO_V );
	init ( 120, 30, 223, 165 );
}

void WallChart24100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink(CRSBLINK_SPEED);
}

int WallChart24100::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		new WallChartInset24100;
		return TRUE;
	}
	return FALSE;
}

WallBox24100::WallBox24100()
{
	name = "WallBox24100";
	setHotspot ( DO_V );
	init ( 10, 126 +16 -10, 80, 80 );	// maybe cursor point is off (-10)
}

void WallBox24100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink(CRSBLINK_SPEED);
}

int WallBox24100::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		curRoom->setScript( new FolderSndScript24100 );
		new Yellow( 98630 );
		return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Views
 *
/*******************************/

DeskSprite24000::DeskSprite24000( int v_num, int x, int y )
{
	name = "YingPhotoView24100";
	setView( v_num );
	init ( x, y );
	noSkip();
}

YingPhotoView24100::YingPhotoView24100()
{
	name = "YingPhotoView24100";
	setHotspot ( DO_V );
	setView(24101);
	init ( 267, 356 );
	noSkip();
}

void YingPhotoView24100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink(CRSBLINK_SPEED);
}

int YingPhotoView24100::doVerb (int theVerb)
{
	if ( theVerb == DO_V ) {
		new YingPhotoInset24100;
		inventry->items[iYINGPHOTO]->activate();
		dispose();		
    	return TRUE;
	}
	return FALSE;
}

YingStickyNoteView24100::YingStickyNoteView24100()
{
	name = "YingStickyNoteView24100";
	setHotspot ( DO_V );
	setView(24106);
	init ( 346, 332 );
}

void YingStickyNoteView24100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink(CRSBLINK_SPEED);
}

int YingStickyNoteView24100::doVerb (int theVerb)
{
	if ( theVerb == DO_V ) {
		new YingStickyNoteInset24100;
		inventry->items[iYINGBEALEPOSTIT]->activate();
		dispose();			
    	return TRUE;
	}
	return FALSE;
}

// removed
LockPickView24100::LockPickView24100()
{
	name = "LockPickView24100";
	setHotspot ( DO_V );
	//noSkip();
	setView(24107);
	//noSkip();
	init ( 189, 309 );
	noSkip();
}

// removed
void LockPickView24100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink(CRSBLINK_SPEED);
}

// removed
int LockPickView24100::doVerb (int theVerb)
{
	if ( theVerb == DO_V ) {
		new LockPickInset24100;
		sfxSystemCursor( SYS_WAIT );
		sfxLoadRes( 24180, RES_ATS );
		theGame->handsOn();
		dispose();		
    	return TRUE;
	}
	return FALSE;
} 

BeowulfView24100::BeowulfView24100()
{
	name = "BeowulfView24100";
	setHotspot ( DO_V );
	setView(24108);
	init ( 180, 338 );
}

void BeowulfView24100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink(CRSBLINK_SPEED);
}

int BeowulfView24100::doVerb (int theVerb)
{
	if ( theVerb == DO_V ) {
		new BeowulfInset24100;
		inventry->items[iYINGBEOWULF]->activate();
		dispose();
		return TRUE;
	}
	return FALSE;
}

// not currently used
InternetGuideView24100::InternetGuideView24100()
{
	name = "InternetGuideView24100";
	setHotspot ( DO_V );
	setView(24109);
	init ( 120, 298 +16 );
}

// not currently used
void InternetGuideView24100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink(CRSBLINK_SPEED);
}

// not currently used
int InternetGuideView24100::doVerb (int theVerb)
{
	if ( theVerb == DO_V ) {
		new InternetGuideInset24100;
		inventry->items[iYINGINTERNETGUIDE]->activate();
		dispose();		
		return TRUE;
	}
	return FALSE;
}

YingEBMCatView24100::YingEBMCatView24100()
{
	name = "YingEBMCatView24100";
	setHotspot ( DO_V );
	setView( 24112 );
	//init ( 8, 357 +16 );
	init( 127, 303 +16 );
}

void YingEBMCatView24100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink(CRSBLINK_SPEED);
}

int YingEBMCatView24100::doVerb (int theVerb)
{
	if ( theVerb == DO_V ) {
		new YingEBMCatInset24100;
		inventry->items[iEBMCATALOG]->activate();
		dispose();		
		return TRUE;
	}
	return FALSE;
}

ShanghaiView24100::ShanghaiView24100()
{
	name = "ShanghaiView24100";
	setHotspot ( DO_V );
	setView(24111);
	init ( 83, 366 );
}

void ShanghaiView24100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink(CRSBLINK_SPEED);
}

int ShanghaiView24100::doVerb (int theVerb)
{
	if ( theVerb == DO_V ) {
		new ShanghaiInset24100;
		inventry->items[iSHANGHAIDISK]->activate();
		dispose();		
		return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Insets
 *
/*******************************/

YingPhotoInset24100::YingPhotoInset24100()
{
	init( 24120, 0, 16, 512, 384 );
	normalCursor->rest();	// stops respond blink
	inventry->get( iYINGPHOTO );
}

YingStickyNoteInset24100::YingStickyNoteInset24100()
{
	init( 24170, 0, 16, 512, 384 );
	normalCursor->rest();	// stops respond blink
	inventry->get( iYINGBEALEPOSTIT );
}

LockPickInset24100::LockPickInset24100()
{
	init( 24185, 0, 16, 512, 384 );
	normalCursor->rest();	// stops respond blink
	inventry->get( iLOCKPICK );
}

void LockPickInset24100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink(CRSBLINK_SPEED);
}

int LockPickInset24100::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		if( curRoom->script == NULL )
			curRoom->setScript( new ZipScript24100 );
		dispose();
		new LockPickOpenInset24100;
		//dispose();
    	return TRUE;
	}
	return FALSE;
}

LockPickOpenInset24100::LockPickOpenInset24100()
{
	init( 24180, 0, 16, 512, 384 );
	normalCursor->rest();	// stops respond blink
}

void LockPickOpenInset24100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink(CRSBLINK_SPEED);
}

int LockPickOpenInset24100::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		if( curRoom->script == NULL )
			curRoom->setScript( new ZipScript24100 );
		dispose();
		new LockPickInset24100;
		//dispose();
    	return TRUE;
	}
	return FALSE;
}

// not currently used
InternetGuideInset24100::InternetGuideInset24100()
{
	init( 24165, 0, 16, 512, 384 );	 // set number
	normalCursor->rest();	// stops respond blink
	inventry->get( iYINGINTERNETGUIDE );
}

YingEBMCatInset24100::YingEBMCatInset24100()
{
	init( 24199, 0, 16, 512, 384 );	 // set number
	normalCursor->rest();	// stops respond blink
	inventry->get( iEBMCATALOG );
}

BeowulfInset24100::BeowulfInset24100()
{
	init( 24175, 0, 16, 512, 384 );	// set number
	normalCursor->rest();	// stops respond blink
	inventry->get( iYINGBEOWULF );
}

ShanghaiInset24100::ShanghaiInset24100()
{
	init( 24135, 0, 16, 512, 384 );	  // set number
	normalCursor->rest();	// stops respond blink
	inventry->get( iSHANGHAIDISK );
}

PhoneInset24100::PhoneInset24100()
{
	init( 24190, 0, 16, 512, 384 );
	normalCursor->rest();	// stops respond blink
}

int PhoneInset24100::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		//Do something . . .
    	return TRUE;
	}
	return FALSE;							   
}

// not used
RolodexInset24100::RolodexInset24100()
{
	init( 24195, 0, 16, 512, 384 );
	normalCursor->rest();	// stops respond blink
}

// not used
int RolodexInset24100::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		//Do something . . .
    	return TRUE;
	}
	return FALSE;
}

WallChartInset24100::WallChartInset24100()
{
	init( 24125, 0, 16, 512, 384 );
	normalCursor->rest();	// stops respond blink
}

/*******************************
 *
 *	Props
 *
/*******************************/

// 24000
// not used
ScreenSaverProp24000::ScreenSaverProp24000()
{
	name = "ScreenSaverProp24000";
	setView( 24020 );
 	init( 304, 214 );	
	setHotspot( DO_V );
	cycleSpeed = 4;
	setCycle( new Forward( this ) );
}

// not used
void ScreenSaverProp24000::cue( void )
{
 	setCycle( new Forward( this ) );	
}

// 24100
// not used
ScreenSaverProp24100::ScreenSaverProp24100()
{
	name = "ScreenSaverProp24100";
	setView( 24145 );
 	init( 383, 211 );	
	setHotspot( DO_V );
	cycleSpeed = 4;
	setCycle( new Forward( this ) );
}

// not used
void ScreenSaverProp24100::cue( void )
{
 	setCycle( new Forward( this ) );	
}

/*******************************
 *
 *	Scripts
 *
/*******************************/

void JaimieBlamScript24000::changeState( int newState )
{
	switchTo
		GameFlag.set( fJaimieSentSoundHackToPlayer );
		seconds = 1 + (rand() % 4);	 // avg 3.5 sec delay
	END

	BEG
		// Jaimie's SoundHack
		sound2->number = SND_VO_SOUNDHACK; // Freeze, blam, blam!, 104 + 107
		sound2->play();
		dispose();
	END
}

void EBMCatScript24100::changeState( int newState )
{
	static Sound*	soundptr;
	static Web*		thisWeb;
	static View*	home;
	static View*	box;
	static View*	boxText;

	// pseudo htm pause page
	switchTo
		soundptr = new Sound;
		soundptr->play( 175, NULL);
		theGame->handsOff();
		ticks = 60;	// appx sound play time
	END

	BEG		
		home = new View;
		home->setView(97627);
		home->init( 0, 16 );
		home->setPri( 410 );
		home->noSkip();
		ticks = 20;
	END

	BEG
		box = new View;
		box->setView(97611);
		box->init( 131, 154 );
		box->setPri( 410 );
		box->noSkip();
		ticks = 20;
	END

	BEG
		boxText = new View;
		boxText->setView(97612);
		boxText->init( 156, 189 );
		boxText->setPri( 410 );
		boxText->noSkip();	
		ticks = 60;
	END

	BEG
		theGame->handsOn();
 		home->dispose();
		box->dispose();
		boxText->dispose();
 		thisWeb = new Web;
		thisWeb->caller = this;
		if( GameFlag.test( fNewElectBookAvail ) )
			new HTMLPage( 99730 );	
		else
			new HTMLPage( 99485 );
	END

	BEG
		soundptr->dispose();
		dispose();		
	END
}

void YingBealeScript24100::changeState( int newState )
{
	static Sound*	soundptr;

	switchTo
		soundptr = new Sound;
		soundptr->play( 175, NULL );
		theGame->handsOff();
		ticks = 60;
	END

	BEG
		theGame->handsOn();
		soundptr->dispose();
		GameFlag.set( fYingCipherLoaded ); // HTM flag 203
		new PCScreen( 24100 );
		dispose();
	END
}

void ZipScript24100::changeState( int newState )
{
	static Sound*	soundPtr;

	switchTo
		soundPtr = new Sound;			
		soundPtr->play( SND_ZIPPER, this );
	END

	BEG
		soundPtr->dispose();
		dispose();
	END
}

void FolderSndScript24100::changeState( int newState )
{
	static Sound*	sndptr;

 	switchTo
		sndptr = new Sound;
		sndptr->play( 26010 , this );	// picking up and opening folders, same as Holt's
	END

	BEG
		sndptr->dispose();
		dispose();
	END
}

void launchShanghai::changeState( int newState )
{ 	
#define MAXLINE (MAX_PATH +10)
#define SHANGHAIEXESTR "SHANGHAI.EXE"
	static char		prevDir[MAX_PATH];
	static char		path[MAX_PATH];
	int		result;
	static int		soundNumber = -1;
	
	switchTo
		// change directories to shanghai directory--it's not smart enough to use win.ini file
		GetCurrentDirectory( MAX_PATH, prevDir );
		strcpy( path, prevDir );
		strcat( path, "\\SHANGHAI" );
		SetCurrentDirectory( path );

		// run Shanghai
	
		if( sound1->isPlaying() )
		{
			soundNumber = sound1->number;
			sound1->stop();
		}
		else if( theMusic->isPlaying() )
		{
			soundNumber = theMusic->number;
			theMusic->stop();
		}
		sfxDisableSound();
		sfxDisableMusic();
		user->canInput( FALSE );
		//result = WinExec( SHANGHAIEXESTR, SW_SHOWMAXIMIZED	);
		result = sfxRunProgram( SHANGHAIEXESTR );
		ticks = 20;
	END

	BEG
		// restore
		/*
		sfxPrintf( "\n\n\n\n\n"
		           "               One  Moment  Please                  "
				   "\n\n\n\n\n"
				   "          Click OK After Playing Shanghai              "
				   "\n\n\n\n\n"
				   "                %s"
				   "\n\n",  result?"OK":"ERROR"  );
		*/
		SetCurrentDirectory( prevDir );
		sfxEnableSound();
		sfxEnableMusic();
		if( soundNumber != -1 )
			startSound( soundNumber, PLAYWAVEMIDI );
		user->canInput( TRUE );
		soundNumber = -1;
		dispose();
	END
}


/*******************************
 *
 *	Misc
 *
/*******************************/

int	launchHacker( void )
{
// returns TRUE if successful, FALSE if not.
#define MAXLINE (MAX_PATH +10)
#define HACKEREXESTR "C64.EXE GAME05.C64"
	char	prevDir[MAX_PATH];
	char	path[MAX_PATH];
	int		result;
	int		soundNumber = -1;
	
	// change directories to hacker directory
	GetCurrentDirectory( MAX_PATH, prevDir );
	strcpy( path, prevDir );
	strcat( path, "\\HACKER" );
	SetCurrentDirectory( path );

	// run Hacker
	if( sound1->isPlaying() )
	{
		soundNumber = sound1->number;
		sound1->stop();
	}
	else if( theMusic->isPlaying() )
	{
		soundNumber = theMusic->number;
		theMusic->stop();
	}
	sfxDisableSound();
	sfxDisableMusic();
	user->canInput( FALSE );
	result = sfxRunProgram( HACKEREXESTR );

	// restore
	/*
	sfxPrintf( "\n\n\n\n\n"
	           "               One  Moment  Please                  "
			   "\n\n\n\n\n"
			   "            Click OK After Playing Hacker              "
			   "\n\n\n\n\n"
			   "                %s"
			   "\n\n",  result?"OK":"ERROR"  );
	*/
	SetCurrentDirectory( prevDir );
	sfxEnableSound();
	sfxEnableMusic();
	if( soundNumber != -1 )
		startSound( soundNumber, PLAYWAVEMIDI );
	user->canInput( TRUE );
	return TRUE;
}


/* DEBUG */
#ifdef _DEBUG
	YingStuffToggle::YingStuffToggle()
	{
		name = "YingStuffToggle";
		init( 4, 52, 25, 35 );
	}

	YingStuffToggle::doVerb( int theVerb )
	{
		if( GameFlag.test( fYingStuffArrived ) ) {
			GameFlag.clear( fYingStuffArrived );
			//sfxPrintf( "Cleared fYingStuffArrived" );
		}
		else {
			GameFlag.set( fYingStuffArrived );
			//sfxPrintf( "Set fYingStuffArrived" );
		}
		return TRUE;
	}
#endif	
