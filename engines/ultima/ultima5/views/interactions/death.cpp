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

#include "ultima/ultima5/views/interactions/death.h"

namespace Ultima {
namespace Ultima5 {
namespace Views {
namespace Interactions {

// The area of the map, which is blacked out
constexpr int AREA_LEFT = 8;
constexpr int AREA_TOP = 8;
constexpr int AREA_SIZE = 176;

Death::Death(MessageLog &log) : Interaction(log), _logic(log) {
	_logic.begin();
}

bool Death::keypress(const KeypressMessage &) {
	_logic.finish();
	_revived = true;
	return true;
}

void Death::draw(View &view) {
	if (_revived)
		return;

	auto s = view.getSurface();
	s.fillRect(Common::Rect(AREA_LEFT, AREA_TOP, AREA_LEFT + AREA_SIZE, AREA_TOP + AREA_SIZE), 0);
}

} // namespace Interactions
} // namespace Views
} // namespace Ultima5
} // namespace Ultima
