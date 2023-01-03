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

#ifndef LEGEND_IMAGE_H
#define LEGEND_IMAGE_H

#include "common/rect.h"
#include "legend/gfx/picture.h"
#include "legend/gfx/view_element.h"

namespace Legend {
namespace Gfx {

class Image : public ViewElement {
	bool msgFocus(const FocusMessage &msg);
	bool msgUnfocus(const UnfocusMessage &msg); 
private:
	int _picNum = -1;
	int _frameNumber = 0;
private:
	/**
	 * Sets up the state for a blank image
	 */
	void setup();

	/**
	 * Internal method for loading an image
	 */
	bool loadImage();
public:
	bool _active = false;
	byte _field1 = 0;
	byte _fieldA = 0;
	byte _fieldB = 0;
	Picture *_pic = nullptr;

public:
	Image() : ViewElement() {}
	Image(UIElement *parent, const Common::Rect &r) : ViewElement(parent, r) {}
	Image(UIElement *parent, const Common::String &name) : ViewElement(parent, name) {}
	Image(UIElement *parent, const Common::String &name, const Common::Rect &r) :
		ViewElement(parent, name, r) {}
	~Image();

	/**
	 * Draws the image on the screen
	 */
	virtual void draw();

	/**
	 * Loads the picture for the image
	 */
	bool load(int picNumber, int frameNumber = 0);

	/**
	 * Sets up the image with a solid background
	 * @remarks		It will be destroyed when the image is hidden
	 */
	void fill(int color = 0);
};

} // End of namespace Gfx
} // End of namespace Legend

#endif
