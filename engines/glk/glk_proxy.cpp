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

#include "glk/glk_proxy.h"
#include "glk/glk_api.h"

namespace Glk {

void glk_exit() {
	g_api->glk_exit();
}
void glk_set_interrupt_handler(void(*func)(void)) {
	g_api->glk_set_interrupt_handler(func);
}
void glk_tick() {
	g_api->glk_tick();
}

uint glk_gestalt(uint id, uint val) {
	return g_api->glk_gestalt(id, val);
}
uint glk_gestalt_ext(uint id, uint val, uint *arr, uint arrlen) {
	return g_api->glk_gestalt_ext(id, val, arr, arrlen);
}

unsigned char glk_char_to_lower(unsigned char ch) {
	return g_api->glk_char_to_lower(ch);
}
unsigned char glk_char_to_upper(unsigned char ch) {
	return g_api->glk_char_to_upper(ch);
}

winid_t glk_window_get_root(void) {
	return g_api->glk_window_get_root();
}

winid_t glk_window_open(winid_t split, uint method, uint size,
	uint wintype, uint rock) {
	return g_api->glk_window_open(split, method, size, wintype, rock);
}

void glk_window_close(winid_t win, stream_result_t *result) {
	return g_api->glk_window_close(win, result);
}
void glk_window_get_size(winid_t win, uint *width, uint *height) {
	return g_api->glk_window_get_size(win, width, height);
}
void glk_window_set_arrangement(winid_t win, uint method,
	uint size, winid_t keyWin) {
	return g_api->glk_window_set_arrangement(win, method, size, keyWin);
}
void glk_window_get_arrangement(winid_t win, uint *method,
	uint *size, winid_t *keyWin) {
	g_api->glk_window_get_arrangement(win, method, size, keyWin);
}
winid_t glk_window_iterate(winid_t win, uint *rock) {
	return g_api->glk_window_iterate(win, rock);
}
uint glk_window_get_rock(winid_t win) {
	return g_api->glk_window_get_rock(win);
}
uint glk_window_get_type(winid_t win) {
	return g_api->glk_window_get_type(win);
}
winid_t glk_window_get_parent(winid_t win) {
	return g_api->glk_window_get_parent(win);
}
winid_t glk_window_get_sibling(winid_t win) {
	return g_api->glk_window_get_sibling(win);
}
void glk_window_clear(winid_t win) {
	g_api->glk_window_clear(win);
}
void glk_window_move_cursor(winid_t win, uint xpos, uint ypos) {
	g_api->glk_window_move_cursor(win, xpos, ypos);
}

strid_t glk_window_get_stream(winid_t win) {
	return g_api->glk_window_get_stream(win);
}
void glk_window_set_echo_stream(winid_t win, strid_t str) {
	g_api->glk_window_set_echo_stream(win, str);
}
strid_t glk_window_get_echo_stream(winid_t win) {
	return g_api->glk_window_get_echo_stream(win);
}
void glk_set_window(winid_t win) {
	g_api->glk_set_window(win);
}

strid_t glk_stream_open_file(frefid_t fileref, FileMode fmode, uint rock) {
	return g_api->glk_stream_open_file(fileref, fmode, rock);
}
strid_t glk_stream_open_memory(char *buf, uint buflen, FileMode fmode, uint rock) {
	return g_api->glk_stream_open_memory(buf, buflen, fmode, rock);
}
void glk_stream_close(strid_t str, stream_result_t *result) {
	g_api->glk_stream_close(str, result);
}
strid_t glk_stream_iterate(strid_t str, uint *rockptr) {
	return g_api->glk_stream_iterate(str, rockptr);
}
uint glk_stream_get_rock(strid_t str) {
	return g_api->glk_stream_get_rock(str);
}
void glk_stream_set_position(strid_t str, int pos, uint seekMode) {
	g_api->glk_stream_set_position(str, pos, seekMode);
}
uint glk_stream_get_position(strid_t str) {
	return g_api->glk_stream_get_position(str);
}
void glk_stream_set_current(strid_t str) {
	g_api->glk_stream_set_current(str);
}
strid_t glk_stream_get_current() {
	return g_api->glk_stream_get_current();
}

void glk_put_char(unsigned char ch) {
	g_api->glk_put_char(ch);
}
void glk_put_char_stream(strid_t str, unsigned char ch) {
	g_api->glk_put_char_stream(str, ch);
}
void glk_put_string(const char *s) {
	g_api->glk_put_string(s);
}
void glk_put_string_stream(strid_t str, const char *s) {
	g_api->glk_put_string_stream(str, s);
}
void glk_put_buffer(const char *buf, uint len) {
	g_api->glk_put_buffer(buf, len);
}
void glk_put_buffer_stream(strid_t str, const char *buf, uint len) {
	g_api->glk_put_buffer_stream(str, buf, len);
}
void glk_set_style(uint styl) {
	g_api->glk_set_style(styl);
}
void glk_set_style_stream(strid_t str, uint styl) {
	g_api->glk_set_style_stream(str, styl);
}

int glk_get_char_stream(strid_t str) {
	return g_api->glk_get_char_stream(str);
}
uint glk_get_line_stream(strid_t str, char *buf, uint len) {
	return g_api->glk_get_line_stream(str, buf, len);
}
uint glk_get_buffer_stream(strid_t str, char *buf, uint len) {
	return g_api->glk_get_buffer_stream(str, buf, len);
}

void glk_stylehint_set(uint wintype, uint styl, uint hint, int val) {
	g_api->glk_stylehint_set(wintype, styl, hint, val);
}
void glk_stylehint_clear(uint wintype, uint style, uint hint) {
	g_api->glk_stylehint_clear(wintype, style, hint);
}
uint glk_style_distinguish(winid_t win, uint style1, uint style2) {
	return g_api->glk_style_distinguish(win, style1, style2);
}
bool glk_style_measure(winid_t win, uint style, uint hint, uint *result) {
	return g_api->glk_style_measure(win, style, hint, result);
}

frefid_t glk_fileref_create_temp(uint usage, uint rock) {
	return g_api->glk_fileref_create_temp(usage, rock);
}
frefid_t glk_fileref_create_by_name(uint usage, const char *name, uint rock) {
	return g_api->glk_fileref_create_by_name(usage, name, rock);
}
frefid_t glk_fileref_create_by_prompt(uint usage, FileMode fmode, uint rock) {
	return g_api->glk_fileref_create_by_prompt(usage, fmode, rock);
}
frefid_t glk_fileref_create_from_fileref(uint usage, frefid_t fref, uint rock) {
	return g_api->glk_fileref_create_from_fileref(usage, fref, rock);
}
void glk_fileref_destroy(frefid_t fref) {
	g_api->glk_fileref_destroy(fref);
}
frefid_t glk_fileref_iterate(frefid_t fref, uint *rockptr) {
	return g_api->glk_fileref_iterate(fref, rockptr);
}
uint glk_fileref_get_rock(frefid_t fref) {
	return g_api->glk_fileref_get_rock(fref);
}
void glk_fileref_delete_file(frefid_t fref) {
	g_api->glk_fileref_delete_file(fref);
}
uint glk_fileref_does_file_exist(frefid_t fref) {
	return g_api->glk_fileref_does_file_exist(fref);
}

void glk_select(event_t *event) {
	g_api->glk_select(event);
}
void glk_select_poll(event_t *event) {
	g_api->glk_select_poll(event);
}
void glk_request_timer_events(uint millisecs) {
	g_api->glk_request_timer_events(millisecs);
}
void glk_request_line_event(winid_t win, char *buf, uint maxlen, uint initlen) {
	g_api->glk_request_line_event(win, buf, maxlen, initlen);
}
void glk_request_char_event(winid_t win) {
	g_api->glk_request_char_event(win);
}
void glk_request_mouse_event(winid_t win) {
	g_api->glk_request_mouse_event(win);
}

void glk_cancel_line_event(winid_t win, event_t *event) {
	g_api->glk_cancel_line_event(win, event);
}
void glk_cancel_char_event(winid_t win) {
	g_api->glk_cancel_char_event(win);
}
void glk_cancel_mouse_event(winid_t win) {
	g_api->glk_cancel_mouse_event(win);
}

#ifdef GLK_MODULE_LINE_ECHO
void glk_set_echo_line_event(winid_t win, uint val) {
	g_api->glk_set_echo_line_event(win, val);
}
#endif /* GLK_MODULE_LINE_ECHO */

#ifdef GLK_MODULE_LINE_TERMINATORS
void glk_set_terminators_line_event(winid_t win, const uint32 *keycodes, uint count) {
	g_api->glk_set_terminators_line_event(win, keycodes, count);
}
#endif /* GLK_MODULE_LINE_TERMINATORS */

uint glk_buffer_to_lower_case_uni(uint32 *buf, uint len, uint numchars) {
	return g_api->glk_buffer_to_lower_case_uni(buf, len, numchars);
}
uint glk_buffer_to_upper_case_uni(uint32 *buf, uint len, uint numchars) {
	return g_api->glk_buffer_to_upper_case_uni(buf, len, numchars);
}
uint glk_buffer_to_title_case_uni(uint32 *buf, uint len, uint numchars, uint lowerrest) {
	return g_api->glk_buffer_to_title_case_uni(buf, len, numchars, lowerrest);
}
void glk_put_char_uni(uint32 ch) {
	g_api->glk_put_char_uni(ch);
}
void glk_put_string_uni(const uint32 *s) {
	g_api->glk_put_string_uni(s);
}
void glk_put_buffer_uni(const uint32 *buf, uint len) {
	g_api->glk_put_buffer_uni(buf, len);
}
void glk_put_char_stream_uni(strid_t str, uint32 ch) {
	g_api->glk_put_char_stream_uni(str, ch);
}
void glk_put_string_stream_uni(strid_t str, const uint32 *s) {
	g_api->glk_put_string_stream_uni(str, s);
}
void glk_put_buffer_stream_uni(strid_t str, const uint32 *buf, uint len) {
	g_api->glk_put_buffer_stream_uni(str, buf, len);
}

int glk_get_char_stream_uni(strid_t str) {
	return g_api->glk_get_char_stream_uni(str);
}
uint glk_get_buffer_stream_uni(strid_t str, uint32 *buf, uint len) {
	return g_api->glk_get_buffer_stream_uni(str, buf, len);
}
uint glk_get_line_stream_uni(strid_t str, uint32 *buf, uint len) {
	return g_api->glk_get_line_stream_uni(str, buf, len);
}

strid_t glk_stream_open_file_uni(frefid_t fileref, FileMode fmode, uint rock) {
	return g_api->glk_stream_open_file_uni(fileref, fmode, rock);
}
strid_t glk_stream_open_memory_uni(uint32 *buf, uint buflen, FileMode fmode, uint rock) {
	return g_api->glk_stream_open_memory_uni(buf, buflen, fmode, rock);
}

void glk_request_char_event_uni(winid_t win) {
	g_api->glk_request_char_event_uni(win);
}
void glk_request_line_event_uni(winid_t win, uint32 *buf, uint maxlen, uint initlen) {
	g_api->glk_request_line_event_uni(win, buf, maxlen, initlen);
}

#ifdef GLK_MODULE_UNICODE_NORM

uint glk_buffer_canon_decompose_uni(uint32 *buf, uint len, uint numchars) {
	return g_api->glk_buffer_canon_decompose_uni(buf, len, numchars);
}
uint glk_buffer_canon_normalize_uni(uint32 *buf, uint len, uint numchars) {
	return g_api->glk_buffer_canon_normalize_uni(buf, len, numchars);
}

#endif /* GLK_MODULE_UNICODE_NORM */

#ifdef GLK_MODULE_IMAGE

bool glk_image_draw(winid_t win, uint image, int val1, int val2) {
	return g_api->glk_image_draw(win, image, val1, val2);
}
bool glk_image_draw_scaled(winid_t win, uint image,
		int val1, int val2, uint width, uint height) {
	return g_api->glk_image_draw_scaled(win, image, val1, val2, width, height);
}
bool glk_image_draw(winid_t win, const Graphics::Surface &image, uint transColor,
		int xp, int yp) {
	return g_api->glk_image_draw(win, image, transColor, xp, yp);
}
bool glk_image_draw_scaled(winid_t win, const Graphics::Surface &image, uint transColor,
	int xp, int yp, uint width, uint height) {
	return g_api->glk_image_draw_scaled(win, image, transColor, xp, yp, width, height);
}
bool glk_image_draw(winid_t win, const Common::String &image, int val1, int val2) {
	return g_api->glk_image_draw(win, image, val1, val2);
}
bool glk_image_draw_scaled(winid_t win, const Common::String &image,
	int val1, int val2, uint width, uint height) {
	return g_api->glk_image_draw_scaled(win, image, val1, val2, width, height);
}

bool glk_image_get_info(uint image, uint *width, uint *height) {
	return g_api->glk_image_get_info(image, width, height);
}
bool glk_image_get_info(const Common::String &name, uint *width, uint *height) {
	return g_api->glk_image_get_info(name, width, height);
}

void glk_window_flow_break(winid_t win) {
	g_api->glk_window_flow_break(win);
}

void glk_window_erase_rect(winid_t win,
	int left, int top, uint width, uint height) {
	g_api->glk_window_erase_rect(win, left, top, width, height);
}
void glk_window_fill_rect(winid_t win, uint color,
	int left, int top, uint width, uint height) {
	g_api->glk_window_fill_rect(win, color, left, top, width, height);
}
void glk_window_set_background_color(winid_t win, uint color) {
	g_api->glk_window_set_background_color(win, color);
}

#endif /* GLK_MODULE_IMAGE */

#ifdef GLK_MODULE_SOUND

schanid_t glk_schannel_create(uint rock) {
	return g_api->glk_schannel_create(rock);
}
void glk_schannel_destroy(schanid_t chan) {
	g_api->glk_schannel_destroy(chan);
}
schanid_t glk_schannel_iterate(schanid_t chan, uint *rockptr) {
	return g_api->glk_schannel_iterate(chan, rockptr);
}
uint glk_schannel_get_rock(schanid_t chan) {
	return g_api->glk_schannel_get_rock(chan);
}
uint glk_schannel_play(schanid_t chan, uint snd) {
	return g_api->glk_schannel_play(chan, snd);
}
uint glk_schannel_play_ext(schanid_t chan, uint snd, uint repeats, uint notify) {
	return g_api->glk_schannel_play_ext(chan, snd, repeats, notify);
}
void glk_schannel_stop(schanid_t chan) {
	g_api->glk_schannel_stop(chan);
}
void glk_schannel_set_volume(schanid_t chan, uint vol) {
	g_api->glk_schannel_set_volume(chan, vol);
}
void glk_sound_load_hint(uint snd, uint flag) {
	g_api->glk_sound_load_hint(snd, flag);
}

#ifdef GLK_MODULE_SOUND2
/* Note that this section is nested inside the #ifdef GLK_MODULE_SOUND.
GLK_MODULE_SOUND must be defined if GLK_MODULE_SOUND2 is. */

schanid_t glk_schannel_create_ext(uint rock, uint volume) {
	return g_api->glk_schannel_create_ext(rock, volume);
}
uint glk_schannel_play_multi(schanid_t *chanarray, uint chancount,
	uint *sndarray, uint soundcount, uint notify) {
	return g_api->glk_schannel_play_multi(chanarray, chancount,
		sndarray, soundcount, notify);
}
void glk_schannel_pause(schanid_t chan) {
	g_api->glk_schannel_pause(chan);
}
void glk_schannel_unpause(schanid_t chan) {
	g_api->glk_schannel_unpause(chan);
}
void glk_schannel_set_volume_ext(schanid_t chan, uint vol,
	uint duration, uint notify) {
	g_api->glk_schannel_set_volume_ext(chan, vol, duration, notify);
}

#endif /* GLK_MODULE_SOUND2 */
#endif /* GLK_MODULE_SOUND */

#ifdef GLK_MODULE_HYPERLINKS

void glk_set_hyperlink(uint linkval) {
	g_api->glk_set_hyperlink(linkval);
}
void glk_set_hyperlink_stream(strid_t str, uint linkval) {
	g_api->glk_set_hyperlink_stream(str, linkval);
}
void glk_request_hyperlink_event(winid_t win) {
	g_api->glk_request_hyperlink_event(win);
}
void glk_cancel_hyperlink_event(winid_t win) {
	g_api->glk_cancel_hyperlink_event(win);
}

#endif /* GLK_MODULE_HYPERLINKS */

#ifdef GLK_MODULE_DATETIME

void glk_current_time(glktimeval_t *time) {
	g_api->glk_current_time(time);
}
int glk_current_simple_time(uint factor) {
	return g_api->glk_current_simple_time(factor);
}
void glk_time_to_date_utc(const glktimeval_t *time, glkdate_t *date) {
	g_api->glk_time_to_date_utc(time, date);
}
void glk_time_to_date_local(const glktimeval_t *time, glkdate_t *date) {
	g_api->glk_time_to_date_local(time, date);
}
void glk_simple_time_to_date_utc(int time, uint factor, glkdate_t *date) {
	g_api->glk_simple_time_to_date_utc(time, factor, date);
}
void glk_simple_time_to_date_local(int time, uint factor, glkdate_t *date) {
	g_api->glk_simple_time_to_date_local(time, factor, date);
}
void glk_date_to_time_utc(const glkdate_t *date, glktimeval_t *time) {
	g_api->glk_date_to_time_utc(date, time);
}
void glk_date_to_time_local(const glkdate_t *date, glktimeval_t *time) {
	g_api->glk_date_to_time_local(date, time);
}
int glk_date_to_simple_time_utc(const glkdate_t *date, uint factor) {
	return g_api->glk_date_to_simple_time_utc(date, factor);
}
int glk_date_to_simple_time_local(const glkdate_t *date, uint factor) {
	return g_api->glk_date_to_simple_time_local(date, factor);
}

#endif /* GLK_MODULE_DATETIME */

/* XXX non-official Glk functions that may or may not exist */
#define GARGLK 1

const char *garglk_fileref_get_name(frefid_t fref) {
	return g_api->garglk_fileref_get_name(fref);
}
void garglk_set_program_name(const char *name) {
	g_api->garglk_set_program_name(name);
}
void garglk_set_program_info(const char *info) {
	g_api->garglk_set_program_info(info);
}
void garglk_set_story_name(const char *name) {
	g_api->garglk_set_story_name(name);
}
void garglk_set_story_title(const char *title) {
	g_api->garglk_set_story_title(title);
}
void garglk_set_config(const char *name) {
	g_api->garglk_set_config(name);
}
void garglk_unput_string(const char *str) {
	g_api->garglk_unput_string(str);
}
void garglk_unput_string_uni(const uint32 *str) {
	g_api->garglk_unput_string_uni(str);
}
void garglk_set_zcolors(uint fg, uint bg) {
	g_api->garglk_set_zcolors(fg, bg);
}
void garglk_set_zcolors_stream(strid_t str, uint fg, uint bg) {
	g_api->garglk_set_zcolors_stream(str, fg, bg);
}
void garglk_set_reversevideo(uint reverse) {
	g_api->garglk_set_reversevideo(reverse);
}
void garglk_set_reversevideo_stream(strid_t str, uint reverse) {
	g_api->garglk_set_reversevideo_stream(str, reverse);
}
void garglk_window_get_cursor(winid_t win, uint *xpos, uint *ypos) {
	g_api->garglk_window_get_cursor(win, xpos, ypos);
}
void garglk_window_get_cursor_current(uint *xpos, uint *ypos) {
	g_api->garglk_window_get_cursor_current(xpos, ypos);
}

void gidispatch_set_object_registry(gidispatch_rock_t(*regi)(void *obj, uint objclass),
		void(*unregi)(void *obj, uint objclass, gidispatch_rock_t objrock)) {
	g_api->gidispatch_set_object_registry(regi, unregi);
}
void gidispatch_set_retained_registry(gidispatch_rock_t(*regi)(void *array, uint len, const char *typecode),
	void(*unregi)(void *array, uint len, const char *typecode, gidispatch_rock_t objrock)) {
	g_api->gidispatch_set_retained_registry(regi, unregi);
}

uint32 gidispatch_count_classes() {
	return g_api->gidispatch_count_classes();
}
const gidispatch_intconst_t *gidispatch_get_class(uint32 index) {
	return g_api->gidispatch_get_class(index);
}
uint32 gidispatch_count_intconst() {
	return g_api->gidispatch_count_intconst();
}
const gidispatch_intconst_t *gidispatch_get_intconst(uint32 index) {
	return g_api->gidispatch_get_intconst(index);
}
const char *gidispatch_prototype(uint32 funcnum) {
	return g_api->gidispatch_prototype(funcnum);
}
void gidispatch_call(uint32 funcnum, uint32 numargs, gluniversal_t *arglist) {
	g_api->gidispatch_call(funcnum, numargs, arglist);
}
gidispatch_rock_t gidispatch_get_objrock(void *obj, uint objclass) {
	return g_api->gidispatch_get_objrock(obj, objclass);
}

} // namespace Glk
