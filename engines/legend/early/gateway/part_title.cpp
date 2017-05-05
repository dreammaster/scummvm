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

#include "legend/early/gateway/part_title.h"
#include "legend/early/screen.h"
#include "legend/legend.h"

namespace Legend {
namespace Early {
namespace Gateway {

BEGIN_MESSAGE_MAP(PartTitle, Gfx::VisualContainer)
	ON_MESSAGE(ShowMsg)
	ON_MESSAGE(FrameMsg)
END_MESSAGE_MAP()

PartTitle::PartTitle() : Gfx::VisualContainer() {
	_closeTime = 0;
	_name = "PartTitle";
	_title.addUnder(this);
	setBounds(Screen::get().getBounds());
}

bool PartTitle::ShowMsg(CShowMsg &msg) {
	// Set up for the display of the part number image
	_title.load(260 + getGameState()._partNumber);

	// Set up an expirty time to close the view after 2 seconds
	_closeTime = getEvents()->getTicksCount() + 2000;

	return Gfx::VisualContainer::ShowMsg(msg);
}

bool PartTitle::FrameMsg(CFrameMsg &msg) {
	if (getEvents()->getTicksCount() >= _closeTime) {
		// TODO: Close window
	}

	return true;
}

void PartTitle::draw() {
	if (!_isDirty)
		return;
	Gfx::VisualContainer::draw();

	// Fill the entire screen with black. The title sub-item
	// will then draw itself on top of that
	getSurface().fillRect(getBounds(), 0);
}

} // End of namespace Gateway
} // End of namespace Early
} // End of namespace Legend
