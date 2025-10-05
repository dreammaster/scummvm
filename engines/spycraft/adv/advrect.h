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

#ifndef SPYCRAFT_ADVRECT_H
#define SPYCRAFT_ADVRECT_H

namespace Spycraft {

struct SRect {
	int top;
	int left;
	int bottom;
	int right; 
	int width;
	int height;
};

extern int SRect_Intersects(const SRect *rect1, const SRect *rect2);
extern int SRect_Contains(const SRect *rect1, const SRect *rect2);
extern void SRect_Copy(SRect *dest, const SRect *src);
extern void SRect_Union(const SRect *rect1, const SRect *rect2, SRect *out);
extern void SRect_Init(SRect *rect, int l, int t, int r, int b);
extern void SRect_GetIntersection(const SRect *r1, const SRect *r2, SRect *out);

} // namespace Spycraft

#endif
