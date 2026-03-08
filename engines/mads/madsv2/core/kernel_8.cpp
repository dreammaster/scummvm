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
#include "mads/madsv2/core/anim.h"
#include "mads/madsv2/core/matte.h"
#include "mads/madsv2/core/error.h"
#include "mads/madsv2/core/player.h"
#include "mads/madsv2/core/sprite.h"
#include "mads/madsv2/core/attr.h"

#include "mads/madsv2/core/kernel.h"
#include "mads/madsv2/core/kernel_1.h"
#include "mads/madsv2/core/kernel_f.h"
#include "mads/madsv2/core/kernel_9.h"

namespace MADS {
namespace MADSV2 {

Sequence sequence_list[KERNEL_MAX_SEQUENCES];

void kernel_seq_init(void) {
	int count;

	for (count = 0; count < KERNEL_MAX_SEQUENCES; count++) {
		sequence_list[count].active_flag = false;
		sequence_list[count].dynamic_hotspot = -1;
	}
}

int kernel_seq_add(int series_id, int mirror, int initial_sprite,
	int low_sprite, int high_sprite, int loop_mode, int loop_direction,
	int depth, int scale, int auto_locating, int x, int y,
	word ticks, word interval_ticks, word start_ticks, int expire) {
	int result = -1;
	int id = -1;
	int found = false;
	int count;

	for (count = 0; !found && (count < KERNEL_MAX_SEQUENCES); count++) {
		id = count;
		found = !sequence_list[count].active_flag;
	}

	if (!found) {
#if !defined(disable_error_check)
		error_report(ERROR_SEQUENCE_LIST_FULL, WARNING, MODULE_KERNEL, KERNEL_MAX_SEQUENCES, 0);
#endif
		goto done;
	}

	if (low_sprite <= 0) {
		low_sprite = 1;
	}

	if (high_sprite <= 0) {
		high_sprite = series_list[series_id]->num_sprites;
	}

	if (high_sprite == low_sprite) {
		loop_direction = 0;
	}

	sequence_list[id].active_flag = true;
	sequence_list[id].series_id = (byte)series_id;
	sequence_list[id].mirror = (byte)mirror;
	sequence_list[id].sprite = initial_sprite;
	sequence_list[id].start_sprite = low_sprite;
	sequence_list[id].end_sprite = high_sprite;
	sequence_list[id].loop_mode = loop_mode;
	sequence_list[id].loop_direction = loop_direction;
	sequence_list[id].depth = (byte)depth;
	sequence_list[id].scale = (byte)scale;
	sequence_list[id].auto_locating = (byte)auto_locating;
	sequence_list[id].x = x;
	sequence_list[id].y = y;
	sequence_list[id].ticks = ticks;
	sequence_list[id].interval_ticks = interval_ticks;
	sequence_list[id].base_time = kernel.clock + start_ticks;
	sequence_list[id].expire = (byte)expire;
	sequence_list[id].expired = false;

	sequence_list[id].motion = 0;
	sequence_list[id].dynamic_hotspot = -1;

	sequence_list[id].num_triggers = 0;
	sequence_list[id].trigger_dest = (byte)kernel.trigger_setup_mode;

	for (count = 0; count < 3; count++) {
		sequence_list[id].trigger_words[count] = player2.words[count];
	}

	sequence_list[id].last_image.flags = -1;

	result = id;

done:
	return result;
}

int kernel_seq_forward(int series_id, int mirror, word ticks,
	word interval_ticks, word start_ticks, int expire) {
	int depth;
	SpritePtr sprite;

	sprite = &series_list[series_id]->index[0];

	depth = attr_depth(&depth_map,
		sprite->x + (sprite->xs >> 1),
		sprite->y + (sprite->ys >> 1)) - 1;

	return kernel_seq_add(series_id, mirror, 1, 0, 0, AA_LINEAR, 1,
		depth, 100, true, 0, 0, ticks, interval_ticks,
		start_ticks, expire);
}

int kernel_seq_forward_scroll(int series_id, int mirror, word ticks,
		word interval_ticks, word start_ticks, int expire) {
	// TODO: depth isn't initialized?
	int depth = 0;
	SpritePtr sprite;

	sprite = &series_list[series_id]->index[0];

	return kernel_seq_add(series_id, mirror, 1, 0, 0, AA_LINEAR, 1,
		depth, 100, true, 0, 0, ticks, interval_ticks,
		start_ticks, expire);
}

int kernel_seq_pingpong(int series_id, int mirror, word ticks,
		word interval_ticks, word start_ticks, int expire) {
	int depth;
	SpritePtr sprite;

	sprite = &series_list[series_id]->index[0];

	depth = attr_depth(&depth_map,
		sprite->x + (sprite->xs >> 1),
		sprite->y + (sprite->ys >> 1)) - 1;

	return kernel_seq_add(series_id, mirror, 1, 0, 0, AA_PINGPONG, 1,
		depth, 100, true, 0, 0, ticks, interval_ticks,
		start_ticks, expire);
}


int kernel_seq_pingpong_scroll(int series_id, int mirror, word ticks,
		word interval_ticks, word start_ticks, int expire) {
	// TODO: depth isn't initialized?
	int depth = 0;
	SpritePtr sprite;

	sprite = &series_list[series_id]->index[0];

	return kernel_seq_add(series_id, mirror, 1, 0, 0, AA_PINGPONG, 1,
		depth, 100, true, 0, 0, ticks, interval_ticks,
		start_ticks, expire);
}

int kernel_seq_backward(int series_id, int mirror, word ticks,
		word interval_ticks, word start_ticks, int expire) {
	int depth;
	SpritePtr sprite;

	sprite = &series_list[series_id]->index[0];

	depth = attr_depth(&depth_map,
		sprite->x + (sprite->xs >> 1),
		sprite->y + (sprite->ys >> 1)) - 1;

	return kernel_seq_add(series_id, mirror,
		series_list[series_id]->num_sprites,
		0, 0, AA_LINEAR, -1, depth, 100, true, 0, 0,
		ticks, interval_ticks, start_ticks, expire);
}

int kernel_seq_backward_scroll(int series_id, int mirror,
		word ticks, word interval_ticks, word start_ticks, int expire) {
	// TODO: depth isn't initialized?
	int depth = 0;
	SpritePtr sprite;

	sprite = &series_list[series_id]->index[0];

	return kernel_seq_add(series_id, mirror,
		series_list[series_id]->num_sprites,
		0, 0, AA_LINEAR, -1, depth, 100, true, 0, 0,
		ticks, interval_ticks, start_ticks, expire);
}

void kernel_synch(int slave_type, int slave_id, int master_type, int master_id) {
	long master_time;

	switch (master_type) {
	case KERNEL_SERIES:
		master_time = sequence_list[master_id].base_time;
		break;

	case KERNEL_ANIM:
		master_time = kernel_anim[master_id].next_clock;
		break;

	case KERNEL_NOW:
		master_time = kernel.clock + (long)master_id;
		break;

	case KERNEL_PLAYER:
	default:
		master_time = player.clock;
		break;
	}


	switch (slave_type) {
	case KERNEL_SERIES:
		sequence_list[slave_id].base_time = master_time;
		break;

	case KERNEL_PLAYER:
		player.clock = master_time;
		break;

	case KERNEL_ANIM:
		kernel_anim[slave_id].next_clock = master_time;
		break;
	}
}

void kernel_player_expire(int sequence_id) {
	sequence_list[sequence_id].expired = true;
	sequence_list[sequence_id].base_time = player.clock;
}

void kernel_seq_depth(int sequence_id, int depth) {
	sequence_list[sequence_id].depth = (byte)depth;
}

void kernel_seq_scale(int sequence_id, int scale) {
	sequence_list[sequence_id].scale = (byte)scale;
}

void kernel_seq_loc(int sequence_id, int x, int y) {
	sequence_list[sequence_id].x = x;
	sequence_list[sequence_id].y = y;
	sequence_list[sequence_id].auto_locating = false;
}

void kernel_seq_motion(int sequence_id, int flags,
	int delta_x_times_100, int delta_y_times_100) {
	sequence_list[sequence_id].motion = (byte)(KERNEL_MOTION | flags);
	sequence_list[sequence_id].sign_x = sgn(delta_x_times_100);
	sequence_list[sequence_id].sign_y = sgn(delta_y_times_100);
	sequence_list[sequence_id].delta_x = abs(delta_x_times_100);
	sequence_list[sequence_id].delta_y = abs(delta_y_times_100);
	sequence_list[sequence_id].accum_x = 0;
	sequence_list[sequence_id].accum_y = 0;
}

void kernel_seq_range(int sequence_id, int first, int last) {
	int num_sprites;
	int from, unto;
	SequencePtr sequence;

	sequence = &sequence_list[sequence_id];

	num_sprites = series_list[sequence->series_id]->num_sprites;

	switch (first) {
	case KERNEL_FIRST:
	case KERNEL_DEFAULT:
		from = 1;
		break;
	case KERNEL_LAST:
		from = num_sprites;
		break;
	default:
		from = first;
		break;
	}

	switch (last) {
	case KERNEL_FIRST:
		unto = 1;
		break;
	case KERNEL_LAST:
	case KERNEL_DEFAULT:
		unto = num_sprites;
		break;
	default:
		unto = last;
		break;
	}

	sequence->start_sprite = from;
	sequence->end_sprite = unto;

	sequence->sprite = (sequence->loop_direction >= 0) ? from : unto;
}

int kernel_seq_stamp(int series_id, int mirror, int sprite) {
	int id;

	id = kernel_seq_forward(series_id, mirror, 32767, 0, 0, 0);
	if (id >= 0) {
		kernel_seq_range(id, sprite, sprite);
		sequence_list[id].loop_direction = AA_STAMP;
	}
	return id;
}

int kernel_seq_stamp_scroll(int series_id, int mirror, int sprite) {
	int id;

	id = kernel_seq_forward_scroll(series_id, mirror, 32767, 0, 0, 0);
	if (id >= 0) {
		kernel_seq_range(id, sprite, sprite);
		sequence_list[id].loop_direction = AA_STAMP;
	}
	return id;
}

int kernel_seq_trigger(int sequence_id, int trigger_type,
	int trigger_sprite, int trigger_code) {
	int error_code = true;
	int id;
	SequencePtr sequence;

	sequence = &sequence_list[sequence_id];

	if (sequence->num_triggers >= KERNEL_MAX_TRIGGERS) goto done;

	id = sequence->num_triggers++;

	sequence->trigger_type[id] = (byte)trigger_type;
	sequence->trigger_sprite[id] = trigger_sprite;
	sequence->trigger_code[id] = (byte)trigger_code;

	error_code = false;

done:
	return error_code;
}

int kernel_timing_trigger(int ticks, int trigger_code) {
	int id = -1;
	int found = false;
	int count;

	for (count = 0; !found && (count < KERNEL_MAX_SEQUENCES); count++) {
		id = count;
		found = !sequence_list[count].active_flag;
	}

	if (!found) {
#if !defined(disable_error_check)
		error_report(ERROR_SEQUENCE_LIST_FULL, WARNING, MODULE_KERNEL, KERNEL_MAX_SEQUENCES, 0);
#endif
		goto done;
	}

	sequence_list[id].active_flag = true;
	sequence_list[id].series_id = KERNEL_SPECIAL_TIMING;

	sequence_list[id].ticks = ticks;
	sequence_list[id].interval_ticks = 0;
	sequence_list[id].base_time = kernel.clock + ticks;
	sequence_list[id].expire = 1;
	sequence_list[id].expired = false;

	sequence_list[id].num_triggers = 0;
	sequence_list[id].trigger_dest = (byte)kernel.trigger_setup_mode;

	for (count = 0; count < 3; count++) {
		sequence_list[id].trigger_words[count] = player2.words[count];
	}

	kernel_seq_trigger(id, KERNEL_TRIGGER_EXPIRE, 0, trigger_code);

done:
	return id;
}

int kernel_seq_purge(int sequence_id) {
	int count;
	int purged_any = -1;

	for (count = 0; count < (int)image_marker; count++) {
		if (image_list[count].segment_id == (byte)sequence_id) {
			image_list[count].flags = IMAGE_ERASE;
			purged_any = count;
		}
	}

	return purged_any;
}

void kernel_seq_full_update(void) {
	int count, id;

	for (count = 0; count < KERNEL_MAX_SEQUENCES; count++) {
		if (sequence_list[count].active_flag) {
			if ((int)(sequence_list[count].series_id) != KERNEL_SPECIAL_TIMING) {
				id = matte_allocate_image();
				if (id >= 0) {
					if (sequence_list[count].last_image.flags >= 0) {
						image_list[id] = sequence_list[count].last_image;
					} else {
						kernel_seq_image(&sequence_list[count], &image_list[id], count);
					}
				}
			}
		}
	}
}

void kernel_seq_correction(long old_clock, long new_clock) {
	int count;
	SequencePtr sequence;

	for (count = 0; count < KERNEL_MAX_SEQUENCES; count++) {
		sequence = &sequence_list[count];
		if (sequence->active_flag) {
			sequence->base_time += (new_clock - old_clock);
		}
	}
}

void kernel_draw_to_background(int series_id, int sprite_id,
	int x, int y, int depth, int scale) {
	if (x == KERNEL_HOME) {
		x = series_list[series_id]->index[sprite_id - 1].x;
	}

	if (y == KERNEL_HOME) {
		y = series_list[series_id]->index[sprite_id - 1].y;
	}

	sprite_draw_3d_scaled_big(series_list[series_id],
		sprite_id, &scr_orig, &scr_depth,
		x - picture_map.pan_base_x,
		y - picture_map.pan_base_y,
		depth, scale, 0, 0);

	matte_refresh_work();
}

} // namespace MADSV2
} // namespace MADS
