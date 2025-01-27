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

#include "wasteland/wasteland1/views/gfx/paged_pane.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Views {
namespace Gfx {

PagedPane::PagedPane(const Common::String &name) : Gfx::Pane(name),
		_upArrow(this, "Up", true, 39, 3, Common::KEYCODE_UP),
		_downArrow(this, "Down", false, 39, 10, Common::KEYCODE_DOWN) {	
	// Wasteland also allowed for I/K to be used instead of up/down arrows
	_upArrow.addKeycode(Common::KEYCODE_i);
	_downArrow.addKeycode(Common::KEYCODE_k);

	for (int i = 0; i < PAGED_LINES; ++i) {
		_clickableText[i] = new Gfx::ClickableText(this,
			Common::String::format("Line%d", i + 1));
		_clickableText[i]->setBounds(TextRect(15, 1 + i + PAGED_Y_START,
			39, 1 + i + PAGED_Y_START));
	}
}

PagedPane::~PagedPane() {
	for (int i = 0; i < PAGED_LINES; ++i)
		delete _clickableText[i];
}

void PagedPane::draw() {
	Pane::draw();

	// Draw the items for the page
	Surface s = getSurface();

	uint index = _pageNum * PAGED_LINES;
	for (int y = 0; y < PAGED_LINES; ++y, ++index) {
		if (index < _text.size()) {
			_clickableText[y]->setEnabled(_clickable);
			_clickableText[y]->setText(_text[index]);
		} else {
			_clickableText[y]->setEnabled(false);
			_clickableText[y]->setText("");
		}
	}

	// If there's more than one page, also draw "MORE" indicator
	if (_text.size() > PAGED_LINES) {
		s.writeString("\x01MORE\x5e\x01", 0, PAGED_Y_START + PAGED_LINES);
	}
}

bool PagedPane::msgKeypress(const KeypressMessage &msg) {
	if (msg.keycode >= Common::KEYCODE_1 &&
			msg.keycode <= Common::KEYCODE_9) {
		send(GameMessage("Line", msg.keycode - Common::KEYCODE_0 +
			(_pageNum * PAGED_LINES)));
		return true;
	}

	return Pane::msgKeypress(msg);
}

bool PagedPane::msgGame(const GameMessage &msg) {
	if (msg._name == "Up") {
		if (_pageNum > 0)
			--_pageNum;

		redraw();
		return true;

	} else if (msg._name == "Down") {
		int maxPage = (_text.size() - 1) / PAGED_LINES;
		if (_pageNum < maxPage)
			++_pageNum;

		redraw();
		return true;

	} else if (msg._name.hasPrefix("Line") && Common::isDigit(msg._name.lastChar())) {
		// Get out the line number and re-generate the message
		send(GameMessage("Line", msg._name.lastChar() - '0' +
			(_pageNum * PAGED_LINES)));
		return true;

	} else {
		return Pane::msgGame(msg);
	}
}

void PagedPane::clearText() {
	_pageNum = 0;
	_text.clear();
	redraw();
}

void PagedPane::addText(const Common::String &str) {
	_text.push_back(str);
}

void PagedPane::setClickable(bool clickable) {
	_clickable = clickable;
	redraw();
}

} // namespace Gfx
} // namespace Views
} // namespace Wasteland1
} // namespace Wasteland
