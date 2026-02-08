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

#ifndef ULTIMA0_SHARED_CORE_RECT_H
#define ULTIMA0_SHARED_CORE_RECT_H

#include "common/rect.h"
#include "common/system.h"

namespace Ultima {
namespace Shared {
namespace Core {

class TextPoint : public Common::Point {
public:
	TextPoint() : Common::Point() {
	}
	TextPoint(int x_, int y_);
};

class TextRect : public Common::Rect {
public:
	TextRect() : Common::Rect() {
	}
	TextRect(int left_, int top_, int right_, int bottom_);
};

} // namespace Core

using Core::TextPoint;
using Core::TextRect;

} // namespace Shared
} // namespace Ultima

#endif
