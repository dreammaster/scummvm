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

#ifndef ULTIMA5_DATA_OBJECT_DATA_H
#define ULTIMA5_DATA_OBJECT_DATA_H

#include "common/scummsys.h"

namespace Ultima {
namespace Ultima5 {
namespace Data {

constexpr int HIDDEN_COUNT = 0x71;

extern const byte HIDDEN_MAP[113];
extern const byte HIDDEN_LEVEL[113];
extern const byte HIDDEN_X[113];
extern const byte HIDDEN_Y[113];
extern const byte HIDDEN_KIND[113];
extern const byte HIDDEN_AMOUNT[113];
extern const byte HERB_X[3];
extern const byte HERB_Y[3];
extern const byte HERB_REAGENT[3];
extern const byte CHEST_KIND[8];
extern const byte CHEST_ODDS[8];
extern const byte CHEST_SIZE[8];
extern const byte CHEST_ITEM_KIND[48];
extern const byte CHEST_ITEM_ODDS[48];

} // namespace Data
} // namespace Ultima5
} // namespace Ultima

#endif
