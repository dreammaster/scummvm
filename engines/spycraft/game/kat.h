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

#ifndef SPYCRAFT_GAME_KAT_H
#define SPYCRAFT_GAME_KAT_H

#include "spycraft/game/interface_puzzle.h"
#include "spycraft/game/sound.h"

namespace Spycraft {

#define LINESTART		1
#define LINEEND			2

#define ZOOMIN			1
#define ZOOMOUT			2

typedef struct
{
	int beg_left;
	int beg_top;
	int beg_right;
	int beg_bottom;
	int end_left;
	int end_top;
	int end_right;
	int end_bottom;
} SNAPRECT;

typedef struct
{
	bool lineDrawn;
	int beg_x;
	int beg_y;
	int end_x;
	int end_y;
} LINEDATA;

struct _pointxy
{
	int x;
	int y;
};

class ModelHilight : public IntrPuzzView
{
public:
	ModelHilight();
	void setHilight();
};

class KatBack : public IntrPuzzView
{
public:
	KatBack();
};

class TitleBar : public IntrPuzzFeature
{
public:
	TitleBar();
	int handleEvent(MADEEventStamp *);
};

class KatWindow : public IntrPuzzFeature
{
public:
	KatWindow();
	int handleEvent(MADEEventStamp *);
	void drawBox();
};

class DragBox : public IntrPuzzView
{
public:
	DragBox();
};

class Camera : public IntrPuzzView
{
public:
	Camera();
};

class CameraView : public IntrPuzzView
{
public:
	CameraView(int theView);
};

class Model3DAnim : public IntrPuzzView
{
public:
	Model3DAnim();
};

class Model3D : public IntrPuzzView
{
public:
	bool buttonDown;
	bool lineDrawn;

	int line;
	CommandLine *modelCL;

	Model3D(int theView, int theLoop);
	void init(int theX, int theY);
	void clear();
	int pointInBounds(int x, int y);
	struct _pointxy findLineXY(int x, int y, int bounds);
	void changeZoom(bool lineDrawn, int zoom);
	int handleEvent(MADEEventStamp *);
	int moveLine(int zoomPos, int direction);
	int clipModelLine(int zoomPos);
	bool drawSavedLine();
	void transformLine(_pointxy XY, int direction);
};

class KatButton : public Button
{
public:
	bool disabled;

	void disable();
	void enable();
};

class CameraLeft : public Button
{
public:

	CameraLeft();
	void activate();
};

class CameraRight : public Button
{
public:

	CameraRight();
	void activate();
};


class WinZoomIn : public Button
{
public:

	WinZoomIn();
	void activate();
};

class WinZoomOut : public Button
{
public:

	WinZoomOut();
	void activate();
};

class ZoomIn3D : public KatButton
{
public:

	ZoomIn3D();
	void activate();
};

class ZoomOut3D : public KatButton
{
public:

	ZoomOut3D();
	void activate();
};

class MixBut : public Button
{
public:
	MixBut();
	void activate();
};

class WindowClose : public Button
{
public:
	WindowClose();
	void activate();
};

class WindowOpen : public Button
{
public:
	WindowOpen();
	void activate();
};

class MagGlass : public IntrPuzzView
{
public:
	MagGlass(int theX, int theY);
};

class CameraWindow : public IntrPuzzView
{
public:
	WinZoomIn *zoomin;
	WinZoomOut *zoomout;
	MixBut *mix;
	MagGlass *magglass;
	CameraView *cameraview;
	WindowClose *windowclose;
	TitleBar *titlebar;
	Sprite *minimize;

	int currentZoom;
	int cameraWin_x;
	int cameraWin_y;
	bool closed;

	CameraWindow(int x, int y, int currZoom);
	~CameraWindow();
	int handleEvent(MADEEventStamp *);
	void moveTo(int x, int y);
};

class MagFlip : public Script
{
public:
	MagGlass *theGlass;
	int zoomLevel;

	MagFlip(MagGlass *glass, int magnification);
	void changeState(int);
};

class Convert3D : public Script
{
public:
	Convert3D() {
		name = "Convert3D";
	};
	void changeState(int);
};

class explodeScript : public Script
{
public:
	Sprite *tempsprite;
	Sprite *newsprite;
	int begin_x;
	int begin_y;
	int dest_x;
	int dest_y;
	int move_x;
	int move_y;

	explodeScript(int beginx, int beginy, int endX, int endY);
	void changeState(int);
};

class KATInfo : public Object
{
public:
	int lineBeg_x, lineBeg_y, lineEnd_x, lineEnd_y;
	int camWinX;
	int camWinY;
	int ModelZoom;
	int cameraZoom;
	int camPosition;
	bool lineDrawn;
	LINEDATA saveLineData[4];
	bool seenHTM;

	KATInfo();
	void load();
	void save();
	void dispose();
};

class CrossHairCursor : public MadeCursor
{
public:
	CrossHairCursor();
};

class KAT : public IntrPuzzPage
{
public:
	Camera *camera;
	int cameraPosition;
	int CameraZoomView[4];
	Model3D *model3d;
	Model3DAnim *model3danim;
	bool windowDrag;
	int box_originX;
	int box_originY;
	LINEDATA lineData[4];
	int current3dzoom;
	ModelHilight *hilight;
	Sound *katSFX;
	Sound *cameraSFX;
	Sound *modelSFX;
	Sound *zoomSFX;
	Sound *katBetty;

	KAT();
	void cue();
	void saveState();
	void clearLineData();
	void setLineData(int zoomLevel, int beg_x, int beg_y, int end_x, int end_y);
	void copyLineToGlobal(int zoomLevel);
	void dispose();
	int isTheJackassClose(SNAPRECT theRect);
};

extern KATInfo *katinfo;

} // namespace Spycraft

#endif
