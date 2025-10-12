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

#include "common/system.h"
#include "graphics/paletteman.h"
#include "wizardry/views/castle.h"

namespace Wizardry {

bool Castle::msgKeypress(const KeypressMessage &msg) {
	// Any keypress to close the view
	close();
	return true;
}

void Castle::draw() {
	auto s = getSurface();
	s.clear();

	s.writeString("ABCDE");
}

void Castle::horzLine(Surface &surf, int line) {
	surf.MVCURSOR(0, line);
	surf.PRINTCHR(39);		// Tilted 'T' on left of line
#if 0
	HORZHYPH;
	PRINTCHR(CHR(40))          (*TILTED "T" ON RIGHT OF LINE *)
		END;
#endif
}

} // namespace Wizardry
