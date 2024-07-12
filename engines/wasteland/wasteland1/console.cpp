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

#include "wasteland/wasteland1/console.h"
#include "wasteland/wasteland1/wasteland1.h"

namespace Wasteland {
namespace Wasteland1 {

Console::Console() : Wasteland::Console() {
	registerCmd("font", WRAP_METHOD(Console, cmdFont));
}

bool Console::cmdFont(int argc, const char **argv) {
	auto *font = g_engine->_fonts[1];
	Graphics::Screen *screen = g_engine->getScreen();

	screen->clear();
	for (uint i = 0; i < 172; ++i) {
		font->drawChar(screen, i, (i % 16) * 16, (i / 16) * 16, 255);
	}

	screen->update();
	return false;
}

} // namespace Wasteland1
} // namespace Wasteland
