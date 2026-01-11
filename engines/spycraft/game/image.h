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

#ifndef SPYCRAFT_GAME_IMAGE_H
#define SPYCRAFT_GAME_IMAGE_H

#include "spycraft/game/interface_puzzle.h"
#include "spycraft/game/sound.h"			   
#include "spycraft/game/web.h"

namespace Spycraft {

typedef struct
{
	int quad;
	int left;
	int top;
	int right;
	int bottom;
	int view;
	int loop;
	int x;
	int y;
	int clipped;
	//	char Text[25];
} TextObj;

enum {
	CENTER,
	TOPLEFT,
	TOPRIGHT,
	BOTTOMLEFT,
	BOTTOMRIGHT,
	MIDDLE
};

#define NOSCALE				0
#define HALFSCALE			1

#define IMAGE_WIDTH			799
#define IMAGE_HEIGHT		563

#define WINDOW_WIDTH		399		
#define WINDOW_HEIGHT		280			

#define ORIGIN_X				16
#define ORIGIN_Y				89		

#define NO_ZOOM				0
#define ZOOM_IN				1
#define ZOOM_OUT			2

#define MAX_ZOOMS			4
#define MAX_PICS			3
#define MAX_SAVE_PICS		4

class ImageCursor : public MadeCursor
{
public:
	ImageCursor();
	~ImageCursor();
};

class ScanLineA : public IntrPuzzView
{
public:
	TextObj textFound[2];
	bool clipped;
	int currentItem;
	int photoHeight;
	int scanLineCtr;
	IntrPuzzTimer *scanTimer;

	ScanLineA();
	void cue();
};

class ScanLineDos8 : public IntrPuzzView //IntrPuzzActor
{
public:
	TextObj textFound[2];
	bool clipped;
	int currentItem;
	int photoHeight;
	int scanLineCtr;
	int clippingPlate;
	int plateHeight;
	int currentPlateScanLine;

	SpriteSharedPtr clipPlate;
	IntrPuzzTimer *scanTimer;

	ScanLineDos8();
	~ScanLineDos8();
	void cue();
};

class ImageInfo : public Object
{
public:
	int numPictures;
	bool gotBlend;
	int theView[MAX_SAVE_PICS];
	int theLoop[MAX_SAVE_PICS];
	int picNum[MAX_SAVE_PICS];
	int theX[MAX_SAVE_PICS];
	int theY[MAX_SAVE_PICS];
	int zoomPos[MAX_SAVE_PICS];
	SRect theZooms[MAX_SAVE_PICS][MAX_ZOOMS];
	int zoomQuad[MAX_SAVE_PICS];
	int numtextobjs;
	TextObj textobjs[7];
	int picZoomViews[MAX_SAVE_PICS][6];
	//	int saveBlendViews[MAX_PICS][6];
	int currentPhoto;

	ImageInfo();
	void save();
	void load();
	void dispose();
};

class ImageEnhance : public Button
{
public:
	ImageEnhance();
	virtual void activate();
};

class BlendBut : public Button
{
public:
	BlendBut();
	virtual void activate();
};

class CapsSendBut : public WebReport
{
public:
	CapsSendBut();
	virtual void activate();
	void cue();
};

class TankSendBut : public WebReport
{
public:
	TankSendBut();
	virtual void activate();
	void cue();
};

class PlateSendBut : public WebReport
{
public:
	PlateSendBut();
	void setFlags();
	virtual void activate();
	void cue();
};

class PegSendBut : public WebReport
{
public:
	PegSendBut();
	virtual void activate();
	void cue();
};

class AmesMixMatch : public Button
{
public:
	AmesMixMatch();
	virtual void activate();
};

class CapsMixMatch : public Button
{
public:
	CapsMixMatch();
	virtual void activate();
};

class ZoomBut : public Button
{
public:
	ZoomBut();
	virtual void activate();
	//	int handleEvent ( MADEEventStamp * );
};

class unZoomBut : public Button
{
public:
	unZoomBut();
	virtual void activate();
};

class ImageBack : public IntrPuzzView
{
public:
	ImageBack();
};

/*class EnhanceText : public Print
{
public:

	EnhanceText(int theX, int theY, char *);
};
*/

class Picture : public Object
{
public:
	SpriteSharedPtr scaleSprite;
	IntrPuzzView *photoSprite;

	//	EnhanceText *textSprites[5];
	IntrPuzzView *textSprites[2];

	int numTextSprites;

	int numTextObjs;
	TextObj textObjList[10];

	SRect zoomList[MAX_ZOOMS];
	int zoomViews[6];

	int currentZoom;
	int currentQuad;

	Picture();
	~Picture();
	void dispose();
	void init(int view, int loop, int cel, bool scaled);
	void setATSQuads(int startPhoto, int quadTopLeft, int quadTopRight, int quadBottLeft, int quadBottRight, int middle);
	void initTextObjs(TextObj textObj[], int numObjs);
};

class PictureButton : public Button
{
public:
	int PicNum;
	Picture *picture;

	void init(int theX, int theY, int theView, int theLoop, int thePictureLoop);
	virtual void activate();
};

//Dialog box for the blend
class BlendokButton : public DialogButton
{
public:
	BlendokButton();
	void activate();
};

class cancelButton : public DialogButton
{
public:
	cancelButton();
	void activate();
};

class CheckButton : public DialogButton
{
public:
	int picNum;

	CheckButton(int x, int y, int picnum);
	int handleEvent(MADEEventStamp *);
};

class DialogPic : public DialogView
{
public:
	DialogPic(int x, int y, int view, int loop, int cel, int picnum);

};

class ZoomArea : public IntrPuzzFeature
{
public:

	ZoomArea();
	int handleEvent(MADEEventStamp *);
};

class BlendBox : public DlgBox
{
public:
	int numchecked;
	bool checked[4];

	BlendBox();
};

class ImageVoice : public Sound
{
public:
	void playStream(int soundNumber);
	void play(int soundNumber);
	void play(int soundNumber, Object *whoToCue);
};

class ImageSFX : public Sound
{
public:
	void playStream(int soundNumber);
	void play(int soundNumber);
};

class ImageAnal : public IntrPuzzPage
{
public:
	bool linkmode;
	int zoomMode;
	ImageVoice *imageVoice;
	ImageSFX *imageSFX;

	ImageAnal();
	void setBlendViews(int blendNum, int startView, int quadTopLeft, int quadTopRight, int quadBottLeft, int quadBottRight, int middle);
	void saveState();
	void init();
	void dispose();
};

class ImageAnalPlate : public ImageAnal
{
public:
	ImageAnalPlate();
};

class ImageAnalTank : public ImageAnal
{
public:
	ImageAnalTank();
};

class ImageAnalCourier : public ImageAnal
{
public:
	ImageAnalCourier();
};

class ImageAnalPeg : public ImageAnal
{
public:
	ImageAnalPeg();
};

class ImageAnalCaps : public ImageAnal
{
public:
	ImageAnalCaps();
};

class ImageStartScript : public Script
{
public:
	ImageStartScript() {
		name = "ImageStartScript";
	};
	void changeState(int);
};

class ImageTankScript : public Script
{
public:
	ImageTankScript() {
		name = "ImageTankScript";
	};
	void changeState(int);
};

extern ImageInfo *imageinfo;  //extern declared for mix and match 

} // namespace Spycraft

#endif
