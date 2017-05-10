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

CommandButtons::CommandButtons(const Common::String &name, const Common::Rect &r) :
		Gfx::VisualContainer(name, r),
		_btnPicture(Common::Rect(33, 3, 115, 25)),
		_btnStatus(Common::Rect(33, 30, 115, 52)),
		_btnInventory(Common::Rect(33, 57, 115, 79)),
		_btnLook(Common::Rect(33, 84, 115, 106)),
		_btnHelp(Common::Rect(4, 3, 47, 3)),
		_btnHalf(Common::Rect(4, 30, 47, 30)),
		_btnText(Common::Rect(4, 57, 47, 79)),
		_btnErase(Common::Rect(4, 84, 47, 106)),
		_btnDo(Common::Rect(4, 111, 115, 133)) {
}

void CommandButtons::draw() {
	if (!_isDirty)
		return;

	// Draw the background
	Graphics::ManagedSurface s = getSurface();
	Common::Rect r = s.getBounds();
	s.fillRect(r, 15);
	r.grow(-1);
	s.fillRect(r, 0);

	Gfx::VisualContainer::draw();
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
