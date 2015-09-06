/* RogueVM - RPG Engines
 *
 * RogueVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef AESOP_GIL2VFX_H
#define AESOP_GIL2VFX_H

#include "aesop/defs.h"
#include "aesop/stubs.h"
#include "aesop/screen.h"

namespace Aesop {

LONG GIL2VFX_get_bitmap_width(void *shape_table, LONG shape_num);
LONG GIL2VFX_get_bitmap_height(void *shape_table, LONG shape_num);
LONG GIL2VFX_visible_bitmap_rect(LONG x1, LONG y1, LONG mirror,
                                 UBYTE *shapes, LONG shape_num, WORD *bounds);

void GIL2VFX_cout(LONG c);

} // End of namespace Aesop

#endif

