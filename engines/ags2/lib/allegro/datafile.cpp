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

#include "common/file.h"
#include "common/system.h"
#include "common/savefile.h"
#include "image/bmp.h"
#include "image/iff.h"
#include "image/pcx.h"
#include "image/tga.h"
#include "ags2/lib/allegro/datafile.h"
#include "ags2/lib/allegro/surface.h"

namespace AGS2 {

#define VGA_COLOR_TRANS(x) ((x) * 255 / 63)

template<class DECODER>
BITMAP *decodeImageStream(Common::SeekableReadStream &stream, color *pal) {
	DECODER decoder;

	if (decoder.loadStream(stream)) {
		// Create the output surface
		const Graphics::Surface *src = decoder.getSurface();

		// Copy the decoded surface
		int bpp = 8 * src->format.bytesPerPixel;
		if (bpp == 24)
			bpp = 32;
		BITMAP *dest = create_bitmap_ex(bpp, src->w, src->h);
		dest->getSurface().blitFrom(*src);

		// Copy the palette
		const Graphics::Palette &palP = decoder.getPalette();
		if (pal) {
			for (uint idx = 0; idx < palP.size(); ++idx) {
				palP.get(idx, pal[idx].r, pal[idx].g, pal[idx].b);
				pal[idx].filler = 0xff;
			}
		}

		return dest;
	} else {
		return nullptr;
	}
}

template<class DECODER>
BITMAP *decodeImage(const char *filename, color *pal) {
	Common::File f;
	if (!f.open(filename))
		return nullptr;

	return decodeImageStream<DECODER>(f, pal);
}

BITMAP *load_bmp(const char *filename, color *pal) {
	return decodeImage<Image::BitmapDecoder>(filename, pal);
}

BITMAP *load_lbm(const char *filename, color *pal) {
	return decodeImage<Image::IFFDecoder>(filename, pal);
}

BITMAP *load_pcx(const char *filename, color *pal) {
	return decodeImage<Image::PCXDecoder>(filename, pal);
}

BITMAP *load_tga(const char *filename, color *pal) {
	return decodeImage<Image::TGADecoder>(filename, pal);
}

BITMAP *load_bitmap(const char *filename, color *pal) {
	Common::String fname(filename);

	if (fname.hasSuffixIgnoreCase(".bmp"))
		return load_bmp(filename, pal);
	else if (fname.hasSuffixIgnoreCase(".lbm"))
		return load_lbm(filename, pal);
	else if (fname.hasSuffixIgnoreCase(".pcx"))
		return load_pcx(filename, pal);
	else if (fname.hasSuffixIgnoreCase(".tga"))
		return load_tga(filename, pal);
	else
		error("Unknown image file - %s", filename);
}

bool save_bitmap(Common::WriteStream &out, BITMAP *bmp, const RGB *pal) {
	const Graphics::ManagedSurface &src = bmp->getSurface();
	if (bmp->format.isCLUT8() && pal) {
		byte palette[256 * 3];
		for (int c = 0, i = 0; c < 256; ++c, i += 3) {
			palette[i] = VGA_COLOR_TRANS(pal[c].r);
			palette[i + 1] = VGA_COLOR_TRANS(pal[c].g);
			palette[i + 2] = VGA_COLOR_TRANS(pal[c].b);
		}

		return Image::writeBMP(out, src, palette);
	} else {
		return Image::writeBMP(out, src);
	}
}

int save_bitmap(const char *filename, BITMAP *bmp, const RGB *pal) {
	Common::OutSaveFile *sf = g_system->getSavefileManager()->openForSaving(filename, false);
	if (!sf)
		return -1;

	save_bitmap(*sf, bmp, pal);
	return 0;
}

} // namespace AGS2
