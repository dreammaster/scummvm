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

Button::Button(UIElement *parent, const Common::String &name, const Common::String &text) :
	UIElement(name, parent), _text(text) {
}

Button::Button(UIElement *parent, const Common::String &name, const Common::String &text, int x, int y) :
	UIElement(name, parent), _text(text) {
	setBounds(Window(x - 1, y, x + _text.size(), y));
	_bounds.setBorderSize(FONT_W, 0, FONT_W, 0);
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

void Button::draw() {
	Surface s = getSurface(_bounds);

	s.setFont(1);
	int firstChar = _focused ? 52 : 24;

	Button *btnLeft = leftSideButton();
	Button *btnRight = rightSideButton();

	s.writeChar(btnLeft ? (btnLeft->isFocused() ? 52 : 24) - 'A' + 'Z' : 23);

	for (uint x = 0; x < _text.size(); ++x)
		s.writeChar(toupper(_text[x]) - 'A' + firstChar);

	s.writeChar(btnRight ? firstChar - 'A' + 'Z' : firstChar - 'A' + 'Q');
}

bool Button::msgMouseDown(const MouseDownMessage &msg) {
	_parent->send(GameMessage(_name));
	return true;
}

Button *Button::leftSideButton() const {
	for (auto *child : _parent->getChildren()) {
		Button *btn = dynamic_cast<Button *>(child);
		if (btn && btn->_bounds.top == _bounds.top &&
				btn->_bounds.right == (_bounds.left + FONT_W))
			return btn;
	}

	return nullptr;
}

Button *Button::rightSideButton() const {
	for (auto *child : _parent->getChildren()) {
		Button *btn = dynamic_cast<Button *>(child);
		if (btn && btn->_bounds.top == _bounds.top &&
			btn->_bounds.left == (_bounds.right - FONT_W))
			return btn;
	}

	return nullptr;
}
} // namespace Views
} // namespace Wasteland1
} // namespace Wasteland
