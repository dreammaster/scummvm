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

#include "spycraft/game/invinset.h"
#include "spycraft/game/list.h"
#include "spycraft/game/game.h"
#include "spycraft/game/user.h"
#include "spycraft/game/globals.h"
#include "spycraft/logic/1000.h"
#include "spycraft/game/movie.h"

namespace Spycraft {

extern int currentFocus;
extern SouthCursor *southCursor;
extern EastCursor *eastCursor;
extern WestCursor *westCursor;
extern int goingBackwards;

FeatureList *invInsetFeatureList;
InvInset *theInvInset;
InvInsetHotspotList *invInsetHotspots;
InvInsetFeature *invInsetHotObj;

InvInset::InvInset() {
	name = "InvInset";
	left = top = right = bottom = 0;
	invInsetHotObj = NULL;
	hotObj = 0;
	invInsetView = 0;
}

InvInset::~InvInset() {



	/*if(!invInsetHotspots->isEmpty())
		invInsetHotspots->releaseAll();
	if(!invInsetFeatureList->isEmpty())
		invInsetFeatureList->dispose();
	delete invInsetFeatureList;
	delete invInsetHotspots;
	theGame->resetHotspot();*/
}

void InvInset::init(int resId, int theX, int theY, int width, int height) {

	left = theX;
	top = theY;
	right = left + width - 1;
	bottom = top + height - 1;
	oldFocus = currentFocus;
	currentFocus = INV_INSET_FOCUS;
	invInsetView = new(InvInsetView);
	invInsetFeatureList = new(FeatureList);
	theInvInset = this;
	invInsetHotspots = new InvInsetHotspotList;
	invInsetView->setView(resId);
	invInsetView->init(theX, theY);
	invInsetView->noSkip();
	invInsetView->setPri(402);
	new(InvInsetEastExit);
	new(InvInsetWestExit);
	new(InvInsetSouthExit);
	invInsetHotObj = NULL;
	hotObj = 0;
}

void InvInset::dispose() {
	currentFocus = oldFocus;
	delete this->invInsetView;
	invInsetView = NULL; //2/24

	if (!invInsetHotspots->isEmpty())
		invInsetHotspots->releaseAll();
	if (!invInsetFeatureList->isEmpty())
		invInsetFeatureList->dispose();
	delete invInsetFeatureList;
	delete invInsetHotspots;
	theGame->resetHotspot();

	if (!strcmp(curRoom->name, "Rm1000")) {
		goingBackwards = true;
		rm1000->showRegion(curMap);
	}
	if ((!strcmp(curRoom->name, "Rm1270")) && (theMovie->paused))
		theMovie->resume();
	//theInvInset = NULL;
	//delete this;
}

int InvInset::onMe(int theX, int theY) {
	return ((theX >= left) &&
		(theY >= top) &&
		(theX <= right) &&
		(theY <= bottom));
}

int InvInset::onMe(Feature *theObj) {
	return (!((theObj->left > right) ||
		(theObj->top > bottom) ||
		(theObj->right < left) ||
		(theObj->bottom < top)));
}

int InvInset::onMe(MADEEventStamp *event) {
	long theX, theY;
	theX = event->cursor_pt.x;
	theY = event->cursor_pt.y;
	if (theX < right && theX > left && theY > top && theY < bottom)
		return true;
	else
		return false;
}

int InvInset::doVerb(int theVerb) {
	return false;
}

int InvInset::handleEvent(MADEEventStamp *event) {
	if (user->input)
		invInsetHotspots->doit();
	if ((event_type & USER_LEFT_UP) && user->input) // mouse up
	{
		if (invInsetFeatureList->handleEvent(event))
			return(true);
		else	 // onMe and mouse up (proc. of elim.)
			return doVerb(user->message);
	}
	return(false);
}

void InvInsetFeature::dispose() {
	if (hotspot)
		setHotspot(NULL);
	invInsetHotspots->release(this);
	invInsetFeatureList->release(this);
	deathList->add(this);;
}

void InvInsetFeature::setHotspot(int theVerb) {

}


InvInsetFeature::~InvInsetFeature() {
	if (hotspot)
		setHotspot(NULL);
}

void InvInsetFeature::init(int theX, int theY, int theWidth, int theHeight) {
	left = theX;
	top = theY;
	right = left + theWidth - 1;
	bottom = top + theHeight - 1;

	invInsetFeatureList->add(this);

	invInsetHotspots->add(this);
}

int InvInsetFeature::handleEvent(MADEEventStamp *event) {
	return Feature::handleEvent(event);
}

void InvInsetView::init(int theX, int theY) {
	x = theX;
	y = theY;
	draw();
	setPri(402);
}

InvInsetSouthExit::InvInsetSouthExit() {
	name = "InvInsetSouthExit";
	//setHotspot ( DO_V );
	init(SOUTH_EXIT_X, 340, SOUTH_EXIT_WIDTH, 70);
}

void InvInsetSouthExit::respond() {
	theGame->setCursor(southCursor);
	aniCursor();
}

int InvInsetSouthExit::doVerb(int theVerb) {
	if (theVerb == DO_V) {
		theInvInset->dispose();
		return true;
	}
	return false;
}

InvInsetWestExit::InvInsetWestExit() {
	name = "InvInsetWestExit";
	//	setHotspot ( DO_V );
	init(0, 0, 50, 400);
}

void InvInsetWestExit::respond() {
	theGame->setCursor(westCursor);
	aniCursor();
}

int InvInsetWestExit::doVerb(int theVerb) {
	if (theVerb == DO_V) {
		theInvInset->dispose();
		return true;
	}
	return false;
}

InvInsetEastExit::InvInsetEastExit() {
	name = "InvInsetEastExit";
	//	setHotspot ( DO_V );
	init(450, 0, 62, 400);
}

void InvInsetEastExit::respond() {
	theGame->setCursor(eastCursor);
	aniCursor();
}

int InvInsetEastExit::doVerb(int theVerb) {
	if (theVerb == DO_V) {
		theInvInset->dispose();
		return true;
	}
	return false;
}


InvInsetHotspotList::InvInsetHotspotList() {
	name = "InvInsetHotspots";
}

void InvInsetHotspotList::doit() {
	Node *n = head;
	InvInsetFeature *obj;

	while (n) {
		obj = (InvInsetFeature *)(n->data);

		/* CHANGE HOTSPOT TO NO HOTSPOT */
		if (invInsetHotObj == obj) {
			if (!obj->onMe(mouse_x, mouse_y)) {
				theGame->resetHotspot();
				invInsetHotObj = NULL;
			}
		}

		/* CHANGE NO HOTSPOT TO HOTSPOT */
		else if (!invInsetHotObj && obj->onMe(mouse_x, mouse_y))
		{
			invInsetHotObj = obj;
			obj->respond();
			return;
		}

		n = n->next;
	}
}

} // namespace Spycraft
