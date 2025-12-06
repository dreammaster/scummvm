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

#include "spycraft/game/phodoc.h"
#include "spycraft/game/pcscreen.h"
#include "spycraft/game/flag.h"
#include "spycraft/game/invent.h"
#include "spycraft/game/verbs.h"
#include "spycraft/game/vlink.h"
#include "spycraft/game/web.h"

namespace Spycraft {

#define OBJECT_V			5

#define NEWS				701
#define CIGS				702
#define HEADS				703

SRect placeArea = {255, 268, 348, 503};

SRect noPlaceArea[9] = {{238, 270, 277, 379}, {256, 380, 280, 421}, {291, 342, 305, 385},
						{257, 481, 268, 502}, {329, 270, 347, 287}, {322, 407, 331, 503},
						{306, 479, 349, 503}, {317, 447, 340, 481}, {328, 419, 338, 445}};

bool scaleBox;
bool boxButtondown;
BoxCorner			*boxcorner;
Photo				*currentPhoto;

ObjectCursor		*currentCursor;
extern BlinkCursor	*normalCursor;
PrintedPhoto		*printedPhoto;

PhotoDocInfo		*photodocinfo;
PhotoDoc			*photodoc;

PhotoDoc::PhotoDoc()
{
	webreport->hide();

	currentTarget = NULL;
	currentObject = NULL;
	targetphoto = NULL;
	cursorHidden = false;
	pcLastUsed[PC_PHOTODOC] = true;	//make photodoc available to the pc	screen

	photodoc = this;
	IntrPuzzPage::init(WEB_PHOTODOC);    
	helppage = 92450;

	new(BackGrnd);
	currentPhoto = new(Photo);
	currentPhoto->init(8, 90, 701, 0);
	targetphoto = new(TargetPhoto);
	new PhotoObject(12, 119, 76, 29, 701, 1, 0);
	new PhotoObject(89, 110, 66, 41, 701, 1, 1);
	new PhotoObject(166, 100, 64, 50, 701, 1, 2);
	new PhotoObject(11, 186, 89, 27, 701, 2, 0);
	new PhotoObject(99, 173, 79, 39, 701, 2, 1);
	new PhotoObject(173, 160, 77, 55, 701, 2, 2);
	new PhotoObject(12, 249, 81, 26, 701, 3, 0);
	new PhotoObject(90, 235, 71, 40, 701, 3, 1);
	new PhotoObject(165, 224, 66, 52, 701, 3, 2);
	new PhotoObject(13, 315, 87, 30, 701, 4, 0);
	new PhotoObject(103, 309, 69, 36, 701, 4, 1);
	new PhotoObject(173, 285, 71, 62, 701, 4, 2);

	new(NewsButt);
	new(CigsButt);
	new(FaceButt);

	///new(ScaleBut);
	new(UndoBut);
	new(ResetBut);
	new(PrintBut);
	new(NewsButt);
	new(CigsButt);
	new(FaceButt);
	TargetObjects = new(List);

	new(DoctorWin);
	if(intrpuzz->isStateSaved("photodocinfo"))
	{
		photodocinfo->load();
	}
}

void PhotoDoc::dispose()
{
	if(web_intrface)
		webreport->show();

	if(!TargetObjects->isEmpty())
	{
		TargetObjects->dispose();
	}
	delete TargetObjects;

	IntrPuzzPage::dispose();
}

void PhotoDoc::saveState()
{
		if(!intrpuzz->isStateSaved("photodocinfo"))
		{
			new(PhotoDocInfo);
		}		
		photodocinfo->save();
}	

/***********************************
 *		
 *		 Puzzle Save State
 *		      
 **********************************/
PhotoDocInfo::PhotoDocInfo()
{
	name = "photodocinfo";
	web_intrface->addToFront(this); 
	photodocinfo = this;
}

void PhotoDocInfo::save()
{
	Node *n = photodoc->TargetObjects->head;
	int indexCtr = 0;

	while ( n ) {
		saveObjects[indexCtr].theX = ((TargetObject*)n->data)->x;
		saveObjects[indexCtr].theY = ((TargetObject*)n->data)->y;
		saveObjects[indexCtr].theView = ((TargetObject*)n->data)->view;
		saveObjects[indexCtr].theLoop = ((TargetObject*)n->data)->loop;
		saveObjects[indexCtr].theCel = ((TargetObject*)n->data)->cel;
		saveObjects[indexCtr].theScaleX = ((TargetObject*)n->data)->theScaleX;
		saveObjects[indexCtr].theScaleY = ((TargetObject*)n->data)->theScaleY;
		indexCtr++;

		n = n->next;
	}
	numTargetObjects = indexCtr;
}

void PhotoDocInfo::load()
{
	for(int indexCtr = 0; indexCtr < numTargetObjects; indexCtr++)
	{
		TargetObject* temp = new TargetObject();
		temp->init(saveObjects[indexCtr].theX, saveObjects[indexCtr].theY, saveObjects[indexCtr].theView, saveObjects[indexCtr].theLoop, saveObjects[indexCtr].theCel);
		temp->theScaleX = saveObjects[indexCtr].theScaleX;
		temp->theScaleY = saveObjects[indexCtr].theScaleY;
		sfxSpriteScale(temp->mySprite, saveObjects[indexCtr].theScaleX, saveObjects[indexCtr].theScaleY);
		temp->setRect();	
	}
}

void PhotoDocInfo::dispose()
{
	web_intrface->release ( this );
	deathList->add(this);
}

/***********************************
 *		
 *			 Doctor Window
 *		      
 **********************************/
DoctorWin::DoctorWin()
{
	init(10, 94, 488, 252);
}

int DoctorWin::handleEvent ( MADEEventStamp *event )
{
	if (user->input)
	{
	  	if (currentCursor)
		{
			int cursor_x;
			int cursor_y;
			int objectLeft = mouse_x;
			int objectTop = mouse_y;
			int objectRight	= objectLeft + currentCursor->cursorWidth;
			int objectBottom = objectTop + currentCursor->cursorHeight;
			bool ySet = true;
			bool xSet = true;

			if(objectLeft < left)
			{
				cursor_x = left;
				objectLeft = left;
			}		
			else if(objectRight > right)
			{
				cursor_x = right - currentCursor->cursorWidth;
				objectLeft = right - currentCursor->cursorWidth;
			}
			else
			{
				xSet = false;
				cursor_x = mouse_x;
			}

		 	if(objectTop < top)
			{
				cursor_y = top;
				objectTop = top;
			}
			else if(objectBottom > bottom)
			{
				cursor_y = bottom - currentCursor->cursorHeight;
				objectTop = bottom - currentCursor->cursorHeight;
			}	
			else
			{
				ySet = false;
				cursor_y = mouse_y;
			}
			
			if(ySet || xSet)
				sfxSetCursorPos(cursor_x, cursor_y);

			return false;
		}
	}
	return false;
}
/***********************************
 *		
 *			 Cursors
 *		      
 **********************************/
ObjectCursor::ObjectCursor()
{
	name = "ObjectCursor";
}

void ObjectCursor::init(int theView, int theLoop, int theCel, int theWidth, int theHeight)
{
	view = theView;
	loop = theLoop;
	cel = theCel;
	orig_pt.x = mouse_x;
	orig_pt.y = mouse_y;
	cursorWidth = theWidth;
	cursorHeight = theHeight;
	verb = OBJECT_V;
	theGame->setCursor(this);
}
		 
/***********************************
 *		
 *		Features	  
 *		      
 **********************************/
BoxCorner::BoxCorner(int theX, int theY/*, int theLocation*/)
{
	name = "BoxCorner";
	boxcorner = this;
	theX -= 5;
	theY -= 5;	
	init(theX, theY, 700, 0, 0);
}	

int BoxCorner::handleEvent ( MADEEventStamp *event )
{
	if (user->input && onMe(event))
	{
	  	if (event_type == USER_LEFT_DOWN && scaleBox)
		{
			boxButtondown = true;
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
VLine::VLine(int theX, int theY)
{
	view = 700;
	loop = 9;
	cel = 0;
	init(theX, theY);
	ratioY = (float)512 / (float)16;
	setPri(555);
}

HLine::HLine(int theX, int theY)
{
	view = 700;
	loop = 9;
	cel = 1;
	init(theX, theY);
	ratioX = (float)512 / (float)16;
	setPri(555);
}
	
TargetPhoto::TargetPhoto()
{
	view = 700;
	loop = 1;
	cel = 0;
	init(260, 90);
	scaleBox = false;
	objectSelected = false;
//	addToPic();
}


int TargetPhoto::handleEvent ( MADEEventStamp *event )
{
	if(user->input)
	{
	 	if(photodoc->currentTarget)
		{
			if(photodoc->currentTarget->drag)
			{
			  	if (event_type == USER_LEFT_UP || !onMe(event))
				{
					photodoc->currentTarget->drag = false;
					photodoc->currentTarget->fromLeft = 0;
					photodoc->currentTarget->fromTop = 0;
				}
				else
				{		 
					int cursor_x;
					int cursor_y;
					int objectLeft = mouse_x - photodoc->currentTarget->fromLeft;
					int objectTop = mouse_y - photodoc->currentTarget->fromTop;
					int objectRight	= objectLeft + photodoc->currentTarget->width;
					int objectBottom = objectTop + photodoc->currentTarget->height;
					bool ySet = true;
					bool xSet = true;
				
					if(objectLeft < left)
					{
						cursor_x = left + photodoc->currentTarget->fromLeft;
						objectLeft = left;
					}		
					else if(objectRight > right)
					{
						cursor_x = (right - photodoc->currentTarget->width) + photodoc->currentTarget->fromLeft;
						objectLeft = right - photodoc->currentTarget->width;
					}
					else
					{
						xSet = false;
						cursor_x = mouse_x;
					}

				 	if(objectTop < top)
					{
						cursor_y = top + photodoc->currentTarget->fromTop;
						objectTop = top;
					}
					else if(objectBottom > bottom)
					{
						cursor_y = (bottom - photodoc->currentTarget->height) + photodoc->currentTarget->fromTop;
						objectTop = bottom - photodoc->currentTarget->height;
					}	
					else
					{
						ySet = false;
						cursor_y = mouse_y;
					}
					
					if(xSet || ySet)
					{
						sfxSetCursorPos(cursor_x, cursor_y);
					}
					photodoc->currentTarget->posn(objectLeft, objectTop);
					return true;
				}
			}		
		}	
				  
		if (onMe(event))
		{
			if(currentCursor)//we have got an object from the source photo
			{
			  	if (event_type == USER_LEFT_UP)	
				{
					TargetObject* target = new TargetObject();
					target->init(mouse_x, mouse_y, photodoc->currentObject->theView, photodoc->currentObject->theLoop, photodoc->currentObject->theCel);
					delete currentCursor;
					currentCursor = NULL;
					photodoc->currentObject = NULL;
					theGame->setCursor(normalCursor);

					if(objectSelected)
					{
						photodoc->targetphoto->eraseBox();
						photodoc->currentTarget = NULL;
					}						
					photodoc->currentTarget = target;
					int theX = target->x - 2;
					int theY = target->y - 2;
					int theWidth = target->width + 4;
					int theHeight = target->height + 4;

					photodoc->targetphoto->vlineLeft = new VLine(theX, theY);
					photodoc->targetphoto->hlineTop = new HLine(theX, theY);
					photodoc->targetphoto->vlineRight = new VLine(theX + theWidth, theY);
					photodoc->targetphoto->hlineBottom = new HLine(theX, theY + (theHeight - 1));
 
					photodoc->targetphoto->drawBox(theX, theY, theWidth, theHeight);
					new BoxCorner(theX + theWidth, theY + theHeight);
				}
				return true;		
			}
	  
	/*		if(!photodoc->currentTarget && objectSelected)
			{
				eraseBox();
			}	
	*/
			if(boxButtondown)
			{
				TargetObject *target = photodoc->currentTarget;
																
			  	if (event_type == USER_LEFT_UP)
				{
					boxButtondown = false;
					target->theScaleX = int((float)boxwidth * target->ratioX);
					target->theScaleY = int((float)boxheight * target->ratioY);
					sfxSpriteScale(target->mySprite, target->theScaleX, target->theScaleY);
					target->setRect();	
					boxcorner->posn(target->x + boxwidth, target->y + boxheight);
				}

				int theX, theY;
				theX = target->x - 2;
				theY = target->y - 2;
				boxwidth = mouse_x - theX;
				boxheight = mouse_y - theY;
				if((boxwidth < 7) || (boxheight < 7))
				{
					if(boxwidth < 7)
					{
						boxwidth = 7;
						sfxSetCursorPos(target->x + boxwidth, mouse_y);
					}
					if(boxheight < 7)
					{
						boxheight = 7;	
						sfxSetCursorPos(mouse_x, target->y + boxheight);
					}
					return true;
				}
			/*	if(boxwidth > boxheight)
				{
					boxheight = boxwidth;
					sfxSetCursorPos(mouse_x, theY + boxheight);
				}
				else if(boxheight > boxwidth)
				{
					boxwidth = boxheight;
					sfxSetCursorPos(theX + boxwidth, mouse_y);
				}*/
				drawBox(theX, theY, boxwidth, boxheight);
				boxcorner->posn((theX + boxwidth) - 4, (theY + boxheight) - 4);
				return true;
			}
			   
		  	if (event_type == USER_LEFT_DOWN) //if we have a left up on the Photo and nothing else is being done
			{
				if(objectSelected)
				{
					photodoc->targetphoto->eraseBox();
					photodoc->currentTarget = NULL;
				}						
			}
					
			photodoc->TargetObjects->handleEvent(event);
		}
		else if(boxButtondown) //if were not on the photo but we are dragging the box
		{
			int cursor_x = 0;
			int cursor_y = 0;

			if(mouse_y > (bottom - 2))
			{
				cursor_y = bottom - 2;
				sfxSetCursorPos(mouse_x, cursor_y);
			}										
			if(mouse_x > (right - 2))
			{
				cursor_x = right - 2;
				sfxSetCursorPos(cursor_x, mouse_y);
			}
//			if(cursor_x || cursor_y)
//				return true;
		}
	}
	return false;
}

void TargetPhoto::eraseBox()
{	
	vlineLeft->dispose();
	vlineRight->dispose();
	hlineTop->dispose();
	hlineBottom->dispose();
	if(boxcorner)
	{
		boxcorner->dispose();
		boxcorner = NULL;
		scaleBox = false;
	}
	objectSelected = false;
}

void TargetPhoto::drawBox(int xp, int yp, int theWidth, int theHeight) {
	int scale_x = int((float)theWidth * hlineTop->ratioX);
	int scale_y = int((float)theHeight * vlineLeft->ratioY);

	vlineRight->posn(xp + (theWidth - 1), yp); 
	hlineBottom->posn(xp, yp + (theHeight - 2)); 
	sfxSpriteScale(vlineLeft->mySprite, 512, scale_y);
	sfxSpriteScale(vlineRight->mySprite, 512, scale_y);
	sfxSpriteScale(hlineTop->mySprite, scale_x, 512);
	sfxSpriteScale(hlineBottom->mySprite, scale_x, 512);
	objectSelected = true;
	scaleBox = true;
}
	
void Photo::init(int theX, int theY, int theView, int theLoop) {
	view = theView;
	loop = theLoop;
	cel = 0;
	x = theX;
	y = theY;
	draw();
	intrpuzz->add ( this );
//	IntrPuzzView::init(theX, theY);
	noSkip();						 
}							  

int Photo::handleEvent ( MADEEventStamp *event )
{

	if (user->input && onMe(event))
	{
		if(currentCursor)
		{
		  	if (event_type == USER_LEFT_UP)
		    {
				delete currentCursor;
				currentCursor = NULL;
				photodoc->currentObject = NULL;
				theGame->setCursor(normalCursor);
			}
			return true;
		}
	}
	return false;
}

BackGrnd::BackGrnd()
{
	view = 704;
	loop = 0;
	cel = 0;
	init(0, 16);
	noSkip();
	addToPic();
	dispose();
}

/***********************************
 *		
 *		Photo Objects	  
 *		      
 **********************************/
void TargetObject::init(int theX, int theY, int theView, int theLoop, int theCel)
{
	name = "Target Object";
	view = theView;
	loop = theLoop;
	cel = theCel;
	x = theX;
	y = theY;
	theScaleX = 512;
	theScaleY = 512;
	drag = false;
	hi_Lit = false;
	fromLeft = 0;
	fromTop = 0;

	draw();
	ratioX = /*(float)*/theScaleX / /*(float)*/width;
	ratioY = /*(float)*/theScaleY / /*(float)*/height;
	
	photodoc->TargetObjects->addToFront(this);
}

int TargetObject::handleEvent ( MADEEventStamp *event )
{
	if (user->input && onMe(event))
	{
		if(event_type == USER_LEFT_DOWN)
		{
			fromLeft = mouse_x - left;
			fromTop = mouse_y - top;	
			if(photodoc->currentTarget != this)
			{
				photodoc->currentTarget = this;
				scaleBox = false;
			}
			drag = true;
			if(photodoc->targetphoto->objectSelected)
			{
				photodoc->targetphoto->eraseBox();
			}						
			return true;
		}

		if((event_type == USER_LEFT_UP) && (!photodoc->targetphoto->objectSelected))
		{
 			drag = false;
			if(photodoc->currentTarget != this)
				return false;
//			TargetObject *target = photodoc->currentTarget;	   
			int theX = x - 2;
			int theY = y - 2;
			int theWidth = width + 4;
			int theHeight = height + 4;

			photodoc->targetphoto->vlineLeft = new VLine(theX, theY);
			photodoc->targetphoto->hlineTop = new HLine(theX, theY);
			photodoc->targetphoto->vlineRight = new VLine(theX + theWidth, theY);
			photodoc->targetphoto->hlineBottom = new HLine(theX, theY + (theHeight - 1));
 
			photodoc->targetphoto->drawBox(theX, theY, theWidth, theHeight);
			new BoxCorner(theX + theWidth, theY + theHeight);
		/*	if(!scaleBox)
			{
				boxcorner->hide();
			}
			*/return true;
		}
	}
	return false;
}

void TargetObject::dispose()
{
	photodoc->TargetObjects->release(this);
  	deathList->add(this);
}

PhotoObject::PhotoObject(int theX, int theY, int theWidth, int theHeight, int view, int loop, int cel)
{
	name = "delete";
	theView = view;
	theLoop = loop;
	theCel = cel;
	init(theX, theY, theWidth, theHeight);
}

int PhotoObject::handleEvent ( MADEEventStamp *event )
{
	if (user->input && onMe(event))
	{
		if(currentCursor)
		{
			return false;
		}
	  	if (event_type == USER_LEFT_DOWN)
	    {
			if(photodoc->TargetObjects->size == 25)
				MessageBox ( GetFocus(), "Unable to add objects to the target image.", "Photo Doctoring Tool", MB_OK );
			else
			{
				currentCursor = new(ObjectCursor);
				currentCursor->init(theView, theLoop, theCel, right - left, bottom - top);
				photodoc->currentObject = this;
				photodoc->cursorHidden = false;
			}
			return true;
		}
	}
	return false;
}

/***********************************
 *		
 *			  Buttons
 *		      
 **********************************/
NewsButt::NewsButt()
{
	init(9, 353, 73, 24);
	objectTab = new(IntrPuzzView);
	objectTab->init(9, 349, 700, 6, 0);
	objectTab->setPri(555);
}

int NewsButt::handleEvent(MADEEventStamp* event)
{
	if (onMe(event) && user->input)  
	{
	  	if (event_type == USER_LEFT_UP)
	    {
			objectTab->setPri(555);
			intrpuzz->remove_MarkedItems();
			currentPhoto->dispose();
			currentPhoto = new(Photo);
			currentPhoto->init(8, 90, 701, 0);

			new PhotoObject(12, 119, 76, 29, 701, 1, 0);
			new PhotoObject(89, 110, 66, 41, 701, 1, 1);
			new PhotoObject(166, 100, 64, 50, 701, 1, 2);
			new PhotoObject(11, 186, 89, 27, 701, 2, 0);
			new PhotoObject(99, 173, 79, 39, 701, 2, 1);
			new PhotoObject(173, 160, 77, 55, 701, 2, 2);
			new PhotoObject(12, 249, 81, 26, 701, 3, 0);
			new PhotoObject(90, 235, 71, 40, 701, 3, 1);
			new PhotoObject(165, 224, 66, 52, 701, 3, 2);
			new PhotoObject(13, 315, 87, 30, 701, 4, 0);
			new PhotoObject(103, 309, 69, 36, 701, 4, 1);
			new PhotoObject(173, 285, 71, 62, 701, 4, 2);

			return (true);
		}
	}
	return(false);
}

CigsButt::CigsButt()
{
	init(94, 353, 73, 24);
	objectTab = new(IntrPuzzView);
	objectTab->init(9, 349, 700, 7, 0);
}

int CigsButt::handleEvent(MADEEventStamp* event)
{
	if (onMe(event) && user->input)  
	{
	  	if (event_type == USER_LEFT_UP)
	    {
			objectTab->setPri(555);
			intrpuzz->remove_MarkedItems();
			currentPhoto->dispose();
			currentPhoto = new(Photo);
			currentPhoto->init(8, 90, 702, 0);

			new PhotoObject(20, 116, 49, 30, 702, 1, 0);
			new PhotoObject(104, 112, 48, 37, 702, 1, 1);
			new PhotoObject(187, 109, 47, 43, 702, 1, 2);
			new PhotoObject(21, 182, 48, 30, 702, 2, 0);
			new PhotoObject(103, 179, 47, 36, 702, 2, 1);
			new PhotoObject(184, 174, 48, 45, 702, 2, 2);
			new PhotoObject(23, 250, 43, 28, 702, 3, 0);
			new PhotoObject(106, 248, 42, 32, 702, 3, 1);
			new PhotoObject(190, 244, 42, 41, 702, 3, 2);
			new PhotoObject(23, 314, 43, 28, 702, 4, 0);
			new PhotoObject(107, 311, 42, 33, 702, 4, 1);
			new PhotoObject(191, 307, 41, 39, 702, 4, 2);
			return (true);
		}
	}
	return(false);
}

FaceButt::FaceButt()
{
	init(170, 353, 73, 24);
	objectTab = new(IntrPuzzView);
	objectTab->init(9, 349, 700, 8, 0);
}

int FaceButt::handleEvent(MADEEventStamp* event)
{
	if (onMe(event) && user->input)  
	{
	  	if (event_type == USER_LEFT_UP)
	    {
			objectTab->setPri(555);
			intrpuzz->remove_MarkedItems();
			currentPhoto->dispose();
			currentPhoto = new(Photo);
			currentPhoto->init(8, 90, 703, 0);

			new PhotoObject(24, 126, 65, 86, 703, 1, 0);
			new PhotoObject(105, 125, 61, 92, 703, 1, 1);
			new PhotoObject(176, 125, 63, 91, 703, 1, 2);
			new PhotoObject(27, 234, 65, 88, 703, 1, 3);
			new PhotoObject(104, 234, 61, 86, 703, 1, 4);
			new PhotoObject(175, 238, 64, 77, 703, 1, 5);
			return (true);
		}
	}
	return(false);
}

UndoBut::UndoBut()
{
	init(318, 355, 700, 2, 0);
	noSkip();
}

void UndoBut::activate()
{
	if(photodoc->currentTarget)
	{
		if(photodoc->targetphoto->objectSelected)
		{
			photodoc->targetphoto->eraseBox();
		//	scaleBox = false;
		}	
		photodoc->currentTarget->dispose();
		photodoc->currentTarget = NULL;
	}
}

ResetBut::ResetBut()
{
	init(381, 355, 700, 3, 0);
	noSkip();
}

void ResetBut::activate()
{
	if(photodoc->targetphoto->objectSelected)
	{
		photodoc->targetphoto->eraseBox();
	}						
	photodoc->TargetObjects->dispose();
	delete photodoc->TargetObjects;
	photodoc->TargetObjects = new(List);

//	photodoc->targetphoto->dispose();
//	photodoc->targetphoto = new(TargetPhoto);
	photodoc->currentTarget = NULL;
}

PrintStatusBar::PrintStatusBar()
{
	user->input = false;
	printbox = new (IntrPuzzView);
	printbox->init(150, 150, 705, 0, 0);
	printbox->setPri(800);
	printbox->noSkip();

	name = "statusbar";
	cycleSpeed = 12;	
	view = 705;
	loop = 1;
	cel = 0;
	x = 172;
	y = 180;
	draw();
	setPri(805);
	intrpuzz->addToFront ( this );
	setCycle(new EndLoop((Prop*)this, this));
}

void PrintStatusBar::cue(void)
{
	printbox->dispose();
	dispose();
	user->input = true;
}

PhotoDocInset::PhotoDocInset()
{
	printedPhoto->insetObjects = new(List);
}

void PhotoDocInset::dispose()
{
	if(!printedPhoto->insetObjects->isEmpty())
		printedPhoto->insetObjects->dispose();
	InvInset::dispose();
}

void InsetObject::init(int theX, int theY, int theView, int theLoop, int theCel)
{
	name = "Inset Object";
	view = theView;
	loop = theLoop;
	cel = theCel;
	x = theX;
	y = theY;
	theScaleX = 512;
	theScaleY = 512;

	draw();
	ratioX = /*(float)*/theScaleX / /*(float)*/width;
	ratioY = /*(float)*/theScaleY / /*(float)*/height;
	
	printedPhoto->insetObjects->addToFront(this);
}

PrintedPhoto::PrintedPhoto()
{
	name = "printed photo";
	printedPhoto = this;
}

void PrintedPhoto::printPhoto()
{
	Node *n = photodoc->TargetObjects->head;
	int indexCtr = 0;

	while ( n ) {
		printObjects[indexCtr].theX = ((TargetObject*)n->data)->x - 260;
		printObjects[indexCtr].theY = ((TargetObject*)n->data)->y - 90;
		printObjects[indexCtr].theView = ((TargetObject*)n->data)->view;
		printObjects[indexCtr].theLoop = ((TargetObject*)n->data)->loop;
		printObjects[indexCtr].theCel = ((TargetObject*)n->data)->cel;
		printObjects[indexCtr].theScaleX = ((TargetObject*)n->data)->theScaleX;
		printObjects[indexCtr].theScaleY = ((TargetObject*)n->data)->theScaleY;
		indexCtr++;

		n = n->next;
	}
	numTargetObjects = indexCtr;
}

void PrintedPhoto::showPhoto()
{
	int insetX = 155;
	int insetY = 90;
	PhotoDocInset *inset = new(PhotoDocInset);
	inset->init(706, insetX, insetY, 200, 240);
	inset->invInsetView->setPri(799);
	
	for(int indexCtr = 0; indexCtr < numTargetObjects; indexCtr++)
	{
		InsetObject* temp = new InsetObject();
		temp->init(printObjects[indexCtr].theX + insetX, printObjects[indexCtr].theY + insetY, printObjects[indexCtr].theView, printObjects[indexCtr].theLoop, printObjects[indexCtr].theCel);
		temp->theScaleX = printObjects[indexCtr].theScaleX;
		temp->theScaleY = printObjects[indexCtr].theScaleY;
		sfxSpriteScale(temp->mySprite, printObjects[indexCtr].theScaleX, printObjects[indexCtr].theScaleY);
		temp->setRect();	
		temp->setPri(800);
	}
}

PrintBut::PrintBut()
{
	init(432, 383, 700, 5, 0);
	noSkip();
}

void PrintBut::activate()
{
	bool gotCigs = false;
	bool gotNews = false;
	bool gotHead = false;

	Node *n = photodoc->TargetObjects->head;
	int indexCtr = 0;

	new(PrintStatusBar);

	if(!GameFlag.test(fSetVL_Max164) && GameFlag.test(fKnowYingInMoscow))
	{
		if( GameFlag.test( fMetMaxFoster ))
		{
			new VLink( "Ying is bagged: Foster, Maxine M.", "1641.avi" );
			GameFlag.set(fSetVL_Max164);
			if (!strcmp(curRoom->name,"Rm24100"))
				GameFlag.set(fYingCaptured);	
		}
		else
		if (strcmp(curRoom->name,"Rm35100"))
		{
	   		new VLink("Ying is bagged: Foster, Maxine M.", "164.avi", 0, 0);
			GameFlag.set(fSetVL_Max164);
			GameFlag.set(fYingCaptured);
		}
	}

	if(!GameFlag.test(fPrintedPhoto))
	{
		new(PrintedPhoto); //this flag knows that you printed out a photo
		inventry->get(iPHOTODOCPICTURE);
		GameFlag.set(fPrintedPhoto);
	}
	printedPhoto->printPhoto();

	while(n)
	{
		switch(((TargetObject*)n->data)->view)
		{
			case CIGS:
				if(((TargetObject*)n->data)->loop == 3)
				{	
					if((((TargetObject*)n->data)->cel == 0) || (((TargetObject*)n->data)->cel == 1))
					{	
						if(inBounds(((TargetObject*)n->data)->x, ((TargetObject*)n->data)->y, ((TargetObject*)n->data)->right, ((TargetObject*)n->data)->bottom))
						{
							if(!outOfBounds(((TargetObject*)n->data)->x, ((TargetObject*)n->data)->y, ((TargetObject*)n->data)->right, ((TargetObject*)n->data)->bottom))
							{
								if((((TargetObject*)n->data)->theScaleX < 430) && (((TargetObject*)n->data)->theScaleY < 443))
								{
									if((((TargetObject*)n->data)->theScaleX > 198) && (((TargetObject*)n->data)->theScaleY > 265))
									{
										gotCigs = true;
									}
								}
							}
						}
					}
				}
				break;
			
			case NEWS:
				if(((TargetObject*)n->data)->loop == 3)
				{	
					if((((TargetObject*)n->data)->cel == 1) || (((TargetObject*)n->data)->cel == 0))
					{	
						if(inBounds(((TargetObject*)n->data)->x + 5, ((TargetObject*)n->data)->y + 5, ((TargetObject*)n->data)->right - 5, ((TargetObject*)n->data)->bottom - 5))
						{
							if(!outOfBounds(((TargetObject*)n->data)->x + 10, ((TargetObject*)n->data)->y + 10, ((TargetObject*)n->data)->right - 10, ((TargetObject*)n->data)->bottom - 10))
							{
								if((((TargetObject*)n->data)->theScaleX < 650) && (((TargetObject*)n->data)->theScaleY < 600))
								{
									if((((TargetObject*)n->data)->theScaleX > 350) && (((TargetObject*)n->data)->theScaleY > 415))
									{
										gotNews = true;
									}
								}
							}
						}
					}
				}
				break;

			case HEADS:
				if((((TargetObject*)n->data)->loop == 1) && ((((TargetObject*)n->data)->cel == 3) || (((TargetObject*)n->data)->cel == 2) || (((TargetObject*)n->data)->cel == 1)))
				{
					if((((TargetObject*)n->data)->x > 340) && (((TargetObject*)n->data)->x < 405))
					{
						if((((TargetObject*)n->data)->y > 170) && (((TargetObject*)n->data)->y < 245))
						{
							if((((TargetObject*)n->data)->right > 340) && (((TargetObject*)n->data)->right < 405))
							{
								if((((TargetObject*)n->data)->bottom > 170) && (((TargetObject*)n->data)->bottom < 245))
								{
									if((((TargetObject*)n->data)->theScaleX > 244) && (((TargetObject*)n->data)->theScaleY > 210))
									{
										gotHead = true;
									}
								}
							}
						}

					}
				}
				break;
		}
														
		n = n->next;
		indexCtr++;
	}

	if(indexCtr != 3)
	{
	//	sfxPrintf("photo not done correctly");
		new EMail("That photo won't fly: McManus, Greg", 98052, 0, 2);
		GameFlag.clear(fDidPhotoDoc);
		return;
	}
					
	if(gotHead && gotNews && gotCigs)
	{	
//		sfxPrintf("photo done correctly");
		new EMail("Good job on the photo: McManus, Greg", 98051, 0, 2);
		GameFlag.set(fDidPhotoDoc);
	}
	else
	{		
		new EMail("That photo won't fly: McManus, Greg", 98052, 0, 2);
		GameFlag.clear(fDidPhotoDoc);
	}
}

int PrintBut::inBounds(int theLeft, int theTop, int theRight, int theBottom)
{
	if((theLeft > placeArea.left) && (theLeft < placeArea.right))
	{
		if((theTop > placeArea.top) && (theTop < placeArea.bottom))
		{
			if((theRight > placeArea.left) && (theRight < placeArea.right))
			{
				if((theBottom > placeArea.top) && (theBottom < placeArea.bottom))
				{
					return true;
				}
			}
		}		 				
	}			

	return false;
}

int PrintBut::outOfBounds(int theLeft, int theTop, int theRight, int theBottom)
{
	for(int i = 0; i < 9; i++)
	{
		if((theLeft > noPlaceArea[i].left) && (theLeft < noPlaceArea[i].right))
		{
			if((theTop > noPlaceArea[i].top) && (theTop < noPlaceArea[i].bottom))
			{
				return true;
			}
		}

		if((theRight > noPlaceArea[i].left) && (theRight < noPlaceArea[i].right))
		{
			if((theBottom > noPlaceArea[i].top) && (theBottom < noPlaceArea[i].bottom))
			{
				return true;
			}
		}
	}
	return false;
}

ScaleBut::ScaleBut()
{
	init(255, 355, 700, 4, 0);
	noSkip();
}

void ScaleBut::activate()
{
	if(photodoc->currentTarget)
	{						  	
		if(scaleBox)
		{
			//photodoc->targetphoto->eraseBox();
			boxcorner->hide();
			scaleBox = false;
		}
		else
		{
			boxcorner->show();
			scaleBox = true;
		}		
	}
}

} // namespace Spycraft
