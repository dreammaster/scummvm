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

#include "spycraft/game/image.h"
#include "spycraft/game/file.h"
#include "spycraft/game/flag.h"
#include "spycraft/game/mixmatch.h"
#include "spycraft/game/html.h"
#include "spycraft/dmade/adveff.h"
#include "spycraft/game/puzztest.h"
#include "spycraft/game/email.h"
#include "spycraft/game/pcscreen.h"
#include "spycraft/game/knee.h"
#include "spycraft/game/colby.h"
#include "spycraft/logic/60000.h"

namespace Spycraft {

//bool mailFlags[5][80];

extern "C" {
	void copyRect(SRect *dest, SRect *src)
	{
		dest->left = src->left;
		dest->right = src->right;
		dest->top = src->top;
		dest->bottom = src->bottom; 
		dest->width	= src->width;
		dest->height = src->height;
	}
	
	void CleanUpSprites();
}
    
PictureButton* picBut[MAX_PICS];

Picture* currentPicture;
Picture* blendPicture;
int blendViews[MAX_PICS][6];

ImageAnal* imageanal;
BlendBox* blendbox;
ImageCursor *imageCursor;
extern BlinkCursor* normalCursor;
unsigned short numPics;
ImageInfo *imageinfo;
bool scanned;
ImageEnhance*	OCEbutt;
WebReport* imagereport;

bool purgeRes;

/***********************************
 *		
 *		 Voice over sound object
 *		      
 **********************************/
void ImageVoice::playStream(int soundNumber)
{
	if ( isPlaying() )
	{
		stop();
	}
	playBuffered(soundNumber, NULL);
}

void ImageVoice::play(int soundNumber)
{
	if ( isPlaying() )
	{
		stop();
	}
	Sound::play(soundNumber, NULL);
}

void ImageVoice::play(int soundNumber, Object* whoToCue)
{
	if ( isPlaying() )
	{
		stop();
	}
	Sound::play(soundNumber, whoToCue);
}
/***********************************
 *		
 *		 SFX sound object
 *		      
 **********************************/
void ImageSFX::playStream(int soundNumber)
{
	if ( isPlaying() )
		stop();
 	playBuffered(soundNumber, NULL);
}

void ImageSFX::play(int soundNumber)
{
	if ( isPlaying() )
		stop();
 	Sound::play(soundNumber, NULL);
}

/***********************************
 *		
 *		 Puzzle Save State
 *		      
 **********************************/
ImageInfo::ImageInfo()
{
	name = "imageinfo";
	web_intrface->addToFront(this); 
	imageinfo = this;
}

void ImageInfo::save()
{
	numPictures = numPics;
	gotBlend = false;
	for(int i = 0; i < numPics; i++)
	{
		int j;
		theX[i] = picBut[i]->x;
		theY[i] = picBut[i]->y;	
		theView[i] = picBut[i]->view;
		theLoop[i] = picBut[i]->loop;
		picNum[i] = 1;
		zoomPos[i] = picBut[i]->picture->currentZoom;
		zoomQuad[i] = picBut[i]->picture->currentQuad;
		
		numtextobjs = picBut[i]->picture->numTextObjs;
	
		for(j = 0; j < picBut[i]->picture->numTextObjs; j++)
		{
			textobjs[j].quad = picBut[i]->picture->textObjList[j].quad;
			textobjs[j].left = picBut[i]->picture->textObjList[j].left;
			textobjs[j].top = picBut[i]->picture->textObjList[j].top;
			textobjs[j].right = picBut[i]->picture->textObjList[j].right;
			textobjs[j].bottom = picBut[i]->picture->textObjList[j].bottom;
			textobjs[j].view = picBut[i]->picture->textObjList[j].view;
			textobjs[j].loop = picBut[i]->picture->textObjList[j].loop;
		}

		for(j = 0; j < 6; j++)
		{
			picZoomViews[i][j] = picBut[i]->picture->zoomViews[j];
		}

		for(j = 0; j < picBut[i]->picture->currentZoom; j++)
		{
			copyRect(&theZooms[i][j], &picBut[i]->picture->zoomList[j]);
		}
	}

	if(blendPicture)
	{
		int j;
		gotBlend = true;
		theView[3] = blendPicture->zoomViews[0];//;photoSprite->view;
		picNum[3] = 0;//blendPicture->photoSprite->loop;
		zoomPos[3] = blendPicture->currentZoom;
		zoomQuad[3] = blendPicture->currentQuad;
		
		numtextobjs = blendPicture->numTextObjs;
									    
		for(j = 0; j < blendPicture->numTextObjs; j++)
		{
			textobjs[j].quad = blendPicture->textObjList[j].quad;
			textobjs[j].left = blendPicture->textObjList[j].left;
			textobjs[j].top = blendPicture->textObjList[j].top;
			textobjs[j].right = blendPicture->textObjList[j].right;
			textobjs[j].bottom = blendPicture->textObjList[j].bottom;
			textobjs[j].view = blendPicture->textObjList[j].view;
			textobjs[j].loop = blendPicture->textObjList[j].loop;
		}

		for(j = 0; j < 6; j++)
		{
			picZoomViews[3][j] = blendPicture->zoomViews[j];
		}

		for(j = 0; j < blendPicture->currentZoom; j++)
		{
			copyRect(&theZooms[3][j], &blendPicture->zoomList[j]);
		}
	}
	else
	{
		for(int i = 0; i < MAX_PICS; i++)
		{
			if(currentPicture == picBut[i]->picture)
			{
				currentPhoto = i;
				break;
			}		 	
		}
	}		
}

void ImageInfo::load()
{
	Sprite *tempSprite;
	SRect newRect;
	int i;

	numPics = numPictures;
	for(i = 0; i < numPics; i++)
	{
		picBut[i] = new(PictureButton);
		picBut[i]->init(theX[i], theY[i], theView[i], theLoop[i], picNum[i]);
		picBut[i]->picture->setATSQuads(picZoomViews[i][0], picZoomViews[i][1], picZoomViews[i][2], picZoomViews[i][3], picZoomViews[i][4], picZoomViews[i][5]);
		picBut[i]->picture->numTextObjs = numtextobjs;
		picBut[i]->picture->initTextObjs(textobjs, numtextobjs);	
		picBut[i]->picture->currentZoom = zoomPos[i];
		picBut[i]->picture->currentQuad = zoomQuad[i];
		for(int j = 0; j < picBut[i]->picture->currentZoom; j++)
		{
			copyRect(&picBut[i]->picture->zoomList[j], &theZooms[i][j]);
		}
	}	

	if(gotBlend)
	{
		
		blendPicture = new(Picture);
		blendPicture->init(theView[3], picNum[3], 0, NOSCALE);
		blendPicture->setATSQuads(picZoomViews[3][0], picZoomViews[3][1], picZoomViews[3][2], picZoomViews[3][3], picZoomViews[3][4], picZoomViews[3][5]);
		blendPicture->zoomViews[0] = picZoomViews[3][0];
		//blendPicture->numTextObjs = numtextobjs;
		//picBut[i]->picture->initTextObjs(textobjs, numtextobjs);	
		blendPicture->currentZoom = zoomPos[3];
		blendPicture->currentQuad = zoomQuad[3];


		if(blendPicture->currentZoom > 0)
		{
			copyRect(&blendPicture->zoomList[0], &theZooms[3][0]);
		//	blendPicture->photoSprite->dispose();
			blendPicture->init(blendPicture->zoomViews[blendPicture->currentQuad], 0, 0, HALFSCALE);

			for(int j = 1; j < zoomPos[3]; j++)
			{
				copyRect(&blendPicture->zoomList[j], &theZooms[3][j]);

				sfxSetDrawInfo (ADV_DRAW_SCALEX, 1024);
				sfxSetDrawInfo (ADV_DRAW_SCALEY, 1024);
				SRect_Init ( &newRect, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT);
				tempSprite = sfxClipSprite ( blendPicture->photoSprite->mySprite, 0, 0, &newRect);
				sfxKillSprite(blendPicture->photoSprite->mySprite);
				CleanUpSprites();
				blendPicture->photoSprite->mySprite = tempSprite;

				sfxSetDrawInfo (ADV_DRAW_SCALEX, 512 );
				sfxSetDrawInfo (ADV_DRAW_SCALEY, 512 );
				tempSprite = sfxClipSprite ( blendPicture->photoSprite->mySprite, 16, 89, &theZooms[3][j] );
				sfxKillSprite(blendPicture->photoSprite->mySprite);	      
				blendPicture->photoSprite->mySprite = tempSprite;
			}
		}
		else
		{
			blendPicture->init(blendPicture->zoomViews[0], 0, 0, NOSCALE);
		}
		currentPicture = blendPicture;
	}
	else
	{
		i = currentPhoto;

		if(picBut[i]->picture->currentZoom > 0)
		{
			copyRect(&picBut[i]->picture->zoomList[0], &theZooms[i][0]);
			//picBut[i]->picture->photoSprite->dispose();
			picBut[i]->picture->init(picBut[i]->picture->zoomViews[picBut[i]->picture->currentQuad], 0, 0, HALFSCALE);

			for(int j = 1; j < zoomPos[i]; j++)
			{
				copyRect(&picBut[i]->picture->zoomList[j], &theZooms[i][j]);

				sfxSetDrawInfo (ADV_DRAW_SCALEX, 1024);
				sfxSetDrawInfo (ADV_DRAW_SCALEY, 1024);
				SRect_Init ( &newRect, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT);
				tempSprite = sfxClipSprite ( picBut[i]->picture->photoSprite->mySprite, 0, 0, &newRect);
				sfxKillSprite(picBut[i]->picture->photoSprite->mySprite);
				CleanUpSprites();
				picBut[i]->picture->photoSprite->mySprite = tempSprite;

				sfxSetDrawInfo (ADV_DRAW_SCALEX, 512 );
				sfxSetDrawInfo (ADV_DRAW_SCALEY, 512 );
				tempSprite = sfxClipSprite ( picBut[i]->picture->photoSprite->mySprite, 16, 89, &theZooms[i][j] );
				sfxKillSprite(picBut[i]->picture->photoSprite->mySprite);	      
				picBut[i]->picture->photoSprite->mySprite = tempSprite;
			}
		}
		else
		{
			picBut[i]->picture->init(picBut[i]->picture->zoomViews[0], 0, 0, NOSCALE);
		}
		currentPicture = picBut[i]->picture;
	}

	currentPicture->photoSprite->show();
}

void ImageInfo::dispose()
{
	web_intrface->release ( this );
	deathList->add(this);
}
			   
/***********************************
 *		
 *		   Image License Plate
 *		      
 **********************************/
ImageAnalPlate::ImageAnalPlate()
{
	IntrPuzzPage::init(WEB_IMAGEPLATE);
	helppage = 92250;

	TextObj objs[8] = {{BOTTOMRIGHT, 138, 270, 0, 0, 157, 0/*brown car right*/}, {MIDDLE, 375, 276, 0, 0, 157, 1/*brown car middle*/}, 
					   {BOTTOMRIGHT, 249, 222, 0, 0, 157, 3/*bluevan right*/}, {MIDDLE, 653, 159, 0, 0, 157, 5/*Blue van middle*/},
					   {MIDDLE, 210, 400, 0, 0, 157, 2 /*toilet truck*/}, {BOTTOMRIGHT, 75, 319, 0, 0, 157, 6 /*toilet truck*/},
					   {TOPRIGHT, 50, 284, 0, 0, 157, 4/*diner*/}, {BOTTOMLEFT, 629, 272, 0, 0, 157, 7/*RV*/}};

	int numObjs = 8;
	numPics = 1;
	GameFlag.set(fPatsHTM_119);
	homobutt->setCel(2);
	if(!intrpuzz->isStateSaved("imageinfo"))
	{
		sfxSystemCursor(SYS_WAIT);
		user->input = false;

	//	sfxLoadRes(40020, RES_WAVE);
		picBut[0] = new(PictureButton);
		picBut[0]->init(444, 76, 178, 0, 1);
		
		if(GameFlag.test(fGreg39))
		{
				picBut[0]->picture->setATSQuads(351, 352, 353, 354, 355, 356);
				currentPicture = picBut[0]->picture;
				currentPicture->init(351, 0, 0, NOSCALE);
		}
		else
		{
			picBut[0]->picture->setATSQuads(151, 152, 153, 154, 155, 156);
			currentPicture = picBut[0]->picture;
			currentPicture->init(151, 0, 0, NOSCALE);
		}
//		currentPicture->initTextObjs(objs, numObjs);	

		setScript(new ImageStartScript);
	}
	else
	{
		imageinfo->load();
	}
	currentPicture->initTextObjs(objs, numObjs);	

	webreport->hide();
	imagereport = new(PlateSendBut);
	((PlateSendBut *)imagereport)->setFlags();
	init();
}
			   
/***********************************
 *		
 *		   Image start script
 *		      
 **********************************/
void ImageStartScript::changeState ( int newState )
{
	switchTo
		imageanal->imageVoice->canInterupt = true;
		imageanal->imageVoice->playBuffered(40020, this);
	END											  
	BEG
	    ticks = 8;
	END
	BEG
		imageanal->imageVoice->canInterupt = true;
		imageanal->imageVoice->playBuffered(40133, this);
	END
	BEG
    	ticks = 8;
	END
	BEG
		sfxMadeCursor();
		user->input = true;
		dispose();
	END
}

/***********************************
 *		
 *		   Image number of Tanks
 *		      
 **********************************/
ImageAnalTank::ImageAnalTank()
{
	IntrPuzzPage::init(WEB_IMAGETANK);
	helppage = 92250;

	if(GameFlag.test(fGreg39))
			setBlendViews(0, 371, 372, 373, 374, 375, 376);
	else
		setBlendViews(0, 171, 172, 173, 174, 175, 176);
			

	GameFlag.set(fPatsHTM_120);
//	backbutton = false;
	homobutt->setCel(2);
	if(!intrpuzz->isStateSaved("imageinfo"))
	{
		sfxSystemCursor(SYS_WAIT);
		user->input = false;
 		//int numObjs = 0;

		numPics = 2;
			
		picBut[1] = new(PictureButton);
		picBut[1]->init(444, 122, 177, 1, 1);

		if(GameFlag.test(fGreg39))
			picBut[1]->picture->setATSQuads(364, 365, 366, 367, 368, 369);
		else
			picBut[1]->picture->setATSQuads(164, 165, 166, 167, 168, 169);
					
		picBut[0] = new(PictureButton);
		picBut[0]->init(444, 76, 177, 0, 1);

		if(GameFlag.test(fGreg39))
		{
			picBut[0]->picture->setATSQuads(358, 359, 360, 361, 362, 363);
			currentPicture = picBut[0]->picture;
			currentPicture->init(358, 0, 0, NOSCALE);
		}
		else
		{
			picBut[0]->picture->setATSQuads(158, 159, 160, 161, 162, 163);
			currentPicture = picBut[0]->picture;
			currentPicture->init(158, 0, 0, NOSCALE);
		}
						
		setScript(new ImageTankScript);
	}
	else
	{
		imageinfo->load();
	}
	webreport->hide();
	imagereport = new(TankSendBut);
	init();
}

void ImageTankScript::changeState ( int newState )
{
	switchTo
		imageanal->imageVoice->canInterupt = true;
		imageanal->imageVoice->playBuffered(40135, this);
	END											  
	BEG
		ticks = 8;
	END
	BEG
		sfxMadeCursor();
		user->input = true;
		dispose();
	END
}

/***********************************
 *		
 *		   Image find the courier
 *		      
 **********************************/
ImageAnalCourier::ImageAnalCourier()
{
	IntrPuzzPage::init(WEB_IMAGECOURIER);
	helppage = 92250;

	setBlendViews(0, 192, 193, 194, 195, 196, 197);
	setBlendViews(1, 121, 122, 123, 124, 125, 126);
	setBlendViews(2, 115, 116, 117, 118, 119, 120);
	if(!intrpuzz->isStateSaved("imageinfo"))
	{
//		TextObj objs[5] = {{BOTTOMRIGHT, 140, 274, "JSK-737"}, {TOPRIGHT, 60, 299, "Mustang Diner"}, {BOTTOMLEFT, 631, 275, "ash-456"}};
 		//int numObjs = 0;
		numPics = 3;

		imageanal->imageVoice->play(40136);

		picBut[2] = new(PictureButton);
		picBut[2]->init(444, 168, 199, 0, 1);
		picBut[2]->picture->setATSQuads(199, 110, 111, 112, 113, 114);
		picBut[1] = new(PictureButton);
		picBut[1]->init(444, 122, 185, 0, 1);
		picBut[1]->picture->setATSQuads(185, 186, 187, 188, 189, 190);
		picBut[0] = new(PictureButton);
		picBut[0]->init(444, 76, 178, 0, 1);
		picBut[0]->picture->setATSQuads(178, 179, 180, 181, 182, 183);
		currentPicture = picBut[0]->picture;
		currentPicture->init(178, 0, 0, NOSCALE);
	}
	else
	{
		imageinfo->load();
	}
	new(AmesMixMatch);
	init();
}


/***********************************
 *		
 *		   Image Peg ID
 *		      
 **********************************/
ImageAnalPeg::ImageAnalPeg()
{
	IntrPuzzPage::init(WEB_IMAGEPEG);
	helppage = 92250;

	GameFlag.set(fGreg11);

	if(!intrpuzz->isStateSaved("imageinfo"))
	{
 		//int numObjs = 0;

		pcLastUsed[PC_IA] = WEB_IMAGEPEG;
		numPics = 1;
				
		picBut[0] = new(PictureButton);
		picBut[0]->init(444, 76, 179, 0, 1);

		if(GameFlag.test(fGreg39))
		{
			picBut[0]->picture->setATSQuads(301, 302, 303, 304, 305, 306);
			currentPicture = picBut[0]->picture;
			currentPicture->init(301, 0, 0, NOSCALE);
		}
		else
		{
			picBut[0]->picture->setATSQuads(101, 102, 103, 104, 105, 106);
			currentPicture = picBut[0]->picture;
			currentPicture->init(101, 0, 0, NOSCALE);
		}
	}
	else
	{
		imageinfo->load();
	}
	webreport->hide();
	imagereport = new(PegSendBut);
	init();
}

/***********************************
 *		
 *		   Image Id KneeCaps
 *		      
 **********************************/
ImageAnalCaps::ImageAnalCaps()
{
	IntrPuzzPage::init(WEB_IMAGEKNEEID);
	helppage = 92250;
	GameFlag.set(fGreg16);

	if(!intrpuzz->isStateSaved("imageinfo"))
	{
		pcLastUsed[PC_IA] = WEB_IMAGEKNEEID;
 		//int numObjs = 0;
		numPics = 1;

		picBut[0] = new(PictureButton);
		picBut[0]->init(444, 76, 180, 0, 1);
		
		if(GameFlag.test(fGreg39))
		{
			picBut[0]->picture->setATSQuads(327, 328, 329, 330, 331, 332);
			currentPicture = picBut[0]->picture;
			currentPicture->init(327, 0, 0, NOSCALE);
		}
		else
		{			
			picBut[0]->picture->setATSQuads(127, 128, 129, 130, 131, 132);
			currentPicture = picBut[0]->picture;
			currentPicture->init(127, 0, 0, NOSCALE);
		}
	}
	else
	{
		imageinfo->load();
	}
	init();
	webreport->hide();
	imagereport = new(CapsSendBut);
}

/***********************************
 *		
 *		   Puzzle Page
 *		      
 **********************************/
ImageAnal::ImageAnal()
{
	imageanal = this;
	picBut[0] = NULL;
	picBut[1] = NULL;
 	picBut[2] = NULL;
	scanned = false;

	sfxLoadRes(129, RES_WAVE); //preload scanning sound 78k

	purgeRes = false;
	imageVoice	= new(ImageVoice);
	imageSFX	= new(ImageSFX);	 
	new(ImageBack);
	blendPicture = NULL;
	currentPicture = NULL;
	if(!intrpuzz->isStateSaved("imageinfo"))
	 	memset (&mailFlags,0,(5 * 80)*sizeof(bool));
}

void ImageAnal::init()
{
	new(ImageEnhance);
	if(currentPicture->currentZoom == MAX_ZOOMS)
		OCEbutt->setCel(0);
	else
		OCEbutt->setCel(2);

	new(unZoomBut);
	new(ZoomArea);

	if(numPics > 1)
	{
		new(BlendBut);
	}
	imageanal->zoomMode = ZOOM_IN;
}

void ImageAnal::setBlendViews(int blendNum, int firstView, int topLeft, int topRight, int bottomLeft, int bottomRight, int middle)
{
	blendViews[blendNum][0] = firstView;
	blendViews[blendNum][1] = topLeft;
	blendViews[blendNum][2] = topRight;
	blendViews[blendNum][3] = bottomLeft;
	blendViews[blendNum][4] = bottomRight;
	blendViews[blendNum][5] = middle;
}					 

void ImageAnal::saveState()
{
		if(!intrpuzz->isStateSaved("imageinfo"))
		{
			new(ImageInfo);
		}		
		imageinfo->save();
}	

void ImageAnal::dispose()
{
	imageVoice->caller = 0;
	imageVoice->dispose();
	imageSFX->caller = 0;
	imageSFX->dispose();

	if(web_intrface)//we are not quitting the web
	{
		imagereport->dispose(); 
		webreport->show();
	}

	sfxMadeCursor();  //added in case player exits while the wait cursor is up
	IntrPuzzPage::dispose();
}		
/***********************************
 *		
 *			 Cursors
 *		      
 **********************************/
ImageCursor::ImageCursor()
{
	name = "ImageCursor";
	view = 150;
	loop = 11;
	cel = 0;
	orig_pt.x = mouse_x;
	orig_pt.y = mouse_y;
	theGame->setCursor(this);
}

ImageCursor::~ImageCursor()
{
	imageCursor = NULL;
}

/***********************************
 *		
 *			 ScanLine Actor
 *		      
 **********************************/
ScanLineA::ScanLineA()
{
	SRect origRect;
	int i;
	int factor;
	int table[] = { 40, 50, 55, 50, 55, 50, 55};
	bool leftIn;
	bool rightIn;
	bool topIn;
	bool bottomIn;
	clipped = false;

	user->input = false;
	sfxSystemCursor(SYS_WAIT);
	
	view = 150;
	loop = 0;
	cel = 1;
	x = 17;
	y = 93;
	IntrPuzzView::init(17, 93);
	setPri(750);

	if(purgeRes)
	{
		sfxPurgeRes(157, RES_ATS);
	}

	imageanal->imageSFX->loop = 30;
	imageanal->imageSFX->play(129);	//scanning sound

	origRect.width = 96;
	origRect.height = 67;
	origRect.left = currentPicture->zoomList[1].left;
	origRect.top = currentPicture->zoomList[1].top;

	for(i = 2, factor = 1; i < MAX_ZOOMS; i++)
	{
		origRect.left += (currentPicture->zoomList[i].left / 2) / factor;
		origRect.top += (currentPicture->zoomList[i].top  / 2 ) / factor;
		factor += factor;
	}
	for(i = 2, factor = 1; i < MAX_ZOOMS; i++)
	{
		origRect.width = ((currentPicture->zoomList[i].right - currentPicture->zoomList[i].left) / 2) / factor;
		origRect.height = ((currentPicture->zoomList[i].bottom - currentPicture->zoomList[i].top) / 2 ) / factor;
		factor += factor;
	}
	origRect.right = origRect.width + origRect.left;
	origRect.bottom = origRect.height + origRect.top;

	for(i = 0, currentPicture->numTextSprites = 0; i < currentPicture->numTextObjs; i++)
	{
		leftIn = false;
		rightIn = false;
		topIn = false;
		bottomIn = false;
		IntrPuzzView *tempSprite;
			
		//we need this to get the width and the height of the sprite
		tempSprite = new (IntrPuzzView);
		tempSprite->init(0, 0, currentPicture->textObjList[i].view, currentPicture->textObjList[i].loop, 0);
		tempSprite->hide();
		currentPicture->textObjList[i].right = currentPicture->textObjList[i].left + (tempSprite->width / 4);
		currentPicture->textObjList[i].bottom = currentPicture->textObjList[i].top + (tempSprite->height / 4);
		tempSprite->dispose();

		if(currentPicture->textObjList[i].quad == currentPicture->currentQuad)
		{
			if((currentPicture->textObjList[i].left > origRect.left) && (currentPicture->textObjList[i].left < origRect.right))
			{
				leftIn = true;
			}
			if((currentPicture->textObjList[i].right < origRect.right) && (currentPicture->textObjList[i].right > origRect.left))				
			{
				rightIn = true;
			}
			if((currentPicture->textObjList[i].top > origRect.top) && (currentPicture->textObjList[i].top < origRect.bottom))				
			{
				topIn = true;
			}
			if((currentPicture->textObjList[i].bottom > origRect.top) && (currentPicture->textObjList[i].bottom < origRect.bottom))				
			{
				bottomIn = true;
			}

			if((leftIn || rightIn) && (topIn || bottomIn))
			{
				int index = currentPicture->numTextSprites;
				textFound[index].left = 0;
				textFound[index].right = 0;
				textFound[index].top = 0;
				textFound[index].bottom = 0;
 				textFound[index].clipped = false;

				if(!leftIn)
				{
					textFound[index].left = (origRect.left - currentPicture->textObjList[i].left) * 4;
					textFound[index].clipped = true;
				}
				if(!rightIn)
				{
					textFound[index].right = (currentPicture->textObjList[i].right - origRect.right) * 4;
					textFound[index].clipped = true;
				}
				if(!topIn)
				{
					textFound[index].top = (origRect.top - currentPicture->textObjList[i].top) * 4;
					textFound[index].clipped = true;
				}
				if(!bottomIn)
				{
					textFound[index].bottom = (currentPicture->textObjList[i].bottom - origRect.bottom) * 4;
					textFound[index].clipped = true;
				}
			
				int theX;
				int theY;
				if(textFound[index].left)
				{
					theX = 0;
				}
				else
				{
					theX = (currentPicture->textObjList[i].left - origRect.left) * 4;
				}

				if(textFound[index].top)
				{
					theY = 0;
				}
				else
				{
					theY = (currentPicture->textObjList[i].top - origRect.top) * 4;
				}

				textFound[index].x = ORIGIN_X + theX;
				textFound[index].y = ORIGIN_Y + theY;
				textFound[index].view = currentPicture->textObjList[i].view;
				textFound[index].loop = currentPicture->textObjList[i].loop;
				currentPicture->numTextSprites++;
				mailFlags[0][i] = true;
				((PlateSendBut*)imagereport)->setFlags();
			}			
		}
	}

	//preload the Text sprites
	for(i = 0; i < currentPicture->numTextSprites; i++)
	{
		currentPicture->textSprites[i] = new (IntrPuzzView);
		currentPicture->textSprites[i]->init(textFound[i].x, textFound[i].y, textFound[i].view, textFound[i].loop, 0);
		currentPicture->textSprites[i]->hide();

		//int theHeight = currentPicture->textSprites[i]->height - 1;
		for(int j = 0; j < currentPicture->textSprites[i]->height; j++)
		{
			sfxLPFilter ( currentPicture->textSprites[i]->mySprite->port, j, table, 7, /*__SFX_LPFILTER_GREEN | __SFX_LPFILTER_RED |*/ __SFX_LPFILTER_BLUE);
			currentPicture->textSprites[i]->mySprite->changed = true;
		}
	}

 	currentItem = 0;
	Sprite *s;
    photoHeight = currentPicture->photoSprite->mySprite->port->height;
	s = currentPicture->photoSprite->mySprite;
		 	
	for (scanLineCtr = 0; scanLineCtr<s->port->height; scanLineCtr++ )
	{
		sfxLPFilter ( s->port, scanLineCtr, table, 7, /*__SFX_LPFILTER_GREEN | __SFX_LPFILTER_RED |*/ __SFX_LPFILTER_BLUE);	 
		s->changed = true;
		posn(17, 93 + scanLineCtr);
		sfxReleaseSprites ( backgrounds[curBack] );
		sfxUpdate();
							  
		if(currentItem != currentPicture->numTextSprites)
		{
			if(textFound[currentItem].y == (scanLineCtr + 93))
			{
				if(currentItem <= currentPicture->numTextSprites)
				{
					SRect clipRect;
					Sprite* temp;
					SRect_Init ( &clipRect, 0, 0, 0, 0);
					if(textFound[currentItem].clipped)
					{
						currentPicture->textSprites[currentItem]->hide();
						clipRect.left = textFound[currentItem].left;
						clipRect.top = textFound[currentItem].top;
						if(textFound[currentItem].right)
						{
							clipRect.right = currentPicture->textSprites[currentItem]->width - textFound[currentItem].right;
						}
						else
						{		  
							clipRect.right = currentPicture->textSprites[currentItem]->width;
						}				
						if(textFound[currentItem].bottom)
						{
							clipRect.bottom = currentPicture->textSprites[currentItem]->height - textFound[currentItem].bottom;
						}
						else
						{
							clipRect.bottom = currentPicture->textSprites[currentItem]->height;
						}
						temp = sfxClipSprite ( currentPicture->textSprites[currentItem]->mySprite, textFound[currentItem].x, textFound[currentItem].y, &clipRect);					
						sfxKillSprite(currentPicture->textSprites[currentItem]->mySprite);
						currentPicture->textSprites[currentItem]->mySprite = temp;
						currentPicture->textSprites[currentItem]->setRect();	
					}
				
					int theHeight = currentPicture->textSprites[currentItem]->height;
					temp = NULL;
					scanLineCtr++;
					for(i = 1; i < theHeight; i++, scanLineCtr++)
					{
					
						SRect_Init ( &clipRect, 0, 0, 0, 0);
						clipRect.left = 0;	
						clipRect.top = 0;	
						clipRect.right = currentPicture->textSprites[currentItem]->width;	
						clipRect.bottom = i;
						sfxSetDrawInfo (ADV_DRAW_SCALEX, 512 );
						sfxSetDrawInfo (ADV_DRAW_SCALEY, 512 );
						if(temp)
							sfxKillSprite(temp);
						temp = sfxClipSprite ( currentPicture->textSprites[currentItem]->mySprite, textFound[currentItem].x, textFound[currentItem].y, &clipRect);					
						currentPicture->textSprites[currentItem]->hide();
						sfxShowSprite(temp);
					
						sfxLPFilter ( s->port, scanLineCtr, table, 7, /*__SFX_LPFILTER_GREEN | __SFX_LPFILTER_RED |*/ __SFX_LPFILTER_BLUE);
						//sfxLPFilter ( currentPicture->textSprites[currentItem]->mySprite->port, i, table, 7, /*__SFX_LPFILTER_GREEN | __SFX_LPFILTER_RED |*/ __SFX_LPFILTER_BLUE);
						s->changed = true;
						//currentPicture->textSprites[currentItem]->mySprite->changed = true;
						posn(17, 93 + scanLineCtr);
						sfxReleaseSprites ( backgrounds[curBack] );
						sfxUpdate();
					}							 
					scanLineCtr--;
					if(temp)
						sfxKillSprite(temp);
					currentPicture->textSprites[currentItem]->show();
					
					currentItem++;
				}
			}					    
		}		    
	}

	dispose();
	imageanal->imageSFX->stop();	//stop scanning sound
	user->input = true;
	sfxMadeCursor();
}

void ScanLineA::cue()
{
	int table[] = { 40, 50, 55, 50, 55, 50, 55};
 
 	currentItem = 0;
	Sprite *s;
    photoHeight = currentPicture->photoSprite->mySprite->port->height;
	s = currentPicture->photoSprite->mySprite;
		 	
	if(scanLineCtr < photoHeight)
	{
		sfxLPFilter ( s->port, scanLineCtr, table, 7, /*__SFX_LPFILTER_GREEN | __SFX_LPFILTER_RED |*/ __SFX_LPFILTER_BLUE);
		s->changed = true;
		posn(17, 93 + scanLineCtr);
		sfxReleaseSprites ( backgrounds[curBack] );
		sfxUpdate();
							  
		if(currentItem != currentPicture->numTextSprites)
		{
			if(textFound[currentItem].y == (scanLineCtr + 93))
			{
				if(currentItem <= currentPicture->numTextSprites)
				{
					SRect clipRect;
					SRect_Init ( &clipRect, 0, 0, 0, 0);
					if(textFound[currentItem].clipped)
					{
						currentPicture->textSprites[currentItem]->hide();
						clipRect.left = textFound[currentItem].left;
						clipRect.top = textFound[currentItem].top;
						if(textFound[currentItem].right)
						{
							clipRect.right = currentPicture->textSprites[currentItem]->width - textFound[currentItem].right;
						}
						else
						{		  
							clipRect.right = currentPicture->textSprites[currentItem]->width;
						}				
						if(textFound[currentItem].bottom)
						{
							clipRect.bottom = currentPicture->textSprites[currentItem]->height - textFound[currentItem].bottom;
						}
						else
						{
							clipRect.bottom = currentPicture->textSprites[currentItem]->height;
						}
						currentPicture->textSprites[currentItem]->mySprite = sfxClipSprite ( currentPicture->textSprites[currentItem]->mySprite, textFound[currentItem].x, textFound[currentItem].y, &clipRect);					
						currentPicture->textSprites[currentItem]->setRect();	
					}
				
					int theHeight = currentPicture->textSprites[currentItem]->height;
					Sprite* temp = NULL;
					scanLineCtr++;
					for(int i = 1; i < theHeight; i++, scanLineCtr++)
					{
					
						SRect_Init ( &clipRect, 0, 0, 0, 0);
						clipRect.left = 0;	
						clipRect.top = 0;	
						clipRect.right = currentPicture->textSprites[currentItem]->width;	
						clipRect.bottom = i;
						sfxSetDrawInfo (ADV_DRAW_SCALEX, 512 );
						sfxSetDrawInfo (ADV_DRAW_SCALEY, 512 );
						if(temp)
							sfxKillSprite(temp);
						temp = sfxClipSprite ( currentPicture->textSprites[currentItem]->mySprite, textFound[currentItem].x, textFound[currentItem].y, &clipRect);					
						currentPicture->textSprites[currentItem]->hide();
						sfxShowSprite(temp);
					
						sfxLPFilter ( s->port, scanLineCtr, table, 7, /*__SFX_LPFILTER_GREEN | __SFX_LPFILTER_RED |*/ __SFX_LPFILTER_BLUE);
						//sfxLPFilter ( currentPicture->textSprites[currentItem]->mySprite->port, i, table, 7, /*__SFX_LPFILTER_GREEN | __SFX_LPFILTER_RED |*/ __SFX_LPFILTER_BLUE);
						s->changed = true;
						//currentPicture->textSprites[currentItem]->mySprite->changed = true;
						posn(17, 93 + scanLineCtr);
						sfxReleaseSprites ( backgrounds[curBack] );
						sfxUpdate();
					}							 
					if(temp)
						sfxKillSprite(temp);
					currentPicture->textSprites[currentItem]->show();
					
					currentItem++;
				}
			}					    
		}
		scanLineCtr++;
	 	scanTimer = new (IntrPuzzTimer);
		scanTimer->setTicks(1,this);
	}
	else
	{
		imageanal->imageSFX->stop();	//stop scanning sound
		dispose();
	}

}

ScanLineDos8::ScanLineDos8()
{
	SRect origRect;
	int i;
	int factor;
	int table[] = { 40, 50, 55, 50, 55, 50, 55};
	bool leftIn;
	bool rightIn;
	bool topIn;
	bool bottomIn;
	clipped = false;
	clippingPlate = false;

	user->input = false;
	sfxSystemCursor(SYS_WAIT);
	
	view = 150;
	loop = 0;
	cel = 1;
	x = 17;
	y = 93;
	IntrPuzzView::init(17, 93);
	setPri(750);

	if(purgeRes)
	{
		sfxPurgeRes(157, RES_ATS);
	}

	imageanal->imageSFX->loop = 30;
	imageanal->imageSFX->play(129);	//scanning sound

	origRect.width = 96;
	origRect.height = 67;
	origRect.left = currentPicture->zoomList[1].left;
	origRect.top = currentPicture->zoomList[1].top;

	for(i = 2, factor = 1; i < MAX_ZOOMS; i++)
	{
		origRect.left += (currentPicture->zoomList[i].left / 2) / factor;
		origRect.top += (currentPicture->zoomList[i].top  / 2 ) / factor;
		factor += factor;
	}
	for(i = 2, factor = 1; i < MAX_ZOOMS; i++)
	{
		origRect.width = ((currentPicture->zoomList[i].right - currentPicture->zoomList[i].left) / 2) / factor;
		origRect.height = ((currentPicture->zoomList[i].bottom - currentPicture->zoomList[i].top) / 2 ) / factor;
		factor += factor;
	}
	origRect.right = origRect.width + origRect.left;
	origRect.bottom = origRect.height + origRect.top;

	for(i = 0, currentPicture->numTextSprites = 0; i < currentPicture->numTextObjs; i++)
	{
		leftIn = false;
		rightIn = false;
		topIn = false;
		bottomIn = false;
		IntrPuzzView *tempSprite;
			
		//we need this to get the width and the height of the sprite
		tempSprite = new (IntrPuzzView);
		tempSprite->init(0, 0, currentPicture->textObjList[i].view, currentPicture->textObjList[i].loop, 0);
		tempSprite->hide();
		currentPicture->textObjList[i].right = currentPicture->textObjList[i].left + (tempSprite->width / 4);
		currentPicture->textObjList[i].bottom = currentPicture->textObjList[i].top + (tempSprite->height / 4);
		tempSprite->dispose();

		if(currentPicture->textObjList[i].quad == currentPicture->currentQuad)
		{
			if((currentPicture->textObjList[i].left > origRect.left) && (currentPicture->textObjList[i].left < origRect.right))
			{
				leftIn = true;
			}
			if((currentPicture->textObjList[i].right < origRect.right) && (currentPicture->textObjList[i].right > origRect.left))				
			{
				rightIn = true;
			}
			if((currentPicture->textObjList[i].top > origRect.top) && (currentPicture->textObjList[i].top < origRect.bottom))				
			{
				topIn = true;
			}
			if((currentPicture->textObjList[i].bottom > origRect.top) && (currentPicture->textObjList[i].bottom < origRect.bottom))				
			{
				bottomIn = true;
			}

			if((leftIn || rightIn) && (topIn || bottomIn))
			{
				int index = currentPicture->numTextSprites;
				textFound[index].left = 0;
				textFound[index].right = 0;
				textFound[index].top = 0;
				textFound[index].bottom = 0;
 				textFound[index].clipped = false;

				if(!leftIn)
				{
					textFound[index].left = (origRect.left - currentPicture->textObjList[i].left) * 4;
					textFound[index].clipped = true;
				}
				if(!rightIn)
				{
					textFound[index].right = (currentPicture->textObjList[i].right - origRect.right) * 4;
					textFound[index].clipped = true;
				}
				if(!topIn)
				{
					textFound[index].top = (origRect.top - currentPicture->textObjList[i].top) * 4;
					textFound[index].clipped = true;
				}
				if(!bottomIn)
				{
					textFound[index].bottom = (currentPicture->textObjList[i].bottom - origRect.bottom) * 4;
					textFound[index].clipped = true;
				}
			
				int theX;
				int theY;
				if(textFound[index].left)
				{
					theX = 0;
				}
				else
				{
					theX = (currentPicture->textObjList[i].left - origRect.left) * 4;
				}

				if(textFound[index].top)
				{
					theY = 0;
				}
				else
				{
					theY = (currentPicture->textObjList[i].top - origRect.top) * 4;
				}

				textFound[index].x = ORIGIN_X + theX;
				textFound[index].y = ORIGIN_Y + theY;
				textFound[index].view = currentPicture->textObjList[i].view;
				textFound[index].loop = currentPicture->textObjList[i].loop;
				currentPicture->numTextSprites++;
				mailFlags[0][i] = true;
				((PlateSendBut*)imagereport)->setFlags();
			}			
		}
	}

	//preload the Text sprites
	for(i = 0; i < currentPicture->numTextSprites; i++)
	{
		currentPicture->textSprites[i] = new (IntrPuzzView);
		currentPicture->textSprites[i]->init(textFound[i].x, textFound[i].y, textFound[i].view, textFound[i].loop, 0);
		currentPicture->textSprites[i]->hide();
	}

 	currentItem = 0;
    photoHeight = currentPicture->photoSprite->mySprite->port->height;
	scanLineCtr = 0;
			 	
 	scanTimer = new (IntrPuzzTimer);
	scanTimer->setTicks(2,this);
/*	for (scanLineCtr = 0; scanLineCtr<s->port->height; scanLineCtr++ )
	{
		posn(17, 93 + scanLineCtr);
		sfxReleaseSprites ( backgrounds[curBack] );
		sfxUpdate();
							  
		if(currentItem != currentPicture->numTextSprites)
		{
			if(textFound[currentItem].y == (scanLineCtr + 93))
			{
				if(currentItem <= currentPicture->numTextSprites)
				{
					SRect clipRect;
					Sprite* temp;
					SRect_Init ( &clipRect, 0, 0, 0, 0);
					if(textFound[currentItem].clipped)
					{
						currentPicture->textSprites[currentItem]->hide();
						clipRect.left = textFound[currentItem].left;
						clipRect.top = textFound[currentItem].top;
						if(textFound[currentItem].right)
						{
							clipRect.right = currentPicture->textSprites[currentItem]->width - textFound[currentItem].right;
						}
						else
						{		  
							clipRect.right = currentPicture->textSprites[currentItem]->width;
						}				
						if(textFound[currentItem].bottom)
						{
							clipRect.bottom = currentPicture->textSprites[currentItem]->height - textFound[currentItem].bottom;
						}
						else
						{
							clipRect.bottom = currentPicture->textSprites[currentItem]->height;
						}
						temp = sfxClipSprite ( currentPicture->textSprites[currentItem]->mySprite, textFound[currentItem].x, textFound[currentItem].y, &clipRect);					
						sfxKillSprite(currentPicture->textSprites[currentItem]->mySprite);
						currentPicture->textSprites[currentItem]->mySprite = temp;
						currentPicture->textSprites[currentItem]->setRect();	
					}
				
					int theHeight = currentPicture->textSprites[currentItem]->height;
					temp = NULL;
					scanLineCtr++;
					for(int i = 1; i < theHeight; i++, scanLineCtr++)
					{
					
						SRect_Init ( &clipRect, 0, 0, 0, 0);
						clipRect.left = 0;	
						clipRect.top = 0;	
						clipRect.right = currentPicture->textSprites[currentItem]->width;	
						clipRect.bottom = i;
						sfxSetDrawInfo (ADV_DRAW_SCALEX, 512 );
						sfxSetDrawInfo (ADV_DRAW_SCALEY, 512 );
						if(temp)
							sfxKillSprite(temp);
						temp = sfxClipSprite ( currentPicture->textSprites[currentItem]->mySprite, textFound[currentItem].x, textFound[currentItem].y, &clipRect);					
						currentPicture->textSprites[currentItem]->hide();
						sfxShowSprite(temp);
					
						posn(17, 93 + scanLineCtr);
						sfxReleaseSprites ( backgrounds[curBack] );
						sfxUpdate();
					}							 
					scanLineCtr--;
					if(temp)
						sfxKillSprite(temp);
					currentPicture->textSprites[currentItem]->show();
					
					currentItem++;
				}
			}					    
		}		    
	}

	dispose();
	imageanal->imageSFX->stop();	//stop scanning sound
	sfxMadeCursor();
*/
}

void ScanLineDos8::cue()
{
	SRect clipRect;

	if (scanLineCtr < photoHeight)
	{
		posn(17, 93 + scanLineCtr);
	  
		if(currentItem != currentPicture->numTextSprites)
		{
			if(textFound[currentItem].y == (scanLineCtr + 93))
			{
				if(currentItem <= currentPicture->numTextSprites)
				{
					Sprite* temp;
					SRect_Init ( &clipRect, 0, 0, 0, 0);
					if(textFound[currentItem].clipped)
					{
						currentPicture->textSprites[currentItem]->hide();
						clipRect.left = textFound[currentItem].left;
						clipRect.top = textFound[currentItem].top;
						if(textFound[currentItem].right)
						{
							clipRect.right = currentPicture->textSprites[currentItem]->width - textFound[currentItem].right;
						}
						else
						{		  
							clipRect.right = currentPicture->textSprites[currentItem]->width;
						}				
						if(textFound[currentItem].bottom)
						{
							clipRect.bottom = currentPicture->textSprites[currentItem]->height - textFound[currentItem].bottom;
						}
						else
						{
							clipRect.bottom = currentPicture->textSprites[currentItem]->height;
						}
						temp = sfxClipSprite ( currentPicture->textSprites[currentItem]->mySprite, textFound[currentItem].x, textFound[currentItem].y, &clipRect);					
						sfxKillSprite(currentPicture->textSprites[currentItem]->mySprite);
						currentPicture->textSprites[currentItem]->mySprite = temp;
						currentPicture->textSprites[currentItem]->setRect();	
					}
				
					plateHeight = currentPicture->textSprites[currentItem]->height;
					clipPlate = NULL;
					currentPlateScanLine = 0;
					clippingPlate = true;
				}
			}					    
		}		    

		if(clippingPlate)
		{
			if(currentPlateScanLine < plateHeight)
			{
			
				SRect_Init ( &clipRect, 0, 0, 0, 0);
				clipRect.left = 0;	
				clipRect.top = 0;	
				clipRect.right = currentPicture->textSprites[currentItem]->width;	
				clipRect.bottom = currentPlateScanLine;
				sfxSetDrawInfo (ADV_DRAW_SCALEX, 512 );
				sfxSetDrawInfo (ADV_DRAW_SCALEY, 512 );
				if(clipPlate)
					sfxKillSprite(clipPlate);
				clipPlate = sfxClipSprite ( currentPicture->textSprites[currentItem]->mySprite, textFound[currentItem].x, textFound[currentItem].y, &clipRect);					
				currentPicture->textSprites[currentItem]->hide();
				sfxShowSprite(clipPlate);
				currentPlateScanLine++;
			}							 
			else
			{
				clippingPlate = false;
				sfxKillSprite(clipPlate);
				currentPicture->textSprites[currentItem]->show();
				currentItem++;
			}
			
		}
		scanLineCtr++;
	 	scanTimer = new (IntrPuzzTimer);
		scanTimer->setTicks(2,this);
	}
	else
	{
		dispose();
		imageanal->imageSFX->stop();	//stop scanning sound
		user->input = true;
		sfxMadeCursor();
	}
}

ScanLineDos8::~ScanLineDos8()
{
	int youSuck;

	youSuck = true;
}

/***********************************
 *		
 *		  	Feature
 *		      
 **********************************/
int ZoomArea::handleEvent ( MADEEventStamp *event )
{
	if (user->input && onMe(event))
	{
		if(!imageCursor && (imageanal->zoomMode == ZOOM_IN))
		{
			imageCursor = new(ImageCursor);
		}

		if (event_type == USER_LEFT_DOWN)
		{
			SRect newRect;
			Sprite *tempSprite;

			if(imageanal->zoomMode == ZOOM_IN)
			{
			  	if(currentPicture->currentZoom == 0)
				{
					//loading in one of 5 replacement pictures
					int zoomQuad;
					int theX = mouse_x - ORIGIN_X;
					int theY = mouse_y - ORIGIN_Y;

					if(theX < 200)   //were in the left half
					{
						if(theY < 140)  //were in the top half
						{
							zoomQuad = TOPLEFT;
							newRect.left = 0;
							newRect.top = 0;
							newRect.right = 199;
							newRect.bottom = 139;
						}
						else 			//were in the bottom half
						{
							zoomQuad = BOTTOMLEFT;
							newRect.left = 0;
							newRect.top = 140;
							newRect.right = 199;
							newRect.bottom = 279;
						}
					}			
					else
					{
						if(theY < 140)  //were in the top half
						{
							zoomQuad = TOPRIGHT;
							newRect.left = 200;
							newRect.top = 0;
							newRect.right = 399;
							newRect.bottom = 139;
						}
						else 			//were in the bottom half
						{
							zoomQuad = BOTTOMRIGHT;
							newRect.left = 200;
							newRect.top = 140;
							newRect.right = 399;
							newRect.bottom = 279;
						}
					}									

					if(theX > 130 && theY > 100)   //were in the middle
					{
						if(theX < 249 && theY < 200)
						{
							zoomQuad = MIDDLE;
							newRect.left = 130;
							newRect.top = 110;
							newRect.right = 249;
							newRect.bottom = 200;
						}
					}								
					currentPicture->currentQuad = zoomQuad;
					currentPicture->photoSprite->dispose();
					//Dos 8
					if(deathList->size)
						deathList->dispose();	
					CleanUpSprites();
					
					currentPicture->init(currentPicture->zoomViews[zoomQuad], 0, 0, HALFSCALE);
					copyRect(&currentPicture->zoomList[currentPicture->currentZoom++], &newRect);
				
			 		scanned = false;	
					imageanal->imageSFX->play(112);

					flushMessageCue();
				}
				else if(currentPicture->currentZoom < MAX_ZOOMS)
				{
					//scale 2X, no clipping so pass in the full rect
					sfxSetDrawInfo (ADV_DRAW_SCALEX, 1024);
					sfxSetDrawInfo (ADV_DRAW_SCALEY, 1024);
					SRect_Init ( &newRect, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT);
					tempSprite = sfxClipSprite ( currentPicture->photoSprite->mySprite, 0, 0, &newRect);
					sfxKillSprite(currentPicture->photoSprite->mySprite);
					currentPicture->photoSprite->mySprite = tempSprite;		
					//Dos 8
					CleanUpSprites();
					//this is delaing with an 800 by 564 Rect
					//we want to cut out a 399 by 280 Rect
					newRect.left = (mouse_x - ORIGIN_X) * 2;
					newRect.top = (mouse_y - ORIGIN_Y) * 2;

					if((newRect.left - 199)	< 0 )
					{
						newRect.left = 0;
						newRect.right = 399;
					}
					else
					{
						newRect.right = newRect.left + 200;
						if(newRect.right > IMAGE_WIDTH)
						{
							newRect.right = IMAGE_WIDTH ;//795;
							newRect.left = IMAGE_WIDTH - 399;
						}
						else
						{
							newRect.left = newRect.left - 199;
						}	
					}		

					if((newRect.top - 140) < 0)
					{
						newRect.top = 0;
						newRect.bottom = 281;
					}
					else
					{
						newRect.bottom = newRect.top + 141;//was 140
						if(newRect.bottom > IMAGE_HEIGHT) //560
						{
							newRect.bottom = IMAGE_HEIGHT;
							newRect.top = IMAGE_HEIGHT - 281;
						}		
						else
						{
							newRect.top = newRect.top - 140;
						}
					}

					copyRect(&currentPicture->zoomList[currentPicture->currentZoom++], &newRect);
					//just clip this time, no scale
					sfxSetDrawInfo (ADV_DRAW_SCALEX, 512 );
					sfxSetDrawInfo (ADV_DRAW_SCALEY, 512 );
					tempSprite = sfxClipSprite ( currentPicture->photoSprite->mySprite, 16, 89, &newRect );
					sfxKillSprite(currentPicture->photoSprite->mySprite);
					currentPicture->photoSprite->mySprite = tempSprite;
					//Dos 8
					CleanUpSprites();

			 		scanned = false;	
					imageanal->imageSFX->play(112);
					if(currentPicture->currentZoom == MAX_ZOOMS)
						OCEbutt->setCel(0);

					flushMessageCue();
				}
			}
		}
		return true;
	}
	else if(imageCursor)
	{
		delete imageCursor;
		theGame->setCursor(normalCursor);
	}
	return false;
}

ZoomArea::ZoomArea()
{
	init(16, 89, 399, 280);
}

/***********************************
 *		
 *		  Blend Dialog Box
 *		      
 **********************************/
BlendBox::BlendBox()
{
	view = 150;
	loop = 7;
	cel = 0;
	numchecked = 0;
	for( int i = 1; i < 4; i++)
	{
		checked[i] = false;
	}
	init(120, 130);
	new(BlendokButton);
	new(cancelButton);
	//there are always at least 2 pictures when this box comes up
	new DialogPic(76, 42, picBut[0]->view, picBut[0]->loop, 0, 1);
	new DialogPic(76, 88, picBut[1]->view, picBut[1]->loop, 0, 2);
	if(numPics == 3)
	{
		new DialogPic(76, 134, picBut[2]->view, picBut[2]->loop, 0, 3);
	}
	noSkip();
	blendbox = this;

	imageanal->imageVoice->play(40032);
}
		
BlendokButton::BlendokButton()
{
	view = 150;
	loop = 8;
	cel = 0;
	init(7, 190);
	noSkip();
}

void BlendokButton::activate()
{
	if(blendbox->checked[1] && blendbox->checked[2])
	{
		currentPicture->photoSprite->dispose();
		//Dos 8
		if(deathList->size)
			deathList->dispose();	
	//	CleanUpSprites();

 		if(blendPicture)
		{
			blendPicture->dispose();
			blendPicture = NULL;	
		}

		blendPicture = new(Picture);
		blendPicture->setATSQuads(blendViews[0][0], blendViews[0][1], blendViews[0][2], blendViews[0][3], blendViews[0][4], blendViews[0][5]);
		currentPicture = blendPicture;
		blendPicture->init(blendViews[0][0], 0, 0, NOSCALE);
		imageanal->imageVoice->play(40033);
	}
	else if(blendbox->checked[1] && blendbox->checked[3])
	{
		currentPicture->photoSprite->dispose();

 		if(blendPicture)
		{
			blendPicture->dispose();
			blendPicture = NULL;	
		}
		//Dos 8
		if(deathList->size)
			deathList->dispose();	
	//	CleanUpSprites();

		blendPicture = new(Picture);
		blendPicture->setATSQuads(blendViews[1][0], blendViews[1][1], blendViews[1][2], blendViews[1][3], blendViews[1][4], blendViews[1][5]);
		currentPicture = blendPicture;
		blendPicture->init(blendViews[1][0], 0, 0, NOSCALE);
		imageanal->imageVoice->play(40034);
	}
	else if(blendbox->checked[2] && blendbox->checked[3])
	{
		currentPicture->photoSprite->dispose();

 		if(blendPicture)
		{
			blendPicture->dispose();
			blendPicture = NULL;	
		}
		//Dos 8
		if(deathList->size)
			deathList->dispose();	
//		CleanUpSprites();

		blendPicture = new(Picture);
		blendPicture->setATSQuads(blendViews[2][0], blendViews[2][1], blendViews[2][2], blendViews[2][3], blendViews[2][4], blendViews[2][5]);
		currentPicture = blendPicture;
		blendPicture->init(blendViews[2][0], 0, 0, NOSCALE);
		imageanal->imageVoice->play(40035);
	}
	else
	{
		imageanal->imageVoice->play(40036);
	}

	dialogbox->dispose();
}


cancelButton::cancelButton()
{
	view = 150;
	loop = 9;
	cel = 0;
	init(88, 190);
	noSkip();
}

void cancelButton::activate()
{
	dialogbox->dispose();
}

DialogPic::DialogPic(int theX, int theY, int theView, int theLoop, int theCel, int picnum)
{
	view = theView;
	loop = theLoop;
	cel = theCel;
	init(theX, theY);
	noSkip();
	new CheckButton(theX, theY, picnum);
}
	    
CheckButton::CheckButton(int theX, int theY, int picnum)
{
	view = 150;
	loop = 10;
	cel = 0;
	picNum = picnum;
	init(theX - 28, theY + 10);
	noSkip();
}

int CheckButton::handleEvent ( MADEEventStamp *event )
{
	if (user->input && onMe(event))
	{
		if (event_type == USER_LEFT_UP)
		{
			if(blendbox->checked[picNum])
			{
				setCel(0);
				blendbox->numchecked--;
				blendbox->checked[picNum] = false;
			}
			else
			{
				if(blendbox->numchecked < 2)
				{
					setCel(1);
					blendbox->numchecked++;
					blendbox->checked[picNum] = true;
				}
			}							
			return true;
		}
	}
	return false;
}

/***********************************
 *		
 *			  Views
 *		      
 **********************************/
ImageBack::ImageBack()
{
	view = 149;
	loop = 0;
	cel = 0;
	init(0, 16);
	noSkip();
	addToPic();
	dispose();
}

Picture::Picture()
{
	name = "Picture";
	currentZoom = 0;
	numTextSprites = 0;
	currentQuad = 0;
	numTextObjs = 0;
	numTextSprites = 0;
	scaleSprite = NULL;
	textSprites[0] = NULL;
	textSprites[1] = NULL;
	intrpuzz->add(this);
}

void Picture::init(int theView, int theLoop, int theCel, bool scale)
{
	SRect theRect;	

	photoSprite = new(IntrPuzzView);
	photoSprite->init(ORIGIN_X, ORIGIN_Y, theView, theLoop, theCel);
	photoSprite->noSkip();

	if(scale)
	{
		photoSprite->hide();
		SRect_Init ( &theRect, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT );

		sfxSetDrawInfo (ADV_DRAW_SCALEX, 256);
		sfxSetDrawInfo (ADV_DRAW_SCALEY, 256);
		scaleSprite = sfxClipSprite ( photoSprite->mySprite, ORIGIN_X, ORIGIN_Y, &theRect );
		sfxKillSprite(photoSprite->mySprite);
		photoSprite->mySprite = scaleSprite;
		//Dos 8
	//	CleanUpSprites();

		scaleSprite = NULL;
	}
}

void Picture::setATSQuads(int startPhoto, int topLeft, int topRight, int bottomLeft, int bottomRight, int middle)
{
	zoomViews[0] = startPhoto;
	zoomViews[1] = topLeft;
	zoomViews[2] = topRight;
	zoomViews[3] = bottomLeft;
	zoomViews[4] = bottomRight;
	zoomViews[5] = middle;
}					 

void Picture::initTextObjs(TextObj objs[], int numObjs)
{
	numTextObjs = numObjs;
	
	for(int i = 0; i < numTextObjs; i++)
	{
		textObjList[i].quad = objs[i].quad;
		textObjList[i].left = objs[i].left;
		textObjList[i].top = objs[i].top;
		textObjList[i].right = objs[i].right;
		textObjList[i].bottom = objs[i].bottom;
		textObjList[i].view = objs[i].view;
		textObjList[i].loop = objs[i].loop;
	}
}

void Picture::dispose()
{
	intrpuzz->release ( this );
	deathList->add(this);
}

Picture::~Picture()
{
	if((currentZoom != 0) && scaleSprite)
		sfxKillSprite(scaleSprite);
}
	   
/*EnhanceText::EnhanceText(int theX, int theY, char *theText)
{
	name 	= "EnhanceText";
	size	= 15;
	fore_color = TEXT_COLOR_BLACK;
  	high_color = TEXT_COLOR_BLACK;
  	x = theX;
  	y = theY;
	priority = 500;
	textBuffer = -1;
	init(theText);
}
*/
/***********************************
 *		
 *			  Buttons
 *		      
 **********************************/
void PictureButton::init(int theX, int theY, int theView, int theLoop, int thePicture)
{
	name = "PicButt";
	view = theView;
	loop = theLoop;
	PicNum = thePicture;
	Button::init(theX, theY, theView, theLoop, 0);
	picture = new(Picture);
//	currentZoom = 0;
//	picture->init(view, PicNum, 0, NOSCALE);
//	picture->photoSprite->hide();
	noSkip();
}
	
void PictureButton::activate()
{
	SRect newRect;
	Sprite *tempSprite;

	if(currentPicture)
	{
		if(currentPicture == this->picture)
		{
			return;
		}
								
	//	if(currentPicture->currentZoom > 0)
	//	{
	//		sfxKillSprite(currentPicture->scaleSprite);
	//		currentPicture->scaleSprite = NULL;
	//	}
		currentPicture->photoSprite->dispose();
		//Dos 8
		if(deathList->size)
			deathList->dispose();
//		CleanUpSprites();
	}

	currentPicture = picture;
	if(currentPicture->currentZoom > 0)
	{
		currentPicture->init(currentPicture->zoomViews[currentPicture->currentQuad], 0, 0, HALFSCALE);

		for(int j = 1; j < currentPicture->currentZoom; j++)
		{
			sfxSetDrawInfo (ADV_DRAW_SCALEX, 1024);
			sfxSetDrawInfo (ADV_DRAW_SCALEY, 1024);
			SRect_Init ( &newRect, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT);
			tempSprite = sfxClipSprite ( currentPicture->photoSprite->mySprite, 0, 0, &newRect);
			sfxKillSprite(currentPicture->photoSprite->mySprite);
			currentPicture->photoSprite->mySprite = tempSprite;
			//Dos 8
			CleanUpSprites();

			sfxSetDrawInfo (ADV_DRAW_SCALEX, 512 );
			sfxSetDrawInfo (ADV_DRAW_SCALEY, 512 );
			tempSprite = sfxClipSprite ( currentPicture->photoSprite->mySprite, 16, 89, &currentPicture->zoomList[j] );
			sfxKillSprite(currentPicture->photoSprite->mySprite);	      
			currentPicture->photoSprite->mySprite = tempSprite;
			//Dos 8
			CleanUpSprites();
		}
	}
	else
	{
		currentPicture->init(currentPicture->zoomViews[CENTER], 0, 0, NOSCALE);
	}

	if(blendPicture)
	{
//		blendPicture->photoSprite->dispose();
		blendPicture->dispose();
		blendPicture = NULL;
	}
	if(currentPicture->currentZoom == MAX_ZOOMS)
		OCEbutt->setCel(0);
	else
		OCEbutt->setCel(2);

}

ImageEnhance::ImageEnhance()
{
	init(432, 298, 150, 6, 0);
	noSkip();
	OCEbutt = this;
	setCel(2);
}

void ImageEnhance::activate()
{
	if(!scanned && (currentPicture->currentZoom == MAX_ZOOMS))//on user left up
	{
		//new(ScanLineA);

		if(GameFlag.test(fGreg39)) //dos 8 meg
			new(ScanLineDos8);
		else
			new(ScanLineA);

		scanned = true;
	}
}

unZoomBut::unZoomBut()
{
	init(431, 249, 150, 3, 0);
	noSkip();
}

void unZoomBut::activate()
{
//	imageanal->zoomMode = NO_ZOOM;
	SRect newRect;
	Sprite *tempSprite;
	if(currentPicture->currentZoom > 0)
	{
		int i;

		scanned = false;	
		if(currentPicture->currentZoom == MAX_ZOOMS)
		{
			OCEbutt->setCel(2);
			if(currentPicture->numTextSprites > 0)
			{
				for(i = 0; i < currentPicture->numTextSprites; i++)
				{
					currentPicture->textSprites[i]->dispose();		
				}
				if (deathList->size)	
					deathList->dispose();

				currentPicture->numTextSprites = 0;
				purgeRes = true;
			}
		}

		currentPicture->currentZoom--;
		if(currentPicture->currentZoom == 0)
		{
		//	sfxKillSprite(currentPicture->scaleSprite);
		//	currentPicture->scaleSprite = NULL;
			currentPicture->photoSprite->dispose();
			currentPicture->init(currentPicture->zoomViews[CENTER], 0, 0, NOSCALE);
			currentPicture->currentZoom = 0;
		}				
		else
		{
			SRect_Init ( &newRect, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT );

			currentPicture->photoSprite->dispose();
			currentPicture->init(currentPicture->zoomViews[currentPicture->currentQuad], 0, 0, NOSCALE);
			sfxSetDrawInfo (ADV_DRAW_SCALEX, 256);
			sfxSetDrawInfo (ADV_DRAW_SCALEY, 256);
			tempSprite = sfxClipSprite ( currentPicture->photoSprite->mySprite, ORIGIN_X, ORIGIN_Y, &newRect );
			sfxKillSprite(currentPicture->photoSprite->mySprite);
			currentPicture->photoSprite->mySprite = tempSprite;
			//Dos 8
			CleanUpSprites();
			//tempSprite = sfxClipSprite ( currentPicture->photoSprite->mySprite, ORIGIN_X, ORIGIN_Y, &newRect );
			//sfxKillSprite(currentPicture->photoSprite->mySprite);
			//currentPicture->photoSprite->mySprite = tempSprite;

	
			for(i = 1; i < currentPicture->currentZoom; i++)
			{
				sfxSetDrawInfo (ADV_DRAW_SCALEX, 1024);
				sfxSetDrawInfo (ADV_DRAW_SCALEY, 1024);
				SRect_Init ( &newRect, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT );
				tempSprite = sfxClipSprite ( currentPicture->photoSprite->mySprite, 0, 0, &newRect);
				sfxKillSprite(currentPicture->photoSprite->mySprite);
				currentPicture->photoSprite->mySprite = tempSprite;
				//Dos 8
				CleanUpSprites();
			
				sfxSetDrawInfo (ADV_DRAW_SCALEX, 512 );
				sfxSetDrawInfo (ADV_DRAW_SCALEY, 512 );
				tempSprite = sfxClipSprite ( currentPicture->photoSprite->mySprite, 16, 89, &currentPicture->zoomList[i] );
				sfxKillSprite(currentPicture->photoSprite->mySprite);
				currentPicture->photoSprite->mySprite = tempSprite;
				//Dos 8
				CleanUpSprites();
			}
			currentPicture->photoSprite->show();
			//sfxShowSprite(currentPicture->scaleSprite);
		}
		CleanUpSprites();
		imageanal->imageSFX->play(113);  //zoom out sound effect
		flushMessageCue();
	}
}
					  
ZoomBut::ZoomBut()
{
	init(431, 227, 150, 2, 0);
	noSkip();
}

/*int ZoomBut::handleEvent(MADEEventStamp* event)
{
	if(!ImageButton::handleEvent(event))
	{
		return false;
	}
	else //on user left up
	{
		imageanal->zoomMode = ZOOM_IN;
		return (true);
	}		 
}
*/
void ZoomBut::activate()
{
	imageanal->zoomMode = ZOOM_IN;
}	

BlendBut::BlendBut()
{
	init(432, 349, 150, 4, 0);
	noSkip();
}

void BlendBut::activate()
{
	new(BlendBox);
}

CapsSendBut::CapsSendBut()
{
	init(432, 383, 666, 6, 0);
	noSkip();
}					   

void CapsSendBut::activate()
{
	if(!GameFlag.test(fDCK10))
	{
		if(webreport->flag & OBJECT_HIDDEN)
		{
			if (imageanal->imageVoice->isPlaying() )
				imageanal->imageVoice->stop();
		}
	}
	int i;

	//if(GameFlag.test(fKneeAddressAvailable))

	//if(GameFlag.Test(fSawKneeAddress))
		
	GameFlag.set(fReportKneecaps);

	for (i = 0; i < 4; i++)
	{
		mailFlags[0][i] = true;
	}		 

	for (i = 0; i < 4; i++)
	{
		mailFlags[1][i] = true;
	}		 

	if(GameFlag.test(fSawKneeAddress))
		mailFlags[1][2] = true;
	else
		mailFlags[1][2] = false;

	if(GameFlag.test(fKneeAddressAvailable))
		mailFlags[0][3] = true;
	else
		mailFlags[0][3] = false;

	if(!GameFlag.test(fKneecapsFound))
		new EMailSend(214, this);
	else if (!(GameFlag.test(fFullBriefing)))
		new EMailSend(229, this);
	else
		new EMailSend(230, this);
}

void CapsSendBut::cue()
{
	GameFlag.set(fSentPuzzleMail);
	if(choiceIndex[0] == 3 && choiceIndex[1] == 2 && !GameFlag.test(fKneecapsFound))	
	{
		comlinks->removeVLink("35020.avi");
		new VLink("Settle the score: Warhurst, Eugene J.", "35020.avi", fKneecapsFound, 0, new EMailAttachment( "Help Badman", 98113 ));
		pcLastUsed[PC_IA] = false;
	}
	else if(!GameFlag.test(fKneecapsFound))	
	{
		if(!GameFlag.test(fGreg32))		
		{
			new VoiceMail("That's not it: Holt, David", 30651, HOLT);
			GameFlag.set(fGreg32);
		}
		else if(!GameFlag.test(fGreg33))
		{
			new VoiceMail("Stop guessing: Holt, David", 30653, HOLT);
			GameFlag.set(fGreg33);
		}
		else
		{
			if(!(webreport->flag & OBJECT_HIDDEN))
			{
				WebReport::cue();
			}

			comlinkshort = NULL;
			intrpuzz->quitIntrface();
			if(theInset)
				theInset->dispose();
			theGame->newRoom(new Rm60000(END_FIRED));
			return;	
		}
	}
	
	if(GameFlag.test(fDCK10))
	{
		intrpuzz->setScript(new KneeDrawScript);
	}
	else if(!(webreport->flag & OBJECT_HIDDEN))
		WebReport::cue();

}

PlateSendBut::PlateSendBut()
{
	init(432, 383, 666, 6, 0);
	noSkip();
}

void PlateSendBut::setFlags()
{
	//int numTextObjs = 6;
	int index = 0;
	mailFlags[0][3] = false;
	mailFlags[0][4] = false;

	for (int i = 0; i < 3; i++)
	{
		if(mailFlags[0][index] || mailFlags[0][index + 1])
		{
			mailFlags[0][i] = true;
			mailFlags[0][i + 1] = false;
		}
		index += 2;
	}		 
	mailFlags[0][3] = true;
	mailFlags[0][4] = true;
}

void PlateSendBut::activate()
{
	mailFlags[0][3] = true;
	mailFlags[0][4] = true;

	if(webreport->flag & OBJECT_HIDDEN)
	{
		if (imageanal->imageVoice->isPlaying() )
			imageanal->imageVoice->stop();
	}
	new EMailSend(200, this);
}

void PlateSendBut::cue()
{
	GameFlag.set(fSentPuzzleMail);
	if(choiceIndex[0] == 0)	//brown car plate
	{
		GameFlag.clear(fPatsHTM_119);
		GameFlag.set( fFarmIALicenseDone );
//		intrpuzz->dispose();
//		if(!webmap->isEmpty())
//			webmap->dispose();	

//		if(intrpuzz->isStateSaved("imageinfo"))
//			imageinfo->dispose();
		intrpuzz->quitIntrface();

//		new HTMLPage( 99170 );
	}
	else
	{
		intrpuzz->quitIntrface();
	}
}
		   
TankSendBut::TankSendBut()
{
	init(432, 383, 666, 6, 0);
	noSkip();
}

void TankSendBut::activate()
{
	if(webreport->flag & OBJECT_HIDDEN)
	{
		if (imageanal->imageVoice->isPlaying() )
			imageanal->imageVoice->stop();
	}

	new EMailSend(201, this);
}

void TankSendBut::cue()
{
	GameFlag.set(fSentPuzzleMail);
	if(choiceIndex[0] == 3)	//8 tanks
	{
		GameFlag.clear(fPatsHTM_120);
		GameFlag.set( fFarmIATanksDone );
		intrpuzz->quitIntrface();
	}
	else
	{
		intrpuzz->quitIntrface();
	}
}

PegSendBut::PegSendBut()
{
	init(432, 383, 666, 6, 0);
	noSkip();
}

void PegSendBut::activate()
{
	memset (&mailFlags,0,(5 * 80)*sizeof(bool));

	mailFlags[0][0] = true;
 	GameFlag.set(fReportPEG);

	if(webreport->flag & OBJECT_HIDDEN)
	{
		if (imageanal->imageVoice->isPlaying() )
			imageanal->imageVoice->stop();
	}

	if(GameFlag.test(fPEGID_shotgun))
		mailFlags[0][1] = true;

	if(GameFlag.test(fPEGID_exploding))
		mailFlags[0][2] = true;

	if(GameFlag.test(fPEGID_liquid))
		mailFlags[0][3] = true;

	if(GameFlag.test(fPEGID_needlepack))
		mailFlags[0][4] = true;

	if(GameFlag.test(fPEGID_7mm))
		mailFlags[0][5] = true;

	if(GameFlag.test(fPEGID_9mm))
		mailFlags[0][6] = true;

	mailFlags[1][0] = true;

	if(GameFlag.test(25))
		mailFlags[1][1] = true;

	if(GameFlag.test(26))
		mailFlags[1][2] = true;

	if(GameFlag.test(27))
		mailFlags[1][3] = true;

	if(GameFlag.test(28))
		mailFlags[1][4] = true;

	if(GameFlag.test(29))
		mailFlags[1][5] = true;

	if(GameFlag.test(30))
		mailFlags[1][6] = true;

	if(!GameFlag.test(fIDPEG))
		new EMailSend(204, this);
	else if (!(GameFlag.test(fFullBriefing)))
		new EMailSend(229, this);
	else
		new EMailSend(230, this);
}

void PegSendBut::cue()
{
	if(choiceIndex[0] == 4 && choiceIndex[1] == 4) //peg chosen
	{
		if(!GameFlag.test(fIDPEG))
		{
			GameFlag.set(fIDPEG);
			testKATAndPEG();
		}	
		pcLastUsed[PC_IA] = false;
	}
	else if(!GameFlag.test(fIDPEG))
	{
		if(!GameFlag.test(fGreg30))		
		{
			new VoiceMail("That's not the projectile: Holt, David", 30654, HOLT);
			GameFlag.set(fGreg30);
		}
		else if(!GameFlag.test(fGreg31))
		{
			new VoiceMail("Stop guessing: Holt, David", 30653, HOLT);
			GameFlag.set(fGreg31);
		}
		else
		{
			if(!(webreport->flag & OBJECT_HIDDEN))
			{
				WebReport::cue();
			}

			comlinkshort = NULL;
			intrpuzz->quitIntrface();
			if(theInset)
				theInset->dispose();
			theGame->newRoom(new Rm60000(END_FIRED));
			return;	
		}
	}

	if(!(webreport->flag & OBJECT_HIDDEN))
		WebReport::cue();
}

AmesMixMatch::AmesMixMatch()
{
	init(432, 348, 150, 5, 0);
	noSkip();
}

void AmesMixMatch::activate()
{
	imageanal->saveState();

	intrpuzz->dispose();
	int pictureNumber = 0;
	for(int i = 0; i < MAX_PICS; i++)
	{
		if(currentPicture == picBut[i]->picture)
		{
			pictureNumber = i;
			break;
		}		 	
	}
	if(blendPicture)
	{
		pictureNumber = 3;
	}
	new AmesMatch(pictureNumber);
}
    
CapsMixMatch::CapsMixMatch()
{
	init(432, 348, 150, 5, 0);
	noSkip();
}

void CapsMixMatch::activate()
{
	imageanal->saveState();

	int pictureNumber = 0;
	for(int i = 0; i < MAX_PICS; i++)
	{
		if(currentPicture == picBut[i]->picture)
		{
			pictureNumber = i;
			break;
		}		 	
	}
	if(blendPicture)
	{
		pictureNumber = 3;
	}
	intrpuzz->dispose();
	new KneeCapsMatch(pictureNumber);
}

/*ClearBut::ClearBut()
{
	init(432, 348, 150, 1, 0);
	noSkip();
}

void ClearBut::activate()
{
	int i;

	if(currentPicture->currentZoom == MAX_ZOOMS)
	{
		if(currentPicture->numTextSprites > 0)
		{
			for(int i = 0; i < currentPicture->numTextSprites; i++)
			{
				currentPicture->textSprites[i]->dispose();		
			}
			currentPicture->numTextSprites = 0;
		}
	}

	for(i = 0; i < numPics; i++)
	{	
		if(picBut[i]->picture->currentZoom > 0)
		{
			sfxKillSprite(picBut[i]->picture->scaleSprite);
		}				
		picBut[i]->picture->photoSprite->dispose();
		picBut[i]->picture->init(picBut[i]->picture->zoomViews[CENTER], 1, 0, NOSCALE);
		picBut[i]->picture->photoSprite->hide();
		picBut[i]->picture->currentZoom = 0;
	}		
	if(blendPicture)
	{
		blendPicture->photoSprite->dispose();
		blendPicture->dispose();
		blendPicture = NULL;	
	}			
	currentPicture = picBut[0]->picture;
	currentPicture->photoSprite->show();
	imageanal->zoomMode = NO_ZOOM;
}*/

} // namespace Spycraft
