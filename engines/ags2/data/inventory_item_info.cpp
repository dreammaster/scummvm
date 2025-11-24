
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

#include "ags2/data/inventory_item_info.h"

namespace AGS2 {

void InventoryItemInfo::synchronize(Common::Serializer &s) {
	s.syncBytes((byte *)name, 25);
	s.syncAsSint32LE(pic);
	s.syncAsSint32LE(cursorPic);
	s.syncAsSint32LE(hotx);
	s.syncAsSint32LE(hoty);
	s.syncBytes((byte *)reserved, 5);
	s.syncAsByte(flags);
}

} // namespace AGS2
