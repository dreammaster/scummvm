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

#include "wasteland/wasteland1/views/gfx/button.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Views {
namespace Gfx {

Button::Button(UIElement *parent, const Common::String &name,
		Common::KeyCode keycode) : UIElement(name, parent) {
	_keycodes.push_back(keycode);
}

Button::Button(UIElement *parent, const Common::String &name,
		KeybindingAction action) : UIElement(name, parent),
		_action(action) {
}

bool Button::msgMouseEnter(const MouseEnterMessage &msg) {
	_focused = true;
	g_events->setCursor(1);

	draw();
	return true;
}

bool Button::msgMouseLeave(const MouseLeaveMessage &msg) {
	_focused = false;
	g_events->setCursor(0);

	draw();
	return true;
}

bool Button::msgMouseDown(const MouseDownMessage &msg) {
	_parent->send(GameMessage(_name));
	return true;
}

bool Button::msgKeypress(const KeypressMessage &msg) {
	for (uint i = 0; i < _keycodes.size(); ++i) {
		if (_keycodes[i] == msg.keycode) {
			_parent->send(GameMessage(_name));
			return true;
		}
	}

	return false;
}

bool Button::msgAction(const ActionMessage &msg) {
	if (_action != KEYBIND_NONE && msg._action == _action) {
		_parent->send(GameMessage(_name));
		return true;
	}

	return false;
}

void Button::addKeycode(Common::KeyCode keycode) {
	_keycodes.push_back(keycode);
}

} // namespace Gfx
} // namespace Views
} // namespace Wasteland1
} // namespace Wasteland
