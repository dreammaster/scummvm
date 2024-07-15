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

#include "wasteland/gfx/text_view.h"

namespace Wasteland {
namespace Shared {
namespace Gfx {

TextView::TextView(const Common::String &name, UIElement *uiParent) :
	UIElement(name, uiParent) {
}

TextView::TextView(const Common::String &name, UIElement *uiParent,
		const Common::Rect &r) :
		UIElement(name, uiParent) {
	setBounds(r);
}

void TextView::draw() {
	Surface s = getSurface();
	s.clear();
	s.setFont(0);

	for (uint y = 0; y < size(); ++y) {
		int lineNum = _text.size() - size() + y;

		if (lineNum >= 0)
			s.writeString(_text[lineNum], 0, y);
	}
}

void TextView::clear() {
	_text.clear();
	redraw();
}

void TextView::print(const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	Common::String output = Common::String::vformat(fmt, va);
	va_end(va);

	print(output);
}

void TextView::print(const Common::String &str) {
	if (str.empty())
		return;
	if (_text.empty())
		_text.push_back("");

	// Add in the text. Each time a newline is encountered,
	// add in a new row
	Common::String temp = str;
	uint32 p;
	while (!temp.empty()) {
		Common::String line;
		p = temp.findFirstOf('\r');

		if (p != Common::String::npos) {
			// Get the line
			line = Common::String(temp.c_str(), p);
			temp = Common::String(temp.c_str() + p + 1);
		} else {
			line = temp;
			temp.clear();
		}

		_text.back() += line;

		if (p != Common::String::npos) {
			// Move to next line
			if (_text.size() == size())
				_text.remove_at(0);
			_text.push_back("");
		}
	}

	redraw();
}

} // namespace Gfx
} // namespace Shared
} // namespace Wasteland
