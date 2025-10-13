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

	scrnOutl(s);
	horzLine(s, 10);
	horzLine(s, 15);

	s.MVCURSOR(12, 0);
	s.PRINTCHR(91);			// TILTED "T" TOP OF LINE
	for (int y = 1; y <= 9; ++y) {
		s.MVCURSOR(12, y);
		s.PRINTCHR(92);		// Vertical bar
	}

	s.MVCURSOR(12, 5);
	s.PRINTCHR(93);			// TILTED "T" LEFT OF LINE
	for (int x = 13; x <= 38; ++x)
		s.PRINTCHR(34);
	s.PRINTCHR(40);			// TILTED "T" RIGHT OF LINE

	s.MVCURSOR(12, 10);
	s.PRINTCHR(94);			// TILTED "T" BOTTOM OF LINE

	//UNITREAD(DRIVE1, CHARSET, BLOCKSZ, SCNTOCBL + 1, 0);
	s.MVCURSOR(1, 16);
	s.PRINTSTR("# CHARACTER NAME  CLASS AC HITS STATUS");
}

void Castle::scrnOutl(Surface &s) {
	s.PRINTCHR(33);		// Upper left corner
	for (int x = 1; x <= 38; ++x)
		s.PRINTCHR(34);
	s.PRINTCHR(35);		// Upper right corner

	for (int y = 1; y <= 22; ++y) {
		s.MVCURSOR(0, y);
		s.PRINTCHR(36);	// Left edge vertical bar
		s.MVCURSOR(39, y);
		s.PRINTCHR(36);	// Right edge vertical bar
	}

	s.MVCURSOR(0, 23);
	s.PRINTCHR(37);		// Bottom left corner
	for (int x = 1; x <= 38; ++x)
		s.PRINTCHR(34);
	s.PRINTCHR(38);		// Bottom right corner
}

void Castle::horzLine(Surface &s, int line) {
	s.MVCURSOR(0, line);

	s.PRINTCHR(39);		// Tilted 'T' on left of line
	for (int x = 1; x <= 38; ++x)
		s.PRINTCHR(34);
	s.PRINTCHR(40);		// TILTED "T" ON RIGHT OF LINE
}

} // namespace Wizardry
