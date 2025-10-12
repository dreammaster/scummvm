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

#include "common/memstream.h"
#include "wizardry/data/scenario.h"

namespace Wizardry {

void TSCNTOC::load(const byte *src) {
	Common::MemoryReadStream s(src, 512);

	s.read(_title, 40);
	_numLevels = s.readUint16LE();
	_monsters = s.readUint16LE();
	_items = s.readUint16LE();
	_spells = s.readUint16LE();
	s.read(_maze, 10);
	_messages = s.readUint16LE();
	_specials = s.readUint16LE();
	_rewards = s.readUint16LE();
}

} // namespace Wizardry
