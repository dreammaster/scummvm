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
#include "graphics/cursorman.h"
#include "mads/madsv2/core/general.h"
#include "mads/madsv2/core/mouse_1.h"

namespace MADS {
namespace MADSV2 {

static const byte cursor_mask[16][16] = {
	/*  0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15  */
	  {255,   0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255},
	  {  0,   7,   0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255},
	  {  0,   7,   7,   0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255},
	  {  0,   7,  15,   7,   0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255},
	  {  0,   7,  15,  15,   7,   0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255},
	  {  0,   7,  15,  15,  15,   7,   0, 255, 255, 255, 255, 255, 255, 255, 255, 255},
	  {  0,   7,  15,  15,  15,  15,   7,   0, 255, 255, 255, 255, 255, 255, 255, 255},
	  {  0,   7,  15,  15,  15,  15,  15,   7,   0, 255, 255, 255, 255, 255, 255, 255},
	  {  0,   7,  15,  15,  15,  15,   7,   7,   7,   0, 255, 255, 255, 255, 255, 255},
	  {  0,   7,  15,   7,  15,  15,   0,   0,   0, 255, 255, 255, 255, 255, 255, 255},
	  {  0,   7,   7,   0,   7,  15,   7,   0, 255, 255, 255, 255, 255, 255, 255, 255},
	  {255,   0,   0, 255,   0,   7,  15,   0, 255, 255, 255, 255, 255, 255, 255, 255},
	  {255, 255, 255, 255,   0,   7,  15,   0, 255, 255, 255, 255, 255, 255, 255, 255},
	  {255, 255, 255, 255, 255,   0,   0, 255, 255, 255, 255, 255, 255, 255, 255, 255},
	  {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255},
	  {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255},
};

void mouse_show() {
	if (!CursorMan.isVisible())
		CursorMan.showMouse(true);
}

void mouse_hide() {
	CursorMan.showMouse(false);
}

void mouse_force(int x, int y) {
	g_system->warpMouse(x, y);
}

} // namespace MADSV2
} // namespace MADS



