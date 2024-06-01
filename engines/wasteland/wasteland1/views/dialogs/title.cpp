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
#include "graphics/palette.h"
#include "wasteland/Wasteland1/views/dialogs/title.h"
#include "wasteland/Wasteland1/Wasteland1.h"
#include "wasteland/gfx/image_decoder.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Views {
namespace Dialogs {

bool Title::msgKeypress(const KeypressMessage &msg) {
	timeout();
	return true;
}

bool Title::msgMouseDown(const MouseDownMessage &msg) {
	timeout();
	return true;
}

void Title::draw() {
	Surface s = getSurface();
	Gfx::ImageDecoder decoder;

	if (!decoder.load("TPICT", 320, 200))
		error("Could not load TPICT");

	s.blitFrom(*decoder.getSurface());
}

bool Title::msgFocus(const FocusMessage &msg) {
	delaySeconds(1);
	return true;
}

void Title::timeout() {
}

} // namespace Dialogs
} // namespace Views
} // namespace Wasteland1
} // namespace Wasteland
