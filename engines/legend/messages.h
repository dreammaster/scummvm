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

#ifndef LEGEND_MESSAGES_H
#define LEGEND_MESSAGES_H

#include "common/scummsys.h"
#include "common/events.h"
#include "common/stack.h"
#include "legend/screen_area.h"

namespace Legend {

class LegendEngine;
class Message;
class MouseMoveMsg;
class MouseButtonDownMsg;
class MouseButtonUpMsg;
class KeyDownMsg;
class KeyUpMsg;

class Message {
private:
	static LegendEngine *_vm;
protected:
	/**
	 * Transfers the message to the passed screen area
	 */
	virtual bool perform(ScreenArea *screenArea) = 0;
public:
	Message() {}
	virtual ~Message() {}

	/**
	 * Initializes the static engine reference
	 */
	static void init(LegendEngine *vm) { _vm = vm; }

	/**
	 * Executes the message
	 */
	void execute();
};

class MouseMoveMsg : public Message {
protected:
	virtual bool handleMessage(MouseMoveMsg &msg);
	virtual bool perform(ScreenArea *screenArea);
public:
	Common::Point _mousePos;
public:
	MouseMoveMsg() : Message() {}
	MouseMoveMsg(const Common::Point &pt) : Message(), _mousePos(pt) {}
};

class MouseButtonMsg : public Message {
public:
	Common::Point _mousePos;
	int _buttons;
public:
	MouseButtonMsg() : Message() {}
	MouseButtonMsg(const Common::Point &pt, int buttons) : Message(),
		_mousePos(pt), _buttons(buttons) {}
};

class MouseButtonDownMsg : public MouseButtonMsg {
protected:
	virtual bool handleMessage(MouseButtonDownMsg &msg);
	virtual bool perform(ScreenArea *screenArea);
public:
	MouseButtonDownMsg() : MouseButtonMsg() {}
	MouseButtonDownMsg(const Common::Point &pt, int buttons) :
		MouseButtonMsg(pt, buttons) {}
};

class MouseButtonUpMsg : public MouseButtonMsg {
protected:
	virtual bool handleMessage(MouseButtonUpMsg &msg);
	virtual bool perform(ScreenArea *screenArea);
public:
	MouseButtonUpMsg() : MouseButtonMsg() {}
	MouseButtonUpMsg(const Common::Point &pt, int buttons) :
		MouseButtonMsg(pt, buttons) {}
};

} // End of namespace Legend

#endif /* LEGEND_MESSAGES_H */
