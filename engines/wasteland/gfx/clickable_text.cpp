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

#include "wasteland/gfx/clickable_text.h"
#include "wasteland/gfx/wasteland_font.h"

namespace Wasteland {
namespace Gfx {

ClickableText::ClickableText(const Common::String &name, UIElement *owner, int x, int y,
		const Common::String &text, Common::KeyCode keycode) : UIElement(name, owner),
		_text(text), _keycode(keycode) {
	setBounds(Common::Rect(x * FONT_W, y * FONT_H, (x + text.size()) * FONT_W, (y + 1) * FONT_H));
}


void ClickableText::draw() {
	Surface s = getSurface();
	s.writeString(_text);
}

bool ClickableText::msgMouseDown(const MouseDownMessage& msg) {
	if (_bounds.contains(msg._pos)) {
		Common::KeyState keyState(_keycode);
		_parent->send(KeypressMessage(keyState));
		return true;
	}

	return false;
}

} // namespace Gfx
} // namespace Wasteland
