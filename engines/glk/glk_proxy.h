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

#ifndef GLK_GLK_PROXY_H
#define GLK_GLK_PROXY_H

#include "glk/glk_types.h"
#include "glk/sound.h"
#include "glk/time.h"
#include "glk/windows.h"

namespace Glk {

extern void glk_exit();
extern void glk_set_interrupt_handler(void(*func)(void));
extern void glk_tick();

extern uint glk_gestalt(uint id, uint val);
extern uint glk_gestalt_ext(uint id, uint val, uint *arr, uint arrlen);

unsigned char glk_char_to_lower(unsigned char ch);
unsigned char glk_char_to_upper(unsigned char ch);

extern winid_t glk_window_get_root();
extern winid_t glk_window_open(winid_t split, uint method, uint size,
	uint wintype, uint rock = 0);

extern void glk_window_close(winid_t win, stream_result_t *result = nullptr);
extern void glk_window_get_size(winid_t win, uint *width, uint *height);
extern void glk_window_set_arrangement(winid_t win, uint method,
	uint size, winid_t keyWin);
extern void glk_window_get_arrangement(winid_t win, uint *method,
	uint *size, winid_t *keyWin);
extern winid_t glk_window_iterate(winid_t win, uint *rock = 0);
extern uint glk_window_get_rock(winid_t win);
extern uint glk_window_get_type(winid_t win);
extern winid_t glk_window_get_parent(winid_t win);
extern winid_t glk_window_get_sibling(winid_t win);
extern void glk_window_clear(winid_t win);
extern void glk_window_move_cursor(winid_t win, uint xpos, uint ypos);

extern strid_t glk_window_get_stream(winid_t win);
extern void glk_window_set_echo_stream(winid_t win, strid_t str);
extern strid_t glk_window_get_echo_stream(winid_t win);
extern void glk_set_window(winid_t win);

extern strid_t glk_stream_open_file(frefid_t fileref, FileMode fmode, uint rock = 0);
extern strid_t glk_stream_open_memory(char *buf, uint buflen, FileMode fmode, uint rock = 0);
extern void glk_stream_close(strid_t str, stream_result_t *result = nullptr);
extern strid_t glk_stream_iterate(strid_t str, uint *rockptr);
extern uint glk_stream_get_rock(strid_t str);
extern void glk_stream_set_position(strid_t str, int pos, uint seekMode);
extern uint glk_stream_get_position(strid_t str);
extern void glk_stream_set_current(strid_t str);
extern strid_t glk_stream_get_current();

extern void glk_put_char(unsigned char ch);
extern void glk_put_char_stream(strid_t str, unsigned char ch);
extern void glk_put_string(const char *s);
extern void glk_put_string_stream(strid_t str, const char *s);
extern void glk_put_buffer(const char *buf, uint len);
extern void glk_put_buffer_stream(strid_t str, const char *buf, uint len);
extern void glk_set_style(uint styl);
extern void glk_set_style_stream(strid_t str, uint styl);

extern int glk_get_char_stream(strid_t str);
extern uint glk_get_line_stream(strid_t str, char *buf, uint len);
extern uint glk_get_buffer_stream(strid_t str, char *buf, uint len);

extern void glk_stylehint_set(uint wintype, uint styl, uint hint, int val);
extern void glk_stylehint_clear(uint wintype, uint style, uint hint);
extern uint glk_style_distinguish(winid_t win, uint style1, uint style2);
extern bool glk_style_measure(winid_t win, uint style, uint hint, uint *result);

extern frefid_t glk_fileref_create_temp(uint usage, uint rock = 0);
extern frefid_t glk_fileref_create_by_name(uint usage, const char *name, uint rock = 0);
extern frefid_t glk_fileref_create_by_prompt(uint usage, FileMode fmode, uint rock = 0);
extern frefid_t glk_fileref_create_from_fileref(uint usage, frefid_t fref, uint rock = 0);
extern void glk_fileref_destroy(frefid_t fref);
extern frefid_t glk_fileref_iterate(frefid_t fref, uint *rockptr);
extern uint glk_fileref_get_rock(frefid_t fref);
extern void glk_fileref_delete_file(frefid_t fref);
extern uint glk_fileref_does_file_exist(frefid_t fref);

extern void glk_select(event_t *event);
extern void glk_select_poll(event_t *event);
extern void glk_request_timer_events(uint millisecs);
extern void glk_request_line_event(winid_t win, char *buf, uint maxlen, uint initlen);
extern void glk_request_char_event(winid_t win);
extern void glk_request_mouse_event(winid_t win);

extern void glk_cancel_line_event(winid_t win, event_t *event);
extern void glk_cancel_char_event(winid_t win);
extern void glk_cancel_mouse_event(winid_t win);

#ifdef GLK_MODULE_LINE_ECHO
extern void glk_set_echo_line_event(winid_t win, uint val);
#endif /* GLK_MODULE_LINE_ECHO */

#ifdef GLK_MODULE_LINE_TERMINATORS
extern void glk_set_terminators_line_event(winid_t win, const uint32 *keycodes, uint count);
#endif /* GLK_MODULE_LINE_TERMINATORS */

extern uint glk_buffer_to_lower_case_uni(uint32 *buf, uint len, uint numchars);
extern uint glk_buffer_to_upper_case_uni(uint32 *buf, uint len, uint numchars);
extern uint glk_buffer_to_title_case_uni(uint32 *buf, uint len, uint numchars, uint lowerrest);
extern void glk_put_char_uni(uint32 ch);
extern void glk_put_string_uni(const uint32 *s);
extern void glk_put_buffer_uni(const uint32 *buf, uint len);
extern void glk_put_char_stream_uni(strid_t str, uint32 ch);
extern void glk_put_string_stream_uni(strid_t str, const uint32 *s);
extern void glk_put_buffer_stream_uni(strid_t str, const uint32 *buf, uint len);

extern int glk_get_char_stream_uni(strid_t str);
extern uint glk_get_buffer_stream_uni(strid_t str, uint32 *buf, uint len);
extern uint glk_get_line_stream_uni(strid_t str, uint32 *buf, uint len);

extern strid_t glk_stream_open_file_uni(frefid_t fileref, FileMode fmode, uint rock = 0);
extern strid_t glk_stream_open_memory_uni(uint32 *buf, uint buflen, FileMode fmode, uint rock = 0);

extern void glk_request_char_event_uni(winid_t win);
extern void glk_request_line_event_uni(winid_t win, uint32 *buf, uint maxlen, uint initlen);

#ifdef GLK_MODULE_UNICODE_NORM

extern uint glk_buffer_canon_decompose_uni(uint32 *buf, uint len, uint numchars);
extern uint glk_buffer_canon_normalize_uni(uint32 *buf, uint len, uint numchars);

#endif /* GLK_MODULE_UNICODE_NORM */

#ifdef GLK_MODULE_IMAGE

extern bool glk_image_draw(winid_t win, uint image, int val1, int val2);
extern bool glk_image_draw_scaled(winid_t win, uint image,
	int val1, int val2, uint width, uint height);
extern bool glk_image_draw(winid_t win, const Graphics::Surface &image, uint transColor = (uint)-1,
	int xp = 0, int yp = 0);
extern bool glk_image_draw_scaled(winid_t win, const Graphics::Surface &image, uint transColor,
	int xp, int yp, uint width, uint height);
extern bool glk_image_draw(winid_t win, const Common::String &image, int val1, int val2);
extern bool glk_image_draw_scaled(winid_t win, const Common::String &image,
	int val1, int val2, uint width, uint height);

extern bool glk_image_get_info(uint image, uint *width, uint *height);
extern bool glk_image_get_info(const Common::String &name, uint *width, uint *height);
extern void glk_window_flow_break(winid_t win);

extern void glk_window_erase_rect(winid_t win,
	int left, int top, uint width, uint height);
extern void glk_window_fill_rect(winid_t win, uint color,
	int left, int top, uint width, uint height);
extern void glk_window_set_background_color(winid_t win, uint color);

#endif /* GLK_MODULE_IMAGE */

#ifdef GLK_MODULE_SOUND

extern schanid_t glk_schannel_create(uint rock = 0);
extern void glk_schannel_destroy(schanid_t chan);
extern schanid_t glk_schannel_iterate(schanid_t chan, uint *rockptr);
extern uint glk_schannel_get_rock(schanid_t chan);
extern uint glk_schannel_play(schanid_t chan, uint snd);
extern uint glk_schannel_play_ext(schanid_t chan, uint snd, uint repeats, uint notify);
extern void glk_schannel_stop(schanid_t chan);
extern void glk_schannel_set_volume(schanid_t chan, uint vol);
extern void glk_sound_load_hint(uint snd, uint flag);

#ifdef GLK_MODULE_SOUND2
/* Note that this section is nested inside the #ifdef GLK_MODULE_SOUND.
GLK_MODULE_SOUND must be defined if GLK_MODULE_SOUND2 is. */

extern schanid_t glk_schannel_create_ext(uint rock, uint volume);
extern uint glk_schannel_play_multi(schanid_t *chanarray, uint chancount,
	uint *sndarray, uint soundcount, uint notify);
extern void glk_schannel_pause(schanid_t chan);
extern void glk_schannel_unpause(schanid_t chan);
extern void glk_schannel_set_volume_ext(schanid_t chan, uint vol,
	uint duration, uint notify);

#endif /* GLK_MODULE_SOUND2 */
#endif /* GLK_MODULE_SOUND */

#ifdef GLK_MODULE_HYPERLINKS

extern void glk_set_hyperlink(uint linkval);
extern void glk_set_hyperlink_stream(strid_t str, uint linkval);
extern void glk_request_hyperlink_event(winid_t win);
extern void glk_cancel_hyperlink_event(winid_t win);

#endif /* GLK_MODULE_HYPERLINKS */

#ifdef GLK_MODULE_DATETIME

extern void glk_current_time(glktimeval_t *time);
extern int glk_current_simple_time(uint factor);
extern void glk_time_to_date_utc(const glktimeval_t *time, glkdate_t *date);
extern void glk_time_to_date_local(const glktimeval_t *time, glkdate_t *date);
extern void glk_simple_time_to_date_utc(int time, uint factor, glkdate_t *date);
extern void glk_simple_time_to_date_local(int time, uint factor, glkdate_t *date);
extern void glk_date_to_time_utc(const glkdate_t *date, glktimeval_t *time);
extern void glk_date_to_time_local(const glkdate_t *date, glktimeval_t *time);
extern int glk_date_to_simple_time_utc(const glkdate_t *date, uint factor);
extern int glk_date_to_simple_time_local(const glkdate_t *date, uint factor);

#endif /* GLK_MODULE_DATETIME */

/* XXX non-official Glk functions that may or may not exist */
#define GARGLK 1

extern const char *garglk_fileref_get_name(frefid_t fref);
extern void garglk_set_program_name(const char *name);
extern void garglk_set_program_info(const char *info);
extern void garglk_set_story_name(const char *name);
extern void garglk_set_story_title(const char *title);
extern void garglk_set_config(const char *name);
extern void garglk_unput_string(const char *str);
extern void garglk_unput_string_uni(const uint32 *str);
extern void garglk_set_zcolors(uint fg, uint bg);
extern void garglk_set_zcolors_stream(strid_t str, uint fg, uint bg);
extern void garglk_set_reversevideo(uint reverse);
extern void garglk_set_reversevideo_stream(strid_t str, uint reverse);
extern void garglk_window_get_cursor(winid_t win, uint *xpos, uint *ypos);
extern void garglk_window_get_cursor_current(uint *xpos, uint *ypos);

extern void gidispatch_set_object_registry(gidispatch_rock_t(*regi)(void *obj, uint objclass),
		void(*unregi)(void *obj, uint objclass, gidispatch_rock_t objrock));
extern void gidispatch_set_retained_registry(gidispatch_rock_t(*regi)(void *array, uint len, const char *typecode),
	void(*unregi)(void *array, uint len, const char *typecode, gidispatch_rock_t objrock));
extern uint32 gidispatch_count_classes();
extern const gidispatch_intconst_t *gidispatch_get_class(uint32 index);
extern uint32 gidispatch_count_intconst();
extern const gidispatch_intconst_t *gidispatch_get_intconst(uint32 index);
extern const char *gidispatch_prototype(uint32 funcnum);
extern void gidispatch_call(uint32 funcnum, uint32 numargs, gluniversal_t *arglist);
extern gidispatch_rock_t gidispatch_get_objrock(void *obj, uint objclass);

} // namespace Glk

#endif
