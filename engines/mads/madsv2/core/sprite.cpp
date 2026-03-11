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

#include "mads/madsv2/core/general.h"
#include "mads/madsv2/core/mem.h"
#include "mads/madsv2/core/error.h"
#include "mads/madsv2/core/pal.h"
#include "mads/madsv2/core/env.h"
#include "mads/madsv2/core/loader.h"
#include "mads/madsv2/core/sprite.h"
#include "mads/madsv2/core/sprite_h.h"

namespace MADS {
namespace MADSV2 {

// Disable Visual studio unreferenced local variables warnings
#pragma warning(push)
#pragma warning(disable: 4101)
#pragma warning(disable: 4102)
#pragma warning(disable: 4189)

//=== sprite_draw =====================================
#define three_d         false           /* depth coding  OFF */
#define bresenham       false           /* bresenham     OFF */
#define packed_attr     false           /* Attr packing  OFF */
#define translate       false           /* 16-color      OFF */
#define interface       false           /* Interface     OFF */


void sprite_draw(SeriesPtr series, int id, Buffer *buf, int target_x, int target_y) {
	#include "mads/madsv2/core/sprite_0.cpp"
}

#undef three_d    
#undef bresenham
#undef packed_attr
#undef translate
#undef interface

//=== sprite_draw_scaled =====================================
#define three_d         false           /* depth coding  OFF */
#define bresenham       true            /* bresenham     ON  */
#define packed_attr     false           /* Attr packing  OFF */
#define translate       false           /* 16-color      OFF */

#define interface       false           /* Interface     OFF */

void fastcall sprite_draw_scaled(SeriesPtr series, int id, Buffer *buf,
	int target_x, int target_y, int scale_factor) {
#include "mads/madsv2/core/sprite_0.cpp"
}

#undef three_d    
#undef bresenham
#undef packed_attr
#undef translate
#undef interface

//=== sprite_draw_3d_big =====================================
#define three_d         true            /* depth coding  ON  */
#define bresenham       false           /* bresenham     OFF */
#define packed_attr     true            /* Attr packing  ON  */
#define translate       false           /* 16-color      OFF */

#define interface       false           /* Interface     OFF */

void sprite_draw_3d_big(SeriesPtr series, int id, Buffer *buf, Buffer *attr,
		int target_x, int target_y, int target_depth, int view_port_x, int view_port_y) {
#include "mads/madsv2/core/sprite_0.cpp"
}

#undef three_d    
#undef bresenham
#undef packed_attr
#undef translate
#undef interface

//=== sprite_draw_3d_scaled_big =====================================
#define three_d         true            /* depth coding  ON  */
#define bresenham       true            /* bresenham     ON  */
#define packed_attr     true            /* Attr packing  ON  */
#define translate       false           /* 16-color      OFF */

#define interface       false           /* Interface     OFF */

void sprite_draw_3d_scaled_big(SeriesPtr series, int id, Buffer *buf, Buffer *attr,
		int target_x, int target_y, int target_depth, int scale_factor,
		int view_port_x, int view_port_y) {
#include "mads/madsv2/core/sprite_0.cpp"
}

#undef three_d    
#undef bresenham
#undef packed_attr
#undef translate
#undef interface

//=== sprite_draw_3d_x16 =====================================
#define three_d         true            /* depth coding  ON  */
#define bresenham       false           /* bresenham     OFF */
#define packed_attr     false           /* Attr packing  OFF */
#define translate       true            /* 16-color      ON  */

#define interface       false           /* Interface     OFF */

void fastcall sprite_draw_3d_x16(SeriesPtr series, int id, Buffer *buf, Buffer *attr,
	int target_x, int target_y, int target_depth) {
#include "mads/madsv2/core/sprite_0.cpp"
}

#undef three_d    
#undef bresenham
#undef packed_attr
#undef translate
#undef interface

//=== sprite_draw_3d_scaled_x16 =====================================
#define three_d         true            /* depth coding  ON  */
#define bresenham       true            /* bresenham     ON  */
#define packed_attr     false           /* Attr packing  OFF */
#define translate       true            /* 16-color      ON  */

#define interface       false           /* Interface     OFF */

void fastcall sprite_draw_3d_scaled_x16(SeriesPtr series, int id,
	Buffer *buf, Buffer *attr,
	int target_x, int target_y, int target_depth,
	int scale_factor) {
#include "mads/madsv2/core/sprite_0.cpp"
}

#undef three_d    
#undef bresenham
#undef packed_attr
#undef translate
#undef interface

//=== sprite_draw_3d_big_x16 =====================================
#define three_d         true            /* depth coding  ON  */
#define bresenham       false           /* bresenham     OFF */
#define packed_attr     true            /* Attr packing  ON  */
#define translate       true            /* 16-color      ON  */

#define interface       false           /* Interface     OFF */

void fastcall sprite_draw_3d_big_x16(SeriesPtr series, int id, Buffer *buf, Buffer *attr,
	int target_x, int target_y, int target_depth,
	int view_port_x, int view_port_y) {
#include "mads/madsv2/core/sprite_0.cpp"
}

#undef three_d    
#undef bresenham
#undef packed_attr
#undef translate
#undef interface

//=== sprite_draw_3d_big_x16 =====================================
#define three_d         true            /* depth coding  ON  */
#define bresenham       true            /* bresenham     ON  */
#define packed_attr     true            /* Attr packing  ON  */
#define translate       true            /* 16-color      ON  */

#define interface       false           /* Interface     OFF */

void fastcall sprite_draw_3d_big_x16(SeriesPtr series, int id,
	Buffer *buf, Buffer *attr, int target_x, int target_y, int target_depth,
	int scale_factor, int view_port_x, int view_port_y) {
#include "mads/madsv2/core/sprite_0.cpp"
}

#undef three_d    
#undef bresenham
#undef packed_attr
#undef translate
#undef interface

//=== sprite_draw_x16 =====================================
#define three_d         false           /* depth coding  OFF */
#define bresenham       false           /* bresenham     OFF */
#define packed_attr     false           /* Attr packing  OFF */
#define translate       true            /* 16-color      ON  */

#define interface       false           /* Interface     OFF */

void fastcall sprite_draw_x16(SeriesPtr series, int id, Buffer * buf,
		int target_x, int target_y) {
#include "mads/madsv2/core/sprite_0.cpp"
}

#undef three_d    
#undef bresenham
#undef packed_attr
#undef translate
#undef interface

//=== sprite_draw_interface =====================================
#define three_d         false           /* depth coding  OFF */
#define bresenham       false           /* bresenham     OFF */
#define packed_attr     false           /* Attr packing  OFF */
#define translate       false           /* 16-color      OFF */

#define interface       true            /* Interface     ON  */

void fastcall sprite_draw_interface(SeriesPtr series, int id, Buffer *buf,
	int target_x, int target_y) {
#include "mads/madsv2/core/sprite_0.cpp"
}

#undef three_d    
#undef bresenham
#undef packed_attr
#undef translate
#undef interface

//=== sprite_draw_3d_scaled_to_attr =====================================
#define three_d         true            /* depth coding  ON  */
#define bresenham       true            /* bresenham     ON  */
#define packed_attr     true            /* Attr packing  ON  */
#define translate       false           /* 16-color      OFF */

#define interface       false           /* Interface     OFF */

#define attribute       true            /* Draw to attribute */

void fastcall sprite_draw_3d_scaled_to_attr
(SeriesPtr series, int id,
	Buffer *buf, Buffer *attr,
	int target_x, int target_y, int target_depth,
	int scale_factor, int view_port_x, int view_port_y) {
#include "mads/madsv2/core/sprite_0.cpp"
}

#undef three_d    
#undef bresenham
#undef packed_attr
#undef translate
#undef interface
#undef attribute

//=== sprite_draw_3d_scaled_mono =====================================
#define three_d         true            /* depth coding  ON  */
#define bresenham       true            /* bresenham     ON  */
#define packed_attr     false           /* Attr packing  OFF */
#define translate       false           /* 16-color      OFF */

#define interface       false           /* Interface     OFF */

#define monodraw        true            /* Draw mono color   */

void fastcall sprite_draw_3d_scaled_mono(SeriesPtr series, int id,
	Buffer *buf, Buffer *attr, int target_x, int target_y, int target_depth,
	int scale_factor, byte color) {
#include "mads/madsv2/core/sprite_0.cpp"
}

#undef three_d    
#undef bresenham
#undef packed_attr
#undef translate
#undef interface

#pragma warning(pop)

//====================================================================




} // namespace MADSV2
} // namespace MADS
