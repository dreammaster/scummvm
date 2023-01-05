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

#ifndef LEGEND_EARLY_GFX_TEXT_WINDOW_H
#define LEGEND_EARLY_GFX_TEXT_WINDOW_H

#include "legend/early/gfx/boxed_element.h"
#include "common/stream.h"

namespace Legend {
namespace Early {
namespace Gfx {

class TextWindow : public BoxedElement {
private:
	Common::WriteStream *_logFile = nullptr;
	bool _cached = false;
	String _cachedText;

protected:
	/**
	 * Adds lines to the text window, taking care of wrapping on
	 * word breaks if the line is too long
	 */
	void addLines(const String &msg);

	/**
	 * Adds text to the display
	 */
	void add(const String &msg);

public:
	TextWindow(UIElement *parent, const Common::Rect &r) : BoxedElement(parent, r) {}
	TextWindow(UIElement *parent, const String &name, const Common::Rect &r,
		bool cached = true) : BoxedElement(parent, name, r),
		_cached(cached) {}
	~TextWindow() override;

	/**
	 * Draws the visual item on the screen
	 */
	void draw() override;

	/**
	 * Handles text display messages
	 */
	bool msgText(const TextMessage &msg) override;
};

} // namespace Gfx
} // namespace Early
} // namespace Legend

#endif
