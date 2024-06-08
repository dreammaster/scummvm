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

#include "wasteland/wasteland1/views/dialogs/dialog.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Views {
namespace Dialogs {

void Dialog::drawFrame(const Common::Rect &r) {
	Surface s = getSurface();

	s.writeChar(14, r.left, r.top);
	for (int x = r.left + 1; x < r.right; ++x)
		s.writeChar(18);
	s.writeChar(15);

	for (int y = r.top + 1; y < r.bottom; ++y) {
		s.writeChar(13, r.left, y);
		s.writeChar(19, r.right, y);
	}

	s.writeChar(16, r.left, r.bottom);
	for (int x = r.left + 1; x < r.right; ++x)
		s.writeChar(18);
	s.writeChar(17);
}

} // namespace Dialogs
} // namespace Views
} // namespace Wasteland1
} // namespace Wasteland
