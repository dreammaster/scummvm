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

#include "legend/early/logic/room.h"

namespace Legend {
namespace Early {

void Room::synchronize(Common::Serializer &s) {
	// TODO: synchronize room data
}

int Room::call(int action) {
	switch (action) {
	case 1: return action01();
	case 2: return action02();
	case 3: return action03();
	case 4: return action04();
	case 5: return action05();
	case 6: return action06();
	case 7: return action07();
	case 8: return action08();
	case 9: return action09();
	case 10: return action10();
	case 11: return action11();
	case 12: return action12();
	case 13: return action13();
	case 14: return action14();
	case 15: return action15();
	case 16: return action16();
	case 17: return action17();
	case 18: return action18();
	case 19: return action19();
	case 20: return action20();
	case 21: return action21();
	case 22: return action22();
	case 23: return action23();
	default: return 0;
	}
}

} // namespace Early
} // namespace Legend
