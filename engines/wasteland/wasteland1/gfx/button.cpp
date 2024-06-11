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

#include "wasteland/wasteland1/gfx/button.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Gfx {

bool Button::msgFocus(const FocusMessage &msg) {
	_focused = true;
	g_events->setCursor(1);

	draw();
	return true;
}

bool Button::msgUnfocus(const UnfocusMessage &msg) {
	_focused = false;
	g_events->setCursor(0);

	draw();
	return true;
}

void Button::draw() {
	Surface s = getSurface();
	s.setFont(1);
	int firstChar = _focused ? 52 : 24;

	for (uint x = 0; x < _text.size(); ++x)
		s.writeChar(toupper(_text[x]) - 'A' + firstChar);
}

bool Button::msgMouseDown(const MouseDownMessage &msg) {
	_parent->send(GameMessage(_name));
	return true;
}

} // namespace Gfx
} // namespace Wasteland1
} // namespace Wasteland
