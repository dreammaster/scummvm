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

#ifndef WASTELAND_WASTELAND1_VIEWS_GFX_UP_DOWN_BUTTON_H
#define WASTELAND_WASTELAND1_VIEWS_GFX_UP_DOWN_BUTTON_H

#include "wasteland/wasteland1/views/gfx/button.h"
#include "wasteland/keymapping.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Views {
namespace Gfx {

class UpDownButton : public Button {
private:
	bool _isUp = false;

public:
	UpDownButton(UIElement *parent, const Common::String &name, bool isUp,
		Common::KeyCode keycode = Common::KEYCODE_INVALID);
	UpDownButton(UIElement *parent, const Common::String &name, bool isUp, int x, int y,
		Common::KeyCode keycode = Common::KEYCODE_INVALID);
	UpDownButton(UIElement *parent, const Common::String &name, bool isUp,
		KeybindingAction action);
	UpDownButton(UIElement *parent, const Common::String &name, bool isUp, int x, int y,
		KeybindingAction action);
	~UpDownButton() override {
	}

	void draw() override;
};

} // namespace Gfx
} // namespace Views
} // namespace Wasteland1
} // namespace Wasteland

#endif
