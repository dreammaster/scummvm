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

#ifdef __AESOP__

#define PROCDEF

#else

namespace Aesop {

void cdecl load_string(LONG argcnt, BYTE *array, ULONG string);
void cdecl load_resource(LONG argcnt, BYTE *array, ULONG resource);
void cdecl copy_string(LONG argcnt, BYTE *src, BYTE *dest);
void cdecl string_force_lower(LONG argcnt, BYTE *dest);
void cdecl string_force_upper(LONG argcnt, BYTE *dest);
ULONG cdecl string_len(LONG argcnt, BYTE *string);
ULONG cdecl string_compare(LONG argcnt, BYTE *str1, BYTE *str2);
void cdecl beep(void);
LONG cdecl strval(LONG argcnt, BYTE *string);
LONG cdecl envval(LONG argcnt, BYTE *name);
void cdecl pokemem(LONG argcnt, LONG *addr, LONG data);
LONG cdecl peekmem(LONG argcnt, LONG *addr);
ULONG cdecl rnd(LONG argcnt, ULONG low, ULONG high);
ULONG cdecl dice(LONG argcnt, ULONG ndice, ULONG nsides, ULONG bonus);
ULONG cdecl absv(LONG argcnt, LONG val);
LONG cdecl minv(LONG argcnt, LONG val1, LONG val2);
LONG cdecl maxv(LONG argcnt, LONG val1, LONG val2);
void cdecl diagnose(LONG argcnt, ULONG dtype, ULONG parm);
ULONG cdecl heapfree(void);

void cdecl notify(LONG argcnt, ULONG index, ULONG message, LONG event,
	LONG parameter);
void cdecl cancel(LONG argcnt, ULONG index, ULONG message, LONG event,
	LONG parameter);
void cdecl drain_event_queue(void);
void cdecl post_event(LONG argcnt, ULONG owner, LONG event, LONG parameter);
void cdecl send_event(LONG argcnt, ULONG owner, LONG event, LONG parameter);
ULONG cdecl peek_event(void);
void cdecl dispatch_event(void);
void cdecl flush_event_queue(LONG argcnt, LONG owner, LONG event, LONG parameter);
void cdecl flush_input_events(void);

void cdecl init_interface(void);
void cdecl shutdown_interface(void);
void cdecl set_mouse_pointer(LONG argcnt, ULONG table, ULONG number, LONG hot_X,
	LONG hot_Y, ULONG scale, ULONG fade_table, ULONG fade_level);
void cdecl set_wait_pointer(LONG argcnt, ULONG number, LONG hot_X, LONG hot_Y);
void cdecl standby_cursor(void);
void cdecl resume_cursor(void);
void cdecl show_mouse(void);
void cdecl hide_mouse(void);
ULONG cdecl mouse_XY(void);
ULONG cdecl mouse_in_window(LONG argcnt, ULONG wnd);
void cdecl lock_mouse(void);
void cdecl unlock_mouse(void);
void cdecl getkey(void);

void cdecl init_graphics(void);
void cdecl draw_dot(LONG argcnt, ULONG page, ULONG x, ULONG y, ULONG color);
void cdecl draw_line(LONG argcnt, ULONG page,
	ULONG x1, ULONG y1, ULONG x2, ULONG y2, ULONG color);
void cdecl line_to(LONG argcnt, ULONG x, ULONG y, ULONG color, ...);
void cdecl draw_rectangle(LONG argcnt, ULONG wndnum, LONG x1, LONG y1, LONG x2,
	LONG y2, ULONG color);
void cdecl fill_rectangle(LONG argcnt, ULONG wndnum, LONG x1, LONG y1, LONG x2,
	LONG y2, ULONG color);
void cdecl hash_rectangle(LONG argcnt, ULONG wndnum, LONG x1, LONG y1, LONG x2,
	LONG y2, ULONG color);
ULONG cdecl get_bitmap_height(LONG argcnt, ULONG table, ULONG number);
void cdecl draw_bitmap(LONG argcnt, ULONG page, ULONG table, ULONG number,
	LONG x, LONG y, ULONG scale, ULONG flip, ULONG fade_table, ULONG
	fade_level);
ULONG cdecl visible_bitmap_rect(LONG argcnt, LONG x, LONG y,
	ULONG flip, ULONG table, ULONG number, WORD *array);
void cdecl set_palette(LONG argcnt, ULONG region, ULONG resource);
void cdecl refresh_window(LONG argcnt, ULONG src, ULONG target);
void cdecl wipe_window(LONG argcnt, ULONG window, ULONG color);
void cdecl shutdown_graphics(void);
void cdecl wait_vertical_retrace(void);
ULONG cdecl read_palette(LONG argcnt, ULONG regnum);
void cdecl write_palette(LONG argcnt, ULONG regnum, ULONG value);
void cdecl pixel_fade(LONG argcnt, ULONG src_wnd, ULONG dest_wnd, ULONG intervals);
void cdecl color_fade(LONG argcnt, ULONG src_wnd, ULONG dest_wnd);
void cdecl light_fade(LONG argcnt, ULONG src_wnd, ULONG color);

ULONG cdecl assign_window(LONG argcnt, ULONG owner, ULONG x1, ULONG y1,
	ULONG x2, ULONG y2);
ULONG cdecl assign_subwindow(LONG argcnt, ULONG owner, ULONG parent,
	ULONG x1, ULONG y1, ULONG x2, ULONG y2);
void cdecl release_window(LONG argcnt, ULONG window);
ULONG cdecl get_x1(LONG argcnt, ULONG window);
ULONG cdecl get_x2(LONG argcnt, ULONG window);
ULONG cdecl get_y1(LONG argcnt, ULONG window);
ULONG cdecl get_y2(LONG argcnt, ULONG window);
void cdecl set_x1(LONG argcnt, ULONG window, ULONG x1);
void cdecl set_x2(LONG argcnt, ULONG window, ULONG x2);
void cdecl set_y1(LONG argcnt, ULONG window, ULONG y1);
void cdecl set_y2(LONG argcnt, ULONG window, ULONG y2);

void cdecl text_window(LONG argcnt, ULONG wndnum, ULONG wnd);
void cdecl text_style(LONG argcnt, ULONG wndnum, ULONG font, ULONG
	justify);
void cdecl text_xy(LONG argcnt, ULONG wndnum, ULONG htab, ULONG vtab);
void cdecl text_color(LONG argcnt, ULONG wndnum, ULONG current, ULONG new);
void cdecl text_refresh_window(LONG argcnt, ULONG wndnum, LONG wnd);

LONG cdecl get_text_x(LONG argcnt, ULONG wndnum);
LONG cdecl get_text_y(LONG argcnt, ULONG wndnum);

void cdecl home(LONG argcnt, ULONG wndnum);

void cdecl print(LONG argcnt, ULONG wndnum, ULONG format, ...);
void cdecl sprint(LONG argcnt, ULONG wndnum, BYTE *format, ...);
void cdecl dprint(LONG argcnt, const char *format, ...);
void cdecl aprint(LONG argcnt, const char *format, ...);
void cdecl crout(LONG argcnt, ULONG wndnum);
ULONG cdecl char_width(LONG argcnt, ULONG wndnum, ULONG ch);
ULONG cdecl font_height(LONG argcnt, ULONG wndnum);

void cdecl solid_bar_graph(LONG argcnt, LONG x0, LONG y0, LONG x1, LONG y1,
	ULONG lb_border, ULONG tr_border, ULONG bkgnd, ULONG grn, ULONG yel,
	ULONG red, LONG val, LONG min, LONG crit, LONG max);

void cdecl init_sound(LONG argcnt, ULONG errprompt);
void cdecl shutdown_sound(void);
void cdecl load_sound_block(LONG argcnt, ULONG first_block, ULONG last_block,
	ULONG *array);
void cdecl sound_effect(LONG argcnt, ULONG index);
void cdecl play_sequence(LONG argcnt, ULONG LA_version, ULONG AD_version, ULONG
	PC_version);
void cdecl load_music(void);
void cdecl unload_music(void);
void cdecl set_sound_status(LONG argcnt, ULONG status);

LONG cdecl create_object(LONG argcnt, ULONG name);
LONG cdecl create_program(LONG argcnt, LONG index, ULONG name);
LONG cdecl destroy_object(LONG argcnt, LONG index);
void cdecl thrash_cache(void);
ULONG cdecl flush_cache(LONG argcnt, ULONG goal);

LONG cdecl step_X(LONG argcnt, ULONG x, ULONG fdir, ULONG mtype, ULONG distance);
LONG cdecl step_Y(LONG argcnt, ULONG y, ULONG fdir, ULONG mtype, ULONG distance);
ULONG cdecl step_FDIR(LONG argcnt, ULONG fdir, ULONG mtype);

LONG cdecl step_square_X(LONG argcnt, ULONG x, ULONG r, ULONG dir);
LONG cdecl step_square_Y(LONG argcnt, ULONG y, ULONG r, ULONG dir);
LONG cdecl step_region(LONG argcnt, ULONG r, ULONG dir);

ULONG cdecl distance(LONG argcnt, ULONG x1, ULONG y1, ULONG x2, ULONG y2);
ULONG cdecl seek_direction(LONG argcnt, ULONG obj_x, ULONG obj_y, ULONG
	dest_x, ULONG dest_y);

ULONG cdecl spell_request(LONG argcnt, BYTE *stat, BYTE *cnt, ULONG typ, ULONG num);
ULONG cdecl spell_list(LONG argcnt, BYTE *cnt, ULONG typ, ULONG lvl, BYTE *list,
	ULONG max);
void cdecl magic_field(LONG argcnt, ULONG p, ULONG redfield, ULONG yelfield,
	LONG sparkle);
void cdecl do_dots(LONG argcnt, LONG view, LONG scrn, LONG exp_x, LONG exp_y,
	LONG scale, LONG power, LONG dots, LONG life, LONG upval, BYTE *colors);
void cdecl do_ice(LONG argcnt, LONG view, LONG scrn, LONG dots, LONG mag,
	LONG grav, LONG life, LONG colors);

void cdecl read_save_directory(void);
BYTE *cdecl savegame_title(LONG argcnt, ULONG num);
void cdecl write_save_directory(void);

ULONG cdecl save_game(LONG argcnt, ULONG slotnum, ULONG lvlnum);
void cdecl suspend_game(LONG argcnt, ULONG cur_lvl);
void cdecl resume_items(LONG argcnt, ULONG first, ULONG last, ULONG restoring);
void cdecl resume_level(LONG argcnt, ULONG cur_lvl);
void cdecl change_level(LONG argcnt, ULONG old_lvl, ULONG new_lvl);
void cdecl restore_items(LONG argcnt, ULONG slotnum);
void cdecl restore_level_objects(LONG argcnt, ULONG slotnum, ULONG lvlnum);
void cdecl read_initial_items(void);
void cdecl write_initial_tempfiles(void);
void cdecl create_initial_binary_files(void);
void cdecl launch(LONG argcnt, BYTE *dirname, BYTE *prgname, BYTE *argn1,
	BYTE *argn2);

void *cdecl open_transfer_file(LONG argcnt, BYTE *filename);
void cdecl close_transfer_file(void);
LONG cdecl player_attrib(LONG argcnt, ULONG plrnum, ULONG offset, ULONG size);
LONG cdecl item_attrib(LONG argcnt, ULONG plrnum, ULONG invslot, ULONG attrib);
LONG cdecl arrow_count(LONG argcnt, ULONG plrnum);

//typedef void (cdecl *FARPROC)();
#define code_resources FARPROC code_resources[] =
#define PROCDEF (FARPROC)

} // namespace Aesop

#endif   

#ifdef __AESOP__

namespace Aesop {

//===========================================
//==                                       ==
//== AESOP resource attribute declarations ==
//==                                       ==
//===========================================

attrib sequence fixed,precious
attrib sample fixed,precious
attrib string moveable,discardable
attrib source moveable,discardable
attrib document temporary
attrib map temporary
attrib palette moveable,discardable
attrib file moveable,discardable

} // namespace Aesop

#endif

//========================================
//==                                    ==
//== AESOP/C code resource declarations ==
//==                                    ==
//========================================

namespace Aesop {

code_resources
{
   //
   // Miscellaneous functions
   //

   PROCDEF load_string,
   PROCDEF load_resource,
   PROCDEF copy_string,
   PROCDEF string_force_lower,
   PROCDEF string_force_upper,
   PROCDEF string_len,
   PROCDEF string_compare,
   PROCDEF strval,
   PROCDEF envval,
   PROCDEF beep,
   PROCDEF pokemem,
   PROCDEF peekmem,
   PROCDEF rnd,
   PROCDEF dice,
   PROCDEF absv,
   PROCDEF minv,
   PROCDEF maxv,
   PROCDEF diagnose,
   PROCDEF heapfree,

   //
   // Event functions
   // 

   PROCDEF notify,
   PROCDEF cancel,
   PROCDEF drain_event_queue,
   PROCDEF post_event,
   PROCDEF send_event,
   PROCDEF peek_event,
   PROCDEF dispatch_event,
   PROCDEF flush_event_queue,
   PROCDEF flush_input_events,

   //
   // Interface functions
   //

   PROCDEF init_interface,
   PROCDEF shutdown_interface,
   PROCDEF set_mouse_pointer,
   PROCDEF set_wait_pointer,
   PROCDEF standby_cursor,
   PROCDEF resume_cursor,
   PROCDEF show_mouse,
   PROCDEF hide_mouse,
   PROCDEF mouse_XY,
   PROCDEF mouse_in_window,
   PROCDEF lock_mouse,
   PROCDEF unlock_mouse,
   PROCDEF getkey,

   //
   // Graphics-related functions
   //

   PROCDEF init_graphics,
   PROCDEF draw_dot,
   PROCDEF draw_line,
   PROCDEF line_to,
   PROCDEF draw_rectangle,
   PROCDEF fill_rectangle,
   PROCDEF hash_rectangle,
   PROCDEF get_bitmap_height,
   PROCDEF draw_bitmap,
   PROCDEF visible_bitmap_rect,
   PROCDEF set_palette,
   PROCDEF refresh_window,
   PROCDEF wipe_window,
   PROCDEF shutdown_graphics,
   PROCDEF wait_vertical_retrace,
   PROCDEF read_palette,
   PROCDEF write_palette,
   PROCDEF pixel_fade,
   PROCDEF color_fade,
   PROCDEF light_fade,

   PROCDEF assign_window,
   PROCDEF assign_subwindow,
   PROCDEF release_window,
   PROCDEF get_x1,
   PROCDEF get_x2,
   PROCDEF get_y1,
   PROCDEF get_y2,
   PROCDEF set_x1,
   PROCDEF set_x2,
   PROCDEF set_y1,
   PROCDEF set_y2,

   PROCDEF text_window,
   PROCDEF text_style,
   PROCDEF text_xy,
   PROCDEF text_color,
   PROCDEF text_refresh_window,
   PROCDEF get_text_x,
   PROCDEF get_text_y,
   PROCDEF home,
   PROCDEF print,
   PROCDEF sprint,
   PROCDEF dprint,
   PROCDEF aprint,
   PROCDEF crout,
   PROCDEF char_width,
   PROCDEF font_height,

   PROCDEF solid_bar_graph,

//   PROCDEF mono_on,
//   PROCDEF mono_off,

   //
   // Sound-related functions
   //

   PROCDEF init_sound,
   PROCDEF shutdown_sound,
   PROCDEF load_sound_block,
   PROCDEF sound_effect,
   PROCDEF play_sequence,
   PROCDEF load_music,
   PROCDEF unload_music,
   PROCDEF set_sound_status,

   //
   // Eye III object management
   //

   PROCDEF create_object,
   PROCDEF create_program,
   PROCDEF destroy_object,
   PROCDEF flush_cache,
   PROCDEF thrash_cache,

   //
   // Eye III support functions
   //

   PROCDEF step_X,
   PROCDEF step_Y,
   PROCDEF step_FDIR,

   PROCDEF step_square_X,
   PROCDEF step_square_Y,
   PROCDEF step_region,

   PROCDEF distance,
   PROCDEF seek_direction,

   PROCDEF spell_request,
   PROCDEF spell_list,
   PROCDEF magic_field,
   PROCDEF do_dots,
   PROCDEF do_ice,

   PROCDEF read_save_directory,
   PROCDEF savegame_title,
   PROCDEF write_save_directory,

   PROCDEF save_game,
   PROCDEF suspend_game,
   PROCDEF resume_items,
   PROCDEF resume_level,
   PROCDEF change_level,
   PROCDEF restore_items,
   PROCDEF restore_level_objects,
   PROCDEF read_initial_items,
   PROCDEF write_initial_tempfiles,
   PROCDEF create_initial_binary_files,
   PROCDEF launch,

   //
   // Eye II savegame file access
   //

   PROCDEF open_transfer_file,
   PROCDEF close_transfer_file,
   PROCDEF player_attrib,
   PROCDEF item_attrib,
   PROCDEF arrow_count,
};

} // namespace Aesop
