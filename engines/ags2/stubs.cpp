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
#include "ags2/ac/acruntime.h"
#include "ags2/ac/agsplugin.h"
#include "ags2/lib/allegro/surface.h"
#include "ags2/common/cscomp.h"

namespace AGS2 {

void domouse(int) {
	error("TODO: domouse");
}

int mgetbutton() {
	error("TODO: mgetbutton");
	return 0;
}

const char *get_engine_version() {
	error("TODO: get_engine_version");
}

void quitprintf(const char *texx, ...) {
	error("TODO: quitprintf");
}

void RefreshMouse() {
	error("TODO: RefreshMouse");
}

int run_script_function_if_exist(ccInstance *, char *, int, int, int, int) {
	error("TODO: run_script_function_if_exist");
}

int IsChannelPlaying(int) {
	error("TODO: IsChannelPlaying");
}

void stop_and_destroy_channel(int) {
	error("TODO: stop_and_destroy_channel");
}

int rec_kbhit() {
	error("TODO: rec_kbhit");
}

int rec_getch() {
	error("TODO: rec_getch");
}

void update_polled_stuff(bool) {
	error("TODO: update_polled_stuff");
}

void invalidate_rect(int, int, int, int) {
	error("TODO: invalidate_rect");
}
int find_word_in_dictionary(char *) {
	error("TODO");
}
void break_up_text_into_lines(int, int, char *) {
	error("TODO");
}
int wgetfontheight(int) {
	error("TODO");
}
void draw_and_invalidate_text(int, int, int, char const *) {
	error("TODO");
}
void scriptDebugHook(struct AGS2::ccInstance *, int) {
	error("TODO");
}
void invalidate_screen(void) {
	error("TODO");
}
void debug_write_console(char const *, ...) {
	error("TODO");
}

void shutdown_sound(void) { error("TODO"); }
int GetScalingAt(int,int) { error("TODO"); }
int wgettextwidth_compensate(char const *,int) { error("TODO"); }
void add_dynamic_sprite(int, BITMAP *,bool) { error("TODO"); }
void free_dynamic_sprite(int) { error("TODO"); }
char const * CreateNewScriptString(char const *,bool) { error("TODO"); }
int divide_down_coordinate(int) { error("TODO"); }
int multiply_up_coordinate(int) { error("TODO"); }
void multiply_up_coordinates(int *,int *) { error("TODO"); }
void get_current_dir_path(char *,char const *) { error("TODO"); }

bool ShouldAntiAliasText(void) { error("TODO"); }
void cc_error(char const *,...) { error("TODO"); }
void fputstring(char const *,class Common::WriteStream *) { error("TODO"); }
void drawing_mode(int, BITMAP *,int,int) { error("TODO"); }
void solid_mode(void) { error("TODO"); }

void ExecutingScript::run_another(char *, int, int) {
	error("TODO");
}

int IAGSEngine::GetWindowHandle(void) { error("TODO"); }
void * IAGSEngine::GetDirectDraw2(void) { error("TODO"); }
void * IAGSEngine::GetBitmapSurface(BITMAP *) { error("TODO"); }
void * IAGSEngine::GetDirectSound(void) { error("TODO"); }
void * IAGSEngine::GetDirectInputKeyboard(void) { error("TODO"); }
void * IAGSEngine::GetDirectInputMouse(void) { error("TODO"); }

} // namespace AGS2
