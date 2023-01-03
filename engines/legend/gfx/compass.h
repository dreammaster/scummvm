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

#ifndef LEGEND_GFX_COMPASS_H
#define LEGEND_GFX_COMPASS_H

#include "legend/gfx/view_element.h"
#include "legend/gfx/image.h"

namespace Legend {
namespace Gfx {

class Compass : public ViewElement {
private:
	PictureArray _images;
private:
	/**
	 * Loads the pictures for the compass
	 */
	void load(uint picNum = 0);
public:
	Compass() : ViewElement() { load(); }
	Compass(UIElement *parent, const Common::Rect &r) : ViewElement(parent, r) { load(); }
	Compass(UIElement *parent, const Common::String &name) : ViewElement(parent, name) { load(); }
	Compass(UIElement *parent, const Common::String &name, const Common::Rect &r) : ViewElement(parent, name, r) { load(); }
	virtual ~Compass() {}

	/**
	 * Draws the container by iterating through each child and letting it draw
	 */
	virtual void draw();
};

} // End of namespace Gfx
} // End of namespace Legend

#endif
