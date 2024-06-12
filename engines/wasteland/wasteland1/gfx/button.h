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

#ifndef WASTELAND_WASTELAND1_GFX_BUTTON_H
#define WASTELAND_WASTELAND1_GFX_BUTTON_H

#include "wasteland/events.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Gfx {

class Button : public UIElement {
private:
	Common::String _text;
	bool _focused = false;

	/**
	 * Returns any button immediately to the left/right of the current button.
	 * Used for drawing the correct left edge between successive buttons
	 */
	Button *leftSideButton() const;
	Button *rightSideButton() const;

public:
	Button(UIElement *parent, const Common::String &name, const Common::String &text);
	Button(UIElement *parent, const Common::String &name, const Common::String &text, int x, int y);
	~Button() override {}

	bool msgFocus(const FocusMessage &msg) override;
	bool msgUnfocus(const UnfocusMessage &msg) override;
	void draw() override;
	bool msgMouseDown(const MouseDownMessage &msg) override;
};

} // namespace Gfx
} // namespace Wasteland1
} // namespace Wasteland

#endif
