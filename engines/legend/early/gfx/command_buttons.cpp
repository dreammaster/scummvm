/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "legend/early/gfx/command_buttons.h"
#include "legend/game_manager.h"

namespace Legend {
namespace Early {

BEGIN_MESSAGE_MAP(CommandButtons, Gfx::VisualContainer)
	ON_MESSAGE(ButtonClicked)
END_MESSAGE_MAP()

enum Commands {
	HELP = 0, HALF = 1, TEXT = 2, ERASE = 3, PICTURE = 4,
	STATUS = 5, INVENTORY = 6, LOOK = 7, DO = 8
};

CommandButtons::CommandButtons(const String &name, const Common::Rect &r) :
		Gfx::VisualContainer(name, r),
		_btnPicture("Picture", PICTURE, Common::Rect(51, 3, 115, 26)),
		_btnStatus("Status", STATUS, Common::Rect(51, 30, 115, 53)),
		_btnInventory("Inventory", INVENTORY, Common::Rect(51, 57, 115, 80)),
		_btnLook("Look", LOOK, Common::Rect(51, 84, 115, 107)),
		_btnHelp("Help", HELP, Common::Rect(4, 3, 47, 26)),
		_btnHalf("Half", HALF, Common::Rect(4, 30, 47, 53)),
		_btnText("Text", TEXT, Common::Rect(4, 57, 47, 80)),
		_btnErase("Erase", ERASE, Common::Rect(4, 84, 47, 107)),
		_btnDo("Do", DO, Common::Rect(4, 111, 115, 134)) {
	_btnPicture.addUnder(this);
	_btnStatus.addUnder(this);
	_btnInventory.addUnder(this);
	_btnLook.addUnder(this);
	_btnHelp.addUnder(this);
	_btnHalf.addUnder(this);
	_btnText.addUnder(this);
	_btnErase.addUnder(this);
	_btnDo.addUnder(this);
}

void CommandButtons::draw() {
	if (!_isDirty)
		return;

	Gfx::VisualContainer::draw();
}

bool CommandButtons::ButtonClicked(CButtonClicked &msg) {
	switch (msg._value) {
	case PICTURE:
		changeView("Commset");
		break;
	case HELP:
		changeView("Help");
		break;
	default:
		break;
	}

	return true;
}

} // End of namespace Early
} // End of namespace Legend
