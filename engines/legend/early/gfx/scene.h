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

#ifndef LEGEND_EARLY_VIEWS_SCENE_H
#define LEGEND_EARLY_VIEWS_SCENE_H

#include "legend/early/gfx/boxed_element.h"

namespace Legend {
namespace Early {
namespace Gfx {

class Scene : public Gfx::BoxedElement {
public:
	Scene(UIElement *parent, const Common::String &name, const Common::Rect &r) :
		BoxedElement(parent, name, r) {}
	~Scene() override {}

	/**
	 * Draws the visual item on the screen
	 */
	virtual void draw();
};

} // namespace Views
} // namespace Early
} // namespace Legend

#endif
