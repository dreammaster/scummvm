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

#ifndef KRONDOR_GFX_MOUSE_CURSOR_H
#define KRONDOR_GFX_MOUSE_CURSOR_H

#include "common/array.h"
#include "graphics/managed_surface.h"
#include "krondor/res/image_resource.h"

namespace Krondor {
namespace Gfx {

enum PointerType {
	PT_SWORD = 0,
	PT_HAND = 1,
	PT_HOURGLASS = 2,
	PT_LOOKING_GLASS = 3,
	PT_EXIT = 4,
	PT_INN = 5,
	PT_TAVERN = 6,
	PT_SHOP = 7,
	PT_PALACE = 8,
	PT_TEMPLE = 9,
	PT_HOUSE = 10,
	PT_TELEPORT = 11,
	PT_PETITION = 12,
	PT_BARD = 13,
	PT_TALK = 14,
	PT_BUY_SELL = 15,
	PT_ENTER = 16,
	PT_REPAIR = 17,
	PT_BUY = 18,
	PT_HAND2 = 19,
	PT_GAB = 20,
	PT_CHAT = 21,
	PT_SEWER = 22,
	PT_BAR = 23,
	PT_BARMAID = 24,
	PT_BARKEEP = 25,
	PT_INNKEEPER = 26
};

class MouseCursors {
private:
	unsigned int _currentCursor = 0;
	ImageResource _cursors;

public:
	MouseCursors();

	void load();
	void setCursor(uint cursorNum);
};

} // namespace Gfx
} // namespace Krondor

#endif
