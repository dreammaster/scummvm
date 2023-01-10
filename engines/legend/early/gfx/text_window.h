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
	Common::Array<String> _pendingAdds;
	Common::Array<String> _lines;
	int _lineNum = 0;				// Line to add text to
	int _linesRemaining = 0;		// Lines remaining before "MORE"
	bool _hasMore = false;

protected:
	/**
	 * Returns true if window is showing the MORE
	 * indicator, and is waiting for a keypress
	 */
	bool hasMore() const {
		return _hasMore;
	}

	/**
	 * Adds lines to the text window, taking care of wrapping on
	 * word breaks if the line is too long
	 */
	void addLines(const String &msg);

	/**
	 * Adds text to the display
	 */
	void add(const String &msg);

	/**
	 * Moves to the next line
	 */
	void newLine();

	/**
	 * Flushes out any further adds done whilst waiting
	 * for - MORE - to be clicked
	 */
	void flush();

public:
	TextWindow(UIElement *parent, const Common::Rect &r) : BoxedElement(parent, r) {
		clear();
	}
	TextWindow(UIElement *parent, const String &name, const Common::Rect &r,
			bool cached = true) : BoxedElement(parent, name, r),
			_cached(cached) {
		clear();
	}
	~TextWindow() override;

	/**
	 * Clears the contents
	 */
	void clear();

	/**
	 * Called when bounds or font are changed
	 */
	void metricsChanged() override {
		clear();
	}

	/**
	 * Draws the visual item on the screen
	 */
	void draw() override;

	/**
	 * Handles text display messages
	 */
	bool msgText(const TextMessage &msg) override;

	/**
	 * Handle keypresses
	 */
	bool msgKeypress(const KeypressMessage &msg) override;

	/**
	 * Handle mouse clicks
	 */
	bool msgMouseDown(const MouseDownMessage &msg) override;
};

} // namespace Gfx
} // namespace Early
} // namespace Legend

#endif
