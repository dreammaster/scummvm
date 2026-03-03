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

#ifndef MADS_CORE_MAGIC_H
#define MADS_CORE_MAGIC_H

#include "common/scummsys.h"

namespace MADS {
namespace MADSV2 {

#define MAGIC_LOWER_LEFT      1
#define MAGIC_LOWER_RIGHT     2
#define MAGIC_UPPER_LEFT      3
#define MAGIC_UPPER_RIGHT     4

#define MAGIC_LEFT            0
#define MAGIC_RIGHT           1

#define MAGIC_IMMEDIATE       0
#define MAGIC_THRU_BLACK      1
#define MAGIC_SECRET_FUJI     2

#define MAGIC_SWAP_BACKGROUND 0
#define MAGIC_SWAP_FOREGROUND 1

typedef struct {
	byte intensity;
	byte map_color;
	word accum[3];
} MagicGrey;

typedef MagicGrey *MagicGreyPtr;

extern byte magic_color_flags[3];
extern byte magic_color_values[3];


/* magic_1.c */
void magic_get_grey_values(Palette *pal, byte *grey_value,
	int base_color, int num_colors);

/* magic_2.c */
void magic_grey_palette(Palette pal);


/* magic_3.c */
void magic_grey_popularity(byte *grey_list,
	byte *grey_table,
	int num_colors);
void magic_set_color_flags(byte r, byte g, byte b);
void magic_set_color_values(byte r, byte g, byte b);
void magic_map_to_grey_ramp(Palette *pal,
	int base_color, int num_colors,
	int base_grey, int num_greys,
	MagicGreyPtr magic_map);

/* magic_4.c */
void magic_grey_ramp_palette(Palette pal, int num_greys);


/* magic_5.c */
void magic_fade_to_grey(Palette pal, byte *map_pointer,
	int base_color, int num_colors,
	int base_grey, int num_greys,
	int tick_delay, int steps);
/* magic_6.c */
void magic_fade_from_grey(RGBcolor *pal, Palette target,
	int base_color, int num_colors,
	int base_grey, int num_greys,
	int tick_delay, int steps);

/* magic_7.c */
void magic_screen_change_corner(Buffer *new_screen, Palette pal,
	int corner_id,
	int buffer_base_x, int buffer_base_y,
	int screen_base_x, int screen_base_y,
	int thru_black, int set_palette,
	int tick_delay);

/* magic_8.c */
void magic_screen_change_edge(Buffer *new_screen, Palette pal,
	int edge_id,
	int buffer_base_x, int buffer_base_y,
	int screen_base_x, int screen_base_y,
	int thru_black, int set_palette,
	int tick_delay);

/* magic_9.c */

extern int magic_special_center_x;
extern int magic_special_center_y;

void magic_screen_change_circle(Buffer *new_screen, Palette pal,
	int inward_flag,
	int buffer_base_x, int buffer_base_y,
	int screen_base_x, int screen_base_y,
	int thru_black, int set_palette,
	int tick_delay, int pixel_rate);


/* magic_a.c */
void magic_shrink_buffer(Buffer *from, Buffer *unto);


/* magic_b.c */
int  magic_shrinking_buffer(Buffer *source, Buffer *rear,
	int grow_flag,
	int buffer_base_x, int buffer_base_y,
	int screen_base_x, int screen_base_y,
	int tick_delay);

/* magic_c.c */
extern int magic_low_fade_bound;
extern int magic_high_fade_bound;

void magic_swap_me_in_the_dark_baby(byte *swap,
	Palette pal,
	int start);
void magic_swap_foreground(byte *background_table,
	Palette background_palette);

/* magic_d.c */
int  magic_closest_color(RGBcolor *match_color,
	byte *list,
	int list_wrap,
	int list_length);

/* magic_e.c */
int magic_hash_color(RGBcolor *hash_color);

} // namespace MADSV2
} // namespace MADS

#endif
