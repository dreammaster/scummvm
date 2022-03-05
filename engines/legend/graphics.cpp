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
#include "graphics/palette.h"
#include "legend/graphics.h"
#include "legend/globals.h"

namespace Legend {

void set_palette_range(const byte *pal, int start, int count) {
	g_system->getPaletteManager()->setPalette(pal, start, count);
}

void set_palette(const Palette &pal) {
	set_palette_range(pal, 0, PALETTE_COUNT - 1);
}

void black_palette() {
	_G(cycling_on) = false;
	Common::fill(&_G(default_palette)[0],
		&_G(default_palette)[PALETTE_SIZE], 0);
	set_palette(_G(default_palette));
	_G(palette_off) = 1;
}


} // namespace Legend
