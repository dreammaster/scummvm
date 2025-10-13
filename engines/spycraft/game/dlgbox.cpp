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

#include "spycraft/game/dlgbox.h"

namespace Spycraft {

DlgBox *dialogbox;
List *dialogList;

DlgBox::DlgBox() {
	intrpuzz->puzzle_focus = DIALOGBOX_FOCUS;
	dialogbox = this;
	dialogList = new(List);
}

void DlgBox::init(int theX, int theY, int pic_id, int loop_id, int cel_id) {
	origin_x = theX;
	origin_y = theY;
	x = theX;
	y = theY;
	view = pic_id;
	loop = loop_id;
	cel = cel_id;
	draw();
	dialogList->addToFront(this);
}

void DlgBox::init(int theX, int theY) {
	origin_x = theX;
	origin_y = theY;
	x = theX;
	y = theY;
	draw();
	dialogList->addToFront(this);
}

void DlgBox::dispose() {
	if (!dialogList->isEmpty())
		dialogList->dispose();

	delete dialogList;
	intrpuzz->puzzle_focus = 0;
}

void DialogView::init(int theX, int theY) {
	x = dialogbox->origin_x + theX;
	y = dialogbox->origin_y + theY;
	draw();
	dialogList->addToFront(this);
}

void DialogView::dispose() {
	dialogList->release(this);
	deathList->add(this);
}

void DialogButton::init(int theX, int theY, int pic_id, int loop_id, int cel_id) {
	x = dialogbox->origin_x + theX;
	y = dialogbox->origin_y + theY;
	view = pic_id;
	loop = loop_id;
	cel = cel_id;
	draw();
	dialogList->addToFront(this);
}

void DialogButton::init(int theX, int theY) {
	x = dialogbox->origin_x + theX;
	y = dialogbox->origin_y + theY;
	draw();
	dialogList->addToFront(this);
}

int DialogButton::handleEvent(MADEEventStamp *event) {
	if (onMe(event)) {
		if (user->input) {
			if (event_type == USER_LEFT_UP) {
				if (cel == 1) {
					setCel(0);
					activate();
					return(true);
				}
			} else if (event_type == USER_LEFT_DOWN) {
				if (cel == 0) {
					setCel(1);
					return true;
				}
			}
		}
	} else {
		if (cel == 1) {
			setCel(0);
		}
	}

	return false;
}

void DialogButton::activate() {
}

/***********************************
 *
 *		  Dialog Feature
 *
 **********************************/
void DialogFeature::init(int theX, int theY, int theWidth, int theHeight) {
	left = dialogbox->origin_x + theX;
	top = dialogbox->origin_y + theY;
	right = left + theWidth - 1;
	bottom = top + theHeight - 1;

	dialogList->addToFront(this);
}

void DialogFeature::dispose() {
	if (hotspot) {
		setHotspot(NULL);
		hotspot = NULL;
	}

	dialogList->release(this);
	deathList->add(this);
}

/***********************************
 *
 *		  Dialog Text Feature
 *
 **********************************/
void DialogText::init(int theX, int theY, int width, int height, int color) {
	left = dialogbox->origin_x + theX;
	top = dialogbox->origin_y + theY;
	right = left + width - 1;
	bottom = top + height - 1;

	myPrint = new(Print);
	myPrint->x = left;
	myPrint->y = top;
	myPrint->priority = 610;
	myPrint->size = 11;
	myPrint->fore_color = color;
	myPrint->style = STYL_PLAIN;
	myPrint->init(name);

	dialogList->addToFront(this);
}

void DialogText::dispose() {
	dialogList->release(this);
	deathList->add(this);
}

DialogText::~DialogText() {
	if (myPrint)
		myPrint->erase();
}

} // namespace Spycraft
