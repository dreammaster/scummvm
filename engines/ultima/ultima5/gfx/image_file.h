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

#ifndef ULTIMA5_GFX_IMAGE_FILE_H
#define ULTIMA5_GFX_IMAGE_FILE_H

#include "common/array.h"
#include "graphics/managed_surface.h"
#include "ultima/ultima5/data/data.h"

namespace Ultima {
namespace Ultima5 {
namespace Gfx {

enum ImageFlags {
	IMAGE_VFLIP = 1,
	IMAGE_HFLIP = 2
};

/**
 * A .16 (EGA) or .4 (CGA) graphics file: a count, then an offset for each image
 * and its mask. EGA images are 4 bits per pixel with each row padded to a
 * multiple of 4 bytes, and CGA ones 2 bits per pixel with no padding. A mask is
 * one bit per pixel, with set bits being transparent
 */
class ImageFile {
private:
	Common::Array<byte> _data;
	uint _count = 0;
	bool _cga = false;

	const byte *imagePtr(uint idx) const;
	const byte *maskPtr(uint idx) const;

public:
	/**
	 * Loads an image file
	 * @param baseName	The name without its extension, such as ultima
	 * @returns		False if it couldn't be loaded
	 */
	bool load(const char *baseName, Data::RenderMode mode);

	uint size() const {
		return _count;
	}

	int getWidth(uint idx) const;
	int getHeight(uint idx) const;

	/**
	 * Draws an image, clipped to the surface
	 */
	void draw(Graphics::ManagedSurface &dst, uint idx, int x, int y, int flags = 0) const;
};

} // namespace Gfx
} // namespace Ultima5
} // namespace Ultima

#endif
