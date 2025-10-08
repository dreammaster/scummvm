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

#include "spycraft/game/actor.h"
#include "spycraft/game/game.h"
#include "spycraft/game/user.h"

namespace Spycraft {

extern int quitting;
Cast *cast;
extern Object *debugListObj;

View::View() {
	name = "View";
	priority = 5;
	mySprite = NULL;
	myPrint = 0;
	myATS = NULL;
	loop = 0;
	cel = 0;
}

View::~View() {
	int ttt;
	if (mySprite != NULL) {
		if (flag & OBJECT_HIDDEN)
			show();
		if ((mySprite->port == NULL) || (mySprite->check != 99999)) {
			ttt = 1;
			ttt = 2;

			//	ADV_ASSERT(0,0);
		}
		sfxKillSprite(mySprite);
		sfxUnlockRes(view, RES_ATS);	// THIS is IMPORTANT if this is a cursor MADE will unlock ats
										// it is a cursor if mySprite started at NULL
		mySprite = NULL;
	}

	if (myPrint) {
		myPrint->dispose();
		myPrint = 0;
	}
}



void View::init(int theX, int theY) {
	x = theX;
	y = theY;
	draw();
	cast->add(this);
}

void View::init(int theX, int theY, int showName) {
	x = theX;
	y = theY;
	draw();
	cast->add(this);

	myPrint = new(Print);
	myPrint->x = theX + 10;
	myPrint->y = theY + 10;
	myPrint->fore_color = TEXT_COLOR_GREEN;
	myPrint->init(name);
}

void View::addToPic() {
	if (mySprite) {
		sfxSpriteAddToPic(mySprite);
		sfxUnlockRes(view, RES_ATS);
	}
	mySprite = NULL;
}

void View::noSkip() {
	if (mySprite != NULL)
		mySprite->state &= ~SPRITE_MASK;
}

void View::show() {
	if (mySprite != NULL) {
		sfxShowSprite(mySprite);
		flag &= ~OBJECT_HIDDEN;
	}
}

void View::hide() {
	if (mySprite != NULL) {
		sfxHideSprite(mySprite);
		flag |= OBJECT_HIDDEN;
	}
}

void View::draw() {
	Viewport *port;

	myATS = (ATS *)sfxLoadRes(view, RES_ATS);
	sfxLockRes(view, RES_ATS);

	if (loop >= myATS->numReels) {
		sfxPrintf("view: %d, doesn't have loop: %d", view, loop);
		ADV_ASSERT(0, 0);
	}
	port = sfxAnimFrame(myATS, loop, cel);

	mySprite = sfxCreateSprite(theGame->display_buffer, x, y, 512, 512, port);

	if (mySprite == NULL) {
		sfxPrintf("Couldn't create sprite in View::draw()");
		return;
	}

	sfxAttachATS2Sprite(mySprite, myATS, loop, cel);
	setRect();
}

void View::draw(bufferNum theBuffer) {
	Viewport *port;

	myATS = (ATS *)sfxLoadRes(view, RES_ATS);
	sfxLockRes(view, RES_ATS);

	port = sfxAnimFrame(myATS, loop, cel);
	mySprite = sfxCreateSprite(theBuffer, x, y, 512, 512, port);

	if (mySprite == NULL) {
		sfxPrintf("Couldn't create sprite in View::draw()");
		return;
	}
	setRect();
}

void View::setRect() {
	left = mySprite->rect.left;
	top = mySprite->rect.top;
	right = mySprite->rect.right;
	bottom = mySprite->rect.bottom;
	height = mySprite->rect.height;
	width = mySprite->rect.width;
}

void View::setPri(uint16 pri) {
	priority = pri;
	if (mySprite)
		sfxSpriteChannel(mySprite, pri);
}

void View::posn(int theX, int theY) {
	if (mySprite) {
		x = theX;
		y = theY;
		sfxMoveSprite(mySprite, x, y);
		setRect();
	}
}

void View::setView(int theView) {
	if (mySprite != NULL) {
		if ((mySprite->port == NULL) || (mySprite->check != 99999))
			ADV_ASSERT(0, 0);

		sfxKillSprite(mySprite);
		sfxUnlockRes(view, RES_ATS);
		view = theView;
		draw();
	}
	view = theView;
}

void View::setLoop(int theLoop) {
	if (mySprite) {
		loop = theLoop;
		cel = 0;

		sfxSpriteFrame(mySprite, sfxAnimFrame(myATS, loop, cel));
		setRect();
	} else
		loop = theLoop;
}

int View::onMe(int theX, int theY) {
	if (flag & OBJECT_HIDDEN)
		return false;
	return Feature::onMe(theX, theY);
}

int View::onMe(MADEEventStamp *event) {
	long theX, theY;
	theX = event->cursor_pt.x;
	theY = event->cursor_pt.y;
	return Feature::onMe(event);
}

int View::onMe(Feature *obj) {
	return Feature::onMe(obj);
}

void Prop::setLoop(int theLoop) {
	if (mySprite) {
		loop = theLoop;
		cel = 0;

		sfxSpriteFrame(mySprite, sfxAnimFrame(myATS, loop, cel));
		setRect();
		if (cycler) {
			cycler->lastCel = sfxAnimFrames(myATS, loop) - 1;
			cycler->curCel = 0;
		}
	}
}

void View::setCel(int theCel) {
	if ((mySprite != NULL) && (!quitting)) {
		cel = theCel;
		sfxSpriteFrame(mySprite, sfxAnimFrame(myATS, loop, cel));
		setRect();
	} else
		cel = theCel;
}

void View::dispose() {
	if (hotspot) {
		setHotspot(NULL);
		hotspot = NULL;
	}

	cast->release(this);
	deathList->add(this);
}

Cast::Cast() {
	name = "cast";
}

int Cast::handleEvent(MADEEventStamp *evt) {
	Node *n = head;
	View *obj;

	while (n) {
		obj = (View *)(n->data);
		n = n->next;

		if ((obj->onMe(evt->cursor_pt.x, evt->cursor_pt.y)) &&
			!(obj->flag & OBJECT_HIDDEN) && (obj->handleEvent(evt))
			) {
			debugListObj = NULL;
			return true;
		}

	}

	debugListObj = NULL;
	return false;
}

void Cast::updateRects(int theX, int theY) {
	Node *n = head;
	View *obj;

	while (n) {
		obj = (View *)(n->data);
		if (obj->flag & OBJECT_LOCALIZED) {
			obj->x -= theX;
			obj->y -= theY;
			obj->left = theX;
			obj->top = theY;
			obj->right = theX + obj->width;
			obj->bottom = theY + obj->height;
			obj->draw();
		}

		n = n->next;
	}
}

Prop::Prop() {
	name = "Prop";
	cycler = NULL;
	cycleSpeed = 10;
	__cycle_cnt = 0;
}

Prop::~Prop() {
	if (cycler) {
		delete cycler;
		cycler = NULL;
	}
}

void Prop::setCycle(Cycle *theCycle) {
	if (cycler)
		delete cycler;

	cycler = theCycle;
}

void Prop::doit() {

	View::doit();

	if (cycler && ((gameTime - __cycle_cnt) > (unsigned)cycleSpeed)) {
		__cycle_cnt = gameTime;

		int oldCel = cel;
		cycler->doit();
		if ((oldCel != cel) && (cycler) && (!quitting)) {
			//sfxSpriteFrame(mySprite, myATS->reels[loop]->frames[cel]);
			sfxSpriteFrame(mySprite, sfxAnimFrame(myATS, loop, cel));
			setRect();
		}
	}
}

} // namespace Spycraft
