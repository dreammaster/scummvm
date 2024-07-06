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
#include "wasteland/wasteland1/files/bit_stream.h"
#include "wasteland/wasteland1/gfx/pic.h"
#include "wasteland/wasteland1/gfx/pics_animation_frame_set.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Gfx {

class PicsAnimation {
private:
	Gfx::Pic *_baseFrame = nullptr;
	Common::List<PicsAnimationFrameSet> _frameSets;

	/**
	 * Reads the animation data from the stream.
	 *
	 * @param src	The input stream
	 */
	void readAnimationData(BitStream *src);

public:
	~PicsAnimation();

	/**
	 * Read in an animation
	 * @param src		Source stream
	 * @param width		Picture width
	 * @return True if an animation could be read,
	 * or false if the end of the file has been reached.
	*/
	bool read(Common::SeekableReadStream *src, int width = 96);

	/**
	 * Return the frame sets for the animation
	 */
	const Common::List<PicsAnimationFrameSet> &getFrameSets() const {
		return _frameSets;
	}
};

} // namespace Gfx
} // namespace Wasteland1
} // namespace Wasteland

#endif
