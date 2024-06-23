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
#include "common/list.h"
#include "wasteland/wasteland1/gfx/pic.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Gfx {

struct PicsAnimationInstruction {
	int _delay = 0;
	int _frame = 0;

	PicsAnimationInstruction() {}
	PicsAnimationInstruction(int delay, int frame) :
		_delay(delay), _frame(frame) {}
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
	Common::List< PicsAnimationInstruction> _instructions;

public:
	PicsAnimationFrameSet() {}

	/**
	 * Adds a frame to the frame set.
	 * @param delay			The delay
	 * @param frameNo		The raw frame to display in this frame
	 * @param baseFrame		The base frame
	 * @param workingFrame	The current frame which acts as a base for this frame
	 * @param rawFrames		The raw frames
	 */
	void addFrame(int delay, int frameNo, Pic baseFrame,
		Pic *workingFrame, Common::List<RawAnimationFrame> &rawFrames) {
		int index;

		// Apply the raw animation to the current working frame
		rawFrames.get(frameNo).apply(workingFrame);

		// Get frame index. Store new frame if not already present
		index = getFrameIndex(workingFrame, baseFrame);
		if (index == -1)
		{
			this.frames.add(workingFrame.clone());
			index = this.frames.size();
		}

		// Add the animation instruction
		this.instructions.add(new PicsAnimationInstruction(delay, index));
	}


	/**
	 * Returns the raw animation frames for this frame set. Map key is a string
	 * in to format "fromFrame-toFrame". Example: "0-1" to indicate the raw
	 * frame is updating frame 0 to get to frame 1.
	 *
	 * @param baseFrame
	 *            The base frame
	 * @return The raw animation frames.
	 */

	public Map<String, RawAnimationFrame> getRawFrames(Pic baseFrame) {
		Map<String, RawAnimationFrame> rawFrames;
		String key;
		int currentFrame, nextFrame;

		rawFrames = new HashMap<String, RawAnimationFrame>();
		currentFrame = 0;
		for (PicsAnimationInstruction instruction : this.instructions)
		{
			nextFrame = instruction.getFrame();
			key = getRawFrameKey(currentFrame, nextFrame);

			// Do nothing if a raw frame for this animation frame combination
			// has already been calculated.
			if (!rawFrames.containsKey(key))
			{
				// Create the raw frame and add it to the raw frames map
				rawFrames.put(key, getRawFrame(currentFrame, nextFrame,
					baseFrame));
			}

			currentFrame = nextFrame;
		}
		return rawFrames;
	}


	/**
	 * Calculated and returns a raw animation frame.
	 *
	 * @param currentFrame
	 *            The current frame number (0=base frame)
	 * @param nextFrame
	 *            The next frame number (0=base frame)
	 * @param baseFrame
	 *            The base frame
	 * @return The raw animation frame
	 */

	private RawAnimationFrame getRawFrame(int currentFrame, int nextFrame,
		Pic baseFrame) {
		Pic current, next;
		List<RawAnimationFramePart> parts;

		current = currentFrame == 0 ? baseFrame : this.frames
			.get(currentFrame - 1);
		next = nextFrame == 0 ? baseFrame : this.frames.get(nextFrame - 1);

		parts = getDiff(current, next);

		return new RawAnimationFrame(parts, 0);
	}


	/**
	 * Returns an array with the diff values of two frames. The array is two
	 * dimensional array. The first index is the y coordinate and the second is
	 * the x corrdinate. Each array entry holds the byte for two pixels.
	 *
	 * @param frame1
	 *            The first frame to compare
	 * @param frame2
	 *            The second frame to compare
	 * @return The diff
	 */

	private List<RawAnimationFramePart> getDiff(Pic frame1, Pic frame2) {
		int x, y;
		int w, h;
		List<RawAnimationFramePart> diff;
		RawAnimationFramePart last, current;
		int xor;

		w = frame1.getWidth();
		h = frame1.getHeight();
		diff = new ArrayList<RawAnimationFramePart>();
		last = null;
		for (y = 0; y < h; y++)
		{
			for (x = 0; x < w; x += 2)
			{
				xor = (((frame1.getPixel(x, y) ^ frame2.getPixel(x,
					y))) << 4)
					| (frame1.getPixel(x + 1, y) ^ frame2.getPixel(x + 1, y));
				if (xor != 0)
				{
					current = new RawAnimationFramePart((y * w + x) / 2, new int[] { xor }, 0);
					if (last != null && last.isMergable(current))
					{
						last.merge(current);
					} else
					{
						diff.add(current);
						last = current;
					}
				}
			}
		}
		return diff;
	}


	/**
	 * Returns the raw animation frame map key.
	 *
	 * @param frame1
	 *            The first frame
	 * @param frame2
	 *            The second frame
	 * @return The map key
	 */

	public static String getRawFrameKey(int frame1, int frame2) {
		int lowFrame, highFrame;

		lowFrame = Math.min(frame1, frame2);
		highFrame = Math.max(frame1, frame2);
		return lowFrame + "-" + highFrame;
	}


	/**
	 * Checks if the specified frame already exists and returns the frame index.
	 * 0 is the base frame index while indices > 0 are the indexes in the frame
	 * list -1. If the frame is unknown then -1 is returned so the caller can
	 * add the new frame to the list of frames.
	 *
	 * @param workingFrame
	 *            The working frmae
	 * @param baseFrame
	 *            The base frame
	 * @return The frame index or -1 if frame was not found
	 */

	private int getFrameIndex(Pic workingFrame, Pic baseFrame) {
		if (workingFrame.equals(baseFrame))
		{
			return 0;
		}
		for (int i = 0; i < this.frames.size(); i++)
		{
			if (this.frames.get(i).equals(workingFrame))
			{
				return i + 1;
			}
		}
		return -1;
	}


	/**
	 * Returns the frames.
	 *
	 * @return The frames
	 */

	public List<Pic> getFrames() {
		return this.frames;
	}


	/**
	 * Returns the instructions.
	 *
	 * @return The instructions
	 */

	public List<PicsAnimationInstruction> getInstructions() {
		return this.instructions;
	}
}

} // namespace Gfx
} // namespace Wasteland1
} // namespace Wasteland

#endif
