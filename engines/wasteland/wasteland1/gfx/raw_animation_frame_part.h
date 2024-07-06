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

#ifndef WASTELAND_WASTELAND1_GFX_RAW_ANIMATION_FRAME_PART_H
#define WASTELAND_WASTELAND1_GFX_RAW_ANIMATION_FRAME_PART_H

#include "common/array.h"
#include "wasteland/wasteland1/gfx/pic.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Gfx {

/**
 * A raw animation frame part defines a starting position in the base frame and
 * a number of byte diffs (palette XORs).
 */
struct RawAnimationFramePart {
private:
	int _offset = 0;				// The starting offset
	Common::Array<int> _diff;		// The diff (palette XORs)
	int _size = 0;					// The frame part size

public:
	RawAnimationFramePart() {}

	/**
	 * Constructor
	 * @param offset		The starting offset
	 * @param diff			The diff (palette XORs)
	 * @param size			The size
	 */
	RawAnimationFramePart(int offset, Common::Array<int> &diff, int size) :
		_offset(offset), _diff(diff), _size(size) {
	}

	/**
	 * Applies this animation frame part to a image.
	 * @param image		The image to modify
	 */
	void apply(Pic *image);

	/**
	 * Parses the next animation frame part from the specified input stream.
	 * @param stream	The input stream
	 */
	bool read(Common::ReadStream *stream);

	/**
	 * Returns the size.
	 * @return The size
	 */
	int getSize() const {
		return _size;
	}

	/**
	 * Checks if this frame part is mergable with the specified animation frame part
	 * @param other		The other animation frame part
	 * @return If the frames are mergable or not
	 */
	bool isMergable(const RawAnimationFramePart &other) const;

	/**
	 * Merges this frame part with an other frame part
	 * @param other		The other frame part
	 */
	void merge(const RawAnimationFramePart &other);
};

} // namespace Gfx
} // namespace Wasteland1
} // namespace Wasteland

#endif
