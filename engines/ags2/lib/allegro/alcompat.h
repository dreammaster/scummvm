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

#ifndef AGS2_LIB_ALLEGRO_ALCOMPAT_H
#define AGS2_LIB_ALLEGRO_ALCOMPAT_H

#include "ags2/lib/allegro/gfx.h"
#include "ags2/lib/allegro/fixed.h"
#include "ags2/lib/allegro/surface.h"

namespace AGS2 {

inline fixed fcos(fixed x) { return fixcos(x); }
inline fixed fsin(fixed x) { return fixsin(x); }
inline fixed ftan(fixed x) { return fixtan(x); }

inline fixed fadd(fixed x, fixed y) { return fixadd(x, y); }
inline fixed fsub(fixed x, fixed y) { return fixsub(x, y); }
inline fixed fmul(fixed x, fixed y) { return fixmul(x, y); }
inline fixed fdiv(fixed x, fixed y) { return fixdiv(x, y); }
inline fixed fatan(fixed x) { return fixatan(x); }

/* the weird old clipping API */
inline void set_clip(BITMAP *bitmap, int x1, int y_1, int x2, int y2) {
}

inline void clear(BITMAP *bmp) {
	clear_bitmap(bmp);
}

inline void SetForegroundWindow(void *) {
}

} // namespace AGS2

#endif
