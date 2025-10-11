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

#ifndef SPYCRAFT_GAME_GLOBALS_H
#define SPYCRAFT_GAME_GLOBALS_H

#include "spycraft/game/views.h"
#include "spycraft/game/verbs.h"
#include "spycraft/game/interface.h"
#include "spycraft/game/object.h"
#include "spycraft/game/dialog.h"

namespace Spycraft {

#define MAX_PATH 256

#define DIALOG_FOCUS 1
#define PDA_FOCUS 2
#define MENU_FOCUS 3
#define INSET_FOCUS 4
#define ALL_EVENT_FOCUS 5
#define INV_INSET_FOCUS 6

#define PRINT_PRI 490
#define INSET_PRI 500
#define MENU_PRI  600

#define NORTH_EXIT_X 	0 //64
#define NORTH_EXIT_Y	0
#define NORTH_EXIT_WIDTH	512
#define NORTH_EXIT_HEIGHT	48

#define SOUTH_EXIT_X 	0//64
#define SOUTH_EXIT_Y	360
#define SOUTH_EXIT_WIDTH	512
#define SOUTH_EXIT_HEIGHT	50

#define EAST_EXIT_X 	480	  
#define EAST_EXIT_Y		0
#define EAST_EXIT_WIDTH		32
#define EAST_EXIT_HEIGHT	360

#define WEST_EXIT_X 	0
#define WEST_EXIT_Y		0
#define WEST_EXIT_WIDTH		32
#define WEST_EXIT_HEIGHT	360

#define NORTHEAST_EXIT_X 	330
#define NORTHEAST_EXIT_Y	0
#define NORTHEAST_EXIT_WIDTH	149
#define NORTHEAST_EXIT_HEIGHT	179

#define NORTHWEST_EXIT_X 	0
#define NORTHWEST_EXIT_Y	0
#define NORTHWEST_EXIT_WIDTH	150
#define NORTHWEST_EXIT_HEIGHT	180

#define SOUTHEAST_EXIT_X 	330
#define SOUTHEAST_EXIT_Y	230
#define SOUTHEAST_EXIT_WIDTH	149
#define SOUTHEAST_EXIT_HEIGHT	179

#define SOUTHWEST_EXIT_X 	0
#define SOUTHWEST_EXIT_Y	230
#define SOUTHWEST_EXIT_WIDTH	150
#define SOUTHWEST_EXIT_HEIGHT	180

#define BACKGROUND_LEFT		0
#define BACKGROUND_TOP		0
#define BACKGROUND_RIGHT	511
#define BACKGROUND_BOTTOM	479

enum {
	STD_DOOR, SWING_DOOR
};

enum {
	fNull,
	// Html flags -- these need to stay constant
	fKnowYingAndCohen,  	// 1)	not currently used
	fSecurityModelAvailable,// 2)	Checked for infosci avail on computer.
	fKnowYingAddress,		// 3)	Set by doing htm reverse phone
	fIDYingVoice,			// 4)	-R Set by snd anal pz of phone w/cohen
	fSolvedDubanskyKAT,		// 5)	-R Set by solving Dubansky video analysis +M&M pz
	fSelectedFinlandNumber,	// 6)	Tried the phone number for Finland on Blake's Power Book
	fFiguredBlakePassword,	// 7)	Figured out Balke's password on his Power Book
	fSentProcatDachaMsg,	// 8)	Sent message to meet at Dacha from Power Book
	fNewElectBookAvail,		// 9)	The elect. book of the mo. club has a new selection.
	fGotNewElectBook,		// 10)	Player downloaded new elect book club book: "Raven"
	fGotInitialElectBook,	// 11)	Player downloaded init. club book or accessed Ying's disk--"Rabbit Hound"
	fKnowGrendelStolePEG,	// 12)	Set by solving M&M: Wayne Allen stole the PEG
	fGotRealProcatFile,		// 13) 	Player found the larger file about Procat
	fPinpointAvailable,		// 14)	Ponpoint weapons database avail under CIA
	fPEGID_shotgun,			// 15)	PEG ID puzzle
	fPEGID_exploding,		// 16)	PEG ID puzzle
	fPEGID_liquid,			// 17)	PEG ID puzzle
	fPEGID_needlepack,		// 18)	PEG ID puzzle
	fPEGID_7mm,				// 19)	PEG ID puzzle
	fPEGID_9mm,				// 20)	PEG ID puzzle

	fTargetGPSAvailable,	// 21)	 
	fTargetFreqAvailable,	// 22)
	fKnowPyramidFreq,		// 23)	Player looked at 
	fPlayerInCharge,		// 24)	DCI2, player in charge of mission
	fGUN_requiem,			// 25)	PEG id
	fGUN_cipher,			// 26)	PEG id
	fGUN_sniper,			// 27)	PEG id
	fGUN_peg,				// 28)	PEG id
	fGUN_china,				// 29)	PEG id
	fGUN_nucleus,			// 30)	PEG id
	fPyramidInfoAvailable,	// 31)	CIA info on Pyramid avail to player
	fGotFalseProcatFile,	// 32)	Got smaller Procat file
	fYuriPh,				// 33)  Put Yuri's phone number in inventory
	fPrisonerDilemmaAvailable,//34)	Files available on archives
	fMetLena,				// 35)	Met Lena in alley
	fDiscoverPit1,			// 36)	Finished cipher puzzle 1
	f1stNIDOnLine,			// 37)	
	f2ndNIDOnLine,			// 38)
	f3rdNIDOnLine,			// 39)
	fHKRoomEnter,	  		// 40)	Entered Hotel Konigen

	fKneeAddressAvailable,	// 41)	Kneecaps' address avail from Pearson's stuff
	fSawKneeAddress,		// 42)	Kneecap's apartment location seen on map
	fGotOldElectBook,		// 43)	"Khan"
	fHeidelbergAvailable,	// 44)	Can go to Heidelberg in Travelink
	fPBShortDownload,		// 45)	amount from Blake's Powerbook	
	fPBMedDownload,			// 46)	amount from Blake's Powerbook
	fPBLongDownload,		// 47)	amount from Blake's Powerbook
	fPBMaxDownload,			// 48)	amount from Blake's Powerbook
	fSawBldgCohen1,			// 49)	6th
	fSawBldgCohen2,			// 50)	10th
	fSawBldgCohen3,			// 51)	12th
	fSawBldgCohen4,			// 52)	24th
	fSawBldgCohen5,			// 53)	26th
	fSawBldgCohen6,			// 54)	29th
	fSawBldgBenton1,		// 55)	10th
	fSawBldgBenton2,		// 56)	12th
	fSawBldgBenton3,		// 57)	16th
	fSawBldgBenton4,		// 58)	21st
	fSawBldgBenton5,		// 59)	29th
	fSawBldgMagruder1,		// 60)	6th

	fSawBldgMagruder2,		// 61)	10th
	fSawBldgMagruder3,		// 62)	17th
	fSawBldgMagruder4,		// 63)	29th
	fSawBldgSung1,			// 64)	10th
	fSawBldgSung2,			// 65)	23rd
	fHeardPhoneCohen1,		// 66)	2nd, with Ying
	fCrossPhoneCohen1,		// 67)	
	fHeardPhoneCohen2,		// 68)	7th
	fCrossPhoneCohen2,		// 69)
	fHeardPhoneCohen3,		// 70)	22nd, with Ying
	fCrossPhoneCohen3,		// 71)
	fHeardPhoneCohen4,		// 72)	27th
	fCrossPhoneCohen4,		// 73)
	fHeardPhoneBenton1,		// 74)	7th
	fCrossPhoneBenton1,		// 75)
	fHeardPhoneBenton2,		// 76)	15th
	fCrossPhoneBenton2,		// 77)
	fHeardPhoneBenton3,		// 78)	15th
	fCrossPhoneBenton3,		// 79)
	fHeardPhoneBenton4,		// 80)	21st

	fCrossPhoneBenton4,		// 81)
	fHeardPhoneMagruder1,	// 82)	8th
	fCrossPhoneMagruder1,	// 83)
	fHeardPhoneMagruder2,	// 84)	29th
	fCrossPhoneMagruder2,	// 85)
	fHeardPhoneSung1,		// 86)	20th
	fCrossPhoneSung1,		// 87)
	fHeardPhoneSung2,		// 88)	16th
	fCrossPhoneSung2,		// 89)
	fSawLabCohen1,			// 90)	6th
	fSawLabCohen2,			// 91)	10th
	fSawLabCohen3,			// 92)	24th, Grendel
	fSawLabBenton1,			// 93)	10th
	fSawLabBenton2,			// 94)	16th
	fSawLabMagruder1,		// 95)	6th
	fSawLabMagruder2,		// 96)	10th
	fSawLabSung1,			// 97)	10th
	fSawLabSung2,			// 98)	23rd
	fSawGrendelDossier,		// 99)
	fSawYingDossier,		// 100)

	fOnlineAvailable,		// 101) Online service attached for real URL's???
	fPlayerViewsSkodi,		// 102) looked at skodi report in html
	fSawSkodiMovie,			// 103) saw Skodi vlink
	fEclipse2Avail,			// 104)	Mole hunt eclipse 2nd level avail
	fEclipse3Avail,			// 105)	Mole hunt eclipse 3rd level avail
	fSawNegronDossier,		// 106) Player looked at the dossier
	fSawCollinsDossier,		// 107)
	fSawHanlonDossier,		// 108)
	fSawMenoharDossier,		// 109)
	fSawGrenaldDossier,		// 110)
	fSawFishmanDossier,		// 111)
	fSawHongDossier,		// 112)
	fSawPhippsDossier,		// 113)
	fSawFakeProcatFile,		// 114)
	fSawHarmonicaDossier,	// 115)
	fSawPawPrint,			// 116)
	fSawBlakeDossier,		// 117)
	fPatsHTM_118,			// 118) 
	fPatsHTM_119,			// 119) 
	fPatsHTM_120,			// 120) 
	fPatsHTM_121,			// 121) 
	fPatsHTM_122,			// 122) 
	fPatsHTM_123,			// 123) 
	fPatsHTM_124,			// 124) 
	fPatsHTM_125,			// 125) 
	fPatsHTM_126,			// 126) 
	fPatsHTM_127,			// 127) 
	fPatsHTM_128,			// 128) 
	fPatsHTM_129,			// 129) 
	fPatsHTM_130,			// 130) 
	fSawSndAnalOpen,		// 131)
	fSawMnMOpen,			// 132)
	fTravelLinkAvail,		///133)
	fInitialBriefing,		///134) 
	fFarmDone,				///135) 
	fFullBriefing,			// 136) R- briefing is complete.
	fWahKa42,				// 137)
	fSaw634,				// 138)
	fSaw98002,				// 139)
	fIDPEG,					// 140) Set by doing image analysis of PEG impact hole pictures
	fPEGBriefing,			// 141) RW- briefing is complete.
	fSaw113,				// 142)
	fSaw139,				// 143)
	fColbySummons,  		// 144) checked in Rm 10000
	fColbyMoleBriefing,     // 145)
	fSaw12001,				// 146)
	fSaw12002,				// 147)
	fSaw157,				// 148)
	fBrokeYingBealeCode,	// 149) Set by solving cypher tool puzzle
	fYingCaptured,			// 150)W- Set...maybe when Holt calls <644>.
	fWahKa40,				// 151)
	fSaw644,				// 152)
	fDidPhotoDoc,			// 153)
	fLinus15,		        // 154)
	fBlakePatioTalkDone,	// 155)
	fSawPyramid,			// 156)
	fMetBirdSong,			// 157)
	fWahKa41,				// 158)
	fYORoomEnter,   		// 159)		//Entered Yuri's Office
	fMaxReportBSDis,	 	// 160)     Max reports BS disappearance
	fDoneSoundAnal,			// 161)		//Finished up Sound Analysis Puzzle
	fWahKa51,				// 162)
	fVCRoomEnter,     		// 163)		//Entered Vilnus Center
	fBSDoorUnlocked,		// 164)
	fWahKa54,				// 165)
	fMetHarmonica,			// 166)	    //Met Harmonica in alley
	fSaw98330,				// 167)
	fOnyxHunt2,				// 168)
	fYuriAboutOnyx,			// 169)		//Yuri calling about Onyx
	fOnceOnyx48000,			// 170)		//do once for talk about Onyx
	fSaw35202,				// 171)
	fMaxDead,           	// 172)		//Max is dead
	fHaveVideoCamera,		// 173)
	fKneecapsFound,			// 174)
	fRescuedBS,				// 175)
	fEndgamePitStopped,		// 176)      R- set at pit deal
	fSaw12000,				// 177)
	fVisitedOnyx,			// 178)
	fTalkedOnyx,			// 179)
	fEndgame,				// 180) R- set in dacha
	fBeenToTunisia,			// 181)
	fLinus12,				// 182)
	fEndGameHarmonica,      // 183)
	fInfosciComplete,       // 184)
	fSawOnyxStuff,			// 185)
	fBrokenYingInterrogation,// 186)
	fTunisiaAvailable,		// 187)
	fYingCipherAvailable,	// 188)
	fPit1CipherAvailable,	// 189)
	fPit2CipherAvailable,	// 190)
	fMole3CipherAvailable,	// 191)
	fMole4CipherAvailable,	// 192)
	fOnyxHunt2Available,	// 193)
	fPowerBook1Available,	// 194)
	fPowerBook2Available,	// 195)
	fPowerBook3Available,	// 196)
	fPowerBook4Available,	// 197)
	fPowerBook5Available,	// 198)
	fPowerBook6Available,	// 199)
	fPowerBook7Available,	// 200)
	fOnyxHunt1,				//201
	fRealProcatFileFound,	// 202)
	fYingCipherLoaded,		// 203)
	fSawInfosciIntro,		// 204)
	fPrintedPhoto,			// 205
	fInvestigateBS,			// 206)
	fBegunMoleHunt,         //207)
	fHaveHarmEnvelope,      //208)
	fSawOnyxPYPDossier,     //209)
	fSawSoundHackMail,		//210)
	fSaw98092,				//211)
	fGottenSwan,			//212)
	fSaw93110,				//213)
	fBSClosed,				//214
	fHTM215,
	fHTM216,
	fHTM217,
	fHTM218,
	fHTM219,
	fHTM220,

	fWashingtonAvailable,	// World Map 
	fMoscowAvailable,
	fCrimeaAvailable,
	fDachaAvailable,

	fDCIOfficeAvailable,
	fHoltSeatonAvailable,
	fBirdsongAvailable,
	fYasevnoAvailable,

	fYingStuffArrived,		// R- Ying appt. raided. 
	fCohenDead,				// W- Set when Warhurst calls, triggered by using database
	fYingEscaped,			// W- Set when Warhurst calls
	fPEGIDEmailSent,		// RW- mail makes PEG id htm avail
	fISciEmailSent,			// RW- mail makes infosci htms avail
	fKnowYingInMoscow,		// Set by cypher tool of Beale code.
	// Workstation
	fUsingWorkstation,		// At a computer (not using PDA)
	fKATAvailable,			// 	(Jaimie put analysis tools on net)
	fIAAvailable,			//	on farm, Image Analysis available
	fBelieveCohenThief,		//  Set by solving security model: Cohen must have stolen PEG

	// rm 2000
	fEnteredFranksOffice,
	fHeardFranksMessages,	// listened to all of Frank's phone messages
	// rm 4000
	fFarmDidPhotoIntro,	// first thing: met Frank
	fFarmDidFrankSad,	// played Frank is sad about Jeffries movie
	//fFarmDone,			// everything at the farm has been done.
	// rm 5000
	fShowedIANews,		// played news clip
	fFarmIALicenseDone,	// image analysis puzzle 
	fFarmIATanksDone,	// second image analysis puzzle
	fFarmIAAmesDone,	// third image analysis puzzle
	fFarmIADone,		// finished image analysis
	fFrankEnteredIA,
	// rm 7000
	fFarmDoingWP,		// weapons proficiency
	fFarmWPDone,		// weapons proficiency
	fFarmDoingCO,		// crisis orienteering
	fFarmCODone,		// crisis orienteering--Jeffries is dead	
	// rm 12000

	// rm 13000
	fPlayerTalkedPEGJaimie,	// RW- set VLink from Jaimie
	fSetJaimies139,			// PEG
	fSetJaimies157,			// Ying's stuff
	// rm 18000--DCI's office
	//fInitialBriefing,	// R- briefing is complete. 

	fEndgameYuriShot,		// R- set in dacha
	fEndgameHarmonicaKilled,	// R- set at president speech

	fForceDCI2,		// set this thing, then teleport to DCI's office (18000)
	fForceDCI3,		// set this thing, then teleport to DCI's office (18000)
	//room 22000
	fYingNoMoreNegotiations,
	//rm 24000		player's Langley office
	fPlayerOfficeStarted,		// Jaimie gives the KAT
	fJaimieSentSoundHackToPlayer,	// RW- Also may be rw in player's Moscow office
	fSetVL_Warhurst113,			// let's meet with Sterling
	fSetVL_Milkovsky124,		// Harmonica is good
	fSetVL_Warhurst147,			// Cohen killed himself
	fSetVL_Colby153,			// Cohen not the mole
	fSetVL_Warhurst155,			// Cho surveillance approved
	fSetVL_Warhurst156,			// Ying escaped
	fSetVL_Holt158,				// Good job breaking Beale, Ying @ Moscow
	fSetVL_Max164,				// Come get Ying in Moscow
	fSetVL_Warhurst631,			// Go see Max in Moscow
	fSetVL_Warhurst632,			// Good, analyze video
	fSetVL_Holt641,				// Wayne Allen could steal the spots off a leopard
	fSetVL_Holt644,				// Confirmed: Ying @ Moscow
	fSetVL_Holt645,				// Help me find Ying
	fSetVL_Milkovsky646,		// Bullpen gets the job done
	fSetVL_Holt660,				// Cohen a patsy--affair
	fSetVL_Warhurst723,			// Help find Ying
	fSetVL_Warh35104,			// Harmonica is in Moscow
	//rm87000
	fLeavingOffice,
	fPlayerPegsPres,
	fGotPeg,
	fMessageCleared,
	fBeenTo61000,
	fSawPainting,
	//room 67000
	fMoscowConvinceBrooksToCancel,
	//room 65000
	fBlakeCaught,
	fBlakeCaptured,
	fStopAssassination,
	//room 93000
	fBlakeQuestioned,
	fBlakeBluesInterestKnown,
	fLondonPlayerExaminingCD,
	fBlakeInitialsSeen,
	fModemConnected,
	//room 80000
	fMetBlake,
	fBlakeGone,
	fBeenTo77000,
	//room 96000
	fShotPEG,		// you only get one shot
	fRoundInPEG,	// player put needlepack in PEG
	fMovedForward96000,			// (global for saved game)
	fDoDissolve96000,			// (global for saved game)
	f1LeaveAttempt96000,		// (global for saved game)
	f2LeaveAttempts96000,		// (global for saved game)
	//flags used in room 43000
	fRedStart,
	fSofkinskySpoken,
	fRSSterlingThanks,
	//used in 1000

	fKnowKneeHasBird,		// set in IA of kneecaps photo 	
	fDisplayStakout,
	fDisplayRed,
	fDisplayAir,
	//room 1060
	fMoscowAirport,
	//used in 1075
	fGrendelGripe,      	//Grendel gripes
	fDisposeDialog1075,
	//room 23000
	fDoneInterrogation,		//Done interrogation room  

	fDisposeDialog23000,
	fDisposeDialog23002,
	//room 25000
	fOnceBullpen25000,      //do once in bullpen    
	//room 30000
	fMoscowAlleyAvailable,
	//room 32000
		//fMetLena,            	//Met Lena in alley  --moved to htm area
	fPlayerTurnsProcat,		//player turned Procat 
	fPlayerKilled,			//Harmonica kills player, game over  
	fMaxSavesPlayer,        //Max saves player
	fMaxDrawerUnlocked,
	fOnce32000,
	fDisposeDialog32000,
	fEnteredProcatVan,
	//used in 33000
	fMetMaxFoster,
	fHarmonicaCalling,
	fHarmonicaTrapSet,
	fWarnMax33000,          //warn Max about Harmonica's meeting in alley
	fMaxSetTrapHarm,        //Max sets trap for Harmonica
	fThornNotMax,           //player didn't warn Max about Harmonica
	fDisposeDialog33000,
	fLeavingSafeHouse,		//player leaving station house
	fOnce33000,
	//34000
	fYingDead,
	fYingAskedToCooperate,
	fYingCooperates,
	//used in 35000
	fOnce35000,                     //Yuri's dialogue 
	fOnce35001,						//Yuri's phone dialogue 
	fOthelloEnvelopeAvailable,
	fGotOthelloPhoto,
	fSwanOneAvailable,
	fScannerOn,

	fBSVoiceMessage,				//Message from Birdsong
	fHaveSwanOne,					//Picked up the orange swan
	fHaveSwanTwo,					//Picked up the music swan
	fHavePresidentPicture,			//Picked up the picture of the President


	fHarmCalls,						//Harmonica calling to meet him in the alley

	fOnce35002,						//swans
	fDisposeDialog35000,
	//room35200
	fDisposeDialog35200,
	fStickPath,						//player has pick the stick path
	fCarrotPath,					//plyaer has pick the carrot path
	//room35300
	fLenaCalls,					//Lena called about meeting her in the alley
	fDisposeDialog35700,
	//used in 36000 (36100, 36200, 36300)
	fBeenTo36000,
	fMORoomEnter,					//Entered Max's Office in Moscow
	fMaxYing,						//Ying's path, first time in Moscow
	fFosterDeskDrawerUnlocked,		//RW- set when player picks Foster's lock
	fProcatPlayerCaught,
	fDisposeDialog36000,
	fGetProcatFile,					//caught getting Procat files
	fStartBullpen,					//start with Bullpen
	fStartInterrogation,			//start with Interrogation
	fYingMetMax,					//met Max, asking about Ying
	fMetMaxDidKAT,					//met Max and completed KAT
	//room 36400
	fDisposeDialog36400,
	//room 36500
	fDisposeDialog36500,
	fDisposeDialog36700,
	//room 37000
	fEnterBadmanVan,
	//room 37200
	fBSTellStory,					//Birdsong tells his story in Kneecaps Apt.
	//room 41000

	//room 48000

	fBeenTo48000,
	fSeenYuriPhoto,
	fSeenSoftball,
	fYuriCallingAboutGift,
	fPlayerHasMetYuri,
	fYuriHasOnyxFile,				//yuri has the onyx file
	fYuriGift,						//get gift from Harmonica in Yuri's office
	fOnceGift48000,					//do once for gift box
	fOnceShocked48000,				//do once for shocked

	fDisposeDialog48000,
	fDisposeDialog48500,
	//room 50000

	fPickedUpCamera,				//Picked up video camera
	//room 53000

	fBeenTo57000,
	fBSMeetingSet,

	fBetrayMaxMafia,	//BS will betray Max through mafia	  
	fBetrayMaxKneecaps,	//BS will betray Max through Kneecaps 
	fBSTrue,			//BS will remain true				  
	fDisposeDialog57000,
	fMoscowMaxFindsEnvelope,
	fBSLangeEnter,		//Lange's dialog
	fKilledLange,   	//player shot Lange	
	fDisposeDialog57100,
	fOnce57100,
	fNoticedWestern,	//looked at Western items in apartment
	//room 69000 hotel
		//fHKRoomEnter,	  	//Entered Hotel Konigen	-- moved to htm area
	fDisposeDialog69000,
	fBeenTo68000,
	fDisguiseKitOpen,
	fDisguiseKitFound,	//after suitcase opened this is set
	fOnyxSuitcaseOpen,
	fOnyxBound,			//you have bound onyx with cuffs
	fGotHandcuffs,		//handcuffs are in memory
	fMovedSuit,
	//room 69100
	fClosetOpen69100,
	fClosetWHand69100,

	//puzzles
	fPDrAvailable,
	fMMAvailable,
	fSAAvailable,

	fSentPuzzleMail,	// reusable

	// pat's flags
	fStartCombat,
	fGotBeacon,
	fGotKilled,
	fWPSqueekBy,
	fYellow,
	fInfosciPartial,

	fRefineryCombat,

	//Colby V-Links


	fSaw90,
	fSaw124,

	fSaw147,
	fSaw155,
	fSaw156,
	fSaw158,
	fSaw630,

	fSaw646,
	fSaw723,
	fSaw731,

	fWahKa1,
	fWahKa2,
	fWahKa3,
	fWahKa4,
	fWahKa5,
	fWahKa6,
	fWahKa7,
	fWahKa8,
	fWahKa9,
	fWahKa10,
	fWahKa11,
	fWahKa12,
	fWahKa13,	   //used in vlink.cpp
	fWahKa14,
	fWahKa15,
	fWahKa16,
	fWahKa17,
	fWahKa18,
	fWahKa19,
	fWahKa20,
	fWahKa21,
	fWahKa22,
	fWahKa23,
	fWahKa24,
	fWahKa25,
	fWahKa26,
	fWahKa27,
	fWahKa28,
	fWahKa29,
	fWahKa30,

	fWahKa31,
	fWahKa32,
	fWahKa33,
	fWahKa34,
	fWahKa35,
	fWahKa36,
	fWahKa37,
	fWahKa38,
	fWahKa39,



	fWahKa43,
	fWahKa44,
	fWahKa45,
	fWahKa46,
	fWahKa47,
	fWahKa48,
	fWahKa49,
	fWahKa50,

	fWahKa52,
	fWahKa53,

	fWahKa55,
	fWahKa56,
	fWahKa57,
	fWahKa58,
	fWahKa59,
	fWahKa60,
	fWahKa61,
	fWahKa62,
	fTortureYingDead,
	fWahKa64,
	fWahKa65,
	fWahKa66,
	fWahKa67,
	fWahKa68,
	fWahKa69,
	fWahKa70,
	fWahKa71,
	fWahKa72,
	fWahKa73,
	fWahKa74,
	fWahKa75,
	fWahKa76,
	fWahKa77,
	fWahKa78,
	fWahKa79,
	fWahKa80,
	fWahKa81,
	fWahKa82,
	fWahKa83,
	fWahKa84,
	fWahKa85,
	fWahKa86,
	fWahKa87,
	fWahKa88,

	fWahKa90,
	fWahKa91,
	fWahKa92,
	fWahKa93,
	fWahKa94,
	fWahKa95,
	fWahKa96,
	fWahKa97,
	fWahKa98,

	fWahKa100,
	fWahKa101,
	fWahKa102,
	fWahKa103,
	fWahKa104,
	fWahKa105,
	fWahKa106,
	fWahKa107,
	fWahKa108,
	fWahKa109,
	fWahKa110,
	fWahKa111,
	fWahKa112,
	fWahKa113,
	fWahKa114,
	fWahKa115,
	fWahKa116,
	fWahKa117,
	fWahKa118,
	fWahKa119,
	fWahKa120,
	fWahKa121,
	fWahKa122,
	fWahKa123,
	fWahKa124,
	fWahKa125,
	fWahKa126,
	fWahKa127,
	fWahKa128,
	fWahKa129,
	fWahKa130,
	fWahKa131,

	//fDiscoverPit1,	-- moved to htm area
	fDiscoverPit2,
	fDiscoverPit3,
	fDoneMole1,
	fDoneMole2,
	fDoneMole3,
	fDoneMole4,
	fDoneMole5,
	fDoneMole6,

	fDCK1,
	fDCK2,
	fDCK3,
	fDCK4,
	fDCK5,
	fDCK6,
	fDCK7,
	fDCK8,
	fDCK9,
	fDCK10,
	fDCK11,
	fDCK12,
	fDCK13,
	fDCK14,
	fDCK15,
	fInit644,
	fInit723,
	fInit646,
	fInit631,
	fInit99020,
	fInit90,
	fInit731,
	fInit153,
	fInit156,
	fInitCarrot,
	//	fTravelLinkAvail,
	fInit100,
	fTom1,
	fTom2,
	fTom3,
	fTom4,
	fTom5,


	fMoleHunt2,
	fSaw25014,
	fSaw100,
	fShow157,
	fPlayBSDeed,
	fPlayLenaMsg,
	fCanDisplayStakeOut,
	fCanDoYuriVilCall,
	fCanDoMaxIntrVLink,
	fCanShowCamera,
	fSaw680,
	fSaw681,
	fPlayedParkerReaction,
	fSwanSolved,
	fShowSwanTwo,
	fSawAchtung,
	fLockOut,
	fNetAvail,
	fRoomPrintIndicate,
	fRoomPrintActive,
	fTom16,
	fTom17,
	fTom18,
	fTom19,
	fTom20,
	fTom21,
	fTom22,
	fTom23,
	fTom24,
	fTom25,
	fTom26,
	fTom27,
	fTom28,
	fTom29,
	fTom30,

	fStartDownLoad,		// Used in helicopter Rm90000	
	fGotPowerBook,
	fLinus1,
	fLinus2,
	fLinus3,
	fLinus4,
	fLinus5,
	fLinus6,
	fLinus7,
	fLinus8,
	fLinus9,
	fLinus10,
	fLinus11,

	fLinus13,
	fLinus14,
	fLondonAvailable,
	fLinus16,
	fLinus17,
	fLinus18,
	fLinus19,
	fLinus20,
	fLinus21,
	fLinus22,
	fLinus23,
	fLinus24,
	fLinus25,
	fLinus26,
	fLinus27,
	fLinus28,
	fLinus29,
	fLinus30,
	fLinus31,
	fLinus32,
	fLinus33,
	fLinus34,
	fLinus35,
	fLinus36,
	fLinus37,
	fLinus38,
	fLinus39,
	fLinus40,
	fLinus41,
	fLinus42,
	fLinus43,
	fLinus44,
	fLinus45,
	fLinus46,
	fLinus47,
	fLinus48,
	fLinus49,
	fLinus50,

	fReportKat,
	fReportPEG,
	fReportInfosci,
	fReportMapTool,
	fReportOnyxOne,
	fReportOnyxTwo,
	fReportKneecaps,
	fReportYingCypher,
	fReportMoleHunt,
	fGreg10,
	fGreg11,
	fGreg12,
	fGreg13,
	fGreg14,
	fGreg15,
	fGreg16,
	fGreg17,
	fGreg18,
	fGreg19,
	fGreg20,
	fGreg21,
	fGreg22,
	fGreg23,
	fGreg24,
	fGreg25,
	fGreg26,
	fGreg27,
	fGreg28,
	fGreg29,
	fGreg30,
	fGreg31,
	fGreg32,
	fGreg33,
	fGreg34,
	fGreg35,
	fGreg36,
	fGreg37,
	fGreg38,
	fGreg39,
	fMoleWarhurst1,
	fMoleWarhurst2,
	fMoleWarhurst3,
	fMoleSterling1,
	fMoleSterling2,
	fMoleHolt1,
	fMoleHolt2,
	fMoleHolt3,
	fMoleSeaton1,
	fMoleSeaton2,
	fMoleSeaton3,
	fMoleFrank1,
	fMoleFrank2,
	fMoleFrank3,
	fMoleMax1,
	fMoleMax2,
	fMoleMax3,
	fMoleCollins1,
	fMoleCollins2,
	fMoleGrenald1,
	fMoleGrenald2,
	fMoleFishman1,
	fMoleFishman2,
	fMoleHong1,
	fMoleHong2,
	fMoleHong3,
	fMolePhipps1,
	fMolePhipps2,
	fMolePhipps3,
	fMoleMenoher1,
	fMoleMenoher2,
	fMoleMenoher3,
	fMoleHanlon1,
	fMoleHanlon2,
	fMoleHanlon3,
	fMoleKate1,
	fMoleKate2,
	fMoleKate3,
	fMoleSung1,
	fMoleSung2,
	fMoleSung3,
	fKilledGrendel
};

} // namespace Spycraft

#endif
