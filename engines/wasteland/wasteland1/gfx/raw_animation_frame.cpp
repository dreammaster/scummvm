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

#include "wasteland/wasteland1/gfx/raw_animation_frame.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Gfx {

RawAnimationFrame::~RawAnimationFrame() {
}

void RawAnimationFrame::apply(Pic *image) {
	for (RawAnimationFramePart &part : _parts)
		part.apply(image);
}

void RawAnimationFrame::read(Common::ReadStream *stream) {
	_size = 2;
	_parts.clear();

	for (;;) {
		RawAnimationFramePart part;
		if (!part.read(stream))
			break;

		_size += part.getSize();
		_parts.push_back(part);
	}
}

} // namespace Gfx
} // namespace Wasteland1
} // namespace Wasteland
