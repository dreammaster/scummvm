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

#include "engines/util.h"
#include "mads/madsv2/core/conv.h"
#include "mads/madsv2/core/env.h"
#include "mads/madsv2/core/game.h"
#include "mads/madsv2/core/himem.h"
#include "mads/madsv2/core/imath.h"
#include "mads/madsv2/core/inter.h"
#include "mads/madsv2/core/kernel.h"
#include "mads/madsv2/phantom/phantom.h"
#include "mads/madsv2/phantom/main.h"
#include "mads/madsv2/phantom/sound_phantom.h"
#include "mads/madsv2/phantom/catacombs.h"
#include "mads/madsv2/phantom/global.h"
#include "mads/madsv2/phantom/rooms/section1.h"
#include "mads/madsv2/phantom/rooms/section2.h"
#include "mads/madsv2/phantom/rooms/section3.h"
#include "mads/madsv2/phantom/rooms/section4.h"
#include "mads/madsv2/phantom/rooms/section5.h"

namespace MADS {
namespace MADSV2 {
namespace Phantom {

Common::Error PhantomEngine::run() {
	initGraphics(320, 200);
	_screen = new Graphics::Screen();
	_soundManager = new PhantomSoundManager(_mixer, _soundFlag);
	_soundManager->validate();

	Phantom::phantom_main();

	return Common::kNoError;
}

void PhantomEngine::global_init_code() {
	Common::fill(global, global + GLOBAL_LIST_SIZE, 0);

	global_catacombs_setup();

	/* Section 1 Initialization */

	player.facing = FACING_NORTH;
	player.turn_to_facing = FACING_NORTH;

	global[temp_var] = false;
	global[room_103_104_transition] = NEW_ROOM;
	global[current_year] = 1993;
	global[trap_door_status] = TRAP_DOOR_IS_CLOSED;
	global[christine_door_status] = CHRIS_IS_IN;
	global[sandbag_status] = SANDBAG_SECURE;
	global[jacques_status] = JACQUES_IS_ALIVE;
	global[chris_f_status] = CHRIS_F_IS_ALIVE;
	global[brie_talk_status] = BEFORE_BRIE_MOTIONS;
	global[panel_in_206] = PANEL_UNDISCOVERED;
	global[fight_status] = FIGHT_NOT_HAPPENED;
	global[julies_door] = CRACKED_OPEN;
	global[prompter_stand_status] = PROMPT_LEFT;
	global[chris_d_status] = BEFORE_LOVE;
	global[julie_name_is_known] = JULIE_NO;
	global[doors_in_205] = BOTH_LOCKED;
	global[madame_giry_loc] = MIDDLE;
	global[ticket_people_here] = NEITHER;
	global[coffin_status] = COFFIN_CLOSED;
	global[done_brie_conv_203] = NO;
	global[florent_name_is_known] = NO;
	global[degas_name_is_known] = NO;
	global[madame_giry_shows_up] = false;
	global[jacques_name_is_known] = NO;
	global[charles_name_is_known] = false;
	global[top_floor_locked] = true;
	global[madame_name_is_known] = NO;
	global[chris_kicked_raoul_out] = false;
	global[looked_at_case] = false;
	global[ring_is_on_finger] = false;
	global[he_listened] = false;
	global[knocked_over_head] = false;
	global[observed_phan_104] = false;
	global[read_book] = false;
	global[can_find_book_library] = false;
	global[looked_at_skull_face] = false;
	global[scanned_bookcase] = false;
	global[ran_conv_in_205] = false;
	global[done_rich_conv_203] = false;
	global[hint_that_daae_is_home_1] = false;
	global[hint_that_daae_is_home_2] = false;
	global[make_brie_leave_203] = false;
	global[make_rich_leave_203] = false;
	global[came_from_fade] = false;
	global[christine_told_envelope] = false;
	global[leave_angel_music_on] = false;
	global[door_in_409_is_open] = false;
	global[priest_piston_puke] = false;
	global[cob_web_is_cut] = false;
	global[christine_is_in_boat] = false;
	global[right_door_is_open_504] = false;
	global[chris_left_505] = false;
	global[chris_will_take_seat] = true;
	global[flicked_1] = 0;
	global[flicked_2] = 0;
	global[flicked_3] = 0;
	global[flicked_4] = 0;
	global[player_score] = 0;
	global[player_score_flags] = 0;

	global[music_selected] = imath_random(TOCCATA_FUGUE_D_MINOR, FUGUE_C_MINOR);

	inter_object_routine = global_object_sprite;


	/* Global preload items */

	player_himem_preload("RAL", GLOBAL);
	himem_preload_series("*BOX", GLOBAL);
	himem_preload_series("*LOGO", GLOBAL);
	himem_preload_series("*MENU", GLOBAL);
	himem_preload_series("*CURSOR", GLOBAL);
	himem_preload_series("*FACERAL", GLOBAL);
	himem_preload_series("*RRD_8", GLOBAL);
	himem_preload_series("*RRD_9", GLOBAL);
	himem_preload_series("*RDR_6", GLOBAL);
	himem_preload_series("*RDR_9", GLOBAL);
	himem_preload_series("*RTK_6", GLOBAL);
	himem_preload_series("*RTK_9", GLOBAL);
	himem_preload_series("*RALRH_9", GLOBAL);
}

void PhantomEngine::section_music(int section_num) {
	switch (section_num) {
	case 1: Rooms::section_1_music(); break;
	case 2: Rooms::section_2_music(); break;
	case 3: Rooms::section_3_music(); break;
	case 4: Rooms::section_4_music(); break;
	case 5: Rooms::section_5_music(); break;
	}
}

void PhantomEngine::global_object_sprite() {
	if (inter_object_id == 1 && global[lantern_status] == LANTERN_IS_ON)
		inter_object_id = 31;

	Common::strcpy_s(inter_object_buf, "*OB");
	env_catint(inter_object_buf, inter_object_id, 3);
	Common::strcat_s(inter_object_buf, "I");
}


void PhantomEngine::stop_walker_basic() {
	int random;
	int count;
	int how_many;

	random = imath_random(1, 30000);

	switch (player.facing) {
	case FACING_SOUTH:
		if (random < 500) {
			how_many = imath_random(4, 10);
			for (count = 0; count < how_many; count++) {
				player_add_stop_walker((random < 250) ? 1 : 2, 0);
			}
		} else if (random < 750) {
			for (count = 0; count < 4; count++) {
				player_add_stop_walker(1, 0);
			}

			player_add_stop_walker(0, 0);

			for (count = 0; count < 4; count++) {
				player_add_stop_walker(2, 0);
			}

			player_add_stop_walker(0, 0);
		}
		break;

	case FACING_SOUTHEAST:
	case FACING_SOUTHWEST:
	case FACING_NORTHEAST:
	case FACING_NORTHWEST:
		if (random < 150) {
			player_add_stop_walker(-1, 0);
			player_add_stop_walker(1, 0);
			for (count = 0; count < 6; count++) {
				player_add_stop_walker(0, 0);
			}
		}
		break;

	case FACING_EAST:
	case FACING_WEST:
		if (random < 250) {
			player_add_stop_walker(-1, 0);
			how_many = imath_random(2, 6);
			for (count = 0; count < how_many; count++) {
				player_add_stop_walker(2, 0);
			}
			player_add_stop_walker(1, 0);
			player_add_stop_walker(0, 0);
			player_add_stop_walker(0, 0);
		} else if (random < 500) {
			WRITE_LE_UINT32(&global[walker_timing], kernel.clock);
		}
		break;

	case FACING_NORTH:
		if (random < 250) {
			player_add_stop_walker(-1, 0);
			how_many = imath_random(3, 7);
			for (count = 0; count < how_many; count++) {
				player_add_stop_walker(2, 0);
			}
			player_add_stop_walker(1, 0);
			player_add_stop_walker(0, 0);
		}
		break;

	}
}

void PhantomEngine::stop_walker_tricks() {
	int state;
	int cmd;
	int random;

	state = global[walker_converse_state];
	cmd = global[walker_converse];

	global[walker_converse_now] = state;

	if ((player.facing != FACING_NORTHEAST) &&
		(player.facing != FACING_NORTHWEST)) {
		state = CONVERSE_NONE;
		cmd = CONVERSE_NONE;
	}

	switch (state) {
	case CONVERSE_LEAN:
		switch (cmd) {
		case CONVERSE_LEAN:
			player_add_stop_walker(3, 0);
			break;

		case CONVERSE_HAND_WAVE:
		case CONVERSE_HAND_WAVE_2:
			player_add_stop_walker(6, 0);
			player_add_stop_walker(5, 0);
			player_add_stop_walker(4, 0);
			state = CONVERSE_HAND_WAVE;
			break;

		case CONVERSE_HAND_CHIN:
			player_add_stop_walker(8, 0);
			player_add_stop_walker(4, 0);
			state = CONVERSE_HAND_CHIN;
			break;

		case CONVERSE_NONE:
		default:
			player_add_stop_walker(-2, 0);
			state = CONVERSE_NONE;
			break;
		}
		break;

	case CONVERSE_HAND_WAVE:
	case CONVERSE_HAND_WAVE_2:
		switch (cmd) {
		case CONVERSE_HAND_WAVE:
		case CONVERSE_HAND_WAVE_2:
			random = imath_random(1, 30000);

			if (state == CONVERSE_HAND_WAVE) {
				if (random < 2000) {
					player_add_stop_walker(10, 0);
					player_add_stop_walker(7, 0);
					state = CONVERSE_HAND_WAVE_2;
				} else {
					player_add_stop_walker(6, 0);
				}
			} else {
				if (random < 1000) {
					player_add_stop_walker(6, 0);
					player_add_stop_walker(7, 0);
					state = CONVERSE_HAND_WAVE;
				} else {
					player_add_stop_walker(10, 0);
				}
			}
			break;

		case CONVERSE_LEAN:
		case CONVERSE_HAND_CHIN:
		case CONVERSE_NONE:
		default:
			player_add_stop_walker(-4, 0);
			player_add_stop_walker(-5, 0);
			if (state == CONVERSE_HAND_WAVE_2) {
				player_add_stop_walker(6, 0);
				player_add_stop_walker(7, 0);
			}
			state = CONVERSE_LEAN;
			break;
		}
		break;

	case CONVERSE_HAND_CHIN:
		switch (cmd) {
		case CONVERSE_HAND_CHIN:
			player_add_stop_walker(9, 0);
			break;

		case CONVERSE_LEAN:
		case CONVERSE_HAND_WAVE:
		case CONVERSE_HAND_WAVE_2:
		case CONVERSE_NONE:
		default:
			player_add_stop_walker(-4, 0);
			player_add_stop_walker(-8, 0);
			state = CONVERSE_LEAN;
			break;
		}
		break;

	case CONVERSE_NONE:
	default:
		switch (cmd) {
		case CONVERSE_LEAN:
		case CONVERSE_HAND_WAVE:
		case CONVERSE_HAND_WAVE_2:
		case CONVERSE_HAND_CHIN:
			player_add_stop_walker(2, 0);
			state = CONVERSE_LEAN;
			break;

		case CONVERSE_NONE:
		default:
			stop_walker_basic();
			break;
		}
		break;
	}

	global[walker_converse] = cmd;
	global[walker_converse_state] = state;
}

void PhantomEngine::global_section_constructor() {
	Phantom::global_section_constructor();
}

void PhantomEngine::global_daemon_code() {
	if (player.walker_visible && !global[stop_walker_disabled] && (player.commands_allowed || (conv_control.running >= 0)) && !player.walking &&
		(player.facing == player.turn_to_facing)) {
		if (kernel.clock >= READ_LE_INT32(&global[walker_timing])) {
			if (!player.stop_walker_pointer) {
				stop_walker_tricks();
			}

			WRITE_LE_INT32(&global[walker_timing], READ_LE_INT32(&global[walker_timing]) + 6);
		}
	}
}

} // namespace Phantom
} // namespace MADSV2
} // namespace MADS
