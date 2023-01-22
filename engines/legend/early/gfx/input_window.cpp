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

#include "legend/early/gfx/input_window.h"
#include "legend/early/gfx/screen.h"
#include "legend/early/engine.h"

namespace Legend {
namespace Early {
namespace Gfx {

void InputWindow::addInputLine() {
	_mode = LMODE_PENDING;

	// If the text window is paused for more text to display,
	// then we'll wait until that is done
	if (hasMore())
		return;

	// Add a newline
	addDirect("\n");

	// Focus on the view
	_mode = LMODE_DISPLAYED;
	g_engine->_mouseCursor->hide();
}

void InputWindow::setHasMore(bool state) {
	TextWindow::setHasMore(state);

	if (!state && _mode != LMODE_NONE) {
		// We have a pending flag to add the input line,
		// so it can now be done
		addInputLine();
	}
}

void InputWindow::draw() {
	TextWindow::draw();

	// Draw the line
	Common::Point pt = getLinePos();
	writeString(pt, ">");
	writeString(_line);

	// Show the text cursor only if the mouse cursor is hidden
	if (!g_engine->_mouseCursor->isVisible()) {
		Graphics::ManagedSurface s = getSurface();
		s.fillRect(Common::Rect(_textPos.x, _textPos.y,
			_textPos.x + 8, _textPos.y + _font->_lineHeight), Gfx::BLACK);
	}
}

bool InputWindow::msgGame(const GameMessage &msg) {
	if (msg._name == "GET_LINE") {
		addInputLine();
		return true;
	}

	return false;
}

bool InputWindow::msgKeypress(const KeypressMessage &msg) {
	if (_mode != LMODE_DISPLAYED)
		return TextWindow::msgKeypress(msg);

	// Hide the mouse cursor if it's visible
	g_engine->_mouseCursor->hide();

	if (msg.keycode == Common::KEYCODE_BACKSPACE) {
		if (!_line.empty())
			_line.deleteLastChar();
	} else if (msg.keycode == Common::KEYCODE_RETURN) {
		lineDone();
	} else if (msg.keycode == Common::KEYCODE_ESCAPE) {
		// Clear the entered line
		_line.clear();
	} else if (Common::isPrint(msg.ascii)) {
		String newLine = _line + msg.ascii;
		size_t maxLen = _innerBounds.width() - _font->_maxCharWidth;

		if (_font->stringWidth(newLine) < maxLen)
			_line += msg.ascii;
	}

	redraw();
	return true;
}

bool InputWindow::msgMouseDown(const MouseDownMessage &msg) {
	if (_mode == LMODE_DISPLAYED) {
		// Clicking the mouse hides the text cursor and shows
		// the mouse cursor
		g_engine->_mouseCursor->show();
		redraw();
		return false;
	} else {
		return TextWindow::msgMouseDown(msg);
	}
}

void InputWindow::lineDone() {
	// Add the line permanently to the window, and any log file
	_mode = LMODE_NONE;
	addDirect(String::format(">%s", _line.c_str()));

	// Pass the inputted line out to the interface
	send(GameMessage("LINE", _line));
}

} // namespace Gfx
} // namespace Early
} // namespace Legend
