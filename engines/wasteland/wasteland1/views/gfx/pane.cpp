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

#include "wasteland/wasteland1/views/gfx/pane.h"
#include "wasteland/wasteland1/views/title/roster.h"
#include "wasteland/events.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Views {
namespace Gfx {

Pane::Pane(const Common::String &name) : Dialogs::Dialog(name) {
	setBounds(TextRect(14, 0, TEXT_W, 14));
	_bounds.setBorderSize(FONT_W);
}

void Pane::draw() {
	Dialogs::Dialog::draw();

	Surface s = getSurface();
	s.clear();
}

bool Pane::msgGame(const GameMessage &msg) {
	if (msg._name == "Escape") {
		close();
		return true;
	}

	return Pane::msgGame(msg);
}

} // namespace Gfx
} // namespace Views
} // namespace Wasteland1
} // namespace Wasteland
