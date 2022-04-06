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

#include "common/system.h"
#include "common/config-manager.h"
#include "dink/events.h"
#include "dink/globals.h"

namespace Dink {

EventsManager *g_events;

EventsManager::EventsManager() {
	g_events = this;
	_keys.resize(Common::KEYCODE_LAST);
	Common::fill(&_joystickAxis[0], &_joystickAxis[32], 0);
	Common::fill(&_joystickButton[0], &_joystickButton[32], 0);
}

EventsManager::~EventsManager() {
	g_events = nullptr;
}

void EventsManager::pollEvents() {
	Common::Event e;
	g_system->delayMillis(10);

	while (g_system->getEventManager()->pollEvent(e)) {
		switch (e.type) {
		case Common::EVENT_JOYAXIS_MOTION:
			assert(e.joystick.axis < 32);
			_joystickAxis[e.joystick.axis] = e.joystick.position;
			break;

		case Common::EVENT_JOYBUTTON_DOWN:
			assert(e.joystick.button < 32);
			_joystickButton[e.joystick.button] = true;
			break;

		case Common::EVENT_JOYBUTTON_UP:
			assert(e.joystick.button < 32);
			_joystickButton[e.joystick.button] = false;
			break;

		case Common::EVENT_KEYDOWN:
			updateKeys(e, true);
			//_keyEvents.push(e);
			break;

		case Common::EVENT_KEYUP:
			updateKeys(e, false);
			break;

		default:
			if (e.type == Common::EVENT_MOUSEMOVE) {
				_mousePos = Common::Point(e.mouse.x, e.mouse.y);

				UpdateCursorPosition(e.mouse.x, 0);
				UpdateCursorPosition(0, e.mouse.y);
			} else {
				if (e.type == Common::EVENT_LBUTTONDOWN)
					mouse1 = true;
			}
			break;
		}
	}
}

void EventsManager::UpdateCursorPosition(int dx, int dy) {
	/*
	*  Pick up any leftover fuzz from last time.  This is important
	*  when scaling down mouse motions.  Otherwise, the user can
	*  drag to the right extremely slow for the length of the table
	*  and not get anywhere.
	*/
	if (spr[1].active) if (spr[1].brain == 13) {
		spr[1].x += dx;
		spr[1].y += dy;
		/* Clip the cursor to our client area */

		if (spr[1].x > 640) spr[1].x = 640;
		if (spr[1].y > 480) spr[1].y = 480;
		if (spr[1].x < 0) spr[1].x = 0;
		if (spr[1].y < 0) spr[1].y = 0;
	}
	if (mode == 1) {
		play.mouse += dy;
		//Msg("play mousey is now %d", play.mouse);
	}

}

void EventsManager::clearEvents() {
	Common::Event e;
	while (g_system->getEventManager()->pollEvent(e)) {
	}
}


bool EventsManager::isModifierKey(const Common::KeyCode &keycode) const {
	return keycode == Common::KEYCODE_LCTRL || keycode == Common::KEYCODE_LALT
	       || keycode == Common::KEYCODE_RCTRL || keycode == Common::KEYCODE_RALT
	       || keycode == Common::KEYCODE_LSHIFT || keycode == Common::KEYCODE_RSHIFT
	       || keycode == Common::KEYCODE_LSUPER || keycode == Common::KEYCODE_RSUPER
	       || keycode == Common::KEYCODE_CAPSLOCK || keycode == Common::KEYCODE_NUMLOCK
	       || keycode == Common::KEYCODE_SCROLLOCK;
}

bool EventsManager::isExtendedKey(const Common::KeyCode &keycode) const {
	const Common::KeyCode EXTENDED_KEYS[] = {
		Common::KEYCODE_F1, Common::KEYCODE_F2, Common::KEYCODE_F3,
		Common::KEYCODE_F4, Common::KEYCODE_F5, Common::KEYCODE_F6,
		Common::KEYCODE_F7, Common::KEYCODE_F8, Common::KEYCODE_F9,
		Common::KEYCODE_F10, Common::KEYCODE_F11, Common::KEYCODE_F12,
		Common::KEYCODE_KP0, Common::KEYCODE_KP1, Common::KEYCODE_KP2,
		Common::KEYCODE_KP3, Common::KEYCODE_KP4, Common::KEYCODE_KP5,
		Common::KEYCODE_KP6, Common::KEYCODE_KP7, Common::KEYCODE_KP8,
		Common::KEYCODE_KP9, Common::KEYCODE_KP_PERIOD,
		Common::KEYCODE_INSERT, Common::KEYCODE_DELETE,
		Common::KEYCODE_HOME, Common::KEYCODE_END,
		Common::KEYCODE_PAGEUP, Common::KEYCODE_PAGEDOWN,
		Common::KEYCODE_LEFT, Common::KEYCODE_RIGHT,
		Common::KEYCODE_UP, Common::KEYCODE_DOWN,
		Common::KEYCODE_INVALID
	};

	for (const Common::KeyCode *kc = EXTENDED_KEYS;
	        *kc != Common::KEYCODE_INVALID; ++kc) {
		if (keycode == *kc)
			return true;
	}

	return false;
}

void EventsManager::updateKeys(const Common::Event &event, bool isDown) {
	_keyModifierFlags = event.kbd.flags;

	_keys[event.kbd.keycode] = isDown ? -1 : 0;
}

short EventsManager::getAsyncState(int key) {
	short result = _keys[key];
	_keys[key] &= ~1;
	return result;
}

uint32 EventsManager::getTickCount() const {
	return g_system->getMillis();
}

void EventsManager::sleep(size_t numSeconds) {
	g_system->delayMillis(numSeconds * 1000);
}

bool CheckJoyStickPresent() {
	int joystickNum = ConfMan.getInt("joystick_num");
	return joystickNum != -1;
}

bool getkey(int key) {
	if (sjoy.realkey[key]) return true;
	else return false;

}

int GetKeyboard(int key) {
	return g_events->getAsyncState(key);
}

char key_convert(int key) {
	if (!getkey(16)) key = tolower(key);

	if (key == 190) if (getkey(16)) key = '>';
	else key = '.';
	if (key == 188) if (getkey(16)) key = '<';
	else key = ',';

	if (key == 49) if (getkey(16)) key = '!';
	if (key == 50) if (getkey(16)) key = '@';
	if (key == 51) if (getkey(16)) key = '#';
	if (key == 52) if (getkey(16)) key = '$';
	if (key == 53) if (getkey(16)) key = '%';
	if (key == 54) if (getkey(16)) key = '^';
	if (key == 55) if (getkey(16)) key = '&';
	if (key == 56) if (getkey(16)) key = '*';
	if (key == 57) if (getkey(16)) key = '(';
	if (key == 48) if (getkey(16)) key = ')';

	if (key == 189) if (getkey(16)) key = '_';
	else key = '-';
	if (key == 187) if (getkey(16)) key = '+';
	else key = '=';
	if (key == 186) if (getkey(16)) key = ':';
	else key = ';';
	if (key == 222) if (getkey(16)) key = '\"';
	else key = '\'';
	if (key == 191) if (getkey(16)) key = '?';
	else key = '/';
	if (key == 220) if (getkey(16)) key = '|';
	else key = '\\';

	return key;
}

} // namespace Dink
