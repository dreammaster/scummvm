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

#include "wasteland/wasteland1/data/parties.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Data {

Party::Party() {
	_members.resize(7);
}

void Party::synchronize(Common::Serializer &s) {
	s.skip(1);
	for (int i = 0; i < 7; ++i)
		s.syncAsByte(_members[i]);

	s.syncAsSByte(_x);
	s.syncAsSByte(_y);
	s.syncAsSByte(_map);
	s.syncAsSByte(_prevX);
	s.syncAsSByte(_prevY);
	s.syncAsSByte(_prevMap);
}

Parties::Parties() {
	_parties.resize(4);
	_roster.resize(8);
}

void Parties::synchronize(Common::Serializer &s) {
	for (int i = 0; i < 4; ++i)
		_parties[i].synchronize(s);
	s.skip(0xc8);

	for (int i = 0; i < 8; ++i)
		_roster[i].synchronize(s);
}

} // namespace Data
} // namespace Wasteland1
} // namespace Wasteland
