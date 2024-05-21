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
#include "graphics/screen.h"
#include "krondor/gfx/palette.h"

namespace Krondor {
namespace Gfx {

void Palette::setActive() {
	g_system->getPaletteManager()->setPalette(data(), 0, size());
}

void Palette::getActive() {
	byte pal[PALETTE_SIZE];
	g_system->getPaletteManager()->grabPalette(pal, 0, PALETTE_COUNT);
	*static_cast<Graphics::Palette *>(this) = Graphics::Palette(pal, PALETTE_COUNT);
}

} // namespace Gfx
} // namespace Krondor
