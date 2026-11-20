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

#ifndef ULTIMA5_DATA_INVENTORY_DATA_H
#define ULTIMA5_DATA_INVENTORY_DATA_H

#include "common/scummsys.h"

namespace Ultima {
namespace Ultima5 {
namespace Data {

extern const char *const CLASS_NAMES[10];
extern const int16 CLASS_NAME_PADDING[9];
extern const char *const STATUS_NAMES[5];
extern const char *const REAGENT_LIST_NAMES[8];
extern const char *const SPELL_NAMES[49];
extern const char *const SPECIAL_ITEM_NAMES[38];
extern const byte EQUIPMENT_SLOTS[48];
extern const byte EQUIPMENT_WEIGHTS[48];
extern const byte EQUIPMENT_MARKERS[48];

} // namespace Data
} // namespace Ultima5
} // namespace Ultima

#endif
