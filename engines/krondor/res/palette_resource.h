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

#ifndef KRONDOR_RES_PALETTE_RESOURCE_H
#define KRONDOR_RES_PALETTE_RESOURCE_H

#include "common/memstream.h"
#include "graphics/palette.h"
#include "krondor/res/resource.h"

namespace Krondor {

class PaletteResource : public TaggedResource, public Graphics::Palette {
public:
	PaletteResource() : TaggedResource(), Graphics::Palette(nullptr, 0) {}
	PaletteResource(const Common::String &name) : TaggedResource(),
			Graphics::Palette(nullptr, 0) {
		load(name);
	}
	~PaletteResource() override {}

	void clear();
	void load(const Common::String &name);

	void setActive();
};

} // namespace Krondor

#endif
