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

#include "wasteland/wasteland1/views/character/reorder_pane.h"
#include "wasteland/wasteland1/wasteland1.h"
#include "wasteland/wasteland1/data/text.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Views {
namespace Character {

ReorderPane::ReorderPane(const Common::String &name) : Gfx::PagedPane(name),
_esc(this, "Escape", "ESC", 18, 0, KEYBIND_ESCAPE),
_reorder(this, "Reorder", "REORDER", 23, 0, Common::KEYCODE_r),
_next(this, "Next", "NEXT", 32, 0, Common::KEYCODE_SPACE) {
}

void ReorderPane::draw() {
	PagedPane::draw();

	// TODO: Handle reordering mode
//	Surface s = getSurface();
}

bool ReorderPane::msgFocus(const FocusMessage &msg) {
	resetLines();
	return true;
}

bool ReorderPane::msgGame(const GameMessage &msg) {
	if (msg._name == "Reorder") {
		// TODO
		return true;
	}

	return PagedPane::msgGame(msg);
}

void ReorderPane::clearLines() {
	_lines.clear();
	clearText();
}

void ReorderPane::addLine(const Common::String &str) {
	_lines.push_back(str);
	addText(Common::String::format("%d> %s",
		((_lines.size() - 1) % PAGED_LINES) + 1,
		str.c_str()));
}

} // namespace Character
} // namespace Views
} // namespace Wasteland1
} // namespace Wasteland
