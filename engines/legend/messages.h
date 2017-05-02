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

#include "common/keyboard.h"
#include "common/rect.h"
#include "legend/core/saveable_object.h"
#include "legend/core/tree_item.h"

namespace Legend {

enum MessageFlag {
	MSGFLAG_SCAN = 1,
	MSGFLAG_BREAK_IF_HANDLED = 2,
	MSGFLAG_CLASS_DEF = 4
};

class Message;

#define MESSAGEDEF(theClass) \
	static ClassDef type() { return ClassDef(#theClass, &Message::type); } \
	virtual ClassDef getType() const { return type(); }
#define MESSAGEDEFP(theClass, baseClass) \
	static ClassDef type() { return ClassDef(#theClass, &baseClass::type); } \
	virtual ClassDef getType() const { return type(); }

#define MESSAGE0(NAME) \
	class NAME: public Message { \
	public: NAME() : Message() {} \
	MESSAGEDEF(NAME); \
	}
#define MESSAGE1(NAME, F1, N1, V1) \
	class NAME: public Message { \
	public: F1 _##N1; \
	NAME() : Message(), _##N1(V1) {} \
	NAME(F1 N1) : Message(), _##N1(N1) {} \
	MESSAGEDEF(NAME); \
	}
#define MESSAGE2(NAME, F1, N1, V1, F2, N2, V2) \
	class NAME: public Message { \
	public: F1 _##N1; F2 _##N2; \
	NAME() : Message(), _##N1(V1), _##N2(V2) {} \
	NAME(F1 N1, F2 N2) : Message(), _##N1(N1), _##N2(N2) {} \
	MESSAGEDEF(NAME); \
	}
#define MESSAGE3(NAME, F1, N1, V1, F2, N2, V2, F3, N3, V3) \
	class NAME: public Message { \
	public: F1 _##N1; F2 _##N2; F3 _##N3; \
	NAME() : Message(), _##N1(V1), _##N2(V2), _##N3(V3) {} \
	NAME(F1 N1, F2 N2, F3 N3) : Message(), _##N1(N1), _##N2(N2), _##N3(N3) {} \
	MESSAGEDEF(NAME); \
	}
#define MESSAGE4(NAME, F1, N1, V1, F2, N2, V2, F3, N3, V3, F4, N4, V4) \
	class NAME: public Message { \
	public: F1 _##N1; F2 _##N2; F3 _##N3; F4 _##N4; \
	NAME() : Message(), _##N1(V1), _##N2(V2), _##N3(V3), _##N4(V4) {} \
	NAME(F1 N1, F2 N2, F3 N3, F4 N4) : Message(), _##N1(N1), _##N2(N2), _##N3(N3), _##N4(N4) {} \
	MESSAGEDEF(NAME); \
	}

class Message : public SaveableObject {
private:
	/**
	 * Find a map entry that supports the given class
	 */
	static const MSGMAP_ENTRY *findMapEntry(const TreeItem *treeItem, const ClassDef &classDef);
public:
	MESSAGEDEFP(Message, SaveableObject);
	Message();

	/**
	 * Executes the message, passing it on to the designated target,
	 * and optionally it's children
	 */
	bool execute(TreeItem *target, const ClassDef *classDef = nullptr,
		int flags = MSGFLAG_SCAN | MSGFLAG_BREAK_IF_HANDLED);

	/**
	 * Executes the message, passing it on to the designated target,
	 * and optionally it's children
	 */
	bool execute(const Common::String &target, const ClassDef *classDef = nullptr,
		int flags = MSGFLAG_SCAN | MSGFLAG_BREAK_IF_HANDLED);

	/**
	 * Makes the passed item execute the message
	 */
	virtual bool perform(TreeItem *treeItem);

	/**
	 * Returns true if the passed item supports the specified message class
	 */
	static bool supports(const TreeItem *treeItem, const ClassDef &classDef);

	/**
	 * Save the data for the class to file
	 */
	virtual void save(SimpleFile *file, int indent);

	/**
	 * Load the data for the class from file
	 */
	virtual void load(SimpleFile *file);

	virtual bool isMouseMsg() const;
	virtual bool isButtonDownMsg() const;
	virtual bool isButtonUpMsg() const;
	virtual bool isMouseMoveMsg() const;
	virtual bool isDoubleClickMsg() const;
};


enum MouseButton { MB_LEFT = 1, MB_MIDDLE = 2, MB_RIGHT = 4 };

class MouseMsg : public Message {
public:
	int _buttons;
	Common::Point _mousePos;
public:
	MESSAGEDEF(MouseMsg);

	MouseMsg() : _buttons(0) {}
	MouseMsg(const Common::Point &pt, int buttons) :
		_mousePos(pt), _buttons(buttons) {}
};

class MouseMoveMsg : public MouseMsg {
public:
	MESSAGEDEFP(MouseMoveMsg, MouseMsg);
	MouseMoveMsg() : MouseMsg() {}
	MouseMoveMsg(const Common::Point &pt, int buttons) : MouseMsg(pt, buttons) {}
};

class MouseButtonMsg : public MouseMsg {
public:
	int _field10;
public:
	MESSAGEDEFP(MouseButtonMsg, MouseMsg);
	MouseButtonMsg() : MouseMsg(), _field10(0) {}
	MouseButtonMsg(const Common::Point &pt, int buttons) : MouseMsg(pt, buttons) {}
};

class MouseButtonDownMsg : public MouseButtonMsg {
public:
	MESSAGEDEFP(MouseButtonDownMsg, MouseButtonMsg);
	MouseButtonDownMsg() : MouseButtonMsg() {}
	MouseButtonDownMsg(const Common::Point &pt, int buttons) : MouseButtonMsg(pt, buttons) {}
};

class MouseButtonUpMsg : public MouseButtonMsg {
public:
	MESSAGEDEFP(MouseButtonUpMsg, MouseButtonMsg);
	MouseButtonUpMsg() : MouseButtonMsg() {}
	MouseButtonUpMsg(const Common::Point &pt, int buttons) : MouseButtonMsg(pt, buttons) {}
};

class MouseWheelMsg : public MouseMsg {
public:
	bool _wheelUp;
public:
	MESSAGEDEFP(MouseWheelMsg, MouseMsg);
	MouseWheelMsg() : MouseMsg(), _wheelUp(false) {}
	MouseWheelMsg(const Common::Point &pt, bool wheelUp) :
		MouseMsg(pt, 0), _wheelUp(wheelUp) {}
};

class MouseDoubleClickMsg : public MouseButtonMsg {
public:
	MESSAGEDEFP(MouseDuobleClickMsg, MouseButtonMsg);
	MouseDoubleClickMsg() : MouseButtonMsg() {}
	MouseDoubleClickMsg(const Common::Point &pt, int buttons) : MouseButtonMsg(pt, buttons) {}
};

MESSAGE1(CKeyCharMsg, int, key, 32);
MESSAGE1(CVirtualKeyCharMsg, Common::KeyState, keyState, Common::KeyState());

} // End of namespace Legend

#endif /* LEGEND_MESSAGES_H */
