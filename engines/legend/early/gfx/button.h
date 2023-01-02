/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef LEGEND_EARLY_GFX_BUTTON_H
#define LEGEND_EARLY_GFX_BUTTON_H

#include "legend/early/gfx/view.h"

namespace Legend {
namespace Early {
namespace Gfx {

class Button : public Legend::Early::Gfx::View {
private:
	Common::String _text;
public:
	Button(const Common::String &text, const Common::Rect &r) :
			Early::Gfx::View(r), _text(text) {
		setFont(1);
	}
	virtual ~Button() {}

	/**
	 * Draws the button on the screen
	 */
	void draw() override;

	/**
	 * Handle mouse clicks
	 */
	bool msgMouseDown(const MouseDownMessage &msg) override;

	/**
	 * Execute the button action
	 */
	virtual void execute() = 0;
};

/**
 * Derived button class that stores an Id, and generates a message
 * to it's immediate owner when clicked
 */
class ValueButton : public Button {
private:
	int _value;
public:
	ValueButton(const Common::String &text, int value, const Common::Rect &r) : Button(text, r), _value(value) {}
	virtual ~ValueButton() {}

	/**
	 * Execute the button action
	 */
	virtual void execute();
};

} // namespace Gfx
} // namespace Early
} // namespace Legend

#endif
