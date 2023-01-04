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

#include "legend/early/gfx/command_buttons.h"
#include "legend/early/gfx/screen.h"

namespace Legend {
namespace Early {
namespace Gfx {

enum Commands {
	HELP = 0, HALF = 1, TEXT = 2, ERASE = 3, PICTURE = 4,
	STATUS = 5, INVENTORY = 6, LOOK = 7, DO = 8
};

CommandButtons::CommandButtons(UIElement *parent, const String &name,
		const Common::Rect &r) : ViewElement(parent, name, r),
		_btnPicture(this, "Picture", PICTURE, Common::Rect(51, 3, 115, 26)),
		_btnStatus(this, "Status", STATUS, Common::Rect(51, 30, 115, 53)),
		_btnInventory(this, "Inventory", INVENTORY, Common::Rect(51, 57, 115, 80)),
		_btnLook(this, "Look", LOOK, Common::Rect(51, 84, 115, 107)),
		_btnHelp(this, "Help", HELP, Common::Rect(4, 3, 47, 26)),
		_btnHalf(this, "Half", HALF, Common::Rect(4, 30, 47, 53)),
		_btnText(this, "Text", TEXT, Common::Rect(4, 57, 47, 80)),
		_btnErase(this, "Erase", ERASE, Common::Rect(4, 84, 47, 107)),
		_btnDo(this, "Do", DO, Common::Rect(4, 111, 115, 134)) {
}

void CommandButtons::draw() {
	Graphics::ManagedSurface s = getSurface();
	s.clear(WHITE);
}

bool CommandButtons::msgValue(const ValueMessage &msg) {
	switch (msg._value) {
	case PICTURE:
		replaceView("Commset");
		break;
	case HELP:
		replaceView("Help");
		break;
	default:
		break;
	}

	return true;
}

} // namespace Gfx
} // namespace Early
} // namespace Legend
