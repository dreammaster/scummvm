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

#ifndef ULTIMA5_DATA_SHOP_DATA_H
#define ULTIMA5_DATA_SHOP_DATA_H

#include "common/scummsys.h"

namespace Ultima {
namespace Ultima5 {
namespace Data {

constexpr int SHOP_KINDS = 8;
constexpr int SHOPS_PER_KIND = 16;

// Where the text of what each kind of shop says is in the SHOPPE.DAT file, four
// greetings, goodbyes and thanks for each
extern const uint16 SHOP_GREETINGS[SHOP_KINDS][4];
extern const uint16 SHOP_GOODBYES[SHOP_KINDS][4];
extern const uint16 SHOP_THANKS[SHOP_KINDS][4];
extern const uint16 GUILD_PRICES[4][4];
extern const uint16 GUILD_TEXT[3];
extern const byte PROVISION_PRICES[5][8];
extern const byte PROVISION_AMOUNTS[5][8];
extern const uint16 PROVISION_TEXT[8];
extern const char *const REAGENT_NAMES[8];
extern const uint16 STABLE_PRICES[4];
extern const byte HEAL_PRICES[8];
extern const byte CURE_PRICES[8];
extern const uint16 RESURRECT_PRICES[8];
extern const byte SHOP_MAPS[SHOP_KINDS][SHOPS_PER_KIND];
extern const char *const SHOP_NAMES[SHOP_KINDS][SHOPS_PER_KIND];
extern const char *const SHOP_KEEPERS[SHOP_KINDS][SHOPS_PER_KIND];

} // namespace Data
} // namespace Ultima5
} // namespace Ultima

#endif
