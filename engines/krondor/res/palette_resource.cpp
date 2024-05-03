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

#include "krondor/res/palette_resource.h"

namespace Krondor {

void PaletteResource::clear() {
	_palette.clear();
}

void PaletteResource::load(const Common::String &name) {
	clear();
	File f(name);
	loadIndex(&f);

	Common::SeekableReadStream *tag = getTag(&f, TAG_VGA);
	assert(tag);

	// Load and convert the palette bytes
	Common::Array<byte> data(tag->size());
	tag->read(&data[0], tag->size());
	delete tag;

	for (uint i = 0; i < data.size(); ++i)
		data[i] = data[i] << 2;

	// Set the palette contents
	_palette = Graphics::Palette(&data[0], data.size() / 3);	 
}

} // namespace Krondor
