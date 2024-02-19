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
#include "common/textconsole.h"
#include "yendor/gfx/font.h"

namespace Yendor {

bool Font::_bgTransparent;
byte Font::_fgColor;
byte Font::_bgColor;

void Font::load(const Common::Path &filename) {
	Common::File f;
	if (!f.open(filename) || f.size() != (FONT_CHAR_COUNT * FONT_H))
		error("Could not load font");

	f.read(_data, FONT_CHAR_COUNT * FONT_H);
}

void Font::writeChar(Graphics::Surface &surf, int x, int y, char c) {
	assert(c >= 32 && c <= 127);
	const byte *srcP = &_data[(c - 32) * FONT_H];
	byte *destP = (byte *)surf.getBasePtr(x, y);
	byte v;

	for (int yp = 0; yp < FONT_H; ++yp, destP += surf.w - FONT_W) {
		v = *srcP++;

		for (int xp = 0; xp < FONT_W; ++xp, ++destP, v <<= 1) {
			if (v & 0x80)
				*destP = _fgColor;
			else if (!_bgTransparent)
				*destP = _bgColor;
		}
	}
}

void Font::writeString(Graphics::Surface &surf, int x, int y,
		const Common::String &str) {
	for (const char *strP = str.c_str(); *strP; ++strP, x += FONT_W)
		writeChar(surf, x, y, *strP);
}

} // End of namespace Yendor
