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

#ifndef AGS2_AC_ACWALKBEHIND_H
#define AGS2_AC_ACWALKBEHIND_H

#include "common/scummsys.h"

namespace AGS2 {

enum WalkBehindMethodEnum {
	DrawOverCharSprite,
	DrawAsSeparateSprite,
	DrawAsSeparateCharSprite
};

extern void update_walk_behind_images();
extern void recache_walk_behinds();
extern int get_walkable_area_pixel(int x, int y);
extern int sort_out_walk_behinds(block sprit, int xx, int yy, int basel, block copyPixelsFrom = NULL, block checkPixelsFrom = NULL, int zoom = 100);
extern void invalidate_cached_walkbehinds();
extern void sort_out_char_sprite_walk_behind(int actspsIndex, int xx, int yy, int basel, int zoom, int width, int height);

} // namespace AGS2

#endif
