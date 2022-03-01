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

#ifndef DINK_LIB_RECT_H
#define DINK_LIB_RECT_H

#include "common/rect.h"

namespace Dink {

typedef Common::Rect RECT;
typedef Common::Point POINT;
typedef RECT *LPRECT;

inline void CopyRect(LPRECT lprcDst, const LPRECT lprcSrc) {
	*lprcDst = *lprcSrc;
}

inline void InflateRect(LPRECT lprc, int dx, int dy) {
	lprc->left -= dx;
	lprc->right += dx;
	lprc->top -= dy;
	lprc->bottom += dy;
}

inline bool inside_box(int16 x, int16 y, const RECT &rect) {
	return rect.contains(x, y);
}


inline void SetRect(Common::Rect *r, int16 x1, int16 y1, int16 x2, int16 y2) {
	*r = Common::Rect(x1, y1, x2, y2);
}

inline void OffsetRect(Common::Rect *r, int16 dx, int16 dy) {
	r->translate(dx, dy);
}


} // namespace Dink

#endif
