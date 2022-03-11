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

#ifndef LEGEND_SHANNARA_FLIX_H
#define LEGEND_SHANNARA_FLIX_H

#include "legend/file.h"
#include "legend/graphics.h"

namespace Legend {
namespace Shannara {

struct QHeader {
	int _id = 0;
	int _field2 = 0;
	int _field4 = 0;
	int _field6 = 0;
	int _field8 = 0;
	int _field9 = 0;
	int _fieldA = 0;
	int _fieldC = 0;
	int _field10 = 0;
	int _field16 = 0;

	void load(Stream *src);
};

struct QInfo {
	int _field0 = 0;
	int _field2 = 0;
	int _field4 = 0;
	int _field6 = 0;
};

extern void q_init(int y, PalettePtr pal);
extern void q_open(const char *filename, size_t bufSize = 0x8000);
extern void q_close();
extern void q_kill();

} // namespace Shannara
} // namespace Legend

#endif
