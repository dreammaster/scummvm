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

#include "spycraft/game/mixmatch.h"
#include "spycraft/game/flag.h"
#include "spycraft/game/image.h"
#include "spycraft/game/vlink.h"
#include "spycraft/game/timer.h"
#include "spycraft/game/puzztest.h"	    
#include "spycraft/game/web.h"
#include "spycraft/game/pcscreen.h"
#include "spycraft/game/kat.h"
#include "spycraft/logic/60000.h"

namespace Spycraft {

#define SEARCH			1		   
#define NOSEARCH		0
bool peopleFound[MAX_PHOTOS];

void CleanUpSprites();

int eyeColor;
int hairColor;
int gender;
int skinColor;

enum {
	 HARMONICA,
	 BRAD,
	 JOHND,
	 AMES,
	 GARY,
	 SETH,
	 VESCO,
	 AUSTIN,
	 YAZ,
	 TOM,
	 GRENDEL,
	 SUNG,
	 COHEN,
	 BENTON,
	 MCGRUDER
	};
		   
//33 = separator
//face, eyes, nose, mouth, hair, beard, skin
/*BYTE matches[MAX_PHOTOS][40] = {{0, 1, 3, 4, 33, 0, 1, 2, 3, 4, 33, 0, 1, 2, 3, 4, 5, 6, 7, 33, 0, 1, 2, 3, 4, 5, 33, 1, 33, 0, 33, 0},//Harmonica(TomPhillips)
								{10, 4, 33, 3, 4, 1, 33, 6, 7, 33, 1, 33, 4, 33, 0, 33, 0},//brad
								{10, 3, 33, 2, 33, 1, 5, 7, 33, 1, 4, 33, 3, 33, 0, 33, 0},	 	//John D
								{10, 33, 1, 33, 0, 1, 7, 33, 4, 0, 1, 33, 3, 33, 0, 33, 0},	 	//Dave Z
								{10, 33, 10, 33, 10, 33, 10, 33, 0, 33, 0, 33, 0},//Gary
								{10, 33, 0, 4, 33, 1, 5, 7, 33, 0, 3, 2, 4, 33, 0, 1, 33, 2, 33, 0},	 //Seth
								{10, 33, 10, 33, 10, 33, 10, 33, 10, 33, 10, 33, 0},//Vesco
								{10, 33, 10, 33, 10, 33, 10, 33, 10, 33, 10, 33, 0},//Austin
								{10, 33, 10, 33, 10, 33, 10, 33, 10, 33, 10, 33, 1},	 	//Yaz
								{10, 4, 33, 3, 33, 3, 5, 6, 7, 33, 5, 33, 3, 33, 0, 33, 0},//Tom
								{0, 1, 3, 4, 33, 0, 1, 2, 3, 4, 33, 0, 1, 2, 3, 4, 5, 6, 7, 33, 0, 2, 3, 1, 4, 5, 33, 2, 3, 4, 33, 0, 5, 33, 0},//Grendel
								{0, 1, 3, 4, 33, 6, 33, 0, 1, 2, 3, 4, 5, 6, 7, 33, 0, 1, 2, 3, 4, 5, 33, 3, 4, 33, 0, 33, 0},	 	//Linus(DR. sung)
								{2, 33, 0, 1, 2, 3, 4, 33, 0, 1, 2, 3, 4, 5, 6, 7, 8, 33, 1, 2, 0, 3, 4, 5, 33, 3, 33, 5, 33, 0},//Daniel
								{2, 33, 0, 1, 2, 3, 4, 33, 0, 1, 2, 3, 4, 5, 6, 7, 33, 0, 1, 2, 3, 4, 5, 33, 0, 1, 33, 0, 33, 0}, //Benton
								{0, 1, 3, 4, 33, 0, 1, 2, 3, 4, 33, 0, 1, 2, 3, 4, 5, 6, 7, 33, 0, 1, 2, 3, 4, 5, 33, 5, 33, 0, 33, 0} //maryanne(McGruder)
								};	 

*/
BYTE matches[MAX_PHOTOS][40] = {{0, 1, 3, 4, 33, 0, 2, 3, 4, 33, 0, 1, 3, 4, 5, 6, 7, 33, 0, 1, 3, 4, 5, 33, 1, 33, 0, 33, 0},//Harmonica(TomPhillips)
								{10, 4, 33, 3, 4, 1, 33, 6, 7, 33, 1, 33, 4, 33, 0, 33, 0},//brad
								{10, 3, 33, 2, 33, 1, 5, 7, 33, 1, 4, 33, 3, 33, 0, 33, 0},	 	//John D
								{10, 33, 1, 33, 0, 1, 7, 33, 4, 0, 1, 33, 3, 33, 0, 33, 0},	 	//Dave Z
								{10, 33, 10, 33, 10, 33, 10, 33, 0, 33, 0, 33, 0},//Gary
								{10, 33, 0, 4, 33, 1, 5, 7, 33, 0, 3, 2, 4, 33, 0, 1, 33, 2, 33, 0},	 //Seth
								{10, 33, 10, 33, 10, 33, 10, 33, 10, 33, 10, 33, 0},//Vesco
								{10, 33, 10, 33, 10, 33, 10, 33, 10, 33, 10, 33, 0},//Austin
								{10, 33, 10, 33, 10, 33, 10, 33, 10, 33, 10, 33, 1},	 	//Yaz
								{10, 4, 33, 3, 33, 3, 5, 6, 7, 33, 5, 33, 3, 33, 0, 33, 0},//Tom
								{0, 1, 3, 4, 33, 0, 2, 4, 33, 0, 1, 3, 4, 5, 6, 7, 33, 0, 2, 3, 1, 4, 5, 33, 2, 3, 4, 33, 0, 5, 33, 0},//Grendel
								{0, 1, 3, 4, 33, 6, 33, 0, 1, 3, 4, 5, 6, 7, 33, 0, 1, 2, 3, 4, 5, 33, 3, 4, 33, 0, 33, 0},	 	//Linus(DR. sung)
								{2, 33, 0, 2, 4, 33, 0, 1, 3, 4, 5, 6, 7, 8, 33, 1, 2, 0, 3, 4, 5, 33, 3, 33, 5, 33, 0},//Daniel
								{1, 2, 3, 33, 1, 33, 0, 1, 2, 3, 4, 5, 6, 7, 33, 0, 1, 3, 4, 5, 33, 0, 1, 33, 0, 33, 0}, //Benton
								{0, 1, 3, 4, 33, 0, 1, 2, 3, 4, 33, 0, 1, 3, 4, 5, 6, 7, 33, 0, 1, 2, 3, 4, 5, 33, 5, 33, 0, 33, 0} //maryanne(McGruder)
								};	 

int matchViews[MAX_PHOTOS] = {814, //Harmonica (Tom Phillps)
							  817, //brad
							  822, //John D
							  819, //Dave Z
							  820, //Gary
							  825, //Seth
							  829, //Vesco
							  815, //Austin
							  830, //Yaz
							  826, //Tom
							  821, //Grendel
							  823, //Linus
							  818, //Daniel
							  816, //Benton
							  824  //mcgruder
							  };

int matchDossier[MAX_PHOTOS] = {99021, //Harmonica
								98610, //brad
								98611, //John D
								98612, //Dave Z
								99021, //Gary
								98613, //Seth
								99021, //Vesco
								99021, //Austin
								99021, //Yaz
								98614, //Tom
								99405, //Grendel
								99907, //Linus
								99902, //Daniel
								99903, //Benton
								99905 //maryanne (mcgruder)
								};
	 
MatchInfo			*matchinfo;
IdWindow			*idWin;
IdDragBox			*iddragbox;
MatchDoneButton		*applybutt;
BodyParts			*bodyparts;
Slider				*mySlider;
MixMatch			*mix;
MatchBox			*matchDialog;

WebReport			*matchreport;	

int sliderWidth;
int photoView;
char currentButton[12];

extern void CleanUpSprites();

/***********************************
 *		
 *			  Ames MixMatch
 *		      
 **********************************/
AmesMatch::AmesMatch()
{
	IntrPuzzPage::init(WEB_AMESMATCH);
	helppage = 92150;

	if(intrpuzz->isStateSaved("matchinfo"))
	{
		matchinfo->load();
	}
	mix->drawPhoto(mix->PhotoView);
	new(AmesSendBut);
}

AmesMatch::AmesMatch(int photoNumber)
{
	photoView = photoNumber;
	IntrPuzzPage::init(WEB_AMESMATCH);
 	helppage = 92150;

	mix->PhotoView = photoNumber;
	mix->drawPhoto(photoNumber);
	init();
	new(AmesSendBut);
}
	
/***********************************
 *		
 *			  KneeCaps MixMatch
 *		      
 **********************************/
KneeCapsMatch::KneeCapsMatch()
{
	IntrPuzzPage::init(WEB_KNEECAPSMATCH);
	helppage = 92150;

	if(intrpuzz->isStateSaved("matchinfo"))
	{
		matchinfo->load();
	}
	mix->drawPhoto(mix->PhotoView);
	new(KneeSendBut);

	if (theMusic->isPlaying())
		theMusic->stop();
	theMusic->setVolume(25);
	theMusic->play(200);
}

KneeCapsMatch::KneeCapsMatch(int photoNumber)
{
	photoView = photoNumber;
	IntrPuzzPage::init(WEB_KNEECAPSMATCH);
 	helppage = 92150;

	mix->PhotoView = photoNumber;
	mix->drawPhoto(photoNumber);
	init();
	new(KneeSendBut);

	if (theMusic->isPlaying())
		theMusic->stop();
	theMusic->setVolume(25);
	theMusic->play(200);
}

/***********************************
 *		
 *			  Grendel MixMatch
 *		      
 **********************************/
GrendelMatch::GrendelMatch()
{
	IntrPuzzPage::init(WEB_GRENDELMATCH);
	helppage = 92150;

	if(intrpuzz->isStateSaved("matchinfo"))
	{
		matchinfo->load();
	}
	new(MatchBox);

	IntrPuzzView *picture = new(IntrPuzzView);
	picture->init(7, 89, mix->PhotoView, 0, mix->PhotoCel); 	
	picture->noSkip();
	picture->addToPic();
	picture->dispose();
//	sfxSpriteScale(picture->mySprite, 400, 400);

//	CleanUpSprites();
	webreport->hide();
	matchreport = new(InfosciSendButt);
}

GrendelMatch::GrendelMatch(int photoNumber)
{
	photoView = photoNumber;
	IntrPuzzPage::init(WEB_GRENDELMATCH);
	helppage = 92150;

	IntrPuzzView *picture = new(IntrPuzzView);
	mix->PhotoView = photoNumber;
	mix->PhotoCel = 0;
	mix->PhotoLoop = 0;
	picture->init(7, 89, mix->PhotoView, 0, mix->PhotoCel); 	
	picture->noSkip();
	picture->addToPic();
	picture->dispose();
//	sfxSpriteScale(picture->mySprite, 400, 400);
//	CleanUpSprites();

	new(MatchBox);
	init();

	webreport->hide();
	matchreport = new(InfosciSendButt);
}

/***********************************
 *		
 *			  Kat MixMatch
 *		      
 **********************************/
KatMatch::KatMatch(bool Load)
{
	IntrPuzzPage::init(WEB_KATMATCH);
 	helppage = 92150;

	if(intrpuzz->isStateSaved("matchinfo"))
	{
		matchinfo->load();
	}
	IntrPuzzView *picture = new(IntrPuzzView);
	picture->init(7, 89, mix->PhotoView, 0, mix->PhotoCel); 	
	picture->noSkip();
	if(mix->PhotoView != 247)
		sfxSpriteScale(picture->mySprite, 821, 813);
	picture->addToPic();
	picture->dispose();

	webreport->hide();
	matchreport = new(KatSendBut);

	new(MatchBox);

	if (theMusic->isPlaying())
		theMusic->stop();
	theMusic->setVolume(25);
	theMusic->play(200);
}

KatMatch::KatMatch(int theView, int theCel)
{
	ZoomView[0] = 243;
	ZoomView[1] = 244;
	ZoomView[2] = 245;
	ZoomView[3] = 246;

 	theView = ZoomView[katinfo->cameraZoom];
	theCel = katinfo->camPosition;

	IntrPuzzPage::init(WEB_KATMATCH);
 	helppage = 92150;

	IntrPuzzView *picture = new(IntrPuzzView);
	if((theView == 246) && (theCel == 11))
	{
		mix->PhotoView = 247;
		mix->PhotoCel = 0;
		mix->PhotoLoop = 0;
		picture->init(7, 89, mix->PhotoView, 0, mix->PhotoCel); 	
		picture->noSkip();
		picture->addToPic();
		picture->dispose();
	}
	else
	{
		picture->init(7, 89, theView, 0, theCel); 	
		picture->noSkip();
		sfxSpriteScale(picture->mySprite, 821, 813);
		picture->addToPic();
		picture->dispose();
		mix->PhotoView = theView;
		mix->PhotoCel = theCel;
		mix->PhotoLoop = 0;
	}

  	init();

	webreport->hide();
	matchreport = new(KatSendBut);
	new(MatchBox);

	if (theMusic->isPlaying())
		theMusic->stop();
	theMusic->setVolume(25);
	theMusic->play(200);

	mix->setScript(new MixStartScript);
}
	

AmesSendBut::AmesSendBut()
{
	init(432, 383, 666, 6, 0);
	noSkip();
}

void AmesSendBut::activate()
{
 	memset (&mailFlags,0,(5 * 80)*sizeof(bool));

	if(peopleFound[JOHND])
		mailFlags[0][0] = true;
	if(peopleFound[TOM])
		mailFlags[0][1] = true;
	if(peopleFound[AMES])
		mailFlags[0][2] = true;
	if(peopleFound[YAZ])
		mailFlags[0][3] = true;
		mailFlags[0][4] = true;
		mailFlags[0][5] = true;
	if(peopleFound[BRAD])
		mailFlags[0][6] = true;
	if(peopleFound[BENTON])
		mailFlags[0][7] = true;
	if(peopleFound[GARY])
		mailFlags[0][8] = true;

	new EMailSend(202, this);
}

void AmesSendBut::cue()
{
	if(choiceIndex[0] == 2)	//Ames
		GameFlag.set( fFarmIAAmesDone );

	intrpuzz->quitIntrface();
}

KatSendBut::KatSendBut()
{
	init(432, 383, 666, 6, 0);
	noSkip();
}

void KatSendBut::activate()
{
 	memset (&mailFlags,0,(5 * 80)*sizeof(bool));
 	GameFlag.set(fReportKat);
	mailFlags[0][0] = true;
	mailFlags[0][1] = true;
	if(GameFlag.test(115))
		mailFlags[0][2] = true;
	new EMailSend(203, this);
}

void KatSendBut::cue()
{
	if(choiceIndex[0] == 2)	//Harmonica Wade Phillips
	{
		if(!GameFlag.test(fSolvedDubanskyKAT))
		{	
			GameFlag.clear( fKATAvailable );
			// mail order here purposely reversed: KAT beat, 124, 98112
			GameFlag.set(fSolvedDubanskyKAT);
			new VLink("Harmonica's history: Milkovsky, Frank", "124.avi");			
			new EMail( "A frightening find: Warhurst, Eugene J.", 98112 ); 
			testKATAndPEG();
		}
		pcLastUsed[PC_KAT] = false;
	}
	else
	{
		if(!GameFlag.test(fGreg28))		
		{
	  		new EMail("Keep looking: Seaton, Jaimie A.", 98081); 		
			GameFlag.set(fGreg28);
		}
		else if(!GameFlag.test(fGreg29))
		{
			new VoiceMail("Stop guessing: Holt, David", 30653, HOLT);
			GameFlag.set(fGreg29);
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

GrendelSendBut::GrendelSendBut()
{
	init(432, 383, 666, 6, 0);
	noSkip();
}

void GrendelSendBut::activate()
{
	int i;
 	memset (&mailFlags,0,(5 * 80)*sizeof(bool));

	for (i = 0; i < 18; i++) //temp make all available
	{
		mailFlags[0][i] = true;
	}		 

	//beginning of people flags
	for(i = 0; i < 4; i++)
	{
		mailFlags[1][i] = true;
	}

	if(peopleFound[GRENDEL] && GameFlag.test(99))//99 = fSawGrendelDossier
		mailFlags[1][4] = true;
				
	for (i = 5; i < 16; i++) //temp make all available
	{
		mailFlags[1][i] = true;
	}		 

	new EMailSend(207, this);
}

void GrendelSendBut::cue()
{
	if((choiceIndex[0] == 7) && (choiceIndex[1] == 4))	//grendel as cohen
	{
		GameFlag.set(fKnowGrendelStolePEG);
		if(GameFlag.test(fIDYingVoice) && !GameFlag.test(fTom4))
		{
			GameFlag.set(fTom4);
			new VLink("155.avi", 0, 0);
		}
		else if(!GameFlag.test(fTom3))
		{
			GameFlag.set(fTom3);
			new EMail("There's still more: Holt, David M.", 98901);
		}		
	}
}

KneeSendBut::KneeSendBut()
{
	init(432, 383, 666, 6, 0);
	noSkip();
}

void KneeSendBut::activate()
{
	GameFlag.set(fWahKa9);
}

void KneeSendBut::cue()
{
}

/***********************************
 *		
 *		  Main MixMatch	Page
 *		      
 **********************************/
MixMatch::MixMatch()
{
	user->input = false;    //precaution just in case its not set when we come in
  	eyeColor = BROWN_EYES;
	hairColor = BROWN_HAIR;
	skinColor = FAIR_SKIN;
	gender = MALE;

	mix = this;
	windowDrag = false;
	numMatches = 0;

	new(MixBack);
	new(HairBut);
	new(EyesBut);
	mySlider = new(ImageSlider);
	new(SearchButt);
	new(NoseBut);
	new(MouthBut);
	new(BeardBut);
 	FaceBut *facebut = new(FaceBut);
	facebut->setCel(1);
	facebut->buttonDown = true;
	Common::strcpy_s(currentButton, facebut->name);

	new(LeftArrow);
	new(RightArrow);


	matchSFX = new(Sound);
	scanSFX = new (Sound);
	//dos
/*	sfxLoadRes(131, RES_WAVE);  	//preload searching sounds
	sfxLoadRes(40041, RES_WAVE);  //preload searching sounds
	sfxLoadRes(45071, RES_WAVE);  //preload searching sounds
	sfxLoadRes(45033, RES_WAVE);  //preload searching sounds
*/
	idWin = new IdWindow(277, 347);
	new(MixMatchWin);
  	memset (&peopleFound,0,(MAX_PHOTOS)*sizeof(bool));

	if(!intrpuzz->isStateSaved("matchinfo"))
	 	memset (&mailFlags,0,(5 * 80)*sizeof(bool));

	user->input = true;
}

void MixStartScript::changeState ( int newState )
{
	switchTo
		mix->matchSFX->play(40038, NULL);
		dispose();
	END
}
			
void MixMatch::init()
{

	face = new(BodyParts);
	face->init(348, 116, FAIRPARTS, 0, 0, 6, HEADPRI);
	mouth = new(BodyParts);
	mouth->init(390, 246, FAIRPARTS, 1, 0, 6, MOUTHPRI);
	nose = new(BodyParts);
	nose->init(395, 203, FAIRPARTS, 2, 0, 8, NOSEPRI);

	eyes = new(BodyParts);
	eyes->init(367, 183, EYESFAIR, 0, 0, 7, EYESPRI);

	irises = new(BodyParts);
	irises->init(387, 197, 828, BROWN_EYES, face->cel, 6, IRISPRI);
	  
	hair = new(BodyParts);
	hair->init(327, 99, BROWNHAIR_VIEW, 0, 0, 6, HAIRPRI);

	beard = new(BodyParts);
	beard->init(362, 209, FACEHAIRFAIR, 0, 0, 8, BEARDPRI);

	mix->currentPart = mix->face;
}

void MixMatch::dispose()
{
	if(web_intrface)
	{
		matchreport->dispose();
		webreport->show();
	}

	if(theMusic->isPlaying())
	{
		theMusic->stop();
		if(intrpuzz->musicNum)
			theMusic->play(intrpuzz->musicNum);
	}
					
	scanSFX->dispose();
	matchSFX->dispose();
	IntrPuzzPage::dispose();
}

void MixMatch::drawPhoto(int photoNumber)
{
	Sprite *tempSprite;
	SRect newRect;
	Picture *picture = new(Picture);
	if(imageinfo->zoomPos[photoNumber] > 0)
	{
		picture->init(imageinfo->picZoomViews[photoNumber][imageinfo->zoomQuad[photoNumber]], 0, 0, HALFSCALE);

		for(int j = 1; j < imageinfo->zoomPos[photoNumber]; j++)
		{
			sfxSetDrawInfo (ADV_DRAW_SCALEX, 1024);
			sfxSetDrawInfo (ADV_DRAW_SCALEY, 1024);
			SRect_Init ( &newRect, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT);
			tempSprite = sfxClipSprite ( picture->scaleSprite, 0, 0, &newRect);
			sfxKillSprite(picture->scaleSprite);
			picture->scaleSprite = tempSprite;

			sfxSetDrawInfo (ADV_DRAW_SCALEX, 512 );
			sfxSetDrawInfo (ADV_DRAW_SCALEY, 512 );
			tempSprite = sfxClipSprite ( picture->scaleSprite, 16, 89, &imageinfo->theZooms[photoNumber][j] );
			sfxKillSprite(picture->scaleSprite);	      
			picture->scaleSprite = tempSprite;
		}

		sfxSetDrawInfo (ADV_DRAW_SCALEX, 328 );
		sfxSetDrawInfo (ADV_DRAW_SCALEY, 340 );
		SRect_Init ( &newRect, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT );
		tempSprite = sfxClipSprite ( picture->scaleSprite, 16, 89, &newRect );
		sfxKillSprite(picture->scaleSprite);	      

		sfxShowSprite(tempSprite);
	  	sfxMoveSprite(tempSprite, 7, 89);
		sfxSpriteAddToPic(tempSprite);
		picture->photoSprite->dispose();
		picture->dispose();
	}
	else
	{
		picture->init(imageinfo->theView[photoNumber], 1, 0, NOSCALE);
		sfxSetDrawInfo (ADV_DRAW_SCALEX, 328 );
		sfxSetDrawInfo (ADV_DRAW_SCALEY, 340 );
		SRect_Init ( &newRect, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT );
		tempSprite = sfxClipSprite ( picture->photoSprite->mySprite, 16, 89, &newRect );

	  	sfxMoveSprite(tempSprite, 7, 89);
		sfxSpriteAddToPic(tempSprite);
		picture->photoSprite->dispose();
		picture->dispose();
	}		
}

MixMatch::~MixMatch()
{
}	
     

void MixMatch::saveState()
{
		if(!intrpuzz->isStateSaved("matchinfo"))
		{
//			matchinfo->dispose();
			new(MatchInfo);
		}		
//		new(MatchInfo);
		matchinfo->save();
}

/***********************************
 *		
 *		 Puzzle Save Status
 *		      
 **********************************/
MatchInfo::MatchInfo()
{
	name = "matchinfo";
	web_intrface->addToFront(this); 
//	photo = NULL;
	matchinfo = this;
}

void MatchInfo::save()
{
 	savePhotoView = mix->PhotoView;
	savePhotoCel = mix->PhotoCel;
	savePhotoLoop = mix->PhotoLoop;

 	saveEyeColor = eyeColor;
	saveHairColor = hairColor;
	saveSkinColor = skinColor;
	saveGender = gender;

	saveMouth.view = mix->mouth->view;
	saveMouth.loop = mix->mouth->loop;
	saveMouth.cel  = mix->mouth->cel;

	saveEyes.view  = mix->eyes->view;
	saveEyes.loop  = mix->eyes->loop;
	saveEyes.cel   = mix->eyes->cel;

 	saveNose.view  = mix->nose->view;
	saveNose.loop  = mix->nose->loop;
	saveNose.cel   = mix->nose->cel;

 	saveFace.view  = mix->face->view;
	saveFace.loop  = mix->face->loop;
	saveFace.cel   = mix->face->cel;

 	saveHair.view  = mix->hair->view;
	saveHair.loop  = mix->hair->loop;
	saveHair.cel   = mix->hair->cel;

 	saveBeard.view  = mix->beard->view;
	saveBeard.loop  = mix->beard->loop;
	saveBeard.cel   = mix->beard->cel;

 	saveIrises.view  = mix->irises->view;
	saveIrises.loop  = mix->irises->loop;
	saveIrises.cel   = mix->irises->cel;

	numberMatches = mix->numMatches;
	photoIndex = mix->currentMatchIndex;
	if(mix->numMatches)
	{
		for(int i = 0; i < mix->numMatches; i++)
		{
			saveMatchViews[i] = mix->matchPhotoViews[i];
	  		saveMatchDossier[i] = mix->matchHTML[i];
		}
	}

 	memset (&savePeopleFound,0,(MAX_PHOTOS)*sizeof(bool));
	for(int i = 0; i < MAX_PHOTOS; i++)
	{
		savePeopleFound[i] = peopleFound[i];
	}
}

void MatchInfo::load()
{
 	mix->PhotoView = savePhotoView;
	mix->PhotoCel = savePhotoCel;
	mix->PhotoLoop = savePhotoLoop;

 	eyeColor = saveEyeColor;
	hairColor = saveHairColor;
	skinColor = saveSkinColor;
	gender = saveGender;

	mix->face = new(BodyParts);
	mix->face->init(348, 116, saveFace.view, saveFace.loop, saveFace.cel, 6, HEADPRI);
	mix->mouth = new(BodyParts);
	mix->mouth->init(390, 246, saveMouth.view, 1, saveMouth.cel, 6, MOUTHPRI);
	mix->nose = new(BodyParts);
	mix->nose->init(395, 203, saveNose.view, 2, saveNose.cel, 8, NOSEPRI);

	mix->eyes = new(BodyParts);
	mix->eyes->init(367, 183, saveEyes.view, saveEyes.loop, saveEyes.cel, 7, EYESPRI);

	mix->irises = new(BodyParts);
	mix->irises->init(387, 197, 828, saveIrises.loop, mix->face->cel, 6, IRISPRI);
	  
	mix->hair = new(BodyParts);
	mix->hair->init(327, 99, saveHair.view, saveHair.loop, saveHair.cel, 6, HAIRPRI);

	mix->beard = new(BodyParts);
	mix->beard->init(362, 209, saveBeard.view, saveBeard.loop, saveBeard.cel, 8, BEARDPRI);

	mix->currentPart = mix->face;

	mix->numMatches = numberMatches;
	if(numberMatches)
	{
		mix->currentMatchIndex = photoIndex;

		for(int i = 0; i < numberMatches; i++)
		{
			mix->matchPhotoViews[i] = saveMatchViews[i];			
	  		mix->matchHTML[i] = saveMatchDossier[i];
		}
		mix->currentPhoto = new MatchPhoto(mix->matchPhotoViews[mix->currentMatchIndex]);
		mix->currentPhoto->hide();
		new(IdWinInfo);
		idWin->idwininfo->hide();
		mix->currentMatchIndex = 0;

		if(mix->numMatches > 1)
		{
			new(IdWinPrev);
			new(IdWinNext);
			idWin->idwinnext->hide();
			idWin->idwinprev->hide();
		}

	 	idWin->closed = false;
		idWin->idwinopen->dispose();
		mix->setScript(new maximizeScript(277, 347, idWin->idWin_x, idWin->idWin_y, NOSEARCH));
	}

	for(int i = 0; i < MAX_PHOTOS; i++)
	{
		peopleFound[i] = savePeopleFound[i];
	}

}

void MatchInfo :: dispose()
{
	web_intrface->release ( this );
	deathList->add(this);
}

MatchInfo::~MatchInfo()
{
}

/***********************************
 *		
 *			  Buttons
 *		      
 **********************************/
SearchButt::SearchButt()
{
	init(420, 347, 800, 6, 0);
	noSkip();
}

void SearchButt::activate()
{
	user->canInput(false);
	if(mix->numMatches)
	{
		idWin->matchphoto->dispose();
		idWin->idwininfo->dispose();
		idWin->idwininfo = NULL;
		idWin->matchphoto = NULL;

		if(mix->numMatches > 1)
		{
			idWin->idwinnext->dispose();
			idWin->idwinprev->dispose();
			idWin->idwinnext = NULL;
			idWin->idwinprev = NULL;
		}
	}
 	if(idWin->closed)
	{
		user->canInput(false);
	 	idWin->closed = false;
		idWin->idwinopen->dispose();
		idWin->idwinopen = NULL;
		mix->setScript(new maximizeScript(277, 347, idWin->idWin_x, idWin->idWin_y, SEARCH));
	}
	else
	{		
		if(GameFlag.test(fGreg39))
			new IdAnim(true); //dos
		else		
			new IdAnim(idWin->idWin_x + 7, idWin->idWin_y + 25);
	}
	
}

int MMButton::handleEvent(MADEEventStamp* event)
{

	if (user->input)  
	{

		if(onMe(event))  
		{
		  	if (event_type == USER_LEFT_DOWN)
		    {
				setCel(1);
				buttonDown = true;
				Common::strcpy_s(currentButton, name);
				return true;
			}
		}

		if(buttonDown)
		{
			if(strcmp(currentButton, name) != 0)
			{
				buttonDown = false;
				setCel(0);
			}
		}
		return true;
	}
	return false;
}


GeneralBut::GeneralBut()
{
	init(227, 65, 800, 6, 0);
	buttonDown = false;
	noSkip();
}

int GeneralBut::handleEvent(MADEEventStamp* event)
{
	MMButton::handleEvent(event);	
	if (onMe(event) && user->input)  
	{
	  	if (event_type == USER_LEFT_DOWN)
	    {
			setCel(0);
			new(MatchBox);
			return (true);
		}
	}
	return(false);
}

FaceBut::FaceBut()
{
	name = "face";
	init(268, 85, 800, 0, 0);
	buttonDown = false;
	noSkip();
}

int FaceBut::handleEvent(MADEEventStamp* event)
{
	MMButton::handleEvent(event);	
	if (onMe(event) && user->input)  
	{
	  	if (event_type == USER_LEFT_DOWN)
	    {
			mix->currentPart = mix->face;
			mySlider->marker->x = 342 + mix->face->cel * (sliderWidth / mix->face->numCels);
			mySlider->marker->posn(mySlider->marker->x,mySlider->marker->y);
			return (true);
		}
	}
	return(false);
}

HairBut::HairBut()
{
	name = "hair";
	init(268, 127, 800, 1, 0);
	buttonDown = false;
	noSkip();
}

int HairBut::handleEvent(MADEEventStamp* event)
{
	MMButton::handleEvent(event);	
	if (onMe(event) && user->input)  
	{
	  	if (event_type == USER_LEFT_DOWN)
	    {
			mix->currentPart = mix->hair;
			mySlider->marker->x = 342 + mix->hair->cel * (sliderWidth / mix->hair->numCels);
			mySlider->marker->posn(mySlider->marker->x,mySlider->marker->y);
			return (true);
		}
	}
	return(false);
}

EyesBut::EyesBut()
{
	name = "eyes";
	init(268, 169, 800, 2, 0);
	buttonDown = false;
	noSkip();
}

int EyesBut::handleEvent(MADEEventStamp* event)
{
	MMButton::handleEvent(event);	
	if (onMe(event) && user->input)  
	{
	  	if (event_type == USER_LEFT_DOWN)
	    {
			mix->currentPart = mix->eyes;
			mySlider->marker->x = 342 + mix->eyes->cel * (sliderWidth / mix->eyes->numCels);
			mySlider->marker->posn(mySlider->marker->x,mySlider->marker->y);
			return (true);
		}
	}
	return(false);
}

NoseBut::NoseBut()
{
	name = "nose";
	init(268, 211, 800, 3, 0);
	buttonDown = false;
	noSkip();
}

int NoseBut::handleEvent(MADEEventStamp* event)
{
	MMButton::handleEvent(event);	
	if (onMe(event) && user->input)  
	{
	  	if (event_type == USER_LEFT_DOWN)
	    {
			mix->currentPart = mix->nose;
			mySlider->marker->x = 342 + mix->nose->cel * (sliderWidth / mix->nose->numCels);
			mySlider->marker->posn(mySlider->marker->x,mySlider->marker->y);
			return (true);
		}
	}
	return(false);
}

MouthBut::MouthBut()
{
	name = "mouth";
	init(268, 253, 800, 4, 0);
	buttonDown = false;
	noSkip();
}

int MouthBut::handleEvent(MADEEventStamp* event)
{
	MMButton::handleEvent(event);	
	if (onMe(event) && user->input)  
	{
	  	if (event_type == USER_LEFT_DOWN)
	    {
			mix->currentPart = mix->mouth;
			mySlider->marker->x = 342 + mix->mouth->cel * (sliderWidth / mix->mouth->numCels);
			mySlider->marker->posn(mySlider->marker->x,mySlider->marker->y);
			return (true);
		}
	}
	return(false);
}

BeardBut::BeardBut()
{
	name = "beard";
	init(268, 295, 800, 5, 0);
	buttonDown = false;
	noSkip();
}

int BeardBut::handleEvent(MADEEventStamp* event)
{
	MMButton::handleEvent(event);	
	if (onMe(event) && user->input)  
	{
	  	if (event_type == USER_LEFT_DOWN)
	    {
			mix->currentPart = mix->beard;
			mySlider->marker->x = 342 + mix->beard->cel * (sliderWidth / mix->beard->numCels);
			mySlider->marker->posn(mySlider->marker->x,mySlider->marker->y);
			return (true);
		}
	}
	return(false);
}
/***********************************
 *		
 *			Dialog Box  
 *		      
 **********************************/
MatchBox::MatchBox()
{
//	init(290, 91);
	matchDialog = this;
	setting[0] = hairColor;
	setting[1] = eyeColor;
	setting[2] = skinColor;

	hairMark = new(RadioMark);
	eyeMark = new(RadioMark);
	skinMark = new(RadioMark);
	genderMark = new(RadioMark);
	new(MatchDoneButton);
	new(BrownEyes);
	new(BlueEyes);
	new(GreenEyes);
	new(Male);
	new(Female);
	new(DarkHair);
	new(BrownHair);
	new(BlondeHair);
//	new(RedHair);
	new(FairSkin);
	new(DarkSkin);
	new(MediumSkin);

	intrpuzz->add(this);
}
	
RadioMark::RadioMark()
{
	x = 0;
	y = 0;
	view = 800;
	loop = 11;
	cel = 0;
	draw();
	intrpuzz->addToFront ( this );
//	setPri(600);
	hide();
}

MatchDoneButton::MatchDoneButton()
{
	view = 800;
	loop = 10;
	cel = 0;
	init(206, 332);
	noSkip();
	applybutt = this;
	hide();
}

void MatchDoneButton::activate()
{
	if(matchDialog->setting[0] != hairColor)
	{
		int hairView = 0;
		//int oldHairView = mix->hair->view;
						
		switch(hairColor)
		{
			case BROWN_HAIR:
				hairView = BROWNHAIR_VIEW;	
				break;
			
			case DARK_HAIR:
				hairView = BLACKHAIR_VIEW;	
				break;
				
			case BLONDE_HAIR:
				hairView = BLONDEHAIR_VIEW;	
				break;
			
			case RED_HAIR:
				hairView = REDHAIR_VIEW;	
				break;
		}
		int hairCel = mix->hair->cel;
		mix->hair->setView(hairView);
		mix->hair->setLoop(mix->face->cel);
		mix->hair->setCel(hairCel);
		mix->hair->setPri(HAIRPRI);

		CleanUpSprites();
	}

	if(matchDialog->setting[1] != eyeColor)
	{
		mix->irises->setLoop(eyeColor);
		mix->irises->setCel(mix->face->cel);
	}
						
	if(matchDialog->setting[2] != skinColor)
	{
		int partsView = 0;
		int eyesView = 0;
		int beardView = 0;

		int faceCel = mix->face->cel;
		int mouthCel = mix->mouth->cel;
		int noseCel = mix->nose->cel;
		int eyesCel = mix->eyes->cel;
		int beardCel = mix->beard->cel;

		switch(skinColor)
		{
			case FAIR_SKIN: 
				partsView = FAIRPARTS;
				eyesView = EYESFAIR;		
				beardView = FACEHAIRFAIR;
				break;
		
			case MEDIUM_SKIN:
				partsView = MEDIUMPARTS;
				eyesView = EYESMEDIUM;
				beardView = FACEHAIRMEDIUM;
				break;

			case DARK_SKIN:
				partsView = DARKPARTS;
				eyesView = EYESDARK;
				beardView = FACEHAIRDARK;
				break;
		}
		mix->eyes->setView(eyesView);
		mix->eyes->setLoop(faceCel);
		mix->eyes->setCel(eyesCel);
		mix->eyes->setPri(EYESPRI);

		mix->face->setView(partsView);
		mix->face->setLoop(0);
		mix->face->setCel(faceCel);
		mix->face->setPri(HEADPRI);

		mix->mouth->setView(partsView);
		mix->mouth->setLoop(1);
		mix->mouth->setCel(mouthCel);
		mix->mouth->setPri(MOUTHPRI);

		mix->nose->setView(partsView);
		mix->nose->setLoop(2);
		mix->nose->setCel(noseCel);
		mix->nose->setPri(NOSEPRI);

		mix->beard->setView(beardView);
		mix->beard->setLoop(faceCel);
		mix->beard->setCel(beardCel);
		mix->beard->setPri(EYESPRI);

		CleanUpSprites();
	//	mix->currentPart = mix->face;
	}	
	matchDialog->setting[0] = hairColor;
	matchDialog->setting[1] = eyeColor;
	matchDialog->setting[2] = skinColor;
	hide();
}

/***********************************
 *		
 *			Dialog Box Features  
 *		      
 **********************************/
BrownEyes::BrownEyes()
{
	init (96, 310, 50, 15);
	type = BROWN_EYES;
	setMark();
}

BlueEyes::BlueEyes()
{
	init (154, 310, 50, 15);
	type = BLUE_EYES;
	setMark();
}

GreenEyes::GreenEyes()
{
	init (211, 310, 50, 15);
	type = GREEN_EYES;
	setMark();
}

void EyesFeature::setMark()
{
	if(type == eyeColor)
	{
		matchDialog->eyeMark->posn(left + 4, top + 4);
		matchDialog->eyeMark->show();
	}
}

int EyesFeature::handleEvent(MADEEventStamp* event)
{
	if (onMe(event) && user->input && !idWin->onMe(event))  
	{
	  	if (event_type == USER_LEFT_UP)
	    {
			matchDialog->eyeMark->posn(left + 4, top + 4);
			eyeColor = type;
			applybutt->activate();
		}
		return true;
	}
	return false;
}


Male::Male()
{
	init (96, 356, 78, 15);
	type = MALE;
	setMark();
}

Female::Female()
{
	init (180, 356, 78, 15);
	type = FEMALE;
	setMark();
}
				 
void GenderFeature::setMark()
{
	if(type == gender)
	{
		matchDialog->genderMark->posn(left + 4, top + 4);
		matchDialog->genderMark->show();
	}
}

int GenderFeature::handleEvent(MADEEventStamp* event)
{
	if (onMe(event) && user->input && !idWin->onMe(event))  
	{
	  	if (event_type == USER_LEFT_UP)
	    {
			matchDialog->genderMark->posn(left + 4, top + 4);
			gender = type;
			applybutt->activate();
 		}
		return true;
	}
	return false;
}

DarkHair::DarkHair()
{
	init (154, 287, 50, 15);
	type = DARK_HAIR;
	setMark();
}

BrownHair::BrownHair()
{
	init (96, 287, 50, 15);
	type = BROWN_HAIR;
	setMark();
}

BlondeHair::BlondeHair()
{
	init (211, 287, 50, 15);
	type = BLONDE_HAIR;
	setMark();
}

RedHair::RedHair()
{
	init (216, 287, 40, 15);
	type = RED_HAIR;
	setMark();
}

void HairFeature::setMark()
{
	if(type == hairColor)
	{
		matchDialog->hairMark->posn(left + 4, top + 4);
		matchDialog->hairMark->show();
	}
}
	
int HairFeature::handleEvent(MADEEventStamp* event)
{
	if (onMe(event) && user->input && !idWin->onMe(event))  
	{
	  	if (event_type == USER_LEFT_UP)
	    {
			matchDialog->hairMark->posn(left + 4, top + 4);
			hairColor = type;
			applybutt->activate();
 		}
		return true;
	}
	return false;
}


FairSkin::FairSkin()
{
	init (96, 333, 50, 15);
	type = FAIR_SKIN;
	setMark();
}

MediumSkin::MediumSkin()
{
	init (154, 333, 50, 15);
	type = MEDIUM_SKIN;
	setMark();
}

DarkSkin::DarkSkin()
{
	init (211, 333, 50, 15);
	type = DARK_SKIN;
	setMark();
}

void SkinFeature::setMark()
{
	if(type == skinColor)
	{
		matchDialog->skinMark->posn(left + 4, top + 4);
		matchDialog->skinMark->show();
	}
}
	
int SkinFeature::handleEvent(MADEEventStamp* event)
{
	if (onMe(event) && user->input && !idWin->onMe(event))  
	{
	  	if (event_type == USER_LEFT_UP)
	    {
			matchDialog->skinMark->posn(left + 4, top + 4);
	  		skinColor = type;	
			applybutt->activate();
 		}
		return true;
	}
	return false;
}

/***********************************
 *		
 *			Views  
 *		      
 **********************************/
MixBack::MixBack()
{
	view = 831;
	loop = 0;
	cel = 0;
	init(0, 16);
	noSkip();
//	setPri(1);
	addToPic();
	dispose();
}

void BodyParts::init(int theX, int theY, int View, int Loop, int Cel, int num_cels, int Priority)
{
	view = View;
	loop = Loop;
	cel = Cel;
	currentCel = 0;
	numCels = num_cels;
	IntrPuzzView::init(theX, theY);
	setPri(Priority);
}	

void BodyParts::dispose()
{
	IntrPuzzView::dispose();
}
	 
/***********************************
 *		
 *			  Sliders
 *		      
 **********************************/
ImageSlider::ImageSlider()
{
	init(342, 316, 142, 16, 800, 7);
	sliderWidth = 142; 
}

int ImageSlider::handleEvent(MADEEventStamp* event)
{
	if (user->input)
	{
		if (event_type == USER_LEFT_UP)
		{
			buttonDown = false;
		}													 

		if (onMe(event))
		{
			if (event_type == USER_LEFT_DOWN)
			{
				buttonDown = true;
			}													 

			if (buttonDown)
			{
				marker->x = event->cursor_pt.x;	
				if(marker->x >= (342 + (sliderWidth - 16)))
					marker->x = 342 + (sliderWidth - 16);
									
				marker->posn(marker->x,marker->y);
				step = (marker->x - left) / (sliderWidth / (mix->currentPart->numCels));
				if(mix->currentPart == mix->face)
				{
					int theCel;
				
					mix->face->setCel(step);	

					mix->irises->setCel(step);	
			
					theCel = mix->hair->cel;
					mix->hair->setLoop(step);
					mix->hair->setCel(theCel);
				
					theCel = mix->eyes->cel;
					mix->eyes->setLoop(step);						
					mix->eyes->setCel(theCel);						

					theCel = mix->hair->cel;
					mix->hair->setLoop(step);
					mix->hair->setCel(theCel);
			
					theCel = mix->beard->cel;
					mix->beard->setLoop(step);						
					mix->beard->setCel(theCel);						
				}
				else
				{
					mix->currentPart->setCel(step);	
				}
		
			}
			return (true);
		}
	}
	return (false);	
}

LeftArrow::LeftArrow()
{
	init(322, 316, 800, 8, 0);
	setPri(2);
	noSkip();
}
		 
void LeftArrow::activate()
{
	int theCel;
	BodyParts* current_part = mix->currentPart;

	if(current_part == mix->face)
	{
		if(mix->face->cel == 0)
		{
			return;
		}						
		else
		{
			int faceCel = mix->face->cel - 1;
			mix->irises->setCel(faceCel);	
			
			theCel = mix->hair->cel;
			mix->hair->setLoop(faceCel);
			mix->hair->setCel(theCel);	

			theCel = mix->eyes->cel;
			mix->eyes->setLoop(faceCel);						
			mix->eyes->setCel(theCel);						

			theCel = mix->beard->cel;
			mix->beard->setLoop(faceCel);						
			mix->beard->setCel(theCel);						
			
		}
	}
	else
	{
		if(current_part->cel == 0)
		{
			return;
		}
	}
	theCel = current_part->cel - 1;
	current_part->setCel(theCel);	
	mySlider->marker->x = 342 + (theCel * (sliderWidth / current_part->numCels));
	mySlider->marker->posn(mySlider->marker->x,mySlider->marker->y);
}

RightArrow::RightArrow()
{
	init(485, 316, 800, 9, 0);
	setPri(2);
	noSkip();
}

void RightArrow::activate()
{
	int theCel;
	BodyParts* current_part = mix->currentPart;

	if(current_part == mix->face)
	{
		if(mix->face->cel >= (mix->face->numCels - 1))
		{
			return;
		}					
		else
		{
			int faceCel = mix->face->cel + 1;

			mix->irises->setCel(faceCel);	
			
			theCel = mix->hair->cel;
			mix->hair->setLoop(faceCel);
			mix->hair->setCel(theCel);	
			
			theCel = mix->eyes->cel;
			mix->eyes->setLoop(faceCel);						
			mix->eyes->setCel(theCel);						

			theCel = mix->beard->cel;
			mix->beard->setLoop(faceCel);						
			mix->beard->setCel(theCel);						
		
		}
	}
	else
	{
		if(current_part->cel >= (current_part->numCels  - 1))
		{
			return;
		}
	}
	theCel = current_part->cel + 1;
	current_part->setCel(theCel);	
	mySlider->marker->x = 342 + theCel * (sliderWidth / current_part->numCels);
	mySlider->marker->posn(mySlider->marker->x,mySlider->marker->y);
}

/***********************************
 *		
 *			  ID window
 *		      
 **********************************/
IdTitleBar::IdTitleBar()
{
	init(idWin->idWin_x + 4, idWin->idWin_y + 4, 108, 17);
}
	
int IdTitleBar::handleEvent ( MADEEventStamp *event )
{
	if (user->input && onMe(event))
	{
	  	if (event_type == USER_LEFT_DOWN)
		{
			mix->windowDrag = true;
			mix->box_originX = mouse_x - idWin->idWin_x;
			mix->box_originY = mouse_y - idWin->idWin_y;
			iddragbox = new(IdDragBox);
			return true; //just added
//			dispose();
		}
	}
	return false;	
}

MatchPhoto::MatchPhoto(int PhotoView)
{
	view = PhotoView;
	loop = 0;
	cel = 0;
	init(idWin->idWin_x + 7, idWin->idWin_y + 25);
	idWin->matchphoto = this;
	noSkip();
	setPri(555);
}

WindowText::WindowText()
{
	view = 800;
	loop = 19;
	cel = 0;
	init(idWin->idWin_x + 22, idWin->idWin_y + 127);
	noSkip();
	setPri(530);
}
	
IdWinPrev::IdWinPrev()
{
	idWin->idwinprev = this;
	init(idWin->idWin_x + 7, idWin->idWin_y + 173, 800, 14, 0);
	noSkip();
	setPri(512);
}

void IdWinPrev::activate()
{
	if(mix->currentMatchIndex)
	{
		mix->currentMatchIndex--;
		mix->currentPhoto->setView(mix->matchPhotoViews[mix->currentMatchIndex]);
		mix->currentPhoto->setPri(555);
		mix->currentPhoto->noSkip();
	
		if(mix->currentMatchIndex == 0)
			hide();
		idWin->idwinnext->show();
	}
}

IdWinNext::IdWinNext()
{
	idWin->idwinnext = this;
	init(idWin->idWin_x + 40, idWin->idWin_y + 173, 800, 13, 0);
	noSkip();
	setPri(512);
}

void IdWinNext::activate()
{
	if(mix->currentMatchIndex < (mix->numMatches - 1))
	{
		mix->currentMatchIndex++;
		mix->currentPhoto->setView(mix->matchPhotoViews[mix->currentMatchIndex]);
		mix->currentPhoto->setPri(555);
		mix->currentPhoto->noSkip();

		if(mix->currentMatchIndex == (mix->numMatches - 1))
			hide();
		idWin->idwinprev->show();
	}
}

IdWinInfo::IdWinInfo()
{
	idWin->idwininfo = this;
	init(idWin->idWin_x + 74, idWin->idWin_y + 173, 800, 15, 0);
	noSkip();
	setPri(512);
}

void IdWinInfo::activate()
{
	mix->saveState();
	intrpuzz->dispose();
	new HTMLPage(mix->matchHTML[mix->currentMatchIndex]);
}

IdWinClose::IdWinClose()
{
	init(idWin->idWin_x + 119, idWin->idWin_y + 4, 800, 16, 1);
	noSkip();
	setPri(512);
}

void IdWinClose::activate()
{
/*	if(idWin->idanim)
	{
		return;
	}
*/	idWin->closed = true;
	idWin->setCel(1);
	idWin->posn(277, 347);
	idWin->windowtext->hide();
	if(idWin->matchphoto)
	{
		idWin->matchphoto->hide();
	}
	if(idWin->idwininfo)
	{
		idWin->idwininfo->hide();
	}
	if(idWin->idwinnext)
	{
		idWin->idwinnext->hide();
	}
	if(idWin->idwinprev)
	{
		idWin->idwinprev->hide();
	}
	if(idWin->idtitlebar)
	{
		idWin->idtitlebar->dispose();
		idWin->idtitlebar = NULL;
	}
	idWin->idwinopen = new(IdWinOpen);
	dispose();
}

IdWinOpen::IdWinOpen()
{
	init(277 + 119, 347 + 4, 800, 17, 0);
	noSkip();
	setPri(512);
}

void IdWinOpen::activate()
{
 	idWin->closed = false;
	mix->setScript(new maximizeScript(277, 347, idWin->idWin_x, idWin->idWin_y, NOSEARCH));
	dispose();
}

IdWindow::IdWindow(int theX, int theY)
{
	SRect theRect;

	name = "idWindow";
	idWin = this;
	idWin_x = theX;
	idWin_y = theY;
	view = 800;
	loop = 12;
	cel = 1;
	closed = true;
	idtitlebar = NULL;
	idanim = NULL;
	matchphoto = NULL;
	init(0, 0);
	idwininfo = NULL;
	idwinprev = NULL;
	idwinnext = NULL;
	idwinopen = new(IdWinOpen);
	windowtext = new(WindowText);
	windowtext->hide();
	setPri(500);

	SRect_Init ( &theRect, 0, 0, 166, 170);

	sfxSetDrawInfo (ADV_DRAW_SCALEX, 32);
	sfxSetDrawInfo (ADV_DRAW_SCALEY, 32);
	minimize = sfxClipSprite ( mySprite, 86, 352, &theRect );
	sfxHideSprite(minimize);
	posn(theX, theY);
	noSkip();
	idWin_y = 125;
	idWin_x = 125;
}

IdWindow::~IdWindow()
{
	sfxKillSprite(minimize);
}

int IdWindow::handleEvent ( MADEEventStamp *event )
{
	if (user->input && onMe(event))
	{
		return true; //just added
	}
	return false;
}

void IdWindow::moveTo(int theX, int theY)
{
	idWin_x = theX;
	idWin_y = theY;

	idWin->posn(theX, theY);
	idwinclose->posn(idWin_x + 119, idWin_y + 4);
	windowtext->posn(idWin_x + 22, idWin_y + 127);
	if(matchphoto)
	{
		matchphoto->posn(idWin_x + 7, idWin_y + 25);
	}		
/*	if(idanim)
	{
		idanim->posn(idWin_x + 36, idWin_y + 39);
	}
*/	if(idtitlebar)
	{
		idtitlebar->dispose();
	}											    
	if(idwininfo)
	{
		idwininfo->posn(idWin_x + 74, idWin_y + 173);
	}
	if(idwinnext)
	{
		idwinnext->posn(idWin_x + 40, idWin_y + 173);
	}
	if(idwinprev)
	{
		idwinprev->posn(idWin_x + 7, idWin_y + 173);
	}

	idtitlebar = new(IdTitleBar);
}

void IdWindow::searchData()
{
//	int partNumber = 0;
	BodyParts *currPart = nullptr;
	bool match;
	int index;

	for(int i = 0; i < MAX_PHOTOS; i++)
	{
		index = 0;
		for(int partNumber = 0; partNumber < 7; partNumber++)
		{
			match = false;
			switch(partNumber)
			{
				case 0:	  //face
					currPart = mix->face;
					break;

				case 1:
					currPart = mix->eyes;
					break;
				
				case 2:
					currPart = mix->nose;
					break;

				case 3:
					currPart = mix->mouth;
					break;
				
				case 4:
					currPart = mix->hair;
					break;

				case 5:
					currPart = mix->beard;
					break;

				case 6:					 
					if(skinColor == matches[i][index])
					{
						if(((i != MCGRUDER) && (gender == MALE)) || ((i == MCGRUDER) && (gender == FEMALE))) 
						{
							int doTheStupidMatch = false;
							//big sucky cludge
							switch(intrpuzz->page->site->flag)
							{
								case WEB_GRENDELMATCH:
								if(mix->PhotoView != 99912)
								{
									if((i == COHEN) || (i == BENTON) || (i == MCGRUDER) || (i == SUNG))
											doTheStupidMatch = true;
								}
								else if(i == GRENDEL) 
									doTheStupidMatch = true;
								break;
								case WEB_KATMATCH:
									if(i == HARMONICA)	
										doTheStupidMatch = true;
									break;
								default:
									doTheStupidMatch = true;
									break;		
							}
							if(doTheStupidMatch)
							{
								index++;
								mix->matchPhotoViews[mix->numMatches] = matchViews[i];
								mix->matchHTML[mix->numMatches] = matchDossier[i]; 
								peopleFound[i] = true;
								mix->numMatches++;
							}
						}
					}		
			}	  

			if(partNumber == 6)
			{	
				break;
			}

			do
			{
				if(matches[i][index] == currPart->cel)
				{
					match = true;
				}
				index++;
			}while(matches[i][index] != SEP);

			index++;
			if(!match)
			{
				break;
			}
		}
	}
	if(mix->numMatches)
	{
		mix->currentMatchIndex = 0;
		mix->currentPhoto = new MatchPhoto(mix->matchPhotoViews[0]);
		new(IdWinInfo);
		if(mix->numMatches > 1)
		{
			new(IdWinNext);
			new(IdWinPrev);
			idwinprev->hide();
		}
	}
}

maximizeScript::maximizeScript(int startX, int startY, int endX, int endY, bool doSearch)
{
	name = "Explode";
	begin_x = startX;
	begin_y = startY;
	dest_x = endX;
	dest_y = endY;
	search = doSearch;

	user->input = false;

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

void maximizeScript::changeState ( int newState )
{
	SRect newRect;

	switchTo
		sfxSetDrawInfo (ADV_DRAW_SCALEX, 1024);
		sfxSetDrawInfo (ADV_DRAW_SCALEY, 1024);
		SRect_Init ( &newRect, 0, 0, idWin->minimize->rect.width, idWin->minimize->rect.height);
		newsprite = sfxClipSprite ( idWin->minimize, begin_x + move_x, begin_y - move_y, &newRect);
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
		idWin->idwinclose = new(IdWinClose);
		idWin->setCel(0);
		idWin->moveTo(idWin->idWin_x, idWin->idWin_y);
		idWin->windowtext->show();
		sfxKillSprite(newsprite);
		sfxHideSprite(idWin->minimize);
		if(search)
		{
			idWin->windowtext->setCel(2);
			ticks = 8;
		}	
		else
		{
			if(idWin->matchphoto)
			{
				idWin->matchphoto->show();
			}

			if(idWin->idwininfo)
			{
				idWin->idwininfo->show();
			}
			if(idWin->idwinprev)
			{
				if(mix->currentMatchIndex > 0)
				{
					idWin->idwinprev->show();
				}
				if(mix->currentMatchIndex < mix->numMatches)
				{
					idWin->idwinnext->show();						
				}
			}
			user->input = true;
			dispose();
		}
	END			
	BEG
		dispose();
		if(GameFlag.test(fGreg39))
			new IdAnim(true); //dos
		else
			new IdAnim(idWin->idWin_x + 7, idWin->idWin_y + 25);

//		user->input = true;
	END
}


IdDragBox::IdDragBox()
{
	name = "iddragBox";
	view = 800;
	loop = 18;
	cel = 0;
	init(idWin->idWin_x, idWin->idWin_y);
	setPri(600);
}

MixMatchWin::MixMatchWin()
{
	init(0, 60, 512, 320);
}

int MixMatchWin::handleEvent ( MADEEventStamp *event )
{
	if (user->input)
	{
	  	if (mix->windowDrag)
		{
			int cursor_x;
			int cursor_y;
			int objectLeft = mouse_x - mix->box_originX;
			int objectTop = mouse_y - mix->box_originY;
			int objectRight	= objectLeft + iddragbox->width;
			int objectBottom = objectTop + iddragbox->height;
			bool ySet = true;
			bool xSet = true;

			if(objectLeft < left)
			{
				cursor_x = left + mix->box_originX;
				objectLeft = left;
			}		
			else if(objectRight > right)
			{
				cursor_x = (right - iddragbox->width) + mix->box_originX;
				objectLeft = right - iddragbox->width;
			}
			else
			{
				xSet = false;
				cursor_x = mouse_x;
			}

		 	if(objectTop < top)
			{
				cursor_y = top + mix->box_originY;
				objectTop = top;
			}
			else if(objectBottom > bottom)
			{
				cursor_y = (bottom - iddragbox->height) + mix->box_originY;
				objectTop = bottom - iddragbox->height;
			}	
			else
			{
				ySet = false;
				cursor_y = mouse_y;
			}
			
			if(ySet || xSet)
				sfxSetCursorPos(cursor_x, cursor_y);

			iddragbox->posn(objectLeft, objectTop);
			if((event_type == USER_LEFT_UP) || !onMe(event))
			{
				mix->windowDrag = false;
				idWin->moveTo(objectLeft, objectTop);	
				iddragbox->dispose();
			}
			return true;
		}
	}
	return false;
}

void IdAnimScript::changeState ( int newState )
{
	switchTo
		mix->matchSFX->play(40041, this);
	//	mix->scanSFX->loop = 10;
//		mix->scanSFX->play(131, this);
	END
/*	BEG
		ticks = 10;
	END
	BEG
		mix->scanSFX->loop = 10;
		mix->scanSFX->play(131, this);
	END*/
	BEG
		dispose();
	END
}	
			
IdAnim::IdAnim(int theX, int theY)
{
	name = "idanim";
	cycleSpeed = 3;
	view = 827;
	loop = 0;
	cel = 0;
	x = theX;
	y = theY;
	loopCtr = 0;
	draw();
	intrpuzz->addToFront ( this );
	noSkip();
	setPri(525);

	idWin->idanim = this;
//	idWin->windowtext->setCel(2);
	idWin->windowtext->hide();
	user->canInput(false);

//	mix->setScript(new IdAnimScript);
	mix->matchSFX->stop();
	mix->matchSFX->play(40041, NULL);
//	mix->scanSFX->loop = 10;
//	mix->scanSFX->play(131, NULL);
	setCycle(new EndLoop((Prop*)this, this));
}		

IdAnim::IdAnim(int Dos)
{
	idWin->idanim = this;
	idWin->windowtext->hide();
	user->canInput(false);
	loopCtr = 0;

	cue();
}

void IdAnim::cue()
{
//	EndLoop* temp;
//	int endFrame;

	switch(loopCtr)
	{
/*		case 0:
			temp = new EndLoop((Prop*)this, this);
			endFrame = rand() & 81;
			temp->lastCel = endFrame;
			setCycle(temp);
			loopCtr++;
			break;
*/
		case 0:
			mix->numMatches = 0;
	//		mix->scanSFX->stop();	
			mix->matchSFX->stop();

			if(!GameFlag.test(fGreg39))
			{
				hide();
				CleanUpSprites();
			}

 			idWin->searchData();
			if(mix->numMatches)
			{
				idWin->windowtext->hide();
				mix->matchSFX->play(45071, NULL);
			}			
			else
			{
				idWin->windowtext->setCel(1);
				idWin->windowtext->show();
				mix->matchSFX->play(45033, NULL);
			}
			user->canInput(true);
			dispose();
			break;
	}
}

IdAnim::~IdAnim()
{
	idWin->idanim = NULL;
}		

} // namespace Spycraft
