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

#ifndef MADS_CORE_KERNEL_N_H
#define MADS_CORE_KERNEL_N_H

namespace MADS {
namespace MADSV2 {

extern void kernel_random_messages_init(int max_messages_at_once,
	int min_x, int max_x, int min_y, int max_y, int min_y_spacing,
	int teletype_rate, int color, int duration, int quote_id, ...);
extern int  kernel_check_random(void);
extern void kernel_random_message_server(void);
extern int kernel_generate_random_message(int chance_major,
	int chance_minor);
extern void kernel_random_purge(void);

} // namespace MADSV2
} // namespace MADS

#endif
