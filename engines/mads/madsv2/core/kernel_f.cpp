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
#include "mads/madsv2/core/vocab.h"
#include "mads/madsv2/core/inter.h"
#include "mads/madsv2/core/error.h"
#include "mads/madsv2/core/kernel.h"
#include "mads/madsv2/core/kernel_1.h"
#include "mads/madsv2/core/kernel_8.h"

namespace MADS {
namespace MADSV2 {

int kernel_add_dynamic(int vocab_id, int verb_id, byte syntax,
	int auto_sequence, int x, int y, int xs, int ys) {
	int id = -1;
	int count;

	for (count = 0; (id < 0) && (count < KERNEL_MAX_DYNAMIC); count++) {
		if (!kernel_dynamic_hot[count].flags) {
			id = count;
		}
	}

	if (id < 0) {
		error_report(ERROR_DYNAMIC_HOTSPOT_OVERFLOW, WARNING, MODULE_KERNEL, id, KERNEL_MAX_DYNAMIC);
		goto done;
	}

	kernel_dynamic_hot[id].flags = true;
	kernel_dynamic_hot[id].vocab_id = vocab_id;
	kernel_dynamic_hot[id].auto_sequence = auto_sequence;
	kernel_dynamic_hot[id].x = x;
	kernel_dynamic_hot[id].y = y;
	kernel_dynamic_hot[id].xs = xs;
	kernel_dynamic_hot[id].ys = ys;

	kernel_dynamic_hot[id].feet_x = WALK_DIRECT_2;
	kernel_dynamic_hot[id].feet_y = 0;
	kernel_dynamic_hot[id].facing = 5;

	kernel_dynamic_hot[id].verb_id = verb_id;
	kernel_dynamic_hot[id].prep = PREP_IN;
	kernel_dynamic_hot[id].syntax = syntax;

	kernel_dynamic_hot[id].cursor = 0;

	kernel_dynamic_hot[id].valid = true;

	kernel_dynamic_hot[id].auto_anim = -1;
	for (count = 0; count < KERNEL_DYNAMIC_MAX_SEGMENTS; count++) {
		kernel_dynamic_hot[id].auto_segment[count] = KERNEL_DYNAMIC_NO_ANIM;
	}


	kernel_num_dynamic++;
	kernel_dynamic_changed = true;

	if (auto_sequence >= 0) {
		sequence_list[auto_sequence].dynamic_hotspot = id;
		kernel_dynamic_hot[id].valid = false;
	}

done:
	return id;
}

void kernel_dynamic_anim(int id, int anim_id, int segment) {
	int count;
	int found = false;

	if ((id >= 0) && (id < KERNEL_MAX_DYNAMIC)) {
		if (kernel_anim[anim_id].anim != NULL) {
			kernel_anim[anim_id].dynamic_hotspot = id;
			if (kernel_dynamic_hot[id].auto_anim < 0) {
				kernel_dynamic_hot[id].valid = false;
			}
			kernel_dynamic_hot[id].auto_anim = (char)anim_id;
			for (count = 0; !found && (count < KERNEL_DYNAMIC_MAX_SEGMENTS); count++) {
				if (kernel_dynamic_hot[id].auto_segment[count] == KERNEL_DYNAMIC_NO_ANIM) {
					kernel_dynamic_hot[id].auto_segment[count] = (byte)segment;
					found = true;
				}
			}

			if (!found) {
				error_report(ERROR_DYNAMIC_HOTSPOT_OVERFLOW, ERROR, MODULE_KERNEL, -9999, id);
			}

			kernel_dynamic_changed = true;
		}
	}
}

int kernel_dynamic_walk(int id, int feet_x, int feet_y, int facing) {
	if (id >= 0) {
		kernel_dynamic_hot[id].feet_x = feet_x;
		kernel_dynamic_hot[id].feet_y = feet_y;
		kernel_dynamic_hot[id].facing = (byte)facing;
	}

	return id;
}

int kernel_dynamic_cursor(int id, int cursor_) {
	if (id >= 0) {
		kernel_dynamic_hot[id].cursor = (byte)cursor_;
	}

	return id;
}

void kernel_delete_dynamic(int id) {
	if (kernel_dynamic_hot[id].flags) {
		if (kernel_dynamic_hot[id].auto_sequence >= 0) {
			sequence_list[kernel_dynamic_hot[id].auto_sequence].dynamic_hotspot = -1;
		}
		if (kernel_dynamic_hot[id].auto_anim >= 0) {
			kernel_anim[kernel_dynamic_hot[id].auto_anim].dynamic_hotspot = -1;
		}
		kernel_dynamic_hot[id].flags = false;
		kernel_num_dynamic--;
		kernel_dynamic_changed = true;
	}
}

void kernel_purge_dynamic() {
	int count;

	for (count = 0; count < KERNEL_MAX_DYNAMIC; count++) {
		kernel_delete_dynamic(count);
	}
	kernel_num_dynamic = 0;
	kernel_dynamic_changed = true;
}

void kernel_init_dynamic() {
	int count;

	for (count = 0; count < KERNEL_MAX_DYNAMIC; count++) {
		kernel_dynamic_hot[count].flags = false;
	}
	kernel_num_dynamic = 0;
	kernel_dynamic_changed = 0;
}

} // namespace MADSV2
} // namespace MADS
