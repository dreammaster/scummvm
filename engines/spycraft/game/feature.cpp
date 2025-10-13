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

#include "spycraft/game/feature.h"
#include "spycraft/game/list.h"
#include "spycraft/game/game.h"
#include "spycraft/game/user.h"
#include "spycraft/game/colby.h"
#include "spycraft/game/invent.h"
#include "spycraft/dmade/advmovie.h"

namespace Spycraft {

extern BlinkCursor *normalCursor;

extern int quitting;
extern Object *debugListObj;

FeatureList *features;
HotspotList *hotspots;
Hotspot *hotObj;

Feature::Feature() {
	name = "Feature";
	hotspot = nullptr;
	left = top = right = bottom = 0;
	myHelp = nullptr;
}

Feature::~Feature() {
	if (hotspot) {
		setHotspot(0);
		hotspot = nullptr;
	}
	if (myHelp) {
		delete myHelp;
		myHelp = 0;
	}
}

void Feature::respond() {
	if (theGame->cursor == normalCursor)
		normalCursor->blink(10);
}

void Feature::dispose() {

	if (hotspot) {
		setHotspot(0);
		hotspot = nullptr;
	}
	features->release(this);
	deathList->add(this);
}

void Feature::init(int theX, int theY, int theWidth, int theHeight) {
	left = theX;
	top = theY;
	right = left + theWidth - 1;
	bottom = top + theHeight - 1;

	features->add(this);
}

int Feature::onMe(int theX, int theY) {
	return ((theX >= left) &&
		(theY >= top) &&
		(theX <= right) &&
		(theY <= bottom));
}

int Feature::onMe(Feature *theObj) {
	return (!((theObj->left > right) ||
		(theObj->top > bottom) ||
		(theObj->right < left) ||
		(theObj->bottom < top)));
}

int Feature::onMe(MADEEventStamp *event) {
	long theX, theY;
	theX = event->cursor_pt.x;
	theY = event->cursor_pt.y;
	if (theX < right && theX > left && theY > top && theY < bottom)
		return true;
	else
		return false;
}

void Feature::setHelp(char *theText) {
	myHelp = new Help(theText, this);
}

int Feature::handleEvent(MADEEventStamp *event) {
	if (event_type & USER_MOUSE_UP)
		return (doVerb(user->message));
	else
		return false;
}

int Feature::doVerb(int theVerb) {
	return false;
}

void Feature::setHotspot(int msg) {
	if (msg) {
		if (!hotspot) {
			hotspot = new Hotspot;
			hotspot->client = this;
		}

		hotspot->addRespondVerb(msg);
	} else {
		if (!quitting) {
			if (hotObj == this->hotspot)
				theGame->resetHotspot();
			if (hotspot) {
				hotspot->dispose();
				//	delete hotspot;  //do dispose instead to keek in sync with its clients deletion
				hotspot = nullptr;
			}
		} else {
			if (hotspot)
				delete hotspot;
			hotspot = nullptr;
		}
	}
}

Hotspot::Hotspot() {
	name = nullptr;
	respond_size = 0;

	hotspots->add(this);
}

Hotspot::~Hotspot() {
}

void Hotspot::dispose() {
	hotspots->release(this);
	deathList->add(this);
}

void Hotspot::addRespondVerb(int v) {
	unsigned short *p = respond_verbs;
	int i;

	for (i = 0; i < respond_size; i++, p++) {
		if (*p == v)
			return;
	}
	if (i < MAX_RESPONSE_VERB) {
		respond_verbs[i] = v;
		respond_size++;
	} else {
		sfxPrintf("addRespondVerb of %s exceeds MAX_RESPONSE_VERB", name);
		sfxQuit();
	}
}

int Hotspot::checkRespondVerb() {
	unsigned short *p = respond_verbs;

	for (int i = 0; i < respond_size; i++, p++) {
		if ((*p == user->message) || (*p == ALL_V) || (*p == invVerb))
			return true;
	}

	return false;
}

int Hotspot::checkRespondVerb(int v) {
	unsigned short *p = respond_verbs;

	for (int i = 0; i < respond_size; i++, p++) {
		if (*p == v)
			return true;
	}
	return false;
}

ExitFeature::ExitFeature() {
	name = "ExitFeature";
}

void ExitFeature::aniCursor() {
	theGame->cursor->setCycle(true);
}

SouthExit::SouthExit() {
	myDelay = 10;//25;
	active = 0;
	name = "SouthExit";
}

int SouthExit::onMe(int theX, int theY) {
	if (!Feature::onMe(theX, theY)) {
		time0 = 0;
		active = false;
		return false;
	} else if (active)
		return true;
	if ((!time0) && (!active))
		time0 = gameTime;
	if ((!myDelay) || ((gameTime - time0) > myDelay) && (!active)) {
		time0 = 0;
		active = true;
		return true;
	}
	return false;
}

TextFeature::TextFeature() {
	name = "TextFeature";
}

void TextFeature::init(int theX, int theY, int theWidth, int theHeight) {
	Feature::init(theX, theY, theWidth, theHeight);
	myPrint = new(Print);
	myPrint->x = theX + 10;
	myPrint->y = theY + 10;
	myPrint->fore_color = TEXT_COLOR_GREEN;//75;//1;
	myPrint->init(name);
}

void TextFeature::dispose() {
	if (myPrint) {
		myPrint->dispose();
		myPrint = 0;
	}
	Feature::dispose();
}

FeatureList::FeatureList() {
	name = "features";
}

void Feature::doit() {
	Object::doit();
	if (myHelp)
		myHelp->doit();
}

void Feature::aniCursor() {
	theGame->cursor->setCycle(true);
	//normalCursor->blink(10);

}

void FeatureList::updateRects(int theX, int theY) {
	Node *n = head;
	Feature *obj;

	while (n) {
		obj = (Feature *)(n->data);
		if (obj->flag & OBJECT_LOCALIZED) {
			obj->left -= theX;
			obj->top -= theY;
			obj->right -= theX;
			obj->bottom -= theY;
		}
		n = n->next;
	}
}

int FeatureList::handleEvent(MADEEventStamp *evt) {
	Node *n = head;
	Feature *obj;

	while (n) {
		obj = (Feature *)(n->data);
		n = n->next;

		//DEBUG
		if (n)
			debugListObj = n->data;
		else
			debugListObj = nullptr;

		//DEBUG
		if ((obj->onMe(evt->cursor_pt.x, evt->cursor_pt.y)) &&
			(obj->handleEvent(evt))
			) {
			debugListObj = nullptr;
			return true;
		}
	}

	debugListObj = nullptr;
	return false;
}

HotspotList::HotspotList() {
	name = "hotspots";
}

void HotspotList::doit() {
	Node *n = head;
	Node *tmp;  //DEBUG
	Hotspot *obj = nullptr;

#ifndef DIRECTX
	if (!sfxIsMCIOpen()) {
#else
	if (!sfxIsMovieOpen()) {
#endif

		//DIRECT if (!sfxIsMCIOpen()) 	{
		while (n) {
			tmp = n; //DEBUG
			obj = (Hotspot *)(n->data);
			n = n->next;
			/* CHANGE HOTSPOT TO NO HOTSPOT */
			if (hotObj == obj) {
				if ((!obj->client->onMe(mouse_x, mouse_y)) && (!quitting)) {
					theGame->resetHotspot();
					hotObj = nullptr;
				}
			}

			/* CHANGE NO HOTSPOT TO HOTSPOT */
			else if (!hotObj && obj->client->onMe(mouse_x, mouse_y) &&
				obj->checkRespondVerb()
				)
			{
				hotObj = obj;
				obj->client->respond();
				return;
			}
			ADV_ASSERT(tmp->next == n, 0);
		}
	}
}

} // namespace Spycraft
