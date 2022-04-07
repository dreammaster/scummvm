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

#ifndef DINK_SCRIPTS_H
#define DINK_SCRIPTS_H

namespace Dink {

extern void init_scripts();
extern int load_script(const char *filename, int sprite, bool set_sprite);
extern void run_script(int script);
extern int get_var(int script, char *name);
extern bool read_next_line(int script, char *line);
extern void kill_script(int num);
extern void kill_scripts_owned_by(int sprite);
extern void kill_all_scripts_for_real();
extern void kill_all_scripts();

} // namespace Dink

#endif
