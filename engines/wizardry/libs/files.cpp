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

#include "wizardry/libs/files.h"
#include "wizardry/data/globals.h"

namespace Wizardry {

void UNITREAD(int unit, void *buffer, int byteCount, int blockNumber, int offset) {
	assert(unit == DRIVE1);
	assert(blockNumber >= 0 && blockNumber < 640);

	auto &f = _G(dsk);
	f.seek(blockNumber * 512 + offset);
	if (f.read(buffer, byteCount) != (uint32)byteCount)
		error("Error reading raw dsk data");
}

} // namespace Wizardry
