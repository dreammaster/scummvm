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

#include "ultima/ultima5/data/location_tables.h"

namespace Ultima {
namespace Ultima5 {
namespace Data {

// Which block of its file each of the first 32 locations starts at
const byte LOCATION_FIRST_BLOCK[LOCATION_FILE_LOCATIONS * 4] = {
	0x00, 0x02, 0x04, 0x07, 0x08, 0x0A, 0x0C, 0x0E,
	0x00, 0x03, 0x06, 0x09, 0x0C, 0x0D, 0x0E, 0x0F,
	0x01, 0x06, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x00, 0x02, 0x04, 0x05, 0x06, 0x07, 0x0A, 0x0E
};

// Where each location is on the map of Britannia, which is also where
// the party comes out when it leaves
const byte LOCATION_ENTRANCE_X[LOCATION_COUNT] = {
	0xE8, 0x51, 0x24, 0x3A, 0x9F, 0x6A, 0x16, 0xBB,
	0x58, 0x98, 0x68, 0xD8, 0x2D, 0xB0, 0xC9, 0x99,
	0x56, 0xC4, 0x54, 0x56, 0x58, 0x62, 0x88, 0x88,
	0x31, 0x0F, 0x40, 0xF8, 0x94, 0xDA, 0x1C, 0x92,
	0xF0, 0x5B, 0x48, 0x7E, 0x9C, 0x3A, 0xEF, 0x80
};

const byte LOCATION_ENTRANCE_Y[LOCATION_COUNT] = {
	0x87, 0x6A, 0xDE, 0x2B, 0x14, 0xB8, 0x80, 0xA9,
	0x78, 0x18, 0xD8, 0x78, 0x3E, 0xD0, 0x3B, 0x5B,
	0x6B, 0xF5, 0x6A, 0x69, 0x6A, 0x91, 0x5A, 0x9E,
	0x3A, 0xA0, 0xF0, 0x08, 0x4A, 0x6B, 0x32, 0xF1,
	0x49, 0x43, 0xA8, 0x14, 0x1B, 0x66, 0xF0, 0x80
};

const char *const LOCATION_NAMES[LOCATION_COUNT] = {
	"MOONGLOW",
	"BRITAIN",
	"JHELOM",
	"YEW",
	"MINOC",
	"TRINSIC",
	"SKARA BRAE",
	"NEW MAGINCIA",
	"FOGSBANE",
	"STORMCROW",
	"GREYHAVEN",
	"WAVEGUIDE",
	"IOLO'S HUT",
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	"WEST BRITANNY",
	"NORTH BRITANNY",
	"EAST BRITANNY",
	"PAWS",
	"COVE",
	"BUCCANEER'S DEN",
	"ARARAT",
	"BORDERMARCH",
	"FARTHING",
	"WINDEMERE",
	"STONEGATE",
	"THE LYCAEUM",
	"EMPATH ABBEY",
	"SERPENT'S HOLD",
	"DECEIT",
	"DESPISE",
	"DESTARD",
	"WRONG",
	"COVETOUS",
	"SHAME",
	"HYTHLOTH",
	"DOOM"
};

} // namespace Data
} // namespace Ultima5
} // namespace Ultima
