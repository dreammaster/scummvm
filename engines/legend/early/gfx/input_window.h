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

#ifndef LEGEND_EARLY_GFX_INPUT_WINDOW_H
#define LEGEND_EARLY_GFX_INPUT_WINDOW_H

#include "legend/early/gfx/text_window.h"

namespace Legend {
namespace Early {
namespace Gfx {

class InputWindow : public TextWindow {
	enum LineMode { LMODE_NONE, LMODE_PENDING, LMODE_DISPLAYED };
private:
	String _line;
	LineMode _mode = LMODE_NONE;

	/**
	 * Turn on input line for input
	 */
	void addInputLine();

	/**
	 * A line has been entered
	 */
	void lineDone();

protected:
	void setHasMore(bool state) override;

public:
	InputWindow(UIElement *parent, const Common::Rect &r) : TextWindow(parent, r) {}
	InputWindow(UIElement *parent, const String &name, const Common::Rect &r) :
		TextWindow(parent, name, r) {}
	~InputWindow() override {}

	/**
	 * Handles game messages
	 */
	bool msgGame(const GameMessage &msg) override;

	/**
	 * Keypress handler
	 */
	bool msgKeypress(const KeypressMessage &msg) override;

	/**
	 * Mouse press
	 */
	bool msgMouseDown(const MouseDownMessage &msg) override;

	/**
	 * Draw the window
	 */
	void draw() override;
};

} // namespace Gfx
} // namespace Early
} // namespace Legend

#endif
