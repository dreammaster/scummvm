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

#include "spycraft/game/verbs.h"
#include "spycraft/game/game.h"
#include "spycraft/game/pcscreen.h"
#include "spycraft/game/plistobj.h"
#include "spycraft/game/interface.h"
#include "spycraft/game/web.h"
#include "spycraft/game/globals.h"
#include "spycraft/game/image.h"
#include "spycraft/game/sndanal.h"						   
#include "spycraft/game/kat.h"
#include "spycraft/game/cypher.h"
#include "spycraft/game/phodoc.h"

namespace Spycraft {

#define CRSBLINK_SPEED	10
#define NUMBUTTONSPOTS	7
#define BUTTON_Y		295
#define BUTTON_WIDTH	48
#define BUTTON_HEIGHT	48

//extern Intrface*	intrface;
extern BlinkCursor *normalCursor;
extern SouthCursor *southCursor;

extern WebHome *homobutt;
extern int isDemo;

short	pcLastUsed[PC_MAXUSEDTYPE];

static int	buttonSpots[NUMBUTTONSPOTS];
static int	curButtonType;		// communication between button feature and script
static int	blinkOK;			// flag: buttons may blink.
static PCScreen *theScreen;	// allows timer to notice that PCScreen is gone
static PCScreenUpdateTimer *pcTimer;	// allows buttons to find the list in the timer

/*******************************
 *
 *	PC Screen
 *
/*******************************/

PCScreen::PCScreen(int roomNum) {
	roomNumber = roomNum;
	init();
}

PCScreen::PCScreen(int roomNum, Object *whoToCue) {
	roomNumber = roomNum;
	Inset::whoToCue = whoToCue;
	init();
}

void PCScreen::init(void) {
	int	atsNum;

	GameFlag.set(fUsingWorkstation);
	theScreen = this;
	blinkOK = true;
	curButtonType = -1;	// init to invalid
	sfxSystemCursor(SYS_WAIT);

	switch (roomNumber)
	{
	case 5000:
	case 5100:
		atsNum = 92010;
		break;
	case 24100:
		atsNum = 92020;
		break;
	case 35100:
		atsNum = 92030;
		break;
	default:
		sfxPrintf("Unknown Room number %d", roomNumber);
		atsNum = 92010;
	}
	Inset::init(atsNum, 0, 16, 512, 384);
	if (roomNumber == 5100)
		insetView->addToPic(); // special case for 5100 which does no drawPic
	// Init X location:
	buttonSpots[5] = 55;
	buttonSpots[3] = 114;
	buttonSpots[1] = 174;
	buttonSpots[0] = 233;	// middle
	buttonSpots[2] = 293;
	buttonSpots[4] = 352;
	buttonSpots[6] = 411;
	ASSERT(NUMBUTTONSPOTS == 7, 0);

	addTheButtons();
	drawButtons();
	sfxMadeCursor();
	new PCScreenUpdateTimer(this);
	cue();
}

void PCScreen::dispose(void) {
	buttons.dispose();
	theScreen = NULL;
	if (whoToCue != NULL)
	{
		new PCDelayedCallBack(whoToCue);
		whoToCue = NULL;	// don't want inset to call back--too soon
	}
	Inset::dispose();
}

void PCScreen::respond(void) {
	theGame->setCursor(normalCursor);
	normalCursor->rest();
}

void PCScreen::addTheButtons(void) {
#define GFT GameFlag.test
	// Add buttons:
	if (pcLastUsed[PC_INTRLINK])
		addButton(PC_INTRLINKBUTTON);
	if (pcLastUsed[PC_IA])
		addButton(PC_IABUTTON);
	if (pcLastUsed[PC_SNDANAL])
		addButton(PC_SOUNDANALBUTTON);
	//if( roomNumber != 35100  || GameFlag.test( fShowSwanTwo ) )
	if (pcLastUsed[PC_CIPHER])
		addButton(PC_CIPHERBUTTON);
	if (pcLastUsed[PC_PHOTODOC])
		addButton(PC_PHOTODOCBUTTON);
	if (pcLastUsed[PC_ECLIPSE])
		addButton(PC_ECLIPSEBUTTON);
	if (GFT(fGotNewElectBook) || GFT(fGotInitialElectBook) || GFT(fGotOldElectBook))
		addButton(PC_EBMCBUTTON);
	if (roomNumber == 24100)
	{
		if (GameFlag.test(fSecurityModelAvailable) || isDemo)
			addButton(PC_INFOSCIBUTTON);
		if (pcLastUsed[PC_PEGID])
			addButton(PC_PEGIDBUTTON);
		if (pcLastUsed[PC_KAT])
			addButton(PC_KATBUTTON);
	}
}

void PCScreen::drawButtons(void) {
	int		i;
	int		max;
	Node *nptr;
	PCButtonView *buttPtr;

	/* current buttons */

	max = (buttons.size < NUMBUTTONSPOTS) ? buttons.size : NUMBUTTONSPOTS;
	for (nptr = buttons.head, i = 0; i < max; ++i, nptr = nptr->next)
	{
		buttPtr = (PCButtonView *)(nptr->data);
		buttPtr->setHotspot(DO_V);
		buttPtr->setView(buttPtr->atsNumber);
		buttPtr->init(buttonSpots[i], BUTTON_Y);	// View::init()
		buttPtr->noSkip();
		buttPtr->setPri(402);
		buttPtr->myFeature = new PCButtonInsetFeature;
		buttPtr->myFeature->init(buttonSpots[i], BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
		buttPtr->myFeature->buttType = buttPtr->buttType;
	}
}

void PCScreen::addButton(int button) {
	if (buttons.size == NUMBUTTONSPOTS)	// full
		return;

	switch (button)
	{
	case PC_INTRLINKBUTTON:
		buttons.add(new PCButtonView(button, 92051));
		break;
	case PC_IABUTTON:
		buttons.add(new PCButtonView(button, 92052));
		break;
	case PC_KATBUTTON:
		buttons.add(new PCButtonView(button, 92053));
		break;
	case PC_INFOSCIBUTTON:
		buttons.add(new PCButtonView(button, 92054));
		break;
	case PC_SOUNDANALBUTTON:
		buttons.add(new PCButtonView(button, 92055));
		break;
	case PC_CIPHERBUTTON:
		buttons.add(new PCButtonView(button, 92056));
		break;
	case PC_PHOTODOCBUTTON:
		buttons.add(new PCButtonView(button, 92057));
		break;
	case PC_PEGIDBUTTON:
		buttons.add(new PCButtonView(button, 92058));
		break;
	case PC_ECLIPSEBUTTON:
		buttons.add(new PCButtonView(button, 92059));
		break;
	case PC_EBMCBUTTON:
		buttons.add(new PCButtonView(button, 92040));
		break;
	default:
		sfxPrintf("Unknown button");
	}
}

void PCScreen::cue(void) {
	// set the blink state of all the buttons
	int		i;
	Node *nptr;
	PCButtonView *buttPtr;

	if (currentFocus != INSET_FOCUS)
		return;					// we're probably in web somewhere--don't need update
	/* current buttons */
	for (nptr = buttons.head, i = 0; i < buttons.size; ++i, nptr = nptr->next)
	{
		ASSERT(nptr != NULL, 0);
		buttPtr = (PCButtonView *)(nptr->data);
		if (buttPtr->blinkOn())	// blink on
		{
			if (buttPtr->blinker == NULL)
				buttPtr->blinker = new PCButtonBlinker(buttPtr);
		} else if (buttPtr->blinker != NULL)	// blink off
		{
			buttPtr->blinker->stop();
			buttPtr->blinker = NULL;
		}
	}
}

/*******************************
 *
 *	Views
 *
/*******************************/

PCButtonView::PCButtonView(int button, int atsNum) {
	name = "PCButtonView";
	buttType = button;
	atsNumber = atsNum;
	blinker = NULL;
}

PCButtonView::~PCButtonView() {
	if (myFeature != NULL)
	{
		myFeature->dispose();
		if (blinker)
			blinker->kill();
	}
	cast->release(this);
}

int PCButtonView::blinkOn(void) {
	int	result = false;

	switch (buttType)
	{
	case PC_INTRLINKBUTTON:
		result = comlinks && !comlinks->isEmpty();
		break;

	case PC_IABUTTON:
		result = !strcmp("Rm5100", curRoom->name);	// in Rm5100
		break;

	case PC_INFOSCIBUTTON:
	case PC_KATBUTTON:
	case PC_SOUNDANALBUTTON:
	case PC_CIPHERBUTTON:
	case PC_PHOTODOCBUTTON:
	case PC_ECLIPSEBUTTON:
		result = false;
		break;

	default:
		result = false;
	}

	return result;
}

/*******************************
 *
 *	Inset Features
 *
/*******************************/

PCButtonInsetFeature::PCButtonInsetFeature() {
	name = "PCButtonInsetFeature";
	setHotspot(DO_V);
	// init() called elsewhere
}

void PCButtonInsetFeature::respond() {
	theGame->setCursor(normalCursor);
	normalCursor->blink(CRSBLINK_SPEED);
}

int PCButtonInsetFeature::doVerb(int theVerb) {
	if (theVerb == DO_V) {
		curButtonType = buttType;
		new PCButtonPressScript;
		return true;
	}
	return false;
}

/*******************************
 *
 *	Scripts
 *
/*******************************/

// Note: delayed callback needed because of a quirk of inset dispose

PCDelayedCallBack::PCDelayedCallBack(Object *whoToCue) {
	PermListObject *plobj;

	name = "PCDelayedCallBack";
	plobj = new PermListObject;
	plobj->setScript(this, whoToCue);
}

void PCDelayedCallBack::changeState(int newState) {
	switchTo
		user->canInput(false);
	ticks = 1;
	END

		BEG
		user->canInput(true);
	caller->cue();
	caller = NULL;	// unnecessary but safe
	client->dispose();	// gets rid of perm list obj and this too
	END
}

PCButtonPressScript::PCButtonPressScript() {
	name = "PCButtonPressScript";
	init(this, NULL, NULL);
}

void PCButtonPressScript::changeState(int newState) // launch Web and puzzle
{
	Web *thisWeb;
	static int	autoExit;		// flag: shut down pcscreen on return from web
	static int	roomMidi;
	static int	puzzMidi;
	PermListObject *plobj;

	switchTo
		normalCursor->rest();
	autoExit = false;
	roomMidi = -1;
	puzzMidi = -1;
	GameFlag.set(fUsingWorkstation);
	// start Web
	if (curButtonType != PC_KATBUTTON)
	{
		thisWeb = new Web;
		thisWeb->caller = this;
	}
	// select and start puzzle
	switch (curButtonType)
	{
	case PC_INTRLINKBUTTON:
		new Home;
		break;
	case PC_IABUTTON:
		switch (pcLastUsed[PC_IA])
		{
		case WEB_IMAGEPLATE:
			new HTMLPage(99169);
			comlinkshort->hide();
			autoExit = true;
			//new ImageAnalPlate;
			break;
		case WEB_IMAGETANK:
			new HTMLPage(99170);
			comlinkshort->hide();
			autoExit = true;
			//new ImageAnalTank;
			break;
		case WEB_IMAGECOURIER:	// no longer used (was Ames)
			new HTMLPage(99171);
			comlinkshort->hide();
			autoExit = true;
			//new ImageAnalCourier;
			break;
		case WEB_IMAGEPEG:
			new ImageAnalPeg;
			break;
		case WEB_IMAGEKNEEID:
			new ImageAnalCaps;
			break;
		default:
#ifdef _DEBUG
			sfxPrintf("Bad last image anal type");
#endif
			NULL;
		}
		homobutt->setCel(2);	// deactivates Home button in Web
		break;
	case PC_KATBUTTON:
		//new KAT;
		//new HTMLPage( 99104 );
		if (theMusic->isPlaying())
			roomMidi = theMusic->number;
		setScript(new PlayKATScriptPC, this);	// begins with a movie
		break;
	case PC_INFOSCIBUTTON:
		//if( theMusic->isPlaying() )
		//	roomMidi = theMusic->number;
		//puzzMidi = 600; 
		GameFlag.set(fGreg12);	// for report dialog box
		new HTMLPage(99900);
		break;
	case PC_SOUNDANALBUTTON:
		new Sndanal(0);	// Birdsong
		break;
	case PC_CIPHERBUTTON:
		switch (pcLastUsed[PC_CIPHER])
		{
		case WEB_CYPHERPITONE:
			//new CypherPitOne;
			//break;
		case WEB_CYPHERPITTWO:
			//new CypherPitTwo;
			//break;
		case WEB_CYPHERPITTHREE:
			//new CypherPitThree;
			//break;
		case WEB_CYPHERMOLEONE:
			//new CypherMoleOne;
			//break;
		case WEB_CYPHERMOLETWO:
			//new CypherMoleTwo;
			//break;
		case WEB_CYPHERMOLETHREE:
			//new CypherMoleThree;
			//break;
		case WEB_CYPHERMOLEFOUR:
			//new CypherMoleFour;
			//break;
		case WEB_YINGCYPHER:
			//new YingCypherNote;
			//new HTMLPage( 99130 );
			//break;
			new HTMLPage(99130);
			break;
		default:
#ifdef _DEBUG
			sfxPrintf("unknown cipher type");
#endif
			NULL;
		}
		break;
	case PC_PHOTODOCBUTTON:
		//new PhotoDoc;
		new HTMLPage(99198);
		break;
	case PC_PEGIDBUTTON:
		new HTMLPage(99655);
		break;
	case PC_ECLIPSEBUTTON:
		GameFlag.set(fGreg20);
		new HTMLPage(97726);
		break;
	case PC_EBMC:
		new HTMLPage(97670);
		break;
	default:
#ifdef _DEBUG
		sfxPrintf("Bad button type");
#endif
		NULL;
	}
	if (roomMidi != -1)
		theMusic->stop();
	if (puzzMidi != -1)
		theMusic->play(puzzMidi);
	END

		BEG
		// rest at return from puzzle
		user->canInput(false);
	plobj = new PermListObject;
	plobj->setScript(this);
	ticks = 2;
	END

		BEG
		if (puzzMidi != -1)
			theMusic->stop();
	if (roomMidi != -1)
	{
		theMusic->loop = SND_LOOP_INFINITE;
		theMusic->play(roomMidi);
	}
	user->canInput(true);
	client->script = NULL;
	client->dispose();		// get rid of perm list object
	GameFlag.clear(fUsingWorkstation);
	theScreen->buttons.dispose();
	theScreen->addTheButtons();
	theScreen->drawButtons();
	theScreen->cue();	// get buttons blinking
	if (autoExit)
	{
		ASSERT(theInset != NULL, 0);
		theInset->dispose();
	}
	dispose();
	END
}

PCScreenUpdateTimer::PCScreenUpdateTimer(Object *whoToCue) {
	PermListObject *plobj;

	name = "PCScreenUpdateTimer";
	pcTimer = this;
	plobj = new PermListObject;
	plobj->setScript(this, whoToCue);
}

void PCScreenUpdateTimer::changeState(int newState) // launch Web and puzzle
{
	switchTo
		seconds = 4;
	END

		BEG
		if (caller == theScreen)
		{
			caller->cue();
			seconds = 0;
			state = -1;
			client->script = NULL;
			client->setScript(this, caller);	// reset
		} else
		{
			client->dispose();	// perm list object, gets this too
			if (theScreen == NULL)
				pcTimer = NULL;
		}
		END
}

void PCScreenUpdateTimer::doit(void) {
	blinkers.doit();
	Script::doit();
}

PCButtonBlinker::PCButtonBlinker(PCButtonView *button) {
	name = "PCButtonBlinker";
	bright = false;
	myButton = button;
	myButton->blinker = this;
	ASSERT(pcTimer != NULL, 0);
	pcTimer->blinkers.add(this);
	init(this, NULL, NULL);	// bogus client: this, needed for dispose()
}

void PCButtonBlinker::stop(void) {
	if (bright)
		myButton->setCel(0);
	kill();
}

void PCButtonBlinker::kill(void) {
	pcTimer->blinkers.release(this);
	dispose();
}

void PCButtonBlinker::changeState(int newState)	// blink
{
	switchTo
		ticks = 30;
	END

		BEG
		if (currentFocus == INSET_FOCUS && blinkOK)//briefcase closeup may be up ->don't blink    	
		{
			if (bright)
			{
				myButton->setCel(0);	// blink off
				bright = false;
			} else
			{
				myButton->setCel(2);	// blink on
				bright = true;
			}
		}
	seconds = 0;
	state = -1;
	init(this, NULL, NULL);	// bogus client: this, needed for dispose()
	END
}

void PCDistantButtonScript::changeState(int newState) // turn blinking on and off
{
#define MYCLIENT ((PCDistantButton*)client)

	switchTo
		if (MYCLIENT->blinkOn())
		{
			if (MYCLIENT->cycler == NULL)
				MYCLIENT->setCycle(new Forward(MYCLIENT));
		} else
			MYCLIENT->setCycle(NULL);
		seconds = 4;
		END

			BEG
			seconds = 0;
		state = -1;
		MYCLIENT->script = NULL;
		MYCLIENT->setScript(this);	// reset	
		END
}

void PlayKATScriptPC::changeState(int newState) {
	Web *thisWeb;
	static int	soundNumber;

	switchTo
		theGame->handsOff();
	blinkOK = false;
	soundNumber = -1;
	if (sound1->isPlaying())
	{
		soundNumber = sound1->number;
		sound1->stop();
	}
	theMovie->play("24901.avi", this, false, 2, false);
	END

		BEG
		theGame->handsOn();
	thisWeb = new Web;
	thisWeb->caller = this;
	new HTMLPage(99104);
	blinkOK = true;
	END

		BEG
		if (soundNumber != -1)
			sound1->playBuffered(soundNumber, NULL);
	dispose();
	END
}

/*******************************
 *
 *	Distant Button Blinker
 *
 *	Note: one of these for each
 *			button
 *
/*******************************/

PCDistantButton::PCDistantButton(int button, int atsnum, int x, int y) {
	name = "PCDistantButton";
	setView(atsnum);
	init(x, y);
	setHotspot(DO_V);
	cycleSpeed = 20;
	buttonType = button;
	setScript(new PCDistantButtonScript);
}

void PCDistantButton::cue(void) {
	setCycle(new Forward(this));
	sfxPrintf("PCDistantButton::cue()");
}

int PCDistantButton::blinkOn(void) {
	int		result;

	switch (buttonType)
	{
	case PC_INTRLINKBUTTON:
		result = comlinks && !comlinks->isEmpty();
		break;

	case PC_IABUTTON:
		result = !strcmp("Rm5100", curRoom->name);	// in Rm5100
		break;

	case PC_INFOSCIBUTTON:
	case PC_KATBUTTON:
	case PC_SOUNDANALBUTTON:
	case PC_CIPHERBUTTON:
	case PC_PHOTODOCBUTTON:
		result = false;
		break;

	default:
		ASSERT(false, 0);
		result = false;
	}

	return result;
}

} // namespace Spycraft
