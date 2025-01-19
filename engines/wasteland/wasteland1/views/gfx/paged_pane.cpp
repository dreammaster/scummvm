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
		_downArrow(this, "Down", false, 39, 6, Common::KEYCODE_DOWN) {	
}

void PagedPane::draw() {
	Pane::draw();

	// Draw the items for the page
	Surface s = getSurface();

	uint index = _pageNum * PAGED_LINES;
	for (int y = 0; y < PAGED_LINES && index < _text.size(); ++y, ++index) {
		s.writeString(_text[index], 0, y + PAGED_Y_START);
	}

	// If there's more than one page, also draw "MORE" indicator
	if (_text.size() > PAGED_LINES) {
		s.setInverseColor(true);
		s.writeString("MORE\x5e", 0, PAGED_Y_START + PAGED_LINES);
	}
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

} // namespace Gfx
} // namespace Views
} // namespace Wasteland1
} // namespace Wasteland
