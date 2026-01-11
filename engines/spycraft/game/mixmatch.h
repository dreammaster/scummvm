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

#ifndef SPYCRAFT_GAME_MIXMATCH_H
#define SPYCRAFT_GAME_MIXMATCH_H

#include "spycraft/game/interface_puzzle.h"
#include "spycraft/game/sound.h"
#include "spycraft/game/web.h"

namespace Spycraft {

#define MAX_PHOTOS		15
#define MAX_PARTS		5
#define SEP				33

#define FAIRPARTS		801
#define MEDIUMPARTS		802
#define DARKPARTS		803

#define EYESFAIR		811
#define EYESMEDIUM		812
#define EYESDARK		813

#define FACEHAIRFAIR	808
#define FACEHAIRMEDIUM	809
#define FACEHAIRDARK	810

#define BROWNHAIR_VIEW	805
#define	REDHAIR_VIEW	807
#define BLACKHAIR_VIEW	806
#define BLONDEHAIR_VIEW	804

#define NA		5;

enum
{
	empty,
	empty1,
	empty2,
	HEADPRI,
	IRISPRI,
	EYESPRI,
	HAIRPRI,
	BEARDPRI,
	NOSEPRI,
	MOUTHPRI
};

enum
{
	BLUE_EYES,
	BROWN_EYES,
	GREEN_EYES
};

enum
{
	MALE,
	FEMALE
};

enum
{
	BROWN_HAIR,
	DARK_HAIR,
	BLONDE_HAIR,
	RED_HAIR
};

enum
{
	FAIR_SKIN,
	MEDIUM_SKIN,
	DARK_SKIN
};

class SearchButt : public Button
{
public:

	SearchButt();
	virtual void activate();
};

class AmesSendBut : public WebReport
{
public:
	AmesSendBut();
	virtual void activate();
	void cue();
};

class KatSendBut : public WebReport
{
public:
	KatSendBut();
	virtual void activate();
	void cue();
};

class GrendelSendBut : public WebReport
{
public:
	GrendelSendBut();
	virtual void activate();
	void cue();
};

class KneeSendBut : public WebReport
{
public:
	KneeSendBut();
	virtual void activate();
	void cue();
};

class MMButton : public Button
{
public:
	bool buttonDown;

	int handleEvent(MADEEventStamp *);
};

class FaceBut : public MMButton
{
public:
	FaceBut();
	int handleEvent(MADEEventStamp *);
};

class HairBut : public MMButton
{
public:
	HairBut();
	int handleEvent(MADEEventStamp *);
};

class EyesBut : public MMButton
{
public:
	EyesBut();
	int handleEvent(MADEEventStamp *);
};

class NoseBut : public MMButton
{
public:
	NoseBut();
	int handleEvent(MADEEventStamp *);
};

class MouthBut : public MMButton
{
public:
	MouthBut();
	int handleEvent(MADEEventStamp *);
};

class BeardBut : public MMButton
{
public:
	BeardBut();
	int handleEvent(MADEEventStamp *);
};

class GeneralBut : public MMButton
{
public:
	GeneralBut();
	int handleEvent(MADEEventStamp *);
};

//Dialog Box
class MatchDoneButton : public Button
{
public:
	MatchDoneButton();
	void activate();
};

class RadioMark : public IntrPuzzView
{
public:
	RadioMark();
};

class EyesFeature : public IntrPuzzFeature
{
public:
	int type;

	int handleEvent(MADEEventStamp *);
	void setMark();
};

class BrownEyes : public EyesFeature
{
public:
	BrownEyes();
};

class BlueEyes : public EyesFeature
{
public:
	BlueEyes();
};

class GreenEyes : public EyesFeature
{
public:
	GreenEyes();
};

class NAEyes : public EyesFeature
{
public:
	NAEyes();
};

class GenderFeature : public IntrPuzzFeature
{
public:
	int type;

	int handleEvent(MADEEventStamp *);
	void setMark();
};

class Male : public GenderFeature
{
public:
	Male();
};

class Female : public GenderFeature
{
public:
	Female();
};

class HairFeature : public IntrPuzzFeature
{
public:
	int type;

	int handleEvent(MADEEventStamp *);
	void setMark();
};

class DarkHair : public HairFeature
{
public:
	DarkHair();
};

class BrownHair : public HairFeature
{
public:
	BrownHair();
};

class BlondeHair : public HairFeature
{
public:
	BlondeHair();
};

class RedHair : public HairFeature
{
public:
	RedHair();
};

class SkinFeature : public IntrPuzzFeature
{
public:
	int type;

	int handleEvent(MADEEventStamp *);
	void setMark();
};

class FairSkin : public SkinFeature
{
public:
	FairSkin();
};

class MediumSkin : public SkinFeature
{
public:
	MediumSkin();
};

class DarkSkin : public SkinFeature
{
public:
	DarkSkin();
};

class MatchBox : public Object
{
public:
	int setting[3];

	RadioMark *hairMark;
	RadioMark *eyeMark;
	RadioMark *skinMark;
	RadioMark *genderMark;

	MatchBox();
};


class GenderBut : public Button
{
public:
	GenderBut();
	int handleEvent(MADEEventStamp *);
};

class BodyParts : public IntrPuzzView
{
public:
	int numCels;
	int currentCel;

	void init(int x, int y, int view, int loop, int cel, int numcels, int priority);
	void dispose();
};

class RightArrow : public Button
{
public:
	RightArrow();
	virtual void activate();
};

class LeftArrow : public Button
{
public:
	LeftArrow();
	virtual void activate();
};

class MixBack : public IntrPuzzView
{
public:
	MixBack();
};

class ImageSlider : public Slider
{
public:
	ImageSlider();
	int handleEvent(MADEEventStamp *event);
};

/*class MatchSFX : public Sound
{
public:

};
*/
class MatchPhoto : public IntrPuzzView
{
public:
	MatchPhoto(int photoView);
};

class MixMatch : public IntrPuzzPage
{
public:
	BodyParts *face;
	BodyParts *eyes;
	BodyParts *nose;
	BodyParts *hair;
	BodyParts *mouth;
	BodyParts *irises;
	BodyParts *beard;
	BodyParts *currentPart;
	int PhotoView;
	int PhotoLoop;
	int PhotoCel;
	int box_originX;
	int box_originY;
	bool windowDrag;
	int numMatches;
	int currentMatchIndex;
	MatchPhoto *currentPhoto;
	int matchPhotoViews[MAX_PHOTOS];
	int matchHTML[MAX_PHOTOS];
	Sound *matchSFX;
	Sound *scanSFX;
	MixMatch();
	~MixMatch();
	void init();
	void drawPhoto(int photoNumber);
	void saveState();
	void dispose();
};

typedef struct
{
	int view;
	int loop;
	int cel;
}PARTDATA;

class MatchInfo : public Object
{
public:
	int savePhotoView;
	int savePhotoLoop;
	int savePhotoCel;

	int saveEyeColor;
	int saveHairColor;
	int saveGender;
	int saveSkinColor;
	PARTDATA saveMouth;
	PARTDATA saveEyes;
	PARTDATA saveNose;
	PARTDATA saveFace;
	PARTDATA saveHair;
	PARTDATA saveIrises;
	PARTDATA saveBeard;
	int numberMatches;
	int saveMatchViews[4];
	int saveMatchDossier[4];
	int photoIndex;
	bool savePeopleFound[MAX_PHOTOS];

	MatchInfo();
	~MatchInfo();
	void load();
	void save();
	void dispose();
};

class KatMatch : public MixMatch
{
public:
	int ZoomView[4];

	KatMatch(bool);
	KatMatch(int view, int cel);
};

class AmesMatch : public MixMatch
{
public:
	AmesMatch();
	AmesMatch(int photoNumber);
};

class KneeCapsMatch : public MixMatch
{
public:
	KneeCapsMatch();
	KneeCapsMatch(int photoNumber);
};

class GrendelMatch : public MixMatch
{
public:
	GrendelMatch();
	GrendelMatch(int photoNumber);
};

/***********************************
 *
 *			 ID Window
 *
 **********************************/
class IdTitleBar : public IntrPuzzFeature
{
public:
	IdTitleBar();
	int handleEvent(MADEEventStamp *);
};

class IdWinClose : public Button
{
public:
	IdWinClose();
	virtual void activate();
};

class IdWinOpen : public Button
{
public:
	IdWinOpen();
	virtual void activate();
};

class IdWinPrev : public Button
{
public:
	IdWinPrev();
	virtual void activate();
};

class IdWinNext : public Button
{
public:
	IdWinNext();
	virtual void activate();
};

class IdWinInfo : public Button
{
public:
	IdWinInfo();
	virtual void activate();
};

class MixStartScript : public Script
{
public:
	MixStartScript() {
		name = "mixstartscript";
	};
	void changeState(int);
};

class maximizeScript : public Script
{
public:
	SpriteSharedPtr tempsprite;
	SpriteSharedPtr newsprite;
	int begin_x;
	int begin_y;
	int dest_x;
	int dest_y;
	int move_x;
	int move_y;
	bool search;

	maximizeScript(int beginx, int beginy, int endX, int endY, bool doSearch);
	void changeState(int);
};

class IdDragBox : public IntrPuzzView
{
public:
	IdDragBox();
};

class MixMatchWin : public IntrPuzzFeature
{
public:
	MixMatchWin();
	int handleEvent(MADEEventStamp *);
};

class WindowText : public IntrPuzzView
{
public:
	WindowText();
};

class IdAnimScript : public Script
{
public:
	IdAnimScript() {
		name = "IdAnimScript";
	};
	void changeState(int);
};

class IdAnim : public IntrPuzzProp
{
public:
	int loopCtr;

	IdAnim(int theX, int theY);
	IdAnim(int Dos);
	~IdAnim();
	void cue();
};

class IdWindow : public IntrPuzzView
{
public:
	int idWin_x;
	int idWin_y;
	IdWinOpen *idwinopen;
	IdWinClose *idwinclose;
	IdTitleBar *idtitlebar;
	IdWinInfo *idwininfo;
	IdWinPrev *idwinprev;
	IdWinNext *idwinnext;

	IdAnim *idanim;
	MatchPhoto *matchphoto;
	WindowText *windowtext;
	SpriteSharedPtr minimize;
	bool closed;

	IdWindow(int x, int y);
	~IdWindow();
	int handleEvent(MADEEventStamp *);
	void moveTo(int x, int y);
	void searchData();
};

} // namespace Spycraft

#endif
