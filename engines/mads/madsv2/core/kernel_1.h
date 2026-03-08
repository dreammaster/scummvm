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

#ifndef MADS_CORE_KERNEL_1_H
#define MADS_CORE_KERNEL_1_H

#include "mads/madsv2/core/general.h"
#include "mads/madsv2/core/room.h"
#include "mads/madsv2/core/color.h"

namespace MADS {
namespace MADSV2 {

extern RoomPtr room;
extern int room_id;
extern int section_id;
extern int room_variant;

extern int new_room;
extern int new_section;

extern int previous_room;
extern int previous_section;

extern int kernel_initial_variant;

extern HotPtr room_spots;
extern int room_num_spots;

extern int kernel_room_series_marker;

extern int kernel_room_bound_dif;
extern int kernel_room_scale_dif;

extern int kernel_allow_peel;   /* Flag if peeling buffers allowed */

extern int kernel_panning_speed;
extern int kernel_screen_fade;


extern Animation kernel_anim[KERNEL_MAX_ANIMATIONS];

/*
extern AnimPtr kernel_animation;
extern int     kernel_animation_cycled;
extern int     kernel_repeat_animation;

extern int     kernel_animation_sprite_loaded;
extern int     kernel_animation_buffer_id;
extern byte *kernel_animation_buffer[2];

extern int     kernel_animation_messages;

extern int     kernel_animation_frame;
extern int     kernel_animation_image;
extern int     kernel_animation_doomed;
extern int     kernel_animation_trigger_code;
extern int     kernel_animation_trigger_mode;
extern int     kernel_animation_trigger_words[3];
extern long    kernel_animation_next_clock;
*/

extern ShadowList kernel_shadow_main;
extern ShadowList kernel_shadow_inter;

extern int kernel_ok_to_fail_load;

extern int kernel_mode;

extern char kernel_cheating_password[16];
extern int  kernel_cheating_allowed;
extern int  kernel_cheating_forbidden;

extern KernelDynamicHotSpot kernel_dynamic_hot[KERNEL_MAX_DYNAMIC];
extern int kernel_num_dynamic;
extern int kernel_dynamic_changed;

extern SeriesPtr cursor;                /* Mouse cursor series */

extern int cursor_id;
extern int cursor_last;

extern Kernel kernel;
extern KernelGame game;              /* Kernel level game data */

} // namespace MADSV2
} // namespace MADS

#endif
