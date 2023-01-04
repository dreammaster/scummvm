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

#ifndef LEGEND_EARLY_GFX_BOXED_ELEMENT_H
#define LEGEND_EARLY_GFX_BOXED_ELEMENT_H

#include "legend/gfx/view_element.h"

namespace Legend {
namespace Early {
namespace Gfx {

/**
 * Base class for visual areas with a white background with a thin black border
 */
class BoxedElement : public Legend::Gfx::ViewElement {
public:
	BoxedElement(UIElement *parent) : Legend::Gfx::ViewElement(parent) {
		_bounds.setBorderSize(1);
	}
	BoxedElement(UIElement *parent, const Common::Rect &r) :
			Legend::Gfx::ViewElement(parent, r) {
		_bounds.setBorderSize(1);
	}
	BoxedElement(UIElement *parent, const String &name) :
			Legend::Gfx::ViewElement(name, parent) {
		_bounds.setBorderSize(1);
	}
	BoxedElement(UIElement *parent, const String &name,
			const Common::Rect &r) : Legend::Gfx::ViewElement(parent, name, r) {
		_bounds.setBorderSize(1);
	}
	~BoxedElement() override {}

	/**
	 * Draws the visual item on the screen
	 */
	void draw() override;
};

} // namespace Gfx
} // namespace Early
} // namespace Legend

#endif
