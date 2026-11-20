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

#include "ultima/ultima5/data/inventory_data.h"

namespace Ultima {
namespace Ultima5 {
namespace Data {

const char *const CLASS_NAMES[10] = {
	"Avatar",
	"Mage",
	"Bard",
	"Fighter",
	"Druid",
	"Tinker",
	"Paladin",
	"Ranger",
	"Shepherd",
	"Adept of Woznir"
};

const int16 CLASS_NAME_PADDING[9] = {
	1, 2, 2, 1, 1, 1, 1, 1,
	0
};

const char *const STATUS_NAMES[5] = {
	"Good Health",
	"Poisoned",
	"Dead",
	"Asleep",
	"Charmed"
};

const char *const REAGENT_LIST_NAMES[8] = {
	"Sulfur Ash",
	"Ginseng",
	"Garlic",
	"Sp. Silk",
	"Blood Moss",
	"Blk. Pearl",
	"Nightshade",
	"Mandrake"
};

const char *const SPELL_NAMES[49] = {
	"In Lor",
	"Grav Por",
	"An Zu",
	"An Nox",
	"Mani",
	"An Ylem",
	"An Sanct",
	"An Xen Cor",
	"Rel Hur",
	"In Wis",
	"Kal Xen",
	"In Xen Man",
	"Vas Lor",
	"Vas Flam",
	"In Flam Gr",
	"In Nox Gr",
	"In Zu Grav",
	"In Por",
	"An Grav",
	"In Sanct",
	"In Sanct G",
	"Uus Por",
	"Des Por",
	"Wis Quas",
	"In Bet Xen",
	"An Ex Por",
	"In Ex Por",
	"Vas Mani",
	"In Zu",
	"Rel Tym",
	"In Vas P Y",
	"Quas An Wi",
	"In An",
	"Wis An Yle",
	"An Xen Ex",
	"Rel Xen Be",
	"Sanct Lo",
	"Xen Corp",
	"In Quas Xe",
	"In Quas Wi",
	"In Nox Hur",
	"In Quas Co",
	"In Mani Co",
	"Kal Xen Co",
	"In Vas G C",
	"In Flam Hu",
	"Vas Rel Po",
	"An Tym",
	"Frotz"
};

const char *const SPECIAL_ITEM_NAMES[38] = {
	"*VL",
	"*RH",
	"*IS",
	"*IA",
	"*IQW",
	"*KXC",
	"*IMC",
	"*AT",
	"!",
	"!",
	"!",
	"!",
	"!",
	"!",
	"!",
	"!",
	"Magic Crpt",
	"Skull Keys",
	"Amulet",
	"Crown",
	"Sceptre",
	"(0",
	"(1",
	"(2",
	"(3",
	"(4",
	"(5",
	"(6",
	"(7",
	"Shard/Falsehd",
	"Shard/Hatred",
	"Shard/Cowrdce",
	"Spyglass",
	"HMS Cape Plan",
	"Sextant",
	"Pocket Watch",
	"Black Badge",
	"Wooden Box"
};

const byte EQUIPMENT_SLOTS[48] = {
	0x80, 0x80, 0x80, 0x80, 0x20, 0x20, 0x20, 0x20,
	0x20, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
	0x20, 0x30, 0x20, 0x30, 0x20, 0x20, 0x20, 0x20,
	0x20, 0x20, 0x30, 0x00, 0x30, 0x00, 0x20, 0x30,
	0x30, 0x30, 0x30, 0x30, 0x30, 0x20, 0x20, 0x20,
	0x20, 0x30, 0x02, 0x02, 0x02, 0x04, 0x04, 0x04
};

const byte EQUIPMENT_WEIGHTS[48] = {
	0x00, 0x01, 0x02, 0x03, 0x02, 0x03, 0x04, 0x00,
	0x00, 0x00, 0x02, 0x04, 0x06, 0x0a, 0x0c, 0x00,
	0x01, 0x02, 0x03, 0x02, 0x03, 0x04, 0x06, 0x05,
	0x07, 0x08, 0x08, 0x00, 0x06, 0x00, 0x09, 0x10,
	0x0f, 0x0d, 0x12, 0x00, 0x00, 0x08, 0x00, 0x05,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const byte EQUIPMENT_MARKERS[48] = {
	0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02,
	0x02, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
	0x04, 0x05, 0x06, 0x07, 0x04, 0x08, 0x09, 0x0b,
	0x0c, 0x1e, 0x0f, 0x17, 0x10, 0x17, 0x0b, 0x11,
	0x12, 0x13, 0x14, 0x15, 0x16, 0x15, 0x17, 0x15,
	0x15, 0x15, 0x18, 0x18, 0x18, 0x19, 0x1a, 0x1b
};

} // namespace Data
} // namespace Ultima5
} // namespace Ultima
