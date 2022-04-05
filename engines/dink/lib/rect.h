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

/**
 * Rectangle class. Note that we're not deriving from Common::Rect
 * because some rects defined in the dink.ini setup have invalid
 * right < left values, so RECT asserts on them
 */
struct RECT {
public:
	int16 left = 0, top = 0, right = 0, bottom = 0;

	RECT() {}
	RECT(int16 x1, int16 y1, int16 x2, int16 y2) :
		left(x1), top(y1), right(x2), bottom(y2) {}

	void clear() {
		left = top = right = bottom = 0;
	}
	bool contains(int16 x, int16 y) const {
		return (x >= left && x <= right && y >= top && y <= bottom);
	}
	void translate(int16 dx, int16 dy) {
		left += dx;
		right += dx;
		top += dy;
		bottom += dy;
	}
};

typedef Common::Point POINT;
typedef RECT *LPRECT;
typedef POINT *LPPOINT;

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


inline void SetRect(RECT *r, int16 x1, int16 y1, int16 x2, int16 y2) {
	*r = RECT(x1, y1, x2, y2);
}

inline void OffsetRect(RECT *r, int16 dx, int16 dy) {
	r->translate(dx, dy);
}


} // namespace Dink

#endif
