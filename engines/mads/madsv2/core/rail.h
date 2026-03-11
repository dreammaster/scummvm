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

namespace MADS {
namespace MADSV2 {

#define RAIL_MAX_NODES          ROOM_MAX_RAILS + 2

extern word rail_solution_stack_pointer;
extern word rail_solution_stack_weight;

extern byte rail_visited[RAIL_MAX_NODES];
extern byte rail_working_stack[RAIL_MAX_NODES];
extern byte rail_solution_stack[RAIL_MAX_NODES];

extern word rail_num_nodes;
extern byte *rail_base;

extern void rail_add_node(int id, int x, int y);
extern void rail_connect_node(int id);
extern void rail_connect_all_nodes(void);
extern void rail_disconnect_line(int from, int unto);
extern void rail_disconnect_node(int id);
extern void rail_check_path (int allow_one_illegal);

} // namespace MADSV2
} // namespace MADS
