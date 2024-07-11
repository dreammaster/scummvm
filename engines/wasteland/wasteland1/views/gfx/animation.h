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

#ifndef WASTELAND_WASTELAND1_VIEWS_GFX_ANIMATION_H
#define WASTELAND_WASTELAND1_VIEWS_GFX_ANIMATION_H

#include "wasteland/wasteland1/gfx/pics_animation_frame_set.h"
#include "wasteland/events.h"

namespace Wasteland {
namespace Wasteland1 {
namespace Views {

/**
 * Animation view class
 */
class Animation : public UIElement {
private:
	int _animIndex;
	int _animSubIndex;
	Gfx::PicsAnimationFrameSet *_anim = nullptr;
	Common::Point _topLeft;

public:
	Animation(const Common::String &name, UIElement *uiParent,
			int animIndex, int animSubIndex, int x = 8, int y = 8) :
		UIElement(name, uiParent),
		_animIndex(animIndex), _animSubIndex(animSubIndex),
		_topLeft(x, y) {
	}
	virtual ~Animation() {}

	bool msgFocus(const FocusMessage &msg) override;
	bool msgUnfocus(const UnfocusMessage &msg) override;
	void draw() override;
	bool tick() override;
};

} // namespace Views
} // namespace Wasteland1
} // namespace Wasteland

#endif
