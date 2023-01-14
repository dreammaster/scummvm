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

#include "legend/games/gateway/core/persisted.h"

namespace Legend {
namespace Early {
namespace Gateway {

void STRUC24::clear() {
	_dummy1 = 0;
	_dummy2 = 0;
	_dummy3 = 0;
	_dummy4 = 0;
	_dummy5 = 0;
	_dummy6 = 0;
	_dummy7 = 0;
	_dummy8 = 0;
	_dummy9 = 0;
	_dummy10 = 0;
	_dummy11 = 0;
	_dummy12 = 0;
}

void STRUC24::synchronize(Common::Serializer &s) {
	s.syncAsUint16LE(_dummy1);
	s.syncAsUint16LE(_dummy2);
	s.syncAsUint16LE(_dummy3);
	s.syncAsUint16LE(_dummy4);
	s.syncAsUint16LE(_dummy5);
	s.syncAsUint16LE(_dummy6);
	s.syncAsUint16LE(_dummy7);
	s.syncAsUint16LE(_dummy8);
	s.syncAsUint16LE(_dummy9);
	s.syncAsUint16LE(_dummy10);
	s.syncAsUint16LE(_dummy11);
	s.syncAsUint16LE(_dummy12);
}

void STRUC3C::synchronize(Common::Serializer &s) {
	s.syncAsUint16LE(_field0);
	s.skip(58);
}

} // namespace Gateway
} // namespace Early
} // namespaceCo Legend
