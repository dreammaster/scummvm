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

#ifndef WASTELAND_WASTELAND1_GFX_PICS_ANIMATION_H
#define WASTELAND_WASTELAND1_GFX_PICS_ANIMATION_H

#include "common/array.h"
#include "common/stream.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Gfx {

class PicsAnimation {
public:
	/**
	 * Read in an animation
	 * @param src		Source stream
	 * @param width		Picture width
	 * @return True if an animation could be read,
	 * or false if the end of the file has been reached.
	*/
	bool read(Common::SeekableReadStream *src, int width = 96);
};

} // namespace Gfx
} // namespace Wasteland1
} // namespace Wasteland

#endif
