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

#ifndef KRONDOR_RES_IMAGE_RESOURCE_H
#define KRONDOR_RES_IMAGE_RESOURCE_H

#include "common/array.h"
#include "graphics/managed_surface.h"
#include "krondor/res/resource.h"
#include "krondor/res/rect.h"

namespace Krondor {

class ImageResource : public Resource {
	enum ImageFlag {
		FLAG_XYSWAPPED = 0x20,
		FLAG_UNKNOWN = 0x40,
		FLAG_COMPRESSED = 0x80
	};
public:
	Common::Array<Graphics::ManagedSurface> _images;

	void loadImage(Graphics::ManagedSurface &surface, uint flags,
		Common::SeekableReadStream *src);

public:
	ImageResource() : Resource() {}
	ImageResource(const Common::String &name) : Resource() {
		load(name);
	}
	~ImageResource() override {}

	void clear();
	void load(const Common::String &name);
};

} // namespace Krondor

#endif
