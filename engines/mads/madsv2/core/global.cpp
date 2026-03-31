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

#include "common/textconsole.h"
#include "mads/madsv2/core/global.h"
#include "mads/madsv2/engine.h"

namespace MADS {
namespace MADSV2 {

int global[GLOBAL_LIST_SIZE];
int global_list_size = GLOBAL_LIST_SIZE;

char global_release_name[] = { "ScummVM" };
char global_release_version[] = { "ScummVM" };
char global_release_date[] = { "ScummVM" };
char global_release_copyright[] = { "ScummVM" };


void global_init_code() {
	g_engine->global_init_code();
}

void global_daemon_code() {
	g_engine->global_daemon_code();
}

void global_pre_parser_code() {
	g_engine->global_pre_parser_code();
}

void global_parser_code() {
	g_engine->global_parser_code();
}

void global_error_code() {
	g_engine->global_error_code();
}

void global_room_init() {
	g_engine->global_room_init();
}

void global_verb_filter() {
	error("TODO: void global_verb_filter(void);");
}

void global_sound_driver() {
	error("TODO: void global_sound_driver(void);");
}

void global_section_constructor() {
	g_engine->global_section_constructor();
}

void global_game_menu() {
	error("TODO: void global_game_menu(void);");
}

void global_menu_system_init() {
	error("TODO: void global_menu_system_init(void);");
}

void global_menu_system_shutdown() {
	error("TODO: void global_menu_system_shutdown(void);");
}

void global_emergency_save() {
	error("TODO: void global_emergency_save(void);");
}

void global_read_config_file() {
	error("TODO: void global_read_config_file(void);");
}

void global_write_config_file() {
	error("TODO: void global_write_config_file(void);");
}

void global_load_config_parameters() {
	error("TODO: void global_load_config_parameters(void);");
}

void global_unload_config_parameters() {
	error("TODO: void global_unload_config_parameters(void);");
}

int global_copy_verify() {
	error("TODO: int global_copy_verify(void);");
}

} // namespace MADSV2
} // namespace MADS
