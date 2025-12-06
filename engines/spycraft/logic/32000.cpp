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
#include "spycraft/logic/32000.h"
#include "spycraft/logic/32100.h"
#include "spycraft/logic/32200.h"
#include "spycraft/logic/1000.h"
#include "spycraft/game/flag.h"
#include "spycraft/game/movie.h"
#include "spycraft/game/invent.h"
#include "spycraft/game/timer.h"
#include "spycraft/game/sound.h"
#include "spycraft/logic/60000.h"

namespace Spycraft {

/*********************
*
*		Room Code 
*
**********************/

extern Flag GameFlag;
extern Intrface* intrface;
extern NorthCursor* northCursor;
extern SouthCursor* southCursor;
extern DialTree* dialogTree;
extern Sound* sound1;
extern MADEEnvirons* colbyInfo;
extern int currentFocus;

//Timer* harmTimer32000;
static HarmKillPlayer*  harmkillplayer; 

HarmDialog* 	harmonicaDialog1;
HarmDialog*		harmonicaDialog2;
HarmDialog*		harmonicaDialog3;
HarmDialog*		harmonicaDialog4;
HarmDialog*		harmonicaDialog5;
HarmDialog*		harmonicaDialog6;
HarmDialog*		harmonicaDialog7;
HarmDialog* 	harmonicaDialog8;
HarmDialog*		harmonicaDialog9;
HarmDialog*		harmonicaDialog10;
HarmDialog*		harmonicaDialog11;
HarmDialog*		harmonicaDialog12;

//NorthExit32000* myExit32000;		  //timer

HarmDItem* whyHere;
HarmDItem* whatWant;
HarmDItem* whereMe; 
HarmDItem* andHarmless;
HarmDItem* procatMe; 
HarmDItem* withMe; 
HarmDItem* fromMe;
HarmDItem* fingerMole; 
HarmDItem* joinProcat;
DItem* beenWork;
DItem* noProcat;
HarmDItem* needPlayer; 
DItem* goodSign;
DItem* maxSave;
DItem* kissMy;
 
enum	
{
	WHYHERE,
	WHATWANT,
	WHEREME,
	ANDHARMLESS,
	PROCATME,
	WITHME,
	FROMME,
	FINGERMOLE,
	JOINPROCAT,
	BEENWORK,
	NOPROCAT,
	NEEDPLAYER,
	GOODSIGN,
	MAXSAVE,
	KISSMY
};

Rm32000::Rm32000()
{
	name = "Rm32000";
	show_style = VE_FLIP;
}

void Rm32000::cue( void )
{
//	sfxPrintf( "cue" );
///	theMovie->fromTo(369, 372);						
///	theMovie->play("32001.avi", this, 0, 0, 0);

//	currentFocus = 0;
//	theMovie->caller = 0;
//	theMovie->stop();
//	setScript(new EnterScript32003);
}

Rm32000::~Rm32000()
{
//	sound1->stop();  
	if( GameFlag.test( fDisposeDialog32000 )) {
		harmonicaDialog1->dispose();
		harmonicaDialog2->dispose();
		harmonicaDialog3->dispose();
		harmonicaDialog4->dispose();
		harmonicaDialog5->dispose();
		harmonicaDialog6->dispose();
		harmonicaDialog7->dispose();
		harmonicaDialog8->dispose();
		harmonicaDialog9->dispose();
		harmonicaDialog10->dispose();
		harmonicaDialog11->dispose();
		harmonicaDialog12->dispose();

		GameFlag.clear( fDisposeDialog32000 );
	}  //endif
}

void Rm32000::init()
{
    Room::init();

//	myExit32000 = new NorthExit32000;   //timer

  	new(NorthExit32000);
  	new(SouthExit32000);

	intrface->show();
	if( GameFlag.test(fPlayLenaMsg) && !GameFlag.test(fPlayerTurnsProcat) && !GameFlag.test(fMetLena) && (!GameFlag.test(fWahKa11) || (GameFlag.test(fMetHarmonica)))) {
//	if( GameFlag.test( fLenaCalls ) && !GameFlag.test( fMetLena )){
//		drawPic( 1 );
   		setScript( new EnterScript32001 );
	}
	else if ( GameFlag.test(fHarmCalls)  && !GameFlag.test(fOnce32000)) {
//		drawPic( 1 );
		setScript( new EnterScript32002 );	   
	}
	else{
		drawPic( 32000 );
		theGame->handsOn();

		if( colbyInfo->slow_system == true )
			theMusic->play ( 32000 );
		else {
			if( sound1->number != 32000  || !sound1->isPlaying() )	{
				sound1->loop = -1; 
				sound1->playBuffered(32000, 0);
			}
		}
	}

/*
	Timer* myTimer;
	myTimer = new Timer;
	myTimer->setReal(1, new EnterScript32002);
*/
}

int Rm32000::handleEvent ( MADEEventStamp *event )
{
/*
	if (event_type == USER_LEFT_UP)	{
		sfxPrintf("timer");
		return true;
	}
*/
	return false;
}

/*********************
*
*	Exit Features 
*
**********************/
NorthExit32000::NorthExit32000()
{
	name = "NorthExit32000";
	setHotspot( DO_V );
	init(197, 106, 98, 172);
//	init(NORTH_EXIT_X, NORTH_EXIT_Y, NORTH_EXIT_WIDTH, NORTH_EXIT_HEIGHT);  
}

void NorthExit32000::respond()
{
	theGame->setCursor( northCursor );
	aniCursor();
}

int NorthExit32000::doVerb ( int theVerb )
{
	if( theVerb == DO_V ) {
    	theGame->newRoom( new Rm32100 );
    	return true;
	}
	return false;
}

SouthExit32000::SouthExit32000()
{
	name = "SouthExit32000";
	setHotspot ( DO_V );
	init(SOUTH_EXIT_X, SOUTH_EXIT_Y, SOUTH_EXIT_WIDTH, SOUTH_EXIT_HEIGHT);  
}

void SouthExit32000::respond()
{
	theGame->setCursor(southCursor);
	aniCursor();
}

int SouthExit32000::doVerb ( int theVerb )
{
	if ( theVerb == DO_V ) {
		sound1->stop();
		theMusic->stop();
		theGame->newRoom(new Rm1000);
    	return true;
	}
	return false;
}

/*******************************
 *
 *	Scripts
 *
 *******************************/
EnterScript32001::EnterScript32001()
{
	name = "EnterScript32001"; 
}

void EnterScript32001::changeState( int newState )
{
	switchTo
//		GameFlag.set(fMetLena);		   
///		theGame->handsOff();
		theMovie->fromTo(0, 105);
		theMovie->play("32000.avi", this, 0, 0, 0);
//		theMovie->play("32000.avi", this);
		inventry->get(iYURI_DOSSIER);		   
	END
	BEG										   
		inventry->items[iYURI_DOSSIER]->activate();

		theMovie->fromTo(106, 456);
		theMovie->play("32000.avi", this, 0, 1, 1);		
	END
	BEG
		GameFlag.set(fMetLena);	
//		sfxPrintf("Play Lena's dialogue?");	
///		theGame->handsOn();
///		dispose();
//		theGame->newRoom( new Rm33000 );
		theGame->newRoom( new Rm1000 );
	END
}

EnterScript32002::EnterScript32002()
{
	name = "EnterScript32002"; 
}

void EnterScript32002::changeState( int newState )
{
	switchTo
//		GameFlag.set(fMetHarmonica);
//		GameFlag.set(fOnce32000);
//		GameFlag.set(fWahKa62);
		GameFlag.set(fDisposeDialog32000);		   
///		theGame->handsOff();
		theMovie->fromTo(0, 364);
		theMovie->play("32001.avi", this, 0, 0, 0);

		harmkillplayer = new HarmKillPlayer;
	END
	BEG
//		sfxPrintf("Play Harmonica's dialogue");	
///		curRoom->cue();
///		theGame->handsOn();

		dialogTree = new DialTree;
		harmonicaDialog1 = new HarmDialog;
		harmonicaDialog2 = new HarmDialog;
		harmonicaDialog3 = new HarmDialog;
		harmonicaDialog4 = new HarmDialog;
		harmonicaDialog5 = new HarmDialog;
		harmonicaDialog6 = new HarmDialog;
		harmonicaDialog7 = new HarmDialog;
		harmonicaDialog8 = new HarmDialog;
		harmonicaDialog9 = new HarmDialog;
		harmonicaDialog10 = new HarmDialog;
		harmonicaDialog11 = new HarmDialog;
		harmonicaDialog12 = new HarmDialog;

		whyHere = new HarmDItem;
		whyHere->addInfo(32000, WHYHERE, harmonicaDialog3);
		whatWant= new HarmDItem;
		whatWant->addInfo(32000, WHATWANT, harmonicaDialog3 );
		whereMe = new HarmDItem;
		whereMe->addInfo(32000, WHEREME, harmonicaDialog4 );
		andHarmless = new HarmDItem;
		andHarmless->addInfo(32000, ANDHARMLESS, harmonicaDialog5 );
		procatMe = new HarmDItem;
		procatMe->addInfo(32000, PROCATME, harmonicaDialog6 );
		withMe = new HarmDItem;
		withMe->addInfo(32000, WITHME, harmonicaDialog3 );
		fromMe = new HarmDItem;
		fromMe->addInfo(32000, FROMME, harmonicaDialog6 );
		fingerMole = new HarmDItem;
		fingerMole->addInfo(32000, FINGERMOLE, harmonicaDialog7 );
		joinProcat = new HarmDItem;
		joinProcat->addInfo(32000, JOINPROCAT, harmonicaDialog8 );
//		beenWork = new HarmDItem;
		beenWork = new TurnProcat;
		beenWork->addInfo(32000, BEENWORK, harmonicaDialog9 );
//		noProcat = new HarmDItem;
if(GameFlag.test(fWahKa60)) {
		noProcat = new NoProcat;
		noProcat->addInfo(32000, NOPROCAT, harmonicaDialog10 );
////	noProcat = new SaveYou;
////	noProcat->addInfo(32000, NOPROCAT, harmonicaDialog12 );
}
else {
		kissMy = new NoProcat;
		kissMy->addInfo(32000, KISSMY, harmonicaDialog10 );

}
		needPlayer = new HarmDItem;
		needPlayer->addInfo(32000, NEEDPLAYER, harmonicaDialog6 );
//		goodSign = new HarmDItem;
		goodSign = new TurnProcat;
		goodSign->addInfo(32000, GOODSIGN, harmonicaDialog9 );
//		maxSave = new HarmDItem;
if(GameFlag.test(fWahKa60)) {
		maxSave = new SaveYou;
		maxSave->addInfo(32000, MAXSAVE, harmonicaDialog12 );
} 
/*
		harmonicaDialog1->addMovieInfo(0, 372, 0, 0);
		harmonicaDialog2->addMovieInfo(387, 700, 0, 0);
		harmonicaDialog3->addMovieInfo(775, 890, 0, 0);
		harmonicaDialog4->addMovieInfo(905, 1000, 0, 0);
		harmonicaDialog5->addMovieInfo(1025, 1313, 0, 0); 
		harmonicaDialog6->addMovieInfo(1338, 1525, 0, 0);
		harmonicaDialog7->addMovieInfo(1533, 1737, 0, 0);
		harmonicaDialog8->addMovieInfo(1743, 1983, 0, 0);
		harmonicaDialog9->addMovieInfo(1985, 2275, 0, 0);
		harmonicaDialog10->addMovieInfo(2300, 2457, 0, 0);
		harmonicaDialog11->addMovieInfo(2460, 2550, 0, 0);
		harmonicaDialog12->addMovieInfo(2555, 3148, 0, 0); 
*/
		harmonicaDialog1->addMovieInfo(0, 364, 0, 0);
		harmonicaDialog3->addMovieInfo(375, 505, 0, 0);	  // thought you were harmless
		harmonicaDialog4->addMovieInfo(510, 629, 0, 0);	  // safe house is a joke
		harmonicaDialog5->addMovieInfo(645, 959, 0, 0);	  // now not so harmless
		harmonicaDialog6->addMovieInfo(975, 1172, 0, 0);  // im going to give you a choice
		harmonicaDialog7->addMovieInfo(1185, 1391, 0, 0); // lets just say youll be filthy rich
		harmonicaDialog8->addMovieInfo(1395, 1638, 0, 0);
		harmonicaDialog9->addMovieInfo(1650, 1974, 0, 0);
		harmonicaDialog2->addMovieInfo(1980, 2072, 0, 0);
		harmonicaDialog10->addMovieInfo(2085, 2159, 0, 0);
		harmonicaDialog11->addMovieInfo(2175, 2270, 0, 0);
		harmonicaDialog12->addMovieInfo(2280, 2912, 0, 0); 

		harmonicaDialog1->add(whyHere);
		harmonicaDialog2->add(whatWant);
		harmonicaDialog2->add(whereMe);
		harmonicaDialog3->add(andHarmless);
		harmonicaDialog4->add(whyHere);
		harmonicaDialog4->add(withMe);
		harmonicaDialog5->add(fromMe);
		harmonicaDialog5->add(fingerMole);
		harmonicaDialog6->add(joinProcat);
		harmonicaDialog6->add(beenWork);
if(GameFlag.test(fWahKa60))
		harmonicaDialog6->add(noProcat);
else
		harmonicaDialog6->add(kissMy);
		harmonicaDialog7->add(needPlayer);
		harmonicaDialog8->add(goodSign);
if(GameFlag.test(fWahKa60))
		harmonicaDialog8->add(maxSave);

		dialogTree->rootNode = harmonicaDialog2;
		dialogTree->resourceID = "32001.avi";
		dialogTree->show(this);
	
	END

	BEG
		GameFlag.set(fMetHarmonica);
		GameFlag.set(fOnce32000);
		GameFlag.set(fWahKa62);

		intrface->enable();
		theMovie->caller = 0;
		theMovie->stop();
		harmkillplayer->dispose();

		if(GameFlag.test(fPlayerKilled)) {
			//endType = END_DEAD;
			theGame->newRoom( new Rm60000(END_DEAD));
		}
		else {
			theGame->newRoom( new Rm1000 );
		}
	END
}

EnterScript32003::EnterScript32003()
{
	name = "EnterScript32003"; 
}

void EnterScript32003::changeState( int newState )
{
	switchTo
		seconds = 10;
	END
	BEG
		theMovie->fromTo(2175, 2270);
		theMovie->play("32001.avi", this, 0, 0, 0);
	END
	BEG
		seconds = 10;
	END
	BEG
		dialogTree->curNode->hide();
		currentFocus = 0;
//		if (myDialog->myTree->caller)	{
//			myDialog->myTree->caller->cue();
//			myDialog->myTree->caller = NULL;
//		}
//		theMovie->caller = 0;
//		theMovie->stop();
		dialogTree->caller = NULL;

		GameFlag.set(fPlayerKilled);
		theMovie->fromTo(2085, 2159);
		theMovie->play("32001.avi", this, 0, 1, 0);
	END
	BEG
//		sfxPrintf("Player get shoot by Harmonica.");
		theGame->newRoom( new Rm60000(END_DEAD));
	END
}

/*******************************
 *
 *	Diagolue
 *
/*******************************/
void NorthExit32000::cue ( void )
{
//	sfxPrintf( "cue" );
}

HarmDialog::HarmDialog()
{
	name = "HarmDialog";
}

void HarmDialog::show( void )
{
//	harmTimer32000 = new Timer;
//	harmTimer32000->setReal(10, curRoom);
	
	Dialog::show();
}

void HarmDialog::hide( void )
{
//	harmTimer32000->dispose();
	Dialog::hide();
}

void HarmDialog::cue( void )
{
 	Node*	curNode = head;
	int		foundOne; 

	// see if there's a DItem up
	foundOne = false;
 	while (curNode)
 	{
		if( !((DItem*)(curNode->data))->hasBeenChosen )
		{
			foundOne = true;
			break;
		}
		curNode = curNode->next;
	}
	//set timer to hang up (if no DItem then we're on the last segment and don't want a timer)
	if( foundOne )
		harmkillplayer->setScript( new EnterScript32003 ); 	

	Dialog::cue();
}

TurnProcat::TurnProcat()
{
	name = "TurnProcat";
}

void TurnProcat::doit()
{
	curMap = MAP_STATION_INT;
	theMovie->pause(false);
	GameFlag.set(fPlayerTurnsProcat);       //player turns Procat
	harmkillplayer->setScript( NULL );
	DItem::doit();
}

NoProcat::NoProcat()
{
	name = "NoProcat";
}

void NoProcat::doit()
{
	theMovie->pause(false);
	GameFlag.set(fPlayerKilled);		 	//didn't join Procat, the end
	harmkillplayer->setScript( NULL );
	DItem::doit();
//	sfxPrintf("Game Over");	
}

SaveYou::SaveYou()
{
	name = "SaveYou";
}

void SaveYou::doit()
{
	theMovie->pause(false);
	GameFlag.set(fMaxSavesPlayer);   	   //Max saves you from Harmonica
	harmkillplayer->setScript( NULL );
	DItem::doit();
}

/*******************************
 *
 *	PermListObj
 *
 *******************************/

HarmKillPlayer::HarmKillPlayer()
{
	name = "HarmKillPlayer";
//	playingVoice = false;
}

void HarmKillPlayer::dispose( void )
{
//	ASSERT( !playingVoice, 0 );
	Object::dispose();
}

void HarmDItem::doit( void )
{
	theMovie->pause(false);
	harmkillplayer->setScript( NULL );
 	DItem::doit();
}
 
} // namespace Spycraft 
