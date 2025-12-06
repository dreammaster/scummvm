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

#include "spycraft/game/save.h"
#include "spycraft/game/flag.h"
#include "spycraft/game/file.h"
#include "spycraft/game/debugmod.h"
#include "spycraft/game/vlink.h"
#include "spycraft/game/sound.h"
#include "spycraft/game/invent.h"
#include "spycraft/game/vlink.h"
#include "spycraft/game/pcscreen.h"
#include "spycraft/game/phodoc.h"
#include "spycraft/game/timer.h"

namespace Spycraft {

extern unsigned long gameTime;
extern short curMap;
extern short curDisc;
extern ComLinks *comlinks;
extern DebugModule *theDebug;
extern short pcLastUsed[PC_MAXUSEDTYPE];
extern short state96000;
extern short attempt96000;
extern unsigned stakeOutTrigger;
short pcArrayInfo;
extern int tryingToQuit;
extern Colby colby;

int havePhoto;
int haveAttach, haveArchAttach;
short restoring;
static short numInvOwned;
extern int lasttool;

File *bufFile = new File;  //DEBUG
SaveGame *saveGame = new SaveGame;
static RestRoom *restRoom;

extern unsigned beatArray[MAX_DELAY_BEATS][2];
extern unsigned beatCountArray[MAX_BEATS];
extern unsigned beatCount;
extern Timer *hurryTimer;

RestRoom::RestRoom(const char *oldGame) {
	name = "RestRoom";
	nextGame = oldGame;
}

void RestRoom::init() {
	//drawPic(1);
	Room::init();
	restRoom = this;

	//must set restoring to true and input to false
	//and in winMade do if (!restoring) theGame->doit();

	saveGame->restore(nextGame);

	//must set restoring to false and input to true
	restoring = true;
	theDebug->teleport(theNextRoom);
}

SaveGame::SaveGame(void) {
	name = "SaveGame";
}

int SaveGame::save(const char *whichSaveGame) {
	if (!bufFile->open(whichSaveGame, FILE_WRITE))
		return false;



	bufFile->write(GameFlag.data, FLAG_SIZE * sizeof(unsigned long));	//first 80 bytes are flags 0-79 #1


	bufFile->writeASCFile(curRoom->name);                	//next write name of room #2

	bufFile->writeASCFile(theGame->prevRoomName);   //write name of the prev room #3


	bufFile->write(&gameTime, sizeof(gameTime));   // 4 time bytes / gameTime #4
	bufFile->write(&curMap, sizeof(curMap));   // 2 bytes / curMap
	bufFile->write(&curDisc, sizeof(curDisc));   // 2 bytes / curDisc

	bufFile->write(beatCountArray, MAX_BEATS * sizeof(unsigned));
	bufFile->write(beatArray, MAX_DELAY_BEATS * 2 * sizeof(unsigned));
	bufFile->write(&beatCount, sizeof(beatCount));

	//inventory*********************************************************************
	numInvOwned = 0;
	short i;
	for (i = 0; i < MAX_INV; i++)
		if (inventry->has(i))
			++numInvOwned;

	bufFile->write(&numInvOwned, sizeof(numInvOwned));  // short2 how many items owned 

	for (i = 0; i < MAX_INV; i++)
		if (inventry->has(i))
			bufFile->write(&i, sizeof(i));			// short2 just save the index


	// Save V-Link list***************************************************************
	// this list now holds emails and voicemails, must chk comType
	bufFile->write(&comlinks->size, sizeof(comlinks->size));  // short2 how many v-links #5
	Node *n = comlinks->head;
	ComData *curLink;
	VLink *theVLink;
	EMail *theEMail;
	VoiceMail *theVoiceMail;

	while (n) {
		curLink = (ComData *)n->data;

		bufFile->writeASCFile(curLink->msgName);
		bufFile->write(&curLink->comType, sizeof(curLink->comType));  	//unsigned short2 
		bufFile->write(&curLink->time0, sizeof(curLink->time0));  		//unsigned long4 
		bufFile->write(&curLink->myFlag, sizeof(curLink->myFlag));   	//unsigned short2 
		bufFile->write(&curLink->myDelay, sizeof(curLink->myDelay)); 	//unsigned short2 

		switch (curLink->comType) {
		case VLINKMAIL:
			theVLink = (VLink *)n->data;
			bufFile->writeASCFile(theVLink->movieName);
			if (theVLink->emailAttachment) {
				haveAttach = true;
				bufFile->write(&haveAttach, sizeof(haveAttach));
				bufFile->writeASCFile(theVLink->emailAttachment->msgName);
				bufFile->write(&theVLink->emailAttachment->pageNumber, sizeof(theVLink->emailAttachment->pageNumber));
				bufFile->write(&theVLink->emailAttachment->myFlag, sizeof(theVLink->emailAttachment->myFlag));
			} else {
				haveAttach = false;
				bufFile->write(&haveAttach, sizeof(haveAttach));
			}
			break;
		case EMAIL:
			theEMail = (EMail *)n->data;
			bufFile->write(&theEMail->pageNumber, sizeof(theEMail->pageNumber));
			break;
		case VOICEMAIL:
			theVoiceMail = (VoiceMail *)n->data;
			bufFile->write(&theVoiceMail->soundNumber, sizeof(theVoiceMail->soundNumber));
			bufFile->write(&theVoiceMail->idNumber, sizeof(theVoiceMail->idNumber));
			break;
		}
		n = n->next;
	}


	// Save Archive list***************************************************************
// this list now holds emails and voicemails, must chk comType
	bufFile->write(&archiveList->size, sizeof(archiveList->size));  // short2 how many v-links #5
	Node *archN = archiveList->head;
	ComData *archCurLink;
	VLink *theArchVLink;
	EMail *theArchEMail;
	VoiceMail *theArchVoiceMail;
	while (archN) {
		archCurLink = (ComData *)archN->data;

		bufFile->writeASCFile(archCurLink->msgName);
		bufFile->write(&archCurLink->comType, sizeof(archCurLink->comType));  	//unsigned short2 
		bufFile->write(&archCurLink->time0, sizeof(archCurLink->time0));  		//unsigned long4 
		bufFile->write(&archCurLink->myFlag, sizeof(archCurLink->myFlag));   	//unsigned short2 
		bufFile->write(&archCurLink->myDelay, sizeof(archCurLink->myDelay)); 	//unsigned short2 

		switch (archCurLink->comType) {
		case VLINKMAIL:
			theArchVLink = (VLink *)archN->data;
			bufFile->writeASCFile(theArchVLink->movieName);
			if (theArchVLink->emailAttachment) {
				haveArchAttach = true;
				bufFile->write(&haveArchAttach, sizeof(haveArchAttach));
				bufFile->writeASCFile(theArchVLink->emailAttachment->msgName);
				bufFile->write(&theArchVLink->emailAttachment->pageNumber, sizeof(theArchVLink->emailAttachment->pageNumber));
				bufFile->write(&theArchVLink->emailAttachment->myFlag, sizeof(theArchVLink->emailAttachment->myFlag));
			} else {
				haveArchAttach = false;
				bufFile->write(&haveArchAttach, sizeof(haveArchAttach));
			}
			break;
		case EMAIL:
			theArchEMail = (EMail *)archN->data;
			bufFile->write(&theArchEMail->pageNumber, sizeof(theArchEMail->pageNumber));
			break;
		case VOICEMAIL:
			theArchVoiceMail = (VoiceMail *)archN->data;
			bufFile->write(&theArchVoiceMail->soundNumber, sizeof(theArchVoiceMail->soundNumber));
			bufFile->write(&theArchVoiceMail->idNumber, sizeof(theArchVoiceMail->idNumber));
			break;
		}
		archN = archN->next;
	}
	//PCScreen
	for (i = 0; i < PC_MAXUSEDTYPE; i++)
		bufFile->write(&pcLastUsed[i], sizeof(pcLastUsed[i]));

	bufFile->write(&state96000, sizeof(state96000));
	bufFile->write(&attempt96000, sizeof(attempt96000));
	bufFile->write(&stakeOutTrigger, sizeof(stakeOutTrigger));

	//Photo Thing  
	if (GameFlag.test(fPrintedPhoto))
		havePhoto = true;
	else
		havePhoto = false;

	bufFile->write(&havePhoto, sizeof(havePhoto));
	if (havePhoto) {
		bufFile->write(&printedPhoto->numTargetObjects, sizeof(printedPhoto->numTargetObjects));
		for (i = 0; i < printedPhoto->numTargetObjects; i++) {
			bufFile->write(&printedPhoto->printObjects[i].theX, sizeof(printedPhoto->printObjects[i].theX));
			bufFile->write(&printedPhoto->printObjects[i].theY, sizeof(printedPhoto->printObjects[i].theY));
			bufFile->write(&printedPhoto->printObjects[i].theView, sizeof(printedPhoto->printObjects[i].theView));
			bufFile->write(&printedPhoto->printObjects[i].theLoop, sizeof(printedPhoto->printObjects[i].theLoop));
			bufFile->write(&printedPhoto->printObjects[i].theCel, sizeof(printedPhoto->printObjects[i].theCel));
			bufFile->write(&printedPhoto->printObjects[i].theScaleX, sizeof(printedPhoto->printObjects[i].theScaleX));
			bufFile->write(&printedPhoto->printObjects[i].theScaleY, sizeof(printedPhoto->printObjects[i].theScaleY));
		}
	}
	//Photo Thing
	bufFile->write(&lasttool, sizeof(lasttool));
	if (!bufFile->close()) {
		ErrMsg("Unable to close save file");
		ASSERT(0, 0);
	}

	return true; //DEBUG

}


//***************curDisk**************************
/*comlinks - done

//currentFocus - currentFocus must be 0 or don't allow save
//*******inv items***** done
//intrface->enabled -don't allow save while intrface disabled


//emailssame as comlinks-done
//permList - how is this used
//*********theGame->script*********
//theGame->prevRoomName - is this used? -done

//curMap NEEDED - done
//sounds list for loop infinite sounds or just sound1 -done
//music list for loop infinite music or just theMusic
//timers  ****we handle caller easily because it must be global, therefore we can grab all the info we need*/
//slowMachine global
//archive list


int SaveGame::restore(const char *whichRestGame) {
	//short oldDisc = curDisc;
	//must delete old comlinks and release (don't dispose or caller will be called and flag will be set)
	//timers, sounds, emails, music
	sound1->caller = 0;
	sound1->stop();
	sound2->caller = 0;
	sound2->stop();
	theMusic->stop();

	if (timers->size) {
		Timer *obj;
		Node *n = timers->head;
		while (n) {
			obj = (Timer *)n->data;
			n = n->next;
			obj->dispose();
		}
	}

	//	timers->dispose();
	hurryTimer = NULL;
	tryingToQuit = false;

	short i;
	for (i = 0; i < MAX_INV; i++)
		inventry->put(i);

	if (GameFlag.test(fPrintedPhoto))
		printedPhoto->dispose();

	comlinks->dispose(); //this calls DELETE
	sounds->dispose();
	archiveList->dispose();
	for (i = 0; i < permList->size; i++)
		permList->at(i)->dispose();
	permList->releaseAll();

	//permList->dispose(); // may not be correct chk w/room progs



	if (!bufFile->open(whichRestGame, FILE_READ))
		return false;							//open the save file

	bufFile->read(&GameFlag.data, FLAG_SIZE * sizeof(unsigned long));	//our flags #1

	restRoom->theNextRoom = bufFile->readString();						//this is the room we will restore #2

	curRoom->name = bufFile->readString();								//this will cause theGame->prevRoomName #3
	//to be set correctly on room change 
	bufFile->read(&gameTime, sizeof(gameTime));						//restore our game time #4

	bufFile->read(&curMap, sizeof(curMap));							//restore curMap #5

	bufFile->read(&curDisc, sizeof(curDisc));							//restore curMap #5

	bufFile->read(&beatCountArray, MAX_BEATS * sizeof(unsigned));
	bufFile->read(&beatArray, MAX_DELAY_BEATS * 2 * sizeof(unsigned));
	bufFile->read(&beatCount, sizeof(beatCount));

	bufFile->read(&numInvOwned, sizeof(numInvOwned));							//short number of vlinks 
	short theIndx;
	for (i = 0; i < numInvOwned; i++) {
		bufFile->read(&theIndx, sizeof(theIndx));
		inventry->get(theIndx);
	}

	//comlinks ******************************************************
	char *msgName;
	char *attachName;
	char *movieName;
	unsigned long time0;
	unsigned long mySoundNumber, myPageNumber, myAttachPageNumber;
	short  numVLinks, myComType;
	unsigned short myFlag, myDelay, myAttachFlag;
	int mySoundID;

	bufFile->read(&numVLinks, sizeof(numVLinks));							//short number of vlinks 

	for (i = 0; i < numVLinks; i++) {
		msgName = bufFile->readString();								//char* msgName	
		bufFile->read(&myComType, sizeof(myComType));					//unsigned short2			
		bufFile->read(&time0, sizeof(time0));  							//unsigned long4	
		bufFile->read(&myFlag, sizeof(myFlag));   						//unsigned short2   
		bufFile->read(&myDelay, sizeof(myDelay)); 						//unsigned short2	

		switch (myComType) {
		case VLINKMAIL:
			movieName = bufFile->readString();
			bufFile->read(&haveAttach, sizeof(haveAttach));
			if (haveAttach) {
				attachName = bufFile->readString();
				bufFile->read(&myAttachPageNumber, sizeof(myAttachPageNumber));
				bufFile->read(&myAttachFlag, sizeof(myAttachFlag));
				new VLink(msgName, movieName, myFlag, myDelay / 60, new EMailAttachment(attachName, myAttachPageNumber, myAttachFlag, 0));
			} else
				new VLink(msgName, movieName, myFlag, myDelay / 60);
			break;
		case EMAIL:
			bufFile->read(&myPageNumber, sizeof(myPageNumber));
			new EMail(msgName, myPageNumber, myFlag, myDelay / 60);
			break;
		case VOICEMAIL:
			bufFile->read(&mySoundNumber, sizeof(mySoundNumber));
			bufFile->read(&mySoundID, sizeof(mySoundID));
			new VoiceMail(msgName, mySoundNumber, mySoundID, myFlag, myDelay / 60); // myDelay / 60 to get seconds
			break;
		}
	}
	//VLinks *********************************************************



	//archives ******************************************************
	char *archMsgName;
	char *archAttachName;
	char *archMovieName;
	unsigned long archTime0;
	unsigned long myArchSoundNumber, myArchPageNumber, myArchAttachPageNumber;
	short  numArchVLinks, myArchComType;
	unsigned short myArchFlag, myArchDelay, myArchAttachFlag;
	int myArchSoundID;

	bufFile->read(&numArchVLinks, sizeof(numArchVLinks));							//short number of vlinks 

	for (i = 0; i < numArchVLinks; i++) {
		archMsgName = bufFile->readString();								//char* msgName	
		bufFile->read(&myArchComType, sizeof(myArchComType));					//unsigned short2			
		bufFile->read(&archTime0, sizeof(archTime0));  							//unsigned long4	
		bufFile->read(&myArchFlag, sizeof(myArchFlag));   						//unsigned short2   
		bufFile->read(&myArchDelay, sizeof(myArchDelay)); 						//unsigned short2	

		switch (myArchComType) {
		case VLINKMAIL:
			archMovieName = bufFile->readString();
			bufFile->read(&haveArchAttach, sizeof(haveArchAttach));
			if (haveArchAttach) {
				archAttachName = bufFile->readString();
				bufFile->read(&myArchAttachPageNumber, sizeof(myArchAttachPageNumber));
				bufFile->read(&myArchAttachFlag, sizeof(myArchAttachFlag));
				new VLink(archMsgName, archMovieName, new EMailAttachment(archAttachName, myArchAttachPageNumber, 0, 0), 2);
			} else
				new VLink(archMsgName, archMovieName, (EMailAttachment *)NULL, 2);
			break;
		case EMAIL:
			bufFile->read(&myArchPageNumber, sizeof(myArchPageNumber));
			new EMail(archMsgName, myArchPageNumber, 2);
			break;
		case VOICEMAIL:
			bufFile->read(&myArchSoundNumber, sizeof(myArchSoundNumber));
			bufFile->read(&myArchSoundID, sizeof(myArchSoundID));
			new VoiceMail(archMsgName, myArchSoundNumber, myArchSoundID, 2); // myDelay / 60 to get seconds
			break;
		}
	}
	//archives*********************************************************


	//PCScreen ******************************
	for (i = 0; i < PC_MAXUSEDTYPE; i++) {
		bufFile->read(&pcArrayInfo, sizeof(pcArrayInfo));
		pcLastUsed[i] = pcArrayInfo;
	}
	//PCScreen ******************************

	bufFile->read(&state96000, sizeof(state96000));
	bufFile->read(&attempt96000, sizeof(attempt96000));
	bufFile->read(&stakeOutTrigger, sizeof(stakeOutTrigger));


	//Photo Thing  


	bufFile->read(&havePhoto, sizeof(havePhoto));
	if (havePhoto) {
		//clear old save state in photo stuff
		new PrintedPhoto();
		bufFile->read(&printedPhoto->numTargetObjects, sizeof(printedPhoto->numTargetObjects));
		for (i = 0; i < printedPhoto->numTargetObjects; i++) {
			bufFile->read(&printedPhoto->printObjects[i].theX, sizeof(printedPhoto->printObjects[i].theX));
			bufFile->read(&printedPhoto->printObjects[i].theY, sizeof(printedPhoto->printObjects[i].theY));
			bufFile->read(&printedPhoto->printObjects[i].theView, sizeof(printedPhoto->printObjects[i].theView));
			bufFile->read(&printedPhoto->printObjects[i].theLoop, sizeof(printedPhoto->printObjects[i].theLoop));
			bufFile->read(&printedPhoto->printObjects[i].theCel, sizeof(printedPhoto->printObjects[i].theCel));
			bufFile->read(&printedPhoto->printObjects[i].theScaleX, sizeof(printedPhoto->printObjects[i].theScaleX));
			bufFile->read(&printedPhoto->printObjects[i].theScaleY, sizeof(printedPhoto->printObjects[i].theScaleY));
		}
	}
	//Photo Thing
	bufFile->read(&lasttool, sizeof(lasttool));

	if (!bufFile->close()) {
		ErrMsg("Can't close restore file");
		ASSERT(0, 0);
	}



	if (theGame)
		colby.checkINI();
	//if (oldDisc != curDisc) //DEBUG for file based
	//	sfxPrintf("Please insert disc %d", curDisc);

	return true; //DEBUG
}

} // namespace Spycraft
