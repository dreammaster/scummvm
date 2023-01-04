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

#ifndef LEGEND_MESSAGES_H
#define LEGEND_MESSAGES_H

#include "common/array.h"
#include "common/events.h"
#include "legend/utils/string.h"

namespace Legend {

struct Message {};
struct FocusMessage : public Message {};
struct UnfocusMessage : public Message {};

struct KeypressMessage : public Message, public Common::KeyState {
	KeypressMessage() : Message() {}
	KeypressMessage(const Common::KeyState &ks) :
		Message(), Common::KeyState(ks) {}
};

struct MouseMessage : public Message {
	enum Button { MB_LEFT, MB_RIGHT, MB_MIDDLE };
	Button _button;
	Common::Point _pos;

	MouseMessage() : Message(), _button(MB_LEFT) {}
	MouseMessage(Button btn, const Common::Point &pos) :
		Message(), _button(btn), _pos(pos) {}
	MouseMessage(Common::EventType type, const Common::Point &pos);
};
struct MouseDownMessage : public MouseMessage {
	MouseDownMessage() : MouseMessage() {}
	MouseDownMessage(Button btn, const Common::Point &pos) :
		MouseMessage(btn, pos) {}
	MouseDownMessage(Common::EventType type, const Common::Point &pos) :
		MouseMessage(type, pos) {}
};
struct MouseUpMessage : public MouseMessage {
	MouseUpMessage() : MouseMessage() {}
	MouseUpMessage(Button btn, const Common::Point &pos) :
		MouseMessage(btn, pos) {}
	MouseUpMessage(Common::EventType type, const Common::Point &pos) :
		MouseMessage(type, pos) {}
};
struct MouseWheelMessage : public MouseMessage {
	bool _wheelUp;

	MouseWheelMessage() : MouseMessage(MB_MIDDLE, Common::Point()), _wheelUp(false) {}
	MouseWheelMessage(bool wheelUp, const Common::Point &pos) :
		MouseMessage(MB_MIDDLE, pos), _wheelUp(wheelUp) {
	}
};
struct MouseDragMessage : public MouseMessage {
	MouseDragMessage() : MouseMessage() {}
	MouseDragMessage(const Common::Point &pos) :
		MouseMessage(MB_LEFT, pos) {}
};

struct GameMessage : public Message {
	String _name;
	int _value;
	String _stringValue;

	GameMessage() : Message(), _value(-1) {}
	GameMessage(const String &name) : Message(),
		_name(name), _value(-1) {}
	GameMessage(const String &name, int value) : Message(),
		_name(name), _value(value) {}
	GameMessage(const String &name, const String &value) :
		Message(), _name(name), _stringValue(value) {}
};

struct ValueMessage : public Message {
	int _value;

	ValueMessage() : Message(), _value(0) {}
	ValueMessage(int value) : Message(),
		_value(value) {}
};

struct ActionMessage : public Message {
	int _action;
	ActionMessage() : Message(), _action(0) {
	}
	ActionMessage(int action) : Message(),
		_action(action) {
	}
};

} // namespace Legend

#endif
