/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "common/file.h"
#include "common/system.h"
#include "engines/util.h"
#include "graphics/palette.h"
#include "legend/early/gfx/screen.h"
#include "legend/legend.h"

namespace Legend {
namespace Early {
namespace Gfx {

static const byte INITIAL_LOW_PALETTE[16 * 3] = {
	0xff, 0xff, 0xff, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55, 0xaa, 0x55,
	0x55, 0xaa, 0x55, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0xaa, 0x55, 0xaa,
	0xaa, 0x55, 0xaa, 0x55, 0x55, 0xff, 0x55, 0xff, 0x55, 0x55, 0xff, 0xff
};

const int Screen::TEXT_ROWS = 30;
const int Screen::TEXT_COLUMNS = 80;

void Screen::setPalette() {
	byte pal[PALETTE_SIZE];

	Common::fill(&pal[0], &pal[PALETTE_SIZE], 0);
	setEGAPalette(pal);

	g_system->getPaletteManager()->setPalette(pal, 0, PALETTE_COUNT);
}

void Screen::setEGAPalette(byte *destPalette) {
	byte tempPalette[PALETTE_SIZE];

	const byte *srcP = INITIAL_LOW_PALETTE;
	byte *destP = &tempPalette[0];
	for (int idx = 0; idx < PALETTE_COUNT; ++idx, ++srcP, destP += 3) {
		destP[0] = (*srcP >> 2 & 1) * 0xaa + (*srcP >> 5 & 1) * 0x55;
		destP[1] = (*srcP >> 1 & 1) * 0xaa + (*srcP >> 4 & 1) * 0x55;
		destP[2] = (*srcP & 1) * 0xaa + (*srcP >> 3 & 1) * 0x55;
	}

	Common::copy(&tempPalette[0], &tempPalette[16 * 3], destPalette);
}

} // namespace Gfx
} // namespace Early
} // namespace Legend
