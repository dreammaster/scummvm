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

#include "common/system.h"
#include "ultima/shared/early/core/rect.h"
#include "ultima/ultima0/data/defines.h"

namespace Ultima {
namespace Shared {
namespace Core {

constexpr int ULTIMA0_SCX = 640;
constexpr int ULTIMA0_SCY = 400;

inline bool isUltima0() {
	return g_system->getWidth() == ULTIMA0_SCX && g_system->getHeight() == ULTIMA0_SCY;
}

TextPoint::TextPoint(int x_, int y_) : Common::Point() {
	const int charSize = isUltima0() ? 16 : 8;
	x = x_ * charSize;
	y = y_ * charSize;
}

TextRect::TextRect(int left_, int top_, int right_, int bottom_) : Common::Rect() {
	const int charSize = isUltima0() ? 16 : 8;
	left = left_ * charSize;
	top = top_ * charSize;
	right = right_ * charSize;
	bottom = bottom_ * charSize;
}

} // namespace Core
} // namespace Shared
} // namespace Ultima
