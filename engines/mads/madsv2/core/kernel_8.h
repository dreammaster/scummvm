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

#ifndef MADS_CORE_KERNEL_8_H
#define MADS_CORE_KERNEL_8_H

#include "mads/madsv2/core/kernel.h"

namespace MADS {
namespace MADSV2 {


extern Sequence sequence_list[KERNEL_MAX_SEQUENCES];

extern void kernel_seq_init(void);

extern int kernel_seq_add(int series_id, int mirror,
	int initial_sprite, int low_sprite, int high_sprite, int loop_mode,
	int loop_direction, int depth, int scale, int auto_locating, int x, int y,
	word ticks, word interval_ticks, word start_ticks, int expire);

int fastcall kernel_seq_forward(int series_id, int mirror,
	word ticks, word interval_ticks, word start_ticks, int expire);

int fastcall kernel_seq_forward_scroll(int series_id, int mirror,
	word ticks, word interval_ticks, word start_ticks, int expire);

int fastcall kernel_seq_pingpong(int series_id, int mirror,
	word ticks, word interval_ticks, word start_ticks, int expire);

int fastcall kernel_seq_pingpong_scroll(int series_id, int mirror,
	word ticks, word interval_ticks, word start_ticks, int expire);

int fastcall kernel_seq_backward(int series_id, int mirror,
	word ticks, word interval_ticks, word start_ticks, int expire);

int fastcall kernel_seq_backward_scroll(int series_id, int mirror,
	word ticks, word interval_ticks, word start_ticks, int expire);

int fastcall kernel_seq_trigger(int sequence_id,
	int trigger_type, int trigger_sprite, int trigger_code);

extern void kernel_seq_loc(int sequence_id, int x, int y);
extern void kernel_seq_depth(int sequence_id, int depth);
extern void kernel_seq_scale(int sequence_id, int scale);


extern int kernel_seq_purge(int sequence_id);

extern void kernel_seq_full_update(void);

extern void kernel_synch(int slave_type, int slave_id,
	int master_type, int master_id);

extern void kernel_draw_to_background(int series_id, int sprite_id,
	int x, int y, int depth, int scale);

} // namespace MADSV2
} // namespace MADS

#endif
