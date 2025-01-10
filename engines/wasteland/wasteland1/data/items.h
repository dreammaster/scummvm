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

struct InventoryItem {
	int8 _id = 0;
	int8 _quantity = 0;

	InventoryItem() {}
	void synchronize(Serializer &s);

	bool hasNoAmmunition() const {
		return _quantity == 0;
	}
};

struct InventoryItems : public Array1<InventoryItem> {
	InventoryItems();
	void synchronize(Serializer &s);
};

} // namespace Data
} // namespace Wasteland1
} // namespace Wasteland

#endif
