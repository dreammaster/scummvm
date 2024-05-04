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

#include "krondor/res/screen_resource.h"
#include "krondor/defs.h"

namespace Krondor {

void ScreenResource::clear() {
	Graphics::ManagedSurface::clear();
}

void ScreenResource::load(const Common::String &name) {
	clear();
	File f(name);

	if (f.readUint16LE() != 0x27b6)
		f.seek(-2, SEEK_CUR);
	if (f.readByte() != 2)
		error("Screen data corruption");

	Common::SeekableReadStream *pixels = f.decompressLZW();
	assert(pixels && pixels->size() == (SCREEN_WIDTH * SCREEN_HEIGHT));

	create(SCREEN_WIDTH, SCREEN_HEIGHT, Graphics::PixelFormat::createFormatCLUT8());
	pixels->read((byte *)getPixels(), pixels->size());

	delete pixels;
}

} // namespace Krondor
