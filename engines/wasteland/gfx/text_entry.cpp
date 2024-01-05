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
#include "wasteland/gfx/wasteland_font.h"

namespace Wasteland {
namespace Gfx {

TextEntry::TextEntry(const Common::String &name, UIElement *uiParent, int x, int y,
					 size_t maxLength) : UIElement(name, uiParent), _maxLength(maxLength) {
	setBounds(Common::Rect(x * FONT_W, y * FONT_H, (x + maxLength + 1) * FONT_W, (y + 1) * FONT_H));
}

void TextEntry::draw() {
	Surface s = getSurface();
	s.writeString(_text);
	if (_caretVisible)
		s.writeChar('_');
}

bool TextEntry::msgKeypress(const KeypressMessage &msg) {
	if (msg.ascii >= ' ' && msg.ascii <= 127 && _text.size() < _maxLength) {
		_text += msg.ascii;
		redraw();
		return true;

	} else if (msg.keycode == Common::KEYCODE_BACKSPACE && !_text.empty()) {
		_text.deleteLastChar();
		redraw();
		return true;

	} else if (msg.keycode == Common::KEYCODE_RETURN) {
		_parent->send(GameMessage("TEXT", _text));
		return true;
	}

	return false;
}

bool TextEntry::tick() {
	if (++_caretCtr >= 10) {
		_caretCtr = 0;
		_caretVisible = !_caretVisible;
		redraw();
	}

	return false;
}

} // namespace Gfx
} // namespace Wasteland
