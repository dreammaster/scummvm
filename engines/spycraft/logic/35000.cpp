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
#include "spycraft/game/movie.h"
#include "spycraft/logic/35000.h"
#include "spycraft/logic/1000.h"
//#include "spycraft/logic/35300.h"
#include "spycraft/logic/35400.h"
#include "spycraft/game/invent.h"
#include "spycraft/game/sndanal.h"
#include "spycraft/game/flag.h"
#include "spycraft/game/vlink.h"
#include "spycraft/game/roomsnd.h"
#include "spycraft/game/image.h"
#include "spycraft/game/cypher.h"
//#include "spycraft/logic/37000.h"
#include "spycraft/game/teletalk.h"
#include "spycraft/game/knee.h"
#include "spycraft/game/html.h"
#include "spycraft/game/pcscreen.h"
#include "spycraft/game/phone.h"
#include "spycraft/logic/35700.h"
#include "spycraft/dmade/advsound.h"
#include "spycraft/dmade/advmusic.h"
#include "spycraft/dmade/gamebox.h"
#include "spycraft/logic/24000.h"

namespace Spycraft {

#define CRSBLINK_SPEED 5

extern Flag GameFlag;
extern Invent* inventry;
extern DialTree* dialogTree;
extern Intrface* intrface;
extern SouthCursor* southCursor;
extern NorthCursor* northCursor;
extern WestCursor* westCursor;
extern EastCursor* eastCursor;
extern BlinkCursor* normalCursor;
extern Sound* sound1;
extern Sound* sound2;
extern MADEEnvirons* colbyInfo;
MusicSwanCursor* musicswanCursor;
VoiceLight36000* voicelight35100;
Envelope35100* envelope35100;
TopEnvelope35100* topenvelope35100;
BottomEnvelope35100* bottomenvelope35100;

Sound* snd35000;

/*********************
*
*		Room Code 
*
**********************/

Rm35000::Rm35000()
{
	name = "Rm35000";
	show_style = VE_FLIP;
}

Rm35000::~Rm35000()
{
 	snd35000->dispose();
//	sound1->stop();	   //2/22
//	theMusic->stop();  //2/22
	GameFlag.clear(fWahKa78);
}

void Rm35000::init()
{
	drawPic( 35000 );
	Room::init();
	/*if (GameFlag.test(fWahKa118) && !GameFlag.test(fWahKa119))	{
		GameFlag.set(fWahKa119);
		new EMail("The break we needed: Holt, David M.", 98331);
	}2/22*/
	if(GameFlag.test(fWahKa94))
		new(WideRoomMusicSwan35001);
	if(GameFlag.test(fWahKa95))
		new(WideRoomEnvelope35001);
	if(GameFlag.test(fWahKa96))
		new(WideRoomVideoCamera35001);

	if(strcmp(theGame->prevRoomName, "Rm1000") == 0) {
//	if((strcmp(theGame->prevRoomName, "Rm35100") != 0)  || (strcmp(theGame->prevRoomName, "Rm35400") != 0)) {

// 	if( GameFlag.test(fVCRoomEnter) && !GameFlag.test(fHaveSwanOne)) {		
// 		new( WideRoomOrangeSwan35001 );
//	}
//just comment out, to take out swan one
// 	else if( GameFlag.test(fVCRoomEnter) && GameFlag.test(fOnce35002) && !GameFlag.test(fHaveSwanTwo)) {		
	if( GameFlag.test(fShowSwanTwo) && GameFlag.test(fBrokenYingInterrogation) && !GameFlag.test(fHaveSwanTwo)) {
		GameFlag.set(fWahKa94);
		new( WideRoomMusicSwan35001 );
		sfxLoadRes ( 35102, RES_ATS );
//		sfxLockRes(35102, RES_ATS);
	}
//	if( GameFlag.test(fOnceShocked48000) && !GameFlag.test(fHavePresidentPicture)) {
//		new( WideRoomPresident35001 );			
//	}
	if( GameFlag.test(fMORoomEnter) && GameFlag.test(fLeavingSafeHouse) && GameFlag.test(fSolvedDubanskyKAT) && GameFlag.test(fSaw139) && !GameFlag.test(fHaveHarmEnvelope)) {	
		GameFlag.set(fWahKa95);
		new( WideRoomEnvelope35001 );
		sfxLoadRes ( 35180, RES_ATS );
//		sfxLockRes(35180, RES_ATS);
	}
	if ( GameFlag.test(fMaxDead) && !GameFlag.test(fPickedUpCamera) && !GameFlag.test(fHaveVideoCamera)) {			
		GameFlag.set(fWahKa96);
		new( WideRoomVideoCamera35001 );
		sfxLoadRes ( 35190, RES_ATS );
//		sfxLockRes(35190, RES_ATS);
	}
	if ( GameFlag.test(fCarrotPath) && GameFlag.test(fCanShowCamera) && !GameFlag.test(fHaveVideoCamera)) {			
		if(!GameFlag.test(fWahKa28)) {
			GameFlag.set(fWahKa28);
			new VLink("Birdsong's in trouble: Foster, Maxine M.", "35201.avi", 0, 0, new EMailAttachment("Camera", 98092));
		}
		GameFlag.set(fWahKa96);
		new( WideRoomVideoCamera35001 );
		sfxLoadRes ( 35190, RES_ATS );
	}
	
	}   //end if for prevRoom

//wahka - take out
/*
	new( WideRoomOrangeSwan35001 );
	new( WideRoomMusicSwan35001 );
	new( WideRoomEnvelope35001 );
	new( WideRoomPresident35001 );
	new( WideRoomVideoCamera35001 );
*/

	snd35000 = new Sound;

  	new(SouthExit35000);
	new(FileCabinet35000);
	new(Desk35000);
	
///	new(SetYingPath35000);	   //take out
///	new(SetMaxFlag35000);
//	new(SetMoscowMap35000);
//	new(SetYuriFlag35000);
//	new(SetBSFlag35000);
//	new(SetWalkPathPart1);	 
//	new(SetWalkPathPart2);
//	new(SetWalkPathPart3);

  	intrface->show();
  	theGame->handsOn();
	
	if( GameFlag.test(fMORoomEnter) && GameFlag.test(fLeavingSafeHouse) && GameFlag.test(fSolvedDubanskyKAT) && GameFlag.test(fSaw139) && !GameFlag.test(fWahKa105)) {	
//		if( snd35000->number != 24104  || !snd35000->isPlaying() )	{
//			GameFlag.set(fWahKa105);
//			snd35000->play(24104, 0);			//Jamimie - SoundHack
//		}
		GameFlag.set(fWahKa105);
		new EMail( "Sound Hack is your friend: Seaton, Jaimie A.", 98086 );	// Jaimie: SoundHack
	}

	/*if( colbyInfo->slow_system == true ) {
		theMusic->play ( 35000 );
	}
	else { 
		/*if( sound1->number != 35000  || !sound1->isPlaying() )	{
			sound1->loop = -1; 
			sound1->playBuffered(35000, 0);
		}
	}*/		  //2/22
		
}

void Rm35000::doit()
{
//	if((GameFlag.test(fYORoomEnter) && !GameFlag.test(fVCRoomEnter) && !GameFlag.test(fWahKa51)) ||		//Yuri about VC
//	if((GameFlag.test(fVCRoomEnter) && GameFlag.test(fDoneSoundAnal) && !GameFlag.test(fWahKa52)) ||	//Lean
	if((GameFlag.test(fOnyxHunt1) && !GameFlag.test(fWahKa53) && !GameFlag.test(fPlayerTurnsProcat) && GameFlag.test(fMetBlake)) ||					//Yuri about Onyx
	   (GameFlag.test(fBrokenYingInterrogation) && GameFlag.test(fOnyxHunt2) && !GameFlag.test(fKilledGrendel) && !GameFlag.test(fWahKa38) && !GameFlag.test(fWahKa54))) {  	//Harmonica
		if(!GameFlag.test(fWahKa78) && (snd35000->number != 35127 || snd35000->isPlaying()) ) {
			snd35000->stop();
			if( snd35000->number != 35127  || !snd35000->isPlaying() )	{
				GameFlag.set(fWahKa78);
				snd35000->loop = -1; 
				snd35000->play(35127, 0);			//phone ringing
////			GameFlag.clear(fWahKa78);
			}
		}
	}
	Room::doit();
}

int Rm35000::handleEvent ( MADEEventStamp *event )
{
	return false;
}

SouthExit35000::SouthExit35000()
{
	name = "SouthExit35000";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
}

void SouthExit35000::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit35000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {

	//	sound1->stop();	  //2/22
	//	theMusic->stop(); //2/22
//		GameFlag.set(fLeavingSafeHouse);                     //leaving station house

		if(GameFlag.test(fBrokenYingInterrogation) && GameFlag.test(fOnyxHunt2) && !GameFlag.test(fKilledGrendel) && !GameFlag.test(fWahKa54) && !GameFlag.test(fWahKa38)) {  	//Harmonica
//check in ops mgr//	GameFlag.set(fWahKa54);		//give player one chance to answer the call from Harmonica
			GameFlag.set(fWahKa38);
		}
		
////	if((GameFlag.test(fBSVoiceMessage) && (GameFlag.test(fLeavingSafeHouse)) && (GameFlag.test(fHaveSwanTwo)) && (!GameFlag.test(fOnce33000)))) {	//BS phone message and leaving station house
//need to combine these two vlinks as one
			//MOVED TO FLAG 12/16 new VLink( "53001.avi", 0, 0 );	   		//vlink 325, BS disappear
//			new VLink( "53002.avi", 0, 0 );   		//vlink 333, Max is pleased
			//MOVED TO FLAG 12/16GameFlag.set(fMaxReportBSDis);	 
			//MOVED TO FLAG 12/16GameFlag.set(fOnce33000);
////	}
////	else if((GameFlag.test(fHaveSwanOne)) && (!GameFlag.test(fOnce35002))) {
////		GameFlag.set(fOnce35002);					//gotten swan one,
////	}

//		if(GameFlag.test(fWahKa54) && !GameFlag.test(fWahKa87)) {
		if(GameFlag.test(fHarmCalls) && !GameFlag.test(fWahKa87)) {
			GameFlag.set(fWahKa87);
			theGame->newRoom(new Rm35700);
		}
		else

////	if(GameFlag.test(fDoneSoundAnal) && !GameFlag.test(fWahKa67)) {
////		GameFlag.set(fWahKa67);
//			pcLastUsed[PC_SNDANAL] = false;				
////		new VLink( "35101.avi", 0, 20 );	   //vlink674, direct agents after BS
////	}

		theGame->newRoom(new Rm1000);
    	return true;
	}
	return false;
}

/*******************************
 *
 *	Features
 *
/*******************************/

FileCabinet35000::FileCabinet35000()
{
	name = "FileCabinet35000";
	setHotspot( DO_V );
	init( 151, 153, 37, 87 );
}

void FileCabinet35000::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int FileCabinet35000::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
//		new FileCabinetInset35000();
		theGame->newRoom( new Rm35400);
		return true;
	}
	return false;
}

Desk35000::Desk35000()
{
	name = "Desk35000";
	setHotspot( DO_V );
	init( 118, 227, 284, 123 );
}

void Desk35000::respond()
{
	theGame->setCursor( northCursor );
}

int Desk35000::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		theGame->newRoom( new Rm35100);
		return true;
	}
	return false;
}

//wahka - take out				   
SetYingPath35000::SetYingPath35000()
{
	name = "SetYingPath35000";
	setHotspot( DO_V );
//	init( 50, 50, 50, 25 );
	init( 3, 98, 42, 51);
}

void SetYingPath35000::respond()
{
//	theGame->setCursor( normalCursor );
//	normalCursor->blink( CRSBLINK_SPEED );
}

int SetYingPath35000::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
//		GameFlag.set(fKnowYingInMoscow);	//coming to Moscow asking about Ying
		GameFlag.set(fYingCaptured);
		GameFlag.set(fSolvedDubanskyKAT); 
		sfxPrintf("Captured Ying and finished KAT");
		return true;
	}
	return false;
}

SetMoscowMap35000::SetMoscowMap35000()
{
	name = "VilniusCenter";
	setHotspot( DO_V );
	init( 50, 75, 50, 25 );
}

void SetMoscowMap35000::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int SetMoscowMap35000::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
//		GameFlag.set(fBSMeetingSet);		//BS
//		GameFlag.set(fYORoomEnter);
//		GameFlag.set(fBeenTo57000);		
		GameFlag.set(fVCRoomEnter);
		GameFlag.set(fOnce35000);
//		GameFlag.set(fMORoomEnter);		
		GameFlag.set(fOnce35001);
//		GameFlag.set(fDisplayStakout);
//		GameFlag.set(fDisplayRed);		
//		GameFlag.set(fDisplayKnee);
//		GameFlag.set(fDisplayAir);	
//		sfxPrintf("Show all Moscow sites on world map.");
		GameFlag.set(fHaveSwanOne);
		GameFlag.set(fHaveSwanTwo);	
		GameFlag.set(fDoneSoundAnal);

		inventry->get(iVIDEO_CAMERA);
		inventry->get(iMUSICSWAN);
		sfxPrintf("Done VC - have video camera and swan");
	}
	return false;
}

SetYuriFlag35000::SetYuriFlag35000()
{
	name = "SetYuriFlag35000";
	setHotspot( DO_V );
	init( 50, 100, 50, 25 );
}

void SetYuriFlag35000::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int SetYuriFlag35000::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		GameFlag.set(fYORoomEnter);	
		GameFlag.set(fOnce35001);

		sfxPrintf("Done Yuri's dialog");		 
		return true;
	}
	return false;
}

SetMaxFlag35000::SetMaxFlag35000()
{
	name = "SetMaxFlag35000";
	setHotspot( DO_V );
//	init( 50, 125, 50, 25 );
	init( 270, 20, 64, 57);
}

void SetMaxFlag35000::respond()
{
//	theGame->setCursor( normalCursor );
//	normalCursor->blink( CRSBLINK_SPEED );
}

int SetMaxFlag35000::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		GameFlag.set(fMORoomEnter);			 //Max's dialogue
		GameFlag.set(fBSMeetingSet);
		GameFlag.set(fMetMaxFoster);

		GameFlag.set(fDidPhotoDoc);
		GameFlag.set(fWahKa43);
		inventry->get(iNUMBERTOYURI);
//		sfxPrintf("Done Max's dialog(asked about Yuri)");		
		sfxPrintf("Hi, I'm Max Foster");
		return true;
	}
	return false;
}

SetBSFlag35000::SetBSFlag35000()
{
	name = "SetBSFlag35000";
	setHotspot( DO_V );
	init( 250, 50, 50, 25 );
}

void SetBSFlag35000::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int SetBSFlag35000::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		GameFlag.set(fBSMeetingSet);	
		GameFlag.set(fBeenTo57000);	
		GameFlag.set(fMetBirdSong);
		sfxPrintf("Done Birdsong's dialog");		 
		return true;
	}
	return false;
}

SetWalkPathPart1::SetWalkPathPart1()
{
//	name = "SetWalkPathPart1";
	name = "Finished KAT";
	setHotspot( DO_V );
	init( 250, 75, 50, 25 );
}

void SetWalkPathPart1::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int SetWalkPathPart1::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
/*
		GameFlag.set(fYORoomEnter);		//Yuri's dialog
		GameFlag.set(fOnce35001);
		GameFlag.set(fBSMeetingSet);	//BS's dialog
		GameFlag.set(fBeenTo57000);	
		GameFlag.set(fMetBirdSong);
		GameFlag.set(fMORoomEnter);		//Max's dialogue
		sfxPrintf("Done part one of Russia walkthrough");		 
*/
		GameFlag.set(fSolvedDubanskyKAT);  //test quickly
		sfxPrintf("Finished KAT puzzle");
		return true;
	}
	return false;
}

SetWalkPathPart2::SetWalkPathPart2()
{
//	name = "SetWalkPathPart2";
	name = "Done GrendelStolePEG";
	setHotspot( DO_V );
	init( 250, 100, 50, 25 );
}

void SetWalkPathPart2::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int SetWalkPathPart2::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
/*
		GameFlag.set(fLeavingSafeHouse);		//leaving safe house, vlink 325
		GameFlag.set(fOnce33000);				//vlink 333
		GameFlag.set(fDoneSoundAnal);			//sound analysis
		GameFlag.set(fHaveHarmEnvelope);		//envelope
		GameFlag.set(fMaxReportBSDis);	 		//BS disappear, vlink do once
		sfxPrintf("Done part two of Russia walkthrough");
*/		
		GameFlag.set(fKnowGrendelStolePEG);										 
		sfxPrintf("Done GrendelStolePEG.");

		return true;
	}
	return false;
}

SetWalkPathPart3::SetWalkPathPart3()
{
//	name = "SetWalkPathPart3";
	name = "Wahka 36";
	setHotspot( DO_V );
	init( 250, 125, 50, 25 );
}

void SetWalkPathPart3::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int SetWalkPathPart3::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
//		sfxPrintf("Done part three of Russia walkthrough");	

		GameFlag.set(fWahKa40);	    //one of Max's dialog
		
		GameFlag.set(fYingCaptured);
		GameFlag.set(fBlakePatioTalkDone);
		GameFlag.set(fPlayerTurnsProcat);
		
		GameFlag.set(fMaxReportBSDis);
		sfxPrintf("strange things will happen now.");
		
		GameFlag.set(fWahKa12);  		//cypher puzzles

		GameFlag.clear(fWahKa44);		//file cabinet
		GameFlag.clear(fWahKa45);
		GameFlag.clear(fWahKa46);
		GameFlag.clear(fWahKa47);
		GameFlag.clear(fWahKa48);
		GameFlag.clear(fWahKa49);
		
		inventry->get(iFOLDER_PAYMENTS);

		return true;
	}
	return false;
}

/*******************************
 *
 *	Insets
 *
 *******************************/
FileCabinetInset35000::FileCabinetInset35000()
{
	normalCursor->rest();
 	init(35160, 0, 15, 512, 385);	  //closed file cabinet
}

void FileCabinetInset35000::respond()
{
}

int FileCabinetInset35000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		dispose();
		new FileCabinetInset35001;
    	return true;
	}
	return false;
}

FileCabinetInset35001::FileCabinetInset35001()
{
	normalCursor->rest();
 	init(35170, 0, 15, 512, 385);	//open file cabinet
}

/*******************************
 *
 *	Scripts
 *
 *******************************/
void Script35000::changeState ( int newState )
{
	switchTo
		seconds = 2;
	END
   
    BEG
		dispose();
    END
}

/*******************************
 *
 *	Views
 *
 *******************************/

WideRoomOrangeSwan35001::WideRoomOrangeSwan35001()
{
	name = "WideRoomOrangeSwan35001";
//	setHotspot ( DO_V );
	setView(35001);
//	init(207, 316, true);
	init(212, 248);
	noSkip();
}

WideRoomMusicSwan35001::WideRoomMusicSwan35001()
{
	name = "WideRoomMusicSwan35001";
//	setHotspot ( DO_V );
	setView(35002);
//	init(207, 316, true);
////init(231, 246);
	init(236, 247);
	noSkip();
}

WideRoomPresident35001::WideRoomPresident35001()
{
	name = "WideRoomPresident35001";
//	setHotspot ( DO_V );
	setView(35006);
//	init(207, 316, true);
	init(190, 235);
	noSkip();
}

WideRoomEnvelope35001::WideRoomEnvelope35001()
{
	name = "WideRoomEnvelope35001";
//	setHotspot ( DO_V );
	setView(35003);
//	init(207, 316, true);
	init(259, 278);
	noSkip();
}

WideRoomVideoCamera35001::WideRoomVideoCamera35001()
{
	name = "WideRoomVideoCamera35001";
//	setHotspot ( DO_V );
	setView(35007);
//	init(207, 316, true);
	init(243, 251);
	noSkip();
}

/*********************
*
*		Room Code 
*
**********************/

Rm35100::Rm35100()
{
	name = "Rm35100";
	show_style = VE_FLIP;
}

Rm35100::~Rm35100()
{
	snd35000->dispose();
	GameFlag.clear(fWahKa78);
}

void Rm35100::init()
{
  	drawPic( 35100 );
    Room::init();
	
// 	if( GameFlag.test(fVCRoomEnter) && !GameFlag.test(fHaveSwanOne)) {		 //after Vilnius Center
// 		new(SwanOne35100);
//	}
//	if( GameFlag.test(fVCRoomEnter) && GameFlag.test(fOnce35002) && !GameFlag.test(fHaveSwanTwo)) {	
////if( GameFlag.test(fShowSwanTwo) && !GameFlag.test(fHaveSwanTwo)) {
////	new(SwanTwo35100);	
////}

//	if( GameFlag.test(fOnceShocked48000) && !GameFlag.test(fHavePresidentPicture)) {			
//		new(President35100);				//if player doesn't turn Procat, show picture
//		GameFlag.set(fHavePresidentPicture);
//	}

////if( GameFlag.test(fMORoomEnter) && GameFlag.test(fLeavingSafeHouse) && GameFlag.test(fSolvedDubanskyKAT) && GameFlag.test(fSaw139) && !GameFlag.test(fHaveHarmEnvelope)) {			//appears after leaving station house
////	new(Envelope35100);
//		GameFlag.set(fHaveHarmEnvelope);
////}

////if( GameFlag.test(fMaxDead) && !GameFlag.test(fPickedUpCamera) && !GameFlag.test(fHaveVideoCamera)) {			
////	new(VideoCamera35100);			
//		GameFlag.set(fHaveVideoCamera);
////}
////if ( GameFlag.test(fCarrotPath) && GameFlag.test(fCanShowCamera) && !GameFlag.test(fHaveVideoCamera)) {			
////	new VideoCamera35100;				
////}

//	if((GameFlag.test(fBeenTo57000)  && !GameFlag.test(fWahKa72) && GameFlag.test(fPlayBSDeed)) ||
//	  (GameFlag.test(fYORoomEnter) && !GameFlag.test(fVCRoomEnter) && !GameFlag.test(fWahKa51))) 
	if(GameFlag.test(fLinus18) || GameFlag.test(fWahKa82))
	{
		voicelight35100 = new(VoiceLight36000);
	}

//	new(VoiceMail35100);
//	voicelight35100 = new(VoiceLight36000);

//wahka - take out
/*
	new(SwanOne35100);			  
	new(SwanTwo35100);
	new(President35100);
	new(Envelope35100);
	new(VideoCamera35100);
*/
	if(GameFlag.test(fWahKa94)) {
		sfxSystemCursor( SYS_WAIT );
		sfxLoadRes ( 35120, RES_WAVE );
//		sfxLoadRes ( 35102, RES_ATS );
		sfxLoadRes ( 35130, RES_PIC );
		sfxLoadRes ( 35140, RES_PIC );
//		sfxLockRes(35102, RES_ATS);
		new(SwanTwo35100);
	}
	if(GameFlag.test(fWahKa95)) {
//		new(Envelope35100);
		sfxSystemCursor( SYS_WAIT );
		sfxLoadRes ( 35018, RES_WAVE );
//		sfxLoadRes ( 35180, RES_ATS );
//		sfxLockRes(35180, RES_ATS);
		topenvelope35100 = new(TopEnvelope35100);
		bottomenvelope35100 = new(BottomEnvelope35100);
		envelope35100 = new(Envelope35100);
	}
	if(GameFlag.test(fWahKa96)) {
		sfxSystemCursor( SYS_WAIT );
		sfxLoadRes ( 50011, RES_WAVE );
//		sfxLoadRes ( 35190, RES_ATS );
//		sfxLockRes(35190, RES_ATS);
		new(VideoCamera35100);
	}
	
	new(SouthExit35100);
	new(WestExit35100);
	new(Computer35100);
	
//	if( GameFlag.test( fMORoomEnter ) && !GameFlag.test(fOnce35001)) {
//		if(GameFlag.test(fWahKa40) || GameFlag.test(fWahKa41))

//	if(GameFlag.test(fWahKa43) && !GameFlag.test(fOnce35001)) {
		new(Phone35100);
//	}
	
  	intrface->show();
  	theGame->handsOn();

	snd35000 = new Sound;
	
	if(GameFlag.test(fWahKa6) && !GameFlag.test(fWahKa7)) {
		GameFlag.set(fWahKa7);	
		inventry->items[iMUSICSWAN]->activate();
	}
}

void Rm35100::doit()
{
//	if((GameFlag.test(fYORoomEnter) && !GameFlag.test(fVCRoomEnter) && !GameFlag.test(fWahKa51)) ||		//Yuri about VC
//	if((GameFlag.test(fVCRoomEnter) && GameFlag.test(fDoneSoundAnal) && !GameFlag.test(fWahKa52)) ||	//Lean
	if((GameFlag.test(fOnyxHunt1) && !GameFlag.test(fWahKa53) && !GameFlag.test(fPlayerTurnsProcat) && GameFlag.test(fMetBlake)) ||					//Yuri about Onyx
	   (GameFlag.test(fBrokenYingInterrogation) && GameFlag.test(fOnyxHunt2) && !GameFlag.test(fKilledGrendel) && !GameFlag.test(fWahKa38) && !GameFlag.test(fWahKa54))) {  	//Harmonica
		if(!GameFlag.test(fWahKa78) && (snd35000->number != 35127 || snd35000->isPlaying()) ) {
			snd35000->stop();
			if( snd35000->number != 35127  || !snd35000->isPlaying() )	{
				GameFlag.set(fWahKa78);
				snd35000->loop = -1; 
				snd35000->play(35127, 0);			//phone ringing
////			GameFlag.clear(fWahKa78);
			}
		}
	}
	Room::doit();
}

int Rm35100::handleEvent ( MADEEventStamp *event )
{
	return false;
}

SouthExit35100::SouthExit35100()
{
	name = "SouthExit35100";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
}

void SouthExit35100::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit35100::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
	
    	theGame->newRoom(new Rm35000);
    	return true;
	}
	return false;
}

WestExit35100::WestExit35100()
{
	name = "WestExit35100";
	setHotspot ( DO_V );
//	init(WEST_EXIT_X, WEST_EXIT_Y, WEST_EXIT_WIDTH, WEST_EXIT_HEIGHT);
	init(0, 0, 32, 200); 
}

void WestExit35100::respond()
{
	theGame->setCursor(westCursor);
	aniCursor();
}

int WestExit35100::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
	
    	theGame->newRoom(new Rm35400);
    	return true;
	}
	return false;
}

/*******************************
 *
 *	Features
 *
/*******************************/

Computer35100::Computer35100()
{
	name = "Computer35100";
	setHotspot( DO_V );
	hotspot->addRespondVerb(VIDEOCAMERA_V);  
//	hotspot->addRespondVerb(MUSICSWAN_V);
	hotspot->addRespondVerb(HACKERDISK_V);
	hotspot->addRespondVerb(YINGEBMCATALOG_V);
	hotspot->addRespondVerb(SHANGHAIDISK_V);	
	init( 3, 210, 138, 138 );
////hotspot->addRespondVerb(ALL_V);

	musicswanCursor = new(MusicSwanCursor);
}

void Computer35100::respond()
{
///	if(GameFlag.test(fHaveSwanTwo) && !GameFlag.test(fWahKa83)) {
//		GameFlag.set(fWahKa83);
///		theGame->setCursor( musicswanCursor );
///	}
///	else
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Computer35100::doVerb( int theVerb )
{
	pcLastUsed[PC_INTRLINK] = true;
	if( GameFlag.test( fMaxReportBSDis ) && !GameFlag.test(fDoneSoundAnal)) {	  
		pcLastUsed[PC_SNDANAL] = true;
	}
/*
	if( invVerb == MUSICSWAN_V )	{
		GameFlag.set(fWahKa83);
		inventry->clearCurrentItem();
		normalCursor->rest();
//		if( sound2->number != 175  || !sound2->isPlaying()) {
//			sound2->number = 175;
//			sound2->play();
//		}
//		new(Web);
//		new HTMLPage(97150);
		curRoom->setScript(new UnfoldMusicSwanScript35100);
		return true;
	}
*/
	if( invVerb == VIDEOCAMERA_V ){
		curRoom->setScript( new IDKneeScript35000);
		return true;
	}
	else if( invVerb == HACKERDISK_V ) {
/*
		inventry->clearCurrentItem();
		sound1->stop();
		theMusic->stop();
		sfxDisableSound();
		sfxDisableMusic();
		user->canInput( false );

		char	prevDir[MAX_PATH];		   // change directory to Hacker under the game directory
		char	path[MAX_PATH];
		GetCurrentDirectory( MAX_PATH, prevDir );
		strcpy( path, prevDir );
		strcat( path, "\\HACKER" );
		SetCurrentDirectory( path );

		int		result;
		result = sfxRunProgram( "C64.EXE GAME05.C64" );

		sfxPrintf("Thank you for playing Hacker.");
		SetCurrentDirectory( prevDir );
		
		sfxEnableSound();
		sfxEnableMusic();
		user->canInput( true );
		return true;
*/		
		GameFlag.set(fWahKa83);
		inventry->clearCurrentItem();
		normalCursor->rest();
		curRoom->setScript(new UnfoldMusicSwanScript35100);
		return true;
	}
	else if( invVerb == YINGEBMCATALOG_V) {
		inventry->clearCurrentItem();
		curRoom->setScript( new EBMCatScript24100 );
/*
		if( sound2->number != 175  || !sound2->isPlaying()) {
			sound2->number = 175;
			sound2->play();
		}
		
		Web*	Webby;
		Webby = new Web;
		Webby->caller = this;

		if( GameFlag.test( fNewElectBookAvail ) )
			new HTMLPage( 99730 );	
		else
			new HTMLPage( 99485 );
*/
		return true;
	}
	else if( invVerb == SHANGHAIDISK_V )
	{	
		inventry->clearCurrentItem();
		curRoom->setScript(new launchShanghai);
		return true;
	}
	else if( theVerb == DO_V ) {
		new PCScreen( 35100 );
		return true;
	}
	return false;
}

Phone35100::Phone35100()
{
	name = "Phone35100";
	setHotspot( DO_V );
//	hotspot->addRespondVerb(NUMBERTOYURI_V);	
//	hotspot->addRespondVerb(NUMBERTOBLAKE_V);
	init( 350, 288, 97, 49 );
}

void Phone35100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int Phone35100::doVerb( int theVerb )
{
	if( theVerb == DO_V ) {
//		if((GameFlag.test(fVCRoomEnter) && GameFlag.test(fDoneSoundAnal) && !GameFlag.test(fWahKa52)) ||	//Lean
/*
		if((GameFlag.test(fOnyxHunt1) && !GameFlag.test(fWahKa53) && !GameFlag.test(fPlayerTurnsProcat) && GameFlag.test(fLinus15)) ||					//Yuri about Onyx
	   	   (GameFlag.test(fBrokenYingInterrogation) && GameFlag.test(fOnyxHunt2) && !GameFlag.test(fKilledGrendel) && !GameFlag.test(fWahKa54))) {  	//Harmonica
			if(!GameFlag.test(fWahKa78) && (snd35000->number != 35127 || snd35000->isPlaying()) ) {
				snd35000->stop();
			}
		}
*/
		snd35000->stop();
//		theGame->newRoom(new Rm35300);
		theGame->newRoom(new PhoneKeyPad(FROM_RUSSIA));
		return true;
	}
/*
	if( theVerb == DO_V )	{
//		if( GameFlag.test( fMORoomEnter ) && !GameFlag.test(fOnce35001)){
//			GameFlag.set(fOnce35001);
//			sfxPrintf("Yuri's phone dialogue, #282 - see you in my office");
//			setScript( new EnterScriptYuriVO35000 );
//		}
//		else
//		 		sfxPrintf("All phone lines are busy, please try again later.");
		theGame->newRoom( new Rm35300 );
		return true;
	}
	else if (theVerb == NUMBERTOYURI_V)	{

 	if (theVerb == NUMBERTOYURI_V)	{
		theGame->handsOff();
		intrface->hide();		
		inventry->clearCurrentItem();
		theGame->newRoom( new Rm35300 );
		return true;
	}
	else if( theVerb == NUMBERTOBLAKE_V )
	{
		theGame->handsOff();
		inventry->clearCurrentItem();
		intrface->hide();
//		curRoom->setScript( new CallScript35100 ); 
		curRoom->setScript(	new BlakeTeletalkScript ); 
		return true;
	}
*/
	return false;
}

TopEnvelope35100::TopEnvelope35100()
{
	name = "TopEnvelope35100";
	setHotspot( DO_V );
	init( 451, 309, 60, 25 );
}

void TopEnvelope35100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int TopEnvelope35100::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		GameFlag.clear(fWahKa95);
		GameFlag.set(fHaveHarmEnvelope); 
		inventry->get(iPHOTOPARKER);
//		curRoom->setScript( new UnfoldEnvelopeScript35100);
		if( sound2->number != 35018  || !sound2->isPlaying()) {
			sound2->number = 35018;
			sound2->play();
		}
//		sfxUnlockRes(35180, RES_ATS);
		envelope35100->dispose();
		new EnvelopeInset35100();

		new VLink( "It's you and Asai: Warhurst, Eugene J.", "35103.avi", fPlayedParkerReaction, 60, new EMailAttachment("Newscasts ", 98114));	   //vlink 92
		bottomenvelope35100->dispose();
		dispose();
		return true;
	}
	return false;
}

BottomEnvelope35100::BottomEnvelope35100()
{
	name = "BottomEnvelope35100";
	setHotspot( DO_V );
	init( 414, 335, 96, 39 );
}

void BottomEnvelope35100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int BottomEnvelope35100::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		GameFlag.clear(fWahKa95);
		GameFlag.set(fHaveHarmEnvelope); 
		inventry->get(iPHOTOPARKER);
//		curRoom->setScript( new UnfoldEnvelopeScript35100);
		if( sound2->number != 35018  || !sound2->isPlaying()) {
			sound2->number = 35018;
			sound2->play();
		}
//		sfxUnlockRes(35180, RES_ATS);
		envelope35100->dispose();
		new EnvelopeInset35100();

		new VLink( "It's you and Asai: Warhurst, Eugene J.", "35103.avi", fPlayedParkerReaction, 60, new EMailAttachment("Newscasts ", 98114));	   //vlink 92
		topenvelope35100->dispose();
		dispose();
		return true;
	}
	return false;
}

/*******************************
 *
 *	Views
 *
 *******************************/

SwanOne35100::SwanOne35100()
{
	name = "SwanOne";
	setHotspot ( DO_V );
	setView(35101);
//	init(207, 316, true);
	init(143, 316);
	noSkip();
}

void SwanOne35100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink(CRSBLINK_SPEED);
}

int SwanOne35100::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
//    	sfxPrintf("Show Message: Procat creates mayhem...."); 
		GameFlag.set(fHaveSwanOne);
		
		new SwanOneInset35100();
		dispose();
    	return true;
	}
	return false;
}

SwanTwo35100::SwanTwo35100()
{
	name = "SwanTwo";
	setHotspot ( DO_V );
	setView(35102);
//	init ( 327, 299, true);
	init(263, 300);
	noSkip();
}

void SwanTwo35100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink(CRSBLINK_SPEED);
}

int SwanTwo35100::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
//    	sfxPrintf("Show Message: soc.politics.nuclear.proliferation...."); 
///		GameFlag.set(fHaveSwanTwo);
		
//		new VLink( "35011.avi", fWahKa12, 10 );	   	//vlink691, Holt finds code
//		curRoom->setScript( new MusicSwanCyperScript35000); 
///		new VLink ( "Nuclear crime: Newscast", "35021.avi", 0, 0 );			//vlink 544
	//CLU 	new EMail( "OnyxHuntOne98330", 98330);		
//		new EMail( "OnyxHuntTwo98331", 98331);

//		new(CypherPitOne);
//		new(CypherPitTwo);
		inventry->get(iMUSICSWAN);
///		new SwanTwoInset35100();
///		dispose();		
//		sfxUnlockRes(35102, RES_ATS);
		theGame->newRoom(new Rm35001);
    	return true;
	}
	return false;
}

President35100::President35100()
{
	name = "President35100";
	setHotspot ( DO_V );
	setView(35106);
	init(38, 295);
	noSkip();
}

void President35100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink(CRSBLINK_SPEED);
}

int President35100::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		GameFlag.set(fHavePresidentPicture);
		new PresidentInset35100();
		dispose();
    	return true;
	}
	return false;
}

Envelope35100::Envelope35100()
{
	name = "Envelope35100";
////setHotspot ( DO_V );
	setView(35103);
//	init(410, 311);
	init(413, 311);
	noSkip();
}

void Envelope35100::respond()
{                                     
////theGame->setCursor( normalCursor );
////normalCursor->blink(CRSBLINK_SPEED);
}

int Envelope35100::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
/*		GameFlag.clear(fWahKa95);
		GameFlag.set(fHaveHarmEnvelope); 
//		curRoom->setScript( new UnfoldEnvelopeScript35100);
		if( sound2->number != 35018  || !sound2->isPlaying()) {
			sound2->number = 35018;
			sound2->play();
		}
		new EnvelopeInset35100();

		new VLink( "35103.avi", fPlayedParkerReaction, 60);	   //vlink 92
		dispose();
		return true;
*/
    	return false;
	}
	return false;
}

VideoCamera35100::VideoCamera35100()
{
	name = "VideoCamera35100";
	setHotspot ( DO_V );
	setView(35107);
	init ( 297, 306 );
	noSkip();
}

void VideoCamera35100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink(CRSBLINK_SPEED);
}

int VideoCamera35100::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		GameFlag.clear(fWahKa96);
		GameFlag.set(fHaveVideoCamera);
		if( sound2->number != 50011  || !sound2->isPlaying()) {
			sound2->number = 50011;
			sound2->play();
		}
		inventry->get(iVIDEO_CAMERA);
		inventry->items[iVIDEO_CAMERA]->activate();
		new VideoCamerInset35100();	
//		sfxUnlockRes(35190, RES_ATS);	
		dispose();
    	return true;
	}
	return false;
}

VoiceMail35100::VoiceMail35100()
{
	name = "VoiceMail35100";
//	setHotspot ( DO_V );
	setView(35198);
	init ( 417, 314 );
}

VoiceLight36000::VoiceLight36000()
{
	name = "VoiceLight36000";
	cycleSpeed = 8;	
	view = 35300;
	loop = 0;
	cel = 0;
	init(417, 316);

	setCycle(new EndLoop((Prop*)this, this));
}

void VoiceLight36000::cue( void )
{
	setCycle(new EndLoop((Prop*)this, this));	
}

/*******************************
 *
 *	Insets
 *
 *******************************/
SwanOneInset35100::SwanOneInset35100()
{
	normalCursor->rest();
 	init(35110, 0, 15, 512, 385);
}

void SwanOneInset35100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink(CRSBLINK_SPEED);
}

int SwanOneInset35100::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		dispose();
		curRoom->setScript( new UnfoldMusicSwanScript35100);
		new SwanOneInset35101;		 //folded orange swan
    	return true;
	}
	return false;
}

SwanOneInset35101::SwanOneInset35101()
{
	normalCursor->rest();
 	init(35120, 0, 15, 512, 385);	 //unfolded orange swan
}

SwanTwoInset35100::SwanTwoInset35100()
{
	normalCursor->rest();
	init(35130, 0, 15, 512, 385);	//folded music swan
}

void SwanTwoInset35100::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink(CRSBLINK_SPEED);
}

int SwanTwoInset35100::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		dispose();
//		curRoom->setScript( new UnfoldMusicSwanScript35100);
		if( sound2->number != 35120  || !sound2->isPlaying()) {
			sound2->number = 35120;
			sound2->play();
		}
		new SwanTwoInset35101;
    	return true;
	}
	return false;
}

SwanTwoInset35101::SwanTwoInset35101()
{
	normalCursor->rest();
 	init(35140, 0, 15, 512, 385); //unfolded music swan
}

PresidentInset35100::PresidentInset35100()
{
 	normalCursor->rest();
 	init(35150, 0, 15, 512, 385);	
}

EnvelopeInset35100::EnvelopeInset35100()
{
 	normalCursor->rest();
 	init(35180, 0, 15, 512, 385);
}

VideoCamerInset35100::VideoCamerInset35100()
{
 	normalCursor->rest();
 	init(35190, 0, 15, 512, 385);	
}

/*******************************
 *
 *	Scripts
 *
/*******************************/

IDKneeScript35000::IDKneeScript35000()
{
	name = "IDKneeScript35000"; 
}

void IDKneeScript35000::changeState( int newState )
{
	switchTo
		GameFlag.set(fKnowKneeHasBird);	
		inventry->clearCurrentItem();
		normalCursor->rest();

		if( sound2->number != 175  || !sound2->isPlaying()) {
			sound2->number = 175;
			sound2->play();
		}

		new Web;
		new Knee();

		ticks = 2;
	END

//	BEG
////	new VLink ( "35020.avi", fWahKa17, 0 );

//		sfxPrintf("go to travel link then to Kneecaps");
//		GameFlag.set(fDisplayKnee);
//		GameFlag.set(fWahKa59);
//		GameFlag.set(fSawKneeAddress);	//flag42 for htm
///		GameFlag.set(fKnowKneeHasBird);		
//		theGame->newRoom( new Rm37000);			   
//		theGame->newRoom( new Rm1000);
//	END
}	  

UnfoldMusicSwanScript35100::UnfoldMusicSwanScript35100()
{
	name = "UnfoldMusicSwanScript35100"; 
}

void UnfoldMusicSwanScript35100::changeState( int newState )
{	   
	static View*	home;
	static View*	box;
	static View*	boxText;

	switchTo
		sound2->play( 175, this);
		theGame->handsOff();
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
		boxText->setView(97614);
		boxText->init( 156, 189 );
		boxText->setPri( 410 );
		boxText->noSkip();	
		ticks = 20;
	END
	BEG
	 	home->dispose();
		box->dispose();
		boxText->dispose();
		new(Web);
		new HTMLPage(97150);
	END
}

UnfoldEnvelopeScript35100::UnfoldEnvelopeScript35100()
{
	name = "UnfoldEnvelopeScript35100"; 
}

void UnfoldEnvelopeScript35100::changeState( int newState )
{	   
	switchTo
//		snd35000->number = 35018;
//		snd35000->play();
	END
}

MusicSwanCypherScript35000::MusicSwanCypherScript35000()
{
	name = "MusicSwanCypherScript35000"; 
}

void MusicSwanCypherScript35000::changeState( int newState )
{	   
	switchTo
		new(Web);
		new(CypherPitOne);
		ticks = 2;
	END
	BEG
		new(Web);
		new(CypherPitTwo);
		ticks = 2;
	END
}

Rm35001::Rm35001()
{
	name = "Rm35001";
	show_style = VE_FLIP;
}

Rm35001::~Rm35001()
{
}

void Rm35001::init()
{
	drawPic( 35130 );
	Room::init();

	new( OpenMusciSwan35001 );
	
  	intrface->show();
  	theGame->handsOn();
}

int Rm35001::handleEvent ( MADEEventStamp *event )
{
	return false;
}

SouthExit35001::SouthExit35001()
{
	name = "SouthExit35001";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
}

void SouthExit35001::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit35001::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
//		inventry->items[iMUSICSWAN]->activate();
    	theGame->newRoom(new Rm35100);
    	return true;
	}
	return false;
}

WestExit35001::WestExit35001()
{
	name = "WestExit35001";
	setHotspot ( DO_V );
	init(WEST_EXIT_X, WEST_EXIT_Y, WEST_EXIT_WIDTH, WEST_EXIT_HEIGHT);
}

void WestExit35001::respond()
{
	theGame->setCursor(westCursor);
	aniCursor();
}

int WestExit35001::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
//		inventry->items[iMUSICSWAN]->activate();
    	theGame->newRoom(new Rm35100);
    	return true;
	}
	return false;

}

EastExit35001::EastExit35001()
{
	name = "EastExit35001";
	setHotspot ( DO_V );
	init(EAST_EXIT_X, EAST_EXIT_Y, EAST_EXIT_WIDTH, EAST_EXIT_HEIGHT);
}

void EastExit35001::respond()
{
	theGame->setCursor(eastCursor);
	aniCursor();
}

int EastExit35001::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
//		inventry->items[iMUSICSWAN]->activate();
    	theGame->newRoom(new Rm35100);
    	return true;
	}
	return false;
}

OpenMusciSwan35001::OpenMusciSwan35001()
{
	name = "OpenMusciSwan35001";
	setHotspot( DO_V );
	init( 172, 15, 255, 384 );
}

void OpenMusciSwan35001::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink( CRSBLINK_SPEED );
}

int OpenMusciSwan35001::doVerb( int theVerb )
{
	if( theVerb == DO_V )	{
		if( sound2->number != 35120  || !sound2->isPlaying()) {
			sound2->number = 35120;
			sound2->play();
		}
		theGame->newRoom(new Rm35002);

		return true;
	}
	return false;
}

Rm35002::Rm35002()
{
	name = "Rm35002";
	show_style = VE_FLIP;
}

Rm35002::~Rm35002()
{
}

void Rm35002::init()
{
	drawPic( 35140 );
	Room::init();

	new(SouthExit35001);
	new(WestExit35001);
	new(EastExit35001);
	
  	intrface->show();
  	theGame->handsOn();

	GameFlag.clear(fWahKa94);				//do not show the music swan
	GameFlag.set(fHaveSwanTwo);
	GameFlag.set(fWahKa6);
	GameFlag.set(fGottenSwan);
////new VLink ( "Imminent boom: Holt, David M.","35012.avi", 0, 0 );		//vlink 679
//	new VLink ( "Nuclear crime: Newscast", "35021.avi", 0, 0 );		//vlink 544
}

int Rm35002::handleEvent ( MADEEventStamp *event )
{
	return false;
}

MusicSwanCursor::MusicSwanCursor()
{
	name = "MusicSwanCursor";
	view = 960;
	loop = 9;
	cel = 0;
	orig_pt.x = 0;
	orig_pt.y = 0;
	verb = MUSICSWAN_V;
}
 
} // namespace Spycraft 
