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

#ifndef ULTIMA5_VIEWS_KEYS_H
#define ULTIMA5_VIEWS_KEYS_H

#include "common/keyboard.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {

/**
 * Returns true for the keys that don't count as a keypress by themselves
 */
inline bool isModifierKey(Common::KeyCode key) {
	return key == Common::KEYCODE_LSHIFT || key == Common::KEYCODE_RSHIFT ||
		key == Common::KEYCODE_LCTRL || key == Common::KEYCODE_RCTRL ||
		key == Common::KEYCODE_LALT || key == Common::KEYCODE_RALT ||
		key == Common::KEYCODE_LMETA || key == Common::KEYCODE_RMETA ||
		key == Common::KEYCODE_CAPSLOCK || key == Common::KEYCODE_NUMLOCK ||
		key == Common::KEYCODE_SCROLLOCK;
}

/**
 * Returns the code the original game gave to a key: the arrow keys and page keys have
 * small numbers, the others are what's typed, with the letters in capitals
 */
inline int originalKey(const Common::KeyState &key) {
	switch (key.keycode) {
	case Common::KEYCODE_LEFT:
		return 1;
	case Common::KEYCODE_RIGHT:
		return 2;
	case Common::KEYCODE_UP:
		return 3;
	case Common::KEYCODE_DOWN:
		return 4;
	case Common::KEYCODE_HOME:
		return 0xD3;
	case Common::KEYCODE_END:
		return 0xD4;
	case Common::KEYCODE_PAGEUP:
		return 0xD5;
	case Common::KEYCODE_PAGEDOWN:
		return 0xD6;
	case Common::KEYCODE_ESCAPE:
		return 27;
	case Common::KEYCODE_RETURN:
	case Common::KEYCODE_KP_ENTER:
		return 13;
	case Common::KEYCODE_SPACE:
		return ' ';
	default:
		return key.ascii >= 'a' && key.ascii <= 'z' ? key.ascii - 'a' + 'A' : key.ascii;
	}
}

} // namespace Views
} // namespace Ultima5
} // namespace Ultima

#endif
