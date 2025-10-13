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

#include "spycraft/game/movie.h"
#include "spycraft/game/dialog.h"
#include "spycraft/game/user.h"
#include "spycraft/game/globals.h"
#include "spycraft/dmade/advmsg.h"

namespace Spycraft {

extern int currentFocus;
extern Movie *theMovie;

int msgLocked;

DialTree *dialogTree;
Dialog *theDialog;

SRect lilRect;

void cleanup(void) {
	//sfxUnloadMovie(TestID);
}

Dialog::Dialog() {
	name = "Dialog";
	x = 240;  		//DEFAULTX defined later
	y = 420;  		//DEFAULTY defined later
	bottom = y;
	myTree = 0;
	resourceID = NULL;
}

void Dialog::init(int theX, int theY, long resourceNum) {
	name = "Dialog";
	x = theX;
	y = theY;
	bottom = y;
	// resourceID = resourceNum;
}

void Dialog::init(long resourceNum, DialTree *curTree) {
	//OLD - when we used a lookup table
	//resourceID = resourceNum;
	myTree = curTree;
}

void Dialog::init(long resourceNum) {
	//resourceID = resourceNum;
}

void Dialog::dispose() {
	Node *n = head;

	while (n) {
		if (n->data) {
			n->data->dispose();   //get rid of DItem->myPrint
			n->data = 0;
		}
		n = n->next;
	}
	currentFocus = 0;
	List::dispose();          //get rid of DItems
	delete this;              //get rid of list
}

void Dialog::add(char *theText, Dialog *whatsNext) {
	Print *thePrint = new Print();
	thePrint->myString = theText;

	DItem *theItem = new DItem();
	theItem->myPrint = thePrint;
	theItem->myDialog = this;
	theItem->nextDialog = whatsNext;
	List::add(theItem);
}

void Dialog::add(DItem *theItem, Dialog *whatsNext) {
	List::add(theItem);
	theItem->myDialog = this;
	myTree = dialogTree;
	if (whatsNext)
		whatsNext->myTree = dialogTree;

	theItem->nextDialog = whatsNext;
}

void Dialog::add(DItem *theItem) {
	List::add(theItem);
	theItem->myDialog = this;
	myTree = dialogTree;
}

void Dialog::add(int index, Dialog *whatsNext) {

	Print *thePrint = new Print();

	if (index >= 0)
	{
		thePrint->myString = myTree->myArray[index];
	}

	DItem *theItem = new DItem();
	if (index >= 0)
	{
		theItem->myPrint = thePrint;
	} else
	{
		theItem->myPrint = NULL;
	}
	theItem->myDialog = this;
	theItem->nextDialog = whatsNext;
	List::add(theItem);
}

void Dialog::add(long textNum) {
	//  Print* thePrint = new Print();
	//  thePrint->myString = curArray->getString(theIndex);
	//  DItem* theItem = new DItem();
	//  theItem->myPrint = thePrint;
	//  theItem->myDialog = this;
	//  theItem->nextDialog = whatsNext;
	//  List::add(theItem);    
}

void Dialog::show() {
	DItem *theNode;
	Node *curNode = head;
	int count = 0;

	//theGame->resetHotspot();
	bottom = y;
	while (curNode) {
		theNode = (DItem *)curNode->data;
		if (theNode->hasBeenChosen) {
			curNode = curNode->next;
			continue;
		}

		++count;
		theNode->myDialog = this;		//so we can reuse items in different dialogs
		if (theNode->myPrint) {
			sfxSetTextFont(FNT_HELVETICA, 18, STYL_BOLD);
			x = (512 - sfxGetTextWidth(theNode->myPrint->myString)) / 2;
			theNode->myPrint->x = x;
			theNode->myPrint->y = bottom;
		}
		theNode->init();
		curNode = curNode->next;
	}
	user->canInput(true);
	if (!count) {
		currentFocus = 0;
		if (myTree->caller) {
			myTree->caller->cue();
			//	myTree->dispose(); //added 2/15
			//	myTree->caller = NULL; myTree is most likely disposed of by caller
		}
		if (msgLocked) {
			sfxUnlockRes(msgLocked, RES_TEXT);
			msgLocked = 0;
		} else
			ADV_ASSERT(0, 0);
	}
}

void Dialog::addMovieInfo(int fromFrame, int toFrame, int loopFromFrame, int loopToFrame) {
	from = fromFrame;
	to = toFrame;
	loopFrom = loopFromFrame;
	loopTo = loopToFrame;
}

void Dialog::doit() {
	/*MADEEventStamp event;

	while (true)  {
	  {
	  sfxGetEvent(&event);
	*/
}

void Dialog::cue(void) {
	/*	if (loopFrom != -1)	{
			theMovie->fromTo(loopFrom, loopTo);
			theMovie->play(myTree->resourceID, this, 0, 0, 0);
		}*/


	if (this != dialogTree->curNode) {
		dialogTree->curNode = this;
		dialogTree->curNode->show();
	}

}

void Dialog::hide() {
	DItem *theNode;
	Node *curNode = head;
	// currentFocus = false;
	while (curNode)
	{
		theNode = (DItem *)curNode->data;
		if (theNode->myPrint)
			theNode->myPrint->erase();
		if (theNode->myView) {
			theNode->myView->dispose();
			theNode->myView = 0;
		}
		curNode = curNode->next;
	}
}

DItem::DItem() {
	hasBeenChosen = false;
	name = "DItem";
	highlighted = false;
	myPrint = NULL;
	myView = NULL;
	myDialog = NULL;
	nextDialog = NULL;
}

DItem::~DItem() {


}

void DItem::addInfo(int theFile, int theIndex, Dialog *whatsNext) {
	if (myView) {
		sfxPrintf("View already attached");
		return;
	}
	myPrint = new Print;

	myPrint->myString = sfxGetMsg(theFile, theIndex, 0);
	if (!msgLocked) {
		msgLocked = theFile;
		sfxLockRes(msgLocked, RES_TEXT);
	}
	nextDialog = whatsNext;

	if (whatsNext)
		whatsNext->myTree = dialogTree;

}

void DItem::addInfo(int theView, int theLoop, int theCel, int theX, int theY, Dialog *whatsNext) {
	if (myPrint) {
		sfxPrintf("Print already attached");
		return;
	}
	myView = new View;
	myView->view = theView;
	myView->loop = theLoop;
	myView->cel = theCel;
	myView->x = theX;
	myView->y = theY;
	nextDialog = whatsNext;
	if (whatsNext)
		whatsNext->myTree = dialogTree;
}

int DItem::handleEvent(MADEEventStamp *event) {
	if ((!hasBeenChosen) && (user->input)) {
		if (onMe(event))
		{
			if ((!highlighted) && (!event_type))
			{
				highlighted = true;
				highlight(true);
				return(false);
			} else if (event_type == USER_LEFT_UP && user->input)
			{
				doit();
				return(true);
			}

		} else
		{

			if (highlighted)
			{
				highlighted = false;
				highlight(false);
				return(false);
			}

		}
	}

	return(false);
}

bool DItem::onMe(MADEEventStamp *event) {
	long theX, theY;
	theX = event->cursor_pt.x;
	theY = event->cursor_pt.y;
	if (theX < right && theX > left && theY > top && theY < bottom)
		return true;
	else
		return false;
}

void DItem::cue() {
	theMovie->fromTo(nextDialog->from, nextDialog->to);
	theMovie->play(nextDialog->myTree->resourceID, nextDialog, 0, 0, 0);
}

void DItem::doit() {
	user->canInput(false);
	hasBeenChosen = true;
	if (nextDialog) {
		if (nextDialog->myTree->resourceID != NULL) {
			theMovie->fromTo(nextDialog->from, nextDialog->to);
			if (!dialogTree->negativeParam)//2/15
				theMovie->play(nextDialog->myTree->resourceID, nextDialog, 0, 0, 0);
			else//2/15
				theMovie->play(nextDialog->myTree->resourceID, nextDialog, 0, -1, 0);
			myDialog->hide();
			// FOR LOOPING	theMovie->caller = this;
		} else
			sfxPrintf("NO movie to play!!!");
	}
	////////////////////////////////////////////added
	else {
		myDialog->hide();
		currentFocus = 0;
		if (myDialog->myTree->caller) {
			myDialog->myTree->caller->cue();
			//myDialog->myTree->caller = NULL;
		}
	}
	///////////////////////////////////////////////////////

}

bool DItem::init() {
	if (myPrint) {
		myPrint->init();
		if (myDialog)
			myDialog->bottom += myPrint->size;

		width = sfxGetTextWidth(myPrint->myString);
		//need MADE call to determine height
		setRect((myDialog->bottom - myPrint->size), myDialog->x, myDialog->bottom, (width + myDialog->x));
		return(true);
	} else if (myView) {
		myView->init(myView->x, myView->y);
		setRect(myView->top, myView->left, myView->bottom, myView->right);
		return true;
	}
	sfxPrintf("Error: No Print or View attached, hide disposes of myView");
	return(false);
}

void DItem::highlight(bool hiOrLow) {
	//will be different for cDButton
	if (myPrint && (myPrint->textBuffer != -1))
		myPrint->highlight(hiOrLow);
}

void DItem::dispose() {
	if (myPrint) {
		myPrint->dispose();
		myPrint = 0;
	}
	if (myView) {
		myView->dispose();
		myView = 0;
	}
	Object::dispose();
}

void DItem::setRect(int theTop, int theLeft, int theBottom, int theRight) {
	//sfxPrintf("t:%d, l:%d, b:%d, r:%d", theTop, theLeft, theBottom, theRight);
	top = theTop;
	left = theLeft;
	bottom = theBottom;
	right = theRight;
}


void vlinkDItem::cue() {
	theMovie->fromTo(nextDialog->from, nextDialog->to);
	theMovie->play(nextDialog->myTree->resourceID, nextDialog, 0);
}

void vlinkDItem::doit() {
	user->canInput(false);
	hasBeenChosen = true;
	if (nextDialog)
	{
		if (nextDialog->myTree->resourceID != NULL)
		{
			theMovie->fromTo(nextDialog->from, nextDialog->to);
			theMovie->play(nextDialog->myTree->resourceID, nextDialog, 0);
			myDialog->hide();
		} else
			sfxPrintf("NO movie to play!!!");
	}
	////////////////////////////////////////////added
	else
	{
		myDialog->hide();
		currentFocus = 0;
		if (myDialog->myTree->caller)
		{
			myDialog->myTree->caller->cue();
			myDialog->myTree->caller = NULL;
		}
	}
	///////////////////////////////////////////////////////
}

DialTree::DialTree() {
	rootNode = 0;
	curNode = 0;
	myArray = 0;
	caller = 0;
	negativeParam = 0;
}

int DialTree::handleEvent(MADEEventStamp *event) {
	curNode->handleEvent(event);
	return(true);
}

void DialTree::show(Object *whoToCue) {
	curNode = rootNode;
	caller = whoToCue;
	currentFocus = DIALOG_FOCUS;
	this->rootNode->show();
}

void DialTree::dispose() {
	Object::dispose();
	dialogTree = NULL;  //added 2/15
}

} // namespace Spycraft
