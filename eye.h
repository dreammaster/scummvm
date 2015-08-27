/* RogueVM - RPG Engines
 *
 * RogueVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef AESOP_EYE
#define AESOP_EYE

namespace Aesop {

#ifdef __AESOP__

#define PROCDEF

#else

void load_string(LONG argcnt, BYTE *array, ULONG string);
void load_resource(LONG argcnt, BYTE *array, ULONG resource);
void copy_string(LONG argcnt, BYTE *src, BYTE *dest);
void string_force_lower(LONG argcnt, BYTE *dest);
void string_force_upper(LONG argcnt, BYTE *dest);
ULONG string_len(LONG argcnt, BYTE *string);
ULONG string_compare(LONG argcnt, BYTE *str1, BYTE *str2);
void beep(void);
LONG strval(LONG argcnt, BYTE *string);
LONG envval(LONG argcnt, BYTE *name);
void pokemem(LONG argcnt, LONG *addr, LONG data);
LONG peekmem(LONG argcnt, LONG *addr);
ULONG rnd(LONG argcnt, ULONG low, ULONG high);
ULONG dice(LONG argcnt, ULONG ndice, ULONG nsides, ULONG bonus);
ULONG absv(LONG argcnt, LONG val);
LONG minv(LONG argcnt, LONG val1, LONG val2);
LONG maxv(LONG argcnt, LONG val1, LONG val2);
void diagnose(LONG argcnt, ULONG dtype, ULONG parm);
ULONG heapfree(void);

void notify(LONG argcnt, ULONG index, ULONG message, LONG event,
   LONG parameter);
void cancel(LONG argcnt, ULONG index, ULONG message, LONG event, 
   LONG parameter);
void drain_event_queue(void);
void post_event(LONG argcnt, ULONG owner, LONG event, LONG parameter);
void send_event(LONG argcnt, ULONG owner, LONG event, LONG parameter);
ULONG peek_event(void);
void dispatch_event(void);
void flush_event_queue(LONG argcnt, LONG owner, LONG event, LONG parameter);
void flush_input_events(void);

void init_interface(void);
void shutdown_interface(void);
void set_mouse_pointer(LONG argcnt, ULONG table, ULONG number, LONG hot_X,
   LONG hot_Y, ULONG scale, ULONG fade_table, ULONG fade_level);
void set_wait_pointer(LONG argcnt, ULONG number, LONG hot_X, LONG hot_Y);
void standby_cursor(void);
void resume_cursor(void);
void show_mouse(void);
void hide_mouse(void);
ULONG mouse_XY(void);
ULONG mouse_in_window(LONG argcnt, ULONG wnd);
void lock_mouse(void);
void unlock_mouse(void);
void getkey(void);

void init_graphics(void);
void draw_dot(LONG argcnt, ULONG page, ULONG x, ULONG y, ULONG color);
void draw_line(LONG argcnt, ULONG page,
   ULONG x1, ULONG y1, ULONG x2, ULONG y2, ULONG color);
void line_to(LONG argcnt, ULONG x, ULONG y, ULONG color, ...);
void draw_rectangle(LONG argcnt, ULONG wndnum, LONG x1, LONG y1, LONG x2,
   LONG y2, ULONG color);
void fill_rectangle(LONG argcnt, ULONG wndnum, LONG x1, LONG y1, LONG x2,
   LONG y2, ULONG color);
void hash_rectangle(LONG argcnt, ULONG wndnum, LONG x1, LONG y1, LONG x2,
   LONG y2, ULONG color);
ULONG get_bitmap_height(LONG argcnt, ULONG table, ULONG number);
void draw_bitmap(LONG argcnt, ULONG page, ULONG table, ULONG number,
   LONG x, LONG y, ULONG scale, ULONG flip, ULONG fade_table, ULONG
   fade_level);
ULONG visible_bitmap_rect(LONG argcnt, LONG x, LONG y, 
   ULONG flip, ULONG table, ULONG number, WORD *array);
void set_palette(LONG argcnt, ULONG region, ULONG resource);
void refresh_window(LONG argcnt, ULONG src, ULONG target);
void wipe_window(LONG argcnt, ULONG window, ULONG color);
void shutdown_graphics(void);
void wait_vertical_retrace(void);
ULONG read_palette(LONG argcnt, ULONG regnum);
void write_palette(LONG argcnt, ULONG regnum, ULONG value);
void pixel_fade(LONG argcnt, ULONG src_wnd, ULONG dest_wnd, ULONG intervals);
void color_fade(LONG argcnt, ULONG src_wnd, ULONG dest_wnd);
void light_fade(LONG argcnt, ULONG src_wnd, ULONG color);

ULONG assign_window(LONG argcnt, ULONG owner, ULONG x1, ULONG y1,
   ULONG x2, ULONG y2);
ULONG assign_subwindow(LONG argcnt, ULONG owner, ULONG parent,
   ULONG x1, ULONG y1, ULONG x2, ULONG y2);
void release_window(LONG argcnt, ULONG window);
ULONG get_x1(LONG argcnt, ULONG window);
ULONG get_x2(LONG argcnt, ULONG window);
ULONG get_y1(LONG argcnt, ULONG window);
ULONG get_y2(LONG argcnt, ULONG window);
void set_x1(LONG argcnt, ULONG window, ULONG x1);
void set_x2(LONG argcnt, ULONG window, ULONG x2);
void set_y1(LONG argcnt, ULONG window, ULONG y1);
void set_y2(LONG argcnt, ULONG window, ULONG y2);

void text_window(LONG argcnt, ULONG wndnum, ULONG wnd);
void text_style(LONG argcnt, ULONG wndnum, ULONG font, ULONG
   justify);
void text_xy(LONG argcnt, ULONG wndnum, ULONG htab, ULONG vtab);
void text_color(LONG argcnt, ULONG wndnum, ULONG current, ULONG newColor);
void text_refresh_window(LONG argcnt, ULONG wndnum, LONG wnd);

LONG get_text_x(LONG argcnt, ULONG wndnum);
LONG get_text_y(LONG argcnt, ULONG wndnum);

void home(LONG argcnt, ULONG wndnum);

void print(LONG argcnt, ULONG wndnum, ULONG format, ...);
void sprint(LONG argcnt, ULONG wndnum, BYTE *format, ...);
void dprint(LONG argcnt, BYTE *format, ...);
void aprint(LONG argcnt, BYTE *format, ...);
void crout(LONG argcnt, ULONG wndnum);
ULONG char_width(LONG argcnt, ULONG wndnum, ULONG ch);
ULONG font_height(LONG argcnt, ULONG wndnum);

void solid_bar_graph(LONG argcnt, LONG x0, LONG y0, LONG x1, LONG y1,
   ULONG lb_border, ULONG tr_border, ULONG bkgnd, ULONG grn, ULONG yel,
   ULONG red, LONG val, LONG min, LONG crit, LONG max);

void init_sound(LONG argcnt, ULONG errprompt);
void shutdown_sound(void);
void load_sound_block(LONG argcnt, ULONG first_block, ULONG last_block,
   ULONG *array);
void sound_effect(LONG argcnt, ULONG index);
void play_sequence(LONG argcnt, ULONG LA_version, ULONG AD_version, ULONG
   PC_version);
void load_music(void);
void unload_music(void);
void set_sound_status(LONG argcnt, ULONG status);

LONG create_object(LONG argcnt, ULONG name);
LONG create_program(LONG argcnt, LONG index, ULONG name);
LONG destroy_object(LONG argcnt, LONG index);
void thrash_cache(void);
ULONG flush_cache(LONG argcnt, ULONG goal);

LONG step_X(LONG argcnt, ULONG x, ULONG fdir, ULONG mtype, ULONG distance);
LONG step_Y(LONG argcnt, ULONG y, ULONG fdir, ULONG mtype, ULONG distance);
ULONG step_FDIR(LONG argcnt, ULONG fdir, ULONG mtype);

LONG step_square_X(LONG argcnt, ULONG x, ULONG r, ULONG dir);
LONG step_square_Y(LONG argcnt, ULONG y, ULONG r, ULONG dir);
LONG step_region(LONG argcnt, ULONG r, ULONG dir);

ULONG distance(LONG argcnt, ULONG x1, ULONG y1, ULONG x2, ULONG y2);
ULONG seek_direction(LONG argcnt, ULONG obj_x, ULONG obj_y, ULONG
   dest_x, ULONG dest_y);

ULONG spell_request(LONG argcnt, BYTE *stat, BYTE *cnt, ULONG typ, ULONG num);
ULONG spell_list(LONG argcnt, BYTE *cnt, ULONG typ, ULONG lvl, BYTE *list, 
   ULONG max);
void magic_field(LONG argcnt, ULONG p, ULONG redfield, ULONG yelfield,
   LONG sparkle);
void do_dots(LONG argcnt, LONG view, LONG scrn, LONG exp_x, LONG exp_y,
   LONG scale, LONG power, LONG dots, LONG life, LONG upval, BYTE *colors);
void do_ice(LONG argcnt, LONG view, LONG scrn, LONG dots, LONG mag, 
   LONG grav, LONG life, LONG colors);

void read_save_directory(void);
char *savegame_title(LONG argcnt, ULONG num);
void write_save_directory(void);

ULONG save_game(LONG argcnt, ULONG slotnum, ULONG lvlnum);
void suspend_game(LONG argcnt, ULONG cur_lvl);
void resume_items(LONG argcnt, ULONG first, ULONG last, ULONG restoring);
void resume_level(LONG argcnt, ULONG cur_lvl);
void change_level(LONG argcnt, ULONG old_lvl, ULONG new_lvl);
void restore_items(LONG argcnt, ULONG slotnum);
void restore_level_objects(LONG argcnt, ULONG slotnum, ULONG lvlnum);
void read_initial_items(void);
void write_initial_tempfiles(void);
void create_initial_binary_files(void);
void launch(LONG argcnt, const char *dirname, const char *prgname, const char *argn1,
	const char *argn2);

void *open_transfer_file(LONG argcnt, const char *filename);
void close_transfer_file(void);
LONG player_attrib(LONG argcnt, ULONG plrnum, ULONG offset, ULONG size);
LONG item_attrib(LONG argcnt, ULONG plrnum, ULONG invslot, ULONG attrib);
LONG arrow_count(LONG argcnt, ULONG plrnum);

//typedef void (*FARPROC)();
#define code_resources FARPROC code_resources[] =
#define PROCDEF (FARPROC)

#endif   

#ifdef __AESOP__

//ллллллллллллллллллллллллллллллллллллллллллл
//лл                                       лл
//лл AESOP resource attribute declarations лл
//лл                                       лл
//ллллллллллллллллллллллллллллллллллллллллллл

attrib sequence fixed,precious
attrib sample fixed,precious
attrib string moveable,discardable
attrib source moveable,discardable
attrib document temporary
attrib map temporary
attrib palette moveable,discardable
attrib file moveable,discardable

#endif

//лллллллллллллллллллллллллллллллллллллллл
//лл                                    лл
//лл AESOP/C code resource declarations лл
//лл                                    лл
//лллллллллллллллллллллллллллллллллллллллл

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

} // End of namespace Aesop

#endif
