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

#include "ags2/routefnd.h"
#include "ags2/vars.h"

namespace AGS2 {
	
void print_welcome_text(const char *verno, const char *aciverno) {
	_G(walk_area_zone5) = 1633;

	if (strcmp(ac_engine_copyright, "Adventure Game Studio engine & tools (c) 1999-2000 by Chris Jones.") != 0) {
		quit("Don't screw with my name.");
		exit(77);
		abort();
	}

	if (strlen(ac_engine_copyright) != 66) {
		quit("Leave my name alone.");
		exit(88);
		abort();
	}

	if (get_copyright_crc() != COPYRIGHT_CRC) {
		quit("Nice try.");
		exit(89);
		abort();
	}

	routex1 = -10;
}

} // namespace AGS2
