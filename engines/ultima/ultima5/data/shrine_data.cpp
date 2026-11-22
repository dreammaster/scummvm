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

#include "ultima/ultima5/data/shrine_data.h"

namespace Ultima {
namespace Ultima5 {
namespace Data {

const char *const SHRINE_VIRTUES[SHRINE_COUNT] = {
	"Honesty",
	"Compassion",
	"Valour",
	"Justice",
	"Sacrifice",
	"Honor",
	"Spirituality",
	"Humility"
};

const char *const SHRINE_MANTRAS[SHRINE_COUNT] = {
	"Ahm",
	"Mu",
	"Ra",
	"Beh",
	"Cah",
	"Summ",
	"Om",
	"Lum"
};

const char *const SHRINE_VIRTUE_WORDS[SHRINE_COUNT] = {
	"hone",
	"comp",
	"valo",
	"just",
	"sacr",
	"hono",
	"spir",
	"humi"
};

const byte SHRINE_X[SHRINE_COUNT] = {
	0xE9, 0x80, 0x24, 0x49, 0xCD, 0x51, 0x00, 0xE7
};

const byte SHRINE_Y[SHRINE_COUNT] = {
	0x42, 0x5C, 0xE5, 0x0B, 0x2D, 0xCF, 0x00, 0xD8
};

const uint16 SHRINE_QUEST_TEXT[SHRINE_COUNT] = {
	0x0000, 0x001B, 0x0039, 0x005F, 0x007B, 0x009F, 0x00B9, 0x00D6
};

const uint16 SHRINE_CODEX_TEXT[SHRINE_COUNT] = {
	0x0100, 0x014E, 0x0189, 0x01D4, 0x022B, 0x027F, 0x02C0, 0x0310
};

const byte SHRINE_STRENGTH[SHRINE_COUNT] = {
	0x00, 0x00, 0x01, 0x00, 0x01, 0x01, 0x01, 0x00
};

const byte SHRINE_DEXTERITY[SHRINE_COUNT] = {
	0x00, 0x01, 0x00, 0x01, 0x01, 0x00, 0x01, 0x00
};

const byte SHRINE_INTELLIGENCE[SHRINE_COUNT] = {
	0x01, 0x00, 0x00, 0x01, 0x00, 0x01, 0x01, 0x00
};

const char *const SHRINE_WORDS[SHRINE_COUNT] = {
	"FALLAX",
	"VILIS",
	"INOPIA",
	"MALUM",
	"AVIDUS",
	"INFAMA",
	"IGNAVUS",
	"VERAMOCOR"
};

const byte SHRINE_GATE_TILES[SHRINE_COUNT] = {
	0x18, 0x16, 0x16, 0x18, 0x18, 0x17, 0x17, 0x16
};

const byte MOON_PHASES[28 * 2] = {
	0, 0, 1, 0, 1, 1, 2, 2, 2, 3, 3, 4, 3, 5,
	4, 6, 5, 7, 5, 0, 6, 0, 6, 1, 7, 2, 7, 3,
	0, 4, 1, 5, 1, 6, 2, 7, 2, 0, 3, 0, 3, 1,
	4, 2, 5, 3, 5, 4, 6, 5, 6, 6, 7, 7, 7, 0
};

} // namespace Data
} // namespace Ultima5
} // namespace Ultima
