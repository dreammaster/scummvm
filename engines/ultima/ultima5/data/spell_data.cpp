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

#include "ultima/ultima5/data/spell_data.h"

namespace Ultima {
namespace Ultima5 {
namespace Data {

const char *const SPELL_SYLLABLES[26] = {
	"AN",
	"BET",
	"CORP",
	"DES",
	"EX",
	"FLAM",
	"GRAV",
	"HUR",
	"IN",
	"",
	"KAL",
	"LOR",
	"MANI",
	"NOX",
	"",
	"POR",
	"QUAS",
	"REL",
	"SANCT",
	"TYM",
	"UUS",
	"VAS",
	"WIS",
	"XEN",
	"YLEM",
	"ZU"
};

const char *const SPELL_LETTERS[SPELL_COUNT] = {
	"IL",
	"GP",
	"AZ",
	"AN",
	"M",
	"AY",
	"AS",
	"ACX",
	"HR",
	"IW",
	"KX",
	"IMX",
	"LV",
	"FV",
	"FGI",
	"GIN",
	"GIZ",
	"IP",
	"AG",
	"IS",
	"GIS",
	"PU",
	"DP",
	"QW",
	"BIX",
	"AEP",
	"EIP",
	"MV",
	"IZ",
	"RT",
	"IPVY",
	"AQW",
	"AI",
	"AWY",
	"AEX",
	"BRX",
	"LS",
	"CX",
	"IQX",
	"IQW",
	"HIN",
	"CIQ",
	"CIM",
	"CKX",
	"CGIV",
	"FHI",
	"PRV",
	"AT"
};

const byte SPELL_PLACES[SPELL_COUNT] = {
	0x0E, 0x01, 0x0F, 0x0F, 0x0F, 0x05, 0x0F, 0x01, 0x08, 0x08, 0x01, 0x0F,
	0x0E, 0x01, 0x03, 0x03, 0x03, 0x09, 0x03, 0x0F, 0x03, 0x02, 0x02, 0x01,
	0x01, 0x05, 0x05, 0x0F, 0x01, 0x0F, 0x01, 0x01, 0x0F, 0x0C, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x0E, 0x01, 0x01, 0x0E, 0x01, 0x01, 0x01, 0x0E, 0x0F
};

const byte SPELL_RECIPES[SPELL_COUNT] = {
	0x80, 0x84, 0x60, 0x60, 0x50, 0x28, 0x88, 0xA0, 0x88, 0x02, 0x11, 0x61,
	0x81, 0x84, 0x94, 0x16, 0x54, 0x18, 0x84, 0xE0, 0x15, 0x18, 0x18, 0x12,
	0x98, 0xA8, 0x88, 0x51, 0x52, 0x89, 0x89, 0x03, 0xA1, 0x81, 0x16, 0x93,
	0x0B, 0x06, 0xD9, 0x03, 0x8A, 0x23, 0xF9, 0x39, 0x83, 0x89, 0x85, 0x29
};

} // namespace Data
} // namespace Ultima5
} // namespace Ultima
