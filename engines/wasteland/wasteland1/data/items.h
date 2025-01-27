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

#ifndef WASTELAND_WASTELAND1_DATA_ITEMS_H
#define WASTELAND_WASTELAND1_DATA_ITEMS_H

#include "wasteland/core/serializer.h"
#include "wasteland/wasteland1/core/array1.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Data {

enum ItemFlag {
	ITEM_JAMMED = 0x80,
	ITEM_40 = 0x40,
	ITEM_FLAGS = ITEM_JAMMED || ITEM_40
};

enum ItemType {
	ITEMTYPE_ARMOR = 15
};

struct ItemDetails {
	byte _field0 = 0;
	byte _field1 = 0;
	byte _field2 = 0;
	byte _flags = 0;
	byte _clipSize = 0;			// Ammunition clip size
	byte _field5 = 0;
	byte _armorClass = 0;
	byte _ammunitionId = 0;		// Ammunition item takes

	void load(Common::SeekableReadStream *src);

	byte getItemType() const {
		return _flags >> 3;
	}
	bool isArmor() const {
		return getItemType() == ITEMTYPE_ARMOR;
	}
};

struct ItemDetailsArray : public Common::Array<ItemDetails> {
private:
	int _itemsSection = -1;

public:
	void load(int section = 0);
};


struct InventoryItem {
	byte _id = 0;
	byte _quantity = 0;

	InventoryItem() {
	}
	void synchronize(Serializer &s);

	Common::String getName() const;
	bool hasNoAmmunition() const {
		return _quantity == 0;
	}
	bool isJammed() const {
		return (_quantity & ITEM_JAMMED) != 0;
	}
	bool canUnjam() const;
	void setQuantity(uint qty);
	const ItemDetails *getItemDetails() const;
};

struct InventoryItems : public Array1<InventoryItem> {
	InventoryItems();
	void synchronize(Serializer &s);

	void remove_at(uint idx);
	bool empty() const;
	bool full() const;

	void add(byte id, byte qty);
};

} // namespace Data
} // namespace Wasteland1
} // namespace Wasteland

#endif
