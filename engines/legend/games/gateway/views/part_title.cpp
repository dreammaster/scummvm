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

#include "legend/games/gateway/views/part_title.h"
#include "legend/early/gfx/screen.h"
#include "legend/legend.h"

namespace Legend {
namespace Early {
namespace Gateway {
namespace Views {

PartTitle::PartTitle() : Legend::Gfx::View("PartTitle"),
		_title(this, _bounds) {
}

bool PartTitle::msgFocus(const FocusMessage &msg) {
	// Set up for the display of the part number image
	_title.load(259 + _partNumber);

	// Set up an expirty time to close the view after 2 seconds
	_closeTime = g_system->getMillis() + 2000;

	return Legend::Gfx::View::msgFocus(msg);
}

bool PartTitle::tick() {
	if (g_system->getMillis() >= _closeTime) {
		// TODO: Close window
	}

	return true;
}

void PartTitle::draw() {
	// Fill the entire screen with black. The title sub-item
	// will then draw itself on top of that
	getSurface().clear(Gfx::BLACK);
}

} // namespace Views
} // namespace Gateway
} // namespace Early
} // namespace Legend
