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

} // namespace Ultima0
} // namespace Ultima
