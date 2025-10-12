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
#include "graphics/paletteman.h"
#include "wizardry/gfx/palette.h"

namespace Wizardry {

// Apple II lo-res / text mode palette (approximate RGB)
static const byte APPLE2_PALETTE[16][3] = {
	{ 0x00, 0x00, 0x00 }, // 0: Black
	{ 0xFF, 0xFF, 0xFF }, // 1: White
	{ 0xFF, 0x00, 0x00 }, // 2: Red
	{ 0x00, 0xFF, 0xFF }, // 3: Cyan
	{ 0xFF, 0x00, 0xFF }, // 4: Purple
	{ 0x00, 0xFF, 0x00 }, // 5: Green
	{ 0x00, 0x00, 0xFF }, // 6: Blue
	{ 0xFF, 0xFF, 0x00 }, // 7: Yellow
	{ 0xC0, 0xC0, 0xC0 }, // 8: Light gray (alternate intensity)
	{ 0x80, 0x80, 0x80 }, // 9: Dark gray
	{ 0x80, 0x00, 0x00 }, // 10: Dark red
	{ 0x00, 0x80, 0x80 }, // 11: Dark cyan
	{ 0x80, 0x00, 0x80 }, // 12: Dark purple
	{ 0x00, 0x80, 0x00 }, // 13: Dark green
	{ 0x00, 0x00, 0x80 }, // 14: Dark blue
	{ 0x80, 0x80, 0x00 }  // 15: Olive / dark yellow
};

void Gfx::setupPalette() {
	g_system->getPaletteManager()->setPalette(&APPLE2_PALETTE[0][0], 0, 16);
}

} // namespace Wizardry
