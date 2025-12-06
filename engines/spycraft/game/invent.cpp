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

//This is where we handle the player's inventory

#include "spycraft/game/invent.h"
#include "spycraft/game/web.h"
#include "spycraft/game/interface.h"
#include "spycraft/game/invinset.h"
#include "spycraft/game/phodoc.h"
#include "spycraft/logic/1000.h"
#include "spycraft/game/movie.h"

#ifdef DIRECTX
#include "spycraft/dmade/advmovie.h"
#define PLAY_BRIEF_INVENT     !sfxIsMovieOpen()
#else
#include "spycraft/dmade/mcimovie.h"
#define PLAY_BRIEF_INVENT     !sfxIsMCIOpen()
#endif

namespace Spycraft {

extern Sound *briefSound;
extern BlinkCursor *normalCursor;
extern ArrowCursor *arrowCursor;
extern InvCursor *invCursor;
extern Briefcase *theCase;
extern Intrface *intrface;
Invent *inventry = new(Invent);
int invVerb;

extern InvInset *theInvInset;

InvView *theInvView;

InvItem::InvItem() {
	name = "invItem";
	owned = false;
	insetView = 0;
	myInset = 0;
}

void InvItem::activate() {
	//normalCursor->verb = verb;
	//user->message = verb;
	invVerb = verb;
	if (inventry->theHand)
		inventry->theHand->dispose();
	inventry->theHand = new InvHand;
	inventry->theHand->init(myInset, myIndex, handView, insetView);
	inventry->curItem = myIndex;
	inventry->active = true;
	inventry->theHand->setHotspot(ALL_V);
}

int InvItem::handleEvent(MADEEventStamp *event) {
	if ((onMe(event)) && (event_type & USER_LEFT_UP)) {
		if (!inventry->active) {
			inventry->hide();
			activate();
			theGame->resetHotspot();
			//theCase->close();

			return(true);
		} else {
			normalCursor->verb = DO_V;
			theCase->close();
			return(true);
		}
	}
	return(false);
}

void InvItem::respond(void) {
	sfxSetCursor(970, 0, 1);
}

void InvItem::dispose() {
	if ((view >= 0) && (mySprite)) {
		sfxUnlockRes(view, RES_ATS);
		sfxKillSprite(this->mySprite);
		mySprite = 0;
	}
	intrface->release(this);
}

InvHand::InvHand() {
	x = HAND_X;
	y = HAND_Y;
}

void InvHand::dispose() {
	inventry->theHand = NULL;
	IntrView::dispose();
}

InvHand::~InvHand() {
	//inventry->theHand = NULL;
}

int InvHand::handleEvent(MADEEventStamp *event) {
	if ((onMe(event)) && (event_type & USER_LEFT_UP)) {
		if (insetViewNum)
		{
			//if (!strcmp(curRoom->name, "Rm1000"))
			//	rm1000->cleanUp();

			if (insetViewNum == 706)
			{
				printedPhoto->showPhoto();
			} else if (insetViewNum < 50000)
			{

				if (!strcmp(curRoom->name, "Rm1270"))
					theMovie->pause(false);
				theInvInset->init(insetViewNum, 0, 16, 512, 384);

			} else
				new Yellow(insetViewNum);
		}
		//sfxPrintf("Show inventory close up of %s.", inventry->items[indexNum]->name);
		return true;
	}
	return false;
}

void InvHand::init(Inset *inset, int indxNum, int viewNum, int theInsetView) {
	loop = viewNum;				//bitmap
	x = HAND_X;
	y = HAND_Y;
	view = 960;
	cel = 0;
	draw();
	intrface->addToFront(this);
	myInset = inset;			//here is the close up
	indexNum = indxNum;			//index in inventory list
	insetViewNum = theInsetView;
}

void InvHand::respond(void) {
	sfxSetCursor(970, 0, 1);
}

void InvHand::init(Inset *inset, int indxNum, int viewNum, int theX, int theY, int theInsetView) {
	loop = viewNum;				//bitmap
	x = theX;
	y = theX;
	view = 960;
	cel = 0;
	draw();
	intrface->addToFront(this);
	myInset = inset;			//here is the close up
	indexNum = indxNum;			//index in inventory list
	insetViewNum = theInsetView;
}


Invent::Invent() {
	name = "invent";
	theHand = NULL;
	active = 0;
	curItem = -1;

	items[0] = new(Lint);
	items[0]->myIndex = 0;

	items[1] = new(YuriDossier);
	items[1]->myIndex = 1;

	items[2] = new(ProcatFile);
	items[2]->myIndex = 2;

	items[3] = new(NeedlePack);
	items[3]->myIndex = 3;

	items[4] = new(PEG);
	items[4]->myIndex = 4;

	items[5] = new(LockPick);
	items[5]->myIndex = 5;

	items[6] = new(FolderCitation);
	items[6]->myIndex = 6;

	items[7] = new(FolderPayments);
	items[7]->myIndex = 7;

	items[8] = new(DossierZhironev);
	items[8]->myIndex = 8;

	items[9] = new(DossierShustak);
	items[9]->myIndex = 9;

	items[10] = new(FolderOrders);
	items[10]->myIndex = 10;

	items[11] = new(OthelloPicture);
	items[11]->myIndex = 11;

	items[12] = new(VideoCamera);
	items[12]->myIndex = 12;

	items[13] = new(SecurityDiagrams);
	items[13]->myIndex = 13;

	items[14] = new(Rifle);
	items[14]->myIndex = 14;

	items[15] = new(Goggles);
	items[15]->myIndex = 15;

	items[16] = new(LaserTagGun);
	items[16]->myIndex = 16;

	items[17] = new(Handcuffs);
	items[17]->myIndex = 17;

	items[18] = new(OrangeSwan);
	items[18]->myIndex = 18;

	items[19] = new(MusicSwan);
	items[19]->myIndex = 19;

	items[iYINGPHOTO] = new(YingPhoto);
	items[iYINGPHOTO]->myIndex = iYINGPHOTO;

	items[iYINGINTERNETGUIDE] = new(YingInternetGuide);
	items[iYINGINTERNETGUIDE]->myIndex = iYINGINTERNETGUIDE;

	items[iEBMCATALOG] = new(YingEBMCatalog);
	items[iEBMCATALOG]->myIndex = iEBMCATALOG;

	items[iYINGBEALEPOSTIT] = new(YingBealePostit);
	items[iYINGBEALEPOSTIT]->myIndex = iYINGBEALEPOSTIT;

	items[iYINGBEOWULF] = new(YingBeowulf);
	items[iYINGBEOWULF]->myIndex = iYINGBEOWULF;

	items[iSHANGHAIDISK] = new(ShanghaiDisk);
	items[iSHANGHAIDISK]->myIndex = iSHANGHAIDISK;

	items[26] = new(NumbertoYuri);
	items[26]->myIndex = 26;

	items[27] = new(NumbertoBlake);
	items[27]->myIndex = 27;

	items[28] = new(HackerDisk);
	items[28]->myIndex = 28;

	items[29] = new(PhotoDocPicture);
	items[29]->myIndex = 29;

	items[30] = new(OnyxDossier);
	items[30]->myIndex = 30;

	items[31] = new(PhotoParker);
	items[31]->myIndex = 31;
}

void Invent::goForward() {
	int i;
	for (i = curItem + 1; i < MAX_INV; i++) {
		if (inventry->has(i)) {
			if (PLAY_BRIEF_INVENT)
				briefSound->play(103, NULL);
			hide();
			curItem = i;
			show();
			return;
		}
	}
	for (i = 0; i < curItem; i++) {
		if (inventry->has(i)) {
			if (PLAY_BRIEF_INVENT)
				briefSound->play(103, NULL);
			hide();
			curItem = i;
			show();
			return;
		}
	}

}

void Invent::goBackward() {
	int i;
	for (i = curItem - 1; i >= 0; i--) {
		if (inventry->has(i)) {
			if (PLAY_BRIEF_INVENT)
				briefSound->play(103, NULL);
			hide();
			curItem = i;
			show();
			return;
		}
	}
	for (i = MAX_INV - 1; i > curItem; i--) {
		if (inventry->has(i)) {
			if (PLAY_BRIEF_INVENT)
				briefSound->play(103, NULL);
			hide();
			curItem = i;
			show();
			return;
		}
	}

}

Invent::~Invent() {
	delete items[0];	//DEBUG as we add items we must also delete them
	delete items[1];
	delete items[2];
	delete items[3];
	delete items[4];
	delete items[5];
	delete items[6];
	delete items[7];
	delete items[8];
	delete items[9];
	delete items[10];
	delete items[11];
	delete items[12];
	delete items[13];
	delete items[14];
	delete items[15];
	delete items[16];
	delete items[17];
	delete items[18];
	delete items[19];
	delete items[iYINGPHOTO];
	delete items[iYINGINTERNETGUIDE];
	delete items[iEBMCATALOG];
	delete items[iYINGBEALEPOSTIT];
	delete items[iYINGBEOWULF];
	delete items[iSHANGHAIDISK];
	delete items[26];
	delete items[27];
	delete items[28];
	delete items[29];
	delete items[30];
	delete items[31];
	//delete theHand;
	//delete this;
}

int Invent::handleEvent(MADEEventStamp *event) {
	return true;
}

void Invent::show() {
	int i = 0, theX = 30, theY = 422; // DEBUG use INV_Y when finalized;

	if (curItem == -1) {
		while (i < MAX_INV) {
			if ((this->items[i]) && (this->has(i)))
				curItem = i;
			break;
			++i;
		}
	}
	if ((curItem != -1) && (inventry->has(curItem))) {
		inventry->items[curItem]->init(theX, theY);
		inventry->items[curItem]->setPri(490);   //INV_VIEW_PRI;
		//	active = true;
	}
}

void Invent::hide() {
	if ((curItem != -1) && (this->items[curItem]->mySprite) && (this->has(curItem)))
		inventry->items[curItem]->dispose(); //InvItem dispose does not use deathlist
	//active = false; 
}

int Invent::get(short theIndex) {
	if (theIndex > (MAX_INV - 1)) {
		sfxPrintf("index larger than MAX inventory");
		return false;
	}
	inventry->items[theIndex]->owned = true;
	curItem = theIndex;
	return true;
}

int  Invent::put(short theIndex) {
	if (theIndex > (MAX_INV - 1)) {
		sfxPrintf("index larger than MAX inventory");
		return false;
	}
	inventry->items[theIndex]->owned = false;
	if (theIndex == curItem)
		curItem = -1;
	return true;
}

int Invent::has(short theIndex) {
	if (theIndex > (MAX_INV - 1)) {
		sfxPrintf("index larger than MAX inventory");
		return false;
	}
	if (inventry->items[theIndex]->owned)
		return true;
	else
		return false;
}

int Invent::clearCurrentItem(void) {
	if (active) {
		normalCursor->verb = DO_V;
		user->message = DO_V;
		invVerb = 0;
		active = false;
		if (theHand) {
			theHand->dispose();
			theHand = NULL;
		}
		return true;
	} else
		return false;
}

InvView::InvView() {
	name = "invView";
	view = vINTERFACE;
	loop = 2;
	cel = 0;
	priority = INV_VIEW_PRI;
}

int InvView::handleEvent(MADEEventStamp *event) {
	return false;
}

void InvView::init(int theX, int theY) {
	IntrView::init(theX, theY);
	//inventry->show();
}

void InvView::dispose() {
	//inventry->hide();
	IntrView::dispose();
}

Lint::Lint() {
	name = "Lint";
	view = vINVCURSORS;
	loop = 0;
	cel = 0;
	verb = LINT_V;
	handView = loop;
	owned = false;
}

ProcatFile::ProcatFile() {
	name = "ProcatFile";

	view = 960;
	loop = 18;
	cel = 0;

	verb = PROCATFILE_V;
	handView = loop;
	insetView = 99430;

	owned = false;
}

NeedlePack::NeedlePack() {
	name = "NeedlePack";

	view = 960;
	loop = 19;
	cel = 0;

	verb = NEEDLEPACK_V;
	handView = loop;
	//	insetView = 976;
	insetView = 38160;
	owned = false;
}

PEG::PEG() {
	name = "PEG";

	view = 960;
	loop = 20;
	cel = 0;

	verb = PEG_V;
	handView = loop;

	owned = false;
}

LockPick::LockPick() {
	name = "LOCKPICK";
	view = 960;
	loop = 3;
	cel = 0;
	verb = LOCKPICK_V;
	handView = loop;
	insetView = 963;
	owned = false;
}

YuriDossier::YuriDossier() {
	name = "YURIDOSSIER";

	view = 960;
	loop = 7;
	cel = 0;
	verb = YURIDOSSIER_V;
	handView = loop;
	insetView = 99046;
	owned = false;
}

FolderCitation::FolderCitation() {
	name = "FOLDERCITATION";

	view = 960;
	loop = 13;
	cel = 0;
	verb = FOLDERCITATION_V;
	handView = loop;
	insetView = 98670;

	owned = false;
}

FolderPayments::FolderPayments() {
	name = "FOLDERPAYMENTS";

	view = 960;
	loop = 11;
	cel = 0;
	verb = FOLDERPAYMENTS_V;
	handView = loop;
	insetView = 98660;
	owned = false;
}

DossierZhironev::DossierZhironev() {
	name = "DOSSIERZHIRONEV";

	view = 960;
	loop = 14;
	cel = 0;
	verb = DOSSIERZHIRONEV_V;
	handView = loop;

	owned = false;
}

DossierShustak::DossierShustak() {
	name = "DOSSIERSHUSTAK";

	view = 960;
	loop = 15;
	cel = 0;
	verb = DOSSIERSHUSTAK_V;
	handView = loop;
	owned = false;
}

FolderOrders::FolderOrders() {
	name = "FOLDERORDERS";
	view = 960;
	loop = 12;
	cel = 0;

	verb = FOLDERORDERS_V;
	handView = loop;
	insetView = 98650;
	owned = false;
}

OthelloPicture::OthelloPicture() {
	name = "OTHELLOPICTURE";
	view = vOTHELLOPICTURE;
	verb = OTHELLOPICTURE_V;
	handView = loop;

	owned = false;
}

VideoCamera::VideoCamera() {
	name = "VIDEOCAMERA";

	view = 960;
	loop = 8;
	cel = 0;
	verb = VIDEOCAMERA_V;
	handView = loop;
	insetView = 968;
	owned = false;
}

SecurityDiagrams::SecurityDiagrams() {
	name = "SECURITYDIAGRAMS";
	view = 960;
	loop = 17;
	cel = 0;

	verb = SECURITYDIAGRAMS_V;
	handView = loop;

	owned = false;
}

Rifle::Rifle() {
	name = "RIFLE";

	view = 960;
	loop = 21;
	cel = 0;

	verb = RIFLE_V;
	handView = loop;

	owned = false;
}

Goggles::Goggles() {
	name = "GOGGLES";
	view = vRIFLE;
	verb = GOGGLES_V;
	handView = loop;

	owned = false;
}

LaserTagGun::LaserTagGun() {
	name = "LASERTAGGUN";
	view = vRIFLE;
	verb = LASERTAGGUN_V;
	handView = loop;

	owned = false;
}

Handcuffs::Handcuffs() {
	name = "HANDCUFFS";

	view = 960;
	loop = 26;
	cel = 0;

	verb = HANDCUFFS_V;
	handView = loop;
	insetView = 983;

	owned = false;
}

OrangeSwan::OrangeSwan() {
	name = "ORANGESWAN";
	view = vORANGESWAN;
	verb = ORANGESWAN_V;
	handView = loop;

	owned = false;
}

MusicSwan::MusicSwan() {
	name = "MUSICSWAN";

	view = 960;
	loop = 9;
	cel = 0;
	verb = MUSICSWAN_V;
	handView = loop;
	insetView = 969;

	owned = false;
}



YingPhoto::YingPhoto() {
	name = "YINGPHOTO";
	view = 960;
	loop = 0;
	cel = 0;
	verb = YINGPHOTO_V;
	insetView = 961;

	handView = loop;
	owned = false;
}

YingInternetGuide::YingInternetGuide() {
	name = "YINGINTERNETGUIDE";
	view = 960;
	loop = 4;
	cel = 0;

	verb = YINGINTERNETGUIDE_V;
	handView = loop;
	insetView = 964;
	owned = false;
}

YingEBMCatalog::YingEBMCatalog() {
	name = "YINGEBMCATALOG";
	view = 960;
	loop = 28;
	cel = 0;

	verb = YINGEBMCATALOG_V;
	handView = loop;
	insetView = 24199;

	owned = false;
}

YingBealePostit::YingBealePostit() {
	name = "YINGBEALEPOSTIT";
	view = 960;
	loop = 1;
	cel = 0;
	verb = YINGBEALEPOSTIT_V;
	handView = loop;
	insetView = 962;
	owned = false;
}

YingBeowulf::YingBeowulf() {
	name = "YINGBEOWULF";
	view = 960;
	loop = 5;
	cel = 0;

	verb = YINGBEOWULF_V;
	handView = loop;
	insetView = 965;
	owned = false;
}

ShanghaiDisk::ShanghaiDisk() {
	name = "SHANGHAIDISK";

	view = 960;
	loop = 6;
	cel = 0;
	verb = SHANGHAIDISK_V;
	handView = loop;
	insetView = 966;
	owned = false;
}

NumbertoYuri::NumbertoYuri() {
	name = "NUMBERTOYURI";
	view = 960;
	loop = 10;
	cel = 0;

	verb = NUMBERTOYURI_V;
	handView = loop;
	insetView = 971;

	owned = false;
}

NumbertoBlake::NumbertoBlake() {
	name = "NUMBERTOBLAKE";

	view = 960;
	loop = 22;
	cel = 0;

	verb = NUMBERTOBLAKE_V;
	handView = loop;
	insetView = 979;
	owned = false;
}

HackerDisk::HackerDisk() {
	name = "HACKERDISK";

	view = 960;
	loop = 23;
	cel = 0;

	verb = HACKERDISK_V;
	handView = loop;
	insetView = 980;

	owned = false;
}

PhotoDocPicture::PhotoDocPicture() {
	name = "PhotoDocPicture";

	view = 960;
	loop = 2;
	cel = 0;

	verb = PHOTODOCPICTURE_V;
	handView = loop;
	insetView = 706;

	owned = false;
}

OnyxDossier::OnyxDossier() {
	name = "OnyxDossier";

	view = 960;
	loop = 25;
	cel = 0;

	verb = ONYXDOSSIER_V;
	handView = loop;
	insetView = 98638;

	owned = false;
}

PhotoParker::PhotoParker() {
	name = "PhotoParker";

	view = 960;
	loop = 27;
	cel = 0;

	verb = PHOTOPARKER_V;
	handView = loop;
	insetView = 986;

	owned = false;
}

} // namespace Spycraft
