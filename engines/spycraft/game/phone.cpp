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

#include "spycraft/game/globals.h"
#include "spycraft/game/game.h"
#include "spycraft/game/verbs.h"
#include "spycraft/game/views.h"
#include "spycraft/game/movie.h"
#include "spycraft/game/flag.h"
#include "spycraft/game/phone.h"
#include "spycraft/game/teletalk.h"
#include "spycraft/logic/35000.h"
#include "spycraft/logic/35300.h"
#include "spycraft/logic/24000.h"
#include "spycraft/dmade/advcursor.h"

namespace Spycraft {

#define CRSBLINK_SPEED 5

// flags : fLinus15 : Blake call done.
// flags : Yuri call done. -> change to fOnce35001
// flags : fLinus17 : Check for call enable.
// flags : fLinus16 : used to lock out key press on key pad
// flags : fLinus18 : use for BS message waiting.
// flags : fWahKa82 : use for YURI message waiting.
// flags : fLinus19 : use for checking incoming calls
// flags : fLinus21 : use to lock out line access
// flags : fLinus26 : use to lock out hook during push
// flags : fLinus27 : use to lock out message during push
// flags : fLinus29 : use to set Activision message machine
// flags : fLinus30 : inside pass code
// flags : fLinus31 : inside command mode

extern BlinkCursor* normalCursor;
extern SouthCursor* southCursor;
extern Intrface* intrface;
extern int canSaveColby;

KeyPress	*keyPress;

NumberKeyFeature	*oneButton, *twoButton, *threeButton;
NumberKeyFeature	*fourButton, *fiveButton, *sixButton;
NumberKeyFeature	*sevenButton, *eightButton, *nineButton;
NumberKeyFeature	*zeroButton, *poundButton, *starButton;

KeyDisplay	*keyDisplay[12];

MessageBlink	*messageBlink;
MessageButton	*messageButton;
MessagePress	*messagePress;
HangUpButton 	*hangUpButton;
LineButton		*lineButton;
HangPress	  	*hangPress;
LinePress		*linePress;

int ExtMax = 17;

int LegalNumber1[15] 	= {0,1,1,4,4,1,7,1,5,5,5,0,9,0,9};	// Blake number
int LegalNumber2[7] 	= {2,3,3,4,8,1,9};					// Yuri number
int LegalNumber3[7] 	= {9,7,6,2,8,8,8}; 
int LegalNumber4[11] 	= {1,3,1,0,4,7,3,9,2,0,0};			// Activision number
int InputNumber[15] 	= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int CountryCall[3] 		= {0,1,1};
int AccountNumber[7]	= {1,2,3,4,5,6,7};					// Account number
int PassCode[4]			= {1,2,3,4};
int Extention[17][4]	= {	{2,6,5,5},	//1: me				// OFFICE EXTENTIONS
							{2,6,9,7},	//2: wyndam
							{2,2,2,8},	//3: yaz
							{2,2,2,7},	//4: pat
							{7,7,7,7},	//5: ray
							{2,5,6,7},	//6: larry
							{2,6,1,6},	//7: kawah
							{2,2,4,6},	//8: greg
							{2,2,4,2},	//9: doug
							{2,5,4,1},	//10: derek
							{2,5,7,0},	//11: daniel
							{2,5,8,1},	//12: coto
							{2,2,4,1},	//13: tom
							{7,7,7,6},	//14: sung (15)
							{2,2,2,1},	//15: andrew
							{2,6,0,5},	//16: budlong
							{4,5,1,3}	//17: aaron
						};

int keyCount, i, j, result1, result2, result3, result4;
int wherePhone, lastDigit, displayLimit;
int ExtMessage = 999;

PhoneKeyPad::PhoneKeyPad(int location)
{
	name = "PhoneKeyPad";
	show_style = VE_FLIP;

	wherePhone = location;
	lastDigit = 7;
	displayLimit = 11;
}

PhoneKeyPad::~PhoneKeyPad()
{
	if (!GameFlag.test(fLinus19))
	{
		oneButton->dispose();
		twoButton->dispose();
		threeButton->dispose();
		fourButton->dispose();
		fiveButton->dispose();
		sixButton->dispose();
		sevenButton->dispose();
		eightButton->dispose();
		nineButton->dispose();
		zeroButton->dispose();
		poundButton->dispose();
		starButton->dispose();
	}else GameFlag.clear(fLinus19);
	canSaveColby = true;
}

void PhoneKeyPad::init()
{
  	drawPic( 1300 );
    Room::init();
	keyCount = 0;
	intrface->disable();
	canSaveColby = false;
	
	//if(strcmp(curRoom->name, "35000.avi") == 0)
	sound1->stop(); //2/22

	theMusic->stop();

//	GameFlag.set(fBeenTo57000);
//	GameFlag.set(fPlayBSDeed);
//	GameFlag.set(fYORoomEnter);
//	GameFlag.set(fBSTrue);
	theGame->handsOff();

/*	if (GameFlag.test(fBeenTo57000)  && !GameFlag.test(fWahKa72) && GameFlag.test(fPlayBSDeed))
			GameFlag.set ( fLinus18 ); // message waiting

	if (wherePhone == FROM_RUSSIA)
	{
		if (GameFlag.test(fYORoomEnter) && !GameFlag.test(fVCRoomEnter) && !GameFlag.test(fWahKa51))
			GameFlag.set ( fWahKa82 ); // second message
	}
*/

	if (GameFlag.test( fLinus18 ) || GameFlag.test( fWahKa82 ))
		if (wherePhone == FROM_RUSSIA)
			messageBlink = new MessageBlink;

//	if((GameFlag.test(fVCRoomEnter) && GameFlag.test(fDoneSoundAnal) && !GameFlag.test(fWahKa52)) ||	//Lean

	if((GameFlag.test(fOnyxHunt1)  && !GameFlag.test(fWahKa53) && !GameFlag.test(fPlayerTurnsProcat) && GameFlag.test(fMetBlake) && (wherePhone == FROM_RUSSIA)) ||				//Yuri about Onyx
	   (GameFlag.test(fBrokenYingInterrogation) && GameFlag.test(fOnyxHunt2) && !GameFlag.test(fKilledGrendel) && !GameFlag.test(fWahKa38) && !GameFlag.test(fWahKa54) && (wherePhone == FROM_RUSSIA))) {  	//Harmonica
		GameFlag.set(fLinus19);
		curRoom->setScript ( new IncomingCallScript );	
	}
	else 
	{
		theGame->handsOn();

	//	sound1->stop();
	//	theMusic->stop();

		hangUpButton 	= new HangUpButton;
		messageButton 	= new MessageButton;
		lineButton		= new LineButton;

		new SouthExitPhone;  

		oneButton 	= new NumberKeyFeature(1);
		twoButton 	= new NumberKeyFeature(2);
		threeButton = new NumberKeyFeature(3);
		fourButton 	= new NumberKeyFeature(4);
		fiveButton 	= new NumberKeyFeature(5);
		sixButton 	= new NumberKeyFeature(6);
		sevenButton = new NumberKeyFeature(7);
		eightButton = new NumberKeyFeature(8);
		nineButton 	= new NumberKeyFeature(9);
		zeroButton  = new NumberKeyFeature(0);
		poundButton = new NumberKeyFeature(10);
		starButton  = new NumberKeyFeature(11);

		keyDisplay[0] = new KeyDisplay(0);
		keyDisplay[1] = new KeyDisplay(1);
		keyDisplay[2] = new KeyDisplay(2);
		keyDisplay[3] = new KeyDisplay(3);
		keyDisplay[4] = new KeyDisplay(4);
		keyDisplay[5] = new KeyDisplay(5);
		keyDisplay[6] = new KeyDisplay(6);
		keyDisplay[7] = new KeyDisplay(7);
		keyDisplay[8] = new KeyDisplay(8);
		keyDisplay[9] = new KeyDisplay(9);
		keyDisplay[10] = new KeyDisplay(10);
		keyDisplay[11] = new KeyDisplay(11);
	}
}

int PhoneKeyPad::handleEvent ( MADEEventStamp *event )
{
	return false;
}

SouthExitPhone::SouthExitPhone()
{
	name = "SouthExitPhone";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);
}

void SouthExitPhone::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExitPhone::doVerb ( int theVerb )
{
	if (( theVerb == DO_V ) && !GameFlag.test(fLinus16) && !GameFlag.test(fLinus26))
	{
		intrface->enable();
		sound1->stop();
		GameFlag.clear(fLinus21);
		if (wherePhone == FROM_RUSSIA) {
			/*if( sound1->number != 35000  || !sound1->isPlaying() )	{
				sound1->loop = -1; 
				sound1->playBuffered(35000, 0);
			}*/
			theGame->newRoom(new Rm35100);
		}
		else theGame->newRoom(new Rm24100);

    	return true;
	}
	return false;
}

/******************************
 *
 *		Features
 *
 ******************************/
LineButton::LineButton()
{
	name = "LineButton";
	setHotspot( DO_V );
 	init( 213, 221, 75, 20 );
}

void LineButton::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink(CRSBLINK_SPEED);
}

int LineButton::doVerb( int theVerb )
{
	if ( theVerb == DO_V ) {
		if (!GameFlag.test(fLinus21) && !GameFlag.test(fLinus26))
		{
			GameFlag.set(fLinus21);
			curRoom->setScript ( new LineScript );
		}
	 	return true;
	}
	return false;
}

MessageButton::MessageButton()
{
	name = "MessageButton";
	setHotspot( DO_V );
 	init( 213, 190, 75, 20 );
}

void MessageButton::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink(CRSBLINK_SPEED);
}

int MessageButton::doVerb( int theVerb )
{
	if (( theVerb == DO_V ) && (wherePhone == FROM_RUSSIA) && 
		!GameFlag.test(fLinus26) && !GameFlag.test(fLinus16) && !GameFlag.test(fLinus21))
	{
		if (GameFlag.test(fLinus18) || GameFlag.test(fWahKa82))
		{	
			theGame->handsOff();
			messageBlink->dispose();
			curRoom->setScript ( new MessageScript(-1) );
	 	}
		return true;
	}
	return false;
}

HangUpButton::HangUpButton()
{
	name = "HangUpButton";
	setHotspot( DO_V );
 	init( 214, 256, 75, 20 );
}

void HangUpButton::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink(CRSBLINK_SPEED);
}

int HangUpButton::doVerb( int theVerb )
{
	if ( theVerb == DO_V ) {
		if (!GameFlag.test(fLinus16) && !GameFlag.test(fLinus26))
		{
			GameFlag.set(fLinus26);
			keyCount = 0;
			curRoom->setScript( new HangButtonScript );
		}
		return true;
	}
	return false;
}

NumberKeyFeature::NumberKeyFeature( int Mynum )
{
	name = "NumberKeyFeature";
	setHotspot( DO_V );
	keynum = Mynum;
	
	switch( Mynum )
	{
		case 1 : init( 316, 220, 42, 22 );
				break;
		case 2 : init( 368, 220, 42, 22 );
				break;
		case 3 : init( 420, 220, 42, 22 );
				break;
		case 4 : init( 318, 256, 42, 22 );
				break;
		case 5 : init( 370, 256, 42, 22 );
				break;
		case 6 : init( 423, 256, 42, 22 );
				break;
		case 7 : init( 320, 292, 42, 22 );
				break;
		case 8 : init( 373, 292, 42, 22 );
				break;
		case 9 : init( 426, 292, 42, 22 );
				break;
		case 0 : init( 375, 329, 42, 22 );
				break;
		case 10 : init ( 429, 329, 42, 22 );  	// pound
				break;
		case 11 : init ( 322, 329, 42, 22 );	// star
				break;
	}
}

void NumberKeyFeature::respond()
{
	theGame->setCursor( normalCursor );
	normalCursor->blink(CRSBLINK_SPEED);
}

int NumberKeyFeature::doVerb( int theVerb )
{
	if ( theVerb == DO_V ) {
		if (!GameFlag.test( fLinus16 ) && GameFlag.test( fLinus21 ) && !GameFlag.test(fLinus26))
		{
			GameFlag.set( fLinus16 );
			InputNumber[keyCount] = keynum;
			curRoom->setScript ( new KeyPressScript(keynum) );
	   	}
		return true;
	}
	return false;
}


/*********************
 *
 *	Views
 *
 *********************/
LinePress::LinePress()
{
 	name = "LinePress";
	setCel(0);
	setView(1330);
	init(213,221);
	noSkip();
}

MessagePress::MessagePress()
{
 	name = "MessagePress";
	setCel(0);
	setView(1330);
	init(213,186);
	noSkip();
}

MessageBlink::MessageBlink()
{
 	name = "MessageBlink";
	
	cycleSpeed = 8;	
	view = 1340;
	loop = 0;
	cel = 0;
	init(295,326);

	setCycle(new EndLoop((Prop*)this, this));
}

void MessageBlink::cue(void)
{
	setCycle(new EndLoop((Prop*)this, this));
}

HangPress::HangPress()
{
 	name = "HangPress";
	setCel(2);
	setView(1330);
	init(214,256);
	noSkip();
}

KeyDisplay::KeyDisplay(int place)
{
	name = "KeyDisplay";
	setCel(0);
	setView(1320);
	
	switch ( place )
	{
		case 0 : init(410, 70); 
			break;
		case 1 : init(380, 70);
			break;
		case 2 : init(350, 70);
			break;
		case 3 : init(320, 70);
			break;
		case 4 : init(290, 70);
			break;
		case 5 : init(260, 70);
			break;
		case 6 : init(230, 70);
			break;
		case 7 : init(200, 70); 
			break;
		case 8 : init(170, 70);
			break;
		case 9 : init(140, 70);
			break;
		case 10 : init(110, 70);
			break;
		case 11 : init(80, 70);
			break;
	}
 	hide();
}

KeyPress::KeyPress(int number)
{
 	name = "KeyPress";

	setCel(number);
	setView(1310); 
	switch ( number )
	{
		case 1 : init(316, 220);
			break;
		case 2 : init(368, 220);
			break;
		case 3 : init(420, 220);
			break;
		case 4 : init(318, 256);
			break;	
		case 5 : init(370, 256);
			break;
		case 6 : init(423, 256);
			break;
		case 7 : init(320, 292);
			break;
		case 8 : init(373, 292);
			break;	
		case 9 : init(426, 292);
			break;
		case 0 : init(375, 329);
			break;
		case 10 : init (429, 329);		// pound
			break;
		case 11 : init (322, 329);	  	// star
			break;
	}
	noSkip();
}

/*********************
 *
 * 	Scripts
 *
 *********************/

// Script to control the bank auto teller
void BankService::changeState ( int newState )
{
 	switchTo
		BankVoice->play ( 150000, this );
	END
	
	BEG
		BankVoice->dispose();
		lastDigit = 4;
		keyCount = 0;
		theGame->handsOn();
		dispose();
	END

	BEG
		theGame->handsOff();
	 	for(i = 0; i < ExtMax; i++)
		{
			for(j = 0; j < 4; j++)
			{
		 		if (Extention[i][j] != InputNumber[j])
				{
				//	sfxPrintf("That account is invalid!");
					ExtMessage = 999;	// Bad Ext
					break;
				}
				else ExtMessage = i;
			}
			if (ExtMessage == i)
				break;
		}
		switch ( ExtMessage )
		{
			case 0 : BankVoice->play ( 150001, this ); 
					break;
			case 1 : BankVoice->play ( 150002, this );
					break;
			case 2 : BankVoice->play ( 150003, this );
					break;
			case 3 : BankVoice->play ( 150004, this );
					break;
			case 4 : BankVoice->play ( 150005, this );
					break;
			case 5 : BankVoice->play ( 150006, this );
					break;
			case 6 : BankVoice->play ( 150007, this );
					break;
			case 7 : BankVoice->play ( 150008, this );
					break;
			case 8 : BankVoice->play ( 150009, this );
					break;
			case 9 : BankVoice->play ( 150010, this );
					break;
			case 10 : BankVoice->play ( 150011, this );
					break;
			case 11 : BankVoice->play ( 150012, this );
					break;
			case 12 : BankVoice->play ( 150013, this );
					break;
			case 13 : BankVoice->play ( 150015, this );
					break;
			case 14 : BankVoice->play ( 150017, this );
					break;
			case 15 : BankVoice->play ( 150018, this );
					break;
			case 16 : BankVoice->play ( 150019, this );
					break;
			case 999 : curRoom->setScript ( new BankService ( 2 ));
					break;
		}
	END

//	BEG
//	   	curRoom->setScript ( new BankService ( -1 ));
//	END

	BEG
		ticks = 2;
	END

	BEG
	   	BankVoice->play ( 150014, this );
	END

	BEG
	   	ticks = 2;
	END

	BEG
		BankVoice->dispose();
		theGame->handsOn();
		keyCount = 0;
		lastDigit = 7;
		GameFlag.clear(fLinus29);
		GameFlag.clear(fLinus21);
		dispose();	
	END
}

// Script to play the phone tone sound
void LineScript::changeState ( int newState )
{
	switchTo
 	   	linePress = new LinePress;
		ticks = 3;
	END

 	BEG
		linePress->dispose();
		sound1->loop = SND_LOOP_INFINITE;
		sound1->play( 1307, NULL );
		ticks = 1;
	END

	BEG
	   	dispose();
	END
}

// Script to control incoming calls
void IncomingCallScript::changeState ( int newState )
{
 	switchTo
//		sound1->stop();
/*	   	if (GameFlag.test(fVCRoomEnter) && GameFlag.test(fDoneSoundAnal) && !GameFlag.test(fWahKa52))
		{	
			GameFlag.set(fWahKa52);
			GameFlag.set(fWahKa10);
			GameFlag.set(fWahKa20);
	   		GameFlag.set(fLenaCalls);
			GameFlag.set(fMoscowAlleyAvailable);
			callSound->play ( 35108, this );
		} else */
		if(GameFlag.test(fOnyxHunt1)  && !GameFlag.test(fWahKa53) && !GameFlag.test(fPlayerTurnsProcat) && GameFlag.test(fMetBlake))  //Yuri about Onyx
		{
			GameFlag.set(fWahKa53);
			GameFlag.set(fYuriAboutOnyx);		// Yuri calls about onyx file
			GameFlag.set(fDCK2);				//make Yasevno available in travelink
			callSound->play ( 35111, this );
		} else if (GameFlag.test(fBrokenYingInterrogation) && GameFlag.test(fOnyxHunt2) && !GameFlag.test(fKilledGrendel) && !GameFlag.test(fWahKa38) && !GameFlag.test(fWahKa54))  	//Harmonica
		{
			GameFlag.set(fWahKa54);
			GameFlag.set(fWahKa11);
			GameFlag.set(fWahKa21);				// Harmonica calls
			GameFlag.set(fHarmCalls);
			GameFlag.set(fMoscowAlleyAvailable);
			callSound->play ( 35110, this );
		}
	END

	BEG
		ticks = 2;
	END
	
	BEG
	   	callSound->dispose();
		intrface->enable();
		theGame->handsOn();
		if (wherePhone == FROM_RUSSIA) {
			/*if( sound1->number != 35000  || !sound1->isPlaying() )	{
				sound1->loop = -1; 
				sound1->playBuffered(35000, 0);
			}*/
			theGame->newRoom(new Rm35100);
		}
		else theGame->newRoom(new Rm24100);
	END
}

void MessageScript::changeState ( int newState )
{
 	switchTo
		messagePress = new MessagePress;
		ticks = 2;
	END

	BEG
	   	messagePress->dispose();
		phoneSound->play ( 1306, this );
	END

	BEG
	    ticks = 2;
	END

/*	BEG
	   	if(GameFlag.test(fLinus18))
			phoneSound->play( 1324, this);
		else cue();
	END

	BEG
	   	ticks = 2;
	END	*/

	BEG
		if(GameFlag.test(fLinus18))
		{    
			GameFlag.clear(fLinus18);
			if (GameFlag.test(fBSTrue)) 
            {
                GameFlag.set(fWahKa72);
				GameFlag.set(fBSVoiceMessage);		 	// Birdsong message 
				phoneSound->play( 35126, this );
			}
			else if(GameFlag.test(fBetrayMaxKneecaps)) {
				GameFlag.set(fWahKa72);
				GameFlag.set(fBSVoiceMessage);
				phoneSound->play( 35125, this );
			}
			else {
				GameFlag.set(fWahKa72);
			//	sfxPrintf("this is a bug-no Ditem selected.");          //DEBUG wahka
				cue();	
			}
		}else cue();
	END

	BEG
	   	ticks = 2;
	END

/*	BEG
	   	if (GameFlag.test(fWahKa82))
			phoneSound->play ( 1324, this );
		else cue();
	END

	BEG
	   	ticks = 2;
	END	*/

	BEG
		if(GameFlag.test(fWahKa82))
		{
			GameFlag.clear(fWahKa82);
			GameFlag.set(fWahKa51);
			GameFlag.set(fOnce35000);
			phoneSound->play ( 35109, this );
		}else cue();
	END

// Check for beat

	BEG
   		if (GameFlag.test(fBeenTo57000)  && !GameFlag.test(fWahKa72) && GameFlag.test(fPlayBSDeed))
		{
			GameFlag.set(fLinus18);
			curRoom->setScript ( new MessageScript(1));
		}else cue();	
	END

	BEG
		theGame->handsOn();
		phoneSound->dispose();
		dispose();
	END 
}

void HangButtonScript::changeState ( int newState )
{
	switchTo
		sound1->stop();
		ticks = 2;
	END

	BEG
		keyCount = 0;
		lastDigit = 7;
		hangPress = new HangPress;
		hangSound->play ( 1304, this );
	END

	BEG
	   	ticks = 2;
	END

	BEG
	   	hangPress->dispose();
		cue();
	END

	BEG
	   	for(j = 0; j <= displayLimit; j++)
		{
			keyDisplay[j]->hide();
		}			
		cue();	
	END

	BEG
		GameFlag.clear(fLinus21);
		GameFlag.clear(fLinus26);
		GameFlag.clear(fLinus29);
			dispose();
	END
}

void DialingProcedure::changeState ( int newState )
{
	switchTo
		theGame->handsOff();
		phoneSound->play ( 1320, this );
	END

	BEG
	   	ticks = 4;
	END
	
	BEG
	result1 = 0;
	result2 = 0;
	result3 = 0;
	result4 = 0;
						 
	if ( lastDigit == 7 )
	{
		result1 = 1;
		result4 = 1;
		for ( i = 0; i < lastDigit; i++ )
		{
			if (LegalNumber2[i]	!= InputNumber[i])
			 	result2 = 1;
			if (LegalNumber3[i]	!= InputNumber[i])
			 	result3 = 1;
		}
	} else if ( lastDigit == 11)
	{
		result1 = 1;
		result2 = 1;
		result3 = 1;
	 	for ( i = 0; i < lastDigit; i++ )
		{
			if (LegalNumber4[i] != InputNumber[i])
				result4 = 1;
	 	}
	}
	else 
	{
		result4 = 1;
		result2 = 1;
		result3 = 1;
	 	for ( i = 0; i < lastDigit; i++ )
		{
			if (LegalNumber1[i] != InputNumber[i])
				result1 = 1;
	 	}
	}

	if ((result1 == 0) && GameFlag.test(fSawBlakeDossier) && !GameFlag.test(fLinus15))
	{
		theMovie->stop();
		GameFlag.set ( fLinus15 );
		sfxSystemCursor ( 10 );
		for(j = 0; j <= displayLimit; j++)
		{
			keyDisplay[j]->hide();
		}			
	   	phoneSound->dispose();
		keyCount = 0;
		GameFlag.clear(fLinus21);

		sfxSystemCursor(10);
		curRoom->setScript ( new BlakeTeletalkScript );
	}
	else if (((result2 + wherePhone) == 0) && !GameFlag.test(fOnce35001) && GameFlag.test(fWahKa41))
	{
		theMovie->stop();
		GameFlag.set ( fOnce35001 );
		GameFlag.set ( fDCK2 );				//make Yasevno available in travelink
		sfxSystemCursor ( 10 );
		for(j = 0; j <= displayLimit; j++)
		{
			keyDisplay[j]->hide();
		}			
		phoneSound->dispose();
		keyCount = 0;
		GameFlag.clear(fLinus21);

		sfxSystemCursor(10);
		curRoom->setScript ( new EnterScriptYuriVO35000 );
	}
	else if (result3 == 0)				// Beavis
	{
		GameFlag.clear(fLinus21);
	 	phoneSound->play( 656, this );
	}
	else if (result4 == 0)				// Activision number
	{
		GameFlag.set( fLinus29 );
		for(j = 0; j <= displayLimit; j++)
		{
			keyDisplay[j]->hide();
		}			
	   	phoneSound->dispose();
	 	curRoom->setScript ( new BankService (-1) );
	}
	else 
	{
		GameFlag.clear(fLinus21);		// wrong number
		phoneSound->play ( 1301, this );
	}
	END

	BEG
		for(j = 0; j <= displayLimit; j++)
		{
			keyDisplay[j]->hide();
		}			
	   	phoneSound->dispose();
		keyCount = 0;
		theGame->handsOn();
	END
}			

KeyPressScript::KeyPressScript ( int Mynum )
{
 	name = "KeyPressScript";
	tone = new Sound;
	keynum = Mynum;
}

void KeyPressScript::changeState ( int newState )
{
 	switchTo
		sound1->stop();
		keyPress = new KeyPress(keynum);
		ticks = 3;
	END

	BEG
		if ((InputNumber[keyCount] < 10) && !GameFlag.test( fLinus29 ))
		{	
			if (keyCount > 11)
			{	
				for(j = 0; j <= displayLimit; j++)
				{
					keyDisplay[j]->setCel(InputNumber[keyCount - j]);
					keyDisplay[j]->show();
				}
		  	}else
			{
				for(j = 0; j <= keyCount; j++)
				{
					keyDisplay[j]->setCel(InputNumber[keyCount - j]);
					keyDisplay[j]->show();
				}
			}
		}
		switch(keynum)
		{
			case 1 : tone->play( 1311, this);
						break;
			case 2 : tone->play( 1312, this);
						break;
			case 3 : tone->play( 1313, this);
						break;
			case 4 : tone->play( 1314, this);
						break;
			case 5 : tone->play( 1315, this);
						break;
			case 6 : tone->play( 1316, this);
						break;
			case 7 : tone->play( 1317, this);
						break;
			case 8 : tone->play( 1316, this);
						break;
			case 9 : tone->play( 1314, this);
						break;
			case 0 : tone->play( 1312, this);
						break;
			case 10 : tone->play( 1311, this);
						break;
			case 11 : tone->play( 1311, this);
						break;
		}
	END

	BEG
		keyPress->dispose();
		GameFlag.clear( fLinus16 );
		if (keynum < 10)
			keyCount++;
		if (keyCount == 3)
		{
		 	for(i = 0; i < 3; i++)
			{
			 	if (CountryCall[i] != InputNumber[i])
				{
					lastDigit = 7;
					break;
				}
				else lastDigit = 15;
			}
		}
		if (GameFlag.test(fLinus29))
		{
	//	 	sfxPrintf("ld is 4, key count is: %d",keyCount);
			lastDigit = 4;
		}
		else if ((InputNumber[0] == 1) && wherePhone == FROM_USA)
			lastDigit = 11;
	
		if (keyCount == lastDigit)
			if (GameFlag.test( fLinus29 ))
				curRoom->setScript(new BankService ( 1 ));
			else curRoom->setScript(new DialingProcedure);
		else dispose();
	END
}

} // namespace Spycraft
