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

struct ItemDetails {
	byte _field0 = 0;
	byte _field1 = 0;
	byte _field2 = 0;
	byte _field3 = 0;
	byte _clipSize = 0;			// Ammunition clip size
	byte _field5 = 0;
	byte _field6 = 0;
	byte _ammunitionId = 0;		// Ammunition item takes

	void load(Common::SeekableReadStream *src);
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

	bool hasNoAmmunition() const {
		return _quantity == 0;
	}
	bool isJammed() const {
		return (int8)_quantity < 0;
	}

	const ItemDetails *getItemDetails() const;
};

struct InventoryItems : public Array1<InventoryItem> {
	InventoryItems();
	void synchronize(Serializer &s);

	void remove_at(uint idx) {
		Array1<InventoryItem>::remove_at(idx);
		push_back(InventoryItem());
	}

	bool empty() const {
		return Array1<InventoryItem>::empty() || front()._id == 0;
	}
};

} // namespace Data
} // namespace Wasteland1
} // namespace Wasteland

#endif
