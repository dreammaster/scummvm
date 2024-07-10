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

#ifndef WASTELAND_WASTELAND1_GFX_PICS_H
#define WASTELAND_WASTELAND1_GFX_PICS_H

#include "common/array.h"
#include "common/stream.h"
#include "wasteland/wasteland1/gfx/pics_animation.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Gfx {

class PicsDecoder : public Common::Array<PicsAnimation> {
private:
	Common::Array<PicsAnimation> _animations;

public:
	void load(const char *filename);
	void load(Common::SeekableReadStream *src);

	size_t size() const {
		return _animations.size();
	}
	PicsAnimation &getAnimation(uint idx) {
		return _animations[idx];
	}
};

} // namespace Gfx
} // namespace Wasteland1
} // namespace Wasteland

#endif
