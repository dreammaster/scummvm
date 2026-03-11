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
#include "mads/madsv2/core/room.h"
#include "mads/madsv2/core/color.h"
#include "mads/madsv2/core/kernel.h"

namespace MADS {
namespace MADSV2 {

byte video_mode;
RoomPtr room     = NULL;
int room_id      = KERNEL_STARTING_GAME;
int section_id   = KERNEL_STARTING_GAME;
int room_variant = 0;

int new_room     = 101;
int new_section  = 1;

int previous_room    = 0;
int previous_section = 0;

int kernel_initial_variant = 0;

HotPtr room_spots     = NULL;
int    room_num_spots = 0;

int kernel_room_series_marker = 0;

int kernel_room_bound_dif;
int kernel_room_scale_dif;

int kernel_allow_peel = false;

int kernel_panning_speed = 0;
int kernel_screen_fade   = 0;

Animation kernel_anim[KERNEL_MAX_ANIMATIONS];

ShadowList kernel_shadow_main  = { 0 };
ShadowList kernel_shadow_inter = { 1, { 15 } };

int kernel_ok_to_fail_load = false;

int kernel_mode = KERNEL_GAME_LOAD;

char kernel_cheating_password[16];
int  kernel_cheating_allowed   = 0;
int  kernel_cheating_forbidden = 0;

KernelDynamicHotSpot kernel_dynamic_hot[KERNEL_MAX_DYNAMIC];
int kernel_num_dynamic     = 0;
int kernel_dynamic_changed = 0;


SeriesPtr cursor = NULL;                /* Mouse cursor series */

int cursor_id   = 1;
int cursor_last = -1;

Kernel     kernel;                      /* Kernel data            */
KernelGame game;                        /* Kernel level game data */

} // namespace MADSV2
} // namespace MADS
