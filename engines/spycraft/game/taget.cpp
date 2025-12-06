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

#include "spycraft/game/target.h"
#include "spycraft/logic/60000.h"
#include "spycraft/logic/1000.h"
#include "spycraft/game/flag.h"
#include "spycraft/game/web.h"
#include "spycraft/dmade/advmovie.h"
#include "spycraft/game/movie.h"					  
#include "spycraft/game/email.h"					  

namespace Spycraft {

#ifdef DIRECTX
extern void EraseAltLine();
#endif
Target *target = NULL;
int startframe = 0;    
/***********************************
 *									   
 *		Puzzle code
 *											 
 **********************************/
Target::Target()
{
	int count =0;
	GameFlag.set(fPatsHTM_125);
	IntrPuzzPage::init(WEB_TARGET);
	helppage = 92650;
	GameFlag.set(fTargetGPSAvailable);
	GameFlag.set(fTargetFreqAvailable);
	target = this;
	sfxSystemCursor(SYS_WAIT);
	curframe = 270;
	harm = false;
	kill = NULL;
	end = false;
	nah = NULL;
	if (webreport)
   {
//		webreport->hide();
      webreport->dispose();
      webreport = NULL;
	}
	if (comlinkshort)
	{
//		comlinkshort->hide();
      comlinkshort->dispose();
      comlinkshort = NULL;
	}
	homobutt->setCel(2);
	sfxLoadRes(253, RES_ATS);
	sfxLockRes(253, RES_ATS);

	if (GameFlag.test(fTargetGPSAvailable))
	{
		GPS = true;
		new GPSTab;
	}
	else
		GPS = false;

	if (GameFlag.test(fTargetFreqAvailable))
	{
		Freq = true;
		new FreqTab;

		freqs[0] =  new Freqs(385,181,3,0);
		freqs[0]->hide();
		freqs[1] =  new Freqs( 77,202,1,0);
		freqs[1]->hide();
		freqs[2] =  new Freqs( 40,249,1,0);
		freqs[2]->hide();
		freqs[3] =  new Freqs( 45,160,1,0);
		freqs[3]->hide();				
		freqs[4] =  new Freqs(120,316,2,0);
		freqs[4]->hide();
		freqs[5] =  new Freqs(181, 91,2,0);
		freqs[5]->hide();
		freqs[6] =  new Freqs(300,234,2,0);
		freqs[6]->hide();
		freqs[7] =  new Freqs(277,262,2,0);
		freqs[7]->hide();
		freqs[8] =  new Freqs(330,234,2,0);
		freqs[8]->hide();
		freqs[9] =  new Freqs(277,292,2,0);							   
		freqs[9]->hide();
		freqs[10] = new Freqs(120,316,3,0);
		freqs[10]->hide();
		freqs[11] = new Freqs(161, 91,3,0);
		freqs[11]->hide();

		red[0] =  new Freqs(74,272,3,0);
		red[0]->hide();
		red[1] =  new Freqs(246,167,3,0);
		red[1]->hide();
		red[2] =  new Freqs(240,250,3,0);
		red[2]->hide();
		red[3] =  new Freqs(328,337,3,0);
		red[3]->hide();
	}
	else
	{
		Freq = false;
		for (count = 0;count <12;count++)
			freqs[count] = NULL;
		for (count = 0;count <4;count++)
			red[count] = NULL;
	}
	if (GPS && Freq)
	{
		sfxLoadRes(251, RES_ATS);
		sfxLockRes(251, RES_ATS);
	}
	else if (!GPS && !Freq)
	{
		sfxLoadRes(250, RES_ATS);
		sfxLockRes(250, RES_ATS);
	}
	else 
	{
		sfxLoadRes(252, RES_ATS);
		sfxLockRes(252, RES_ATS);
	}

	GPSClicked = false;
	FreqClicked = false;
	quito = NULL;
	dalzel = new Dalzel;
	volpe = new Volpe;
	scharf = new Scharf;
	hwang = new Hwang;
	grenald = new Grenald;
	ehlko = new Ehlko;
	me = new Me;
	live = new Live;
	new RedSquare;
	out = true;
	me->hide();
	hidebuttons();
	setScript(new (TargetHScript));

	if (Freq)
		intrpuzz->setScript(new FreqScript);
}
int Target::handleEvent ( MADEEventStamp *event )
{
	if (!out)
	{
		if (event_type == USER_LEFT_UP)
		{
			if (event->cursor_pt.y < 345 && event->cursor_pt.y > 80)
			{
				me->destX = event->cursor_pt.x;
				me->destY = event->cursor_pt.y;
				me->activate();
				return true;
			}
		}
	}
	return IntrPuzzPage::handleEvent(event);
}
void Target::hidebuttons(void)
{
	if (quito)
	{
		web_intrface->release(quito);
		deathList->add(quito);		
		quito = NULL;				
	}
	Node *n = web_intrface->head;
	while ( n ) {
		((View*)n->data)->hide();
		n = n->next;
	}
}
void Target::showbuttons(void)
{
	Node *n = web_intrface->head;

	while ( n ) {
		if (!strcmp(n->data->name,"webhelp"))
			((View*)n->data)->setCel(2);
		((View*)n->data)->show();
		n = n->next;
	}
	quito = new QuitAlert(4,19,666,3,3);
	if (webreport)
		webreport->hide();
	if (comlinkshort)
		comlinkshort->hide();
}
void Target::dispose()
{
	GameFlag.clear(fPatsHTM_125);
	sfxMadeCursor();
	if (GPS && Freq)
		sfxUnlockRes(251, RES_ATS);
	else if (!GPS && !Freq)
		sfxUnlockRes(250, RES_ATS);
	else 
		sfxUnlockRes(252, RES_ATS);
	sfxUnlockRes(253, RES_ATS);
	target = NULL;
	IntrPuzzPage::dispose();
}

void Target::doit()
{
//	if (!out && !harm)
//	{
		if (me->x < 181 && me->x > 141 && me->y >71 && me->y<111)
		{
			harm = true;
//			leave();
		}	
		else 
		{
			harm = false;
		}
//	}
//	else if ()
	IntrPuzzPage::doit();
}
													 
void Target::leave()
{
	int count=0;
	if (!end)
	{
		theMovie->pause(false);
		curframe = theMovie->getPosn();
#ifndef DIRECTX
		theMovie->caller = NULL;
		theMovie->stop();
#endif
//		theMovie->open("250.avi");
//		theMovie->open("252.avi");
		if (!harm)
		{
//			if (curframe<495)
//			{
//				setScript(new(TargetAudioScript));
//				return; 
//			}
//			else if (curframe<945)
			if (curframe<945)
			{
				startframe = 2427;
				theMovie->fromTo(2427,2959);
			}
			else if (curframe<1995)
			{
				startframe = 2963;
				theMovie->fromTo(2963,3417);
			}
			else if (curframe<1845)
			{
				startframe = 3443;
				theMovie->fromTo(3443,3899);
			}
			else
			{
				startframe = 3907;
				theMovie->fromTo(3907,4413);
			}
		}
		else
		{
			startframe = 4448;
			theMovie->fromTo(4448,4524);
		}
	}
	out = true;
	if (GPSClicked)
	{
		dalzel->hide();
		volpe->hide();
		scharf->hide();
		hwang->hide();
		grenald->hide();
		ehlko->hide();
	}
	if (FreqClicked)
	{
		for(count=0;count<12;count++)
		{
			target->freqs[count]->hide();
		}	
		for(count=0;count<4;count++)
		{
			target->red[count]->hide();
		}	
	}
	me->hide();
	hidebuttons();

	if (!end)
	{
		kill = new Kill;
		nah = new Nah;
		if (!harm)
			setScript(new(TargetXtraScript));
		else
			setScript(new(HarmScript));
	}	
}

void Target::returned()
{
	int count=0;
	out = false;
	if (kill)
		kill->dispose();
	if (nah)
		nah->dispose();
	if (GPSClicked)
	{
		dalzel->show();
		volpe->show();
		scharf->show();
		hwang->show();
		grenald->show();
		ehlko->show();
	}
	if (FreqClicked)
	{
		for(count=0;count<12;count++)
		{
			target->freqs[count]->show();
		}	
		for(count=0;count<4;count++)
		{
			target->red[count]->show();
		}	
	}
	me->show();
	showbuttons();
	setScript(new(TargetAudioScript));
	sfxSystemCursor(SYS_ARROW);
}

void TargetHScript::changeState ( int newState )
{
	switchTo				  
		ticks = 15;
	END
	BEG						  
		theMovie->fromTo(0,268);
#ifndef DIRECTX
		theMovie->play("252.avi",this,false,true,false);
#else
		theMovie->play("252.avi",this,false,false,false);
#endif
//		theMovie->play("250.avi",this,false,false,false);
	END
	BEG										    
		target->returned();
	END
}
void HarmScript::changeState ( int newState )
{
	switchTo
		ticks = 15;
	END
#ifndef DIRECTX
   BEG
  		theMovie->open("252.avi");
		ticks = 15;
   END
#endif
	BEG
#ifdef DIRECTX
      EraseAltLine();
#endif
		sfxSetVideoOffMCI(false);
		ticks = 15;
	END
	BEG
#ifndef DIRECTX
		theMovie->play("252.avi",this,false,true,false);
#else
		theMovie->play("252.avi",this,false,false,false);
#endif
//		theMovie->play("250.avi",this,false,false,false);
	END
	BEG	
		ticks = 15;
	END
	BEG										    
		target->kill->dispose();
		target->nah->dispose();
		theMovie->fromTo(4544,4625);
		theMovie->play("252.avi",this,false,true,false);
//		theMovie->play("250.avi",this,false,true,false);
	END
	BEG										    
		GameFlag.clear(fPatsHTM_127);
		intrpuzz->quitIntrface();
		theGame->newRoom(new Rm60000(END_DEAD));
	END
}
void MobScript::changeState ( int newState )
{
	switchTo
		ticks = 15;
	END
	BEG
		target->kill->dispose();
		target->nah->dispose();
		theMovie->fromTo(4754,4832);
		theMovie->play("252.avi",this,false,true,false);
//		theMovie->play("250.avi",this,false,true,false);
	END
	BEG										    
		GameFlag.clear(fPatsHTM_127);
		intrpuzz->quitIntrface();
		theGame->newRoom(new Rm60000(END_DEAD));
	END
}
void KillScript::changeState ( int newState )
{
	switchTo
		ticks = 15;
	END
	BEG
		target->kill->dispose();
		target->nah->dispose();
		theMovie->fromTo(4634,4750);
		theMovie->play("252.avi",this,false,true,false);
//		theMovie->play("250.avi",this,false,false,false);
	END
	BEG			 
		ticks = 15;
	END
	BEG
		theMovie->fromTo(0,402);
		theMovie->play("253.avi",this,false,true,false);
//		theMovie->fromTo(4829,5239);
//		theMovie->play("250.avi",this,false,true,false);
	END
	BEG										    
		GameFlag.clear(fPatsHTM_127);
		intrpuzz->quitIntrface();
		if (GameFlag.test(fSentProcatDachaMsg) && GameFlag.test(fLinus25))
			new EMail("On to Yalta: Holt, David M.",98227);
		theGame->newRoom(new Rm1000);
	END
}

void LiveScript::changeState ( int newState )
{
	switchTo
		if (!target->out)
   	  		((View *)client)->show();	
		ticks = 15;
	END
	BEG										    
		if (!target->out)
   	  		((View *)client)->hide();	
		ticks = 15;
	END
	BEG										    
		client->setScript(new LiveScript);		
	END
}

void TargetAudioScript::changeState ( int newState )
{
	switchTo
		ticks = 15;
	END
#ifndef DIRECTX
   BEG
  		theMovie->open("252.avi");
		ticks = 15;
   END
#endif
	BEG
		sfxSetVideoOffMCI(true);
		ticks = 15;
	END

	BEG							    
		theMovie->fromTo(target->curframe,2425);
		theMovie->play("252.avi",this,false,false,false);
//		theMovie->play("252.avi",this,false,false,false);
	END

	BEG
		target->end = true;
		target->leave();
#ifdef DIRECTX
      EraseAltLine();
#endif
		sfxSetVideoOffMCI(false);
		ticks = 15;
	END

	BEG
		theMovie->caller = NULL;
		theMovie->stop();
		ticks = 15;
	END

	BEG							    
//		theMovie->fromTo(5684,6924);
//		theMovie->play("250.avi",this,false,false,false);
		theMovie->fromTo(848,2096);
		theMovie->play("253.avi",this,false,false,false);
	END

	BEG							    
		theMovie->fromTo(3098,3290);
		theMovie->play("253.avi",this,false,true,false);
//		theMovie->fromTo(7935,8174);
//		theMovie->play("250.avi",this,false,true,false);
	END

	BEG										    
		GameFlag.clear(fPatsHTM_127);
		intrpuzz->quitIntrface();
		theGame->newRoom(new Rm60000(END_FIRED));
	END
}											  

void TargetXtraScript::changeState ( int newState )
{
	switchTo
		ticks = 15;
	END
#ifndef DIRECTX
   BEG
  		theMovie->open("252.avi");
		ticks = 15;
   END
#endif
   BEG
		sfxSetVideoOffMCI(false);
		ticks = 15;
   END

   BEG
#ifdef DIRECTX
      	EraseAltLine();
#endif
//		theMovie->play("250.avi",this,false,false,true);
#ifndef DIRECTX
		theMovie->play("252.avi",this,false,true,false);
#else
		theMovie->play("252.avi",this,false,false,false);
#endif
	END
	BEG										    
		target->returned();
	END
}
RedSquare::RedSquare()
{
	name = "RedSquare";

	if (target->GPS && target->Freq)
		view = 251;
	else if (!target->GPS && !target->Freq)
		view = 250;
	else 
		view = 252;

	loop = 0;

	switch (view)
	{
		case 250:
			cel = 0;			
			break;
		case 251:
			if (target->GPSClicked && target->FreqClicked)
				cel = 1;
			else if (!target->GPSClicked && !target->FreqClicked)
				cel = 0;
			else if (target->GPSClicked)
				cel = 3;
			else
				cel = 2;
			break;
		case 252:
			if (target->GPS)
			{
				if(target->GPSClicked)
					cel = 3;
				else
					cel = 2;
			}
			else
			{
				if(target->FreqClicked)
					cel = 1;
				else											  
					cel = 0;
			}
			break;
	}

	init(0, 16);
	noSkip();
	addToPic();
	if (target->GPS && target->GPSClicked)
	{
		target->volpe->show();
		target->scharf->show();
		target->hwang->show();
		target->grenald->show();
		target->ehlko->show();
		target->dalzel->show();
	}
	else if	(target->GPS && !target->GPSClicked)
	{
		target->volpe->hide();
		target->scharf->hide();
		target->hwang->hide();
		target->grenald->hide();
		target->ehlko->hide();
		target->dalzel->hide();
	}
}
/***********************************
 *		
 *		  Features
 *		      
 **********************************/
int TabFeature::handleEvent ( MADEEventStamp *event )
{
	if (!target->out)
	{
		if (user->input && onMe(event))
		{
			if (event_type == USER_LEFT_UP)
			{
				activate();
				return true;
			}
		}
	}
	return false;
}
		
FreqTab::FreqTab()
{
	init (0,350,117,32);
}

void FreqTab::activate(void)
{
	int count=0;
	if (target->FreqClicked)
	{
		target->FreqClicked = false;

		for(count=0;count<12;count++)
		{
			target->freqs[count]->hide();
		}	
		for(count=0;count<4;count++)
		{
			target->red[count]->hide();
		}	
	}
	else
		target->FreqClicked = true;

	new RedSquare;
}

GPSTab::GPSTab()
{
	init (126,350,117,32);
}

void GPSTab::activate(void)
{
	if (target->GPSClicked)
		target->GPSClicked = false;
	else
		target->GPSClicked = true;
	new RedSquare;
}
/***********************************
 *		
 *		  Those Blasted Guards
 *		      
 **********************************/
void Guard::init()
{
	setInc(1,1);
	draw();
	intrpuzz->addToFront ( this );
	setScript (new(Beat));
	if (!target->GPS || !target->GPSClicked)
		hide();
}	
Volpe::Volpe()
{
	name = "volpe";
	int myposts[5][2] = {{30,82},{57,159},{100,127},{145,87},{153,161}};
	memcpy (&posts,&myposts,(10*sizeof(int)));
	view = 253;
	loop = 4;
	cel = 5;
	x = 100;
	y = 111;
	curPos = 2;
	init();
}	
Scharf::Scharf()
{
	name = "scharf";
	int myposts[5][2] = {{385,84},{421,88},{385,166},{368,220},{428,230}};
	memcpy (&posts,&myposts,(10*sizeof(int)));
	view = 253;
	loop = 4;
	cel = 1;
	x = 478;
	y = 214;
	curPos = 5;
	init();
}	
Hwang::Hwang()
{
	name = "hwang";
	int myposts[5][2] = {{203,86},{329,88},{250,126},{210,63},{338,164}};
	memcpy (&posts,&myposts,(10*sizeof(int)));
	view = 253;
	loop = 4;
	cel = 3;
	x = 329;
	y = 72;
	curPos = 1;
	init();
}	
Grenald::Grenald()
{
	name = "grenald";
	int myposts[5][2] = {{182,204},{289,222},{243,256},{187,339},{280,352}};
	memcpy (&posts,&myposts,(10*sizeof(int)));
	view = 253;
	loop = 4;
	cel = 2;
	x = 243;
	y = 240;
	curPos = 2;					    
	init();
}									    
Ehlko::Ehlko()
{
	name = "ehlko";
	int myposts[5][2] = {{406,278},{425,281},{325,338},{436,336},{0,0}};
	memcpy (&posts,&myposts,(10*sizeof(int)));
	view = 253;
	loop = 4;
	cel = 4;
	x = 325;
	y = 342;
	curPos = 2;
	init();
}	
Dalzel::Dalzel()
{
	name = "dalzel";
	int myposts[5][2] = {{86,124},{146,202},{43,269},{41,337},{321,343}};
	memcpy (&posts,&myposts,(10*sizeof(int)));
	view = 253;
	loop = 4;
	cel = 0;
	x = 81;
	y = 251;
	curPos = 3;
//	init();
	draw();
	intrpuzz->addToFront ( this );
	if (!target->GPS || !target->GPSClicked)
		hide();
}	


void Beat::changeState ( int newState )				    
{
	int num = 0;
 	switchTo
		num = ((Guard*)client)->curPos;

		while (((Guard*)client)->curPos == num || num > 4 || !((Guard*)client)->posts[num][0])
			num = rand();
		((Guard*)client)->curPos = num;
		((Guard*)client)->setMotion(new MoveTo(((Guard*)client)->posts[num][0],((Guard*)client)->posts[num][1],
												(Actor*)client,this));
	END

	BEG
		((Guard*)client)->setScript(new (Beat));		
	END
}
								 
/***********************************
 *		
 *	Other Blinkin' Objects
 *		      
 **********************************/
Freqs::Freqs(int theX, int theY, int loop_id,int cel_id)
{
	cycleSpeed = 6;
	init(theX,theY,253,loop_id,cel_id);
}	
Live::Live()
{
	name = "live";
	view = 253;
	loop = 0;
	cel = 0;
	init(368,361);
	setScript(new LiveScript);
}
Kill::Kill()
{
	name = "kill";
	view = 911;
	loop = 1;
	cel = 0;
	init(0,400);
	noSkip();
}
void Kill::dispose(void)
{
	Button::dispose();
	target->kill = NULL;
}																															   

void Kill::activate(void)
{
	theMovie->pause(false);
	target->curframe = theMovie->getPosn();

	if (target->curframe < 4413)
		target->setScript(new(MobScript));
	else
		target->setScript(new(KillScript));
}																															   

Nah::Nah()
{
	name = "Nah";
	view = 904;
	loop = 2;
	cel = 0;
 	init(391,400);
	noSkip();
	setScript(new NahBlinkScript);
}
void Nah::dispose(void)
{
	setScript(NULL);
	Button::dispose();
	target->nah = NULL;
}																															   
void NahBlinkScript::changeState ( int newState )
{
	switchTo
		ticks=30;
	END
	BEG
		((Nah*)client)->setCel(1);
		ticks=30;
	END
	BEG
		((Nah*)client)->setCel(0);
		client->setScript(new NahBlinkScript);
	END
}
void Nah::activate(void)
{

#ifdef DIRECTX
	int pos;
	Object* callit = theMovie->caller;
	theMovie->pause(false);
	pos = theMovie->getPosn();
	theMovie->caller = NULL;
	theMovie->resume();
	theMovie->stop();
	theMovie->fromTo(pos-1,pos);
	theMovie->play("252.avi",callit,false,false,false);
#else
	theMovie->pause(false);
	theMovie->caller = NULL;
//	interrupted = true;
	theMovie->caller->cue();
#endif
}																															   
/***********************************
 *		
 *	Player
 *		      
 **********************************/
Me::Me()
{
	name = "Me";
	cycleSpeed = 15;	
	destX = 0;
	destY = 0;
	tempdestX = 0;
	tempdestY = 0;
	diverted = false;
	view = 253;
	loop = 5;
	cel = 0;
	x = 410;
	y = 314;
	setInc(1,1);
	draw();
	intrpuzz->addToFront ( this );
	setScript(new (MeScript));
}
void Me::cue(void)
{
	if (diverted)
	{
		diverted = false;
		setMotion( new MoveTo(destX,destY,(Actor*)this,this));
	}
}
void Me::activate(void)
{
	setMotion( new MoveTo(destX,destY,(Actor*)this,this));
}
void Me::doit(void)
{
	if (mover)
	{
		if (x < 86 && y > 134 && y < 269)
		{
			if (x + xInc > 85)
			{
				diverted = true;
				x += xInc;	
				posn(x,y);
				tempdestX = x;

				if (y <= destY)
					tempdestY = 279;
				else
					tempdestY = 124;

				setMotion( new MoveTo(tempdestX,tempdestY,(Actor*)this,this));
			}
			else
			{
				diverted = true;
				if (y + yInc > 268)
					y += yInc;
				else
					y -= yInc;
				posn(x,y);
				tempdestY = y;
				tempdestX = 100;
				setMotion( new MoveTo(tempdestX,tempdestY,(Actor*)this,this));
			}
		}
		else if (x > 287 && x < 376 && y > 246 && y < 301)
		{
			if (x + xInc > 375)
			{
				diverted = true;
				x += xInc;	
				posn(x,y);
				tempdestX = x;

				if (y >= destY)
					tempdestY = 237;
				else
					tempdestY = 321;

				setMotion( new MoveTo(tempdestX,tempdestY,(Actor*)this,this));
			}
			else if (x - xInc < 288)
			{
				diverted = true;
				x -= xInc;	
				posn(x,y);
				tempdestX = x;

				if (y >= destY)
					tempdestY = 237;
				else
					tempdestY = 321;

				setMotion( new MoveTo(tempdestX,tempdestY,(Actor*)this,this));
			}
			else if (y - yInc < 247)
			{
				diverted = true;
				y -= yInc;
				
				if (x >= destX)
					tempdestX = 275;
				else
					tempdestX = 396;

				posn(x,y);
				tempdestY = y;
				setMotion( new MoveTo(tempdestX,tempdestY,(Actor*)this,this));
			}
			else
			{
				diverted = true;
				y += yInc;
				
				if (x >= destX)
					tempdestX = 275;
				else
					tempdestX = 396;

				posn(x,y);
				tempdestY = y;
				setMotion( new MoveTo(tempdestX,tempdestY,(Actor*)this,this));
			}
		}
	}
	IntrPuzzActor::doit();
}
void MeScript::changeState ( int newState )
{
	switchTo
		ticks=30;
	END
	BEG
		((Me*)client)->setCel(1);
		ticks=30;
	END
	BEG
		((Me*)client)->setCel(0);
		client->setScript(new MeScript);
	END
}
void FreqScript::changeState ( int newState )
{
	switchTo
		target->freqs[4]->setCycle(new EndLoop((Prop*)target->freqs[4],this));
		target->freqs[5]->setCycle(new EndLoop((Prop*)target->freqs[5],NULL));
		target->freqs[6]->setCycle(new EndLoop((Prop*)target->freqs[6],NULL));
		target->freqs[7]->setCycle(new EndLoop((Prop*)target->freqs[7],NULL));
		target->freqs[8]->setCycle(new EndLoop((Prop*)target->freqs[8],NULL));
		target->freqs[9]->setCycle(new EndLoop((Prop*)target->freqs[9],NULL));

		if (!target->out &&	target->FreqClicked)
		{
			target->freqs[4]->show();
			target->freqs[5]->show();
			target->freqs[6]->show();
			target->freqs[7]->show();
			target->freqs[8]->show();
			target->freqs[9]->show();
		}
	END
	BEG
		target->freqs[4]->hide();
		target->freqs[5]->hide();
		target->freqs[6]->hide();
		target->freqs[7]->hide();
		target->freqs[8]->hide();
		target->freqs[9]->hide();
		seconds = 1;
	END
	BEG
		target->freqs[0]->setCycle(new EndLoop((Prop*)target->freqs[0],this));
		target->freqs[10]->setCycle(new EndLoop((Prop*)target->freqs[10],NULL));
		target->freqs[11]->setCycle(new EndLoop((Prop*)target->freqs[11],NULL));
		target->red[0]->setCycle(new EndLoop((Prop*)target->red[0],NULL));
		target->red[1]->setCycle(new EndLoop((Prop*)target->red[1],NULL));
		target->red[2]->setCycle(new EndLoop((Prop*)target->red[2],NULL));
		target->red[3]->setCycle(new EndLoop((Prop*)target->red[3],NULL));

		if (!target->out &&	target->FreqClicked)
		{
			target->freqs[0]->show();
			target->freqs[10]->show();
			target->freqs[11]->show();
			target->red[0]->show();
			target->red[1]->show();
			target->red[2]->show();
			target->red[3]->show();
		}
	END
	BEG										    
		target->freqs[0]->hide();
		target->freqs[10]->hide();
		target->freqs[11]->hide();
		target->red[0]->hide();
		target->red[1]->hide();
		target->red[2]->hide();
		target->red[3]->hide();
		seconds = 1;
	END
	BEG
		target->freqs[1]->setCycle(new EndLoop((Prop*)target->freqs[1],this));
		target->freqs[2]->setCycle(new EndLoop((Prop*)target->freqs[2],NULL));
		target->freqs[3]->setCycle(new EndLoop((Prop*)target->freqs[3],NULL));

		if (!target->out &&	target->FreqClicked)
		{
			target->freqs[1]->show();
			target->freqs[2]->show();
			target->freqs[3]->show();
		}
	END
	BEG										    
		target->freqs[1]->hide();
		target->freqs[2]->hide();
		target->freqs[3]->hide();
		seconds = 1;
	END
	BEG
		client->setScript(new FreqScript);
	END
}

} // namespace Spycraft
