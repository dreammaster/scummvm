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

#include "common/algorithm.h"
#include "common/str.h"
#include "ultima/ultima0/data/player.h"

namespace Ultima {
namespace Ultima0 {

PLAYER::PLAYER() {
	Common::fill(Name, Name + MAX_NAME + 2, '\0');
	Common::fill(Attr, Attr + MAX_ATTR, 0);
	Common::fill(Object, Object + MAX_OBJ, 0.0);
}

void PLAYER::setIsEnhanced(bool isEnh) {
	Attributes = isEnh ? MAX_ATTR : 6;
	Objects = isEnh ? MAX_OBJ : 6;
}

void PLAYER::setDemo() {
	int i;
	Common::strcpy_s(Name, "Demo");		// Characters Name
	Class = 'F';						// Fighter
	LuckyNumber = 42;					// Always the same.....
	Skill = 1;							// Skill level 1
	Common::fill(Attr, Attr + Attributes, 15);	// High attributes
	Attr[AT_HP] = 18;
	Attr[AT_GOLD] = 99;
	for (i = 0; i < Objects; i++)		// Lots of nice objects
		Object[i] = (i == OB_FOOD || i == OB_BOW) ? 999 : 4.0;
}

void PLAYER::setDebug() {
	int i;
	Common::strcpy_s(Name, "Debuggo");	// Characters Name
	Class = 'F';						// Fighter
	LuckyNumber = 42;					// Always the same.....
	Skill = 1;							// Skill level 1
	Common::fill(Attr, Attr + Attributes, 99);	// High attributes
	Attr[AT_HP] = 999;
	Attr[AT_GOLD] = 9999;
	for (i = 0; i < Objects; i++)		// Lots of nice objects
		Object[i] = (i == OB_FOOD || i == OB_BOW) ? 9999.9 : 99.0;
}

} // namespace Ultima0
} // namespace Ultima
