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

#include "spycraft/game/kat.h"
#include "spycraft/game/mixmatch.h"
#include "spycraft/game/flag.h"
#include "spycraft/game/vlink.h"
#include "spycraft/game/pcscreen.h"
#include "spycraft/logic/1000.h"

namespace Spycraft {

KAT *kat;
KATInfo *katinfo;
	   
extern BlinkCursor* normalCursor;
CrossHairCursor* crosshair;

CameraWindow	*camWin;
DragBox			*dragbox;
Sprite			*explode;
MagGlass		*modelMagGlass; 

WebReport		*katreport;

bool gotLine;
int beg_x;
int beg_y;
int end_x;
int end_y;

struct _pointxy zoomx4[13] = {{-38, 68}, {12, 67}, {36, 56}, {43, 38},{48, -6}, {90, 0},
							 {126, 6}, {161, 7}, {194, 8}, {234, 13}, {278, 18}, {335, 22},
							 {373, 26}};

struct _pointxy zoomx16[13] = {{122, 70}, {125, 69}, {128, 73}, {127, 69}, {126,70},
							{127, 70}, {127, 70}, {128, 70}, {129, 71}, {125, 69}, {127, 72},
							{126, 71}, {126, 69}};


struct _pointxy zoomx64 = {127, 72};

struct _pointxy hilightXY[200] = {{1, 222}, {81, 107}, {2, 63}, {2, 63},
								{13, 236}, {106, 140}, {2, 91}, {2, 63},
								{52, 259}, {180, 181}, {113, 160}, {2, 122},
								{72, 266}, {218, 194}, {192, 187}, {125, 168},
								{27, 229}, {30, 122}, {2, 63}, {2, 63},
								{126, 261}, {227, 183}, {202, 158}, {145, 122},
								{137, 266}, {249, 194}, {246, 193}, {232, 184},
								{139, 269}, {253, 199}, {250, 205}, {243, 208},
								{128, 208}, {184, 102}, {116, 63}, {2, 63},
								{147, 241}, {225, 167}, {189, 124}/**/, {122, 63},
								{151, 247}, {232, 182}, {213, 148}, {171, 96},
								/*{---},*/  {240, 184}, {223, 162}, {189, 120},
								{151, 166}, {213, 63}, {171, 63}, {86, 63},
								{164, 220}, {239, 166}, {221, 132}, {187, 68},
								{168, 234}, {248, 190}, {240, 179}, {225, 155},
								/*{---},*/ {252, 198}, {248, 196}, {241, 186},
								{160, 158}, {222, 126}, {191, 63}, {126, 63},
								{160, 185}, {223, 179}, {191, 155}, {125, 105},
								{161, 187}, {223, 182}, {192, 162}, { 130, 123},
								{174, 196}, {249, 200},{244, 195}, {232, 188},
								{170, 158}, {158, 115}, {59, 63}, {2, 63},
								{201, 189}, {221, 176}, {187, 147}, {115, 91},
								{211, 198}, {245, 195}, {234, 185}, {211, 166},
								{217, 201}, {252, 201}, {246, 199}, {236, 194},
								{215, 177}, {174, 139}, {92, 75}, {2, 63},
								{239, 193}, {226, 172}, {197, 142}, {135, 80},
								{249, 205}, {245, 194}, {234, 183}, {213, 165},
								{253, 208}, {252, 200}, {247, 196}, {236, 191},
								{251, 177}, {180, 137}, {104, 73}, {2, 63},
								{269, 198}, {216, 179}, {176, 155}, {121, 106},
								{284, 205}, {244, 193}, {232, 182}, {208, 161},
								{288, 209}, {251, 199}, {247, 194}, {236, 190},
								{260, 185}, {143, 151}, {68, 98}, {2, 63},
								{293, 200}, {198, 183}, {142, 160}, {94, 119},
								{317, 206}, {246, 196}, {232, 188}, {208, 173},
								{321, 210}, {252, 202}, {246, 200}, {235, 196},
								{270, 190}, {92, 151}, {18, 101}, {2, 63},
								{321, 204}, {172, 181}, {98, 161}, {57, 116},
								{356, 211}, {241, 194}, {230, 184}, {204, 168},
								{361, 214}, {248, 200}, {244, 198}, {230, 194},
								{281, 191}, {29, 150}, {2, 93}, {2, 63},
								{351, 211}, {144, 185}, {38, 164}, {7, 127},
								{400, 217}, {238, 198}, {223, 187}, {189, 173},
								{405, 220}, {245, 202}, {238, 194}, {221, 188},
								{284, 199}, {2, 152}, {2, 102}, {2, 63},
								{378, 215}, {89, 183}, {2, 161}, {2, 117},
								{455, 221}, {238, 198}, {221, 192}, {188, 181},
								{460, 224}, {245, 201}, {241, 204}, {228, 205},
								{292, 206}, {2, 156}, {2, 110},
								{397, 243}, {48, 232}, {2, 261}};

SNAPRECT snapRects[8] = {{/*beg rect*/237, 211, 270, 255,/*end rect*/499, 178, 530, 245}, //pos 1 zoom3
					     {/*beg rect*/26, 284, 71, 319, /*end rect*/492, 204, 530, 272}, //pos 2 zoom 3
					     {/*beg rect*/248, 206, 262, 220, /*end rect*/503, 187, 520, 214}, //pos 1 zoom 2
					     {/*beg rect*/143, 244, 162, 252, /*end rect*/494, 196, 520, 242}}; //pos 2 zoom 2

KAT::KAT()
{
	user->input = false;
	pcLastUsed[PC_KAT] = true;
	GameFlag.set(fGreg10); //used for the report dialog box
		
	kat = this;
	katSFX = new(Sound);
 	cameraSFX = new(Sound);
	modelSFX = new(Sound);
	zoomSFX = new(Sound);
	katBetty = new(Sound);

	IntrPuzzPage::init(WEB_KAT);
	helppage = 92100;

	cameraPosition = 6; 
	current3dzoom = 0;
	crosshair = NULL;
	CameraZoomView[0] = 243;
	CameraZoomView[1] = 244;
	CameraZoomView[2] = 245;
	CameraZoomView[3] = 246;
	clearLineData();
	gotLine = false;
	windowDrag = false;
	new(KatBack);					  

	if (theMusic->isPlaying())
		theMusic->stop();
	theMusic->setVolume(25);
	theMusic->play(200);

	if(!intrpuzz->isStateSaved("katinfo"))
	{
		user->input = false;
		new(CameraLeft);
		new(CameraRight);
		new(ZoomIn3D);
		new(ZoomOut3D);
		modelMagGlass = new MagGlass(446, 355);
//		if(!GameFlag.test( fSetVL_Warhurst632 ) /*&& !GameFlag.test(fBeenToMoscow)*/)
//		{
//			new VLink( "632.avi", 0, 0 );  //Warhurst: good, analyze video (Pz)
//			GameFlag.set( fSetVL_Warhurst632 );
 //		}

		setScript(new Convert3D);
	}
	else
	{							    
		katinfo->load();
	}			

	webreport->hide();
	katreport = new(KatSendBut);

	user->input = true;
}

void KAT :: clearLineData()
{
	for(int i = 0; i < 4; i++)
	{
		lineData[i].lineDrawn = false;
	}	 	
}

void KAT :: setLineData(int zoomLevel, int start_x, int start_y, int finish_x, int finish_y)
{
	lineData[zoomLevel].lineDrawn = true;
	lineData[zoomLevel].beg_x = start_x;
	lineData[zoomLevel].beg_y = start_y;
	lineData[zoomLevel].end_x = finish_x;
	lineData[zoomLevel].end_y = finish_y;
}
	
void KAT :: copyLineToGlobal(int zoomLevel)
{
	beg_x = lineData[zoomLevel].beg_x;
	beg_y = lineData[zoomLevel].beg_y;
	end_x = lineData[zoomLevel].end_x;
	end_y = lineData[zoomLevel].end_y;
}
	
int KAT::isTheJackassClose(SNAPRECT r)
{
	if((beg_x > r.beg_left) && (beg_x < r.beg_right))
	{
		if((beg_y > r.beg_top) && (beg_y < r.beg_bottom))
		{
			if((end_x > r.end_left) && (end_x < r.end_right))
			{
				if((end_y > r.end_top) && (end_y < r.end_bottom))
				{
					return true;
				}	
			}
		}		 				
	}			
	return false;
}

void KAT::saveState()
{
		if(!intrpuzz->isStateSaved("katinfo"))
		{
			new(KATInfo);
//			katinfo->dispose();
		}		
		katinfo->save();
}

void KAT::dispose()
{
	katSFX->dispose();
	cameraSFX->dispose();
	modelSFX->dispose();
	zoomSFX->dispose();
	katBetty->dispose();

	if(web_intrface)
	{
		webreport->show();
		katreport->dispose();
	}
	theMusic->stop();
	if(intrpuzz->musicNum)
		theMusic->play(intrpuzz->musicNum);

	IntrPuzzPage::dispose();
}

void KAT::cue()
{
	sfxMadeCursor();	//this is for the opening betty file
	user->input = true;
}
	
/***********************************
 *		
 *		 Puzzle Save Status
 *		      
 **********************************/
KATInfo::KATInfo()
{
	name = "katinfo";
	web_intrface->addToFront(this); 
	katinfo = this;
 	seenHTM = false;
}

void KATInfo::save()
{
	camPosition = kat->cameraPosition;
	cameraZoom = camWin->currentZoom;
	camWinX = camWin->cameraWin_x;
	camWinY = camWin->cameraWin_y;

	for(int i = 0; i < 4; i++)
	{
		saveLineData[i].lineDrawn = kat->lineData[i].lineDrawn;
		saveLineData[i].beg_x = kat->lineData[i].beg_x;
		saveLineData[i].beg_y = kat->lineData[i].beg_y;
		saveLineData[i].end_x = kat->lineData[i].end_x;
		saveLineData[i].end_y = kat->lineData[i].end_y;
	}

	if(kat->model3d->lineDrawn)
	{
		lineDrawn = true;
		lineBeg_x = beg_x;
		lineBeg_y = beg_y;
		lineEnd_x = end_x;
		lineEnd_y = end_y;
	}
	else
	{
		lineDrawn = false;
	}				

	ModelZoom = kat->current3dzoom;

}	  	  
	 
void KATInfo::load()
{
	new(ZoomIn3D);
	new(ZoomOut3D);
	new(CameraLeft);
	new(CameraRight);

	kat->current3dzoom = ModelZoom;
	kat->cameraPosition = camPosition;
	kat->camera = new(Camera);
	kat->camera->setCel(kat->cameraPosition);

	int modelView;
	switch(ModelZoom)
	{
		case 0:
			kat->model3d = new Model3D(798, 0);
			kat->model3d->posn(2, 133);
			break;
		 	 
		case 1:
/*			if(kat->cameraPosition < 6)
			{
				kat->model3d = new Model3D(202, 0);
			}
			else
			{
*/			
			
			modelView = 202 + kat->cameraPosition;
			kat->model3d = new Model3D(modelView, 0);
//			}														
			kat->model3d->posn(2, 63);
			break;

		case 2:
			modelView = 215 + kat->cameraPosition;
			kat->model3d = new Model3D(modelView, 0);
			kat->model3d->posn(2, 63);
			break;
							
		case 3:
			modelView = 230 + kat->cameraPosition;
			kat->model3d = new Model3D(modelView, 0);
			kat->model3d->posn(2, 63);
			break;
			
	}

	new CameraWindow(camWinX, camWinY, cameraZoom);
	camWin->cameraview->setCel(kat->cameraPosition);
	camWin->magglass->setCel(cameraZoom);
	new(KatWindow);

	if(lineDrawn)
	{
		for(int i = 0; i < 4; i++)
		{
			kat->lineData[i].lineDrawn = saveLineData[i].lineDrawn;
			kat->lineData[i].beg_x = saveLineData[i].beg_x;
			kat->lineData[i].beg_y = saveLineData[i].beg_y;
			kat->lineData[i].end_x = saveLineData[i].end_x;
			kat->lineData[i].end_y = saveLineData[i].end_y;
		}

		kat->model3d->lineDrawn = true;
		beg_x = lineBeg_x;
		beg_y = lineBeg_y;
		end_x = lineEnd_x;
		end_y = lineEnd_y;

		if(kat->lineData[ModelZoom].lineDrawn)
			kat->model3d->line = sfxDrawLine(kat->model3d->mySprite, kat->lineData[ModelZoom].beg_x, kat->lineData[ModelZoom].beg_y, kat->lineData[ModelZoom].end_x,
													 kat->lineData[ModelZoom].end_y, MK_COLOR16(255, 0, 0));
		
		if(kat->model3d->line == -1)
			ASSERT(0, 0);
		gotLine = true;
	}
	else
	{
		kat->model3d->lineDrawn = false;
	}				
	kat->model3d->buttonDown = false;
	kat->windowDrag = false;
	kat->hilight = new(ModelHilight);	
	kat->hilight->setHilight();
	modelMagGlass = new MagGlass(446, 355);
	modelMagGlass->setCel(kat->current3dzoom);
}

void KATInfo :: dispose()
{
	web_intrface->release ( this );
	deathList->add(this);
}

/***********************************
 *		
 *			 Cursors
 *		      
 **********************************/
CrossHairCursor::CrossHairCursor()
{
	name = "CrossHairCursor";
	view = 200;
	loop = 11;
	cel = 0;
	orig_pt.x = mouse_x;
	orig_pt.y = mouse_y;
	theGame->setCursor(this);
}

/***********************************
 *		
 *			 Features
 *		      
 **********************************/
KatWindow::KatWindow()
{
	init(0, 60, 512, 320);
}
	
int KatWindow::handleEvent ( MADEEventStamp *event )
{
	if (user->input)
	{
	  	if (kat->windowDrag)
		{
			int cursor_x;
			int cursor_y;
			int objectLeft = mouse_x - kat->box_originX;
			int objectTop = mouse_y - kat->box_originY;
			int objectRight	= objectLeft + dragbox->width;
			int objectBottom = objectTop + dragbox->height;
			bool ySet = true;
			bool xSet = true;
			
			if(objectLeft < left)
			{
				cursor_x = left + kat->box_originX;
				objectLeft = left;
			}		
			else if(objectRight > right)
			{
				cursor_x = (right - dragbox->width) + kat->box_originX;
				objectLeft = right - dragbox->width;
			}
			else
			{
				xSet = false;
				cursor_x = mouse_x;
			}

		 	if(objectTop < top)
			{
				cursor_y = top + kat->box_originY;
				objectTop = top;
			}
			else if(objectBottom > bottom)
			{
				cursor_y = (bottom - dragbox->height) + kat->box_originY;
				objectTop = bottom - dragbox->height;
			}	
			else
			{
				ySet = false;
				cursor_y = mouse_y;
			}
		
			if(ySet || xSet)
				sfxSetCursorPos(cursor_x, cursor_y);

			dragbox->posn(objectLeft, objectTop);
			if(event_type == USER_LEFT_UP || !onMe(event))
			{
				kat->windowDrag = false;
				camWin->moveTo(objectLeft, objectTop);	
				dragbox->dispose();
			}
			return true;
		}
	}
	return false;
}

TitleBar::TitleBar()
{
	init(camWin->cameraWin_x + 3, camWin->cameraWin_y + 4, 144, 24);
}
	
int TitleBar::handleEvent ( MADEEventStamp *event )
{
	if (user->input && onMe(event))
	{
	  	if (event_type == USER_LEFT_DOWN)
		{
			kat->windowDrag = true;
			kat->box_originX = mouse_x - camWin->cameraWin_x;
			kat->box_originY = mouse_y - camWin->cameraWin_y;
			dragbox = new(DragBox);
//			dispose();
			return true;
		}
	}
	return false;	
}
/***********************************
 *		
 *			 Views
 *		      
 **********************************/
KatBack::KatBack()
{
	name = "katBack";
	view = 799;
	loop = 0;
	cel = 0;
	init(0, 16);
	noSkip();
//	setPri(1);
	addToPic();
	dispose();
}

MagGlass::MagGlass(int theX, int theY)
{
	name = "MagGlass";
	view = 200;
	loop = 8;
	cel = 0;
	init(theX, theY);
	setPri(512);
}

ModelHilight::ModelHilight()
{
	name = "ModelHilight";
	view = 1500;
	loop = 0;
	cel = 0;
	init(camWin->cameraWin_x, camWin->cameraWin_y);
	setPri(450);
}
			  
void ModelHilight::setHilight()
{
	int hilightView;

	hilightView = 1500 + (kat->cameraPosition * 16) + (camWin->currentZoom * 4) + kat->current3dzoom;	
	if(hilightView == 1544 || hilightView == 1560 || hilightView > 1698 || hilightView == 1695)
	{
		hide();
		return;
	}

	if(hilightView > 1695)
	{
		hilightView -= 3;
	}
	else if(hilightView > 1560)
	{
		hilightView -= 2;
	}
	else if(hilightView > 1544)
	{
		hilightView -= 1;
	}

	setView(hilightView);
	sfxSpriteGlass(mySprite,2,MK_COLOR16(0,0,0),40);
	hilightView -= 1500;
	posn(hilightXY[hilightView].x, hilightXY[hilightView].y);
	setPri(450);
//	sfxReleaseSprites ( backgrounds[curBack] );
//	sfxUpdate();
}

DragBox::DragBox()
{
	name = "dragBox";
	view = 200;
	loop = 10;
	cel = 0;
	init(camWin->cameraWin_x, camWin->cameraWin_y);
	setPri(550);
}

Camera::Camera()
{
	name = "Camera";
	view = 200;
	loop = 9;
	cel = kat->cameraPosition;
	init(363, 356);
	noSkip();
}

CameraView::CameraView(int theView)
{
	name = "cameraView";
	view = theView;
	loop = 0;
	cel = kat->cameraPosition;
	init(camWin->cameraWin_x + 4, camWin->cameraWin_y + 22);
	setPri(512);
	noSkip();
}

CameraWindow::CameraWindow(int theX, int theY, int currZoom)
{
	SRect theRect;

	name = "cameraWindow";
	camWin = this;
	cameraWin_x = theX;
	cameraWin_y = theY;
	view = 200;
	loop = 2;
	cel = 1;
	currentZoom = currZoom;
	closed = false;
	init(0, 0);

	zoomin = new(WinZoomIn);
	zoomout = new(WinZoomOut);
	mix = new(MixBut);
	cameraview = new CameraView(kat->CameraZoomView[currentZoom]);
	magglass = new MagGlass(cameraWin_x + 27, cameraWin_y + 143);
	titlebar = new(TitleBar);
	windowclose = new(WindowClose);
	setPri(500);

	SRect_Init ( &theRect, 3, 1, 166, 170);

	sfxSetDrawInfo (ADV_DRAW_SCALEX, 32);
	sfxSetDrawInfo (ADV_DRAW_SCALEY, 32);
	minimize = sfxClipSprite ( mySprite, 12, 343, &theRect );
	sfxHideSprite(minimize);
	posn(theX, theY);
	setCel(0);
}

CameraWindow::~CameraWindow()
{
	sfxKillSprite(minimize);
}

int CameraWindow::handleEvent(MADEEventStamp* event)
{
	if(onMe(event) && user->input && (!closed))
	{
		if(crosshair)   
		{
			delete crosshair;
			crosshair = NULL;
			theGame->setCursor(normalCursor);
			kat->model3d->modelCL->active = false;
			kat->model3d->modelCL->commandPrint->erase();
		}

		if(event_type == USER_LEFT_UP)
		{
			if(kat->model3d->buttonDown)
			{
				sfxEraseLine(kat->model3d->mySprite, kat->model3d->line);
			//	lineDrawn = false;   //11-1
				kat->model3d->buttonDown = false;
			}
		}	
		return true;
	}
	else
		return false;
}

void CameraWindow::moveTo(int theX, int theY)
{
	cameraWin_x = theX;
	cameraWin_y = theY;

	camWin->posn(theX, theY);
	zoomin->posn(cameraWin_x + 5, cameraWin_y + 143);
	zoomout->posn(cameraWin_x + 57, cameraWin_y + 143);
	mix->posn(cameraWin_x + 81, cameraWin_y + 143);
	magglass->posn(cameraWin_x + 27, cameraWin_y + 143);
	windowclose->posn(cameraWin_x + 145, cameraWin_y + 5);
	cameraview->posn(cameraWin_x + 4, cameraWin_y + 22);
	if(titlebar)
	{
		titlebar->dispose();  
	 	titlebar = NULL;
	}
	titlebar = new(TitleBar);
}

Model3DAnim::Model3DAnim()
{
	name = "modelanim";
	view = 201;
	loop = 0;
	cel = 0;
	init(30, 64);
	setPri(15);
}

Model3D::Model3D(int theView, int theLoop)
{
	name = "Model3D";
	view = theView;
	loop = theLoop;
	cel = 0;
	clear();
	init(2, 133);
	setPri(15);

	if(kat->current3dzoom == 0)
	{
		modelCL = new CommandLine(left, top, right - left, bottom - top, "Connect bullet holes; create trajectory.");
	}
	else
	{
		modelCL = new CommandLine(left, 63, right - left, bottom - top, "Connect bullet holes; create trajectory.");
	}
}
	
void Model3D::clear()
{
	buttonDown = false;
	lineDrawn = false;
	line = -1;
}

void Model3D :: init(int theX,int theY)
{
	x = theX;
	y = theY;
	draw();
	intrpuzz->add( this );
}

int Model3D::pointInBounds(int theX, int theY)
{
	if((theX > left) && (theX < right))
	{
		if((theY > top) && (theY < bottom))
		{
			return true;
		}		 				
	}			
	return false;
}

struct _pointxy Model3D::findLineXY(int theX, int theY, int bounds)
{
	struct _pointxy PointXY;
	int difference_y;
	int difference_x;
	int move_y;
	int move_x;
	int error_term;
	int line_x = theX;
	int line_y = theY;
	PointXY.x = theX;
	PointXY.y = theY;

	difference_x = end_x - beg_x;
	difference_y = end_y - beg_y;

	if(difference_x < 0)
	{
		difference_x = -difference_x;
		move_x = -1;
	}
	else
	{
		if(difference_x == 0)
		{	
			move_x = 0;
		}
		else
		{
			move_x = 1;
		}
	}
			
	if(difference_y < 0)
	{
		difference_y = -difference_y;
		move_y = -1;
	}
	else
	{
		if(difference_y == 0)
		{	
			move_y = 0;
		}
		else
		{
			move_y = 1;
		}
	}

	error_term = 0;
	if(difference_x > difference_y)
	{
		while(true)
		{
			line_x += move_x;
			error_term += difference_y;

			if((error_term > difference_x) || (difference_y == 0))
			{
				error_term -= difference_x;
				line_y += move_y;

				if(bounds == LINESTART)
				{
					if(line_x > 15000 || line_x < -15000) //(these constants are for test) if the point never comes in bounds
					{
						PointXY.x = 0;
						PointXY.y = 0;
						return PointXY;
					}

					if(pointInBounds(line_x, line_y))//do this when the point comes in bounds
					{
						PointXY.x = line_x;
						PointXY.y = line_y;
						return PointXY;
					}
				}
				else
				{
					if(!pointInBounds(line_x, line_y)) //do this when the point goes out of bounds
					{
						PointXY.x = line_x;
						PointXY.y = line_y;
						return PointXY;
					}
				}				
			}
		}
	}
	else
	{
		while(true)
		{
			line_y += move_y;
			error_term += difference_x;
			if((error_term > difference_y) || (difference_x == 0))
			{
				error_term -= difference_y;
				line_x += move_x;

				if(bounds == LINESTART)
				{
					if(line_y > 15000 || line_y < -15000) //(these constants are for test) if the point never comes in bounds
					{
						PointXY.x = 0;
						PointXY.y = 0;
						return PointXY;
					}

					if(pointInBounds(line_x, line_y))
					{
						PointXY.x = line_x;
						PointXY.y = line_y;
						return PointXY;
					}
				}
				else
				{
					if(!pointInBounds(line_x, line_y))
					{
						PointXY.x = line_x;
						PointXY.y = line_y;
						return PointXY;
					}
				}				
			}
	
		}
	}
	return PointXY;
}	

int Model3D::handleEvent(MADEEventStamp* event)
{
	if(user->input)
	{
	 	if(kat->windowDrag)
			return false;

		if (onMe(event))
		{
			if(!crosshair)
			{	
				crosshair = new(CrossHairCursor);
			}

			if(buttonDown)	//we are currently drawing the trajectory line
			{
				sfxEraseLine(mySprite, line);
				line = sfxDrawLine(mySprite, beg_x, beg_y, mouse_x, mouse_y, MK_COLOR16(255, 0, 0));
				if(line == -1)
					ASSERT(0, 0);

				end_x = mouse_x;
				end_y = mouse_y;
			}

			if(event_type == USER_LEFT_UP && buttonDown)  //we are done dragging the line
			{
				struct _pointxy lineXY;
				int i;

				buttonDown = false;
				sfxEraseLine(mySprite, line);  //erase the line we were dragging out

				if((beg_x == end_x) && (beg_y == end_y))  //there is only a single point
				{
					return true;
				}

				lineXY = findLineXY(end_x, end_y, LINEEND);  //draw the line out to the end of the screen
				end_x = lineXY.x;
				end_y = lineXY.y;
				line = sfxDrawLine(mySprite, beg_x, beg_y, end_x, end_y, MK_COLOR16(255, 0, 0));
				if(line == -1)
					ASSERT(0, 0);

				//this is for the snap to crap
				//this blows if you can't get the line close
				//you suck!!!!!!!
				switch(kat->current3dzoom)
				{
					case 3:
						switch(kat->cameraPosition)
						{
							case 1:
								if(kat->isTheJackassClose(snapRects[0]))
								{
									beg_x = 248;
									beg_y = 228;
									end_x = 494;	
									end_y = 193;
							 	}	
							 	break;

							case 2:
								if(kat->isTheJackassClose(snapRects[1]))
								{
									beg_x = 46;
									beg_y = 302;
									end_x = 491;	
									end_y = 237;
								}
							 	break;
						}
						break;

					case 2:
						switch(kat->cameraPosition)
						{
							case 1:
								if(kat->isTheJackassClose(snapRects[2]))
								{
									beg_x = 253;
									beg_y = 219;
									end_x = 499;	
									end_y = 183;
							 	}	
							 	break;

							case 2:
								if(kat->isTheJackassClose(snapRects[3]))
								{
									beg_x = 152;
									beg_y = 256;
									end_x = 500;	
									end_y = 205;
								}
							 	break;
						}
						break;
				}

				lineDrawn = true;
				gotLine = true;
				kat->setLineData(kat->current3dzoom, beg_x, beg_y, end_x, end_y);

				//transform Line for all possible zooms at this camera position	
				for(i = kat->current3dzoom - 1; i >= 0; i--)
				{
					kat->model3d->moveLine(i, ZOOMOUT);
				}								
				
				kat->copyLineToGlobal(kat->current3dzoom);
					
				for(i = kat->current3dzoom + 1; i < 4; i++)
				{
					kat->model3d->moveLine(i, ZOOMIN);
				}								
				/////////////////////////////////////////////
				return(true);
			}
			else if(event_type == USER_LEFT_DOWN)  
			{
				kat->clearLineData(); //we have a new line drawn so clear the prevous line data
				gotLine = false;     
				buttonDown = true;
				if(lineDrawn)
				{
					sfxEraseLine(mySprite, line);
					lineDrawn = false;
				}				

				beg_x = mouse_x;
				beg_y = mouse_y;
				line = sfxDrawLine(mySprite, beg_x, beg_y, mouse_x, mouse_y, MK_COLOR16(255, 0, 0)); 
				if(line == -1)
					ASSERT(0, 0);
				return (true);
			}
		}
		else  //not onME
		{
			if(buttonDown)
			{
				sfxEraseLine(mySprite, line);
				buttonDown = false;
			}
			if(crosshair)
			{
				delete crosshair;
				crosshair = NULL;
				theGame->setCursor(normalCursor);
			}
		}
		return (false);	
	}
	return (false);	
}

bool Model3D::drawSavedLine()
{
	if(kat->lineData[kat->current3dzoom].lineDrawn)	  //this line was previously transformed
	{
		line = sfxDrawLine(mySprite, kat->lineData[kat->current3dzoom].beg_x, kat->lineData[kat->current3dzoom].beg_y,
											 kat->lineData[kat->current3dzoom].end_x, kat->lineData[kat->current3dzoom].end_y, MK_COLOR16(255, 0, 0));
		if(line == -1)
			ASSERT(0, 0);
		lineDrawn = true;
		kat->copyLineToGlobal(kat->current3dzoom);
		return true;
	}				
	return false;
}

int Model3D::clipModelLine(int zoomPos)
{

	if(!pointInBounds(beg_x, beg_y))	 //if point is not in bounds
	{
		struct _pointxy lineXY = findLineXY(beg_x, beg_y, LINESTART);//find out beg_x, beg_y point in bounds
		if((lineXY.x == 0) && (lineXY.x == 0))
			return false;

		kat->setLineData(zoomPos, lineXY.x, lineXY.y, end_x, end_y);
	}
	else
	{	
		kat->setLineData(zoomPos, beg_x, beg_y, end_x, end_y);
	}													
	return true;
}

void Model3D::transformLine(_pointxy XY, int direction)
{
	if(direction == ZOOMIN)
	{
		beg_x = (beg_x - XY.x) * 2;	
		end_x = (end_x - XY.x) * 2;	
		end_y = (end_y - XY.y) * 2;
		beg_y = (beg_y - XY.y) * 2;
	}
	else
	{
		beg_x = (beg_x / 2) + XY.x;	
		end_x = (end_x / 2) + XY.x;	
		end_y = (end_y / 2) + XY.y;
		beg_y = (beg_y / 2) + XY.y;
	}
}	
	
int Model3D::moveLine(int zoomPos, int direction)
{
	struct _pointxy endXY;

	switch(zoomPos)
	{
		case 0:
			beg_x -= 2;
			end_x -= 2;
			beg_y -= 63;
			end_y -= 63;

   			transformLine(zoomx4[kat->cameraPosition], direction);

			beg_y += 133;
			end_y += 133;
			beg_x += 2;
			end_x += 2;

			endXY = kat->model3d->findLineXY(end_x, end_y, LINEEND);  //if line was drawn on a zoomed model we need to extend the line out to the edge
			end_x = endXY.x;
			end_y = endXY.y;

			return(clipModelLine(zoomPos));
		break;

		case 1:
			if(direction == ZOOMIN)
			{
				beg_x -= 2;
				end_x -= 2;
				beg_y -= 133;
				end_y -= 133;

				transformLine(zoomx4[kat->cameraPosition], direction);	
			}
			else //we have a ZOOMOUT
			{
				beg_x -= 2;
				end_x -= 2;
				beg_y -= 63;
				end_y -= 63;

				transformLine(zoomx16[kat->cameraPosition], direction);

				endXY = findLineXY(end_x, end_y, LINEEND);//extend line to edge of model
				end_x = endXY.x;
				end_y = endXY.y;
			}
			beg_y += 63;
			end_y += 63;
			beg_x += 2;
			end_x += 2;
			return(clipModelLine(zoomPos));
		break;

		case 2:
			beg_x -= 2;
			end_x -= 2;
			beg_y -= 63;
			end_y -= 63;

			if(direction == ZOOMIN)
			{
				transformLine(zoomx16[kat->cameraPosition], direction);
			}
			else //we have ZOOMOUT
			{
				transformLine(zoomx64, ZOOMOUT);

				endXY = findLineXY(end_x, end_y, LINEEND);
				end_x = endXY.x;
				end_y = endXY.y;
			}

			beg_y += 63;
			end_y += 63;
			beg_x += 2;
			end_x += 2;
			return(clipModelLine(zoomPos));
		break;

		case 3:
			beg_x -= 2;
			end_x -= 2;
			beg_y -= 63;
			end_y -= 63;

			transformLine(zoomx64, ZOOMIN);

			beg_y += 63;
			end_y += 63;
			beg_x += 2;
			end_x += 2;
			return(clipModelLine(zoomPos));
		break;	
	}
	return false;		
}

void Model3D::changeZoom(bool drawnLine, int zoom) {
	int modelView;

	switch(kat->current3dzoom)
	{
		case 0:
			setView(798);
			setLoop(0);
			setPri(15);
			posn(2, 133);
	//		sfxReleaseSprites ( backgrounds[curBack] );
	//		sfxUpdate();

			kat->hilight->setHilight();

			if(drawSavedLine()) //line was previously drawn
				return;
			break;
		 	 
		case 1:

			if(zoom == ZOOMIN)
			{
				modelView = 202 + kat->cameraPosition;
				setLoop(0);
				setView(modelView);
				setPri(15);
		//		sfxReleaseSprites ( backgrounds[curBack] );
		//		sfxUpdate();

				kat->hilight->setHilight();

			}
			else //we have a ZOOMOUT
			{
				modelView = 202 + kat->cameraPosition;
				setLoop(0);
				setView(modelView);
				setPri(15);
			//	sfxReleaseSprites ( backgrounds[curBack] );
			//	sfxUpdate();

				kat->hilight->setHilight();
			}
			kat->model3d->posn(2, 63);

			if(drawSavedLine())
				return;

			break;

		case 2:
			modelView = 215 + kat->cameraPosition;
			setLoop(0);
			setView(modelView);
			setPri(15);
			posn(2, 63);
		//	sfxReleaseSprites ( backgrounds[curBack] );
		//	sfxUpdate();

			kat->hilight->setHilight();

			if(drawSavedLine()) //line was previously drawn
				return;

			break;
			   
		case 3:
			modelView = 230 + kat->cameraPosition;
			setLoop(0);
			setView(modelView);
			setPri(15);
			posn(2, 63);
		//	sfxReleaseSprites ( backgrounds[curBack] );
		//	sfxUpdate();

			kat->hilight->setHilight();

			if(drawSavedLine()) //line was previously drawn
				return;

			break;
	}
}

 /***********************************
 *		
 *			  Buttons
 *		      
 **********************************/
void KatButton::disable()
{
	disabled = true;
	setCel(2);
}

void KatButton::enable()
{
	disabled = false;
	setCel(0);
}

ZoomIn3D::ZoomIn3D()
{
	name = "zoomin3d";
	init(422, 355, 200, 0, 0);
	noSkip();
}

void ZoomIn3D::activate()
{
	user->input = false;

	if(kat->current3dzoom != 3)
	{
		kat->current3dzoom++;
		kat->model3d->clear();
		kat->model3d->changeZoom(gotLine, ZOOMIN);					
		kat->setScript(new MagFlip(modelMagGlass, kat->current3dzoom));
		kat->zoomSFX->stop();
		//if(!kat->zoomSFX->isPlaying())
		kat->zoomSFX->play(171, NULL);
	}

	flushMessageCue();
	user->input = true;
}

ZoomOut3D::ZoomOut3D()
{
	name = "zoomout3d";	
	init(474, 355, 200, 1, 0);
	noSkip();
}

void ZoomOut3D::activate()
{
	user->input = false;

	if(kat->current3dzoom != 0)
	{
		kat->current3dzoom--;
		kat->model3d->clear();
		kat->model3d->changeZoom(gotLine, ZOOMOUT);
		kat->setScript(new MagFlip(modelMagGlass, kat->current3dzoom));
		kat->zoomSFX->stop();
	//	if(!kat->zoomSFX->isPlaying())
		kat->zoomSFX->play(172, NULL);
	}
	flushMessageCue();
	user->input = true;
}

WinZoomIn::WinZoomIn()
{
	init(camWin->cameraWin_x + 5, camWin->cameraWin_y + 143, 200, 0, 0);
	noSkip();
	setPri(512);
}

void WinZoomIn::activate()
{
	if(camWin->closed)
		return;

	if(camWin->currentZoom != 3)
	{
		camWin->currentZoom++;
		camWin->cameraview->setView(kat->CameraZoomView[camWin->currentZoom]);
		camWin->cameraview->setPri(512);
		camWin->cameraview->noSkip();
	//	sfxReleaseSprites ( backgrounds[curBack] );
	//	sfxUpdate();

		kat->hilight->setHilight();
		kat->setScript(new MagFlip(camWin->magglass, camWin->currentZoom));
		kat->zoomSFX->stop();
	//	if(!kat->zoomSFX->isPlaying())
		kat->zoomSFX->play(171, NULL);
	}
	flushMessageCue();
}

WinZoomOut::WinZoomOut()
{
	init(camWin->cameraWin_x + 57, camWin->cameraWin_y + 143, 200, 1, 0);
	noSkip();
	setPri(512);
}

void WinZoomOut::activate()
{
	if(camWin->closed)
		return;

	if(camWin->currentZoom != 0)
	{
		camWin->currentZoom--;
		camWin->cameraview->setView(kat->CameraZoomView[camWin->currentZoom]);
		camWin->cameraview->setPri(512);
		camWin->cameraview->noSkip();
	//	sfxReleaseSprites ( backgrounds[curBack] );
	//	sfxUpdate();

		kat->hilight->setHilight();
		kat->setScript(new MagFlip(camWin->magglass, camWin->currentZoom));
		kat->zoomSFX->stop();
	//	if(!kat->zoomSFX->isPlaying())
		kat->zoomSFX->play(172, NULL);
	}
	flushMessageCue();
}

CameraLeft::CameraLeft()
{
	init(338, 355, 200, 4, 0);
	noSkip();
}

void CameraLeft::activate()
{
	if(kat->cameraPosition != 0)
	{
		int i;
		if(kat->current3dzoom > 0)
		{
			kat->camera->setCel(--kat->cameraPosition);
			camWin->cameraview->setCel(kat->cameraPosition);
 			kat->cameraSFX->stop();
	//		if(!kat->cameraSFX->isPlaying())
			kat->cameraSFX->play(115, NULL);

			if(gotLine)
			{
				kat->copyLineToGlobal(0); //preserve the line from zoom 0
				kat->clearLineData();
				kat->setLineData(0, beg_x, beg_y, end_x, end_y); //set Line data for zoom 0
				for(i = 1; i < 4; i++)
				{
					kat->model3d->moveLine(i, ZOOMIN);
				}								
				kat->model3d->clear();
				kat->model3d->changeZoom(gotLine, ZOOMIN);					
				kat->copyLineToGlobal(kat->current3dzoom);
			}
			else
			{
				kat->model3d->clear();
				kat->model3d->changeZoom(gotLine, ZOOMIN);					
			}
 		//	kat->modelSFX->stop();
//				kat->modelSFX->play(116, NULL);
		}
		else
		{
			kat->camera->setCel(--kat->cameraPosition);
			camWin->cameraview->setCel(kat->cameraPosition);
 			kat->cameraSFX->stop();
		//	if(!kat->cameraSFX->isPlaying())
			kat->cameraSFX->play(115, NULL);

			if(gotLine)	//a line was drawn at some point
			{
				kat->copyLineToGlobal(0); //preserve the line from zoom 0
				kat->clearLineData();
				kat->setLineData(0, beg_x, beg_y, end_x, end_y); //set Line data for zoom 0
				for(i = 1; i < 4; i++)
				{
					kat->model3d->moveLine(i, ZOOMIN);
				}								
			}
			kat->hilight->setHilight();
		}
	}	 
	flushMessageCue();
}

CameraRight::CameraRight()
{
	init(389, 355, 200, 5, 0);
	noSkip();
}

void CameraRight::activate()
{
	if(kat->cameraPosition != 12)
	{
		int i;
		if(kat->current3dzoom > 0)
		{
			kat->camera->setCel(++kat->cameraPosition);
			camWin->cameraview->setCel(kat->cameraPosition);
 			kat->cameraSFX->stop();
		//	if(!kat->cameraSFX->isPlaying())
			kat->cameraSFX->play(115, NULL);

			if(gotLine)
			{
				kat->copyLineToGlobal(0); //preserve the line from zoom 0
				kat->clearLineData();
				kat->setLineData(0, beg_x, beg_y, end_x, end_y); //set Line data for zoom 0
				for(i = 1; i < 4; i++)
				{
					kat->model3d->moveLine(i, ZOOMIN);
				}								
				kat->model3d->clear();
				kat->model3d->changeZoom(gotLine, ZOOMIN);					
				kat->copyLineToGlobal(kat->current3dzoom);
			}
			else
			{
				kat->model3d->clear();
				kat->model3d->changeZoom(gotLine, ZOOMIN);					
			}
 		//	kat->modelSFX->stop();
	//			kat->modelSFX->play(116, NULL);
		}
		else
		{
			kat->camera->setCel(++kat->cameraPosition);
			camWin->cameraview->setCel(kat->cameraPosition);
 			kat->cameraSFX->stop();
		//	if(!kat->cameraSFX->isPlaying())
			kat->cameraSFX->play(115, NULL);

			if(gotLine)
			{
				kat->copyLineToGlobal(0); //preserve the line from zoom 0
				kat->clearLineData();
				kat->setLineData(0, beg_x, beg_y, end_x, end_y); //set Line data for zoom 0
				for(i = 1; i < 4; i++)
				{
					kat->model3d->moveLine(i, ZOOMIN);
				}								
			}
			kat->hilight->setHilight();
		}
	}
	flushMessageCue();
}

WindowClose::WindowClose()
{
	init(camWin->cameraWin_x + 145, camWin->cameraWin_y + 5, 200, 6, 0);
	noSkip();
	setPri(512);
}

void WindowClose::activate()
{
	camWin->closed = true;
	camWin->setCel(1);
	camWin->mix->hide();
	camWin->zoomin->hide();
	camWin->zoomout->hide();
	camWin->cameraview->hide();
	camWin->titlebar->dispose();
	camWin->titlebar = NULL;
	camWin->magglass->hide();
	camWin->posn(12, 352);
	new(WindowOpen);
	dispose();
}

WindowOpen::WindowOpen()
{
	init(12 + 145, 352 + 5, 200, 7, 0);
	noSkip();
	setPri(512);
}

void WindowOpen::activate()
{
 	camWin->closed = false;
	kat->setScript(new explodeScript(12, 343, camWin->cameraWin_x, camWin->cameraWin_y));
	dispose();
}

MixBut::MixBut()
{
	init(camWin->cameraWin_x + 81, camWin->cameraWin_y + 143, 200, 3, 0);
	noSkip();
	setPri(550);
}

void MixBut::activate()
{
 	if(camWin->closed)
		return;

	int theView = kat->CameraZoomView[camWin->currentZoom];
	int theCel = kat->camera->cel;

	intrpuzz->page->saveState();
	intrpuzz->dispose();
	
	if(katinfo->seenHTM)
		new KatMatch(theView, theCel);
	else
	{
		katinfo->seenHTM = true;
		new HTMLPage(98790);
	}
	
}

/*********************
*
*		Scripts
*
**********************/
void Convert3D::changeState ( int newState )
{
	switchTo
		if(GameFlag.test(fGreg39)) //dos 8 meg
		{
			user->input = false;
			state = 10;	
		}
		else
		{
			kat->model3danim = new(Model3DAnim);
			sfxSystemCursor(SYS_WAIT);
		}
		kat->katBetty->canInterupt = true;
		kat->katBetty->playBuffered(40126, kat);
		ticks = 5;
	END

	BEG
		user->input = false;
		kat->model3danim->setCel(1);
		ticks = 5;
	END

	BEG			  
		kat->model3danim->setCel(2);
		ticks = 5;
	END
	BEG
		kat->model3danim->setCel(3);
		ticks = 5;
	END
	BEG
		kat->model3danim->setCel(4);
		ticks = 5;
	END
	BEG
		kat->model3danim->setCel(5);
		ticks = 5;
	END
	BEG
		kat->model3danim->posn(2, 133);
		kat->model3danim->setCel(6);
		ticks = 5;
	END

	BEG
		kat->model3danim->setCel(7);
		ticks = 5;
	END
	BEG
		kat->model3danim->setCel(8);
		ticks = 5;
	END
	BEG
		kat->model3danim->setCel(9);
		ticks = 5;
	END
	BEG
		kat->model3danim->setCel(10);
		ticks = 1;
	END

	BEG
		if(!GameFlag.test(fGreg39))
		{
			kat->model3danim->dispose();
		}
		user->input = false;
		kat->model3d = new Model3D(798, 0);
		ticks = 8;
	END
	BEG
		int currZoom = 0;
		camWin = new CameraWindow(10, 63, currZoom);
		kat->camera = new(Camera);
		new(KatWindow);
		kat->hilight = new(ModelHilight);	
		kat->hilight->setHilight();
		ticks = 1;
	END
	BEG
		if(!kat->katBetty->isPlaying())
			user->input = true;
		dispose();
	END			
}

MagFlip::MagFlip(MagGlass *glass, int magnification)
{
	name = "MagFlip";
	theGlass = glass;
	zoomLevel = magnification;
}

void MagFlip::changeState ( int newState )
{
	switchTo
		theGlass->setCel(4);
		ticks = 5;
	END

	BEG
		theGlass->setCel(5);
		ticks = 5;
	END

	BEG
		theGlass->setCel(6);
		ticks = 5;
	END

	BEG
		theGlass->setCel(zoomLevel);
		dispose();
	END

}

explodeScript::explodeScript(int startX, int startY, int endX, int endY)
{
	name = "Explode";
	begin_x = startX;
	begin_y = startY;
	dest_x = endX;
	dest_y = endY;

	if(dest_x > begin_x)
	{
		move_x = (dest_x - begin_x)/8;
	}
	else
	{
		move_x = -((begin_x - dest_x)/8);
	}
			
	move_y = (begin_y - dest_y)/8;

}

void explodeScript::changeState ( int newState )
{
	SRect newRect;

	switchTo
		sfxSetDrawInfo (ADV_DRAW_SCALEX, 1024);
		sfxSetDrawInfo (ADV_DRAW_SCALEY, 1024);
		SRect_Init ( &newRect, 0, 0, camWin->minimize->rect.width, camWin->minimize->rect.height);
		newsprite = sfxClipSprite ( camWin->minimize, begin_x + move_x, begin_y - move_y, &newRect);
		move_x += move_x;
		move_y += move_y;
		ticks = 3;
	END

	BEG
		sfxSetDrawInfo (ADV_DRAW_SCALEX, 1024);
		sfxSetDrawInfo (ADV_DRAW_SCALEY, 1024);
		SRect_Init ( &newRect, 0, 0, newsprite->rect.width, newsprite->rect.height);
		tempsprite = sfxClipSprite ( newsprite, begin_x + move_x, begin_y - move_y, &newRect);
		sfxKillSprite(newsprite);
		newsprite = tempsprite;
		move_x += move_x;
		move_y += move_y;
		ticks = 3;
	END

	BEG
		sfxSetDrawInfo (ADV_DRAW_SCALEX, 1024);
		sfxSetDrawInfo (ADV_DRAW_SCALEY, 1024);
		SRect_Init ( &newRect, 0, 0, newsprite->rect.width, newsprite->rect.height);
		tempsprite = sfxClipSprite ( newsprite, begin_x + move_x, begin_y - move_y, &newRect);
		sfxKillSprite(newsprite);
		newsprite = tempsprite;
		move_x += move_x;
		move_y += move_y;
		ticks = 3;
	END

	BEG
		sfxSetDrawInfo (ADV_DRAW_SCALEX, 1024);
		sfxSetDrawInfo (ADV_DRAW_SCALEY, 1024);
		SRect_Init ( &newRect, 0, 0, newsprite->rect.width, newsprite->rect.height);
		tempsprite = sfxClipSprite ( newsprite, begin_x + move_x, begin_y - move_y, &newRect);
		sfxKillSprite(newsprite);
		newsprite = tempsprite;
		move_x += move_x;
		move_y += move_y;
		ticks = 5;
	END

	BEG	
		camWin->windowclose = new(WindowClose);
		camWin->setCel(0);
		camWin->moveTo(camWin->cameraWin_x, camWin->cameraWin_y);
		camWin->mix->show();
		camWin->zoomin->show();
		camWin->zoomout->show();
		camWin->cameraview->show();
		camWin->magglass->show();
		sfxKillSprite(newsprite);
		sfxHideSprite(camWin->minimize);
		dispose();
	END			
}



/*void explodeScript::changeState ( int newState )
{

	switchTo
		sfxShowSprite(camWin->minimize);
  		sfxMoveSprite(camWin->minimize, begin_x, begin_y);
		sfxSpriteScale(camWin->minimize, 1024, 1024);
  		sfxMoveSprite(camWin->minimize, begin_x + move_x, begin_y - move_y);
		move_x += move_x;
		move_y += move_y;
		ticks = 5;
	END

	BEG
		sfxSpriteScale(camWin->minimize, 2048, 2048);
  		sfxMoveSprite(camWin->minimize, begin_x + move_x, begin_y - move_y);
		move_x += move_x;
		move_y += move_y;
		ticks = 5;
	END

	BEG
		sfxSpriteScale(camWin->minimize, 4096, 4096);
  		sfxMoveSprite(camWin->minimize, begin_x + move_x, begin_y - move_y);
		move_x += move_x;
		move_y += move_y;
		ticks = 5;
	END

	BEG
		sfxSpriteScale(camWin->minimize, 8192, 8192);
  		sfxMoveSprite(camWin->minimize, begin_x + move_x, begin_y - move_y);
		ticks = 5;
	END			

	BEG	
		camWin->windowclose = new(WindowClose);
		camWin->setCel(0);
		camWin->moveTo(camWin->cameraWin_x, camWin->cameraWin_y);
		camWin->mix->show();
		camWin->zoomin->show();
		camWin->zoomout->show();
		camWin->cameraview->show();
		sfxHideSprite(camWin->minimize);
		dispose();
	END			
}

*/

} // namespace Spycraft
