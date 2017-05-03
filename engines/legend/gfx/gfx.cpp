/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "legend/gfx/gfx.h"

namespace Legend {

Gfx::Gfx() : _bounds(0, 0, 319, 199) {
	_field0 = 0;
	_field2 = 0;
	_field4 = 0;
	_field6 = 0;
	_field8 = 0;
	_fieldA = 0;
	_fieldC = 1;
	_fieldE = 0;
	_field10 = 1;
	_field12 = 0;
	_field14 = 0;
	_field16 = 0;
	_field18 = 0;
	_field22 = 0;
	_color = 0;
	_field26 = 1;
	_field28 = -1;
	_field2A = 1;
	_field2C = 0;
	_field2E = 0;
	_field30 = 0;
	_field32 = 0;
	_field34 = 0;
	_field36 = 0;
	_field38 = 0x3FF;
	_field3A = 0x3FF;
	_field3C = 0;
	_field3E = 0;
	_field40 = 0;
	_field42 = 0;
	_field44 = 0;
	_field46 = 0;
	_field48 = 0;
	_field4A = 0;
	_field4C = 0;
	_field4E = 1;
	_field50 = 3;
	_field52 = 1;
	_field54 = 1;
	_field56 = 1;
}

int Gfx::fn1(bool flag, int color, int val3) {
	if (val3 < 0 || val3 > 11)
		return -4001;

	_field22 = val3;
	_color = color;
	_field26 = flag ? 1 : 8;
	return 0;
}

void Gfx::eraseRect(const Common::Rect &r, int val) {
	// TODO
}

} // End of namespace Legend
