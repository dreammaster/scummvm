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

#ifndef SPYCRAFT_GAME_PHODOC_H
#define SPYCRAFT_GAME_PHODOC_H
   
#include "spycraft/game/object.h"
#include "spycraft/game/interface_puzzle.h"
#include "spycraft/game/invinset.h"

namespace Spycraft {

class TargetObject : public IntrPuzzView
{
public:
	int theScaleX;
	int theScaleY;
	float ratioX;
	float ratioY;
	bool drag;
	bool hi_Lit;
	int fromLeft;
	int fromTop;

	void init(int x, int y, int view, int loop, int cel);
	int handleEvent(MADEEventStamp *);
	void dispose();
};

class PrintStatusBar : public IntrPuzzProp
{
public:
	IntrPuzzView *printbox;

	PrintStatusBar();
	void cue(void);
};

class DoctorWin : public IntrPuzzFeature
{
public:
	DoctorWin();
	int handleEvent(MADEEventStamp *);
};

class PhotoObject : public IntrPuzzFeature
{
public:
	//	int hi_Lit;
	int theView;
	int theLoop;
	int theCel;

	PhotoObject(int x, int y, int width, int height, int view, int loop, int cel);
	int handleEvent(MADEEventStamp *);
};

class Bar : public IntrPuzzView
{
public:
	Bar();
};

class BoxCorner : public IntrPuzzView
{
public:

	BoxCorner(int x, int y);
	int handleEvent(MADEEventStamp *);
};

class BackGrnd : public IntrPuzzView
{
public:
	BackGrnd();
};

class ObjectCursor : public MadeCursor
{
public:
	int cursorWidth;
	int cursorHeight;

	ObjectCursor();
	void init(int view, int loop, int cel, int width, int height);
};

class Photo : public IntrPuzzView
{
public:
	void init(int x, int y, int view, int loop);
	int handleEvent(MADEEventStamp *);
};

class VLine : public IntrPuzzView
{
public:
	float ratioY;

	VLine(int theX, int theY);
};

class HLine : public IntrPuzzView
{
public:
	float ratioX;

	HLine(int theX, int theY);
};

class TargetPhoto : public IntrPuzzView
{
public:
	int boxline1, boxline2, boxline3, boxline4;
	int boxwidth;
	int boxheight;
	VLine *vlineLeft;
	VLine *vlineRight;
	HLine *hlineTop;
	HLine *hlineBottom;
	bool objectSelected;

	TargetPhoto();
	int handleEvent(MADEEventStamp *);
	void eraseBox();
	void drawBox(int x, int y, int widh, int height);
};

class PhotoTab : public IntrPuzzFeature
{
public:
	IntrPuzzView *objectTab;
};

class NewsButt : public PhotoTab
{
public:
	NewsButt();
	int handleEvent(MADEEventStamp *);
};

class CigsButt : public PhotoTab
{
public:
	CigsButt();
	int handleEvent(MADEEventStamp *);
};

class FaceButt : public PhotoTab
{
public:
	FaceButt();
	int handleEvent(MADEEventStamp *);
};

class UndoBut : public Button
{
public:
	UndoBut();
	void activate();
};

class ResetBut : public Button
{
public:
	ResetBut();
	void activate();
};

class PrintBut : public Button
{
public:

	PrintBut();
	void activate();
	int inBounds(int theLeft, int theTop, int theRight, int theBottom);
	int outOfBounds(int theLeft, int theTop, int theRight, int theBottom);
};

class ScaleBut : public Button
{
public:
	ScaleBut();
	void activate();
};

class PhotoDoc : public IntrPuzzPage
{
public:
	List *TargetObjects;
	bool cursorHidden;
	PhotoObject *currentObject;
	TargetPhoto *targetphoto;
	TargetObject *currentTarget;

	PhotoDoc();
	void dispose();
	void saveState();
};

struct _saveobjects
{
	int theX;
	int theY;
	int theView;
	int theLoop;
	int theCel;
	int theScaleX;
	int theScaleY;
};

class PhotoDocInfo : public Object
{
public:
	struct _saveobjects saveObjects[30];
	int numTargetObjects;

	PhotoDocInfo();
	void save();
	void load();
	void dispose();
};

class InsetObject : public View
{
public:
	int theScaleX;
	int theScaleY;
	float ratioX;
	float ratioY;

	void init(int x, int y, int view, int loop, int cel);
};

class PhotoDocInset : public InvInset
{
public:
	PhotoDocInset();
	void dispose();
};

class PrintedPhoto : public Object
{
public:
	struct _saveobjects printObjects[30];
	int numTargetObjects;
	List *insetObjects;

	PrintedPhoto();
	void printPhoto();
	void showPhoto();
};
extern PrintedPhoto *printedPhoto;

} // namespace Spycraft

#endif
