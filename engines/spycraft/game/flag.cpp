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

#include "spycraft/game/flag.h"
#include "spycraft/game/globals.h"
#include "spycraft/game/vlink.h"	  

namespace Spycraft {

Flag GameFlag;

int triggeredOne, beatClosed, pendingBeat, beatCurrentlyOpen;

unsigned beatCount = 0;  //save
unsigned stakeOutTrigger = 0;

unsigned beatCountArray[MAX_BEATS] =
//0 Ying thing
{ fBrokeYingBealeCode,
//1 Procat file
	fGotRealProcatFile,
	//2 Skodi Report
		fSawSkodiMovie,
	//3 BS done the deed
		fBSVoiceMessage,
	//4 Yuri calls player to Vil
		fOnce35000,
	//5 BS Blown the cage
		fDoneSoundAnal,
	//6 Chk BS Pad
		NEVER_SET,   //need design
	//7 Lena in Alley
		fLenaCalls,
	//8 Swan Cipher
		fSwanSolved,
	//9 Max Montana
		fWahKa18, //??
	//10 Meet Max at Stake out
		fMaxDead,
	//11 Get Camera
		fPickedUpCamera,
	//12 Mole 2
		fMoleHunt2,
	//13 PEG ID
		fIDPEG,
	//14 Jaimie sends photo
		fWahKa84,
	//15 Photo Doc
		fBrokenYingInterrogation,
	//16 Achtung
		fSawAchtung,  //need to find out number and who's playing
	//17 President redirects player
		fWahKa15,
	//18 Onyx Hunt 1
		fOnyxHunt1,
	//19 Onyx Hunt 2
		fOnyxHunt2,
	//20 DCI #1 office
		fTravelLinkAvail,  //This flag means that the beat has closed
	//21 Farm
		fTravelLinkAvail,
	//22 DCI #2
		fTravelLinkAvail,
	//23 KAT
		fSolvedDubanskyKAT,
	//24 DCI #3
		fPEGBriefing,
	//25 InfoSci
		fInfosciComplete,
	//26 Colby done
		fColbyMoleBriefing,
	//27 Meet Max: Ying
		fWahKa40,
	//28 Photo Doc
		fBrokenYingInterrogation,
	//29 Broke Ying
		fBrokenYingInterrogation,
	//30 Holt conversation
		fSaw100,
	//31 Blake conversation
		fLinus15,
	//32 Blake Meeting
		fBlakePatioTalkDone,
	//33 Meet Max about BS
		fWahKa42,
	//34 Meet Max about Yuri
		fWahKa41,
	//35 Meet Max about Bird and Yuri
		NEVER_SET,  //handled as two beats above
	//36 Meet BS
		fBeenTo57000,
	//37 Call Yuri
		fOnce35001,
	//38 MEET Yuri
		fYORoomEnter,
	//39 Parker picture
		fHaveHarmEnvelope,
	//40 Warhurst reponds to Parker
		fPlayedParkerReaction,
	//41 Vilnius center
		fVCRoomEnter,
	//42 Harmonica calls
		fWahKa11,
	//43 Harmonica in alley
		fMetHarmonica,
	//44 Yuri calls about onyx
		fYuriAboutOnyx,
	//45 Meet Yuri for dossier
		fOnceOnyx48000,
	//46 Kneecaps ID
		fSawKneeAddress,
	//47 BADMAN
		fRescuedBS,
	//48 Meet with Onyx
		fHKRoomEnter,
	//49 Got to Pit
		fEndgamePitStopped,
	//50 Place holder
		NEVER_SET };


unsigned beatArray[MAX_DELAY_BEATS][2] =

//0 Ying Things
{ fInfosciComplete, 1,  //I'll set fShow157 to trigger this, then travel should chk fShow157
//1 Procat File
	fBrokenYingInterrogation, 1,
	//2 Skodi Report
		fBrokenYingInterrogation, 1,
	//3 Birdsong Done the deed
		fBeenTo57000, 3,
	//4 Yuri calls player to Vilnius
		fYORoomEnter, 1,
	//5 Birdsong blown the cage
		fBSVoiceMessage, 2,
	//6 Check B.S.'s pad
		NEVER_SET, 2,
	//7 Lena in the alley
		MULTI, 2,        //fVCRoomEnter, Harmonica call not pending
	//8 Swan Cipher
		fVCRoomEnter, 0,
	//9 Max Montana
		MULTI, 3,
	//10 Meet Max at Stakeout
		MULTI, 0,			//see #42	
	//11 Get the Camera
		fCarrotPath, 3,
	//12 Mole Hunt 2	
		MULTI, 1,		//fSaw680 and fBlakePatioDone
	//13 PEG ID
		fSolvedDubanskyKAT, 0,
	//14 Jaimie sends Mafia photo
		fSolvedDubanskyKAT, 0,
	//15 Photo docs
		fBrokeYingBealeCode, 0,
	//16 Achtung Story
		fSwanSolved, 0,
	//17 President redirects 		
		MULTI, 0,
	//18 Onyx Hunt 1
		fSwanSolved, 0,
	//19 Onyx Hunt 2
		fOnceOnyx48000, 0,
	//20 Mole hunt 1
		NEVER_SET, 0,
	//21 Mole Hunt 2
		NEVER_SET, 0 };

Flag::Flag() {
	name = "Flag";

	for (int i = 0; i < FLAG_SIZE; i++)
		data[i] = 0;

}

void Flag::set(unsigned posn) {
	int p;
	beatClosed = false;
	triggeredOne = false;
	if (posn > ((FLAG_SIZE * sizeof(unsigned long)) << 3)) {
		sfxPrintf("Corrupted save game, please try another.");
		sfxQuit();
	} else {
		p = posn >> (sizeof(unsigned long) + 1);
		data[p] |= (1 << (posn - ((sizeof(unsigned long)) << 3) * p));
	}
	//	if (posn == fWahKa84)
			//sfxPrintf("here");
	updateBeat(posn);
	checkBeat(posn);
}

void Flag::clear(unsigned posn) {
	int p;

	if (posn > ((FLAG_SIZE * sizeof(unsigned long)) << 3)) {
		sfxPrintf("Exceed FLAG_SIZE=%d", FLAG_SIZE);
		sfxQuit();
	} else {
		p = posn >> (sizeof(unsigned long) + 1);
		data[p] &= ~(1 << (posn - ((sizeof(unsigned long)) << 3) * p));
	}
}

int Flag::test(unsigned posn) {
	int p;

	if (posn > ((FLAG_SIZE * sizeof(unsigned long)) << 3)) {
		sfxPrintf("Exceed FLAG_SIZE=%d", FLAG_SIZE);
		sfxQuit();
		return false;
	} else {
		p = posn >> (sizeof(unsigned long) + 1);
		return (data[p] & (1 << (posn - ((sizeof(unsigned long)) << 3) * p)));
	}
}

int Random(int from, int to) {
	int range = abs(to - from) + 1;

	return (from + rand() % range);
}

void Bset(unsigned p) {
	GameFlag.set(p);
}

void Bclr(unsigned p) {
	GameFlag.clear(p);
}

int Btst(unsigned p) {
	return (GameFlag.test(p));
}

int Flag::updateBeat(unsigned posn) {
	int i;
	for (i = 0; i < MAX_BEATS; i++)
		if (beatCountArray[i] == posn) {
			++beatCount;  //save
			if (i == fWahKa18)
				stakeOutTrigger = beatCount + 3;
			//sfxPrintf("close beat: %d" , i);
			beatCountArray[i] = BEAT_CLOSED;
			beatClosed = true;
			break;
		}
	if ((!beatClosed) && (posn == fWahKa38)) {
		++beatCount;
		beatClosed = true;
	}
	return true;
}

int Flag::checkBeat(unsigned posn) {
	int i;

	for (i = 0; i < MAX_DELAY_BEATS; i++) {
		if (beatArray[i][0] == posn) {
			beatArray[i][0] = START_COUNT;
			beatArray[i][1] += beatCount;
		}
		if (beatArray[i][0] == MULTI) {
			switch (i) {
			case 7: //lena calls
				if ((test(fVCRoomEnter)) && (!test(fWahKa11) || (test(fMetHarmonica)))) {  //add chk for harmonica call made

					beatArray[i][0] = START_COUNT;
					beatArray[i][1] += beatCount;
				}
				break;
			case 9:  //9 Max Montana
				if (test(fDoneSoundAnal) && test(fBrokenYingInterrogation)) {
					beatArray[i][0] = START_COUNT;
					beatArray[i][1] += beatCount;
				}
			case 10: //Meet Max at Stakeout
				if ((beatCount >= stakeOutTrigger) && test(fStickPath) && test(fWahKa41) && test(fWahKa40) && (test(fMetHarmonica) || test(fKilledGrendel) || test(fWahKa38)) && !test(fPlayerTurnsProcat)) {
					beatArray[i][0] = START_COUNT;
					beatArray[i][1] += beatCount;

				}
				break;

			case 12: //mole hunt 2


				break;
			case 17: //President redirects player
				if (test(fOnyxHunt2) && !test(fPlayerTurnsProcat)) {
					beatArray[i][0] = START_COUNT;
					beatArray[i][1] += beatCount;
				}


				break;
			}
		}

	}
	triggeredOne = false;
	pendingBeat = false;
	beatCurrentlyOpen = false;



	int iterCount = 0;
	for (i = 0; i < MAX_DELAY_BEATS; i++) {
		if (beatArray[i][0] == BEAT_TRIGGERED && beatCountArray[i] != BEAT_CLOSED)
			beatCurrentlyOpen = true;
	}

	if (GameFlag.test(fFullBriefing) && !GameFlag.test(fSolvedDubanskyKAT))  //beat 4 kat tool
		beatCurrentlyOpen = true;
	if (GameFlag.test(fSaw113) && !GameFlag.test(fPEGBriefing)) //beat 7  //peg dci briefing
		beatCurrentlyOpen = true;
	if (GameFlag.test(fPEGBriefing) && !GameFlag.test(fInfosciComplete))  //8 info sci complete
		beatCurrentlyOpen = true;
	if (GameFlag.test(fColbySummons) && !GameFlag.test(fColbyMoleBriefing))  //beat 9
		beatCurrentlyOpen = true;
	if (GameFlag.test(fYingCaptured) && !GameFlag.test(fWahKa40))   //beat 11 meet max re: ying
		beatCurrentlyOpen = true;
	if (GameFlag.test(fWahKa40) && GameFlag.test(fSaw646) && !GameFlag.test(fBrokenYingInterrogation))     		//beat 13 break ying
		beatCurrentlyOpen = true;
	if (GameFlag.test(fGotRealProcatFile) && !GameFlag.test(fSaw100))   //16 holt conversation
		beatCurrentlyOpen = true;
	if (GameFlag.test(fSawBlakeDossier) && !GameFlag.test(fLinus15))  //17 blake conversation
		beatCurrentlyOpen = true;
	if (GameFlag.test(fLondonAvailable) && !GameFlag.test(fBlakePatioTalkDone))  //18 patio
		beatCurrentlyOpen = true;
	if (GameFlag.test(fPEGBriefing) && !GameFlag.test(fWahKa42))  //19 max about bird
		beatCurrentlyOpen = true;
	if (GameFlag.test(fSolvedDubanskyKAT) && !GameFlag.test(fWahKa41))   // 20 max about yuri
		beatCurrentlyOpen = true;
	if (GameFlag.test(fWahKa42) && !GameFlag.test(fBeenTo57000))   //22 meet bird
		beatCurrentlyOpen = true;
	if (GameFlag.test(fWahKa41) && !GameFlag.test(fOnce35001)) 	//23 call yuri
		beatCurrentlyOpen = true;
	if (GameFlag.test(fOnce35001) && !GameFlag.test(fYORoomEnter))  //24 meet yuri
		beatCurrentlyOpen = true;
	if (GameFlag.test(fBrokenYingInterrogation) && !GameFlag.test(fHaveHarmEnvelope))  // 26 parker picture
		beatCurrentlyOpen = true;
	if (GameFlag.test(fWahKa51) && !GameFlag.test(fVCRoomEnter))  // 28 meet yuri vil
		beatCurrentlyOpen = true;
	if (GameFlag.test(fBrokenYingInterrogation) && GameFlag.test(fOnyxHunt2) && !GameFlag.test(fKilledGrendel) && (!(test(fWahKa38) || test(fWahKa11))))  //35 harm calls
		beatCurrentlyOpen = true;
	if (GameFlag.test(fWahKa11) && !GameFlag.test(fMetHarmonica))  // 36 met harmonica
		beatCurrentlyOpen = true;
	if (GameFlag.test(fHaveSwanTwo) && !GameFlag.test(fOnyxHunt1))  //39 onyx hunt 1
		beatCurrentlyOpen = true;
	if (GameFlag.test(fHaveSwanTwo) && GameFlag.test(fMetHarmonica) && !GameFlag.test(fYuriAboutOnyx))   // 40yuri calls about onyx
		beatCurrentlyOpen = true;
	if (GameFlag.test(fYuriAboutOnyx) && !GameFlag.test(fOnceOnyx48000)) 				//41 meet yuri, get dossier
		beatCurrentlyOpen = true;
	if (GameFlag.test(fPickedUpCamera) && !GameFlag.test(fSawKneeAddress)) 		//44 knee id												//42
		beatCurrentlyOpen = true;
	if (GameFlag.test(fSawKneeAddress) && GameFlag.test(fKnowKneeHasBird) && !GameFlag.test(fRescuedBS)) //45 badman
		beatCurrentlyOpen = true;
	if (GameFlag.test(fOnceOnyx48000) && !GameFlag.test(fOnyxHunt2))  // 46 onyx hunt 2
		beatCurrentlyOpen = true;
	if (GameFlag.test(fHeidelbergAvailable) && !GameFlag.test(fHKRoomEnter))   // 47 meet onyx
		beatCurrentlyOpen = true;

	//	if (!comlinks->isEmpty())
		//	beatCurrentlyOpen = true;	


	if (!beatCurrentlyOpen) {
		for (i = 0; i < MAX_DELAY_BEATS; i++) {
			if (beatArray[i][0] == START_COUNT)
				pendingBeat = true;
		}
	}

	while (true) {
		for (i = 0; i < MAX_DELAY_BEATS; i++) {
			//time to open a beat
			if ((beatArray[i][0] == START_COUNT) && (beatArray[i][1] == beatCount)) {
				beatArray[i][0] = BEAT_TRIGGERED;
				switch (i) {
				case 0: 	//0 Ying Things
					triggeredOne = true;
					set(fShow157);								//chk in 1000 to show vlink 157
					break;

				case 1: 	//1 Procat File
					triggeredOne = true;
					//					new VLink( "25008.avi", fWahKa34, 0, new EMailAttachment("Investigate Procat", 98050));		//vlink 657, Jaimie implicates Holt
					break;

				case 2:		//2 Skodi Report
					triggeredOne = true;
					new VLink("Dangerous new weapon: Warhurst, Eugene J.", "25014.avi", fSawSkodiMovie, 0, new EMailAttachment("Skodi Report", 99610));
					break;
				case 3:		//3 Birdsong Done the deed
					triggeredOne = true;
					set(fPlayBSDeed);
					break;
				case 4:		//4 Yuri calls player to Vilnius
					triggeredOne = true;
					set(fCanDoYuriVilCall);
					break;
				case 5:		//5 Birdsong blown the cage
					triggeredOne = true;
					new VLink("Birdsong blew the cage: Foster, Maxine M.", "53001.avi", fMaxReportBSDis, 0, new EMailAttachment("Skodi Report", 2001));
					//set(fMaxReportBSDis);	 
					set(fOnce33000);
					//set(fDCK1);				//make BS apt available in travelink
					break;
				case 6:		//6 Check B.S.'s pad
					triggeredOne = true;
					//just a beat now per Larry
					break;
				case 7:		//7 Lena in the alley
					triggeredOne = true;
					set(fPlayLenaMsg);
					break;
				case 8:		//8 Swan Cipher
					triggeredOne = true;
					set(fShowSwanTwo);
					break;
				case 9:		//9 Max Montana
					triggeredOne = true;
					set(fCanDoMaxIntrVLink);
					break;
				case 10:	//10 Meet Max at Stakeout
					triggeredOne = true;
					//set(fCanDisplayStakeOut);
					// 35202 was formerly in vlink.cpp where fWahKa18 was necessary
					new VLink("On to the stakeout: Foster, Maxine M.", "35202.avi", fSaw35202, 0);
					break;
				case 11:	//11 Get the Camera
					triggeredOne = true;
					set(fCanShowCamera);
					break;
				case 12:	//12 Mole Hunt 2
					triggeredOne = true;
					//ASSERT(0,0); //should be MULTI
					break;
				case 13:
					triggeredOne = true;
					new VoiceMail("Identify assassination weapon: Holt, David M.", 98002, HOLT, fSaw98002, 0);
					break;
				case 14:
					triggeredOne = true;
					break;
				case 15: //photo doc
					triggeredOne = true;
					//	new VLink("Use Ying against Grendel: Holt, David M.","644.avi", 0, 0, new EMailAttachment("Photo Doctoring", 98030));
						//new VLink( "723.avi", fYingCaptured,  0 );
					break;
				case 16: //Actung story
					triggeredOne = true;
					//new VLink("Imminent boom: Holt, David M.", "35012.avi", fSawAchtung, 0, new EMailAttachment("Track Onyx", 98330));
					break;
				case 17:
					triggeredOne = true;
					new VLink("Red line communication: Oval Office", "35014.avi", fWahKa15, 0);
					break;
				case 18: //Onyx hunt 1
					triggeredOne = true;
					//new EMail( "Track Onyx", 98330); //now attached to achtung
					break;
				case 19: //Onyx hunt 2
					triggeredOne = true;
					GameFlag.set(fWahKa118);
					new EMail("The break we needed: Holt, David M.", 98331);
					//	new VLink( "48005.avi", fHeidelbergAvailable, 0 );    //vlink681, Holt finds Onyx
					new VLink("Follow that lead: Warhurst, Eugene J.", "356.avi", 0, 0);  					  //vlink356, Warhurst
					break;
				case 20:
					break;
				case 21:
					break;

				}
			}
		}

		if (beatClosed && !triggeredOne && pendingBeat) {
			//sfxPrintf("adding a beat");
			++iterCount;
			++beatCount;
		}
		if (!beatClosed || triggeredOne || !pendingBeat)
			break;
		/*if (iterCount > 3)	{ //DEBUG
			sfxPrintf("Beat iteration issue - call Tom 'the beat man' DeSalvo @2241");
			break;
		}*/
	}
	return true;
}

} // namespace Spycraft
