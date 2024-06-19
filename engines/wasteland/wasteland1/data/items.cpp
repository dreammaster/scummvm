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

#include "wasteland/wasteland1/data/items.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Data {

Items::Items() {
	resize(30);
}

void Items::synchronize(Serializer &s) {
	for (uint i = 1; i <= size(); ++i)
		(*this)[i].synchronize(s);
}

void Item::synchronize(Serializer &s) {
	s.syncAsByte(_id);
	s.syncAsByte(_load);
}

} // namespace Data
} // namespace Wasteland1
} // namespace Wasteland
