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

#include "spycraft/game/torture.h"
#include "spycraft/game/vlink.h"
#include "spycraft/game/flag.h"

namespace Spycraft {

extern DialTree	*dialogTree;
HeartInfo		*heartinfo;
Torture 		*torture;
Question		*currentQ;
Meter			*meter;
FryHandle		*handle;
ShockMeter		*shockmeter;
HeartMonitor	*monitor;
ShockIncrease	*shockincrease;
ShockDecrease	*shockdecrease;

int dialogCue;
bool cursorExit;
				  
unsigned int threshhold;
int cookLevel;
int tortureFocus = 0;
unsigned int minimumHeartRate = 0;
bool specialFlags[20];

Sequence sequence[3] = {{0, 4}, {0, 8}, {0, 8}};

#define MAX_HEARTRATE			213
#define HEARTATTACK_RATE		194
#define YINGDIALOG				1

MovieData currentLoop;

MovieData movieData[33] = {{990, 1045, 1045}, {1410, 1445, 1445}, {4770, 4830, 4830},   //low cooklevel
							  /*{4500, 4558, 4558}*/{4770, 4830, 4830}, {0, 0, 0}, {0, 0, 0},
							  {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 
 							  
 							  {150, 190, 190}, {255, 269, 269}, {1215, 1263, 1263}, //medium cook level
							  {3120, 3144, 3144}, {4950, 5051, 5051}, {2340, 2392, 2392},
							  {4845, 4890, 4890}, {315, 370, 370}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 

 							  {855, 917, 917}, {1680, 1717, 1717}, {1725, 1807, 1807}, //high cook level
							  {2130, 2172, 2172}, {2190, 2243, 2243}, {4320, 4426, 4426},
							  {4440, 4498, 4498}, {4905, 4937, 4937}, {0, 0, 0},
							  {0, 0, 0}, {0, 0, 0}}; 

Dialog* 	tortureDialog0;
Dialog* 	tortureDialog1;
Dialog*		tortureDialog2;
Dialog*		tortureDialog3;
Dialog*		tortureDialog4;
Dialog*		tortureDialog5;
Dialog*		tortureDialog6;
Dialog*		tortureDialog7;
Dialog* 	tortureDialog8;
Dialog*		tortureDialog9;

DItemLine1* relationshipCohen;
DItemLine1* cohenKnow;
DItemLine1* pickCohen;

DItemLine2* rolePEG;
DItemLine2* deliverWeapon; 
DItemLine2* whatElse;

DItemLine3* workFor;
DItemLine3* writesChecks;
DItemLine3* elseKnow;

enum	
{
	RELATIONSHIPCOHEN,
	COHENKNOW,
	PICKCOHEN,
	ROLEPEG,
	DELIVERWEAPON,
	WHATELSE,
	WORKFOR,
	WRITESCHECKS,
	ELSEKNOW
};

/***********************************
 *		
 *			 Puzzle Page
 *		      
 **********************************/
Torture::Torture()
{

	torture = this;
	IntrPuzz::init(1);
 	memset (&specialFlags,0,20 * sizeof(bool));
	sfxSystemCursor(SYS_WAIT);

	sfxLoadRes(300, RES_ATS);
	sfxLockRes(300, RES_ATS);
	setScript(new ScriptBeginTorture);
}

Torture::~Torture()
{
	sfxUnlockRes(300, RES_ATS);
	currentFocus = false;
}

int Torture::handleEvent ( MADEEventStamp *event )
{
/*  	if (event_type == USER_RIGHT_UP)  //temporary to quit out of the torture
    {
		monitor->setCycle(NULL);
		theMovie->caller = 0;
		theMovie->stop();
		intrpuzz->quitIntrface();
		return true;		
    }
*/
	if(tortureFocus == YINGDIALOG)
	   	dialogTree->handleEvent(event);

	IntrPuzz::handleEvent(event);

	return false;
}

void Torture::cue ( void )
{
	if(yingDead)	 //we killed Ying
	{
		theGame->handsOff();
		currentFocus = PUZZLE_FOCUS;
		//sfxPrintf("ying is fried");
		GameFlag.set(fTortureYingDead);
		sfxMadeCursor();
		monitor->dispose();
		theMovie->caller = 0;
		theMovie->stop();
		intrpuzz->quitIntrface();
		return;
	}	

	if(yingQuestion)  //we have broken ying and are starting interactive dialog
	{
		theMovie->fromTo(2040, 2118);
		theMovie->play("25012.avi", this, 0, 0, 0);
		return;		
	}

	if(!zapping)
	{
		if(gotVoltage)
		{
			char rate[5];

			gotVoltage = false;
			currentQ->myPrint->highlight(false);//deselect the current topic			

			HeartRate += rateIncrease;
			heartinfo->dispose();
			Common::sprintf_s(rate, "%d", HeartRate);
			new HeartInfo(rate);

			if(yingBroke)
			{
				if(currentQ == q1)
					dialogCue = 1;

				if(currentQ == q2)
					dialogCue = 2;

				if(currentQ == q3)
					dialogCue = 3;

				q1->shockSwitch->dispose();
				q1->questionbox->dispose();
				q1->dispose();
				q2->shockSwitch->dispose();
				q2->dispose();
				q3->shockSwitch->dispose();
				q3->dispose();
	 	 		yingQuestion = true;
				torture->setScript(new ScriptBeginQuestion);
				return;
			}
		}

/*		if(killYing)
		{
			theMovie->fromTo(390, 630);
			theMovie->play("25012.avi", this, 0, 0, 0);
			yingDead = true;	
			return;
		}
		  */
		theMovie->fromTo(currentLoop.inFrame, currentLoop.outFrame);
		theMovie->play("25012.avi", this, 0, 0, 0);
//		theGame->handsOn();
		user->input = true;
	}
	else
	{
		if(killYing)
		{
			theMovie->fromTo(390, 630);
			theMovie->play("25012.avi", this, 0, 0, 0);
			yingDead = true;	
			return;
		}

		if(sequence[cookLevel].number >= sequence[cookLevel].maxMovies)
		{ 
			sequence[cookLevel].number = 0;
		}
		int index = ((cookLevel) * 11) + sequence[cookLevel].number;

		MovieData frames;	
		if(specialMovieCase(&frames))
		{
			theMovie->fromTo(frames.inFrame, frames.outFrame);
		}
		else
		{	  
			theMovie->fromTo(movieData[index].inFrame, movieData[index].outFrame);
		}
		theMovie->play("25012.avi", this, 0, 0, 0);

		if(movieData[index].loopFrame)
		{
//			currentLoop.inframe = movieData[index].inFrame;
//			currentLoop.outFrame = movieData[index].loopFrame;			
			if(cookLevel == 0)
			{
				currentLoop.inFrame = 2040;
				currentLoop.outFrame = 2118;			
			}
		/*	else			 
			{
				currentLoop.inFrame = 2250;
				currentLoop.outFrame = 2314;			
			} */
		}
		sequence[cookLevel].number++;
		gotVoltage = true;	 //after done playing the movie do got voltage stuff
		zapping = false;						
	}

}					 

ExitTorture::ExitTorture()
{
	init(16, 464, 300, 10, 0);
	noSkip();
}

void ExitTorture::activate()
{
	theGame->handsOff();
	if(tortureFocus == YINGDIALOG)		
	{
		//using the global pickCohen to get to myDialog
		pickCohen->myDialog->hide();
		currentFocus = PUZZLE_FOCUS;
		if (pickCohen->myDialog->myTree->caller)
			pickCohen->myDialog->myTree->caller = NULL;

	//	GameFlag.set(fBrokenYingInterrogation);
		tortureFocus = 0;
		sfxMadeCursor();
		monitor->dispose();
		theMovie->caller = 0;
		theMovie->stop();
		intrpuzz->quitIntrface();
	}	
	else
	{
		sfxMadeCursor();
		monitor->dispose();
		theMovie->caller = 0;
		theMovie->stop();
		intrpuzz->quitIntrface();
	}
	theGame->handsOff();
}

/*SouthExitYing::SouthExitYing()
{
	name = "SouthExit";
//	setHotspot ( DO_V );
	left = 46;
	top = 390;
	right = left + 495 - 1;
	bottom = top + 20 - 1;

	intrpuzz->add ( this );
}

int SouthExitYing::handleEvent(MADEEventStamp* event)
{
	if (user->input)
	{
		if(onMe(event))
		{
			if(!cursorExit)
			{
				theGame->setCursor(southCursor);
				cursorExit = true;
			}

			if (event_type == USER_LEFT_DOWN)
			{
				return true;
			}
		}
		else
		{
			if(cursorExit)
			{
			//	theGame->setCursor(normalCursor);
				sfxSystemCursor(SYS_ARROW);
				cursorExit = false;
			}
		}	
	}
	return false;
}				
*/

int Torture::specialMovieCase(MovieData *movieFrames)
{
	
	if(currentQ == q2)
	{
	 	if(HeartRate > (threshhold - 10))
		{
			if(!specialFlags[10])
			{
				movieFrames->inFrame = 3855;
				movieFrames->outFrame = 3928;			
				specialFlags[10] = true;
				return true;
			}				
		}
		else if(HeartRate > (threshhold - 20))
		{
			if(!specialFlags[11])
			{
				movieFrames->inFrame = 2415;
				movieFrames->outFrame = 2481;			
				specialFlags[11] = true;
				return true;
			}
		}
	}

	if(currentQ == q3)
	{
		if(HeartRate > (threshhold - 20))
		{
			if(!specialFlags[12])
			{
				movieFrames->inFrame = 3015;
				movieFrames->outFrame = 3101;			
				specialFlags[12] = true;
				return true;
			}
		}

		if(specialFlags[12] && !specialFlags[13])
		{
			movieFrames->inFrame = 4245;
			movieFrames->outFrame = 4312;			
			specialFlags[13] = true;
			return true;
		}
	}				

	if(HeartRate > 165)
	{
		if(currentQ == q3 && !specialFlags[0]) //
		{
			movieFrames->inFrame = 1350;
			movieFrames->outFrame = 1395;			
			specialFlags[0] = true;
			return true;
		}
	}

	if(HeartRate > 160 && !specialFlags[1])
	{
		specialFlags[1] = true;
		movieFrames->inFrame = 660;
		movieFrames->outFrame = 836;			
		return true;
	}

	if(HeartRate > 155)
	{
		if(currentQ == q3 && !specialFlags[2]) //
		{
			specialFlags[2] = true;
			movieFrames->inFrame = 285;
			movieFrames->outFrame = 306;			
			return true;
		}
		else if(!specialFlags[3])
		{
			specialFlags[3] = true;
			movieFrames->inFrame = 105;
			movieFrames->outFrame = 143;			
			return true;
		}
	}

	if(HeartRate > 145)
	{
/*		if(currentQ == q1 && !specialFlags[4]) //
		{
 			specialFlags[4] = true;
			movieFrames->inFrame = 1275;
			movieFrames->outFrame = 1334;			
			return true;
		}*/
		if(currentQ == q1 && !specialFlags[5])
		{
 			specialFlags[5] = true;
			movieFrames->inFrame = 1515;
			movieFrames->outFrame = 1607;			
			return true;
		}
	}
								
	if(HeartRate > 135  && !specialFlags[6])
	{
		specialFlags[6] = true;
		movieFrames->inFrame = 195;
		movieFrames->outFrame = 246;			
		return true;
	}

	if(HeartRate > 125)
	{
		if(currentQ == q2  && !specialFlags[7]) //
		{
			specialFlags[7] = true;
			movieFrames->inFrame = 1065;
			movieFrames->outFrame = 1133;			
			return true;
		}
		else if(!specialFlags[8])
		{
			specialFlags[8] = true;
			movieFrames->inFrame = 1620;
			movieFrames->outFrame = 1674;			
			return true;
		}
	}

	if((HeartRate > 64) && (HeartRate < 74))
	{
		if(!specialFlags[9])
		{
			movieFrames->inFrame = 0;
			movieFrames->outFrame = 83;			
			specialFlags[9] = true;
			return true;
		}
	}

	return false;
}

void Torture::setRate()
{
	char rate[20];

	rateIncrease = ((cookLevel + 1) * (rand() % 8) + 3);
	HeartRate += rateIncrease;
	heartinfo->dispose();
	Common::sprintf_s(rate, "%d", HeartRate);
	new HeartInfo(rate);
}
	
/***********************************
 *		
 *			  Print
 *		      
 **********************************/

HeartInfo::HeartInfo(const char *infoText) {
	heartinfo = this;

	fore_color = TEXT_COLOR_GREEN;
	x = 26;
	y = 417;
	size = 17;
	init(infoText);
}

/***********************************
 *		
 *			  Shock Buttons
 *		      
 **********************************/
ShockSwitch::ShockSwitch(int theX, int theY)
{
	init(theX, theY, 300, 7, 0);
	right = right +	280;
	noSkip();
}


int ShockSwitch::handleEvent(MADEEventStamp* event)
{
	if (user->input)
	{
		if(onMe(event))
		{
			if(!thisQ->hilit)
			{
				thisQ->hilit = true;
				thisQ->myPrint->highlight(true);
			}
		  	if (event_type == USER_LEFT_UP)
		    {
		   		if (cel == 1)
		   		{
			   		setCel(0);
					right = right +	280;
					activate();
					return(true);
				}
		    }
			else if (event_type == USER_LEFT_DOWN)
			{
				if (cel == 0)
				{
					setCel(1);
					right = right +	280;
					return (true);
				}
			}
			
		}
		else
		{
			if(thisQ->hilit)
			{
				thisQ->hilit = false;
				thisQ->myPrint->highlight(false);
			}

			if(cel == 1)
			{
				setCel(0);				
				right = right +	280;
			}
		}
	}
	return false;
}

void ShockSwitch::activate()
{
	if(thisQ->thresh > threshhold)
		threshhold = thisQ->thresh;

	currentQ = thisQ;

	torture->setRate();
	torture->zapping = true;
 	user->input = false;
	theMovie->caller = NULL;
	theMovie->pause(false);
	torture->cue();	
}

QuestionBox::QuestionBox()
{
	view = 300;
	loop = 8;
	cel = 0;
	init(117, 405);
	noSkip();
	setPri(20);
}

void SwitchActivateScript::changeState( int newState )
{
	switchTo
		((ShockSwitch *)client)->setCel(1);
		ticks = 10;
	END
	BEG
		((ShockSwitch *)client)->setCel(0);
		((ShockSwitch *)client)->activate();
		dispose();
	END
}
/***********************************
 *		
 *			  TextFeature
 *		      
 **********************************/
Q1::Q1()
{
	thresh = 160;
	name = "Tell me about the PEG theft.";
//	init(103, 415, 280, 15);
	init(123, 412, 280, 15);
	shockSwitch = new ShockSwitch(88, 404);
	shockSwitch->thisQ = this;
	questionbox = new(QuestionBox);
}
	
Q2::Q2()
{
	thresh = 145;
	name = "What is your relationship to Darren Cohen?";
//	init(117, 434, 280, 15);
	init(123, 433, 280, 15);
	shockSwitch = new ShockSwitch(88, 425);
	shockSwitch->thisQ = this;
}
    
Q3::Q3()
{
	thresh = 185;
	name = "Who do you work for?";
	init(123, 455, 280, 15);
//	init(161, 453, 150, 15);
	shockSwitch = new ShockSwitch(88, 447);
	shockSwitch->thisQ = this;
}

void Question::init(int theX, int theY, int width, int height)
{
	left = theX;
	top = theY;
	right = left + width - 1;
	bottom = top + height - 1;

  	myPrint = new(Print);
  	myPrint->x = theX;
  	myPrint->y = theY;
	myPrint->init(name);

	intrpuzz->addToFront ( this );	 
}

int Question::handleEvent(MADEEventStamp* event)
{
	return false;

/*	if (user->input)
	{
		if(onMe(event))
		{
			if(!hilit)
			{
				hilit = true;
				myPrint->highlight(true);
			}
		  	if (event_type == USER_LEFT_UP)
		    {
		   		if (shockSwitch->cel == 1)
		   		{
			   		shockSwitch->setCel(0);
					shockSwitch->activate();
			//		return(true);
				}
		    }
			else if (event_type == USER_LEFT_DOWN)
			{
				if (shockSwitch->cel == 0)
				{
					shockSwitch->setCel(1);
		//			return (true);
				}
			}
			
		}
		else
		{
			if(hilit)
			{
				hilit = false;
				myPrint->highlight(false);
			}

			if(shockSwitch->cel == 1)
				shockSwitch->setCel(0);				
		}
	}
	return false;*/
}

/***********************************
 *		
 *			  Buttons
 *		      
 **********************************/
Shock::Shock()
{
	buttonDown = false;
}

int Shock::handleEvent(MADEEventStamp* event)
{
	if (user->input)
	{
		if (onMe(event) && (cel != 2))  
	  	{
		  	if (event_type == USER_LEFT_UP)
		    {
		   		if (cel == 1)
		   		{
			   		setCel(0);
					return(true);
				}
		    }
			else if (event_type == USER_LEFT_DOWN)
			{
				if (cel == 0)
				{
					setCel(1);
					activate();
					return (true);
				}

			}
			else if(cel == 1)
			{
				activate();
				return (true);
			}
		}
	}
	else
	{
		if(cel == 1)
		{	
	  		setCel(0);
		}
	}
  return false;
}

void Shock::activate()
{
	int newCel = shockmeter->cel + increment;
	
	if((newCel >= 2) && (newCel <= 17))	
	{
		time = gameTime;
		if((time - prevtime) > 4)
		{						
			shockmeter->setCel(newCel);
			prevtime = time;
			cookLevel = newCel / 6;
		}			
	}		
}

int Shock::getLevel()
{
	return (cel / 6);
}

ShockIncrease::ShockIncrease()
{
	view = 300;
	loop = 5;
	cel = 0;
	init(471, 441);
	increment = 1;
	noSkip();
	shockincrease = this;
	setPri(555);
}

ShockDecrease::ShockDecrease()
{
	view = 300;
	loop = 6;
	cel = 0;
	init(440, 441);
	increment = -1;
	noSkip();
	shockdecrease = this;
	setPri(555);
}

ShockMeter::ShockMeter()
{
	view = 300;
	loop = 4;
	cel = 2;
	init(440, 414);
	noSkip();
	setPri(555);
}

HeartMonitor::HeartMonitor()
{
	name = "HeartMonitor";
	cycleSpeed = 4;	
	view = 300;
	loop = 2;
	cel = 0;
	x = 1;
	y = 399;
	monitor = this;
	
	draw();
	intrpuzz->addToFront ( this );
	setCycle(new EndLoop((Prop*)this, this));
}
			    
void HeartMonitor::cue(void)
{
	
/*	if(torture->yingDead)
	{
		setLoop(3);
	}*/	
	if(torture->HeartRate > 110)
	{
		if(loop == 2)
			setLoop(1);
	}
	else
	{
		if(loop == 1)
			setLoop(2);
	}

	setCycle(new EndLoop((Prop*)this, this));
}

/***********************************
 *		
 *			  Views
 *		      
 **********************************/
ControlPanel::ControlPanel()
{
	view = 300;
	loop = 0;
	cel = 0;
	init(0, 399);
	noSkip();
//	setPri(1);
///	addToPic();
}

Meter::Meter()
{
	view = 300;
	loop = 3;
	cel = 0;
	init(444, 413);
	noSkip();
	setPri(555);
}

/***********************************
 *		
 *			Heart Rate Timer
 *		      
 **********************************/
YingHeart::YingHeart()
{
	breakctr = 0;
	deathctr = 0;
 	HeartRateTimer = new (IntrPuzzTimer);
	HeartRateTimer->setReal(1,this);
}	

void YingHeart::cue()
{
	char rate[20];

	if(torture->HeartRate > MAX_HEARTRATE)
	{
		torture->killYing = true;
	}
/*	else if(torture->yingQuestion)
	{
//		currentFocus = PUZZLE_FOCUS;
	}*/
	else if((torture->HeartRate > threshhold) && threshhold)
	{
		breakctr++;
		if(breakctr > 20)
		{
	 		torture->yingBroke = true;
	 		breakctr = 0;
		}
	}	


	if(torture->HeartRate > HEARTATTACK_RATE)
	{
		if(deathctr > 5)
		{		
			if((rand() % 5) == 3)
			{
				torture->killYing = true;
			}
			deathctr = 0;		
		}
		else
		{
			deathctr++;
		}		
	}

	if((torture->HeartRate > minimumHeartRate) && user->input)//(!torture->gotVoltage))
	{
		if(dropctr > 3)
		{	
			torture->HeartRate -= 2;
			heartinfo->dispose();
			Common::sprintf_s(rate, "%d", torture->HeartRate);
			new HeartInfo(rate);
			dropctr = 0;	
		}
		else
		{
			dropctr++;
		}
	}

	if(!torture->yingDead)
	{
	 	HeartRateTimer = new (IntrPuzzTimer);
		HeartRateTimer->setReal(1,this);
	}
}		

void ScriptBeginTorture::changeState( int newState )
{
	switchTo
		user->input = false;
		theMovie->fromTo(1815, 2021);
		theMovie->play("25012.avi", this, 0, 0, 0);
	END
	BEG
		new(ControlPanel);
		new(ShockIncrease);
		new(ShockDecrease);
		shockmeter = new(ShockMeter);
		new(HeartMonitor);
		torture->q1 = new(Q1);
		torture->q2 = new(Q2);
		torture->q3 = new(Q3);
		cursorExit = false;
		//new(SouthExitYing);
		new(ExitTorture);

		minimumHeartRate = 60;
		torture->HeartRate = 60;
		cookLevel = 0;
		threshhold = 0;
		torture->zapping = false;
		torture->killYing = false;
		torture->yingDead = false;
		torture->yingBroke = false;
		torture->yingQuestion = false;
		torture->gotVoltage = false;
		torture->rateIncrease = 0;
		currentQ = NULL;

		currentLoop.inFrame = 2040;
		currentLoop.outFrame = 2118;
	 	intrface->disable();

		new HeartInfo("60");
		new(YingHeart);

		torture->cue();
		user->input = true;
		sfxSystemCursor(SYS_ARROW);
		dispose();
	END
}

/***********************************
 *		
 *		 Interactive dialog
 *		      
 **********************************/
int DItemLine::yingBaked()
{
	if(torture->HeartRate > MAX_HEARTRATE)
	{
		myDialog->hide();
		currentFocus = PUZZLE_FOCUS;
		if (myDialog->myTree->caller)
			myDialog->myTree->caller = NULL;

		tortureFocus = 0;
		theMovie->fromTo(390, 630);
		theMovie->play("25012.avi", torture, 0, 0, 0);
		torture->yingDead = true;	
		return true;
	}
	return false;
}

void DItemLine1::doit()
{
////test
	if(relationshipCohen->hasBeenChosen)
	{
		if(cohenKnow->hasBeenChosen)
		{
			if(!pickCohen->hasBeenChosen)
			{
				nextDialog->add(pickCohen);
			}
		}
		else
		{
			nextDialog->add(cohenKnow);
		}
	}
	else
	{
		nextDialog->add(relationshipCohen);
	}
//////

	if(rolePEG->hasBeenChosen)
	{
		if(deliverWeapon->hasBeenChosen)
		{
			if(!whatElse->hasBeenChosen)
			{
				nextDialog->add(whatElse);
			}
		}
		else
		{
			nextDialog->add(deliverWeapon);
		}
	}
	else
	{
			nextDialog->add(rolePEG);
	}
			
	if(workFor->hasBeenChosen)
	{
		if(writesChecks->hasBeenChosen)
		{
			if(!elseKnow->hasBeenChosen)
			{
				nextDialog->add(elseKnow);
			}
		}
		else
		{
			nextDialog->add(writesChecks);
		}
	}
	else
	{
		nextDialog->add(workFor);
	}
//	torture->setRate();
	
//	if(!yingBaked())	
		DItem::doit();
}	   

void DItemLine2::doit()
{
	if(relationshipCohen->hasBeenChosen)
	{
		if(cohenKnow->hasBeenChosen)
		{
			if(!pickCohen->hasBeenChosen)
			{
				nextDialog->add(pickCohen);
			}
		}
		else
		{
			nextDialog->add(cohenKnow);
		}
	}
	else
	{
		nextDialog->add(relationshipCohen);
	}

	if(workFor->hasBeenChosen)
	{
		if(writesChecks->hasBeenChosen)
		{
			if(!elseKnow->hasBeenChosen)
			{
				nextDialog->add(elseKnow);
			}
		}
		else
		{
			nextDialog->add(writesChecks);
		}
	}
	else
	{
		nextDialog->add(workFor);
	}

///test 
	if(rolePEG->hasBeenChosen)
	{
		if(deliverWeapon->hasBeenChosen)
		{
			if(!whatElse->hasBeenChosen)
			{
				nextDialog->add(whatElse);
			}
		}
		else
		{
			nextDialog->add(deliverWeapon);
		}
	}
	else
	{
		nextDialog->add(rolePEG);
	}
///////
/*	torture->setRate();

	if(!yingBaked())	
*/		DItem::doit();

}	   

void DItemLine3::doit()
{
	if(relationshipCohen->hasBeenChosen)
	{
		if(cohenKnow->hasBeenChosen)
		{
			if(!pickCohen->hasBeenChosen)
			{
				nextDialog->add(pickCohen);
			}
		}
		else
		{
			nextDialog->add(cohenKnow);
		}
	}
	else
	{
		nextDialog->add(relationshipCohen);
	}

////test
	if(workFor->hasBeenChosen)
	{
		if(writesChecks->hasBeenChosen)
		{
			if(!elseKnow->hasBeenChosen)
			{
				nextDialog->add(elseKnow);
			}
		}
		else
		{
			nextDialog->add(writesChecks);
		}
	}
	else
	{
		nextDialog->add(workFor);
	}
/// 

	if(rolePEG->hasBeenChosen)
	{
		if(deliverWeapon->hasBeenChosen)
		{
			if(!whatElse->hasBeenChosen)
			{
				nextDialog->add(whatElse);
			}
		}
		else
		{
			nextDialog->add(deliverWeapon);
		}
	}
	else
	{
		nextDialog->add(rolePEG);
	}

/*	torture->setRate();

	if(!yingBaked())	
*/
		DItem::doit();
}	   


void ScriptBeginQuestion::changeState( int newState )
{
	switchTo
		shockincrease->setCel(2);
		shockdecrease->setCel(2);
		shockmeter->setCel(0);
		minimumHeartRate = 70;
		theMovie->fromTo(2040, 2118);
		theMovie->play("25012.avi", this, 0, 0, 0);
//		ticks = 2;
	END
	BEG
//		torture->cue();
		theMovie->pause(false);

		dialogTree = new DialTree;
		tortureDialog0 = new Dialog;
		tortureDialog1 = new Dialog;
		tortureDialog2 = new Dialog;
		tortureDialog3 = new Dialog;
		tortureDialog4 = new Dialog;
		tortureDialog5 = new Dialog;
		tortureDialog6 = new Dialog;
		tortureDialog7 = new Dialog;
		tortureDialog8 = new Dialog;
		tortureDialog9 = new Dialog;
	
		relationshipCohen = new DItemLine1;
		relationshipCohen->addInfo(25001, RELATIONSHIPCOHEN, tortureDialog1);
		cohenKnow = new DItemLine1;
		cohenKnow->addInfo(25001, COHENKNOW, tortureDialog2);
		pickCohen = new DItemLine1;
		pickCohen->addInfo(25001, PICKCOHEN, tortureDialog3 );

		rolePEG = new DItemLine2;
		rolePEG->addInfo(25001, ROLEPEG, tortureDialog4 );
		deliverWeapon = new DItemLine2;
		deliverWeapon->addInfo(25001, DELIVERWEAPON, tortureDialog5);
		whatElse = new DItemLine2;
		whatElse->addInfo(25001, WHATELSE, tortureDialog6);

		workFor = new DItemLine3;
		workFor->addInfo(25001, WORKFOR, tortureDialog7);
		writesChecks = new DItemLine3;
		writesChecks->addInfo(25001, WRITESCHECKS, tortureDialog8 );
		elseKnow = new DItemLine3;
		elseKnow->addInfo(25001, ELSEKNOW, tortureDialog9 );

		//tortureDialog0->addMovieInfo(3050, 3250, 0, 29);   //16
		tortureDialog1->addMovieInfo(3945, 4132, 0, 29);   //16
		tortureDialog2->addMovieInfo(2490, 2572, 0, 29);	//29
		tortureDialog3->addMovieInfo(2595, 2730, 0, 29);	//30
		tortureDialog4->addMovieInfo(3480, 3592, 0, 29);	//31
		tortureDialog5->addMovieInfo(3165, 3283, 0, 29); 	//36
		tortureDialog6->addMovieInfo(2895, 2937, 0, 29);	//17
		tortureDialog7->addMovieInfo(4575, 4756, 0, 29);	//17
		tortureDialog8->addMovieInfo(3315, 3454, 0, 29);	//17
		tortureDialog9->addMovieInfo(3615, 3682, 0, 29);	//17

		switch(dialogCue)
		{
			case 1:
				rolePEG->hasBeenChosen = true;
				tortureDialog0->add(deliverWeapon);
				tortureDialog0->add(relationshipCohen);
				tortureDialog0->add(workFor);
				break;

			case 2:
				relationshipCohen->hasBeenChosen = true;	
				tortureDialog0->add(cohenKnow);
				tortureDialog0->add(rolePEG);
				tortureDialog0->add(workFor);
				break;
								
			case 3:
				//workFor->hasBeenChosen = true;
				tortureDialog0->add(workFor);
				tortureDialog0->add(relationshipCohen);
				tortureDialog0->add(rolePEG);
				break;
		}
								
		tortureDialog1->add(cohenKnow);

		tortureDialog2->add(pickCohen);

		tortureDialog3->add(deliverWeapon);

		tortureDialog4->add(deliverWeapon);

		tortureDialog5->add(whatElse);

		tortureDialog6->add(workFor);
   
		tortureDialog7->add(writesChecks);

		tortureDialog8->add(elseKnow);

		tortureDialog9->add(relationshipCohen);

		dialogTree->rootNode = tortureDialog0;
		dialogTree->resourceID = "25012.avi";
		dialogTree->show(this);
		currentFocus = PUZZLE_FOCUS;
		tortureFocus = YINGDIALOG;
	END
		
	BEG
		theGame->handsOff();
		sfxMadeCursor();
		monitor->dispose();
		theMovie->caller = 0;
		theMovie->stop();
		GameFlag.set(fBrokenYingInterrogation);
	//	GameFlag.set(fPyramidInfoAvailable);
		dispose();
		intrpuzz->quitIntrface();
	END
}

} // namespace Spycraft
