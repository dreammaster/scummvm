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

#ifndef WASTELAND_WASTELAND1_GFX_RAW_ANIMATION_FRAME_H
#define WASTELAND_WASTELAND1_GFX_RAW_ANIMATION_FRAME_H

#include "common/list.h"
#include "wasteland/wasteland1/gfx/pic.h"
#include "wasteland/wasteland1/gfx/raw_animation_frame_part.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Gfx {

/**
 * A raw animation frame represents the raw frame found in the wasteland
 * animation data block. It consists of multiple raw animation frame parts.
 */
struct RawAnimationFrame {
private:
	// The raw animation frame parts
	Common::Array<RawAnimationFramePart *> *_parts = nullptr;

	// The size of the animation frame
	int _size = 0;

public:
	/**
	 * Constructor
	 */
	RawAnimationFrame() {}

	/**
	 * Constructor
	 * @param parts		The animation frame parts
	 * @param size		The size of the animation frame
	 */
	RawAnimationFrame(Common::Array<RawAnimationFramePart *> *parts, int size) :
		_parts(parts), _size(size) {
	}
	~RawAnimationFrame();

	/**
	 * Applies this animation frame to a image
	 * @param image		The image to update
	 */
	void apply(Pic *image);

	/**
	 * Reads the next raw animation frame from the input stream.
	 *
	 * @param stream	The input stream
	 */
	void read(Common::ReadStream *stream);

	/**
	 * Returns the size.
	 *
	 * @return The size
	 */

	int getSize() const {
		return _size;
	}
};

} // namespace Gfx
} // namespace Wasteland1
} // namespace Wasteland

#endif
