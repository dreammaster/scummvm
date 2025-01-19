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

#ifndef WASTELAND_WASTELAND1_VIEWS_GFX_BUTTON_H
#define WASTELAND_WASTELAND1_VIEWS_GFX_BUTTON_H

#include "common/array.h"
#include "common/events.h"
#include "wasteland/events.h"
#include "wasteland/keymapping.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Views {
namespace Gfx {

class Button : public UIElement {
private:
	Common::Array<Common::KeyCode> _keycodes;
	KeybindingAction _action = KEYBIND_NONE;

protected:
	bool _focused = false;

public:
	Button(UIElement *parent, const Common::String &name,
		Common::KeyCode keycode = Common::KEYCODE_INVALID);
	Button(UIElement *parent, const Common::String &name,
		KeybindingAction action);
	~Button() override {
	}

	void draw() override = 0;
	bool msgMouseEnter(const MouseEnterMessage &msg) override;
	bool msgMouseLeave(const MouseLeaveMessage &msg) override;
	bool msgMouseDown(const MouseDownMessage &msg) override;
	bool msgKeypress(const KeypressMessage &msg) override;
	bool msgAction(const ActionMessage &msg) override;

	void addKeycode(Common::KeyCode keycode);
};

} // namespace Gfx
} // namespace Views
} // namespace Wasteland1
} // namespace Wasteland

#endif
