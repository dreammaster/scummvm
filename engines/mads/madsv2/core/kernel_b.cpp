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
#include "mads/madsv2/core/player.h"
#include "mads/madsv2/core/matte.h"
#include "mads/madsv2/core/mem.h"
#include "mads/madsv2/core/error.h"
#include "mads/madsv2/core/buffer.h"
#include "mads/madsv2/core/digi.h"
#include "mads/madsv2/core/kernel.h"
#include "mads/madsv2/core/kernel_1.h"
#include "mads/madsv2/core/kernel_c.h"
#include "mads/madsv2/core/kernel_k.h"

namespace MADS {
namespace MADSV2 {

int stop_speech_on_run_anim = true;

void kernel_animation_init(void) {
	int count;

	for (count = 0; count < KERNEL_MAX_ANIMATIONS; count++) {
		kernel_anim[count].anim = NULL;
		kernel_anim[count].cycled = false;
		kernel_anim[count].repeat = false;
	}
}

int kernel_run_animation(char *name, int trigger_code) {
	int result = -1;
	int found = -1;
	int error_flag = true;
	int count;
	int load_flags;
	int id;
	long largest_block;


	if (stop_speech_on_run_anim) {
		digi_stop(1);
		digi_stop(2);
		digi_stop(3);
	}

	anim_error = -2;

	for (count = 0; (found < 0) && (count < KERNEL_MAX_ANIMATIONS); count++) {
		if (kernel_anim[count].anim == NULL) {
			found = count;
		}
	}

	if (found < 0) goto done;

	load_flags = 0;
	if (kernel.translating) load_flags |= ANIM_LOAD_TRANSLATE;
	kernel_anim[found].anim = anim_load(name,
		NULL, NULL,
		NULL, NULL,
		NULL, NULL,
		NULL, NULL, load_flags);
	if (kernel_anim[found].anim == NULL) goto done;

	kernel_anim[found].messages = 0;
	kernel_anim[found].dynamic_hotspot = -1;

	kernel_anim[found].sprite_loaded = -1;
	if (kernel_anim[found].anim->misc_any_packed) {
		kernel_anim[found].buffer_id = -1;
		id = kernel_anim[found].anim->series_id[kernel_anim[found].anim->misc_packed_series];
		memcpy(&largest_block, &series_list[id]->misc_largest_block, sizeof(long));
		if (mem_get_avail() - 128 >= largest_block) {
			mem_free(series_list[id]->arena);
			series_list[id]->arena = (byte *)mem_get_name(largest_block * 2, "$arena$");
			if (series_list[id]->arena == NULL) {
				series_list[id]->arena = (byte *)mem_get_name(largest_block, "$arena$");
				anim_error = -1;
				if (series_list[id]->arena == NULL) goto done;
			} else {
				kernel_anim[found].buffer[0] = series_list[id]->arena;
				kernel_anim[found].buffer[1] = (byte *)mem_normalize(series_list[id]->arena + largest_block);
				kernel_anim[found].buffer_id = 0;
			}
		}
		kernel_animation_get_sprite(found, 1);
	}

	if (kernel_mode == KERNEL_ACTIVE_CODE) kernel_new_palette();

	kernel_anim[found].frame = 0;
	kernel_anim[found].image = 0;
	kernel_anim[found].next_clock = kernel.clock;

	kernel_anim[found].view_changes = false;

	kernel_anim[found].trigger_code = trigger_code;
	kernel_anim[found].trigger_mode = kernel.trigger_setup_mode;
	for (count = 0; count < 3; count++) {
		kernel_anim[found].trigger_words[count] = player2.words[count];
	}

	for (count = 0; count < kernel_anim[found].anim->num_speech; count++) {
		kernel_anim[found].anim->speech[count].flags = (word)-1;
	}

	error_flag = false;
	result = found;

	kernel_anim[found].last_frame = -1;

done:
	if (error_flag) {
		if (found >= 0) kernel_abort_animation(found);
#ifndef disable_error_check
		Common::strcpy_s(error_string, name);
		error_report(ERROR_KERNEL_NO_ANIMATION, WARNING, MODULE_KERNEL, trigger_code, anim_error);
#endif
	}

	anim_error = 0;
	return found;
}

/* run a talking animation */
int kernel_run_animation_talk(char thing, int num, int trigger_code) {
	Common::String test = "*talk_";
	if (thing == 'r')
		test += "r";
	else if (thing == 'b')
		test += "b";
	else if (thing == 'e')
		test += "e";

	test += Common::String::format("%d", num);

	return kernel_run_animation(test.c_str(), trigger_code);
}

/* run a displacement animation */
int kernel_run_animation_disp(char thing, int num, int trigger_code) {
	Common::String test = "*disp_";

	if (thing == 'r')
		test += "ru";
	else if (thing == 'b')
		test += "ab";
	else if (thing == 'e')
		test += "ed";

	test += Common::String::format("%d", num);

	return kernel_run_animation(test.c_str(), trigger_code);
}

/* run a writing anim (always edgar) */
int kernel_run_animation_write(int trigger_code) {
	int feedback;

	feedback = kernel_run_animation("*write_e", trigger_code);
	return feedback;
}

/* run a pointing animation (always abigail) */
int kernel_run_animation_point(int num, int trigger_code) {
	Common::String test = Common::String::format("*point_b%d", num);
	return kernel_run_animation(test.c_str(), trigger_code);
}

} // namespace MADSV2
} // namespace MADS
