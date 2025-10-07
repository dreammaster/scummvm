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

#include "spycraft/dmade/advlib.h"
#include "spycraft/dmade/advdebug.h"
#include "spycraft/dmade/advport.h"
#include "spycraft/dmade/advscreen.h"
#include "spycraft/dmade/advback.h"
#include "spycraft/dmade/advrect.h"
#include "spycraft/dmade/advbits.h"
#include "spycraft/dmade/advscreen.h"
#include "spycraft/dmade/advtime.h"

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
