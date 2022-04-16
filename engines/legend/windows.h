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

#ifndef LEGEND_WINDOWS_H
#define LEGEND_WINDOWS_H

#include "common/rect.h"

namespace Legend {

#define WINDOW_COUNT 10

struct Region {

};

class Regions {
private:
	int _rgn[WINDOW_COUNT];
	int _num_regions[WINDOW_COUNT];
public:
	int _region_color = 13;
	int _button_height = 11;
	int _up_arrow = 0, _dn_arrow = 0, _elevator_box = 0;
	int _up_arrow_on = 0, _dn_arrow_on = 0;
public:
	Regions();

	
};

struct Window {
	Common::Rect _bounds;
	int _w_x2 = 0;
};

class Windows {
private:
	Common::Array<Window> _windows;
	
public:
	Windows();
};

} // namespace Legend

#endif
