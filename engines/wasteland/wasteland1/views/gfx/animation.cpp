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

#include "wasteland/wasteland1/views/gfx/animation.h"
#include "wasteland/wasteland1/gfx/pics.h"
#include "wasteland/wasteland1/wasteland1.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Views {

using Wasteland1::Gfx::PicsDecoder;
using Wasteland1::Gfx::PicsAnimation;
using Wasteland1::Gfx::PicsAnimationFrameSet;

bool Animation::msgFocus(const FocusMessage &msg) {
	// Get a reference to the specified animation
	PicsDecoder &pics = g_engine->_pics;
	PicsAnimation &anim = pics.getAnimation(_animIndex);
	auto &frameSets = anim.getFrameSets();

	Common::List<PicsAnimationFrameSet>::iterator it = frameSets.begin();
	for (int i = 0; i < _animSubIndex; ++i)
		++it;

	auto &frameSet = *it;
	_anim = &frameSet;

	// Set the animation control area
	setBounds(Common::Rect(_topLeft.x, _topLeft.y,
		_topLeft.x + _anim->width(),
		_topLeft.y + _anim->height()));

	// Start the animation
	_anim->start();
	return UIElement::msgFocus(msg);
}

bool Animation::msgUnfocus(const UnfocusMessage &msg) {
	if (msg._isClosing)
		_anim->stop();
	return UIElement::msgUnfocus(msg);
}

void Animation::draw() {
	Surface s = getSurface();

	const Graphics::Surface *frame = _anim->getSurface();
	s.blitFrom(*frame);
}

bool Animation::tick() {
	if (_anim->isPlaying() && _anim->needsUpdate())
		draw();

	return true;
}

bool Animation::backgroundTick() {
	if (_anim->isPlaying() && _anim->needsUpdate())
		draw();

	return true;
}

} // namespace Views
} // namespace Wasteland1
} // namespace Wasteland
