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

#ifndef WASTELAND_GFX_IMAGE_DECODER_H
#define WASTELAND_GFX_IMAGE_DECODER_H

#include "graphics/managed_surface.h"
#include "common/stream.h"

namespace Wasteland {
namespace Gfx {

/**
 * Implements an image decoder for Wasteland image files
 */
class ImageDecoder {
private:
	Graphics::ManagedSurface _surface;

	/**
	 * Handles raw decompression of the s
	 */
	static void loadStream(Common::ReadStream &src, Common::WriteStream &dest);

	/**
	 * Copies a decompressed stream to the surface
	 */
	void copyPixelsToSurface(Common::SeekableReadStream &src);

public:
	/**
	 * Loads an image into the internal _surface
	*/
	bool load(const Common::Path &name, uint16 w, uint16 h);

	/**
	 * Special variation for loading the Fountain of Dreams Welcome image.
	 * It seems to maybe be a primitive collection format, with the first
	 * byte being an image count (1 for welcome), and the next two bytes
	 * being an offset to just after the first image, which starts at offset 9
	 */
	bool loadWelcome(const Common::Path &name);

	/**
	 * Returns the surface of the decoded image
	 */
	const Graphics::ManagedSurface *getSurface() const { return &_surface; }
};

} // namespace Gfx
} // namespace Wasteland

#endif
