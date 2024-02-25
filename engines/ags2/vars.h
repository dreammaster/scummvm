
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

#ifndef AGS2_VARS_H
#define AGS2_VARS_H

namespace AGS2 {
	
class Vars;

extern Vars *g_vars;

class Vars {
public:
	static const char *ac_engine_copyright;

public:
	// routefnd
	int _walk_area_zone5 = 0;
	int _routex1 = 0;

public:
	Vars() {}
};

#define _G(X) (g_vars->_##X)

} // namespace AGS2

#endif
