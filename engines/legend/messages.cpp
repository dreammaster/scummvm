/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "legend/messages.h"
#include "legend/legend.h"
#include "legend/core/screen.h"
#include "legend/core/screen_area.h"

namespace Legend {

LegendEngine *Message::_vm;

void Message::execute() {
	Screen &screen = *_vm->_screen;
	Common::List<ScreenArea *>::iterator i;
	for (i = screen._screenAreas.begin(); i != screen._screenAreas.end(); ++i) {
		perform(*i);
	}
}

/*-------------------------------------------------------------------*/

bool MouseMoveMsg::handleMessage(MouseMoveMsg &msg) {
	return false;
}

bool MouseMoveMsg::perform(ScreenArea *screenArea) {
	MouseMoveMsg *dest = dynamic_cast<MouseMoveMsg *>(screenArea);
	return dest ? dest->handleMessage(*this) : false;
}

/*-------------------------------------------------------------------*/

bool MouseButtonDownMsg::handleMessage(MouseButtonDownMsg &msg) {
	return false;
}

bool MouseButtonDownMsg::perform(ScreenArea *screenArea) {
	MouseButtonDownMsg *dest = dynamic_cast<MouseButtonDownMsg *>(screenArea);
	return dest ? dest->handleMessage(*this) : false;
}

/*-------------------------------------------------------------------*/

bool MouseButtonUpMsg::handleMessage(MouseButtonUpMsg &msg) {
	return false;
}

bool MouseButtonUpMsg::perform(ScreenArea *screenArea) {
	MouseButtonUpMsg *dest = dynamic_cast<MouseButtonUpMsg *>(screenArea);
	return dest ? dest->handleMessage(*this) : false;
}

} // End of namespace Legend
