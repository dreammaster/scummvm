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
#include "rail.h"
#include "mads/madsv2/core/room.h"
#include "mads/madsv2/core/buffer.h"
#include "mads/madsv2/core/imath.h"
#include "mads/madsv2/core/player.h"
#include "mads/madsv2/core/error.h"
#include "mads/madsv2/core/matte.h"
#include "mads/madsv2/core/kernel.h"

namespace MADS {
namespace MADSV2 {

word rail_solution_stack_pointer;
word rail_solution_stack_weight;
byte rail_visited[RAIL_MAX_NODES];
byte rail_working_stack[RAIL_MAX_NODES];
byte rail_solution_stack[RAIL_MAX_NODES];
word rail_num_nodes;
byte *rail_base;
byte rail_active[ROOM_MAX_RAILS + 2];

void rail_connect_node(int id) {
	int count;
	int x1, y1;
	int x, y;
	word legal;
	word weight;

	x = room->rail[id].x;
	y = room->rail[id].y;

	rail_active[id] = true;

	for (count = 0; count < (int)rail_num_nodes; count++) {
		if (count != id) {
			if (rail_active[count] && rail_active[id]) {
				x1 = room->rail[count].x;
				y1 = room->rail[count].y;
				if (player.walk_anywhere) {
					legal = LEGAL;
				} else {
					legal = buffer_legal(scr_walk, room->xs, x, y, x1, y1);
				}
				weight = MIN(imath_hypot(abs(x1 - x), abs(y1 - y)), WEIGHT_MASK) | legal;
			} else {
				weight = WEIGHT_MASK | TOTALLY_ILLEGAL;
			}
		} else {
			weight = WEIGHT_MASK | TOTALLY_ILLEGAL;
		}
		room->rail[count].weight[id] = weight;
		room->rail[id].weight[count] = weight;
	}
}

static void disconnector(int alpha, int beta) {
	room->rail[alpha].weight[beta] = WEIGHT_MASK | TOTALLY_ILLEGAL;
}

void rail_disconnect_line(int from, int unto) {
	disconnector(from, unto);
	disconnector(unto, from);
}

void rail_disconnect_node(int id) {
	int count;

	rail_active[id] = false;

	for (count = 0; count < (int)rail_num_nodes; count++) {
		if (count != id) {
			rail_disconnect_line(count, id);
		}
	}
}

void rail_add_node(int id, int x, int y) {
	room->rail[id].x = x;
	room->rail[id].y = y;

	rail_active[id] = true;

	rail_connect_node(id);
}

void rail_connect_all_nodes(void) {
	int count;

	for (count = 0; count < ROOM_MAX_RAILS + 2; count++) {
		rail_active[count] = true;
	}

	for (count = 0; count < room->num_rails; count++) {
		rail_connect_node(count);
	}
}

} // namespace MADSV2
} // namespace MADS
