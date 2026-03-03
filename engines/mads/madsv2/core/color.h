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

#ifndef MADS_CORE_COLOR_H
#define MADS_CORE_COLOR_H

#include "mads/madsv2/core/general.h"

namespace MADS {
namespace MADSV2 {


#define COLOR_MAX_USER_COLORS           256
#define COLOR_FIRST_USER_COLOR          16

#define COLOR_MAX_SHADOW_COLORS         3

#define COLOR_MAX_CYCLES                8

#define COLOR_HIGHEST                   254
#define COLOR_TRANSPARENT               255

#define COLOR_SOLID                     0
#define COLOR_BLACK_THATCH              0x0d
#define COLOR_NO_X16                    0x0ff

#define COLOR_ERR_LISTOVERFLOW          -1

#define COLOR_GROUP_MAP_TO_CYCLE        0x80  /* Map to cycling colors  */
#define COLOR_GROUP_MAP_TO_CLOSEST      0x40  /* Map to closest color   */
#define COLOR_GROUP_FORCE_TO_CLOSEST    0x20  /* Force to closest color */
#define COLOR_GROUP_MAP_TO_SHADOW       0x10  /* Map to shadow color    */
#define COLOR_GROUP_FORCE_TO_SKIP       0x08  /* Force to skip code     */

#define COLOR_GROUP_SPECIAL_2           0x02  /* Special color group 2  */
#define COLOR_GROUP_SPECIAL_1           0x01  /* Special color group 1  */

#define COLOR_DP(x) (((((x + 1) * 101) + 26) / 64) - 1)


/* A fully parameterized color (in contrast to a plain RGBcolor as */
/* defined in general.mac)                                         */

typedef struct {
	byte r, g, b;         /* RGB values                  */
	byte x16;           /* 16 color dither translation */
	byte cycle;         /* Color cycling handle        */
	byte group;         /* Color grouping flags        */
} Color;

typedef Color *ColorPtr;


/* Palette-independent color list; ready to map into palette */

typedef struct {
	int   num_colors;
	Color table[COLOR_MAX_USER_COLORS];
} ColorList;

typedef ColorList *ColorListPtr;


/* Palette-independent color cycling range & timing information */

typedef struct {
	byte num_colors;                    /* Number of colors in the cycle */
	byte first_list_color;              /* First color in color list     */
	byte first_palette_color;           /* First color in final palette  */
	byte ticks;                         /* 60/s ticks between cycles     */
} Cycle;

typedef Cycle *CyclePtr;


/* List of color cycling ranges */

typedef struct {
	int   num_cycles;
	Cycle table[COLOR_MAX_CYCLES];
} CycleList;

typedef CycleList *CycleListPtr;


typedef struct {
	int   num_shadow_colors;
	int   shadow_color[COLOR_MAX_SHADOW_COLORS];
} ShadowList;

typedef ShadowList *ShadowListPtr;


/* color_1.c */
byte color_thatch(int color, int thatching);

void color_list_start_scan(byte *list_flags);

int  color_list_update(ColorListPtr list, Buffer *scan_buf,
	Palette *scan_pal, byte *list_flags,
	byte *palette_map, CycleListPtr cycle);

void color_list_purge(ColorListPtr list, byte *list_flags);

int  color_list_palette(ColorListPtr list, Buffer *scan_buf,
	Palette *scan_pal, int base_color,
	byte *palette_map, CycleListPtr cycle);

void color_list_conform(ColorListPtr list, Buffer *scan_buf,
	Palette *scan_pal, Palette *main_pal,
	int base_color);

void color_trans_fill_buf(Buffer unto,
	int unto_x, int unto_y,
	int size_x, int size_y,
	byte thatch_color);

void color_trans_show_buf(Buffer from, Buffer unto,
	int from_x, int from_y,
	int unto_x, int unto_y,
	int size_x, int size_y,
	ColorListPtr list,
	byte *palette_map, int mask_flag);

void color_buffer_palette_to_list(ColorListPtr list,
	Buffer *scan_buf,
	byte *palette_map);

void color_buffer_list_to_palette(Buffer *scan_buf, int marker);

void color_transparent_swap(Buffer *scan_buf,
	Palette *scan_pal,
	byte transparent);


/* color_2.c */
void color_buffer_list_to_main(ColorListPtr color_list,
	Buffer *scan_buf);

/* color_3.c */
void color_buffer_list_to_x16(ColorListPtr color_list,
	Buffer *scan_buf);

void color_split_thatch(int thatch, int *color, int *thatching);

} // namespace MADSV2
} // namespace MADS

#endif
