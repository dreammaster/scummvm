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
//#include "spycraft/logic/41000.h"
#include "spycraft/logic/41100.h"
#include "spycraft/logic/37000.h"
#include "spycraft/logic/37200.h"
#include "spycraft/logic/1000.h"
#include "flag.h"
#include "vlink.h"
#include "badman.h"
#include "spycraft/logic/60000.h"
#include "invent.h"

namespace Spycraft {

#define CRSBLINK_SPEED 5

/*********************
*
*		Room Code 
*
**********************/

extern Intrface* intrface;
extern BlinkCursor* normalCursor;
extern SouthCursor* southCursor;
extern NorthCursor* northCursor;
extern short curDisc;

Rm41100::Rm41100()
{
	name = "Rm41100";
	show_style = VE_FLIP;
}

Rm41100::~Rm41100()
{
}

void Rm41100::init()
{
// 	drawPic( 41100 );
	drawPic( 1 );
    Room::init();
  	
	new(SouthExit41100);
	new(NorthExit41100);

	new(SetFlag41100);		//wahka
	new(SetFlag41101);
	new(SetFlag41102);
	new(SetFlag41103);
	new(SetFlag41104);
	new(SetFlag41105);
	new(SetFlag41106);
	new(SetFlag41107);
	new(SetFlag41108);
	new(SetFlag41109);
	new(SetFlag41110);
	new(SetFlag41111);
	new(SetFlag41112);
	new(SetFlag41113);
	new(SetFlag41114);

	GameFlag.set(fMoscowAvailable);	//make Moscow available in travelink
	curDisc = 1;					//curDisc = EURODISC;
	curMap = MAP_MOSCOW;			//curMap = moscow

	GameFlag.set(10);				//flags for book club of the month
	GameFlag.set(11);				//infosci
	GameFlag.set(43);				//infosci
	inventry->get( iEBMCATALOG );   //electronic book of the month club
	
	intrface->show();
  	theGame->handsOn();
}

int Rm41100::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}

SouthExit41100::SouthExit41100()
{
	name = "SouthExit41100";
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
    setHotspot ( DO_V );
}

void SouthExit41100::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit41100::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
//    	theGame->newRoom(new Rm41000);
		theGame->newRoom(new Rm1000);
    	return TRUE;
	}
	return FALSE;
}

NorthExit41100::NorthExit41100()
{
	name = "NorthExit41100";
	init(NORTH_EXIT_X, NORTH_EXIT_Y, NORTH_EXIT_WIDTH, NORTH_EXIT_HEIGHT);
    setHotspot ( DO_V );
}

void NorthExit41100::respond()
{
	theGame->setCursor(northCursor);
	aniCursor();
}

int NorthExit41100::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm41200);
    	return TRUE;
	}
	return FALSE;
}

SetFlag41100::SetFlag41100()
{
	name = "CapturedYing";
	setHotspot( DO_V );
	init( 50, 50, 50, 25 );
}

void SetFlag41100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int SetFlag41100::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		GameFlag.set(fYingCaptured);
		inventry->get( iLOCKPICK );
		GameFlag.set(fSaw646);					//to do Ying torture
		sfxPrintf("Captured Ying, lock set, saw 646.avi.");
		return TRUE;
	}
	return FALSE;
}

SetFlag41101::SetFlag41101()
{
	name = "CompletedKAT";
	setHotspot( DO_V );
	init( 50, 75, 50, 25 );
}

void SetFlag41101::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int SetFlag41101::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		GameFlag.set(fPEGBriefing);
		GameFlag.set(fSolvedDubanskyKAT); 
		GameFlag.set(fSaw139);					 //for the envelope to show up.
		GameFlag.set(fWahKa84);
//		new EMail("Additional Photo of Harmonica", 98130, fWahKa84, 0 ); //to do Max about Yuri only
		sfxPrintf("Completed KAT and saw 139.avi/98130.htm.");
//wahka - try removing vlinks and emails.
		comlinks->removeEMail(98002);
		return TRUE;
	}
	return FALSE;
}

SetFlag41102::SetFlag41102()
{
	name = "CompletedPhotoDoc";
	setHotspot( DO_V );
	init( 50, 100, 50, 25 );
}

void SetFlag41102::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int SetFlag41102::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		GameFlag.set(fDidPhotoDoc);
//		GameFlag.set(fWahKa99);
		sfxPrintf("Completed PhotoDoc.");
		return TRUE;
	}
	return FALSE;
}

SetFlag41103::SetFlag41103()
{
	name = "TalkedtoMax";
	setHotspot( DO_V );
	init( 250, 50, 50, 25 );
}

void SetFlag41103::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int SetFlag41103::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		GameFlag.set(fMORoomEnter);			 //Max's dialogue
		GameFlag.set(fBSMeetingSet);
		GameFlag.set(fMetMaxFoster);
		GameFlag.set(fWahKa43);
		inventry->get(iNUMBERTOYURI);
		sfxPrintf("Talked to Max about BS and Yuri.");
		return TRUE;
	}
	return FALSE;
}	   

SetFlag41104::SetFlag41104()
{
	name = "TalkedtoBlake";
	setHotspot( DO_V );
	init( 250, 75, 50, 25 );
}

void SetFlag41104::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int SetFlag41104::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		GameFlag.set(fBlakePatioTalkDone);

		sfxPrintf("Talked to Blake.");	
		return TRUE;
	}
	return FALSE;
}

SetFlag41105::SetFlag41105()
{
	name = "TalkedtoBirdsong";
	setHotspot( DO_V );
	init( 250, 100, 50, 25 );
}

void SetFlag41105::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int SetFlag41105::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		GameFlag.set(fBSMeetingSet);	
		GameFlag.set(fBeenTo57000);	
		GameFlag.set(fMetBirdSong);
		sfxPrintf("Talked to Birdsong.");
		return TRUE;
	}
	return FALSE;
}

SetFlag41106::SetFlag41106()
{
	name = "TalkedtoYuri";
	setHotspot( DO_V );
	init( 250, 125, 50, 25 );
}

void SetFlag41106::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int SetFlag41106::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		GameFlag.set(fYORoomEnter);	
		GameFlag.set(fOnce35001);
		sfxPrintf("Talked to Yuri.");
		return TRUE;
	}
	return FALSE;
}

SetFlag41107::SetFlag41107()
{
	name = "BSDisappears";
	setHotspot( DO_V );
	init( 250, 150, 50, 25 );
}

void SetFlag41107::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int SetFlag41107::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		GameFlag.set(fMaxReportBSDis);			 		
		sfxPrintf("Max reports BS disappearance.");
		return TRUE;
	}
	return FALSE;
}

SetFlag41108::SetFlag41108()
{
	name = "DoneSoundAnalysis";
	setHotspot( DO_V );
	init( 250, 175, 50, 25 );
}

void SetFlag41108::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int SetFlag41108::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		GameFlag.set(fDoneSoundAnal);					
		sfxPrintf("Completed Sound Analysis.");
		return TRUE;
	}
	return FALSE;
}

SetFlag41109::SetFlag41109()
{
	name = "VisitedVilinuis";
	setHotspot( DO_V );
	init( 250, 200, 50, 25 );
}

void SetFlag41109::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int SetFlag41109::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		GameFlag.set(fVCRoomEnter);					
		GameFlag.set(fHaveSwanTwo);
		GameFlag.set(fHaveVideoCamera);	

		inventry->get(iVIDEO_CAMERA);
		inventry->get(iMUSICSWAN);
		sfxPrintf("Visited Vilinius Center.");
		return TRUE;
	}
	return FALSE;
}

SetFlag41110::SetFlag41110()
{
	name = "BrokenYing";
	setHotspot( DO_V );
	init( 250, 225, 50, 25 );
}

void SetFlag41110::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int SetFlag41110::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		GameFlag.set(fBrokenYingInterrogation);

		GameFlag.set(fPyramidInfoAvailable);
//		new VLink( "25003.avi", 0, 0);			//vlink 654, Look into other thefts
//		new VLink( "25004.avi", 0, 10);			//vlink 655, Skodi details
//		GameFlag.set(fWahKa19);					//link to vlink 205, Warhurst
		GameFlag.set(fWahKa32);					//link to vlink 657, Jaimie

//		new EMail( "Killers for Hire: Seaton, Jaimie A.", 98070);
		
		sfxPrintf("Broken Ying.");
		return TRUE;
	}
	return FALSE;
}

SetFlag41111::SetFlag41111()
{
	name = "TurnedProcat";
	setHotspot( DO_V );
	init( 250, 250, 50, 25 );
}

void SetFlag41111::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int SetFlag41111::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		GameFlag.set(fWahKa54);
		GameFlag.set(fWahKa11);
		GameFlag.set(fWahKa21);
		GameFlag.set(fHarmCalls);
		GameFlag.set(fMoscowAlleyAvailable);
		GameFlag.set(fWahKa62);
		GameFlag.set(fPlayerTurnsProcat);

		sfxPrintf("Turned Procat.");
		return TRUE;
	}
	return FALSE;
}

SetFlag41112::SetFlag41112()
{
	name = "RestartRussia";
	setHotspot( DO_V );
	init( 250, 275, 50, 25 );
}

void SetFlag41112::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int SetFlag41112::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		GameFlag.clear(fYingCaptured);
		GameFlag.clear(fSaw646);
		inventry->get( iLOCKPICK );
		
		GameFlag.clear(fPEGBriefing);
		GameFlag.clear(fSolvedDubanskyKAT); 
		GameFlag.clear(fSaw139);
		GameFlag.clear(fWahKa84);
////	GameFlag.clear(fWahKa99);
		GameFlag.clear(fPrintedPhoto);	  //did but incorrectly
		GameFlag.clear(fDidPhotoDoc);
		
		GameFlag.clear(fMORoomEnter);			 
		GameFlag.clear(fBSMeetingSet);
		GameFlag.clear(fMetMaxFoster);

		GameFlag.clear(fWahKa43);
//		inventry->get(iNUMBERTOYURI);
		GameFlag.clear(fBlakePatioTalkDone);				
		
		GameFlag.clear(fBSMeetingSet);	
		GameFlag.clear(fBeenTo57000);	
		GameFlag.clear(fMetBirdSong);
		
		GameFlag.clear(fYORoomEnter);	
		GameFlag.clear(fOnce35001);
		
		GameFlag.clear(fMaxReportBSDis);
		GameFlag.clear(fDoneSoundAnal);

		GameFlag.clear(fVCRoomEnter);					
		GameFlag.clear(fHaveSwanTwo);
		GameFlag.clear(fHaveVideoCamera);	
		
		GameFlag.clear(fBrokenYingInterrogation);
		GameFlag.clear(fPyramidInfoAvailable);
		GameFlag.clear(fDoneInterrogation);
		GameFlag.clear(fOnceBullpen25000);

								
		GameFlag.clear(fWahKa54);
		GameFlag.clear(fWahKa11);
		GameFlag.clear(fWahKa21);
		GameFlag.clear(fHarmCalls);
		GameFlag.clear(fMoscowAlleyAvailable);
		GameFlag.clear(fWahKa62);
		GameFlag.clear(fPlayerTurnsProcat);

		GameFlag.clear(fWahKa40);
		GameFlag.clear(fWahKa41);
		GameFlag.clear(fWahKa42);
		GameFlag.clear(fWahKa31);
		GameFlag.clear(fMetMaxFoster);
		
//		new EMail( "OnyxHuntOne98330", 98330);
//		new EMail( "OnyxHuntTwo98331", 98331);
		GameFlag.clear(fCanShowCamera);
		GameFlag.clear(fCarrotPath);
		GameFlag.clear(fStickPath);
		GameFlag.clear(fSawKneeAddress);
		GameFlag.clear(fKnowKneeHasBird);

		GameFlag.clear(fWahKa51);
		GameFlag.clear(fWahKa52);
		GameFlag.clear(fWahKa53);
		GameFlag.clear(fWahKa54);
		
		for(unsigned i=0; i==700; i++) { 
			GameFlag.clear(i);
		}

		sfxPrintf("Reset key flags in Russia.");
		return TRUE;
	}
	return FALSE;
}

SetFlag41113::SetFlag41113()
{
	name = "VideoCamera";
	setHotspot( DO_V );
	init( 250, 300, 50, 25 );
}

void SetFlag41113::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int SetFlag41113::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
//		GameFlag.set(fHaveVideoCamera);	
		GameFlag.set(fCanShowCamera);
		GameFlag.set(fCarrotPath);

//		inventry->get(iVIDEO_CAMERA);
		sfxPrintf("Put video camera on desk.");
		return TRUE;
	}
	return FALSE;
}

SetFlag41114::SetFlag41114()
{
	name = "ID Kneecaps";
	setHotspot( DO_V );
	init( 250, 325, 50, 25 );
}

void SetFlag41114::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int SetFlag41114::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		GameFlag.set(fSawKneeAddress);
		GameFlag.set(fKnowKneeHasBird);
		GameFlag.set(fCarrotPath);

		sfxPrintf("Kneecaps apt available.");
		return TRUE;
	}
	return FALSE;
}

SetFlag41115::SetFlag41115()
{
	name = "MakeBSaptAvailable";
	setHotspot( DO_V );
	init( 250, 50, 50, 25 );
}

void SetFlag41115::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int SetFlag41115::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		GameFlag.set(fDCK1);		
		sfxPrintf("Make BS apt available in travelink.");
		return TRUE;
	}
	return FALSE;
}	   

SetFlag41116::SetFlag41116()
{
	name = "MakeYasevnoAvailable";
	setHotspot( DO_V );
	init( 250, 75, 50, 25 );
}

void SetFlag41116::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int SetFlag41116::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		GameFlag.set(fDCK2);		
		sfxPrintf("Make Yasevno available in travelink.");
		return TRUE;
	}
	return FALSE;
}

SetFlag41117::SetFlag41117()
{
	name = "SetFlag41117";
	setHotspot( DO_V );
	init( 250, 100, 50, 25 );
}

void SetFlag41117::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int SetFlag41117::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{		
		sfxPrintf(".");
		return TRUE;
	}
	return FALSE;
}

SetFlag41118::SetFlag41118()
{
	name = "SetFlag41118";
	setHotspot( DO_V );
	init( 250, 125, 50, 25 );
}

void SetFlag41118::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int SetFlag41118::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{		
		sfxPrintf(".");
		return TRUE;
	}
	return FALSE;
}
SetFlag41119::SetFlag41119()
{
	name = "SetFlag41119";
	setHotspot( DO_V );
	init( 250, 125, 50, 25 );
}

void SetFlag41119::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int SetFlag41119::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{		
		sfxPrintf(".");
		return TRUE;
	}
	return FALSE;
}

SetFlag41120::SetFlag41120()
{
	name = "SetFlag41120";
	setHotspot( DO_V );
	init( 250, 125, 50, 25 );
}

void SetFlag41120::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int SetFlag41120::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{		
		sfxPrintf(".");
		return TRUE;
	}
	return FALSE;
}

Rm41200::Rm41200()
{
	name = "Rm41200";
	show_style = VE_FLIP;
}

Rm41200::~Rm41200()
{
}

void Rm41200::init()
{
	drawPic( 1 );
////drawPic( 41200 );
    Room::init();
  
	new(SouthExit41200);
//	new(NorthExit41200);
	new(SetFlag41115);
	new(SetFlag41116);
	
  	intrface->show();
  	theGame->handsOn();
}

int Rm41200::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}

SouthExit41200::SouthExit41200()
{
	name = "SouthExit41200";
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
    setHotspot ( DO_V );
}

void SouthExit41200::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit41200::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm41100);
////	theGame->newRoom(new Rm1000);
    	return TRUE;
	}
	return FALSE;
}

NorthExit41200::NorthExit41200()
{
	name = "NorthExit41200";
	setHotspot ( DO_V );
	init(96, 30, 313, 335);
//	init(NORTH_EXIT_X, NORTH_EXIT_Y, NORTH_EXIT_WIDTH, NORTH_EXIT_HEIGHT);
    
}

void NorthExit41200::respond()
{
	theGame->setCursor(northCursor);
	aniCursor();
}

int NorthExit41200::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm41300);
    	return TRUE;
	}
	return FALSE;
}

Rm41300::Rm41300()
{
	name = "Rm41300";
	show_style = VE_FLIP;
}

Rm41300::~Rm41300()
{
}

void Rm41300::init()
{
// 	drawPic( 41300 );
	drawPic( 1 );
    Room::init();
  
	new(SouthExit41300);
	new(NorthExit41300);
	
  	intrface->show();
  	theGame->handsOn();
}

int Rm41300::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}

SouthExit41300::SouthExit41300()
{
	name = "SouthExit41300";
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
    setHotspot ( DO_V );
}

void SouthExit41300::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit41300::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
//    	theGame->newRoom(new Rm41200);
		theGame->newRoom(new Rm37000);
    	return TRUE;
	}
	return FALSE;
}

NorthExit41300::NorthExit41300()
{
	name = "NorthExit41300";
	setHotspot ( DO_V );
	init(200, 220, 117, 111);	
//	init(NORTH_EXIT_X, NORTH_EXIT_Y, NORTH_EXIT_WIDTH, NORTH_EXIT_HEIGHT);
}

void NorthExit41300::respond()
{
	theGame->setCursor(northCursor);
	aniCursor();
}

int NorthExit41300::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm41400);
    	return TRUE;
	}
	return FALSE;
}

Rm41400::Rm41400()
{
	name = "Rm41400";
	show_style = VE_FLIP;
}

Rm41400::~Rm41400()
{
}

void Rm41400::init()
{
// 	drawPic( 41400 );
	drawPic( 1 );
    Room::init();
  
	new(SouthExit41400);
	new(Monitor41400);
		
  	intrface->show();
  	theGame->handsOn();
}

int Rm41400::handleEvent ( MADEEventStamp *event )
{
	return FALSE;
}

SouthExit41400::SouthExit41400()
{
	name = "SouthExit41400";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
}

void SouthExit41400::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit41400::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm41300);
    	return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Features
 *
/*******************************/

Monitor41400::Monitor41400()
{
	name = "Monitor41400";
    setHotspot ( DO_V );
	init(118, 32, 95, 151);
}

void Monitor41400::respond()
{
	theGame->setCursor(normalCursor);
	normalCursor->blink( CRSBLINK_SPEED );
}

int Monitor41400::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm41500);
    	return TRUE;
	}
	return FALSE;
}

Rm41500::Rm41500()
{
	name = "Rm41500";
	show_style = VE_FLIP;
}

Rm41500::~Rm41500()
{
}

void Rm41500::init()
{
  	drawPic( 41500 );
    Room::init();
  
//disable the south exit because it is too close to the green button
//	new(SouthExit41500);
	new(Screen41500);
	
  	intrface->show();
  	theGame->handsOn();
}

int Rm41500::handleEvent ( MADEEventStamp *event )
{

	return FALSE;
}

SouthExit41500::SouthExit41500()
{
	name = "SouthExit41500";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
}

void SouthExit41500::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit41500::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
    	theGame->newRoom(new Rm41400);
    	return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Features
 *
/*******************************/

Screen41500::Screen41500()
{
	name = "Screen41500";
	setHotspot( DO_V );
//	init( 128, 156, 215, 167 );
	init( 175, 375, 19, 23 ); 
}

void Screen41500::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Screen41500::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		normalCursor->rest();
		curRoom->setScript( new EnterBadmanPuzzle41500 );
//		curRoom->setScript( new EnterScript41500 );
		return TRUE;
	}
	return FALSE;
}

/*******************************
 *
 *	Scripts
 *
/*******************************/
EnterBadmanPuzzle41500::EnterBadmanPuzzle41500()
{
	name = "EnterBadmanPuzzle41500"; 
}

void EnterBadmanPuzzle41500::changeState( int newState )
{
	switchTo
		theGame->handsOff();
		theMovie->play("41502.avi", this, 0, 1, 0);
	END
	BEG
		theGame->handsOn();
		dispose();
//		theGame->newRoom(new Rm1000);
		curRoom->setScript( new EnterScript41500 );
	END
}

EnterScript41500::EnterScript41500()
{
	name = "EnterScript41500"; 
}

void EnterScript41500::changeState( int newState )
{
	switchTo
//		sfxPrintf( "Badman Puzzle-Rescue BS" );  
		if (pdaProp) {
				intrface->blinking = FALSE;
				intrface->release(pdaProp);
				pdaProp->setCycle(0);
				pdaProp->setHotspot(NULL);
				delete pdaProp;
				pdaProp = NULL;
		}
		curRoom->drawPic( -1 );
		new(BadMan);
		ticks=2;
	END
	BEG
		ticks = 15;
	END
	BEG
//commented out vlink for walk
//		new VLink( "41100.avi", fWahKa27, 0);		 	//vlink 686, all clear 
//		if(GameFlag.test(fWahKa8))
		if(GameFlag.test(fRescuedBS)) {
			GameFlag.set(fWahKa100);
			theGame->newRoom(new Rm37200);				//Kneecaps' Pad interior
		}
		else if(GameFlag.test(fWahKa36)){
			theGame->newRoom(new Rm60000(END_DEAD));
		}
		else if(GameFlag.test(fWahKa37)) {
			theGame->newRoom(new Rm60000(END_JAIL));
		}
		else {
			theGame->newRoom(new Rm60000(END_FIRED));
		}
	END
}
 
} // namespace Spycraft 
