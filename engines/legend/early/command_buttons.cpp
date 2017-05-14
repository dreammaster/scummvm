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

#include "legend/early/command_buttons.h"
#include "legend/game_manager.h"

namespace Legend {
namespace Early {

EMPTY_MESSAGE_MAP(CommandButtons, Gfx::VisualContainer);

CommandButtons::CommandButtons(const String &name, const Common::Rect &r) :
		BoxedElement(name, r),
		_btnPicture(Common::Rect(51, 3, 115, 26)),
		_btnStatus(Common::Rect(51, 30, 115, 53)),
		_btnInventory(Common::Rect(51, 57, 115, 80)),
		_btnLook(Common::Rect(51, 84, 115, 107)),
		_btnHelp(Common::Rect(4, 3, 47, 26)),
		_btnHalf(Common::Rect(4, 30, 47, 53)),
		_btnText(Common::Rect(4, 57, 47, 80)),
		_btnErase(Common::Rect(4, 84, 47, 107)),
		_btnDo(Common::Rect(4, 111, 115, 134)) {
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

	BoxedElement::draw();
}

void CommandButtons::PictureButton::execute() {
}

void CommandButtons::StatusButton::execute() {
}

void CommandButtons::InventoryButton::execute() {
}

void CommandButtons::LookButton::execute() {
}

void CommandButtons::HelpButton::execute() {
	getGameManager()->changeView("Help");
}

void CommandButtons::HalfButton::execute() {
}

void CommandButtons::TextButton::execute() {
}

void CommandButtons::EraseButton::execute() {
}

void CommandButtons::DoButton::execute() {
}

} // End of namespace Early
} // End of namespace Legend
