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

#include "wasteland/wasteland1/views/gfx/up_down_button.h"
#include "wasteland/gfx/text_rect.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Views {
namespace Gfx {

UpDownButton::UpDownButton(UIElement *parent, const Common::String &name, bool isUp,
	Common::KeyCode keycode) : Button(parent, name, keycode), _isUp(isUp) {
}

UpDownButton::UpDownButton(UIElement *parent, const Common::String &name, bool isUp,
	int x, int y, Common::KeyCode keycode) : Button(parent, name, keycode),
		_isUp(isUp) {
	setBounds(TextRect(x, y, x, y + 1));
}

UpDownButton::UpDownButton(UIElement *parent, const Common::String &name, bool isUp,
	KeybindingAction action) : Button(parent, name, action),
	_isUp(isUp) {
}

UpDownButton::UpDownButton(UIElement *parent, const Common::String &name, bool isUp,
		int x, int y, KeybindingAction action) : Button(parent, name, action),
		_isUp(isUp) {
	setBounds(Window(x, y, x + 1, y));
	_bounds.setBorderSize(FONT_W, 0, FONT_W, 0);
}

void UpDownButton::draw() {
	Surface s = getSurface(_bounds);

	s.setFont(1);
	unsigned char firstChar = _focused ? 30 : 58;

	if (_isUp) {
		s.writeChar(firstChar + 0, 0, 0);
		s.writeChar(firstChar + 1, 0, 1);
	} else {
		s.writeChar(firstChar + 3, 0, 0);
		s.writeChar(firstChar + 4, 0, 1);
	}
}

} // namespace Gfx
} // namespace Views
} // namespace Wasteland1
} // namespace Wasteland
