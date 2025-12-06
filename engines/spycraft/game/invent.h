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

#ifndef SPYCRAFT_GAME_INVENT_H
#define SPYCRAFT_GAME_INVENT_H

#include "spycraft/game/object.h"
#include "spycraft/game/feature.h"
#include "spycraft/game/actor.h"
#include "spycraft/game/views.h"
#include "spycraft/game/interface.h"
#include "spycraft/game/inset.h"

namespace Spycraft {

//#define MAX_INV 20  see enum below --dck
#define DELTA_X_INV 30
#define ROW2_Y_INV 397
#define INV_VIEW_PRI 80
#define HAND_X 100
#define HAND_Y 426

// inventory items
enum {
	iLINT,
	iYURI_DOSSIER,
	iPROCATFILE,
	iNEEDLEPACK,
	iPEG,
	iLOCKPICK,
	iFOLDER_CITATION,
	iFOLDER_PAYMENTS,
	iDOSSIER_ZHIRONEV,
	iDOSSIER_SHUSTAK,
	iFOLDER_ORDERS,
	iOTHELLO_PICTURE,
	iVIDEO_CAMERA,
	iSECURITY_DIAGRAMS,
	iRIFLE,
	iGOGGLES,
	iLASERTAGGUN,
	iHANDCUFFS,
	iORANGESWAN,
	iMUSICSWAN,
	iYINGPHOTO,
	iYINGINTERNETGUIDE,
	iEBMCATALOG,
	iYINGBEALEPOSTIT,
	iYINGBEOWULF,
	iSHANGHAIDISK,
	iNUMBERTOYURI,
	iNUMBERTOBLAKE,
	iHACKERDISK,
	iPHOTODOCPICTURE,
	iONYX_DOSSIER,
	iPHOTOPARKER,
	MAX_INV
};


//Individual items

class InvItem : public IntrView {
public:
	const char *name;
	int         verb;
	int         handView;     	//what it looks like in case
	int         owned;          //does the player own it
	int			current;		//is the player using it now
	int			myIndex;
	Inset *myInset;		//what does this look like up close?
	int 		insetView;
	InvItem();
	void activate();
	int handleEvent(MADEEventStamp *event);
	void dispose(void);
	void respond(void);
};

//The hand on the intrface that holds our current item

class InvHand : public IntrView {
public:
	Inset *myInset;
	int		indexNum;
	int 	insetViewNum;

	InvHand();
	~InvHand();
	void dispose(void);
	void init(Inset *theInset, int indxNum, int theView, int theInsetView);
	void init(Inset *theInset, int indxNum, int theView, int theX, int theY, int theInsetView);
	int handleEvent(MADEEventStamp *event);
	void respond(void);

};

//Contains array list of all possible items

class Invent : public Object {
public:
	const char *name;
	InvItem *items[MAX_INV];    //Array of InvItems
	int    		numOwned;
	InvHand *theHand;
	short		active;
	short		curItem;
	Invent();
	~Invent();
	void show();
	void hide();
	int get(short theIndex);       //give item to player
	int put(short theIndex);       //take item from player
	int has(short theIndex);       //does player own this item
	int handleEvent(MADEEventStamp *event);
	int clearCurrentItem(void);
	void goForward(void);
	void goBackward(void);

};

//This is the view of the backround grid

class InvView : public IntrView {
public:
	const char *name;
	InvView();
	void init(int x_start, int y_start);  //where to show it
	int handleEvent(MADEEventStamp *event);
	void dispose(void);
};



//Our first inventory item is lint because everybody has some 

class Lint : public InvItem {
public:
	const char *name;
	Lint();
};

class ProcatFile : public InvItem {
public:
	const char *name;
	ProcatFile();
};

class NeedlePack : public InvItem {
public:
	const char *name;
	NeedlePack();
};

class PEG : public InvItem {
public:
	const char *name;
	PEG();
};

class LockPick : public InvItem {
public:
	const char *name;
	LockPick();
};

class YuriDossier : public InvItem {
public:
	const char *name;
	YuriDossier();
};

class FolderCitation : public InvItem {
public:
	const char *name;
	FolderCitation();
};

class FolderPayments : public InvItem {
public:
	const char *name;
	FolderPayments();
};

class DossierZhironev : public InvItem {
public:
	const char *name;
	DossierZhironev();
};

class DossierShustak : public InvItem {
public:
	const char *name;
	DossierShustak();
};

class FolderOrders : public InvItem {
public:
	const char *name;
	FolderOrders();
};

class OthelloPicture : public InvItem {
public:
	const char *name;
	OthelloPicture();
};

class VideoCamera : public InvItem {
public:
	const char *name;
	VideoCamera();
};

class SecurityDiagrams : public InvItem {
public:
	const char *name;
	SecurityDiagrams();
};

class Rifle : public InvItem {
public:
	const char *name;
	Rifle();
};

class Goggles : public InvItem {
public:
	const char *name;
	Goggles();
};

class LaserTagGun : public InvItem {
public:
	const char *name;
	LaserTagGun();
};

class Handcuffs : public InvItem {
public:
	const char *name;
	Handcuffs();
};

class OrangeSwan : public InvItem {
public:
	const char *name;
	OrangeSwan();
};

class MusicSwan : public InvItem {
public:
	const char *name;
	MusicSwan();
};

class YingPhoto : public InvItem {
public:
	const char *name;
	YingPhoto();
};

class YingInternetGuide : public InvItem {
public:
	const char *name;
	YingInternetGuide();
};

class YingEBMCatalog : public InvItem {
public:
	const char *name;
	YingEBMCatalog();
};

class YingBealePostit : public InvItem {
public:
	const char *name;
	YingBealePostit();
};

class YingBeowulf : public InvItem {
public:
	const char *name;
	YingBeowulf();
};

class ShanghaiDisk : public InvItem {
public:
	const char *name;
	ShanghaiDisk();
};

class NumbertoYuri : public InvItem {
public:
	const char *name;
	NumbertoYuri();
};

class NumbertoBlake : public InvItem {
public:
	const char *name;
	NumbertoBlake();
};

class HackerDisk : public InvItem {
public:
	const char *name;
	HackerDisk();
};

class PhotoDocPicture : public InvItem {
public:
	const char *name;
	PhotoDocPicture();
};

class OnyxDossier : public InvItem {
public:
	const char *name;
	OnyxDossier();
};

class PhotoParker : public InvItem {
public:
	const char *name;
	PhotoParker();
};

extern Invent *inventry;
extern int invVerb;

} // namespace Spycraft

#endif

