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

#include "wasteland/gfx/text_entry.h"

namespace Wasteland {
namespace Shared {
namespace Gfx {

TextEntry::TextEntry(const Common::String &name, UIElement *uiParent, int x, int y, size_t maxLength) :
	UIElement(name, uiParent), _maxLength(maxLength) {
	setBounds(Common::Rect(x * FONT_W, y * FONT_H, (x + maxLength + 1) * FONT_W, (y + 1) * FONT_H));
}

TextEntry::TextEntry(const Common::String &name, UIElement *uiParent, const Position &pos, size_t maxLength) :
	UIElement(name, uiParent), _maxLength(maxLength) {
	Common::Rect r(pos.x * FONT_W, pos.y * FONT_H, (pos.x + maxLength + 1) * FONT_W, (pos.y + 1) * FONT_H);
	r.translate(pos._xOffset, pos._yOffset);
	setBounds(r);
}

void TextEntry::draw() {
	Surface s = getSurface();
	s.clear();
	s.writeString(_text);
	s.writeChar(_caretVisible ? '\x1b' : ' ');
}

bool TextEntry::msgKeypress(const KeypressMessage &msg) {
	bool changed = false;

	if (msg.ascii >= ' ' && msg.ascii <= 127 && _text.size() < _maxLength) {
		_text += msg.ascii;
		changed = true;

	} else if (msg.keycode == Common::KEYCODE_BACKSPACE && !_text.empty()) {
		_text.deleteLastChar();
		changed = true;

	} else if (msg.keycode == Common::KEYCODE_RETURN) {
		_parent->send(GameMessage("TEXT", _text));
		return true;
	}

	if (changed) {
		_parent->send(GameMessage("TEXT_CHANGED", _text));
		return true;
	}

	return false;
}

bool TextEntry::tick() {
	if (++_caretCtr >= 5) {
		_caretCtr = 0;
		_caretVisible = !_caretVisible;
		redraw();
	}

	return false;
}

void TextEntry::setText(const Common::String &text) {
	_text = text;
	redraw();
}

void TextEntry::setPosition(const Position &pos) {
	Common::Rect r(pos.x * FONT_W, pos.y * FONT_H, (pos.x + _maxLength + 1) * FONT_W, (pos.y + 1) * FONT_H);
	r.translate(pos._xOffset, pos._yOffset);
	setBounds(r);
}

} // namespace Gfx
} // namespace Shared
} // namespace Wasteland
