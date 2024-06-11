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

namespace Wasteland {
namespace Shared {
namespace Gfx {

ClickableText::ClickableText(const Common::String &name, UIElement *owner, int x, int y,
	const Common::String &text, Common::KeyCode keycode) : UIElement(name, owner),
	_text(text), _keycode(keycode) {
	setBounds(Common::Rect(x * FONT_W, y * FONT_H, (x + text.size()) * FONT_W, (y + 1) * FONT_H));
}

ClickableText::ClickableText(const Common::String &name, UIElement *owner, const Position &pos, const Common::String &text, Common::KeyCode keycode) : UIElement(name, owner),
_text(text), _keycode(keycode) {
	Common::Rect r(pos.x * FONT_W, pos.y * FONT_H, (pos.x + text.size()) * FONT_W, (pos.y + 1) * FONT_H);
	r.translate(pos._xOffset, pos._yOffset);
	setBounds(r);
}

ClickableText::ClickableText(const Common::String &name, UIElement *owner, int x, int y,
	const Common::String &text, const Common::String &message, int tag) : UIElement(name, owner),
	_text(text), _keycode(Common::KEYCODE_INVALID), _message(message), _tag(tag) {
	setBounds(Common::Rect(x * FONT_W, y * FONT_H, (x + text.size()) * FONT_W, (y + 1) * FONT_H));
}

ClickableText::ClickableText(const Common::String &name, UIElement *owner, const Position &pos,
	const Common::String &text, const Common::String &message, int tag) : UIElement(name, owner),
	_text(text), _keycode(Common::KEYCODE_INVALID), _message(message), _tag(tag) {
	Common::Rect r(pos.x * FONT_W, pos.y * FONT_H, (pos.x + text.size()) * FONT_W, (pos.y + 1) * FONT_H);
	r.translate(pos._xOffset, pos._yOffset);
	setBounds(r);
}

void ClickableText::setText(const Common::String &text) {
	_text = text;
	setBounds(Common::Rect(_bounds.left, _bounds.top, _bounds.left + text.size() * FONT_W, _bounds.bottom));
	redraw();
}

void ClickableText::draw() {
	Surface s = getSurface();
	s.setInverseColor(_inverseColor);
	s.writeString(_text);
}

bool ClickableText::msgMouseDown(const MouseDownMessage &msg) {
	if (!_bounds.contains(msg._pos))
		return false;

	if (_keycode != Common::KEYCODE_INVALID) {
		Common::KeyState keyState(_keycode);
		_parent->send(KeypressMessage(keyState));

	} else {
		const char *strParam = (msg._button == MouseDownMessage::MB_LEFT) ? "LEFT" : "RIGHT";
		_parent->send(GameMessage(_message, strParam, _tag));
	}

	return true;
}

} // namespace Gfx
} // namespace Shared
} // namespace Wasteland
