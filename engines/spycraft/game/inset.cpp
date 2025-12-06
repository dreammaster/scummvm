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

#include "spycraft/game/inset.h"
#include "spycraft/game/list.h"
#include "spycraft/game/game.h"
#include "spycraft/game/user.h"
#include "spycraft/game/globals.h"

namespace Spycraft {

extern int currentFocus;
extern SouthCursor *southCursor;
extern EastCursor *eastCursor;
extern WestCursor *westCursor;

FeatureList *insetFeatureList;
Inset *theInset;
InsetHotspotList *insetHotspots;
InsetFeature *insetHotObj;

Inset::Inset() {
	name = "Inset";
	hotspot = NULL;
	whoToCue = NULL;
	left = top = right = bottom = 0;
	insetHotObj = NULL;
	hotObj = 0;
}

Inset::~Inset() {
	if (!insetHotspots->isEmpty())
		insetHotspots->releaseAll();
	if (!insetFeatureList->isEmpty())
		insetFeatureList->dispose();
	delete insetFeatureList;
	delete insetHotspots;
	theGame->resetHotspot();
}

void Inset::init(int resId, int theX, int theY, int width, int height) {

	left = theX;
	top = theY;
	right = left + width - 1;
	bottom = top + height - 1;
	currentFocus = INSET_FOCUS;

	insetView = new(InsetView);
	insetFeatureList = new(FeatureList);
	theInset = this;
	insetHotspots = new InsetHotspotList;
	insetView->setView(resId);
	insetView->init(theX, theY);
	insetView->noSkip();
	insetView->setPri(401);
	new(InsetEastExit);
	new(InsetWestExit);
	new(InsetSouthExit);
}

void Inset::init(int resId, int theX, int theY, int theWidth, int theHeight, Object *cue) {
	whoToCue = cue;
	init(resId, theX, theY, theWidth, theHeight);
}

void Inset::dispose() {
	currentFocus = false;
	delete this->insetView;
	if (whoToCue != NULL)
		whoToCue->cue();
	theInset = NULL;
	delete this;
}

int Inset::onMe(int theX, int theY) {
	return ((theX >= left) &&
		(theY >= top) &&
		(theX <= right) &&
		(theY <= bottom));
}

int Inset::onMe(Feature *theObj) {
	return (!((theObj->left > right) ||
		(theObj->top > bottom) ||
		(theObj->right < left) ||
		(theObj->bottom < top)));
}

int Inset::onMe(MADEEventStamp *event) {
	long theX, theY;
	theX = event->cursor_pt.x;
	theY = event->cursor_pt.y;
	if (theX < right && theX > left && theY > top && theY < bottom)
		return true;
	else
		return false;
}

int Inset::doVerb(int theVerb) {
	return false;
}

int Inset::handleEvent(MADEEventStamp *event) {
	if (user->input)
		insetHotspots->doit();
	if ((event_type & USER_LEFT_UP) && user->input) // mouse up
	{
		if (insetFeatureList->handleEvent(event))
			return(true);
		//else if(!onMe(event))
		//{  
		//	dispose();
		//	return true;
		//}
		else	 // onMe and mouse up (proc. of elim.)
			return doVerb(user->message);
	}
	return(false);
}

void InsetFeature::dispose() {
	insetFeatureList->release(this);
	insetHotspots->release(this);
	deathList->add(this);
}

void InsetFeature::setHotspot(int theVerb) {

}


InsetFeature::~InsetFeature() {
	if (insetHotObj == this)
		insetHotObj = NULL;
}

void InsetFeature::init(int theX, int theY, int theWidth, int theHeight) {
	left = theX;
	top = theY;
	right = left + theWidth - 1;
	bottom = top + theHeight - 1;

	insetFeatureList->add(this);

	insetHotspots->add(this);
}

int InsetFeature::handleEvent(MADEEventStamp *event) {
	return Feature::handleEvent(event);
}

void InsetView::init(int theX, int theY) {
	x = theX;
	y = theY;
	draw();
	setPri(402);
}

InsetSouthExit::InsetSouthExit() {
	name = "InsetSouthExit";
	//setHotspot ( DO_V );
	init(SOUTH_EXIT_X, 340, SOUTH_EXIT_WIDTH, 70);
}

void InsetSouthExit::respond() {
	theGame->setCursor(southCursor);
	aniCursor();
}

int InsetSouthExit::doVerb(int theVerb) {
	if (theVerb == DO_V) {
		theInset->dispose();
		return true;
	}
	return false;
}

InsetWestExit::InsetWestExit() {
	name = "InsetWestExit";
	//	setHotspot ( DO_V );
	init(0, 0, 50, 400);
}

void InsetWestExit::respond() {
	theGame->setCursor(westCursor);
	aniCursor();
}

int InsetWestExit::doVerb(int theVerb) {
	if (theVerb == DO_V) {
		theInset->dispose();
		return true;
	}
	return false;
}

InsetEastExit::InsetEastExit() {
	name = "InsetEastExit";
	//	setHotspot ( DO_V );
	init(450, 0, 62, 400);
}

void InsetEastExit::respond() {
	theGame->setCursor(eastCursor);
	aniCursor();
}

int InsetEastExit::doVerb(int theVerb) {
	if (theVerb == DO_V) {
		theInset->dispose();
		return true;
	}
	return false;
}


InsetHotspotList::InsetHotspotList() {
	name = "InsetHotspots";
}

void InsetHotspotList::doit() {
	Node *n = head;
	InsetFeature *obj;

	while (n) {
		obj = (InsetFeature *)(n->data);

		/* CHANGE HOTSPOT TO NO HOTSPOT */
		if (insetHotObj == obj) {
			if (!obj->onMe(mouse_x, mouse_y)) {
				theGame->resetHotspot();
				insetHotObj = NULL;
			}
		}

		/* CHANGE NO HOTSPOT TO HOTSPOT */
		else if (!insetHotObj && obj->onMe(mouse_x, mouse_y))
		{
			insetHotObj = obj;
			obj->respond();
			return;
		}

		n = n->next;
	}
}

} // namespace Spycraft
