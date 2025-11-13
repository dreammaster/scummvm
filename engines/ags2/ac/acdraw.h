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

#ifndef AGS2_AC_ACDRAW_H
#define AGS2_AC_ACDRAW_H

#include "ags2/ac/acruntime.h"

namespace AGS2 {

#define MAX_SPRITES_ON_SCREEN 76
#define MAX_THINGS_TO_DRAW 125

extern void clear_draw_list();
extern void add_thing_to_draw(IDriverDependantBitmap *bmp, int x, int y, int trans, bool alphaChannel);
extern void clear_sprite_list();
extern void add_to_sprite_list(IDriverDependantBitmap *spp, int xx, int yy, int baseline, int trans, int sprNum, bool isWalkBehind);
extern void draw_sprite_compensate(int picc, int xx, int yy, int useAlpha);
extern void put_sprite_256(int xxx, int yyy, block piccy);
extern void repair_alpha_channel(block dest, block bgpic);

} // namespace AGS2

#endif
