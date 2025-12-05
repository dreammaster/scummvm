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

#include "spycraft/game/goggles.h"
#include "spycraft/logic/93000.h"
#include "spycraft/logic/95100.h"
#include "spycraft/logic/95200.h"
#include "spycraft/logic/95300.h"
#include "spycraft/logic/95000.h"
#include "spycraft/logic/60000.h"
#include "spycraft/logic/1000.h"
#include "spycraft/game/advmem.h"
#include "spycraft/game/vlink.h"
#include "spycraft/game/email.h"

namespace Spycraft {

extern SoundList *sounds;


int move[9][4]	= {   TRY_NOTHING, 	 TRY_NOTHING, 	TRY_NOTHING,   TRY_NOTHING,
				           TRY_UP,	 TRY_UP_LEFT,  TRY_UP_RIGHT,   TRY_NOTHING,
				      TRY_UP_LEFT,		  TRY_UP,  TRY_UP_RIGHT, TRY_DOWN_LEFT,
				     TRY_UP_RIGHT,		  TRY_UP,   TRY_UP_LEFT,TRY_DOWN_RIGHT,
				      TRY_UP_LEFT, TRY_DOWN_LEFT,        TRY_UP,      TRY_DOWN,
				     TRY_UP_RIGHT,TRY_DOWN_RIGHT,        TRY_UP,      TRY_DOWN,
				   		 TRY_DOWN, TRY_DOWN_LEFT,TRY_DOWN_RIGHT,   TRY_NOTHING,
				    TRY_DOWN_LEFT,		TRY_DOWN,TRY_DOWN_RIGHT,   TRY_UP_LEFT,
				   TRY_DOWN_RIGHT,	    TRY_DOWN, TRY_DOWN_LEFT,  TRY_UP_RIGHT};

int gridToHex [9][5]	= {-1,-1, 0,-1,-1,
						   -1, 1,-1, 2,-1,
						    3,-1, 4,-1, 5,
						   -1, 6,-1, 7,-1,			   
						    8,-1, 9,-1,10,
						   -1,11,-1,12,-1,
						   13,-1,14,-1,15,										   
						   -1,16,-1,17,-1,
						   -1,-1,18,-1,-1};

MADEPt coords[19] = {	{381,396},
						{362,405},	
						{399,405},	
						{344,412},	
						{381,412},	
						{414,412},	
						{362,420},	
						{399,420},	
						{344,428},	
						{381,428},	
						{414,428},	
						{362,436},	
						{399,436},	
						{344,444},	
						{381,444},	
						{414,444},	
						{362,452},	
						{399,452},	
						{381,460}};
int E_map [9][5];
int P_map [9][5];
int T_list[19][15];
int	addshooters = true;

ShootText * warning;

int	secOnes;
int	secTens;
int	minOnes;
int	minTens;
int Hp;

int g1;
int g2;
int g3;
int g4;
int h1;
int h2;
int f1;
int f2;
bea* b_ptr;
int	shooterRow;
int	shooterColumn;

int shooterpos = 0;

SecOnes*	psecOnes;
SecTens*	psecTens;
MinOnes*	pminOnes;
MinTens*	pminTens;
Seperator * seperator;
MADEPt		enePos[19];
eneme *		enes[19];

Gunman * Gman[4];
int sniped = false;
int firstsniped = true;
int *baddies;
int *posted;
int gridnumber;
int base;
int ammo;
int clips;
int curhud;
int mt;
int countrooms = 0;
Grid*	grid;
Elapsed * elapsed;
Sound* instructions = new Sound;
int number =  95098;
int grendlPos = 95209;
Patrol *dude[6] = {NULL,NULL,NULL,NULL,NULL};
 
void TunisiaScript::changeState ( int newState )
{
	switchTo
		if (GameFlag.test(fStartCombat))
		{
			theGame->handsOff();
			instructions->canInterupt = true;
			instructions->number = 95300;
			instructions->playBuffered(95300,this);
		}
		else
			cue();
    END

	BEG
		if (GameFlag.test(fStartCombat))
		{
			theMusic->loop = -1;
			theMusic->setVolume(5);
			theMusic->play(95300);
			theGame->handsOn();
			GameFlag.clear(fStartCombat);
		}
		instructions->canInterupt = false;
		curRoom->setScript(new (WaitingScript));
	END
}
void RefScript::changeState ( int newState )
{
	switchTo
		if (GameFlag.test(fStartCombat))
		{
			theGame->handsOff();
			instructions->canInterupt = true;
			instructions->number = 95200;
			instructions->playBuffered(95200,this);
		}
		else
			cue();
    END
	BEG
		if (GameFlag.test(fStartCombat))
		{
			theMusic->loop = -1;
			theMusic->setVolume(5);
			theMusic->play(95200);
			instructions->number = 95205;
			instructions->playBuffered(95205,this);
		}
		else
			cue();
    END
	BEG
		if (GameFlag.test(fStartCombat))
		{
			theGame->handsOn();
			GameFlag.clear(fStartCombat);
		}
		instructions->canInterupt = false;
		curRoom->setScript(new (WaitingScript));
	END
}
void COScript::changeState ( int newState )
{
	switchTo
		if (GameFlag.test(fStartCombat))
		{
			theGame->handsOff();
			instructions->canInterupt = true;
			instructions->number = 95099;
			instructions->playBuffered(95099,this);
		}
		else
			cue();
    END

	BEG
		if (GameFlag.test(fStartCombat))
		{
			theMusic->loop = -1;
			theMusic->setVolume(5);
			theMusic->play(95100);
			theGame->handsOn();
			elapsed = new Elapsed;
			GameFlag.clear(fStartCombat);
			elapsed->setScript(new TimerScript);
		}
		instructions->canInterupt = false;
		curRoom->setScript(new (WaitingScript));
	END
}

void WPStartScript::changeState ( int newState )
{
	switchTo
		if (GameFlag.test(fStartCombat))
		{
			theGame->handsOff();
			instructions->canInterupt = true;
			if (number == 95094)
			{
				number = 95095;
				instructions->number = 95095;
				instructions->playBuffered(95095,this);
			}
			else if	(number == 95095)
			{
				number = 95094;
				instructions->number = 95098;
				instructions->playBuffered(95098,this);
			}
			else
			{
				number = 95094;
				instructions->number = 95094;
				instructions->playBuffered(95094,this);
			}
		}
		else
			cue();

    END

	BEG
		if (GameFlag.test(fStartCombat))
		{
			theMusic->loop = -1;
			theMusic->setVolume(5);
			theMusic->play(95000);
			theGame->handsOn();
			elapsed = new Elapsed;
			GameFlag.clear(fStartCombat);
			elapsed->setScript(new TimerScript);
		}
		instructions->canInterupt = false;
		curRoom->setScript(new (WaitingScript));
	END
}

Seperator::Seperator()
{															 
	fore_color = madeRGB(255, 255, 0);
	size = 16;
	style = STYL_PLAIN | STYL_ITALIC;
	x = 156;
	y = 448;
}

SecOnes::SecOnes()
{															 
	fore_color = madeRGB(255, 255, 0);
	size = 16;
	style = STYL_PLAIN | STYL_ITALIC;
	x = 167;
	y = 448;
}
SecTens::SecTens()
{															 
	fore_color = madeRGB(255, 255, 0);
	size = 16;
	style = STYL_PLAIN | STYL_ITALIC;
	x = 159;
	y = 448;
}
MinOnes::MinOnes()
{															 
	fore_color = madeRGB(255, 255, 0);
	size = 16;
	style = STYL_PLAIN | STYL_ITALIC;
	x = 148;
	y = 448;
}
MinTens::MinTens()
{															 
	fore_color = madeRGB(255, 255, 0);
	size = 16;
	style = STYL_PLAIN | STYL_ITALIC;
	x = 140;
	y = 448;
}

Elapsed::Elapsed()
{
	permList->addToFront(this);
}

Grid::Grid()
{
	view = 95299;

	if (gridnumber < 95200)
		loop = 2;
	else if (gridnumber < 95300)
		loop = 0;
	else 
		loop = 1;
	cel = curhud;
	init(335,389);
	setPri(5);
}

int Grid::doVerb (int theVerb)
{
	if (++curhud > 2)
		curhud = 0;
	setCel(curhud);		 
	return true;
}

ShootText::ShootText()
{
	view = 615;
	loop = 7;
	cel = 0;
	init(64,406);
	setPri(100);
	hide();				    
}

RightHP::RightHP()
{
	view = 623;
	loop = 0;
	cel = 5 - Hp;
	if (cel > 5)
		cel = 5;					  
	init(0,397);
	addToPic();		   
	dispose();
}
LeftHP::LeftHP()
{
	view = 623;
	loop = 1;
	cel = 5 - Hp;
	if (cel > 5)
		cel = 5;					  
	init(471,397);
	addToPic();
	dispose();											 
}

Rings::Rings()
{
	view = 619;
	loop = 2;
	cel = 0;
	init(330,388);
  	setCycle(new EndLoop(this, this));
	trap = new CursorTrap;
	setPri(100);
}
void Rings::doit(void)
{
	if (trap->sup)
	{
		if (!trap->onMe(mouse_x,mouse_y))
		{
			trap->sup = false;
			sfxMadeCursor();
		}
	}
	Prop::doit();
}

void Rings::cue(void)
{
	setCel(0);
  	setCycle(new EndLoop(this, this));
}

CursorTrap::CursorTrap()
{
	name = "cursortrap";
	sup = false;
 	setHotspot ( GUN_V );
	init ( 0,375,512,105);
}

void CursorTrap::respond(void)
{
	if (!sup)
	{
		sup = true;
		sfxSystemCursor(SYS_ARROW);		
	}
}

ShootBack::ShootBack(int backnum)
{
	view = backnum;
	loop = 0;
	cel = 0;
	init(0, 0);
	noSkip();
	addToPic();
	if (base < 95200)
	{
		new LeftHP;
		new RightHP;
	}
	dispose();
}

bea::bea(int theX, int theY)
{
	view = 95199;
	loop = 1;
	cel = 0;
	init(theX,theY);
	setScript(new (PulseScript));
}

bea::bea()
{
	view = 95199;
	loop = 1;
	cel = 0;
	if (base == 95100)
		init(381,412);
	else
	{
		switch (grendlPos)
		{
			case 95201:
				init(362,405);
				break;
			case 95203:
				init(344,412);
				break;
			case 95204:
				init(381,412);
				break;
			case 95206:
				init(362,420);
				break;
			case 95209:
				init(381,428);
				break;
		}
	}
	setScript(new (PulseScript));
}

me::me()
{
	view = 615;
	loop = 9;
	cel = 0;
	setScript(new (PulseScript));
}

eneme::eneme()
{
	view = 615;
	loop = 11;
	cel = 0;
	setScript(new (PulseScript));
}
Patrol::Patrol(int pos1,int pos2,int pos3,int pos4,int pos5,int pos6)
{
	int count = 0;
	int locs[6]	= {pos1,pos2,pos3,pos4,pos5,pos6};
	alive = true;
	
	for (count=0;count<6;count++)
	{
		switch (locs[count])
		{
			case 0:
				row[count] = 	0;
				column[count] =	2;
				break;
			case 1:
				row[count] = 	1;
				column[count] =	1;
				break;
			case 2:
				row[count] = 	1;
				column[count] =	3;
				break;
			case 3:
				row[count] = 	2;
				column[count] =	0;
				break;
			case 4:
				row[count] = 	2;
				column[count] =	2;
				break;
			case 5:
				row[count] = 	2;
				column[count] =	4;
				break;
			case 6:
				row[count] = 	3;
				column[count] =	1;
				break;
			case 7:
				row[count] = 	3;
				column[count] =	3;
				break;
			case 8:
				row[count] = 	4;
				column[count] =	0;
				break;
			case 9:
				row[count] = 	4;
				column[count] =	2;
				break;
			case 10:
				row[count] = 	4;
				column[count] =	4;
				break;
			case 11:
				row[count] = 	5;
				column[count] =	1;
				break;
			case 12:
				row[count] = 	5;
				column[count] =	3;
				break;
			case 13:
				row[count] = 	6;
				column[count] =	0;
				break;
			case 14:
				row[count] = 	6;
				column[count] =	2;
				break;
			case 15:
				row[count] = 	6;
				column[count] =	4;
				break;
			case 16:
				row[count] = 	7;
				column[count] =	1;
				break;
			case 17:
				row[count] = 	7;
				column[count] =	3;
				break;
			case 18:
				row[count] = 	8;
				column[count] =	2;
				break;
		}
	}
}

Arrow::Arrow(int start_loop,int start_x, int start_y,ExitFeature *thisexit)
{
	view = 615;
	loop = start_loop;
	cel = 1;
	exit = thisexit;
	init (start_x,start_y);
	hide();
}

void Arrow::doit(void)
{
	if (!colby.gunning && (flag & OBJECT_HIDDEN))
	{
		show();
	}
/*	else if (!colby.gunning && cel && !exit->onMe(mouse_x,mouse_y))
	{
		setCel(0);	
	}
	else if (!colby.gunning && !cel && exit->onMe(mouse_x,mouse_y))
	{
		setCel(1);	
	} */
	else if (colby.gunning && !(flag & OBJECT_HIDDEN))
	{
		hide();
	}
}

Gunman::Gunman(int theview, int thex, int they)
{
	name = "gunman";
	dead = false;
	view = theview;
	init(thex,they);
	scream = new Sound;
	scream->number = 0;
	bang = new Sound;
	bang->number = 95001;
	setPri(2);
	hide();
}

void Gunman::dispose(void)
{
 	bang->dispose();
	scream->dispose();
	View::dispose();
}

int Gunman::handleEvent ( MADEEventStamp *event )
{
  if (onMe(event))  
  {
  	if (event_type == USER_LEFT_DOWN)
    {
   		if (!dead && !theGun->mt)
   		{
			dead = true;
		}
    }
  }
  return false;
} 
void gogglesClean (void)
{
	int count = 0;
	ammo = theGun->shots;
	clips = theGun->clips;
	mt = theGun->mt;

	user->canInput(false);
	if (gridnumber < 95199)
	{
		psecOnes->erase();
		psecTens->erase();
		pminOnes->erase();
		pminTens->erase();
		seperator->erase();
		elapsed->setScript(NULL);
	}	
	for (count=0;count<19;count++)
	{
		if (enes[count])
		{
			enes[count]->dispose();
			enes[count] = NULL;
		}
	}
		b_ptr = NULL;

	if (g1)
		sfxPurgeRes (g1,RES_ATS);
	if (g2)
		sfxPurgeRes (g2,RES_ATS);
	if (g3)
		sfxPurgeRes (g3,RES_ATS);
	if (g4)
		sfxPurgeRes (g4,RES_ATS);
	if (h1)
		sfxPurgeRes (h1,RES_WAVE);
	if (h2)
		sfxPurgeRes (h2,RES_WAVE);
	if (f1)
		sfxPurgeRes (f1,RES_WAVE);
	if (f2)
		sfxPurgeRes (f2,RES_WAVE);
	
}

void gogglesStart (int row,int column)
{
	int count = 0;
	me * mark;
	memset(&enes,0,(19*sizeof(eneme*)));
	shooterRow = row;
	shooterColumn = column;
	sniped = false;
	user->input = false;
	colby.gunning = false;
	if ((base == 95100 && GameFlag.test(fGotBeacon)) || (Hp < 0 && !GameFlag.test(fStartCombat)))
	{
		if (Hp < 0)
			GameFlag.set(fGotKilled);
		endPuzzle();	
		return;
	}
	if (base == 95300 && GameFlag.test(fGotBeacon))
		new bea(381,428);
	if (GameFlag.test(fStartCombat))
	{
		number =  95098;
		countrooms = 0;
		shooterpos = 0;
		mt = false;
		ammo = 16;
		curhud = 0;

		if (base < 95200)
			clips = 1;
		else /*if (!(base == 95200 && GameFlag.test(fRefineryCombat)))*/
			clips = 2;

		if (base > 95100)
		{
			Hp = 2;
			if (!GameFlag.test(fGotBeacon))	
			{
				grendlPos = 95209;
				shooterpos = 0;
				memset(&dude,0,sizeof(dude));

				dude[0] = new Patrol( 3, 1, 0, 2, 0, 1);
				E_map[dude[0]->row[shooterpos]][dude[0]->column[shooterpos]] = 2;

				dude[1] = new Patrol( 5,10,15,17,15,10);
				E_map[dude[1]->row[shooterpos]][dude[1]->column[shooterpos]] = 2;

				dude[2] = new Patrol( 8,13,16,18,16,13);
				E_map[dude[2]->row[shooterpos]][dude[2]->column[shooterpos]] = 2;
				
				if (base > 95200)
				{
					dude[3] = new Patrol( 1, 3, 1, 1, 1, 3);
					E_map[dude[3]->row[shooterpos]][dude[3]->column[shooterpos]] = 2;

					dude[4] = new Patrol( 10,15,10,10,10,15);
					E_map[dude[4]->row[shooterpos]][dude[4]->column[shooterpos]] = 2;

					dude[5] = new Patrol( 16,16,13,16,13,16);
					E_map[dude[5]->row[shooterpos]][dude[5]->column[shooterpos]] = 2;
				}
		
			}
			else
			{
				for (count = 0;count<6;count++)
				{
					if (dude[count])
					{	
						if (dude[count]->alive)
						{
							E_map[dude[count]->row[shooterpos]][dude[count]->column[shooterpos]] = 2;
						}
					}
				}
				P_map [2][0] = 2;
				P_map [2][4] = 2;
				P_map [6][4] = 2;
				P_map [7][1] = 2;
			}
		}
		else
			Hp = 5;

	}
	sfxSystemCursor(SYS_WAIT);
//    new LeftHP;
//    new RightHP;
	theGun = new(Gun);
	theGun->clip->setCel(ammo-1);
	theGun->mags->setCel(clips);
	theGun->shots = ammo;
	theGun->clips = clips;
	theGun->mt = mt;

	warning = new ShootText;
	warning->hide();

	grid = new (Grid);
	new (Rings);

	mark = new(me);
	
	if (column > 2) 
    	mark->init(coords[gridnumber - base].x - 2,coords[gridnumber-base].y - 2);
    else if	(column < 2)
	    mark->init(coords[gridnumber - base].x + 2,coords[gridnumber-base].y - 2);
	else
    	mark->init(coords[gridnumber - base].x,coords[gridnumber-base].y - 2);

	mark->setPri(99);

	if (base == 95100 && !(GameFlag.test(fGotBeacon)) || (grendlPos && base == 95200))
	 	b_ptr = new bea;

	clearShot();	

	if (countrooms > 0)
		moveEnemy(row,column);
	else
		countrooms++;

	if (elapsed)
	{
		psecOnes->init(itoa(secOnes,(char*)&psecOnes->buff,10));
		psecTens->init(itoa(secTens,(char*)&psecTens->buff,10));
		pminOnes->init(itoa(minOnes,(char*)&pminOnes->buff,10));
		pminTens->init(itoa(minTens,(char*)&pminTens->buff,10));
		seperator->init(":");
	}
	switch(base)
	{
		case 95000:
			new bea(380,390);
			break;
/*		case 95300:
			new bea(343,407);
			new bea(417,407);
			new bea(343,449);
			new bea(417,449);
			break;
*/	}
	if (GameFlag.test(fStartCombat))
	{
		secOnes = 0;
		secTens = 0;
		minOnes = 5;
		minTens = 0;

		sfxLoadRes(95001, RES_WAVE);
		sfxLockRes(95001, RES_WAVE);
		if (base < 95200)
		{
			sfxLoadRes(95003, RES_WAVE);
			sfxLockRes(95003, RES_WAVE);
		}
		else if (base == 95300)
		{
			sfxLoadRes(95034, RES_WAVE);
			sfxLockRes(95034, RES_WAVE);
		}
		sfxLoadRes(906, RES_ATS);
		sfxLockRes(906, RES_ATS);
		sfxLoadRes(902, RES_ATS);
		sfxLockRes(902, RES_ATS);
	}	

	baddies = &E_map[row][column];
	posted = &P_map[row][column];

		if (g1)
			sfxLoadRes(g1, RES_ATS);
		if (g2)
			sfxLoadRes(g1, RES_ATS);
		if (g3)
			sfxLoadRes(g1, RES_ATS);
		if (g4)
			sfxLoadRes(g1, RES_ATS);

		if (f1)
			sfxLoadRes(f1, RES_WAVE);
		if (f2)
			sfxLoadRes(f2, RES_WAVE);
		if (h1)
			sfxLoadRes(h1, RES_WAVE);
		if (h2)
			sfxLoadRes(h2, RES_WAVE);

	if (((*baddies && *baddies /*!=*/< NOGO)||( *posted && *baddies < NOGO)) && (g1 || g2 || g3 || g4))
	{
		*baddies += *posted;
		*posted = 0;

		while (*baddies >100 && *baddies < NOGO)
			*baddies -= 100;					 
		
		colby.gunning = true;

		warning->show();
		warning->setLoop(7);

		memset (&Gman,0,(4 * (sizeof(Gunman*))));

		if (!GameFlag.test(fStartCombat) || (base == 95200 && GameFlag.test(fRefineryCombat)))
		{
			if (*baddies > 2)
				*baddies = 2;
			if (base > 95100)
			{
				for (count=0; count <6;count++)
				{
					if (dude[count])
					{
						if (dude[count]->row[shooterpos] == shooterRow && dude[count]->column[shooterpos] == shooterColumn)
							dude[count]->alive = false;
					}
				}
			}
			else
			{
				if (instructions->number > 0)
					instructions->stop();

				instructions->number = 95055;
				instructions->playBuffered(95055,NULL);
			}
			theGame->resetHotspot();
			curRoom->setScript(new (ManagerScript));
		}	
		if (base == 95200)
			GameFlag.set(fRefineryCombat);
	}
	else
	{
		if (!GameFlag.test(fStartCombat) || (base == 95200 && GameFlag.test(fRefineryCombat))/* || base > 95100*/)
		{
			if (!(g1 || g2 || g3 || g4))
				*baddies = NOGO;
			theGame->resetHotspot();
			curRoom->setScript(new (WaitingScript));
		}
	}

	sfxMadeCursor();		
	user->canInput(true);
	flushMessageCue();
	if (elapsed && !GameFlag.test(fStartCombat))
		elapsed->setScript(new TimerScript);
}

void clearShot (void)
{
	int count_r = 0;
	int count_c = 0;
	int count = 0;
	int* targets;
	int temp = 0;
	int index = 0;
	int chance = 0;

	temp = 0;
	
	for (count_r = 0; count_r < 9; count_r++)
	{
		for (count_c = 0; count_c < 5; count_c++ )
		{
			index = gridToHex[count_r][count_c];
			if(E_map[count_r][count_c] && E_map[count_r][count_c] != NOGO)
			{
				targets = &T_list[index][0];

				if (!enes[index])
				{
					enes[index] = new (eneme);
					enes[index]->init(enePos[index].x,enePos[index].y);
					enes[index]->setPri(50);
				}

				while(*targets)
				{				
					if (*targets == gridnumber - base)
					{
						temp ++;
						if (E_map[count_r][count_c] < 100)
							E_map[count_r][count_c] += 100;
						enes[index]->setLoop(10);
						break;
					}
					targets++;
				}
			}

			if(P_map[count_r][count_c])
			{
				index = gridToHex[count_r][count_c];
				if (!enes[index])
				{
					enes[index] = new (eneme);
					enes[index]->init(enePos[index].x,enePos[index].y);
					enes[index]->setPri(50);
				}
				targets = &T_list[index][0];
				while(*targets)
				{				
					if (*targets == gridnumber - base)
					{
						temp ++;
						enes[index]->setLoop(10);
					}
					targets++;
				}
			}

		}
	}

	if (temp && !GameFlag.test(fStartCombat))
	{
		warning->show();
		warning->setLoop(8);
		for (count=0;count<temp;count++)
		{
			chance = (rand() & 5);
			if(!chance)
			{
				sniped = true;
//				if (base != 95200)
//				{
					if (instructions->number < 0)
					{
						instructions->stop();
						instructions->number = 95053;
						instructions->playBuffered(95053,NULL);
					}
/*				
					if (firstsniped && base < 95200)  NEED NEW CD
					{
						instructions->number = 95111;
						instructions->playBuffered(95111,NULL);
						firstsniped = false;
					}
				}
				else
				{
					if (instructions->number < 0)
					{
						instructions->stop();
						instructions->number = 95302;
						instructions->playBuffered(95302,NULL);
					}
				}
*/				return;
			}
		}
	}
	temp = 0;
	return;
}												 


void moveEnemy (int row, int column)
{
	static int doubletime = 0;
	int count_r = 0;
	int count_c = 0;
	int count = 0;
	int temp_map [9][5];
	int cur_loc;
	int temp = 0;
	int index = 0;
	int direction = 0;
	int temppos = 0;
	int go = false;
	int anyone = false;
	memset (&temp_map,0,(45 * (sizeof(int))));
	countrooms = 0;
	if (!GameFlag.test(fStartCombat) && !colby.gunning)
	{
		if (base > 95100)
		{
			memset(&E_map,0,(45 * (sizeof(int))));
			temppos = shooterpos;
			if (++shooterpos > 5)
				shooterpos = 0;

			for (count = 0;count<6;count++)
			{
				if (dude[count])
				{	
					if (dude[count]->alive)
					{
						if (dude[count]->row[temppos] == row && dude[count]->column[temppos] == column)
						{
							E_map[dude[count]->row[temppos]][dude[count]->column[temppos]] = 2;
							dude[count]->alive = false;
						}
						else
							E_map[dude[count]->row[shooterpos]][dude[count]->column[shooterpos]] = 2;

						anyone = true;
					}
				}
			}

			if (base == 95300 && !anyone && !GameFlag.test(fGotBeacon))
			{
				GameFlag.set(fGotBeacon);						
				if (row != 4 && column != 2)
					new bea(381,428);
			}			
	
			if (base == 95200 && GameFlag.test(fRefineryCombat))
			{
				switch (grendlPos)
				{
					case 95209:
						grendlPos = 95204;
						break;
	 				case 95204:
						grendlPos = 95206;
						break;
					case 95206:
						grendlPos = 95201;
						break;
					case 95201:
						grendlPos = 95203;
						break;
					case 95203:
						grendlPos = 0;
						break;
				}				
				if (b_ptr)
					b_ptr->dispose();
				if (grendlPos)
					b_ptr = new bea;
				else 
					b_ptr = NULL;
			}
		}
		else
		{
			if (addshooters)
			{
				doubletime++;

				if (!(doubletime%3)) 
				{
					temp = (rand() & 2);
					switch (temp)
					{
						case 0:
							E_map[0][2] += 1;
							break;
						case 1:
							E_map[1][1] += 1;
							break;
						case 2:
							E_map[1][3] += 1;
							break;
					}
				}
			}
			for (count_r = 0; count_r < 9; count_r++)
			{
				for (count_c = 0; count_c < 5; count_c++ )
				{
					direction = 0;
					cur_loc = E_map[count_r][count_c];

					if (cur_loc != NOGO)
					{
						temp_map[count_r][count_c] -= cur_loc;

						if (cur_loc > 100)
						{
							while (cur_loc > 100)
								cur_loc -= 100;					 

							temp_map[count_r][count_c] += 1;
						}
					}
					else
					{
						temp_map[count_r][count_c] = 0;	
					}	

					if (cur_loc != NOGO)
					{
						if (count_r < row)
						{
							direction = SHOOTER_LEFT;						
						}
						else if	(count_r >row)
						{
							direction = SHOOTER_RIGHT;
						}
						else
							direction = 0;
											
						if (count_c < column)
						{
							if (direction)
							{
								if (direction == SHOOTER_LEFT)
									direction = SHOOTER_BELOW_LEFT;
								else						
									direction = SHOOTER_BELOW_RIGHT;
							}
							else
								direction = SHOOTER_BELOW;						
						}
						else if	(count_c > column)
						{
							if (direction)
							{
								if (direction == SHOOTER_LEFT)
									direction = SHOOTER_ABOVE_LEFT;
								else						
									direction = SHOOTER_ABOVE_RIGHT;
							}
							else
								direction = SHOOTER_BELOW;						
						}
					
						if (direction)
						{
							go = false;
							for (count = 0;count < 4;count ++)
							{
								switch (move[direction][count])
								{
									case TRY_UP:
										if (count_r)
										{
											if (E_map[count_r - 1][count_c] != NOGO && gridToHex[count_r - 1][count_c] != -1 &&
												(E_map[count_r - 1][count_c] < 2 || (E_map[count_r - 1][count_c] < 102 && E_map[count_r - 1][count_c] >= 100)) &&
												(temp_map[count_r - 1][count_c] < 2 || (temp_map[count_r - 1][count_c] < 102 && temp_map[count_r - 1][count_c] >= 100)))
											{										
												temp_map[count_r - 1][count_c] += cur_loc;

												if (temp_map[count_r - 1][count_c] > 2)
												{
													temp_map[count_r][count_c] += (cur_loc - 1);											
													temp_map[count_r - 1][count_c] = 2;											
												}
												go = true;
											}	
										}
										break;
									case TRY_UP_RIGHT:
										if (count_r)
										{
											if (count_c < 4)
											{
												if (E_map[count_r - 1][count_c + 1] != NOGO && gridToHex[count_r - 1][count_c + 1] != -1 &&
													(E_map[count_r - 1][count_c + 1] < 2 || (E_map[count_r - 1][count_c + 1] < 102 && E_map[count_r - 1][count_c + 1] >= 100)) &&
													(temp_map[count_r - 1][count_c + 1] < 2 || (temp_map[count_r - 1][count_c + 1] < 102 && temp_map[count_r - 1][count_c + 1] >= 100)))
												{										
													temp_map[count_r - 1][count_c + 1] += cur_loc;
													if (temp_map[count_r - 1][count_c + 1] > 2)
													{
														temp_map[count_r][count_c] += (cur_loc - 1);											
														temp_map[count_r - 1][count_c + 1] = 2;											
													}
													go = true;
												}	
											}	
										}
										break;
									case TRY_UP_LEFT:
										if (count_r)
										{
											if (count_c)
											{
												if (E_map[count_r - 1][count_c - 1] != NOGO && gridToHex[count_r - 1][count_c - 1] != -1 &&
													(E_map[count_r - 1][count_c - 1] < 2 || (E_map[count_r - 1][count_c - 1] < 102 && E_map[count_r - 1][count_c - 1] >= 100)) &&
													(temp_map[count_r - 1][count_c - 1] < 2 || (temp_map[count_r - 1][count_c - 1] < 102 && temp_map[count_r - 1][count_c - 1] >= 100)))
												{										
													temp_map[count_r - 1][count_c - 1] += cur_loc;
													if (temp_map[count_r - 1][count_c - 1] > 2)
													{
														temp_map[count_r][count_c] += (cur_loc - 1);											
														temp_map[count_r - 1][count_c - 1] = 2;											
													}
													go = true;
												}	
											}	
										}
										break;
									case TRY_DOWN_LEFT:
										if (count_r<9)
										{
											if (count_c)
											{
												if (E_map[count_r + 1][count_c - 1] != NOGO && gridToHex[count_r + 1][count_c - 1] != -1 &&
													(E_map[count_r + 1][count_c - 1] < 2 || (E_map[count_r + 1][count_c - 1] < 102 && E_map[count_r + 1][count_c - 1] >= 100)) &&
													(temp_map[count_r + 1][count_c - 1] < 2 || (temp_map[count_r + 1][count_c - 1] < 102 && temp_map[count_r + 1][count_c - 1] >= 100)))
												{										
													temp_map[count_r + 1][count_c - 1] += cur_loc;
													if (temp_map[count_r + 1][count_c - 1] > 2)
													{
														temp_map[count_r][count_c] += (cur_loc - 1);											
														temp_map[count_r + 1][count_c - 1] = 2;
													}
													go = true;
												}	
											}	
										}
										break;
									case TRY_DOWN_RIGHT:
										if (count_r<9)
										{
											if (count_c<5)
											{
												if (E_map[count_r + 1][count_c + 1] != NOGO && gridToHex[count_r + 1][count_c + 1] != -1 &&
													(E_map[count_r + 1][count_c + 1] < 2 || (E_map[count_r + 1][count_c + 1] < 102 && E_map[count_r + 1][count_c + 1] >= 100)) &&
													(temp_map[count_r + 1][count_c + 1] < 2 || (temp_map[count_r + 1][count_c + 1] < 102 && temp_map[count_r + 1][count_c + 1] >= 100)))
												{										
													temp_map[count_r + 1][count_c + 1] += cur_loc;
													if (temp_map[count_r + 1][count_c + 1] > 2)
													{
														temp_map[count_r][count_c] += (cur_loc - 1);											
														temp_map[count_r + 1][count_c + 1] = 2;
													}	
													go = true;
												}	
											}	
										}
										break;
									case TRY_DOWN:
										if (count_r<9)
										{
											if (E_map[count_r + 1][count_c] != NOGO && gridToHex[count_r + 1][count_c] != -1 &&
												(E_map[count_r + 1][count_c] < 2 || (E_map[count_r + 1][count_c] < 102 && E_map[count_r + 1][count_c] >= 100)) &&
												(temp_map[count_r + 1][count_c] < 2 || (temp_map[count_r + 1][count_c] < 102 && temp_map[count_r + 1][count_c] >= 100)))
											{										
												temp_map[count_r + 1][count_c] += cur_loc;
												if (temp_map[count_r + 1][count_c] > 2)
												{
													temp_map[count_r][count_c] += (cur_loc - 1);											
													temp_map[count_r + 1][count_c] = 2;											
												}
												go = true;
											}	
										}
										break;
									default:
										break;
								}
								if (go)
									break;
							}
						}
					}
				}
			}
		}
		for (count_r = 0; count_r < 9; count_r++)
		{
			for (count_c = 0; count_c < 5; count_c++ )
			{
				E_map[count_r][count_c] += temp_map[count_r][count_c];

				index = gridToHex[count_r][count_c];

				if (E_map[count_r][count_c] && E_map[count_r][count_c] < /*!=*/ NOGO)
				{
					if(!enes[index])
					{
						enes[index] = new (eneme);
						enes[index]->init(enePos[index].x,enePos[index].y);
		 				enes[index]->setPri(50);
					}
				}
				else if (index >= 0)
				{
					if (enes[index] && !P_map[count_r][count_c])
					{
						enes[index]->dispose();
						enes[index] = NULL;
					}	
				}
			}
		}
	}
}

void GunmanScript::changeState ( int newState )
{
	int temp = 0;

	switchTo
		while (temp < 1)
			temp = (rand() & 5);

		seconds = temp;
	END

	BEG
	  ((Gunman *)client)->show();
	  ((Gunman *)client)->setCycle(new EndLoop(((Gunman *)client), this));
	END

    BEG
	  if (((Gunman *)client)->dead)
	  {
    	  ((Gunman *)client)->setScript(new(DieScript));	
	  }
	  else
	  {
		  ((Gunman *)client)->setLoop (1);	
		  ((Gunman *)client)->setCycle(new EndLoop(((Gunman *)client), this));
//		  ((Gunman *)client)->bang->play();						  	
		  sfxPlaySound ( 95001, 100, 64, 64, NULL );
	  }	
    END

    BEG
	  if (((Gunman *)client)->dead)
	  {
    	  ((Gunman *)client)->setScript(new(DieScript));	
	  }
	  else
	  {	
		  if (base < 95200)
		  	sfxPlaySound ( 95003, 100, 64, 64, NULL);
//		  else/* if (base == 95300)*/
//			sfxPlaySound ( 95034, 100, 64, 64, NULL);
/*		  else if(instructions->number < 0)
		  {
			if (base == 95200)
			{
				instructions->number = 95202;
				instructions->playBuffered(95202,NULL);
			}
			else
			{
				instructions->number = 95306;
				instructions->playBuffered(95306,NULL);
			}
	  	  }
*/		  if (--Hp < 0)
		  {
			GameFlag.set(fGotKilled);
     	    ((Gunman *)client)->setScript(NULL);	
			gogglesClean();
			endPuzzle();	
		  }
		  else
		  {
			  if (base < 95200)
			  {
				  new LeftHP;
				  new RightHP;
			  }	
			  ((Gunman *)client)->setLoop (1);	
			  ((Gunman *)client)->setCycle(new EndLoop(((Gunman *)client), this));
//			  ((Gunman *)client)->bang->play();						  	
			  sfxPlaySound ( 95001, 100, 64, 64, NULL );
		  }	
	  } 	
    END

    BEG
	  if (((Gunman *)client)->dead)
	  {
    	  ((Gunman *)client)->setScript(new(DieScript));	
	  }
	  else
	  {	
//          if (base < 95200)
			sfxPlaySound ( 95003, 100, 64, 64, NULL);
/*		  else if (base == 95200 && instructions->number < 0)
		  {
			if (base == 95200)
			{
				instructions->number = 95201;
				instructions->playBuffered(95201,NULL);
			}
			else
			{
				instructions->number = 95304;
				instructions->playBuffered(95304,NULL);
			}
		  }
*/		  if (--Hp < 0)
		  {
			GameFlag.set(fGotKilled);
     	    ((Gunman *)client)->setScript(NULL);	
			gogglesClean();
			endPuzzle();	
		  }
		  else
		  {
			  if (base < 95200)
			  {
				  new LeftHP;
				  new RightHP;
			  }	
			  ((Gunman *)client)->setLoop(2);	
			  ((Gunman *)client)->setCycle(new EndLoop(((Gunman *)client), this));
		  }	
	  } 	
    END

	BEG
	  ((Gunman *)client)->hide();
	  ((Gunman *)client)->setScript(NULL);	
	END
}

void DieScript::changeState ( int newState )
{
	int temp = 0;
	switchTo
	  temp = (rand() & 8);
	  if (temp > 3 || !h2)
		  ((Gunman *)client)->scream->number = h1;	
	  else
		  ((Gunman *)client)->scream->number = h2;	
	  ticks = 2;								  
    END

	BEG
	  ((Gunman *)client)->scream->play();	
	  ((Gunman *)client)->setLoop(3);	
	  ((Gunman *)client)->setCycle(new EndLoop(((Gunman *)client), this));
    END

	BEG
	  ((Gunman *)client)->scream->stop();		  	
	  temp = (rand() & 8);

	  if (temp > 3 || !f2)
	  	((Gunman *)client)->scream->number = f1;	
	  else
	  	((Gunman *)client)->scream->number = f2;	

	  ((Gunman *)client)->hide();

      ((Gunman *)client)->scream->play(this);
		if (base < 95200)
		{
			if (instructions->number > 0)
				instructions->stop();
			instructions->number = 95056;
			instructions->playBuffered(95056,NULL);
		}
    END

	BEG
	  --(*baddies);
	  ((Gunman *)client)->setScript(NULL);	
	END
}

void DieScript2::changeState ( int newState )
{
	switchTo
	  ticks = 1;
    END

	BEG
	  ((Gunman *)client)->scream->play();	
	  ((Gunman *)client)->setLoop(4);	
	  ((Gunman *)client)->setCycle(new EndLoop(((Gunman *)client), this));
		if (base < 95200)
		{
			if (instructions->number > 0)
				instructions->stop();
			instructions->number = 95056;
			instructions->playBuffered(95056,NULL);
		}
    END
	BEG
	  --(*baddies);
	  ((Gunman *)client)->setScript(NULL);	
    END
}

void LilGunmanScript::changeState ( int newState )
{
	int temp = 0;

	switchTo
		while (temp < 1)
			temp = (rand() & 5);

		seconds = temp;
	END

	BEG
	  ((Gunman *)client)->show();
	  ((Gunman *)client)->setCycle(new EndLoop(((Gunman *)client), this));
	END

    BEG
//	  ((Gunman *)client)->bang->play();						  	
	  sfxPlaySound ( 95001, 100, 64, 64, NULL );

	  if (((Gunman *)client)->dead)
	  {
    	  ((Gunman *)client)->setScript(new(LilDieScript));	
	  }
	  else
	  {
		  if (base < 95200)
		  	sfxPlaySound ( 95003, 100, 64, 64, NULL);
/*		  else if (instructions->number < 0)
		  {
			if (base == 95200)
			{
				instructions->number = 95201;
				instructions->playBuffered(95201,NULL);
			}
			else
			{
				instructions->number = 95304;
				instructions->playBuffered(95304,NULL);
			}
		  }
*/		  if (--Hp < 0)
		  {
			GameFlag.set(fGotKilled);
     	    ((Gunman *)client)->setScript(NULL);	
			gogglesClean();
			endPuzzle();	
		  }
		  else
		  {
			  if (base < 95200)
			  {
				  new LeftHP;
				  new RightHP;
			  }	
			  ((Gunman *)client)->setLoop (1);	
			  ((Gunman *)client)->setCycle(new EndLoop(((Gunman *)client), this));
		  }	
	  }	
    END

	BEG
	  ((Gunman *)client)->hide();
	  ((Gunman *)client)->setScript(NULL);	
	END
}

void LilDieScript::changeState ( int newState )
{
	int temp = 0;
	switchTo
	  temp = (rand() & 8);
  	  if (temp > 3 || !h2)
		  ((Gunman *)client)->scream->number = h1;	
	  else
		  ((Gunman *)client)->scream->number = h2;	
	
	  ticks = 2;								  
    END

	BEG
	  ((Gunman *)client)->scream->play();	
	  ((Gunman *)client)->setLoop(2);	
	  ((Gunman *)client)->setCycle(new EndLoop(((Gunman *)client), this));
    END

	BEG
	  ((Gunman *)client)->scream->stop();		  	

	  temp = (rand() & 8);

	  if (temp > 3 || !f2)
	  	((Gunman *)client)->scream->number = f1;	
	  else
	  	((Gunman *)client)->scream->number = f2;	
	
	  ((Gunman *)client)->hide();
      ((Gunman *)client)->scream->play(this);
		if (base < 95200)
		{
			if (instructions->number > 0)
				instructions->stop();
			instructions->number = 95056;
			instructions->playBuffered(95056,NULL);
		}
    END

	BEG
	  --(*baddies);
	  ((Gunman *)client)->setScript(NULL);	
	END
}

void ManagerScript::changeState ( int newState )
{
	static int flipflop = 0;
	int count = 0;
	int a = 0;
	seconds = 0;
	colby.gunning = true;

	switchTo
		if (grendlPos == gridnumber)
		{
			GameFlag.set(fGotBeacon);
			gogglesClean();
			endPuzzle();
			return;
		}
		for (count = 0;count <4;count ++)
		{
			if (Gman[count])
			{
				a++;
			}	
		}
		if (!g1 && !g2 && !g3 && !g4)
			*baddies = 0;
		if (*baddies > 2)
			*baddies = 2;
		if (*baddies < 0)
			*baddies = 0;
		if (*baddies > 1 && !g2)
			*baddies = 1;
		if (*baddies && !a )
		{
//	       	count = (rand() & 3);
			count = flipflop%2;
			if (!Gman[count])
			{
				switch (count)
				{
					case 0:
						if (g1)
			    		{
//			    			sfxPrintf("G1= %d",g1);
			    			Gman[count] = new Gunman(g1,0,0);	
				        }	
				        break;
					case 1:
						if (g2)
			    		{
//			    			sfxPrintf("G2= %d",g2);
			    			Gman[count] = new Gunman(g2,0,0);	
						}
					    break;
					case 2:
						if (g3)
			    		{
//			    			sfxPrintf("G3= %d",g3);
			    			Gman[count] = new Gunman(g3,0,0);	
				        }
				        break;
					case 3:
						if (g4)
			    		{
//			    			sfxPrintf("G4= %d",g4);
			    			Gman[count] = new Gunman(g4,0,0);	
						}
					    break;
				}
				flipflop ++;
				
				if (Gman[count])
				{
					Gman[count]->cycleSpeed = 4;	
					Gman[count]->setPri(1);	
					if (Gman[count]->myATS->numReels > 3)
						Gman[count]->setScript(new (GunmanScript));
					else
						Gman[count]->setScript(new (LilGunmanScript));
				}
			}	
		}
		else if (*baddies < 1)
		{
			for (count = 0;count < 4; count ++)
			{												   
				if (Gman[count] && !Gman[count]->script)	
				{
					Gman[count]->dispose();
					Gman[count] = NULL;
				}
			}

			colby.gunning = false;
			warning->hide();

			theGame->resetHotspot();
			curRoom->setScript(new (WaitingScript));
			return;
		}								 
		ticks = 1;
    END

	BEG
		for (count = 0;count < 4; count ++)
		{												   
			if (Gman[count])						  
			{
				if(!Gman[count]->script)	
				{
					Gman[count]->dispose();
					Gman[count] = NULL;
				}
			}
		}
		ticks = 1;
	END

	BEG
		curRoom->setScript(new (ManagerScript));
	END
}

void TimerScript::changeState ( int newState )
{
	switchTo
		seconds = 1;
	END

	BEG
		psecOnes->erase();
		psecTens->erase();
		pminOnes->erase();
		pminTens->erase();

		if (secOnes < 1)
		{
			secOnes = 9;
			if (secTens < 1)
			{
				secTens = 5;
				if (minOnes < 1)
				{
					minOnes = 0;
					secTens = 0;
					secOnes = 0;
				}
				else
				{
					minOnes --;
				}
			}	
			else
			{
				secTens --;
			}
		}
		else
		{
			secOnes--;
		} 	

		psecOnes->init(itoa(secOnes,(char*)&psecOnes->buff,10));
		psecTens->init(itoa(secTens,(char*)&psecTens->buff,10));
		pminOnes->init(itoa(minOnes,(char*)&pminOnes->buff,10));
		pminTens->init(itoa(minTens,(char*)&pminTens->buff,10));

		if (!secOnes && ! secTens && !minOnes)
		{
			GameFlag.set(fGotKilled);
			client->setScript(NULL);		
			curRoom->setScript(new EndShooterScript);
		}	
		else
			client->setScript(new TimerScript);		
	END
}

void endPuzzle(void)
{
	int count = 0;
	colby.gunning = false;
	user->canInput(true);
	sfxUnlockRes(95001, RES_WAVE);

	if (base < 95200)
		sfxUnlockRes(95003, RES_WAVE);
	else if (base == 95300)
		sfxUnlockRes(95034, RES_WAVE);

	sfxUnlockRes(906, RES_ATS);
	sfxUnlockRes(902, RES_ATS);
	sfxUnlockRes(623, RES_ATS);
	intrface->enable();
	addshooters = true;
	instructions->stop();
	theMusic->setVolume(80);
	theMusic->stop();
	if (base > 95100)
	{
		if (base < 95200 || GameFlag.test(fGotKilled))
		{
			for (count=0;count<6;count++)
			{
				if (dude[count])
					delete dude[count]; 		
				dude[count] = NULL;
			}	
		}
	}		
	if (elapsed)
	{
		psecOnes->dispose();
		psecTens->dispose();
		pminOnes->dispose();
		pminTens->dispose();
		seperator->dispose();
	}

	if (warning)
		warning=NULL;

	curRoom->setScript(NULL);
	
	normalCursor = arrowCursor;

	if (elapsed)
	{			
		elapsed->setScript(NULL);
		permList->release(elapsed);
		deathList->add(elapsed);
		elapsed = NULL;
	}

	if (gridnumber < 95100 && minOnes < 3)
	{
		GameFlag.set(fWPSqueekBy);
		theGame->newRoom(new Rm7000);
	}
	else if (gridnumber < 95200)
	{
		theGame->newRoom(new Rm7000);
	}
	else if (gridnumber < 95300)
	{
		GameFlag.set(fStartCombat);

		if(GameFlag.test(fGotKilled))
			theGame->newRoom(new Rm60000(END_BLACKDEAD));
		else if (GameFlag.test(fRefineryCombat))
		{
			if (GameFlag.test(fGotBeacon) && grendlPos)
			{
				theGame->newRoom(new Rm93500);
				return;
			}
			else if (GameFlag.test(fGotBeacon) && !grendlPos)
			{
				GameFlag.set(fEndgamePitStopped);
				new VLink( "You've saved us all: DCI Sterling, Peter R.","1077.avi", 0, 0);
					//vlink congrats
				new VLink( "Red line communication: Oval Office", "1078.avi", 0, 0);
					//vlink congrats
			}
			else
			{
				new EMail("Can't win 'em all: Holt, David M.",98206);
			}

			for (count=0;count<6;count++)
			{
				if (dude[count])
					delete dude[count]; 		
				dude[count] = NULL;
			}	
			GameFlag.clear(fHeidelbergAvailable);				
			curMap = MAP_HEIDELBERG;
			theGame->newRoom(new Rm1000);

		}
		else if (GameFlag.test(fGotBeacon))
		{	
			GameFlag.set(fRefineryCombat);
			theGame->newRoom(new Rm93500);
		}
		else 
			theGame->newRoom(new Rm1000);
	}
	else
	{
		GameFlag.set(fStartCombat);
		if(GameFlag.test(fGotKilled))
			theGame->newRoom(new Rm60000(END_BLACKDEAD));
		else
			theGame->newRoom(new Rm93000);
	}
}
void EndShooterScript::changeState ( int newState )
{
	switchTo
	  ticks = 1;
    END
	BEG
      gogglesClean();
	  endPuzzle();
    END
}
void PulseScript::changeState ( int newState )
{
	switchTo
   	  ((View *)client)->setCel(0);	
	  ticks = 15;
    END

	BEG
   	  ((View *)client)->setCel(1);	
	  ticks = 15;
	END
	
	BEG
	  ((View *)client)->setScript(new (PulseScript));	   	  	
	END
}
void WaitingScript::changeState ( int newState )
{
	int count = 0;
	int anyone = 0;
	colby.gunning = false;

	switchTo
		for (count = 0;count<6;count++)
		{
			if (dude[count])
				if (dude[count]->alive)
					anyone = true;
		}
		if (!anyone && base == 95300 && !GameFlag.test(fGotBeacon))
		{
			GameFlag.set(fGotBeacon);
			new bea(381,428);
		}
		if (gridnumber == 95309 && GameFlag.test(fGotBeacon))
		{
			gogglesClean();
			endPuzzle();
		}
		else if (grendlPos == gridnumber)
		{
			GameFlag.set(fGotBeacon);
			gogglesClean();
			endPuzzle();
		}
		else
			seconds = 3;
	END

	BEG
		if (sniped)
		{
			if (--Hp < 0)
			{
				GameFlag.set(fGotKilled);
				if (base < 95200)
					sfxPlaySound ( 95003, 100, 64, 64, NULL);
				else /*if (base == 95300)*/
					sfxPlaySound ( 95034, 100, 64, 64, NULL);
/*				else if (instructions->number < 0)
				{
					if (base == 95200)
					{
						instructions->number = 95201;
						instructions->playBuffered(95201,NULL);
					}
					else
					{
						instructions->number = 95304;
						instructions->playBuffered(95304,NULL);
					}
				}
*/				seconds = 2;
				return;
			}	
			else
			{
				if (base < 95200)
					sfxPlaySound ( 95003, 100, 64, 64, NULL);
				else /*if (base == 95300)*/
					sfxPlaySound ( 95034, 100, 64, 64, NULL);
/*				else if (instructions->number < 0)
				{
					if (base == 95200)
					{
						instructions->number = 95202;
						instructions->playBuffered(95202,NULL);
					}
					else
					{
						instructions->number = 95306;
						instructions->playBuffered(95306,NULL);
					}
				}
*/			  if (base < 95200)
			  {
				  new LeftHP;
				  new RightHP;
			  }	
			}
		}
		moveEnemy(shooterRow,shooterColumn);
		baddies = &E_map[shooterRow][shooterColumn];
		
		while(*baddies > 100 && *baddies </*!=*/ NOGO)
			*baddies -=100;

		if (*baddies && *baddies /*!=*/ < NOGO)
		{
			colby.gunning = true;
			if (*baddies > 2)
				*baddies = 2;
			warning->show();
			warning->setLoop(7);

			memset (&Gman,0,(4 * (sizeof(Gunman*))));
			if (base > 95100)
			{
				for (count=0; count <6;count++)
				{
					if (dude[count])
					{
						if (dude[count]->row[shooterpos] == shooterRow && dude[count]->column[shooterpos] == shooterColumn)
							dude[count]->alive = false;
					}
				}
				if (base == 95200)
					GameFlag.set(fRefineryCombat);
			}
			else
			{
				if (instructions->number > 0)
					instructions->stop();

				instructions->number = 95055;
				instructions->playBuffered(95055,NULL);
			}
			theGame->resetHotspot();
			curRoom->setScript(new (ManagerScript));
		}
		else
		{
			clearShot();	
			curRoom->setScript(new (WaitingScript));
		}
	END
	BEG
		gogglesClean();
		endPuzzle();	
	END
}

} // namespace Spycraft
