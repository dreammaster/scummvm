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

#include "spycraft/game/sound.h"
#include "spycraft/game/game.h"				 
#include "spycraft/dmade/advsound.h"
#include "spycraft/dmade/advres.h"
#include "spycraft/game/flag.h"							   														    
#include "spycraft/game/badman.h"					 
#include "spycraft/dmade/advscreen.h"

namespace Spycraft {

extern BlinkCursor* normalCursor;

/*********************************
*
*	DATA
*
**********************************/						   
FloorMap mastermap[22]={99, 1, 4,99, NULL, NULL, NULL, NULL,			
						99, 2, 5, 0, NULL, NULL, NULL, NULL,			
						99, 3, 6, 1, NULL, NULL, NULL, NULL,			
						99,99, 7, 2, NULL, NULL, NULL, NULL,			
						 0, 5, 8,99, NULL, NULL, NULL, NULL,
						 1, 6, 9, 4, NULL, NULL, NULL, NULL,			
						 2, 7,10, 5, NULL, NULL, NULL, NULL,
			 			 3,99,11, 6, NULL, NULL, NULL, NULL,
						 4, 9,12,20, NULL, NULL, NULL, NULL,
						 5,10,13, 8, NULL, NULL, NULL, NULL,
						 6,11,14, 9, NULL, NULL, NULL, NULL,
						 7,21,15,10, NULL, NULL, NULL, NULL,
						 8,13,16,99, NULL, NULL, NULL, NULL,
						 9,14,17,12, NULL, NULL, NULL, NULL,			
		 				10,15,18,13, NULL, NULL, NULL, NULL,			
						11,99,19,14, NULL, NULL, NULL, NULL,			
						12,17,99,99, NULL, NULL, NULL, NULL,			
						13,18,99,16, NULL, NULL, NULL, NULL,			
						14,19,99,17, NULL, NULL, NULL, NULL,			
						15,99,99,18, NULL, NULL, NULL, NULL,			
						99, 8,99,99, NULL, NULL, NULL, NULL,			
						99,99,99,11, NULL, NULL, NULL, NULL};

int temp_walls[100]={NULL,NULL,DOOR,NONE,NONE,NONE,NONE,NULL,NULL,NULL,DOOR,DOOR,NULL,DOOR,DOOR,NONE,NONE,
				     NONE,DOOR,DOOR,DOOR,DOOR,DOOR,NULL,NULL,NULL,DOOR,NONE,DOOR,NONE,NULL,NULL,NULL,
					 
					 DOOR,NULL,DOOR,NONE,NONE,DOOR,NONE,NULL,NULL,NULL,DOOR,NULL,DOOR,NULL,DOOR,NONE,NONE,
				     NONE,DOOR,DOOR,DOOR,NULL,DOOR,NULL,DOOR,NULL,NONE,NONE,NULL,NONE,NULL,NULL,DOOR,
					 
					 NULL,NONE,NULL,NONE,NULL,NULL,NONE,NULL,NONE,NULL,DOOR,NONE,NONE,DOOR,DOOR,DOOR,NONE,
				     DOOR,DOOR,DOOR,NONE,NONE,DOOR,NULL,NONE,NULL,NONE,NULL,NULL,NONE,NULL,NONE,NULL};


int temp_enemy[70]={LOFT  ,NULL  ,FAM   ,FAM   ,
					LOFT  ,DOG   ,FAM   ,FAM   ,
					HALL  ,HALL  ,HALL  ,HALL  ,
					SALES ,MUSIC ,R1    ,R3	   ,
					SALES ,MUSIC ,R2    ,R3    ,
					HALL  ,HALL  ,

					PARTY ,PARTY ,BIRD  ,R3    ,
					PARTY ,PARTY ,YEL3  ,R3    ,
					HALL  ,HALL  ,HALL  ,HALL  ,
					FUCK  ,YEL3  ,LOFT  ,R3    ,			  				
					FUCK  ,YEL3  ,YEL1  ,R3    ,
					HALL  ,HALL  ,

					NULL  ,NULL  ,NULL  ,PHONES,
					NULL  ,BOILER,BOILER,PHONES,
					NULL  ,BOILER,BOILER,NULL  ,
					CRAZY ,BOILER,BOILER,NULL  ,
					CRAZY ,NULL  ,NULL  ,NULL  ,
					HALL  ,HALL  };										  

extern int currentFocus;
List * team;

/************************
*
*  Interface Stuff
*
*************************/

BadMan::BadMan()
{
	name = "badman";
	show_style = VE_FLIP;
	sfxSetSpritelistSize(256);
	intrface->disable();
	init (610);
}
											 
void BadMan::init(int pic_num)
{
//	usedbutton = 		true; 
	
	micOpen = 			false;	
	theMusic->stop();
	IntrPuzz::init(pic_num);

	normalCursor->cel = 1;
	sfxSystemCursor(SYS_WAIT);
	theGame->handsOff();
/*
	sfxLoadRes(528, RES_WAVE);
	sfxLockRes(528, RES_WAVE);
	sfxLoadRes(534, RES_WAVE);
	sfxLockRes(534, RES_WAVE);
*/
	sfxLoadRes(608, RES_ATS);
	sfxLockRes(608, RES_ATS);
	sfxLoadRes(609, RES_ATS);
	sfxLockRes(609, RES_ATS);
	sfxLoadRes(610, RES_ATS);
	sfxLockRes(610, RES_ATS);
	sfxLoadRes(611, RES_ATS);
	sfxLockRes(611, RES_ATS);
	sfxLoadRes(612, RES_ATS);
	sfxLockRes(612, RES_ATS);
	sfxLoadRes(613, RES_ATS);
	sfxLockRes(613, RES_ATS);
	sfxLoadRes(614, RES_ATS);
	sfxLockRes(614, RES_ATS);
	sfxLoadRes(616, RES_ATS);
	sfxLockRes(616, RES_ATS);
	sfxLoadRes(617, RES_ATS);
	sfxLockRes(617, RES_ATS);
	sfxLoadRes(618, RES_ATS);
	sfxLockRes(618, RES_ATS);
//	sfxMadeCursor();

	page = 				NULL;
	currentFocus = 		PUZZLE_FOCUS;
	curFloorNum = 		1;
	checked = 			false;
	hideGuys = 			false;
	initWalls();
	initEnemy();
	intro = 			true;
	phones = 			false;
	done = 				false;
	death = 			false;
	oops = 				false;
	whoops = 			false;
	somebodyistalkingaboutphones = false;	
	casualties = 		0;
	web_intrface = 		new (List);
	memset 				(&guy,0,(4 * (sizeof(Us *))));
	memset 				(&strikers,0,(sizeof(strikers)));
	min = 				0;
	scanned = 			0;
	team = 				new (List);

	new BaseTab;
	new FirstTab;
	new SecondTab;

	tact = 				new (Tactical);
	Floors[0] =			new (Base);
	Floors[1] =			new (Floor1);
	Floors[2] =			new (Floor2);
	hend = 				new (Henderson);
	wash = 				new (Washington);
	schiff = 			new (Schiff);
	ag = 				new (Aguila);
	
	speech =			new (Sound);
	ambient =			new (Sound);
	back = new FloorBack(1);
											    
//	setScript(new (BadScript));
	setScript(new (SpeechScript));
}

BadMan::~BadMan()
{
	theMusic->stop();
	currentFocus = false;
	if (speech)	
	{
		speech->dispose();
		speech = NULL;
	}
	if (ambient)
	{
		ambient->dispose();								  
		ambient = NULL;
	}
//	sfxUnlockRes(528, RES_WAVE);
//	sfxUnlockRes(534, RES_WAVE);
	sfxUnlockRes(608, RES_ATS);
	sfxUnlockRes(609, RES_ATS);
	sfxUnlockRes(610, RES_ATS);
	sfxUnlockRes(611, RES_ATS);
	sfxUnlockRes(612, RES_ATS);
	sfxUnlockRes(613, RES_ATS);
	sfxUnlockRes(614, RES_ATS);
	sfxUnlockRes(616, RES_ATS);
	sfxUnlockRes(617, RES_ATS);
	sfxUnlockRes(618, RES_ATS);
}
void SpeechScript::changeState ( int newState )
{
	switchTo
		((BadMan *)client)->speech->canInterupt = true;
		((BadMan *)client)->speech->number = 516;
		((BadMan *)intrpuzz)->speech->playBuffered(516,this);
		((BadMan *)intrpuzz)->hend->butt->setScript(new(FlashScript));
	END							  
	
	BEG
		ticks = 15;
	END
	
	BEG
		((BadMan *)client)->speech->number = 515;
		((BadMan *)intrpuzz)->speech->playBuffered(515,this);
	END
	
	BEG
		ticks = 15;
	END
	
	BEG
		if (!GameFlag.test(fPatsHTM_129))
		{
			((BadMan *)client)->speech->number = 517;
			((BadMan *)intrpuzz)->speech->playBuffered(517,this);
			((BadMan *)intrpuzz)->hend->setScript(new WalkupScript);
		}						   
		else
			cue();
	END					    
					   
	BEG
		seconds = 2;
	END

	BEG
		if(((BadMan *)intrpuzz)->hend->script)
		{

			((BadMan *)intrpuzz)->hend->setMotion(NULL);
			((BadMan *)intrpuzz)->schiff->setMotion(NULL);
			((BadMan *)intrpuzz)->wash->setMotion(NULL);
			((BadMan *)intrpuzz)->ag->setMotion(NULL);

			((BadMan *)intrpuzz)->hend->setScript(NULL);
			((BadMan *)intrpuzz)->hend->posn(115,225);
			((BadMan *)intrpuzz)->wash->posn(105,235);
			((BadMan *)intrpuzz)->ag->posn(105,215);
			((BadMan *)intrpuzz)->schiff->posn(95,225);
		}
		((BadMan *)client)->speech->canInterupt = false;
		ticks = 15;
	END									  
	
	BEG										    
		((BadMan *)client)->intro = false;
		theGame->handsOn();
		sfxMadeCursor();
		theMusic->loop = -1;
		theMusic->setVolume(5);
		theMusic->play(400);
		GameFlag.set(fPatsHTM_129);
   		((BadMan *)client)->setScript(new (BadScript));	
	END
}

void BadScript::changeState ( int newState )
{
	switchTo
		seconds=60;
	END
					   
	BEG										    
   		((BadMan *)client)->cue();	
	END
}

void WalkupScript::changeState ( int newState )
{
	switchTo
		seconds=46;
	END
					   
	BEG										    
		((BadMan *)intrpuzz)->hend->setMotion( new MoveTo(33,225,(Actor*)((BadMan *)intrpuzz)->hend,this));
		((BadMan *)intrpuzz)->schiff->setMotion( new MoveTo(38,245,(Actor*)((BadMan *)intrpuzz)->schiff,NULL));
		((BadMan *)intrpuzz)->wash->setMotion( new MoveTo(33,235,(Actor*)((BadMan *)intrpuzz)->wash,NULL));
		((BadMan *)intrpuzz)->ag->setMotion( new MoveTo(33,215,(Actor*)((BadMan *)intrpuzz)->ag,NULL));
	END
	BEG										    
		((BadMan *)intrpuzz)->hend->setMotion( new MoveTo(115,225,(Actor*)((BadMan *)intrpuzz)->hend,this));
		((BadMan *)intrpuzz)->wash->setMotion( new MoveTo(105,235,(Actor*)((BadMan *)intrpuzz)->wash,NULL));
		((BadMan *)intrpuzz)->ag->setMotion( new MoveTo(105,215,(Actor*)((BadMan *)intrpuzz)->ag,NULL));
	END
	BEG										    
		((BadMan *)intrpuzz)->hend->setScript(NULL);
		((BadMan *)intrpuzz)->schiff->setMotion( new MoveTo(95,225,(Actor*)((BadMan *)intrpuzz)->schiff,NULL));
	END
}

void CopScript::changeState ( int newState )
{
	switchTo
		((BadMan *)client)->done = true;
		((BadMan *)client)->micOpen = true;

		((BadMan *)client)->speech->stop();
		((BadMan *)client)->speech->number = 579;
		((BadMan *)intrpuzz)->speech->playBuffered(579,this);
	END			   
					   
	BEG								  
		team->dispose();
		GameFlag.set(fWahKa37);		  
		intrpuzz->quitIntrface();
	END
}
									 
void BadMan::doit()
{
	if (casualties > 3 && !((BadMan *)intrpuzz)->micOpen)
	{
//		if (!done)
//		{
//			sfxPrintf("you LOSE!!!");
//			sfxMadeCursor();
			team->dispose();			 		   
			intrpuzz->quitIntrface();				 
//		}
			return;
	}
	IntrPuzz::doit();
	team->doit();
}
void Selected::doit()
{
	posn(guy->x,guy->y);
}

int BadMan::handleEvent ( MADEEventStamp *event )
{
	int ret = false;

	if (!((BadMan *)intrpuzz)->done)
	{
		ret = team->handleEvent (event);
	
		if (!ret)
		{
			ret = IntrPuzz::handleEvent ( event );
		}	
		if (checked > 2)
		{
			checked = 0;
			hideGuys = false;
		}
	}
	return ret;
}
void BadMan::cue(int dummy)
{
	BadMan::cue();
}

void BadMan::cue(void)
{
	if (done)
	{
		team->dispose();
		if (casualties<4)
			GameFlag.set(fRescuedBS);
			
		intrpuzz->quitIntrface();
		return;
	}
	else
	{
		min ++;

		if ((min > 6 && !phones) || (min > 8))
		{
			setScript(new (CopScript));
			return;
		}
		else
		{
			if (!scanned)
			{
				if(min && min%2) 
					if (((BadMan *)intrpuzz)->hend->alive)			
						((BadMan *)intrpuzz)->hend->talk(494);			
					else if	(((BadMan *)intrpuzz)->ag->alive)			
						((BadMan *)intrpuzz)->ag->talk(495);							
				else if (min)
					if (((BadMan *)intrpuzz)->ag->alive)			
						((BadMan *)intrpuzz)->hend->talk(494);			
					else if	(((BadMan *)intrpuzz)->hend->alive)			
						((BadMan *)intrpuzz)->ag->talk(495);							
			}
			setScript(new (BadScript));
		}
	}
}
 
/************************
*
*  Buttons
*											  
*************************/

void BaseTab::activate(void)
{
	int a=0;

	if (((BadMan *)intrpuzz)->curFloorNum)
	{
		sfxPlaySound (528,100,64,64,NULL);

		((BadMan *)intrpuzz)->hideGuys = true;

		((BadMan *)intrpuzz)->curFloorNum = 0;

		if (((BadMan *)intrpuzz)->back)
		{
			((BadMan *)intrpuzz)->back = new FloorBack(((BadMan *)intrpuzz)->curFloorNum);
		}
		((BadMan*)intrpuzz)->Floors[((BadMan *)intrpuzz)->curFloorNum]->room[0];

		for(a=0;a<21;a++)
		{
			((BadMan*)intrpuzz)->Floors[((BadMan *)intrpuzz)->curFloorNum]->room[a]->check4Hole();
		}
		for(a=0;a<22;a++)
		{
			if (((BadMan*)intrpuzz)->strikers[a])
			{
				intrpuzz->release (((BadMan *)intrpuzz)->strikers[a]);
				deathList->add(((BadMan *)intrpuzz)->strikers[a]);
				((BadMan *)intrpuzz)->strikers[a] = NULL;	
			}
		}
		((BadMan *)intrpuzz)->tact->oakalydokaly = true;	
	}
}

void FirstTab::activate(void)
{
	int a=0;
	if (((BadMan *)intrpuzz)->curFloorNum != 1)
	{
		sfxPlaySound (528,100,64,64,NULL);

		((BadMan *)intrpuzz)->hideGuys = true;

		((BadMan *)intrpuzz)->curFloorNum = 1;

		if (((BadMan *)intrpuzz)->back)
		{
			((BadMan *)intrpuzz)->back = new FloorBack(((BadMan *)intrpuzz)->curFloorNum);
		}

		for(a=0;a<21;a++)
		{
			((BadMan*)intrpuzz)->Floors[((BadMan *)intrpuzz)->curFloorNum]->room[a]->check4Hole();
		}
		for(a=0;a<22;a++)
		{
			if (((BadMan*)intrpuzz)->strikers[a])
			{
				intrpuzz->release (((BadMan *)intrpuzz)->strikers[a]);
				deathList->add(((BadMan *)intrpuzz)->strikers[a]);
				((BadMan *)intrpuzz)->strikers[a] = NULL;	
			}
		}
		((BadMan *)intrpuzz)->tact->oakalydokaly = true;	
	}
}
void SecondTab::activate(void)
{
	int a=0;
	if (((BadMan *)intrpuzz)->curFloorNum != 2)
	{
		sfxPlaySound (528,100,64,64,NULL);

		((BadMan *)intrpuzz)->hideGuys = true;

		((BadMan *)intrpuzz)->curFloorNum = 2;

		if (((BadMan *)intrpuzz)->back)
		{
			((BadMan *)intrpuzz)->back = new FloorBack(((BadMan *)intrpuzz)->curFloorNum);
		}

		for(a=0;a<21;a++)
		{
			((BadMan*)intrpuzz)->Floors[((BadMan *)intrpuzz)->curFloorNum]->room[a]->check4Hole();
		}
		for(a=0;a<22;a++)
		{
			if (((BadMan*)intrpuzz)->strikers[a])
			{
				intrpuzz->release (((BadMan *)intrpuzz)->strikers[a]);
				deathList->add(((BadMan *)intrpuzz)->strikers[a]);
				((BadMan *)intrpuzz)->strikers[a] = NULL;	
			}
		}
		((BadMan *)intrpuzz)->tact->oakalydokaly = true;	
	}
}

void Tactical::activate(void)
{
	int xp = 0;
	int count = 0;
	Floor * floor = ((BadMan *)intrpuzz)->Floors[((BadMan *)intrpuzz)->curFloorNum];
	Movement * strike;
	BadRoom * room;	
	int	num = rand() & 3;
	
	if (oakalydokaly)
	{	
		sfxPlaySound (519,100,64,64,NULL);
		for ( xp = 0; xp < 20; xp++ ) 
		{
			if ((*(floor->fenemy[xp]) < DED && *(floor->fenemy[xp]))/* || (*(floor->fenemy[xp]) == PARTY)*/)
			{
				oakalydokaly = false;
				room = floor->room[xp];
				strike = new (Movement);
				for (count=0;count<22;count++)
				{
					if(!((BadMan *)intrpuzz)->strikers[count])		  
					{
						((BadMan *)intrpuzz)->strikers[count] = strike;
						break;
					}
				}
				if (room->s_wall)
				{
					if (*room->s_wall == NONE)
					{
						strike->init(room->left + 14,room->top + 12,618,1,0);
						strike->setPri(600);
						strike->setCycle(new EndLoop((Prop*)strike, strike));
					}
					else
					{	
						if(room->n_wall)
						{	
							if (*room->n_wall != NONE)
							{
								strike->init(room->left + 10,room->top + 12,618,0,0);
								strike->setPri(599);
								strike->setCycle(new EndLoop((Prop*)strike, strike));
							}
						}
					}
				}
				else
				{
					if(room->n_wall)
					{	
						if (*room->n_wall != NONE)
						{
							strike->init(room->left + 10,room->top + 12,618,0,0);
							strike->setPri(599);
							strike->setCycle(new EndLoop((Prop*)strike, strike));
						}
					}
					else
					{

						strike->init(room->left + 10,room->top + 12,618,0,0);
						strike->setPri(599);
						strike->setCycle(new EndLoop((Prop*)strike, strike));
					}
				}
			}
		}

		if (++((BadMan *)intrpuzz)->scanned > 3)
			((BadMan *)intrpuzz)->min += 2;	

		switch (((BadMan *)intrpuzz)->scanned)
		{
			case 1:
				if (!num)
				{
					if (((BadMan *)intrpuzz)->ag->alive)	
						((BadMan *)intrpuzz)->ag->talk(485);
				}
				else if (num < 2)
				{
					if (((BadMan *)intrpuzz)->wash->alive)	
						((BadMan *)intrpuzz)->wash->talk(486);
				}
				else
				{
					if (((BadMan *)intrpuzz)->hend->alive)	
						((BadMan *)intrpuzz)->hend->talk(487);
				}
				break; 

			case 2:
				if (!num)
				{
					if (((BadMan *)intrpuzz)->ag->alive)	
						((BadMan *)intrpuzz)->ag->talk(488);
				}
				else if (num < 2)
				{
					if (((BadMan *)intrpuzz)->wash->alive)	
						((BadMan *)intrpuzz)->wash->talk(489);
				}
				else
				{
					if (((BadMan *)intrpuzz)->hend->alive)	
						((BadMan *)intrpuzz)->hend->talk(490);
				}
				break; 

			case 3:
				if (!num)
				{
					if (((BadMan *)intrpuzz)->ag->alive)	
						((BadMan *)intrpuzz)->ag->talk(491);
				}
				else if (num < 2)
				{
					if (((BadMan *)intrpuzz)->wash->alive)	
						((BadMan *)intrpuzz)->wash->talk(492);
				}									    
				else
				{
					if (((BadMan *)intrpuzz)->hend->alive)	
						((BadMan *)intrpuzz)->hend->talk(493);
				}
				break; 

			default:
				if (!num)
				{
					if (((BadMan *)intrpuzz)->ag->alive)	
						((BadMan *)intrpuzz)->ag->talk(491);
				}
				else if (num < 2)
				{
					if (((BadMan *)intrpuzz)->wash->alive)	
						((BadMan *)intrpuzz)->wash->talk(492);
				}									    
				else
				{
					if (((BadMan *)intrpuzz)->hend->alive)	
						((BadMan *)intrpuzz)->hend->talk(490);
				}
				break; 
		}
	}
}

void Movement::cue(void)
{
	int count = 0;
	((BadMan *)intrpuzz)->tact->oakalydokaly = true;	
	for (count = 0;count < 22;count++) 
	{
		if (((BadMan *)intrpuzz)->strikers[count] == this)
		{
			((BadMan *)intrpuzz)->strikers[count] = NULL;	
		}	
	}
	intrpuzz->release ( this );
	deathList->add(this);
}

EKG::EKG()
{
	name = "EKG";
	cycleSpeed = 4;	
}

StatButt::StatButt()
{
	name = "statbutt";
}
Card::Card()
{
	name = "card";
}

SpecButt::SpecButt()
{
	name = "specbutt";
}

void SpecButt::activate(void)
{
	if (guy->alive && !guy->fighting && !guy->holding)
	{
		sfxPlaySound (540,100,64,64,NULL);

		if (!((BadMan *)intrpuzz)->guy[guy->id])
			((BadMan *)intrpuzz)->guy[guy->id] = guy;

		if (guy->state == DORMANT || guy->state == WAITING)
		{
			if (guy->command == SPECIAL)
			{
				guy->state = WAITING;
				guy->command = WALK;
				guy->card->setScript(NULL);
				guy->card->setCel(0);
			}
			else
			{
				guy->state = WAITING;
				guy->command = SPECIAL;
				guy->card->setCel(0);
				guy->card->setScript(new CardFlashScript);
			}
		}											     
		else
		{
//			sfxPrintf("BUSY!!!");
			guy->reset();
			guy->state = WAITING;
			guy->command = SPECIAL;
			guy->card->setCel(0);
			guy->card->show();
			guy->card->setScript(new CardFlashScript);
			setCel(0);
			show();
			guy->selected->show();
		}
	}
}


void FlashScript::changeState ( int newState )
{
	switchTo
   	  ((StatButt *)client)->setCel(1);	
	  ticks = 15;
    END

	BEG
   	  ((StatButt *)client)->setCel(0);	
	  ticks = 15;
	END
	
	BEG
   	  ((StatButt *)client)->cue();
	END
}

void CardFlashScript::changeState ( int newState )
{
	switchTo
   	  ((Card *)client)->setCel(1);	
	  ticks = 15;
    END

	BEG
   	  ((Card *)client)->setCel(0);	
	  ticks = 15;
	END
	
	BEG
   	  ((Card *)client)->cue();
	END
}
void StatButt::cue(void)
{
	if (guy->talking || ((BadMan *)intrpuzz)->intro)
		setScript(new(FlashScript));
	else
		setScript(NULL);
}
void Card::cue(void)
{
	if (guy->command == SPECIAL)
		setScript(new(CardFlashScript));
	else
		setScript(NULL);	   	  	
}
void EKG::dispose(void)
{
	web_intrface->release (this);
	deathList->add(this);
}

void EKG::init(int theX, int theY, int pic_id, int loop_id, int cel_id)
{
	view = pic_id;				 
	loop = loop_id;
	cel = cel_id;
	x = theX;
	y = theY;

	draw();
	web_intrface->addToFront ( this );
	setCycle(new EndLoop((Prop*)this, this));
}	

void EKG::cue(void)
{
	if (guy->hitpoints <= 0 && guy->alive && !guy->talking && !guy->fighting)
	{
		guy->alive = false;
		guy->setScript(NULL);	
		guy->die();
		guy->hide();
		((BadMan *)intrpuzz)->casualties ++;
	}

	if(!guy->alive && !guy->talking)
	{
		if (view != 614)
			setView(614);				   
		setCycle(new EndLoop((Prop*)this, this));
	}
	else if (guy->fighting)
	{
	  if (view != 613)
		  setView(613);
	  setCycle(new EndLoop((Prop*)this, this));
	}
	else if (guy->hitpoints < 4)
	{
		if (view != 617)
			setView(617);
		setCycle(new EndLoop((Prop*)this, this));
	}
	else
	{
		if (view != 616)
			setView(616);
		setCycle(new EndLoop((Prop*)this, this));
	}
	
}

/************************
*
*  Generic Guy Stuff
*
*************************/
void Us::init(int theX, int theY, int pic_id, int loop_id, int cel_id)
{
	first = false;
	cycleSpeed = 4;	
	floor = 1;
	alive = true;
	hitpoints = 6;
	state = DORMANT;
	command = NONE;
	target = NULL;
	targetfloor = -1;
	interimtarget= -1;
	room_special = NULL;
	fighting = false;
	holding = false;
	talking = false;
	bombing = false;	
	talkingaboutphones = false;
	view = pic_id;
	loop = loop_id;
	cel = cel_id;
	x = theX;
	y = theY;
	boomroom = NULL;
	dir = false;
		
	here = 0;
	memset (&path,0,(100 * (sizeof(int))));
	memset (&log,0,(66 * (sizeof(int))));

	setInc(3,3);

	draw();
	selected = new (Selected);
	selected->init(theX,theY,612,12,0);
	selected->hide();
	selected->guy = this;
	team->addToFront ( this );
}	
void Us::special(void)
{
//	sfxPrintf("Cuss, blow up, look at, cut it");
}
void Us::die(void)
{
	reset();
//	setScript(new(flatScript));
}

void flatScript::changeState ( int newState )				    
{
	switchTo
	END
					   
	BEG	
		((BadMan *)intrpuzz)->ambient->stop();
		((BadMan *)intrpuzz)->ambient->loop = 5;
		((BadMan *)intrpuzz)->ambient->number = 534;
		((BadMan *)intrpuzz)->speech->play(534,this);
	END
	BEG
		((BadMan *)intrpuzz)->ambient->loop = 0;
	END
}

void SuccessScript::changeState ( int newState )				    
{
	switchTo
		ticks = 15;
	END
					   
	BEG	
		((Us *)client)->talk(((Us *)client)->first);
		setScript(NULL);
	END
}

void Us::announce(int what)
{
}

void Us::startWalk(void)
{
	state = WALKING;
	command = NONE;
	setScript(new walkScript(this));	
}

void Us::startJourney(void)
{
	BadRoom * thisRoom;
	int dirn;
	here = 0;
	memset (&path,0,(100 * (sizeof(int))));

	if (floor != ((BadMan *)intrpuzz)->curFloorNum)
	{
		interimtarget = finaltarget;

		if (x > 320)
			finaltarget = 21;
		else
			finaltarget = 20;
	}
	
	thisRoom = ((BadMan *)intrpuzz)->Floors[floor]->room[finaltarget];

	if (x > thisRoom->right)
	{
		if (y < thisRoom->top)
		{
			dirn = 0;
		}
		else
		{
			dirn = 1;
		}
	}
	else  if (x < thisRoom->left)
	{
		if (y < thisRoom->top)
		{
			dirn = 2;
		}
		else
		{
			dirn = 3;
		}
	}
	else
	{
		if (y < thisRoom->top)
		{
			dirn = 4;
		}
		else
		{
			dirn = 5;
		}
	}

	if (buildpath(dirn))
	{
		here = 0;
		target = path[0];
		((BadRoom *)((BadMan *)intrpuzz)->Floors[floor]->room[target])->checkpath(this);
		destX = ((BadMan *)intrpuzz)->Floors[floor]->room[target]->mid_x;
		destY = ((BadMan *)intrpuzz)->Floors[floor]->room[target]->mid_y;
		state = WALKING;
		command = NONE;
		sfxPlaySound (520,100,64,64,NULL);
		setScript(new walkScript(this));	
	}
	else
	{
		sfxPlaySound (535,100,64,64,NULL);
		reset();
	}	
}

void Us::secondJourney(void)
{
	int dirn;

	here = 0;
	memset (&path,0,(100 * (sizeof(int))));

	if (room > 20)
		dirn = 0;
	else
		dirn = 2;

	if (buildpath(dirn))
	{
		here = 0;
		target = path[0];
		((BadRoom *)((BadMan *)intrpuzz)->Floors[floor]->room[target])->checkpath(this);
		destX = ((BadMan *)intrpuzz)->Floors[floor]->room[target]->mid_x;
		destY = ((BadMan *)intrpuzz)->Floors[floor]->room[target]->mid_y;
		state = WALKING;
		command = NONE;

		setScript(new walkScript(this));	
	}
	else
	{
		reset();
	}	
}

void Us::updateJourney(void)
{
	here++;
	target = path[here];
	((BadRoom *)((BadMan *)intrpuzz)->Floors[floor]->room[target])->checkpath(this);
	destX = ((BadMan *)intrpuzz)->Floors[floor]->room[target]->mid_x;
	destY = ((BadMan *)intrpuzz)->Floors[floor]->room[target]->mid_y;
	state = WALKING;
	command = NONE;
	setScript(new walkScript(this));	
}

void Us::reset(void)
{
	int tmp;

	setMotion(NULL);
	setScript(NULL);
	state = DORMANT;
	command = NONE;
	here = 0;
	memset (&path,0,(100 * (sizeof(int))));

	if (room > 19)
	{
		if (alive)
			show();
		floor = ((BadMan *)intrpuzz)->curFloorNum;
	}	

	tmp = cel;
	setLoop(id);
	setCel(tmp);
	setCycle(new EndLoop((Prop*)this, beacon));
	spec->setCel(0);
	spec->hide();
	selected->hide();
	if (card->script)
	{
		card->setScript(NULL);
		card->setCel(0);
	}
	card->hide();
	((BadMan*)intrpuzz)->guy[id] = NULL;
	for (tmp=0;tmp<4;tmp++)
	{
		if (((BadMan*)intrpuzz)->guy[tmp])
		{
			((BadMan*)intrpuzz)->guy[tmp]->card->show();
			((BadMan*)intrpuzz)->guy[tmp]->spec->show();
			break;
		}
	}
}
void Us::hide(void)
{
	selected->hide();
	IntrPuzzActor::hide();
}
void Us::show(void)
{
	if (alive)
	{
		if (state != DORMANT)
			selected->show();
	
		IntrPuzzActor::show();
	}
}
void Us::activate(void)
{
	if (!fighting && !bombing && !holding)
	{
		sfxPlaySound (540,100,64,64,NULL);

//		if (!((BadMan *)intrpuzz)->guy[id])
//			((BadMan *)intrpuzz)->guy[id] = this;

		if (state == DORMANT || state == WALKING)
		{
			reset();

			if (!((BadMan *)intrpuzz)->guy[id])
				((BadMan *)intrpuzz)->guy[id] = this;

			((BadMan*)intrpuzz)->hend->card->hide();
			((BadMan*)intrpuzz)->hend->spec->hide();
			((BadMan*)intrpuzz)->wash->card->hide();
			((BadMan*)intrpuzz)->wash->spec->hide();
			((BadMan*)intrpuzz)->schiff->card->hide();
			((BadMan*)intrpuzz)->schiff->spec->hide();
			((BadMan*)intrpuzz)->ag->card->hide();
			((BadMan*)intrpuzz)->ag->spec->hide();
		
			card->show();
			spec->show();
			selected->show();
			state = WAITING;
			command = WALK;	
		}
		else if (state == WAITING)
		{
			reset();
		}
		else
		{
/*
			reset();
			spec->show();
			selected->show();
			card->show();
			state = WAITING;
			command = WALK;	
	//		sfxPrintf("BUSY!!!");
*/		}
	}
}	

int Us::handleEvent(MADEEventStamp* event)
{
	  if (((BadMan *)intrpuzz)->hideGuys)
	  {
		if (room > 19)
		{
			if (alive)
			{
				if	(state != WALKING)
				{
					show();
					floor = ((BadMan *)intrpuzz)->curFloorNum; 
				}
				else if (floor != ((BadMan *)intrpuzz)->curFloorNum)
					hide();
				else 
					show();
			}				
		}
		else
		{
		  	if (floor != ((BadMan *)intrpuzz)->curFloorNum)
		  		hide();										    
			else
				show();	
		}

		((BadMan *)intrpuzz)->checked +=1;
		return false;
	  }	

	if (alive && !(flag & OBJECT_HIDDEN))
	{
	  if (onMe(event))  
	  {
	  	if (user->input)
		{
		  	if (event_type == USER_LEFT_UP)
		    {
				activate();
				return true;
		    }
		}
	  }
 	}	
  return false;
}

void Us::dispose()	
{
	setCycle(NULL);
	if ( hotspot )
	{
		setHotspot ( NULL );
		hotspot = NULL;
	}
	team->release (this);
	deathList->add(this);
}
void Us::cue(int dummy)
{
	Us::cue();
}

void Us::cue(void) {
	if (talkingaboutphones)
	{
		talkingaboutphones = false;
		((BadMan *)intrpuzz)->somebodyistalkingaboutphones = false;
	}	
	if (first)
	{
		if (((BadMan *)intrpuzz)->oops || ((BadMan *)intrpuzz)->whoops)
			((BadMan *)intrpuzz)->casualties = 4;
		((BadMan *)intrpuzz)->cue();
		return;	
	}

	((BadRoom *)((BadMan *)intrpuzz)->Floors[floor]->room[room])->combatants = false;
	if (fighting || !alive)
	{
 		if (!(fighting && !alive))
			((BadMan *)intrpuzz)->micOpen = false;

		fighting = false;
		bombing = false;
		talking = false;
		if (script)
			script->cue();
	}
	else
	{
		fighting = false;
		bombing = false;
		talking = false;
		((BadMan *)intrpuzz)->micOpen = false;

		if (boomroom && (floor == ((BadMan *)intrpuzz)->curFloorNum))
		{
			boomroom->drawHole(coords.x,coords.y);		
		}
		if (boomroom)
		{
			boomroom->combatants = false;
			((BadMan *)intrpuzz)->Floors[floor]->room[room]->combatants = false;
		}
//		if (boomwall)
//			*boomwall = HOLE;

		boomwall = NULL;
		boomroom = NULL;
	}
}
int Us::talk(int number)
{
 	if (!((BadMan *)intrpuzz)->done || first)
	{
		if (number == SUCCESS)
		{
			first = true;
			((BadMan *)intrpuzz)->done = true;
			((BadMan *)intrpuzz)->setScript(NULL);
			talking = false;
			((BadMan *)intrpuzz)->micOpen = false;	
			((BadMan *)intrpuzz)->speech->stop();

			switch(id)
			{
				case 0:
					number = 448;
					break;
				case 1:
					number = 449;
					break;
				case 2:
					number = 450;
					break;
				case 3:
					number = 451;
					break;
			}
			setScript(new SuccessScript);
			first = number;
			return true;
		}								  				    

		if (!talking)
		{
			if (!((BadMan *)intrpuzz)->micOpen)
			{
				talking = true;
				((BadMan *)intrpuzz)->micOpen = true;
				((BadMan *)intrpuzz)->speech->stop();
				((BadMan *)intrpuzz)->speech->number = number;
				((BadMan *)intrpuzz)->speech->playBuffered(number,(Us*)this);
				butt->setScript(new(FlashScript));
				return true;	
			}
			else
			{
				return false;	
			}									    
		}
	}
	return false;	
}

int Us::checkpath(void) {
//	int room = target;
	BadRoom * thisRoom = ((BadMan *)intrpuzz)->Floors[floor]->room[target];
 	BadRoom * currentRoom = ((BadMan *)intrpuzz)->Floors[floor]->room[room];

	if (x > thisRoom->right)
	{
		if (currentRoom->w_wall == thisRoom->e_wall)
		{
			if (!thisRoom->e_wall)
			{
				return true;
			}
			else if (*(thisRoom->e_wall))
			{
				return true;
			}
		}
	}
	else  if (x < thisRoom->left)
	{
		if (currentRoom->e_wall == thisRoom->w_wall)
		{
			if (!thisRoom->w_wall)
			{
				return true;
			}
			else if (*(thisRoom->w_wall))
			{
				return true;
			}
		}
	}
	else if (y < thisRoom->top)
	{
		if (currentRoom->s_wall == thisRoom->n_wall)
		{
			if (!thisRoom->n_wall)
			{
				return true;
			}
			else if (*(thisRoom->n_wall))
			{
				return true;
			}
		}
	}
	else if (y > thisRoom->bottom)
	{
		if (currentRoom->n_wall == thisRoom->s_wall)
		{
			if (!thisRoom->s_wall)
			{
				return true;
			}
			else if (*(thisRoom->s_wall))
			{
				return true;
			}
		}
	}	
	return false;
}

int Us::buildpath(int dirn)
{
	int count = 0;
	//Floor * thisfloor =  ((BadMan *)intrpuzz)->Floors[floor];
	BadRoom **thisroom = &((BadMan *)intrpuzz)->Floors[floor]->room[0];
	FloorMap map[22];
	FloorMap * maproom;

	int curroom = room;
	int lastroom = -1;
	int lastdir  = 0;
	//int there = false;
	here = 0;
	memset (&path,0,(100 * (sizeof(int))));
	for (count=0;count<22;count++)
	{
		map[count].n = (*thisroom)->n_room;
		map[count].e = (*thisroom)->e_room;
		map[count].s = (*thisroom)->s_room;
		map[count].w = (*thisroom)->w_room;

		map[count].n_wall = (*thisroom)->n_wall;
		map[count].e_wall = (*thisroom)->e_wall;
		map[count].s_wall = (*thisroom)->s_wall;
		map[count].w_wall = (*thisroom)->w_wall;

		thisroom++;
	}
	switch (dirn)
	{
		case 0:
			while (finaltarget != curroom)
			{
				maproom = &map[curroom];

				if (here >98)
				{
					return false;
				}
				else if(maproom->w != 99 && maproom->w != lastroom && (maproom->w_wall && *(maproom->w_wall)))
				{
					lastroom = curroom;
					lastdir = BAD_WEST;
					path[here] = maproom->w;					
					curroom = maproom->w;
					here ++;
				}
				else if(maproom->s != 99 && maproom->s != lastroom && (maproom->s_wall && *(maproom->s_wall)))
				{
					lastroom = curroom;
					lastdir = BAD_SOUTH;
					path[here] = maproom->s;					
					curroom = maproom->s;
					here ++;
				}
				else if (maproom->e != 99 && maproom->e != lastroom && (maproom->e_wall && *(maproom->e_wall)))
				{
					lastroom = curroom;
					lastdir = BAD_EAST;
					path[here] = maproom->e;					
					curroom = maproom->e;
					here ++;
				}
				else if (maproom->n != 99 && maproom->n != lastroom && (maproom->n_wall && *(maproom->n_wall)))
				{
					lastroom = curroom;
					lastdir = BAD_NORTH;
					path[here] = maproom->n;					
					curroom = maproom->n;
					here ++;
				}
				else
				{
					switch (lastdir)
					{
						case BAD_NORTH:
							map[lastroom].n = 99;
							break;
						case BAD_EAST:
							map[lastroom].e = 99;
							break;
						case BAD_SOUTH:
							map[lastroom].s = 99;
							break;
						case BAD_WEST:
							map[lastroom].w = 99;
							break;
						case 0:
							return false;
							break;
					}
					memset (&path,0,(100 * (sizeof(int))));
					curroom = room;
					lastroom = -1;
					lastdir  = 0;
					here = 0;
				}
				count = 0;
				while(count < here-1)
				{
					if (curroom == path[count] || curroom == room)
					{
						switch (lastdir)
						{
							case BAD_NORTH:
								map[lastroom].n = 99;
								break;
							case BAD_EAST:
								map[lastroom].e = 99;
								break;
							case BAD_SOUTH:
								map[lastroom].s = 99;
								break;
							case BAD_WEST:
								map[lastroom].w = 99;
								break;
							case 0:
								return false;
								break;
						}
						memset (&path,0,(100 * (sizeof(int))));
						curroom = room;
						lastroom = -1;
						lastdir  = 0;
						here = 0;
						break;
					}
					count++;
				}
			}
			break;
	
		case 1:
			while (finaltarget != curroom)
			{
				maproom = &map[curroom];

				if (here >98)
				{
					return false;
				}
				else if(maproom->w != 99 && maproom->w != lastroom && (maproom->w_wall && *(maproom->w_wall)))
				{
					lastroom = curroom;
					lastdir = BAD_WEST;
					path[here] = maproom->w;					
					curroom = maproom->w;
					here ++;
				}
				else if (maproom->n != 99 && maproom->n != lastroom && (maproom->n_wall && *(maproom->n_wall)))
				{
					lastroom = curroom;
					lastdir = BAD_NORTH;
					path[here] = maproom->n;					
					curroom = maproom->n;
					here ++;
				}
				else if (maproom->e != 99 && maproom->e != lastroom && (maproom->e_wall && *(maproom->e_wall)))
				{
					lastroom = curroom;
					lastdir = BAD_EAST;
					path[here] = maproom->e;					
					curroom = maproom->e;
					here ++;
				}
				else if(maproom->s != 99 && maproom->s != lastroom && (maproom->s_wall && *(maproom->s_wall)))
				{
					lastroom = curroom;
					lastdir = BAD_SOUTH;
					path[here] = maproom->s;					
					curroom = maproom->s;
					here ++;
				}
				else
				{
					switch (lastdir)
					{
						case BAD_NORTH:
							map[lastroom].n = 99;
							break;
						case BAD_EAST:
							map[lastroom].e = 99;
							break;
						case BAD_SOUTH:
							map[lastroom].s = 99;
							break;
						case BAD_WEST:
							map[lastroom].w = 99;
							break;
						case 0:
							return false;
							break;
					}
					memset (&path,0,(100 * (sizeof(int))));
					curroom = room;
					lastroom = -1;
					lastdir  = 0;
					here = 0;
				}
				count = 0;
				while(count < here-1)
				{
					if (curroom == path[count] || curroom == room)
					{
						switch (lastdir)
						{
							case BAD_NORTH:
								map[lastroom].n = 99;
								break;
							case BAD_EAST:
								map[lastroom].e = 99;
								break;
							case BAD_SOUTH:
								map[lastroom].s = 99;
								break;
							case BAD_WEST:
								map[lastroom].w = 99;
								break;
							case 0:
								return false;
								break;
						}
						memset (&path,0,(100 * (sizeof(int))));
						curroom = room;
						lastroom = -1;
						lastdir  = 0;
						here = 0;
						break;
					}
					count++;
				}
			}
			break;
		
		case 2:
			while (finaltarget != curroom)
			{
				maproom = &map[curroom];

				if (here >98)
				{
					return false;
				}
				else if (maproom->e != 99 && maproom->e != lastroom && (maproom->e_wall && *(maproom->e_wall)))
				{
					lastroom = curroom;
					lastdir = BAD_EAST;
					path[here] = maproom->e;					
					curroom = maproom->e;
					here ++;
				}
				else if(maproom->s != 99 && maproom->s != lastroom && (maproom->s_wall && *(maproom->s_wall)))
				{
					lastroom = curroom;
					lastdir = BAD_SOUTH;
					path[here] = maproom->s;					
					curroom = maproom->s;
					here ++;
				}
				else if(maproom->w != 99 && maproom->w != lastroom && (maproom->w_wall && *(maproom->w_wall)))
				{
					lastroom = curroom;
					lastdir = BAD_WEST;
					path[here] = maproom->w;					
					curroom = maproom->w;
					here ++;
				}
				else if (maproom->n != 99 && maproom->n != lastroom && (maproom->n_wall && *(maproom->n_wall)))
				{
					lastroom = curroom;
					lastdir = BAD_NORTH;
					path[here] = maproom->n;					
					curroom = maproom->n;
					here ++;
				}
				else
				{
					switch (lastdir)
					{
						case BAD_NORTH:
							map[lastroom].n = 99;
							break;
						case BAD_EAST:
							map[lastroom].e = 99;
							break;
						case BAD_SOUTH:
							map[lastroom].s = 99;
							break;
						case BAD_WEST:
							map[lastroom].w = 99;
							break;
						case 0:
							return false;
							break;
					}
					memset (&path,0,(100 * (sizeof(int))));
					curroom = room;
					lastroom = -1;
					lastdir  = 0;
					here = 0;
				}
				count = 0;
				while(count < here-1)
				{
					if (curroom == path[count] || curroom == room)
					{
						switch (lastdir)
						{
							case BAD_NORTH:
								map[lastroom].n = 99;
								break;
							case BAD_EAST:
								map[lastroom].e = 99;
								break;
							case BAD_SOUTH:
								map[lastroom].s = 99;
								break;
							case BAD_WEST:
								map[lastroom].w = 99;
								break;
							case 0:
								return false;
								break;
						}
						memset (&path,0,(100 * (sizeof(int))));
						curroom = room;
						lastroom = -1;
						lastdir  = 0;
						here = 0;
						break;
					}
					count++;
				}
			}
			break;
		
		case 3:
			while (finaltarget != curroom)
			{
				maproom = &map[curroom];

				if (here >98)
				{
					return false;
				}
				else if (maproom->e != 99 && maproom->e != lastroom && (maproom->e_wall && *(maproom->e_wall)))
				{
					lastroom = curroom;
					lastdir = BAD_EAST;
					path[here] = maproom->e;					
					curroom = maproom->e;
					here ++;
				}
				else if (maproom->n != 99 && maproom->n != lastroom && (maproom->n_wall && *(maproom->n_wall)))
				{
					lastroom = curroom;
					lastdir = BAD_NORTH;
					path[here] = maproom->n;					
					curroom = maproom->n;
					here ++;
				}
				else if(maproom->w != 99 && maproom->w != lastroom && (maproom->w_wall && *(maproom->w_wall)))
				{
					lastroom = curroom;
					lastdir = BAD_WEST;
					path[here] = maproom->w;					
					curroom = maproom->w;
					here ++;
				}
				else if(maproom->s != 99 && maproom->s != lastroom && (maproom->s_wall && *(maproom->s_wall)))
				{
					lastroom = curroom;
					lastdir = BAD_SOUTH;
					path[here] = maproom->s;					
					curroom = maproom->s;
					here ++;
				}
				else
				{
					switch (lastdir)
					{
						case BAD_NORTH:
							map[lastroom].n = 99;
							break;
						case BAD_EAST:
							map[lastroom].e = 99;
							break;
						case BAD_SOUTH:
							map[lastroom].s = 99;
							break;
						case BAD_WEST:
							map[lastroom].w = 99;
							break;
						case 0:
							return false;
							break;
					}
					memset (&path,0,(100 * (sizeof(int))));
					curroom = room;
					lastroom = -1;
					lastdir  = 0;
					here = 0;
				}
				count = 0;
				while(count < here-1)
				{
					if (curroom == path[count] || curroom == room)
					{
						switch (lastdir)
						{
							case BAD_NORTH:
								map[lastroom].n = 99;
								break;
							case BAD_EAST:
								map[lastroom].e = 99;
								break;
							case BAD_SOUTH:
								map[lastroom].s = 99;
								break;
							case BAD_WEST:
								map[lastroom].w = 99;
								break;
							case 0:
								return false;
								break;
						}
						memset (&path,0,(100 * (sizeof(int))));
						curroom = room;
						lastroom = -1;
						lastdir  = 0;
						here = 0;
						break;
					}
					count++;
				}
			}
			break;
		
		case 4:
			while (finaltarget != curroom)
			{
				maproom = &map[curroom];

				if (here >98)
				{
					return false;
				}
				else if(maproom->s != 99 && maproom->s != lastroom && (maproom->s_wall && *(maproom->s_wall)))
				{
					lastroom = curroom;
					lastdir = BAD_SOUTH;
					path[here] = maproom->s;					
					curroom = maproom->s;
					here ++;
				}
				else if (maproom->e != 99 && maproom->e != lastroom && (maproom->e_wall && *(maproom->e_wall)))
				{
					lastroom = curroom;
					lastdir = BAD_EAST;
					path[here] = maproom->e;					
					curroom = maproom->e;
					here ++;
				}
				else if(maproom->w != 99 && maproom->w != lastroom && (maproom->w_wall && *(maproom->w_wall)))
				{
					lastroom = curroom;
					lastdir = BAD_WEST;
					path[here] = maproom->w;					
					curroom = maproom->w;
					here ++;
				}
				else if (maproom->n != 99 && maproom->n != lastroom && (maproom->n_wall && *(maproom->n_wall)))
				{
					lastroom = curroom;
					lastdir = BAD_NORTH;
					path[here] = maproom->n;					
					curroom = maproom->n;
					here ++;
				}
				else
				{
					switch (lastdir)
					{
						case BAD_NORTH:
							map[lastroom].n = 99;
							break;
						case BAD_EAST:
							map[lastroom].e = 99;
							break;
						case BAD_SOUTH:
							map[lastroom].s = 99;
							break;
						case BAD_WEST:
							map[lastroom].w = 99;
							break;
						case 0:
							return false;
							break;
					}
					memset (&path,0,(100 * (sizeof(int))));
					curroom = room;
					lastroom = -1;
					lastdir  = 0;
					here = 0;
				}
				count = 0;
				while(count < here-1)
				{
					if (curroom == path[count] || curroom == room)
					{
						switch (lastdir)
						{
							case BAD_NORTH:
								map[lastroom].n = 99;
								break;
							case BAD_EAST:
								map[lastroom].e = 99;
								break;
							case BAD_SOUTH:
								map[lastroom].s = 99;
								break;
							case BAD_WEST:
								map[lastroom].w = 99;
								break;
							case 0:
								return false;
								break;
						}
						memset (&path,0,(100 * (sizeof(int))));
						curroom = room;
						lastroom = -1;
						lastdir  = 0;
						here = 0;
						break;
					}
					count++;
				}								   
			}
			break;
		
		case 5:
			while (finaltarget != curroom)
			{
				maproom = &map[curroom];

				if (here >98)
				{
					return false;
				}
				else if (maproom->n != 99 && maproom->n != lastroom && (maproom->n_wall && *(maproom->n_wall)))
				{
					lastroom = curroom;
					lastdir = BAD_NORTH;
					path[here] = maproom->n;					
					curroom = maproom->n;
					here ++;
				}
				else if (maproom->e != 99 && maproom->e != lastroom && (maproom->e_wall && *(maproom->e_wall)))
				{
					lastroom = curroom;
					lastdir = BAD_EAST;
					path[here] = maproom->e;					
					curroom = maproom->e;
					here ++;
				}
				else if(maproom->w != 99 && maproom->w != lastroom && (maproom->w_wall && *(maproom->w_wall)))
				{
					lastroom = curroom;
					lastdir = BAD_WEST;
					path[here] = maproom->w;					
					curroom = maproom->w;
					here ++;
				}
				else if(maproom->s != 99 && maproom->s != lastroom && (maproom->s_wall && *(maproom->s_wall)))
				{
					lastroom = curroom;
					lastdir = BAD_SOUTH;
					path[here] = maproom->s;					
					curroom = maproom->s;
					here ++;
				}
				else
				{
					switch (lastdir)
					{
						case BAD_NORTH:
							map[lastroom].n = 99;
							break;
						case BAD_EAST:
							map[lastroom].e = 99;
							break;
						case BAD_SOUTH:
							map[lastroom].s = 99;
							break;
						case BAD_WEST:
							map[lastroom].w = 99;
							break;
						case 0:
							return false;
							break;
					}
					memset (&path,0,(100 * (sizeof(int))));
					curroom = room;
					lastroom = -1;
					lastdir  = 0;
					here = 0;
				}

				count = 0;
				while(count < here-1)
				{
					if (curroom == path[count] || curroom == room)
					{
						switch (lastdir)
						{
							case BAD_NORTH:
								map[lastroom].n = 99;
								break;
							case BAD_EAST:
								map[lastroom].e = 99;
								break;
							case BAD_SOUTH:
								map[lastroom].s = 99;
								break;
							case BAD_WEST:
								map[lastroom].w = 99;
								break;
							case 0:
								return false;
								break;
						}
						memset (&path,0,(100 * (sizeof(int))));
						curroom = room;
						lastroom = -1;
						lastdir  = 0;
						here = 0;
						break;
					}
					count++;
				}
			}
			break;
	}
	here = 0;
	return true;
}

void Us::fight(void)
{
	int num = rand() & 4;
	
	switch (num)
	{
		case 0:
			talk(570);
			break;
		case 1:
			talk(571);
			break;
		case 2:
			talk(572);
			break;
		case 3:
			talk(573);
			break;
		case 4:
			talk(574);
			break;
	}
}

/************************
*
*  Specific Guy Stuff
*
*************************/

Henderson::Henderson()
{
	name = "Henderson";				 

	if (GameFlag.test(fPatsHTM_129))
		init(115,225,607,0,0);
	else
		init(33,175,607,0,0);

	room = 8;
	id = 0;
	sfxSpriteGlass(mySprite,SPRITE_GLASS_LIGHT,MK_COLOR16(0,0,255),100);

	butt = new (StatButt);
	butt->init(5,20,612,0,0);
	butt->noSkip();
	butt->guy = (Us*)this;
	
	spec = new (SpecButt);
	spec->init(380,20,612,4,0);
	spec->noSkip();
	spec->guy = (Us*)this;
	spec->hide();

	ekg = new (EKG);
	ekg->init(11,38,616,0,0);
	ekg->noSkip();
	ekg->guy = (Us*)this;

	card = new (Card);
	card->init(416,20,612,9,0);
	card->noSkip();
	card->guy = (Us*)this;
	card->hide();
	
	beacon = new Beacon((Us*)this);
	setCycle(new EndLoop((Prop*)this, beacon));
}

void Henderson::fight(void)
{
	static int num = 0;
	int ret;	
	switch (num)
	{
		case 0:
			ret = talk(701);
			break;
		case 1:
			ret = talk(702);
			break;
		case 2:
			ret = talk(703);
			break;
	}
	if (num == 2)
		num = 0;
	else
		num++;
}

Washington::Washington()
{
	name = "Washinton";

	grenades = 10;
	id = 2;
	if (GameFlag.test(fPatsHTM_129))
		init(105,235,607,2,0);
	else
		init(33,285,607,2,0);
	
	sfxSpriteGlass(mySprite,SPRITE_GLASS_LIGHT,MK_COLOR16(255,0,0),100);
	room = 8;
	butt = new (StatButt);
	butt->init(192,20,612,2,0);
	butt->noSkip();
	butt->guy = (Us*)this;

	spec = new (SpecButt);
	spec->init(380,20,612,6,0);
	spec->noSkip();
	spec->guy = (Us*)this;
	spec->hide();

	ekg = new (EKG);
	ekg->init(198,38,616,0,8);
	ekg->noSkip();
	ekg->guy = (Us*)this;

	card = new (Card);
	card->init(416,20,612,11,0);
	card->noSkip();
	card->guy = (Us*)this;
	card->hide();
	
	beacon = new Beacon((Us*)this);
	setCycle(new EndLoop((Prop*)this, beacon));
}
void Washington::fight(void)
{
	static int num = 0;
	int ret;
	switch (num)
	{
		case 0:
			ret = talk(707);
			break;
		case 1:
			ret = talk(708);
			break;
		case 2:
			ret = talk(709);
			break;
	}
	if (num == 2)
		num = 0;
	else
		num++;
}

Schiff::Schiff()
{
	name = "Schiff";
	bombs = 10;

	id = 1;
	if (GameFlag.test(fPatsHTM_129))
		init(95,225,607,1,0);
	else
		init(33,275,607,1,0);
	sfxSpriteGlass(mySprite,SPRITE_GLASS_LIGHT,MK_COLOR16(255,255,0),100);
	room = 8;

	butt = new (StatButt);
	butt->init(99,20,612,1,0);
	butt->noSkip();
	butt->guy = (Us*)this;

	spec = new (SpecButt);
	spec->init(380,20,612,7,0);
	spec->noSkip();
	spec->guy = (Us*)this;
	spec->hide();

	ekg = new (EKG);
	ekg->init(104,38,616,0,17);
	ekg->noSkip();
	ekg->guy = (Us*)this;

	card = new (Card);
	card->init(416,20,612,10,0);
	card->noSkip();
	card->guy = (Us*)this;
	card->hide();
	
	beacon = new Beacon((Us*)this);
	setCycle(new EndLoop((Prop*)this, beacon));
}
void Schiff::fight(void)
{
	static int num = 0;
	int ret;
	switch (num)
	{
		case 0:
			ret = talk(704);
			break;
		case 1:
			ret = talk(705);
			break;
		case 2:
			ret = talk(706);
			break;
	}
	if (num == 2)
		num = 0;
	else
		num++;
}

Aguila::Aguila()
{
	name = "Aguila";

	if (GameFlag.test(fPatsHTM_129))
		init(105,215,607,3,0);
	else	
		init(33,165,607,3,0);
	sfxSpriteGlass(mySprite,SPRITE_GLASS_LIGHT,MK_COLOR16(0,255,0),100);
	room = 8;
	id = 3;
	butt = new (StatButt);
	butt->init(284,20,612,3,0);
	butt->noSkip();
	butt->guy = (Us*)this;

	spec = new (SpecButt);
	spec->init(380,20,612,5,0);								    
	spec->noSkip();
	spec->guy = (Us*)this;
	spec->hide();
										     
	ekg = new (EKG);
	ekg->init(289,38,616,0,25);
	ekg->noSkip();
	ekg->guy = (Us*)this;

	card = new (Card);
	card->init(416,20,612,8,0);
	card->noSkip();
	card->guy = (Us*)this;
	card->hide();
	
	beacon = new Beacon((Us*)this);
	setCycle(new EndLoop((Prop*)this, beacon));
}
void Aguila::fight(void)
{
	static int num = 0;
	int ret;	
	switch (num)
	{												  
		case 0:
			ret = talk(710);
			break;
		case 1:
			ret = talk(711);
			break;
		case 2:
			ret = talk(712);
			break;
	}
	if (num == 2)
		num = 0;
	else
		num++;
}

int *Schiff::check4wall(void)
{
	int cur_x = x;
	int cur_y = y;

	BadRoom * testroom = ((BadMan *)intrpuzz)->Floors[floor]->room[target]; 

	if (cur_x > testroom->right)
	{
		if (testroom->e_wall && !(*testroom->e_wall))
		{
			coords.x = testroom->right;			
			coords.y = testroom->mid_y;
			return testroom->e_wall;
		}
		else
		{
			return NULL;
		}
	}
	if (cur_x < testroom->left)
	{
		if (testroom->w_wall && !(*testroom->w_wall))
		{
			coords.x = testroom->left;			
			coords.y = testroom->mid_y;
			return testroom->w_wall;
		}
		else
		{
			return NULL;
		}
	}
	if (cur_y > testroom->bottom)
	{
		if (testroom->s_wall && !(*testroom->s_wall))
		{
			coords.x = testroom->mid_x;			
			coords.y = testroom->bottom;
			return testroom->s_wall;
		}
		else
		{
			return NULL;
		}
	}
	if (cur_y < testroom->top)
	{
		if (testroom->n_wall && !(*testroom->n_wall))
		{
			coords.x = testroom->mid_x;			
			coords.y = testroom->top;
			return testroom->n_wall;
		}
		else
		{
			return NULL;
		}
	}
	return NULL;
	
}

void Aguila::special(void)
{
	static int aguilaFlag = 0;

	if (checkpath() || target == room)
	{
		if(*(((BadMan *)intrpuzz)->Floors[floor]->fenemy[room]) == PHONES && !((BadMan *)intrpuzz)->phones)
		{
			talk(468);
			((BadMan *)intrpuzz)->phones = true;
		}
		else
		{
			if (aguilaFlag)
			{
				talk(475);
				aguilaFlag --;
			}
			else
			{
				talk(476);
				aguilaFlag ++;
			}
		}
	}
	else
	{
		talk(476);
	}
}

void Schiff::special(void)
{
	int * wall;
	static int schiffFlag = 0;
	boomroom = ((BadMan *)intrpuzz)->Floors[floor]->room[target];

	if (bombs && !bombing)
	{
		wall = check4wall();

		if (wall)
		{
			*wall = HOLE;
			bombs --;

			if (*(((BadMan *)intrpuzz)->Floors[floor]->fenemy[target]) == BIRD)
			{
				((BadMan *)intrpuzz)->oops = 1;				
				if (talking)
					talking = false;
				
				((BadMan *)intrpuzz)->micOpen = false;
				((BadMan *)intrpuzz)->speech->stop();
				talk(508);
				first = true;
				((BadMan *)intrpuzz)->setScript(NULL);
				((BadMan *)intrpuzz)->done = true;

			}
			else if (*(((BadMan *)intrpuzz)->Floors[floor]->fenemy[target]) == BOILER)
			{
				GameFlag.set(fWahKa36);
				((BadMan *)intrpuzz)->oops = 10;				
				if (talking)
					talking = false;
				
				((BadMan *)intrpuzz)->micOpen = false;
				((BadMan *)intrpuzz)->speech->stop();
				talk(546);
				first = true;
				((BadMan *)intrpuzz)->setScript(NULL);
				((BadMan *)intrpuzz)->done = true;
			}
			else if (schiffFlag)
			{
				schiffFlag --;
				if (!talk(466))
				{
					*wall = HOLE;
					boomroom->drawHole(coords.x,coords.y);		
					boomroom = NULL;
				}	
				else
				{
					bombing = true;
					((BadMan *)intrpuzz)->Floors[floor]->room[room]->combatants = true;
					boomroom->combatants = true;
//					boomwall = wall;
				}
			}
			else
			{
				schiffFlag ++;
				if (!talk(467))
				{
					*wall = HOLE;
					boomroom->drawHole(coords.x,coords.y);		
					boomroom = NULL;
				}
				else
				{
					bombing = true;
					((BadMan *)intrpuzz)->Floors[floor]->room[room]->combatants = true;
					boomroom->combatants = true;
//					boomwall = wall;
				}
			}
		}
		else
		{
			boomroom = NULL;
			sfxPlaySound (535,100,64,64,NULL);
//			sfxPrintf("Sure, but why???");
		}
	}
	else
	{
			if (schiffFlag)
			{
				schiffFlag --;
				talk(471);
			}
			else
			{
				schiffFlag ++;
				talk(472);
			}
	}

}

void Washington::special(void)
{
	static int washingtonFlag = 0; 
	int count = 0;

	if (grenades)
	{
		if (checkpath())
		{
			if (*(((BadMan *)intrpuzz)->Floors[floor]->fenemy[target]) == BIRD)
			{
				((BadMan *)intrpuzz)->whoops = 1;				
				if (talking)
					talking = false;
				
				((BadMan *)intrpuzz)->micOpen = false;
				((BadMan *)intrpuzz)->speech->stop();
				talk(509);
				first = true;
				((BadMan *)intrpuzz)->setScript(NULL);
				((BadMan *)intrpuzz)->done = true;
			}
			else if (*(((BadMan *)intrpuzz)->Floors[floor]->fenemy[target]) == BOILER)
			{
				GameFlag.set(fWahKa36);
				((BadMan *)intrpuzz)->whoops = 10;				
				if (talking)
					talking = false;
				
				((BadMan *)intrpuzz)->micOpen = false;
				((BadMan *)intrpuzz)->speech->stop();
				talk(546);
				first = true;
				((BadMan *)intrpuzz)->setScript(NULL);
				((BadMan *)intrpuzz)->done = true;
			}
			else if (washingtonFlag)
			{
				washingtonFlag --;
				talk(464);
			}
			else
			{
				washingtonFlag ++;
				talk(465);
			}
			grenades --;

			if	(((BadMan *)intrpuzz)->hend->room == target && ((BadMan *)intrpuzz)->hend->floor == floor)
				((BadMan *)intrpuzz)->hend->hitpoints -=5;				

			if	(((BadMan *)intrpuzz)->schiff->room == target && ((BadMan *)intrpuzz)->schiff->floor == floor)
				((BadMan *)intrpuzz)->schiff->hitpoints -=5;				

			if	(((BadMan *)intrpuzz)->ag->room == target && ((BadMan *)intrpuzz)->schiff->floor == floor)
				((BadMan *)intrpuzz)->ag->hitpoints -=5;				

			if (*(((BadMan *)intrpuzz)->Floors[floor]->fenemy[target]) == PHONES)
			{
			}
			else
			{
				for (count=0;count<22;count++)
				{
/*				 	if ((((BadMan *)intrpuzz)->Floors[floor]->room[count]) == (((BadMan *)intrpuzz)->Floors[floor]->room[target]))
							*(((BadMan *)intrpuzz)->Floors[floor]->fenemy[count]) = DED;
*/					
				 	if ((((BadMan *)intrpuzz)->Floors[floor]->room[count]->parent) == (((BadMan *)intrpuzz)->Floors[floor]->room[target]->parent))
							*(((BadMan *)intrpuzz)->Floors[floor]->fenemy[count]) = DED;
				}
//				*(((BadMan *)intrpuzz)->Floors[floor]->fenemy[target]) = DED;
			}
		}
		else
		{
			sfxPlaySound (535,100,64,64,NULL);
//			sfxPrintf("Can't buddy!!! It'll bounce & hit me");
		}	
	}
	else
	{
		if (washingtonFlag)
		{
			washingtonFlag --;
			talk(473);
		}
		else
		{
			washingtonFlag ++;
			talk(474);
		}
	}
}

int Henderson::checkpath(void) {
	BadRoom * thisRoom = ((BadMan *)intrpuzz)->Floors[floor]->room[target];
 	BadRoom * currentRoom = ((BadMan *)intrpuzz)->Floors[floor]->room[room];

	if (x > thisRoom->right)
	{
		if (currentRoom->w_wall == thisRoom->e_wall)
		{
			if (*(thisRoom->e_wall) != NONE)
			{
				return true;
			}
		}
	}
	else  if (x < thisRoom->left)
	{
		if (currentRoom->e_wall == thisRoom->w_wall)
		{
			if (*(thisRoom->w_wall) != NONE)
			{
				return true;
			}
		}
	}
	else if (y < thisRoom->top)
	{
		if (currentRoom->s_wall == thisRoom->n_wall)
		{
			if (*(thisRoom->n_wall) != NONE)
			{
				return true;
			}
		}
	}
	else if (y > thisRoom->bottom)
	{
		if (currentRoom->n_wall == thisRoom->s_wall)
		{
			if (*(thisRoom->s_wall) != NONE)
			{
				return true;
			}
		}
	}	
	return false;
}
void Henderson::special(void)
{
	int enemies = *(((BadMan *)intrpuzz)->Floors[floor]->fenemy[target]);
	
	if (checkpath())
	{
		if (enemies)
		{
			switch (enemies)
			{
				case G1:
				case YEL1:
				case R1:
					talk(453);
					break; 

				case G2:
				case YEL2:
				case R2:
					talk(454);
					break; 

				case G3:
				case YEL3:
				case R3:
					talk(455);
					break; 

				case BIRD:
					talk(452);
					break; 

				case LOFT:
					talk(456);
					break; 

				case MUSIC:
					talk(458);
					break; 

				case DOG:
					talk(457);
					break; 

				case SALES:
					talk(459);
					break; 

				case FUCK:
					talk(460);
					break; 

				case FAM:
					talk(461);
					break; 

				case PARTY:
					talk(462);
					break; 

				case CRAZY:
					talk(463);
					break; 

				default:
					talk(456);
					break;
			}		
		}
		else
		{
			talk(456);
		}
	}
}
void Henderson::die(void)
{
	if (!((BadMan *)intrpuzz)->whoops && !((BadMan *)intrpuzz)->oops)
	{
		((BadMan *)intrpuzz)->micOpen = false;

		if (talking)
			talking = false;
		((BadMan *)intrpuzz)->speech->stop();
		if (((BadMan *)intrpuzz)->death)
		{
			talk(477);
			((BadMan *)intrpuzz)->death = false;		
		}
		else
		{
			talk(478);
			((BadMan *)intrpuzz)->death	= true;
		}
	}
	Us::die();
}
void Henderson::announce(int what)
{
	switch (what)
	{
		case LOFT:
			talk(401);
			break;
		case DOG:
			talk(402);
			break;
		case MUSIC:
			talk(403);
			break;
		case SALES:
			talk(404);
			break;
		case FUCK:
			talk(405);
			break;
		case FAM:
			talk(406);
			break;
		case PARTY:
			talk(407);
			break;
		case CRAZY:
			talk(410);
			break;
		case PHONES:
			if (!((BadMan *)intrpuzz)->phones && !((BadMan *)intrpuzz)->somebodyistalkingaboutphones)	
			{	
				if (talking)
					talking = false;
				
				((BadMan *)intrpuzz)->micOpen = false;
				((BadMan *)intrpuzz)->speech->stop();
				talk(411);
				((BadMan *)intrpuzz)->somebodyistalkingaboutphones = true;
				talkingaboutphones = true;
			}
			break;
		case BOILER:
			talk(408);
			break;


		case NULL:
			talk(409);
			break;
		case DED:
			talk(409);
			break;
	}
}
void Schiff::announce(int what)
{
	switch (what)
	{
		case LOFT:
			talk(412);
			break;
		case DOG:
			talk(413);
			break;
		case MUSIC:
			talk(414);
			break;
		case SALES:
			talk(415);
			break;
		case FUCK:
			talk(416);
			break;
		case FAM:
			talk(417);
			break;
		case PARTY:
			talk(418);
			break;
		case CRAZY:
			talk(421);
			break;
		case PHONES:
			if (!((BadMan *)intrpuzz)->phones && !((BadMan *)intrpuzz)->somebodyistalkingaboutphones)	
			{	
				if (talking)
					talking = false;
				
				((BadMan *)intrpuzz)->micOpen = false;
				((BadMan *)intrpuzz)->speech->stop();
				talk(422);
				((BadMan *)intrpuzz)->somebodyistalkingaboutphones = true;
				talkingaboutphones = true;
			}
			break;
		case BOILER:
			talk(419);
			break;


		case NULL:
			talk(420);
			break;
		case DED:
			talk(420);
			break;
	}
}
void Washington::announce(int what)
{
	switch (what)
	{
		case LOFT:
			talk(423);
			break;
		case DOG:
			talk(424);
			break;
		case MUSIC:
			talk(425);
			break;
		case SALES:
			talk(426);
			break;
		case FUCK:
			talk(427);
			break;
		case FAM:
			talk(428);
			break;
		case PARTY:
			talk(429);
			break;
		case CRAZY:
			talk(432);
			break;
		case PHONES:
			if (!((BadMan *)intrpuzz)->phones && !((BadMan *)intrpuzz)->somebodyistalkingaboutphones)	
			{	
				if (talking)
					talking = false;
				
				((BadMan *)intrpuzz)->micOpen = false;
				((BadMan *)intrpuzz)->speech->stop();
				talk(433);
				((BadMan *)intrpuzz)->somebodyistalkingaboutphones = true;
				talkingaboutphones = true;
			}
			break;
		
		case BOILER:
			talk(430);
			break;


		case NULL:
			talk(431);
			break;
		case DED:
			talk(431);
			break;
	}
}
void Aguila::announce(int what)
{
	switch (what)
	{
		case LOFT:
			talk(434);
			break;
		case DOG:
			talk(435);
			break;
		case MUSIC:
			talk(436);
			break;
		case SALES:
			talk(437);
			break;
		case FUCK:
			talk(438);
			break;
		case FAM:
			talk(439);
			break;
		case PARTY:
			talk(440);
			break;
		case CRAZY:
			talk(443);
			break;
		case PHONES:
			if (!((BadMan *)intrpuzz)->phones && !((BadMan *)intrpuzz)->somebodyistalkingaboutphones)	
			{	
				if (talking)
					talking = false;
				
				((BadMan *)intrpuzz)->micOpen = false;
				((BadMan *)intrpuzz)->speech->stop();
				talk(444);
				((BadMan *)intrpuzz)->somebodyistalkingaboutphones = true;
				talkingaboutphones = true;
			}
			break;
		case BOILER:
			talk(441);
			break;


		case NULL:
			talk(442);
			break;
		case DED:
			talk(442);
			break;
	}
}
void Schiff::die(void)		 
{
	if (!((BadMan *)intrpuzz)->whoops && !((BadMan *)intrpuzz)->oops)
	{
		((BadMan *)intrpuzz)->micOpen = false;

		if (talking)
			talking = false;
		((BadMan *)intrpuzz)->speech->stop();
												    
		if (((BadMan *)intrpuzz)->death)
		{
			talk(479);												  
			((BadMan *)intrpuzz)->death = false;		
		}
		else
		{
			talk(480);
			((BadMan *)intrpuzz)->death	= true;
		}
	}
	Us::die();
}
void Washington::die(void)
{
	if (!((BadMan *)intrpuzz)->whoops && !((BadMan *)intrpuzz)->oops)
	{
		((BadMan *)intrpuzz)->micOpen = false;

		if (talking)
			talking = false;

		((BadMan *)intrpuzz)->speech->stop();

		if (((BadMan *)intrpuzz)->death)
		{
			talk(481);
			((BadMan *)intrpuzz)->death = false;		
		}
		else
		{
			talk(482);
			((BadMan *)intrpuzz)->death	= true;
		}
	}
	Us::die();
}
void Aguila::die(void)
{
	if (!((BadMan *)intrpuzz)->whoops && !((BadMan *)intrpuzz)->oops)
	{
		((BadMan *)intrpuzz)->micOpen = false;

		if (talking)
			talking = false;

		((BadMan *)intrpuzz)->speech->stop();

		if (((BadMan *)intrpuzz)->death)
		{
			talk(483);
			((BadMan *)intrpuzz)->death = false;		
		}
		else
		{
			talk(484);
			((BadMan *)intrpuzz)->death	= true;
		}
	}
	Us::die();
}
/************************
*
*  rooms Stuff
*
*************************/

int BadRoom::handleEvent(MADEEventStamp* event)
{
  if (onMe(event))  
  {
  	if (user->input)
	{
	  	if (event_type == USER_LEFT_UP)
	    {
			if (!((BadMan *)intrpuzz)->done)
 				activate();
	    }
	}
  }
  return false;
}

int BadRoom::combat(int* enemy, Us * guy)
{
	int num = 0;
	int count = 0;
	int enemies = 0;
	int badCombatFlag = false;

	if (*enemy == DED || *enemy == NULL)
	{
		*enemy = BEEN;
		return false;
	}
	else
	{
		if (*enemy == CIV || *enemy >= DOG)
		{
			return false;
		}
		else
		{
			if (*enemy != BEEN)
			{
				if (!((BadMan *)intrpuzz)->somebodyistalkingaboutphones)
				{
					((BadMan *)intrpuzz)->micOpen = false;
					((BadMan *)intrpuzz)->speech->stop();
				}
				combatants = true;
				guy->fighting = true;
				guy->fight();
				enemies = ((*enemy%10) + 1);			
				while (enemies)
				{
					num = rand() & 10;
					if (num < 8)
					{
						enemies --;		
					}
					else
					{
						guy->hitpoints --;
						badCombatFlag = true;
					}

					if (!guy->hitpoints)
					{
						break;
					}
				}		

				if (badCombatFlag)
				{
	//				sfxPrintf("I'm Hit!!!");
				}

				if (!enemies)
				{
					if (*enemy != BIRD)
					{
//						*enemy = BEEN;		
						for (count=0;count<22;count++)
						{
						 	if (parent == (((BadMan *)intrpuzz)->Floors[floor_num]->room[count])->parent)
								*(((BadMan *)intrpuzz)->Floors[floor_num]->fenemy[count]) = BEEN;
						}
					}	
 					return false;
				}
				else
				{
					if (*enemy != BIRD)
					{
						if (*enemy != LOFT)
							*enemy = enemies + 119;
		  			}	
		  			return true;
				}
			}
			return false;
		}
	}
}

void BadRoom::activate(void)
{
	Us **guy = &((BadMan *)intrpuzz)->guy[0];
	int count = 0;
	int count2 = 0;
	int pass = 2;
	//FloorMap* thisroom = &mastermap[room_num];
	BadRoom* room = NULL;	

	if (floor_num == ((BadMan *)intrpuzz)->curFloorNum)
	{
		while (pass)
		{
			guy = &((BadMan *)intrpuzz)->guy[0];
			if (--pass<0)
				pass = 0;
			for (count = 0;count<4;count++)
			{
				if (*guy)
				{
					if ((*guy)->state == WAITING)
					{
	//					(*guy)->setCel(0);

						if ((*guy)->command == WALK && !pass)
						{
							// set walk to for this guy
							// temporary for rough
							if (!checkpath(*guy) || ((*guy)->floor != ((BadMan *)intrpuzz)->curFloorNum))
							{
								if (room_num == (*guy)->room && ((*guy)->floor == ((BadMan *)intrpuzz)->curFloorNum))
								{
									sfxPlaySound (535,100,64,64,NULL);
									(*guy)->reset();
								}
								else
								{	
									(*guy)->finaltarget = room_num;
									(*guy)->targetfloor = ((BadMan *)intrpuzz)->curFloorNum;
									(*guy)->startJourney();
								}
						
							}
							else
							{
								if ((*guy)->floor == ((BadMan *)intrpuzz)->curFloorNum)
								{
									if (special)
									{
										(*guy)->room_special = special;
									}

									(*guy)->target = room_num;
									(*guy)->finaltarget = room_num;
									(*guy)->destX = mid_x;
									(*guy)->destY = mid_y;
									(*guy)->startWalk();
									sfxPlaySound (520,100,64,64,NULL);
								}
								else
								{
									sfxPlaySound (535,100,64,64,NULL);
									(*guy)->reset();
								}
			 				}	
						}				  
						else if	((*guy)->command != WALK)
						{
							if (pass)
							{
								if(!combatants)
								{
									for (count2=0;count2<22;count2++)
									{
										room = ((BadMan *)intrpuzz)->Floors[floor_num]->room[count2];
										if (parent == room->parent && (room->e_room == (*guy)->room || room->w_room == (*guy)->room || 
											room->s_room == (*guy)->room || room->n_room == (*guy)->room))
										{
											// trigger special ability for guy on this room...			
											(*guy)->target = count2;							
											//temporary for rough
											sfxPlaySound (520,100,64,64,NULL);
											(*guy)->special();

			//								(*guy)->reset();
											break;
										}
									}

									if ((*guy)->id == 3 && (*guy)->room == room_num)
									{
										// trigger special ability for guy on this room...			
										(*guy)->target = room_num;							
										//temporary for rough
										sfxPlaySound (520,100,64,64,NULL);
										(*guy)->special();

			//							(*guy)->reset();
									}
									else if (count>=21)
										sfxPlaySound (535,100,64,64,NULL);
								}
								else
									sfxPlaySound (535,100,64,64,NULL);
								(*guy)->reset();
							}
						}
					}
				}
				*guy++;
			}
		}
		memset(&((BadMan *)intrpuzz)->guy,0,(4*sizeof(Us*)));
	}
}	

int BadRoom::checkpath(Us * guy)
{
	int cur_x = guy->x;
	int cur_y = guy->y;							  
	int room = guy->room;
	FloorMap* thisroom = &mastermap[room_num];

	if (cur_x > right)
	{
		if (room == thisroom->e)
		{ 
			if (!e_wall)
			{
				guy->halfdestX = right;
				guy->halfdestY = mid_y;
				return true;
			}
			else if (*e_wall)
			{
				guy->halfdestX = right;
				guy->halfdestY = mid_y;
				return true;
			}
		}
	}
	else  if (cur_x < left)
	{
		if (room == thisroom->w)
		{
			if (!w_wall)
			{
				guy->halfdestX = left;
				guy->halfdestY = mid_y;
				return true;
			}
			else if (*w_wall)
			{
				guy->halfdestX = left;
				guy->halfdestY = mid_y;
				return true;
			}
		}
	}
	else if (cur_y < top)
	{
		if (room == thisroom->n)
		{
			if (!n_wall)
			{
				guy->halfdestX = mid_x;
				guy->halfdestY = top;
				return true;
			}
			else if (*n_wall)
			{
				guy->halfdestX = mid_x;
				guy->halfdestY = top;
				return true;
			}
		}
	}
	else if (cur_y > bottom)
	{
		if (room == thisroom->s)
		{
			if (!s_wall)
			{
				guy->halfdestX = mid_x;
				guy->halfdestY = bottom;
				return true;
			}
			else if (*s_wall)
			{
				guy->halfdestX = mid_x;
				guy->halfdestY = bottom;
				return true;
			}
		}
	}	
	return false;
}

void BadRoom::init ( int theX, int theY, int theWidth, int theHeight, int* north, int* east, int* south, int* west,
					 int n, int e, int s, int w, int spec, int num,int floor,int group)
{
	IntrPuzzFeature::init (theX,theY,theWidth,theHeight );
	floor_num = floor;
	room_num = num;
	special = spec;
	n_wall = north;
	e_wall = east;
	s_wall = south;
	w_wall = west;
	n_room = n;
	e_room = e;
	s_room = s;
	w_room = w;
	mid_x = theX + (theWidth>>1);
	mid_y = theY + (theHeight>>1);
	parent = group;
	combatants = false;
}
void BadRoom::check4Hole()
{
	if (n_wall)
	{
		if (*n_wall == HOLE)
		{
			drawHole(mid_x,top);		
		}
	}
	if (e_wall)
	{
		if (*e_wall == HOLE)
		{									 
			drawHole(right,mid_y);		
		}
	}
	if (s_wall)
	{
		if (*s_wall == HOLE)
		{
			drawHole(mid_x,bottom);		
		}
	}
	if (w_wall)
	{
		if (*w_wall == HOLE)
		{	 
			drawHole(left,mid_y);		
		}
	}
}

void BadRoom::drawHole(int hole_x,int hole_y)
{
	IntrPuzzView * hole;
	hole = new (Hole);
	hole->init(hole_x,hole_y, 611,4,0);
	hole->addToPic();
}		
Beacon::Beacon(Us* newguy)
{
	guy = newguy;		
	team->addToFront(this);
}
void Beacon::cue(void)
{												  
	guy->setCel(0);
	guy->setCycle(new EndLoop((Prop*)guy, this));
}
/************************
*
*  create Stuff
*
*************************/
FloorBack::FloorBack(int num)
{
	name = "backgrnd";
	switch (num)
	{
		case 0:
			init (0,55,610,0,0);
			break;
		case 1:
			init (0,55,608,0,0);
			break;
		case 2:
			init (0,55,609,0,0);
			break;
	}
	noSkip();
	addToPic();
}

BaseTab::BaseTab()
{
	init(470,247,611,1,0);
	noSkip();
}

FirstTab::FirstTab()
{
	init(470,209,611,2,0);
	noSkip();
}

SecondTab::SecondTab()
{
	init(470,171,611,3,1);
	noSkip();
}

Tactical::Tactical()
{
	oakalydokaly = true;
	init(470,89,611,0,0);
	noSkip();
}

/*********************
*
*		Features
*
**********************/
Floor1::Floor1()
{
	int x;
	int * int_ptr;
	//int count =	0;

	int_ptr = &((BadMan *)intrpuzz)->walls[0];

	for ( x = 0; x < 33; x++ ) 
	{
		fwalls[x] =	int_ptr;
		int_ptr ++;
	}

	int_ptr = &((BadMan *)intrpuzz)->enemy[0];

	for ( x = 0; x < 22; x++ ) 
	{
		fenemy[x] =	int_ptr;
		int_ptr ++;
	}

	room[ 0]=new (BadRoom);
	room[ 1]=new (BadRoom);
	room[ 2]=new (BadRoom);			
	room[ 3]=new (BadRoom);
	room[ 4]=new (BadRoom);
	room[ 5]=new (BadRoom);
	room[ 6]=new (BadRoom);
	room[ 7]=new (BadRoom);
	room[ 8]=new (BadRoom);
	room[ 9]=new (BadRoom);
	room[10]=new (BadRoom);
	room[11]=new (BadRoom);
	room[12]=new (BadRoom);
	room[13]=new (BadRoom);
	room[14]=new (BadRoom);
	room[15]=new (BadRoom);
	room[16]=new (BadRoom);
	room[17]=new (BadRoom);
	room[18]=new (BadRoom);
	room[19]=new (BadRoom);
	room[20]=new (BadRoom);
	room[21]=new (BadRoom);

//	room[ 0]->init ( int theX, int theY, int theWidth, int theHeight, int north, int east, int south, int west
//					int n, int e, int s, int w, int spec, int num,int floor,int parent);

	room[ 0]->init ( 65,106, 82, 48,      NULL,fwalls[ 0],fwalls[ 3],      NULL,99, 1, 4,99,NULL, 0,1, 0);
	room[ 1]->init (148,106, 82, 48,      NULL,fwalls[ 1],fwalls[ 4],fwalls[ 0],99, 2, 5, 0,NULL, 1,1, 1);
	room[ 2]->init (229,106, 82, 48,      NULL,fwalls[ 2],fwalls[ 5],fwalls[ 1],99, 3, 6, 1,NULL, 2,1, 2);
	room[ 3]->init (311,106, 82, 48,      NULL,      NULL,fwalls[ 6],fwalls[ 2],99,99, 7, 2,NULL, 3,1, 3);
	room[ 4]->init ( 65,154, 82, 48,fwalls[ 3],fwalls[ 7],fwalls[10],      NULL, 0, 5, 8,99,NULL, 4,1, 0);
	room[ 5]->init (148,154, 82, 48,fwalls[ 4],fwalls[ 8],fwalls[11],fwalls[ 7], 1, 6, 9, 4,NULL, 5,1, 1);
	room[ 6]->init (229,154, 82, 48,fwalls[ 5],fwalls[ 9],fwalls[12],fwalls[ 8], 2, 7,10, 5,NULL, 6,1, 2);
	room[ 7]->init (312,154, 82, 48,fwalls[ 6],      NULL,fwalls[13],fwalls[ 9], 3,99,11, 6,NULL, 7,1, 3);
	room[ 8]->init ( 65,201, 82, 48,fwalls[10],fwalls[15],fwalls[19],fwalls[14], 4, 9,12,20,NULL, 8,1, 8);
	room[ 9]->init (148,201, 82, 48,fwalls[11],fwalls[16],fwalls[20],fwalls[15], 5,10,13, 8,NULL, 9,1, 8);
	room[10]->init (229,201, 82, 48,fwalls[12],fwalls[17],fwalls[21],fwalls[16], 6,11,14, 9,NULL,10,1, 8);
	room[11]->init (312,201, 82, 48,fwalls[13],fwalls[18],fwalls[22],fwalls[17], 7,21,15,10,NULL,11,1, 8);
	room[12]->init ( 65,249, 82, 48,fwalls[19],fwalls[23],fwalls[26],      NULL, 8,13,16,99,NULL,12,1,12);
	room[13]->init (148,249, 82, 48,fwalls[20],fwalls[24],fwalls[27],fwalls[23], 9,14,17,12,NULL,13,1,13);
	room[14]->init (229,249, 82, 48,fwalls[21],fwalls[25],fwalls[28],fwalls[24],10,15,18,13,NULL,14,1,14);
	room[15]->init (312,249, 82, 48,fwalls[22],      NULL,fwalls[29],fwalls[25],11,99,19,14,NULL,15,1,15);
	room[16]->init ( 65,297, 82, 48,fwalls[26],fwalls[30],      NULL,      NULL,12,17,99,99,NULL,16,1,16);
	room[17]->init (148,297, 82, 48,fwalls[27],fwalls[31],      NULL,fwalls[30],13,18,99,16,NULL,17,1,13);
	room[18]->init (229,297, 82, 48,fwalls[28],fwalls[32],      NULL,fwalls[31],14,19,99,17,NULL,18,1,18);
	room[19]->init (312,297, 82, 48,fwalls[29],      NULL,      NULL,fwalls[32],15,99,99,18,NULL,19,1,15);
	room[20]->init ( 18,201, 51, 48,      NULL,fwalls[14],      NULL,      NULL,99, 8,99,99,NULL,20,1,20);
	room[21]->init (395,201, 51, 48,      NULL,      NULL,      NULL,fwalls[18],99,99,99,11,NULL,21,1,21);
}

Floor2::Floor2()
{
	int x;
	int * int_ptr;
	//int count =	0;

	int_ptr = &((BadMan *)intrpuzz)->walls[33];

	for ( x = 0; x < 33; x++ ) 
	{
		fwalls[x] =	int_ptr;
		int_ptr ++;
	}

	int_ptr = &((BadMan *)intrpuzz)->enemy[22];

	for ( x = 0; x < 22; x++ ) 
	{
		fenemy[x] =	int_ptr;
		int_ptr ++;
	}

	room[ 0]=new (BadRoom);
	room[ 1]=new (BadRoom);
	room[ 2]=new (BadRoom);			
	room[ 3]=new (BadRoom);
	room[ 4]=new (BadRoom);
	room[ 5]=new (BadRoom);
	room[ 6]=new (BadRoom);
	room[ 7]=new (BadRoom);
	room[ 8]=new (BadRoom);
	room[ 9]=new (BadRoom);
	room[10]=new (BadRoom);
	room[11]=new (BadRoom);
	room[12]=new (BadRoom);
	room[13]=new (BadRoom);
	room[14]=new (BadRoom);
	room[15]=new (BadRoom);
	room[16]=new (BadRoom);
	room[17]=new (BadRoom);
	room[18]=new (BadRoom);
	room[19]=new (BadRoom);
	room[20]=new (BadRoom);
	room[21]=new (BadRoom);

//	room[ 0]->init ( int theX, int theY, int theWidth, int theHeight, int north, int east, int south, int west
//					int n, int e, int s, int w, int spec, int num,int floor,int parent);

	room[ 0]->init ( 65,106, 82, 48,      NULL,fwalls[ 0],fwalls[ 3],      NULL,99, 1, 4,99,NULL, 0,2, 0);
	room[ 1]->init (148,106, 82, 48,      NULL,fwalls[ 1],fwalls[ 4],fwalls[ 0],99, 2, 5, 0,NULL, 1,2, 1);
	room[ 2]->init (229,106, 82, 48,      NULL,fwalls[ 2],fwalls[ 5],fwalls[ 1],99, 3, 6, 1,NULL, 2,2, 2);
	room[ 3]->init (311,106, 82, 48,      NULL,      NULL,fwalls[ 6],fwalls[ 2],99,99, 7, 2,NULL, 3,2, 3);
	room[ 4]->init ( 65,154, 82, 48,fwalls[ 3],fwalls[ 7],fwalls[10],      NULL, 0, 5, 8,99,NULL, 4,2, 0);
	room[ 5]->init (148,154, 82, 48,fwalls[ 4],fwalls[ 8],fwalls[11],fwalls[ 7], 1, 6, 9, 4,NULL, 5,2, 1);
	room[ 6]->init (229,154, 82, 48,fwalls[ 5],fwalls[ 9],fwalls[12],fwalls[ 8], 2, 7,10, 5,NULL, 6,2, 6);
	room[ 7]->init (312,154, 82, 48,fwalls[ 6],      NULL,fwalls[13],fwalls[ 9], 3,99,11, 6,NULL, 7,2, 3);
	room[ 8]->init ( 65,201, 82, 48,fwalls[10],fwalls[15],fwalls[19],fwalls[14], 4, 9,12,20,NULL, 8,2, 8);
	room[ 9]->init (148,201, 82, 48,fwalls[11],fwalls[16],fwalls[20],fwalls[15], 5,10,13, 8,NULL, 9,2, 8);
	room[10]->init (229,201, 82, 48,fwalls[12],fwalls[17],fwalls[21],fwalls[16], 6,11,14, 9,NULL,10,2, 8);
	room[11]->init (312,201, 82, 48,fwalls[13],fwalls[18],fwalls[22],fwalls[17], 7,21,15,10,NULL,11,2, 8);
	room[12]->init ( 65,249, 82, 48,fwalls[19],fwalls[23],fwalls[26],      NULL, 8,13,16,99,NULL,12,2,12);
	room[13]->init (148,249, 82, 48,fwalls[20],fwalls[24],fwalls[27],fwalls[23], 9,14,17,12,NULL,13,2,13);
	room[14]->init (229,249, 82, 48,fwalls[21],fwalls[25],fwalls[28],fwalls[24],10,15,18,13,NULL,14,2,14);
	room[15]->init (312,249, 82, 48,fwalls[22],      NULL,fwalls[29],fwalls[25],11,99,19,14,NULL,15,2,15);
	room[16]->init ( 65,297, 82, 48,fwalls[26],fwalls[30],      NULL,      NULL,12,17,99,99,NULL,16,2,12);
	room[17]->init (148,297, 82, 48,fwalls[27],fwalls[31],      NULL,fwalls[30],13,18,99,16,NULL,17,2,13);
	room[18]->init (229,297, 82, 48,fwalls[28],fwalls[32],      NULL,fwalls[31],14,19,99,17,NULL,18,2,18);
	room[19]->init (312,297, 82, 48,fwalls[29],      NULL,      NULL,fwalls[32],15,99,99,18,NULL,19,2,15);
	room[20]->init ( 18,201, 51, 48,      NULL,fwalls[14],      NULL,      NULL,99, 8,99,99,NULL,20,2,20);
	room[21]->init (395,201, 51, 48,      NULL,      NULL,      NULL,fwalls[18],99,99,99,11,NULL,21,2,21);
}

Base::Base()
{
	int x;
	int * int_ptr;
	//int count =	0;

	int_ptr = &((BadMan *)intrpuzz)->walls[66];

	for ( x = 0; x < 33; x++ ) 
	{
		fwalls[x] =	int_ptr;
		int_ptr ++;
	}

	int_ptr = &((BadMan *)intrpuzz)->enemy[44];

	for ( x = 0; x < 22; x++ ) 
	{
		fenemy[x] =	int_ptr;
		int_ptr ++;
	 }

	room[ 0]=new (BadRoom);
	room[ 1]=new (BadRoom);
	room[ 2]=new (BadRoom);			
	room[ 3]=new (BadRoom);
	room[ 4]=new (BadRoom);
	room[ 5]=new (BadRoom);
	room[ 6]=new (BadRoom);
	room[ 7]=new (BadRoom);
	room[ 8]=new (BadRoom);
	room[ 9]=new (BadRoom);
	room[10]=new (BadRoom);
	room[11]=new (BadRoom);
	room[12]=new (BadRoom);
	room[13]=new (BadRoom);
	room[14]=new (BadRoom);
	room[15]=new (BadRoom);
	room[16]=new (BadRoom);
	room[17]=new (BadRoom);
	room[18]=new (BadRoom);
	room[19]=new (BadRoom);
	room[20]=new (BadRoom);
	room[21]=new (BadRoom);

//	room[ 0]->init ( int theX, int theY, int theWidth, int theHeight, int north, int east, int south, int west
//					int n, int e, int s, int w, int spec, int num,int floor,int parent);

	room[ 0]->init ( 65,106, 82, 48,      NULL,fwalls[ 0],fwalls[ 3],      NULL,99, 1, 4,99,NULL, 0,0, 0);
	room[ 1]->init (148,106, 82, 48,      NULL,fwalls[ 1],fwalls[ 4],fwalls[ 0],99, 2, 5, 0,NULL, 1,0, 1);
	room[ 2]->init (229,106, 82, 48,      NULL,fwalls[ 2],fwalls[ 5],fwalls[ 1],99, 3, 6, 1,NULL, 2,0, 1);
	room[ 3]->init (311,106, 82, 48,      NULL,      NULL,fwalls[ 6],fwalls[ 2],99,99, 7, 2,NULL, 3,0, 3);
	room[ 4]->init ( 65,154, 82, 48,fwalls[ 3],fwalls[ 7],fwalls[10],      NULL, 0, 5, 8,99,NULL, 4,0, 0);
	room[ 5]->init (148,154, 82, 48,fwalls[ 4],fwalls[ 8],fwalls[11],fwalls[ 7], 1, 6, 9, 4,NULL, 5,0, 5);
	room[ 6]->init (229,154, 82, 48,fwalls[ 5],fwalls[ 9],fwalls[12],fwalls[ 8], 2, 7,10, 5,NULL, 6,0, 5);
	room[ 7]->init (312,154, 82, 48,fwalls[ 6],      NULL,fwalls[13],fwalls[ 9], 3,99,11, 6,NULL, 7,0, 3);
	room[ 8]->init ( 65,201, 82, 48,fwalls[10],fwalls[15],fwalls[19],fwalls[14], 4, 9,12,20,NULL, 8,0, 8);
	room[ 9]->init (148,201, 82, 48,fwalls[11],fwalls[16],fwalls[20],fwalls[15], 5,10,13, 8,NULL, 9,0, 5);
	room[10]->init (229,201, 82, 48,fwalls[12],fwalls[17],fwalls[21],fwalls[16], 6,11,14, 9,NULL,10,0, 5);
	room[11]->init (312,201, 82, 48,fwalls[13],fwalls[18],fwalls[22],fwalls[17], 7,21,15,10,NULL,11,0,11);
	room[12]->init ( 65,249, 82, 48,fwalls[19],fwalls[23],fwalls[26],      NULL, 8,13,16,99,NULL,12,0,12);
	room[13]->init (148,249, 82, 48,fwalls[20],fwalls[24],fwalls[27],fwalls[23], 9,14,17,12,NULL,13,0, 5);
	room[14]->init (229,249, 82, 48,fwalls[21],fwalls[25],fwalls[28],fwalls[24],10,15,18,13,NULL,14,0, 5);
	room[15]->init (312,249, 82, 48,fwalls[22],      NULL,fwalls[29],fwalls[25],11,99,19,14,NULL,15,0,15);
	room[16]->init ( 65,297, 82, 48,fwalls[26],fwalls[30],      NULL,      NULL,12,17,99,99,NULL,16,0,12);
	room[17]->init (148,297, 82, 48,fwalls[27],fwalls[31],      NULL,fwalls[30],13,18,99,16,NULL,17,0,17);
	room[18]->init (229,297, 82, 48,fwalls[28],fwalls[32],      NULL,fwalls[31],14,19,99,17,NULL,18,0,17);
	room[19]->init (312,297, 82, 48,fwalls[29],      NULL,      NULL,fwalls[32],15,99,99,18,NULL,19,0,15);
	room[20]->init ( 18,201, 51, 48,      NULL,fwalls[14],      NULL,      NULL,99, 8,99,99,NULL,20,0,20);
	room[21]->init (395,201, 51, 48,      NULL,      NULL,      NULL,fwalls[18],99,99,99,11,NULL,21,0,21);
}

void BadMan::initWalls()
{
	memcpy (&walls,&temp_walls,(99 * sizeof(int)));
}

void BadMan::initEnemy()
{
	memcpy (&enemy,&temp_enemy,(99 * sizeof(int)));
}

walkScript::walkScript(Us * guy)
{
	name = "walkscript";
	thisguy = guy;
}

void walkScript::changeState ( int newState )
{
	int * thisEnemy;
	int special;
	int count = 0;
	int tmp;
	int floor = thisguy->floor;

	switchTo
		if (!((BadMan *)intrpuzz)->done)
		{
			if ((thisguy->destX - thisguy->x) > 15 || (thisguy->destX - thisguy->x) < -15)
			{
				tmp = thisguy->cel;
				thisguy->setLoop(thisguy->id + 4);
				thisguy->setCel(tmp);
				thisguy->setCycle(new EndLoop((Prop*)thisguy, thisguy->beacon));
			}
			else
			{
				tmp = thisguy->cel;
				thisguy->setLoop(thisguy->id + 8);
				thisguy->setCel(tmp);
				thisguy->setCycle(new EndLoop((Prop*)thisguy, thisguy->beacon));
			}
			thisguy->setMotion( new MoveTo(thisguy->halfdestX,thisguy->halfdestY,(Actor*)thisguy,this));
		}
		else
		{
			thisguy->setScript(NULL);				
		}
    END

	BEG
		int speak = false;
		BadRoom* room = ((BadMan *)intrpuzz)->Floors[floor]->room[thisguy->room];
		BadRoom* targetRoom = ((BadMan *)intrpuzz)->Floors[floor]->room[thisguy->target];		
		if (targetRoom->combatants)
		{
			thisguy->holding = true;
			state -=1;
			ticks = 15;
			return;
		}
		else 
		{
			thisguy->holding = false;
		}
		special = thisguy->room_special;	
		thisguy->room_special = NULL;
			
		if (!((BadMan *)intrpuzz)->done)
		{
			if (!thisguy->log[floor][thisguy->target])
			{
				if (room->parent != targetRoom->parent)
					speak = true;		
			}
			if (special)
			{
			}
			else
			{
				thisguy->room = thisguy->target;
			}

			thisEnemy = ((BadMan *)intrpuzz)->Floors[floor]->fenemy[thisguy->room];
			room = ((BadMan *)intrpuzz)->Floors[floor]->room[thisguy->room];
			if (speak)
			{
				thisguy->announce(*thisEnemy);
			}

			if (* thisEnemy && *thisEnemy != HALL)
			{
				if (room->combat(thisEnemy,thisguy))
				{
					thisguy->alive = false;
				}
				else
				{
					if	(*thisEnemy == PHONES || *thisEnemy == BOILER || *thisEnemy == BIRD)
					{
					}
					else
					{
//						thisguy->log[thisguy->floor][thisguy->room] = BEEN;
						for (count = 0;count < 22;count++)
						{
							if (((BadMan *)intrpuzz)->Floors[floor]->room[count]->parent == room->parent)
								thisguy->log[floor][count] = BEEN;		
						}
					}
				}
				if ((thisguy->fighting && !thisguy->talking) || !thisguy->fighting)
				{
					thisguy->fighting = false;
					cue();
				}
			}
			else
			{
//				thisguy->log[floor][thisguy->room] = BEEN;
				for (count = 0;count < 22;count++)
				{
					if (((BadMan *)intrpuzz)->Floors[floor]->room[count]->parent == room->parent)
						thisguy->log[floor][count] = BEEN;		
				}
				cue();
			}
		}
		else
		{
			thisguy->reset();
			thisguy->setScript(NULL);				
		}
	END
 /*
	BEG
			if (!thisguy->alive)
				ticks = 15;
			else
				cue();
	END
*/
	BEG
		thisEnemy = ((BadMan *)intrpuzz)->Floors[floor]->fenemy[thisguy->room];

		if (!thisguy->alive)
		{
			thisguy->die();
			thisguy->hide();
			thisguy->setScript(NULL);	
			((BadMan *)intrpuzz)->casualties ++;
			return;
		}

		if (*thisEnemy == BIRD && !((BadMan *)intrpuzz)->done)
		{
			thisguy->talk(SUCCESS);				
		}
		else
		{
			cue();
		}
	END

	BEG
		if (!((BadMan *)intrpuzz)->done)
		{
			if (thisguy->target == thisguy->finaltarget)
				switch (thisguy->id)
				{
					case 0:
						thisguy->setMotion( new MoveTo(thisguy->destX+10,thisguy->destY,(Actor*)thisguy,this));
						break;	
					case 1:
						thisguy->setMotion( new MoveTo(thisguy->destX-10,thisguy->destY,(Actor*)thisguy,this));
						break;	
					case 2:
						thisguy->setMotion( new MoveTo(thisguy->destX,thisguy->destY+10,(Actor*)thisguy,this));
						break;	
					case 3:
						thisguy->setMotion( new MoveTo(thisguy->destX,thisguy->destY-10,(Actor*)thisguy,this));
						break;	
				}
			else
				thisguy->setMotion( new MoveTo(thisguy->destX,thisguy->destY,(Actor*)thisguy,this));
		}
		else
		{
			thisguy->reset();
			thisguy->setScript(NULL);				
		}
    END
	
	BEG	
		if (!((BadMan *)intrpuzz)->done)
		{
			if (thisguy->target == thisguy->finaltarget)
			{
				thisguy->here = 0;
				memset (&thisguy->path,0,(100 * (sizeof(int))));

				if (thisguy->room > 19)
				{
					if (thisguy->interimtarget > -1)
					{
						thisguy->finaltarget = thisguy->interimtarget;
						thisguy->interimtarget = -1;
						thisguy->floor = thisguy->targetfloor;
						thisguy->targetfloor = -1;
						
						if (thisguy->floor == ((BadMan *)intrpuzz)->curFloorNum)
						{
							if (thisguy->alive)
								thisguy->show();
						}	
						else
							thisguy->hide();

						thisguy->secondJourney();																	
						return;
					}
					else
					{
						if (thisguy->alive)
							thisguy->show();
						thisguy->floor = ((BadMan *)intrpuzz)->curFloorNum;
					}
				}	

				tmp = thisguy->cel;
				thisguy->setLoop(thisguy->id);
				thisguy->setCel(tmp);
				thisguy->setCycle(new EndLoop((Prop*)thisguy, thisguy->beacon));
				thisguy->reset();
				thisguy->setScript(NULL);				
			}
			else
			{
				if (thisguy->room > 19)
				{
					if (thisguy->alive)
						thisguy->show();
					thisguy->floor = ((BadMan *)intrpuzz)->curFloorNum;
				}	
				thisguy->updateJourney();
			}
		}
		else
		{
			thisguy->reset();
			thisguy->setScript(NULL);				
		}
	END

	BEG
		thisguy->reset();
		thisguy->setScript(NULL);
	END
} 

} // namespace Spycraft
