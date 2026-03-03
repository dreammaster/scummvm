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

#ifndef MADS_CORE_MCGA_H
#define MADS_CORE_MCGA_H

#include "mads/madsv2/core/general.h"
#include "mads/madsv2/core/font.h"

namespace MADS {
namespace MADSV2 {

extern int  mcga_retrace_computed;
extern word mcga_retrace_ticks;
extern int  mcga_retrace_max_colors;
extern int  mcga_retrace_max_bytes;

extern word mcga_palette_update;
extern int  mcga_palette_fast;


/* mcga_1.c */
void    mcga_put_pixel(word x, word y, byte c);

/* mcga_2.c */
byte    mcga_get_pixel(word x, word y);

/* mcga_3.c */
byte *fastcall mcga_open_window(word x, word y,
	word xsize, word ysize);
void    mcga_close_window(byte *win);

/* mcga_7.c */
void    mcga_setpal(Palette *pal);

/* mcga_8.c */
void    mcga_getpal(Palette *pal);

/* mcga_9.c */
void    mcga_cls(byte inp);

/* mcga_b.c */
void    mcga_retrace(void);
void    mcga_setpal_range(Palette *pal,
	int first_color,
	int num_colors);

/* mcga_c.c */
void mcga_compute_retrace_parameters(void);

/* mcga_d.c */
extern word mcga_shakes;

void mcga_shake(void);

/* mcga_e.c */
void mcga_reset(void);

} // namespace MADSV2
} // namespace MADS

#endif
