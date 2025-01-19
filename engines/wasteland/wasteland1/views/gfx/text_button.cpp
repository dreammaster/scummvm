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

#include "wasteland/wasteland1/views/gfx/text_button.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Views {
namespace Gfx {

TextButton::TextButton(UIElement *parent, const Common::String &name,
		const Common::String &text, Common::KeyCode keycode) :
		Button(parent, name, keycode), _text(text) {
}

TextButton::TextButton(UIElement *parent, const Common::String &name,
		const Common::String &text, int x, int y, Common::KeyCode keycode) :
		Button(parent, name, keycode), _text(text) {
	setBounds(Window(x - 1, y, x + _text.size(), y));
	_bounds.setBorderSize(FONT_W, 0, FONT_W, 0);
}

TextButton::TextButton(UIElement *parent, const Common::String &name,
	const Common::String &text, KeybindingAction action) :
	Button(parent, name, action), _text(text) {
}

TextButton::TextButton(UIElement *parent, const Common::String &name,
		const Common::String &text, int x, int y, KeybindingAction action) :
		Button(parent, name, action), _text(text) {
	setBounds(Window(x - 1, y, x + _text.size(), y));
	_bounds.setBorderSize(FONT_W, 0, FONT_W, 0);
}

void TextButton::draw() {
	Surface s = getSurface(_bounds);

	s.setFont(1);
	int firstChar = _focused ? 24 : 52;

	TextButton *btnLeft = leftSideButton();
	TextButton *btnRight = rightSideButton();

	s.writeChar(btnLeft ? (btnLeft->isFocused() ? 24 : 52) - 'A' + 'Z' : 23);

	for (uint x = 0; x < _text.size(); ++x)
		s.writeChar(toupper(_text[x]) - 'A' + firstChar);

	s.writeChar(btnRight ? firstChar - 'A' + 'Z' : firstChar - 'A' + 'Q');
}

TextButton *TextButton::leftSideButton() const {
	for (auto *child : _parent->getChildren()) {
		TextButton *btn = dynamic_cast<TextButton *>(child);
		if (btn && btn->_bounds.top == _bounds.top &&
			btn->_bounds.right == (_bounds.left + FONT_W))
			return btn;
	}

	return nullptr;
}

TextButton *TextButton::rightSideButton() const {
	for (auto *child : _parent->getChildren()) {
		TextButton *btn = dynamic_cast<TextButton *>(child);
		if (btn && btn->_bounds.top == _bounds.top &&
			btn->_bounds.left == (_bounds.right - FONT_W))
			return btn;
	}

	return nullptr;
}

} // namespace Gfx
} // namespace Views
} // namespace Wasteland1
} // namespace Wasteland
