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
#include "engines/util.h"
#include "graphics/palette.h"
#include "legend/early/gfx/screen.h"
#include "legend/legend.h"

namespace Legend {
namespace Early {
namespace Gfx {

static const uint32 EGA_PALETTE[16] = {
	0xffffff, 0x0000aa, 0x00aa00, 0x00aaaa,
	0xaa0000, 0xaa00aa, 0xaa5500, 0xaaaaaa,
	0x555555, 0x5555ff, 0x55ff55, 0x55ffff,
	0xff5555, 0xff55ff, 0xffff55, 0x000000
};

const int Screen::TEXT_ROWS = 30;
const int Screen::TEXT_COLUMNS = 80;

void Screen::resetPalette() {
	byte pal[PALETTE_SIZE];

	Common::fill(&pal[0], &pal[PALETTE_SIZE], 0);
	setEGAPalette(pal);

	g_system->getPaletteManager()->setPalette(pal, 0, PALETTE_COUNT);
}

void Screen::setEGAPalette(byte *destPalette) {
	byte tempPalette[PALETTE_SIZE];

	const uint32 *srcP = EGA_PALETTE;
	byte *destP = &tempPalette[0];
	for (int idx = 0; idx < 16; ++idx, ++srcP, destP += 3) {
		destP[0] = *srcP & 0xff;
		destP[1] = (*srcP >> 8) & 0xff;
		destP[2] = (*srcP >> 16) & 0xff;
	}

	Common::copy(&tempPalette[0], &tempPalette[16 * 3], destPalette);
}

} // namespace Gfx
} // namespace Early
} // namespace Legend
