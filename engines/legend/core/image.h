/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef LEGEND_IMAGE_H
#define LEGEND_IMAGE_H

#include "common/rect.h"
#include "legend/core/picture.h"

namespace Legend {

class Image {
public:
	bool _active;
	byte _field1;
	Common::Rect _bounds;
	byte _fieldA;
	byte _fieldB;
	const byte *_pixels;
	Picture *_pic;
public:
	Image();
	~Image();

	/**
	 * Loads the picture for the image
	 */
	bool load(int picNumber, int frameNumber = 0);

	bool setBounds(const Common::Rect &r);
};

} // End of namespace Legend

#endif
