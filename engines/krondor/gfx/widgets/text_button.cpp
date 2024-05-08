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

#include "krondor/gfx/widgets/text_button.h"

namespace Krondor {
namespace Gfx {
namespace Widgets {

TextButton::TextButton(const RequestData *reqData) : Button(reqData),
		_label(Common::Rect(
			reqData->_bounds.left + 2, reqData->_bounds.top + 2,
			reqData->_bounds.right - 4, reqData->_bounds.bottom - 4),
			reqData->_label.c_str()) {
	_children.push_back(&_label);
}

TextButton::~TextButton() {
	_children.clear();
}

void TextButton::draw() {
	GfxSurface s = getSurface();

	if (isPressed()) {
		s.clear(BUTTON_COLOR_PRESSED);
		s.vLine(0, 0, _bounds.height() - 1, LIGHT_COLOR);
		s.hLine(1, 0, _bounds.width() - 1, SHADOW_COLOR);
		s.vLine(_bounds.width() - 1, 1, _bounds.height() - 2, SHADOW_COLOR);
		s.hLine(1, _bounds.height() - 1, _bounds.width() - 1, LIGHT_COLOR);
	} else {
		s.clear(BUTTON_COLOR_NORMAL);
		s.vLine(0, 0, _bounds.height() - 1, SHADOW_COLOR);
		s.hLine(1, 0, _bounds.width() - 1, LIGHT_COLOR);
		s.vLine(_bounds.width() - 1, 1, _bounds.height() - 2, LIGHT_COLOR);
		s.hLine(1, _bounds.height() - 1, _bounds.width() - 1, SHADOW_COLOR);
	}

	// Update the state of the button's label. We don't need to manually draw it..
	// it'll be drawn automatically 
	if (isEnabled()) {
		if (isPressed()) {
			_label.setColor(TEXT_COLOR_PRESSED);
			_label.setShadow(SHADOW_COLOR, 0, 1);
		} else {
			_label.setColor(TEXT_COLOR_NORMAL);
			_label.setShadow(SHADOW_COLOR, 0, 1);
		}
	} else {
		_label.setColor(TEXT_COLOR_DISABLED);
		_label.setShadow(NO_SHADOW, 0, 0);
	}
}

} // namespace Widgets
} // namespace Gfx
} // namespace Krondor
