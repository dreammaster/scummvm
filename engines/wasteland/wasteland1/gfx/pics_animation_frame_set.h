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

#ifndef WASTELAND_WASTELAND1_GFX_PICS_ANIMATION_FRAME_SET_H
#define WASTELAND_WASTELAND1_GFX_PICS_ANIMATION_FRAME_SET_H

#include "common/array.h"
#include "common/hash-str.h"
#include "common/list.h"
#include "wasteland/wasteland1/gfx/pic.h"
#include "wasteland/wasteland1/gfx/raw_animation_frame.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Gfx {

struct PicsAnimationInstruction {
private:
	int _delay = 0;
	int _frame = 0;

public:
	PicsAnimationInstruction() {}
	PicsAnimationInstruction(int delay, int frame) :
		_delay(delay), _frame(frame) {}

	int getDelay() const {
		return _delay;
	}
	int getFrame() const {
		return _frame;
	}
};

/**
 * An Animation Frame Set contains a list of animation frames and the playing
 * instructions of the frames which defines the delay and play order of the
 * frames.
 *
 * Each animation frame is just a transparent image where only the part
 * which is changed in this animation frame is opaque. So to get the real
 * complete frame you have to overlay this transparent image over the base frame
 * image.
 */
class PicsAnimationFrameSet {
private:
	Common::Array<Pic *> _frames;
	Common::Array<PicsAnimationInstruction> _instructions;

	/**
	 * Calculated and returns a raw animation frame.
	 *
	 * @param currentFrame	The current frame number (0=base frame)
	 * @param nextFrame		The next frame number (0=base frame)
	 * @param baseFrame		The base frame
	 * @return The raw animation frame
	 */
	RawAnimationFrame *getRawFrame(int currentFrame, int nextFrame,
		Pic *baseFrame);


	/**
	 * Returns an array with the diff values of two frames. The array is two
	 * dimensional array. The first index is the y coordinate and the second is
	 * the x co-ordinate. Each array entry holds the byte for two pixels.
	 *
	 * @param frame1	The first frame to compare
	 * @param frame2	The second frame to compare
	 * @return The diff
	 */
	Common::Array<RawAnimationFramePart> getDiff(Pic *frame1, Pic *frame2);

	/**
	 * Checks if the specified frame already exists and returns the frame index.
	 * 0 is the base frame index while indices > 0 are the indexes in the frame
	 * list -1. If the frame is unknown then -1 is returned so the caller can
	 * add the new frame to the list of frames.
	 *
	 * @param workingFrame	The working frame
	 * @param baseFrame		The base frame
	 * @return The frame index or -1 if frame was not found
	 */
	int getFrameIndex(Pic *workingFrame, Pic *baseFrame);

public:
	PicsAnimationFrameSet() {
	}
	~PicsAnimationFrameSet();

	/**
	 * Adds a frame to the frame set.
	 * @param delay			The delay
	 * @param frameNo		The raw frame to display in this frame
	 * @param baseFrame		The base frame
	 * @param workingFrame	The current frame which acts as a base for this frame
	 * @param rawFrames		The raw frames
	 */
	void addFrame(int delay, int frameNo, Pic *baseFrame,
		Pic *workingFrame, Common::Array<RawAnimationFrame> &rawFrames);

	/**
	 * Returns the raw animation frames for this frame set. Map key is a string
	 * in to format "fromFrame-toFrame". Example: "0-1" to indicate the raw
	 * frame is updating frame 0 to get to frame 1.
	 *
	 * @param baseFrame		The base frame
	 * @return The raw animation frames.
	 */
	Common::HashMap<Common::String, RawAnimationFrame *> *getRawFrames(Pic *baseFrame);

	/**
	 * Returns the raw animation frame map key.
	 *
	 * @param frame1
	 *            The first frame
	 * @param frame2
	 *            The second frame
	 * @return The map key
	 */

	static Common::String getRawFrameKey(int frame1, int frame2);

	/**
	 * Returns the frames.
	 *
	 * @return The frames
	 */
	const Common::Array<Pic *> &getFrames() {
		return _frames;
	}

	/**
	 * Returns the instructions.
	 * @return The instructions
	 */
	const Common::Array<PicsAnimationInstruction> &getInstructions() {
		return _instructions;
	}
};

} // namespace Gfx
} // namespace Wasteland1
} // namespace Wasteland

#endif
