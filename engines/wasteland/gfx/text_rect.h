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

#ifndef WASTELAND_WASTELAND1_DATA_FONT_RECT_H
#define WASTELAND_WASTELAND1_DATA_FONT_RECT_H

#include "common/rect.h"
#include "wasteland/gfx/surface.h"

namespace Wasteland {
namespace Gfx {

struct TextRect : public Common::Rect {
	TextRect() : Common::Rect() {}
	TextRect(int16 w, int16 h) : Common::Rect(w * FONT_W, h * FONT_H) {}
	TextRect(int16 x1, int16 y1, int16 x2, int16 y2) :
		Common::Rect(x1 * FONT_W, y1 * FONT_H, x2 * FONT_W, y2 * FONT_H) {}
};

struct TextPoint : public Common::Point {
	TextPoint() : Common::Point() {}
	TextPoint(int16 x, int16 y) : Common::Point(x * FONT_W, y * FONT_H) {}
};

} // namespace Gfx

using Gfx::TextRect;
using Gfx::TextPoint;

} // namespace Wasteland

#endif
