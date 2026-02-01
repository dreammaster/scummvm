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

#include "ultima/shared/early/font_resources.h"
#include "ultima/shared/early/gfx/dos_font.h"
#include "common/algorithm.h"

namespace Ultima {
namespace Shared {

/*-------------------------------------------------------------------*/

FontResources::FontResources() : LocalResourceFile("COMMON/FONTS") {
}

FontResources::FontResources(Resources *resManager) : LocalResourceFile(resManager, "COMMON/FONTS") {
	Common::copy((const byte *)Gfx::INT10_FONT_08, (const byte *)Gfx::INT10_FONT_08 + 256 * 8, (byte *)_font8x8);
}

void FontResources::synchronize() {
	syncBytes2D((byte *)_font8x8, 256, 8);

	// Load in the Ultima VI font if present
	Common::File f;
	if (f.exists("u6.ch")) {
		f.open("u6.ch");
		for (int idx = 0; idx < 256; ++idx)
			f.read(&_fontU6[idx][0], 8);
	} else {
		for (int idx = 0; idx < 256; ++idx)
			Common::fill(&_fontU6[idx][0], &_fontU6[idx][8], 0);
	}
}

} // End of namespace Shared
} // End of namespace Ultima
