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

#include "common/system.h"
#include "graphics/palette.h"
#include "wasteland/fod/views/menu_view.h"
#include "wasteland/fod/fod.h"

namespace Wasteland {
namespace FOD {
namespace Views {

MenuView::MenuView(const Common::String &name) : UIElement(name),
	_f1("F1", nullptr, 1, 21, Common::Point(0, 4), "F1>", Common::KEYCODE_F1),
	_f2("F2", nullptr, 1, 22, Common::Point(0, 4), "F2>", Common::KEYCODE_F2),
	_f3("F3", nullptr, 1, 23, Common::Point(0, 4), "F3>", Common::KEYCODE_F3) {
}

void MenuView::draw() {
	UIElement::draw();

	Surface s = getSurface();
	s.clear();
	drawBorders();

	// Draw the welcome portrait
	Surface portrait = getSurface(Gfx::Window(1, 1, 12, 11));
	portrait.blitFrom(*g_engine->_pics._welcome.getSurface());

	// If the party display is enabled, show it
	if (_children.contains(&_f1)) {
		Surface partyArea = getSurface(Gfx::Window(1, 20, 38, 23, 0, 4));

		for (uint i = 0; i < g_engine->_disk1._partyCount; ++i) {
			const Data::PartyMember &member = g_engine->_disk1._party[i];
			partyArea.writeString(Common::String::format("%-16.16s - %-16.16s",
				member._name.c_str(),
				g_engine->_archetypes._professions[member._profession]._name.c_str()),
				3, i + 1);
		}

		// Set the currently active party member as highlighted
		_f1.setInverseColor(_selectedPartyMember == 0);
		_f2.setInverseColor(_selectedPartyMember == 1);
		_f3.setInverseColor(_selectedPartyMember == 2);
	}
}

void MenuView::drawBorders() {
	Surface s = getSurface();
	const byte *borders = g_engine->_pics._borders;

	for (int y = 0; y < TEXT_H; ++y) {
		s.setTextPos(0, y);

		for (int x = 0; x < TEXT_W; ++x) {
			s.writeChar(*borders++);
		}
	}
}

void MenuView::writePortraitText(const Common::String &str) {
	Surface portrait = getSurface(Gfx::Window(1, 1, 12, 12));
	portrait.writeCenteredString(str, 11);
}

void MenuView::showParty() {
	_f1.setParent((g_engine->_disk1._partyCount > 0) ? this : nullptr);
	_f2.setParent((g_engine->_disk1._partyCount > 1) ? this : nullptr);
	_f3.setParent((g_engine->_disk1._partyCount > 2) ? this : nullptr);

	redraw();
}

void MenuView::hideParty() {
	_f1.setParent(nullptr);
	_f2.setParent(nullptr);
	_f3.setParent(nullptr);
}

} // namespace Views
} // namespace FOD
} // namespace Wasteland
