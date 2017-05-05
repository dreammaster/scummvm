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

#ifndef LEGEND_GFX_H
#define LEGEND_GFX_H

#include "common/rect.h"

namespace Legend {
namespace Gfx {

struct Gfx {
	int _field0;
	int _field2;
	int _field4;
	int _field6;
	int _field8;
	int _fieldA;
	int _fieldC;
	int _fieldE;
	int _field10;
	int _field12;
	int _field14;
	int _field16;
	int _field18;
	Common::Rect _bounds;
	int _field1C;
	int _field1E;
	int _field20;
	int _field22;
	int _color;
	int _field26;
	int _field28;
	int _field2A;
	int _field2C;
	int _field2E;
	int _field30;
	int _field32;
	int _field34;
	int _field36;
	int _field38;
	int _field3A;
	int _field3C;
	int _field3E;
	int _field40;
	int _field42;
	int _field44;
	int _field46;
	int _field48;
	int _field4A;
	int _field4C;
	int _field4E;
	int _field50;
	int _field52;
	int _field54;
	int _field56;
public:
	/**
	 * Constructor
	 */
	Gfx();

	int fn1(bool flag, int color, int val3);

	void eraseRect(const Common::Rect &r, int val);

	void setBounds(const Common::Rect &r) { _bounds = r; }
};

} // End of namespace Gfx
} // End of namespace Legend

#endif
