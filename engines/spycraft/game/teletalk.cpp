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

#include "spycraft/game/teleTalk.h"
#include "spycraft/game/game.h"
#include "spycraft/game/movie.h"
#include "spycraft/game/invent.h"
#include "spycraft/game/flag.h"
#include "spycraft/game/globals.h"
#include "spycraft/game/vlink.h"
#include "spycraft/dmade/advcursor.h"

namespace Spycraft {

// these enums match the lines in the 35010.msg file
enum {
	TT_HELP,		/* My name is Thorn.  I need your help.				*/
	TT_QPROCAT,		/* I neet to ask you some question about Procat		*/
	TT_WOMAN1,		/* I just interrogated a woman who seems to be ... 	*/
	TT_WOMAN2,		/* I just interrogated a woman who is a member ...	*/
	TT_HOLT1,		/* I just spoke with David Holt about Operation ...	*/
	TT_HLOT2,		/* I just talked with David Holt aobut Operation ...*/
	TT_KNOW,		/* How do you know so much?							*/
	TT_THANKS		/* Thanks, I may drop by for more information		*/
};

extern DialTree *dialogTree;

static BlakeTTDialog *BlakeDialog1;
static BlakeTTDialog *BlakeDialog2;
static BlakeTTDialog *BlakeDialog3;
static BlakeTTDialog *BlakeDialog4;
static BlakeTTDialog *BlakeDialog5;
static BlakeTTDialog *BlakeDialog6;
static BlakeTTDialog *BlakeDialog7;
static BlakeTTDialog *BlakeDialog8;
static BlakeTTDialog *BlakeDialog9;
static BlakeTTDialog *BlakeDialog10;

static BlakeTTDItem *help_1;
static BlakeTTDItem *qProcat_2;
static BlakeTTDItem *woman1_4;
static BlakeTTDItem *woman2_5;
static BlakeTTDItem *holt1_4;
static BlakeTTDItem *holt2_6;
static BlakeTTDItem *know_7;
static BlakeTTDItem *know_8;
static BlakeTTDItem *thanks_10;

static BlakeTTHangUpObj *hangUpObj;//obj holds hang up script on perm list--puts self on/off 

void BlakeTeletalkScript::changeState(int newState) {
	switchTo
		theGame->handsOff();
	sfxSystemCursor(10);
	//		theMovie->fromTo( 0, 18 );
	theMovie->fromTo(0, 19);

#ifdef DIRECTX
	theMovie->play("35010.avi", this, 0, -1, 1);
#else
	theMovie->play("35010.avi", this, 0, 0, 1);
#endif

	hangUpObj = new BlakeTTHangUpObj; // puts it on the perm list too.
	//hangUpObj->setScript( new BlakeTeletalkHangUpScript );
	END

		BEG
		// DIALOGS
		theGame->handsOn();
	sfxSystemCursor(10);
	dialogTree = new DialTree;
	dialogTree->negativeParam = true;
	BlakeDialog1 = new BlakeTTDialog;
	BlakeDialog2 = new BlakeTTDialog;
	BlakeDialog3 = new BlakeTTDialog; // not used
	BlakeDialog4 = new BlakeTTDialog;
	BlakeDialog5 = new BlakeTTDialog;
	BlakeDialog6 = new BlakeTTDialog;
	BlakeDialog7 = new BlakeTTDialog;
	BlakeDialog8 = new BlakeTTDialog;
	BlakeDialog9 = new BlakeTTDialog;
	BlakeDialog10 = new BlakeTTDialog;

	// DITEMS
	help_1 = new BlakeTTDItem;
	help_1->addInfo(35010, TT_HELP, BlakeDialog2);

	qProcat_2 = new BlakeTTDItem;
	qProcat_2->addInfo(35010, TT_QPROCAT, BlakeDialog4);

	woman1_4 = new BlakeTeletalkWoman;
	woman1_4->addInfo(35010, TT_WOMAN1, BlakeDialog6);

	woman2_5 = new BlakeTeletalkWoman;
	woman2_5->addInfo(35010, TT_WOMAN2, BlakeDialog8);

	holt1_4 = new BlakeTeletalkHolt;
	holt1_4->addInfo(35010, TT_HOLT1, BlakeDialog5);

	holt2_6 = new BlakeTeletalkHolt;
	holt2_6->addInfo(35010, TT_HLOT2, BlakeDialog7);

	know_7 = new BlakeTTDItem;
	know_7->addInfo(35010, TT_KNOW, BlakeDialog9);

	know_8 = new BlakeTTDItem;
	know_8->addInfo(35010, TT_KNOW, BlakeDialog9);

	thanks_10 = new BlakeTTDItem;
	thanks_10->addInfo(35010, TT_THANKS, BlakeDialog10);

	// DIALOG MOVIES
//		BlakeDialog1->addMovieInfo( 0, 0, 0, 0 );	// not use-- just play first movie

//		BlakeDialog2->addMovieInfo( 30, 83, 0, 0 );
//		BlakeDialog3->addMovieInfo( 4600, 6250, 0, 0 );// not used
//		BlakeDialog4->addMovieInfo( 120, 215, 0, 0 );
// 		BlakeDialog5->addMovieInfo( 225, 366, 0, 0 );
//		BlakeDialog6->addMovieInfo( 375, 626, 0, 0 );
//		BlakeDialog7->addMovieInfo( 225, 366, 0, 0 );
//		BlakeDialog8->addMovieInfo( 375, 626, 0, 0 );
//		BlakeDialog9->addMovieInfo( 630, 683, 0, 0 );
//		BlakeDialog10->addMovieInfo( 690, 831, 0, 0 );

	BlakeDialog2->addMovieInfo(30, 95, 0, 0);
	BlakeDialog3->addMovieInfo(4600, 6250, 0, 0);// not used
	BlakeDialog4->addMovieInfo(134, 236, 0, 0);
	BlakeDialog5->addMovieInfo(240, 383, 0, 0);
	BlakeDialog6->addMovieInfo(390, 642, 0, 0);
	BlakeDialog7->addMovieInfo(240, 383, 0, 0);
	BlakeDialog8->addMovieInfo(390, 642, 0, 0);
	BlakeDialog9->addMovieInfo(645, 704, 0, 0);
	BlakeDialog10->addMovieInfo(705, 845, 0, 0);


	// DITEMS IN MOVIES
	BlakeDialog1->add(help_1);

	BlakeDialog2->add(qProcat_2);

	BlakeDialog4->add(woman1_4);
	BlakeDialog4->add(holt1_4);

	BlakeDialog5->add(woman2_5);

	BlakeDialog6->add(holt2_6);

	BlakeDialog7->add(know_7);

	BlakeDialog8->add(know_8);

	BlakeDialog9->add(thanks_10);

	//BlakeTTDialog9->add( );	   // end

	// START
	dialogTree->rootNode = BlakeDialog1;
	dialogTree->resourceID = "35010.avi";
	dialogTree->show(this);
	END

		BEG
		theGame->handsOff();
	//	theMovie->fromTo( 645, 645 );
	//	theMovie->fromTo( 390, 390 );
	theMovie->fromTo(855, 855);

#ifdef DIRECTX
	theMovie->play("35010.avi", this, 0, -1, 1);
#else
	theMovie->play("35010.avi", this, 0, 0, 1);
#endif

	END

		BEG
		theGame->handsOn();
	BlakeDialog1->dispose();
	BlakeDialog2->dispose();
	BlakeDialog3->dispose();
	BlakeDialog4->dispose();
	BlakeDialog5->dispose();
	BlakeDialog6->dispose();
	BlakeDialog7->dispose();
	BlakeDialog8->dispose();
	BlakeDialog9->dispose();
	hangUpObj->dispose();
	dispose();
	//	inventry->put( iNUMBERTOBLAKE );
	GameFlag.set(fLondonAvailable);
	theMovie->stop();
	new VLink("Blake's our man: Warhurst, Eugene J.", "720.avi", 0, 0);
	//		intrface->enable();
	END
}

void BlakeTeletalkHangUpScript::changeState(int newState) {
	switchTo
		seconds = 7;
	END

		BEG
		hangUpObj->playingVoice = true;
	//		theMovie->fromTo( 90, 118 );
	theMovie->fromTo(105, 132);

#ifdef DIRECTX
	theMovie->play("35010.avi", this, 0, -1, 1);
#else
	theMovie->play("35010.avi", this, 0, 0, 1);
#endif

	END

		BEG
		hangUpObj->playingVoice = false;
	seconds = 5;
	END

		BEG
		//	theMovie->fromTo( 645, 645 );
		 //	theMovie->fromTo ( 390, 390 );
		theMovie->fromTo(855, 855);

#ifdef DIRECTX
	theMovie->play("35010.avi", this, 0, -1, 1);
#else
	theMovie->play("35010.avi", this, 0, 0, 1);
#endif

	END

		BEG
		BlakeDialog1->dispose();
	BlakeDialog2->dispose();
	BlakeDialog3->dispose();
	BlakeDialog4->dispose();
	BlakeDialog5->dispose();
	BlakeDialog6->dispose();
	BlakeDialog7->dispose();
	BlakeDialog8->dispose();
	BlakeDialog9->dispose();
	hangUpObj->dispose();

	GameFlag.clear(fLinus15);	// allow call blake again
	theMovie->stop();
	//		intrface->enable();
	dispose();
	END
}

/*******************************
 *
 *	Dialogs
 *
 *******************************/

void BlakeTTDialog::cue(void) {
	Node *curNode = head;
	int		foundOne; // flag

	// see if there's a DItem up
	foundOne = false;
	while (curNode)
	{
		if (!((DItem *)(curNode->data))->hasBeenChosen)
		{
			foundOne = true;
			break;
		}
		curNode = curNode->next;
	}
	//set timer to hang up (if no DItem then we're on the last segment and don't want a timer)
	if (foundOne)
		hangUpObj->setScript(new BlakeTeletalkHangUpScript);

	Dialog::cue();
}

/*******************************
*
*	DItems
*
*******************************/

void BlakeTTDItem::doit(void) {
	// stop hang up script
	if (hangUpObj->playingVoice)
	{
		theMovie->pause(false, NULL);
		hangUpObj->playingVoice = false;
	}
	hangUpObj->setScript(NULL);
	DItem::doit();
}

/*******************************
 *
 *	Dialog DItem Doits
 *
 *******************************/

void BlakeTeletalkWoman::doit() {
	woman1_4->hasBeenChosen = true;
	woman2_5->hasBeenChosen = true;
	BlakeTTDItem::doit();
}

void BlakeTeletalkHolt::doit() {
	holt1_4->hasBeenChosen = true;
	holt2_6->hasBeenChosen = true;
	BlakeTTDItem::doit();
}

/*******************************
*
*	misc
*
*******************************/

// This object sits on the perm list.  It holds the hang up script and it keeps
// a flag for Blake in the middle of saying "...are you there?"
BlakeTTHangUpObj::BlakeTTHangUpObj() {
	name = "Blake's hang up obj";
	playingVoice = false;
}

void BlakeTTHangUpObj::dispose(void) {
	ASSERT(!playingVoice, 0);
	Object::dispose();
}

} // namespace Spycraft
