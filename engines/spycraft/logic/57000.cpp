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

#include "flag.h"
#include "globals.h"
#include "game.h"
#include "verbs.h"
#include "views.h"
#include "57000.h"
#include "57100.h"
#include "1000.h"
#include "movie.h"
#include "sound.h"
#include "60000.h"
#include "vlink.h"

namespace Spycraft {

#define CRSBLINK_SPEED 5

/*********************
*
*		Room Code 
*
**********************/

extern Intrface* intrface;
extern SouthCursor* southCursor;
extern WestCursor* westCursor;
extern BlinkCursor* normalCursor;
extern DialTree* dialogTree;
extern Flag GameFlag;
extern Sound* sound1;

Sound* snd57000;

Dialog* 	birdsongDialog1;
Dialog*		birdsongDialog2;
Dialog*		birdsongDialog3;
Dialog*		birdsongDialog4;
Dialog*		birdsongDialog5;
//Dialog*	birdsongDialog6;
Dialog*		birdsongDialog7;

DItem* needAssassination;
DItem* butDanger;
DItem* terriblePaid; 
DItem* askInfo; 
DItem* choiceMafioso; 
DItem* meetingHelp; 
DItem* thinkAsk;
DItem* seeWhile;
 
enum	
{
	NEEDASSASSINATION,
	BUTDANGER,
	TERRIBLEPAID,
	ASKINFO,
	CHOICEMAFIOSO,
	MEETINGHELP,
	THINKASK,
	SEEWHILE
};

Rm57000::Rm57000()
{
	name = "Rm57000";
	show_style = VE_FLIP;
}

void Rm57000::cue( void )
{
///	theMovie->fromTo(127, 128);
///	theMovie->play("57001.avi", this, 0, 0, 0);
}

Rm57000::~Rm57000()
{
	sound1->stop();  
	if( GameFlag.test( fDisposeDialog57000 )) {
		birdsongDialog1->dispose();
		birdsongDialog2->dispose();
		birdsongDialog3->dispose();
		birdsongDialog4->dispose();
		birdsongDialog5->dispose();
//		birdsongDialog6->dispose();
		birdsongDialog7->dispose();

		GameFlag.clear( fDisposeDialog57000 );
	}	//endif
	
	if(GameFlag.test(fWahKa70)) {
		snd57000->dispose();
		GameFlag.clear(fWahKa70);
	}
}

void Rm57000::init()
{						  
    Room::init();

  	new(SouthExit57000);
	new(TopWestExit57000);
	new(BottomWestExit57000);
	new(StatueAndLamp57000);
	new(BookOnTable57000);
	new(BearPhoto57000);
	new(HorseStatue57000);
	new(Calendar57000);

  	intrface->show();

	if( !GameFlag.test( fBeenTo57000 )) {
//		drawPic( 1 );
////	drawPic( 56300 );
   		setScript( new EnterScript57000 );
	}
	else if(GameFlag.test(fBSLangeEnter) && !GameFlag.test(fWahKa70)) {
		GameFlag.set(fWahKa70);
		drawPic( 57000 );
		theGame->handsOn();

		if( sound1->number != 57000  || !sound1->isPlaying() )	{
			sound1->loop = -1; 
			sound1->playBuffered(57000, 0);
		}

		snd57000 = new Sound;
		setScript( new ArrestPlayerScript57000 );
	}
	else {
  		drawPic( 57000 );
		theGame->handsOn();
	
		if( sound1->number != 57000  || !sound1->isPlaying() )	{
			sound1->loop = -1; 
			sound1->playBuffered(57000, 0);
		}
	}
}

int Rm57000::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}

SouthExit57000::SouthExit57000()
{
	name = "SouthExit57000";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
}

void SouthExit57000::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit57000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm1000);
    	return TRUE;
	}
	return FALSE;
}

TopWestExit57000::TopWestExit57000()
{
	name = "TopWestExit57000";
	setHotspot( DO_V );
//	init(WEST_EXIT_X, WEST_EXIT_Y, WEST_EXIT_WIDTH, WEST_EXIT_HEIGHT);  
	init(0, 0, 32, 125);
}

void TopWestExit57000::respond()
{
	theGame->setCursor(westCursor);
	aniCursor();
}

int TopWestExit57000::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
    	theGame->newRoom( new Rm57100 );
    	return TRUE;
	}
	return FALSE;
}

BottomWestExit57000::BottomWestExit57000()
{
	name = "BottomWestExit57000";
	setHotspot( DO_V );
//	init(WEST_EXIT_X, WEST_EXIT_Y, WEST_EXIT_WIDTH, WEST_EXIT_HEIGHT);  
	init(0, 200, 32, 360);
}

void BottomWestExit57000::respond()
{
	theGame->setCursor(westCursor);
	aniCursor();
}

int BottomWestExit57000::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
    	theGame->newRoom( new Rm57100 );
    	return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Features
 *
/*******************************/

StatueAndLamp57000::StatueAndLamp57000()
{
	name = "StatueAndLamp57000";
	setHotspot( DO_V );
	init( 416, 150, 60, 80 );
}

void StatueAndLamp57000::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int StatueAndLamp57000::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		GameFlag.set(fNoticedWestern);
		new StatueAndLampInset57000;
		return TRUE;
	}
	return FALSE;
}

BookOnTable57000::BookOnTable57000()
{
	name = "BookOnTable57000";
	setHotspot( DO_V );
	init( 231, 300, 45, 30 );
}

void BookOnTable57000::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int BookOnTable57000::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		GameFlag.set(fNoticedWestern);
		new BookOnTableInset57000;
		return TRUE;
	}
	return FALSE;
}

BearPhoto57000::BearPhoto57000()
{
	name = "BearPhoto57000";
	setHotspot( DO_V );
	init( 156, 100, 50, 55 );
}

void BearPhoto57000::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int BearPhoto57000::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		GameFlag.set(fNoticedWestern);
		new BearPhotoInset57000;
		return TRUE;
	}
	return FALSE;
}

HorseStatue57000::HorseStatue57000()
{
	name = "HorseStatue57000";
	setHotspot( DO_V );
	init( 266, 180, 60, 55 );
}

void HorseStatue57000::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int HorseStatue57000::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		GameFlag.set(fNoticedWestern);
		new HorseStatueInset57000;
		return TRUE;
	}
	return FALSE;
}

Calendar57000::Calendar57000()
{
	name = "Calendar57000";
	setHotspot( DO_V );
	init( 14, 135, 60, 60 );
}

void Calendar57000::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Calendar57000::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		GameFlag.set(fNoticedWestern);
		new CalendarInset57100;
		return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Insets
 *
/*******************************/

StatueAndLampInset57000::StatueAndLampInset57000()
{
	normalCursor->rest();	
	init( 57010, 0, 16, 512, 384 );
}

int StatueAndLampInset57000::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
    	return TRUE;
	}
	return FALSE;
}

BookOnTableInset57000::BookOnTableInset57000()
{
	normalCursor->rest();		
	init( 57020, 0, 16, 512, 384 );
}

int BookOnTableInset57000::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
    	return TRUE;
	}
	return FALSE;
}

BearPhotoInset57000::BearPhotoInset57000()
{
	normalCursor->rest();	
	init( 57030, 0, 16, 512, 384 );
}

int BearPhotoInset57000::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
    	return TRUE;
	}
	return FALSE;
}

HorseStatueInset57000::HorseStatueInset57000()
{
	normalCursor->rest();		
	init( 57040, 0, 16, 512, 384 );
}

int HorseStatueInset57000::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
    	return TRUE;
	}
	return FALSE;
}

CalendarInset57000::CalendarInset57000()
{
	normalCursor->rest();	
	init( 57120, 0, 16, 512, 384 );
}

int CalendarInset57000::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
    	return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Scripts
 *
/*******************************/

EnterScript57000::EnterScript57000()
{
	name = "EnterScript57000"; 
}

void EnterScript57000::changeState( int newState )
{
	switchTo
//		GameFlag.set(fBeenTo57000);
//		GameFlag.set(fMetBirdSong);
		GameFlag.set(fDisposeDialog57000);

//		theMovie->fromTo(0, 154);
		theMovie->fromTo(0, 132);
		theMovie->play("57001.avi", this, 0, 0, 0);
	END

	BEG
		dialogTree = new DialTree;
		birdsongDialog1 = new Dialog;
		birdsongDialog2 = new Dialog;
		birdsongDialog3 = new Dialog;
		birdsongDialog4 = new Dialog;
		birdsongDialog5 = new Dialog;
//		birdsongDialog6 = new Dialog;
		birdsongDialog7 = new Dialog;

		needAssassination = new DItem;
		needAssassination->addInfo(57000, NEEDASSASSINATION, birdsongDialog2);
		butDanger = new DItem;
		butDanger->addInfo(57000, BUTDANGER, birdsongDialog3 );
//		terriblePaid = new DItem;
		terriblePaid = new BetrayMaxKneecaps;
		terriblePaid->addInfo(57000, TERRIBLEPAID, birdsongDialog5 );
//		askInfo = new DItem;
//		askInfo->addInfo(57000, ASKINFO, birdsongDialog6 );
		askInfo = new LiveMax;
		askInfo->addInfo(57000, ASKINFO, birdsongDialog4 );
//		choiceMafioso = new DItem;
		choiceMafioso = new BetrayMaxKneecaps;
		choiceMafioso->addInfo(57000, CHOICEMAFIOSO, birdsongDialog5 );
//		meetingHelp = new DItem;
//		meetingHelp->addInfo(57000, MEETINGHELP, birdsongDialog6 );
		meetingHelp = new LiveMax;
		meetingHelp->addInfo(57000, MEETINGHELP, birdsongDialog4 );
//		thinkAsk = new DItem;
		thinkAsk = new BetrayMaxMafia;
		thinkAsk->addInfo(57000, THINKASK, birdsongDialog4 );
//		seeWhile = new DItem;
//		seeWhile->addInfo(57000, SEEWHILE, birdsongDialog6 );
		seeWhile = new LiveMax;
		seeWhile->addInfo(57000, SEEWHILE, birdsongDialog4 );

		birdsongDialog1->addMovieInfo(0, 132, 0, 0);
		birdsongDialog2->addMovieInfo(165, 437, 0, 0);
		birdsongDialog3->addMovieInfo(465, 551, 0, 0);
		birdsongDialog4->addMovieInfo(570, 724, 0, 0);
		birdsongDialog5->addMovieInfo(735, 1163, 0, 0); 
		birdsongDialog7->addMovieInfo(1170, 1233, 0, 0);

		birdsongDialog1->add(needAssassination);
		birdsongDialog2->add(butDanger);
		birdsongDialog2->add(terriblePaid);
		birdsongDialog2->add(askInfo);
		birdsongDialog3->add(choiceMafioso);
		birdsongDialog3->add(meetingHelp);
		birdsongDialog7->add(thinkAsk);
		birdsongDialog7->add(seeWhile);

		dialogTree->rootNode = birdsongDialog1;
		dialogTree->resourceID = "57001.avi";
		dialogTree->show(this);
	END

	BEG
		GameFlag.set(fBeenTo57000);
		GameFlag.set(fMetBirdSong);
		GameFlag.clear(fDCK1);			//wahka - get rid of BS in travelink
/*
		if(!GameFlag.test(fSolvedDubanskyKAT))
			new EMail("From Holt, D. - Find the Shooter", 98152, 0, 0 );	
		else
			new EMail("From Holt, D. - Washington Waits", 98153, 0, 0 );	
*/

		theMovie->caller = 0;
		theMovie->stop();
		intrface->enable();
		theGame->newRoom( new Rm1000 );
	END
}

ArrestPlayerScript57000::ArrestPlayerScript57000()
{
	name = "ArrestPlayerScript57000"; 
}

void ArrestPlayerScript57000::changeState( int newState )
{
	switchTo
		seconds = 30;
	END
	BEG								//player stayed in apt too long after shooting Lange
		theGame->handsOff();
//		snd57000->play(579, this);	//sirens
		snd57000->play(56015, this);	//sirens
	END
	BEG
		sound1->stop();
		ticks = 2;
	END
	BEG
	   theGame->newRoom(new Rm60000(END_JAIL));
	END
}

/*******************************
 *
 *	Dialog
 *
/*******************************/
BetrayMaxMafia::BetrayMaxMafia()
{
	name = "BetrayMaxMafia";
}

void BetrayMaxMafia::doit()
{
	GameFlag.set(fBetrayMaxMafia);	  	 	//betray Max by mafia
	DItem::doit();
}

BetrayMaxKneecaps::BetrayMaxKneecaps()
{
	name = "BetrayMaxKneecaps";
}

void BetrayMaxKneecaps::doit()
{
	GameFlag.set(fBetrayMaxKneecaps);	  	//betray Max by Kneecaps
	DItem::doit();
}

LiveMax::LiveMax()
{
	name = "LiveMax";
}

void LiveMax::doit()
{
	GameFlag.set(fBSTrue);				  	//BS remain true, Max will live
	DItem::doit();
}

 
} // namespace Spycraft 
