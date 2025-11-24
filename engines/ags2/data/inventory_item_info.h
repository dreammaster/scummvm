
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

#ifndef AGS2_DATA_INVENTORY_ITEM_INFO_H
#define AGS2_DATA_INVENTORY_ITEM_INFO_H

#include "common/serializer.h"
#include "ags2/data/inventory_item_info.h"

namespace AGS2 {

#define IFLG_STARTWITH 1

struct InventoryItemInfo {
	char name[25];
	int  pic;
	int  cursorPic, hotx, hoty;
	int  reserved[5];
	byte flags;

	void synchronize(Common::Serializer &s);
	void load(Common::SeekableReadStream *rs) {
		Common::Serializer s(rs, nullptr);
		synchronize(s);
	}
	void save(Common::WriteStream *ws) {
		Common::Serializer s(nullptr, ws);
		synchronize(s);
	}
};

} // namespace AGS2

#endif
