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

#include "spycraft/advlib.h"
#include "spycraft/advdebug.h"
#include "spycraft/advport.h"
#include "spycraft/advscreen.h"
#include "spycraft/advback.h"
#include "spycraft/advrect.h"
#include "spycraft/advbits.h"
#include "spycraft/advscreen.h"
#include "spycraft/advtime.h"

namespace Spycraft {

#define numColors 256

/* this should be declared in advback.h */
extern Viewport *SparePort;

void DoFlip(Background *from, Background *to) {
	SRect rect;

	SRect_Init(&rect, 0, 0, scene_width - 1, scene_height - 1);
	DrawBits(to->animPort, to->backPort, &rect);

}

} // namespace Spycraft
