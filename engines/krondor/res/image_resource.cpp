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

#include "krondor/res/image_resource.h"

namespace Krondor {

void ImageResource::clear() {
	_images.clear();
}

void ImageResource::load(const Common::String &name) {
	clear();
	File f(name);

	if (f.readUint16LE() != 0x1066)
		error("Invalid image resource");

	CompressionType compression = (CompressionType)f.readUint16LE();
	uint numImages = (uint)f.readUint16LE();
	Common::Array<uint> imageSize(numImages);
	Common::Array<uint> imageFlags(numImages);
	_images.resize(numImages);

	f.skip(2);
	/*uint size =*/ f.readUint32LE();

	for (uint i = 0; i < numImages; i++) {
		imageSize[i] = (uint)f.readUint16LE();
		imageFlags[i] = (uint)f.readUint16LE();
		uint width = (uint)f.readUint16LE();
		uint height = (uint)f.readUint16LE();

		_images[i].create(width, height);
	}

	Common::SeekableReadStream *decompressed = f.decompress(compression);

	for (uint i = 0; i < numImages; i++) {
		Common::SeekableReadStream *frameSrc = decompressed->readStream(imageSize[i]);
		loadImage(_images[i], imageFlags[i], frameSrc);
		delete frameSrc;
	}

	delete decompressed;
}

void ImageResource::loadImage(Graphics::ManagedSurface &surface,
		uint flags, Common::SeekableReadStream *src) {
	if (flags & FLAG_COMPRESSED)
		src = DecompressRLE(src).decompress();

	if (flags & FLAG_XYSWAPPED) {
		// For x/y swapped, requires doing it by column and then by row
		for (int x = 0; x < surface.w; x++) {
			for (int y = 0; y < surface.h; y++) {
				byte *pixelP = (byte *)surface.getBasePtr(x, y);
				*pixelP = src->readByte();
			}
		}
	} else {
		// Normal loading, can just get it in one read
		src->read((byte *)surface.getPixels(), surface.w * surface.h);
	}

	if (flags & FLAG_COMPRESSED)
		delete src;
}


} // namespace Krondor
