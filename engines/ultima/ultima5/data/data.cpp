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

#include "common/system.h"
#include "common/translation.h"
#include "graphics/paletteman.h"
#include "ultima/ultima5/data/data.h"

namespace Ultima {
namespace Ultima5 {
namespace Data {

static const byte EGA_PALETTE[16 * 3] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0xaa, 0x00, 0xaa, 0x00, 0x00, 0xaa, 0xaa,
	0xaa, 0x00, 0x00, 0xaa, 0x00, 0xaa, 0xaa, 0x55, 0x00, 0xaa, 0xaa, 0xaa,
	0x55, 0x55, 0x55, 0x55, 0x55, 0xff, 0x55, 0xff, 0x55, 0x55, 0xff, 0xff,
	0xff, 0x55, 0x55, 0xff, 0x55, 0xff, 0xff, 0xff, 0x55, 0xff, 0xff, 0xff
};

static const byte CGA_PALETTE[4 * 3] = {
	0x00, 0x00, 0x00, 0x00, 0xaa, 0xaa, 0xaa, 0x00, 0xaa, 0xaa, 0xaa, 0xaa
};

const char *const RENDER_MODE_LABELS[RENDER_MODE_COUNT] = {
	_s("Original (CGA)"), _s("EGA (16 colors)")
};

const char *pictureExtension(RenderMode mode) {
	return mode == RENDER_CGA ? ".4" : ".16";
}

void setPalette(RenderMode mode) {
	if (mode == RENDER_EGA) {
		g_system->getPaletteManager()->setPalette(EGA_PALETTE, 0, 16);
		return;
	}

	byte palette[16 * 3];
	for (int i = 0; i < 16; ++i)
		memcpy(&palette[i * 3], &CGA_PALETTE[(i & 3) * 3], 3);

	g_system->getPaletteManager()->setPalette(palette, 0, 16);
}

} // namespace Data
} // namespace Ultima5
} // namespace Ultima
