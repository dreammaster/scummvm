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

#include "common/hash-str.h"
#include "wasteland/wasteland1/gfx/pics_animation_frame_set.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Gfx {

PicsAnimationFrameSet::~PicsAnimationFrameSet() {
	for (auto it = _frames.begin(); it != _frames.end(); ++it)
		delete *it;
}

void PicsAnimationFrameSet::addFrame(int delay, int frameNo, Pic *baseFrame,
	Pic *workingFrame, Common::Array<RawAnimationFrame> &rawFrames) {
	int index;

	// Apply the raw animation to the current working frame
	rawFrames[frameNo].apply(workingFrame);

	// Get frame index. Store new frame if not already present
	index = getFrameIndex(workingFrame, baseFrame);
	if (index == -1) {
		_frames.push_back(workingFrame->clone());
		index = _frames.size();
	}

	// Add the animation instruction
	_instructions.push_back(PicsAnimationInstruction(delay, index));
}

Common::HashMap<Common::String, RawAnimationFrame *> *PicsAnimationFrameSet::getRawFrames(Pic *baseFrame) {
	Common::HashMap<Common::String, RawAnimationFrame *> *rawFrames;
	Common::String key;
	int currentFrame, nextFrame;

	rawFrames = new Common::HashMap<Common::String, RawAnimationFrame *>();
	currentFrame = 0;
	for (PicsAnimationInstruction instruction : _instructions) {
		nextFrame = instruction.getFrame();
		key = getRawFrameKey(currentFrame, nextFrame);

		// Do nothing if a raw frame for this animation frame combination
		// has already been calculated.
		if (!rawFrames->contains(key)) {
			// Create the raw frame and add it to the raw frames map
			(*rawFrames)[key] = getRawFrame(currentFrame, nextFrame,
				baseFrame);
		}

		currentFrame = nextFrame;
	}

	return rawFrames;
}

RawAnimationFrame *PicsAnimationFrameSet::getRawFrame(int currentFrame, int nextFrame,
		Pic *baseFrame) {
	Pic *current, *next;
	Common::Array<RawAnimationFramePart> *parts;

	current = currentFrame == 0 ? baseFrame : _frames[currentFrame - 1];
	next = nextFrame == 0 ? baseFrame : _frames[nextFrame - 1];

	parts = getDiff(current, next);

	return new RawAnimationFrame(parts, 0);
}

Common::Array<RawAnimationFramePart> *PicsAnimationFrameSet::getDiff(Pic *frame1, Pic *frame2) {
	int x, y;
	int w, h;
	Common::Array<RawAnimationFramePart> *diff;
	RawAnimationFramePart *last, *current;
	int xorVal;

	w = frame1->w;
	h = frame1->h;
	diff = new Common::Array<RawAnimationFramePart>();
	last = nullptr;

	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x += 2) {
			xorVal = (((frame1->getPixel(x, y) ^ frame2->getPixel(x,
				y))) << 4)
				| (frame1->getPixel(x + 1, y) ^ frame2->getPixel(x + 1, y));
			if (xorVal != 0) {
				Common::Array<int> xorArray;
				xorArray.push_back(xorVal);

				current = new RawAnimationFramePart((y * w + x) / 2, xorArray, 0);
				if (last != nullptr && last->isMergable(*current)) {
					last->merge(*current);
				} else {
					diff->push_back(*current);
					last = current;
				}
			}
		}
	}

	return diff;
}

Common::String PicsAnimationFrameSet::getRawFrameKey(int frame1, int frame2) {
	int lowFrame, highFrame;

	lowFrame = MIN(frame1, frame2);
	highFrame = MAX(frame1, frame2);
	return lowFrame + "-" + highFrame;
}

int PicsAnimationFrameSet::getFrameIndex(Pic *workingFrame, Pic *baseFrame) {
	if (workingFrame == baseFrame)
		return 0;

	for (uint i = 0; i < _frames.size(); i++) {
		if (_frames[i] == workingFrame) {
			return i + 1;
		}
	}

	return -1;
}

} // namespace Gfx
} // namespace Wasteland1
} // namespace Wasteland
