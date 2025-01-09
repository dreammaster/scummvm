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

// Sample AESOP code resource/attribute header file                      ==

#ifndef AESOP_EYE_H
#define AESOP_EYE_H

#include "aesop/defs.h"

namespace Aesop {

void load_string(LONG argcnt, BYTE *array, ULONG string);
void load_resource(LONG argcnt, BYTE *array, ULONG resource);
void copy_string(LONG argcnt, BYTE *src, BYTE *dest);
void string_force_lower(LONG argcnt, BYTE *dest);
void string_force_upper(LONG argcnt, BYTE *dest);
ULONG string_len(LONG argcnt, BYTE *string);
ULONG string_compare(LONG argcnt, BYTE *str1, BYTE *str2);
void beep();
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
ULONG heapfree();

void notify(LONG argcnt, ULONG index, ULONG message, LONG event,
	LONG parameter);
void cancel(LONG argcnt, ULONG index, ULONG message, LONG event,
	LONG parameter);
void drain_event_queue();
void post_event(LONG argcnt, ULONG owner, LONG event, LONG parameter);
void send_event(LONG argcnt, ULONG owner, LONG event, LONG parameter);
ULONG peek_event();
void dispatch_event();
void flush_event_queue(LONG argcnt, LONG owner, LONG event, LONG parameter);
void flush_input_events();

void init_interface();
void shutdown_interface();
void set_mouse_pointer(LONG argcnt, ULONG table, ULONG number, LONG hot_X,
	LONG hot_Y, ULONG scale, ULONG fade_table, ULONG fade_level);
void set_wait_pointer(LONG argcnt, ULONG number, LONG hot_X, LONG hot_Y);
void standby_cursor();
void resume_cursor();
void show_mouse();
void hide_mouse();
ULONG mouse_XY();
ULONG mouse_in_window(LONG argcnt, ULONG wnd);
void lock_mouse();
void unlock_mouse();
void getkey();

void init_graphics();
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
void shutdown_graphics();
void wait_vertical_retrace();
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
void sprint(LONG argcnt, ULONG wndnum, const char *format, ...);
void dprint(LONG argcnt, const char *format, ...);
void aprint(LONG argcnt, const char *format, ...);
void crout(LONG argcnt, ULONG wndnum);
ULONG char_width(LONG argcnt, ULONG wndnum, ULONG ch);
ULONG font_height(LONG argcnt, ULONG wndnum);

void solid_bar_graph(LONG argcnt, LONG x0, LONG y0, LONG x1, LONG y1,
	ULONG lb_border, ULONG tr_border, ULONG bkgnd, ULONG grn, ULONG yel,
	ULONG red, LONG val, LONG min, LONG crit, LONG max);

void init_sound(LONG argcnt, ULONG errprompt);
void shutdown_sound();
void load_sound_block(LONG argcnt, ULONG first_block, ULONG last_block,
	ULONG *array);
void sound_effect(LONG argcnt, ULONG index);
void play_sequence(LONG argcnt, ULONG LA_version, ULONG AD_version, ULONG
	PC_version);
void load_music();
void unload_music();
void set_sound_status(LONG argcnt, ULONG status);

LONG create_object(LONG argcnt, ULONG name);
LONG create_program(LONG argcnt, LONG index, ULONG name);
LONG destroy_object(LONG argcnt, LONG index);
void thrash_cache();
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

void read_save_directory();
BYTE *savegame_title(LONG argcnt, ULONG num);
void write_save_directory();

ULONG save_game(LONG argcnt, ULONG slotnum, ULONG lvlnum);
void suspend_game(LONG argcnt, ULONG cur_lvl);
void resume_items(LONG argcnt, ULONG first, ULONG last, ULONG restoring);
void resume_level(LONG argcnt, ULONG cur_lvl);
void change_level(LONG argcnt, ULONG old_lvl, ULONG new_lvl);
void restore_items(LONG argcnt, ULONG slotnum);
void restore_level_objects(LONG argcnt, ULONG slotnum, ULONG lvlnum);
void read_initial_items();
void write_initial_tempfiles();
void create_initial_binary_files();
void launch(LONG argcnt, BYTE *dirname, BYTE *prgname, BYTE *argn1,
	BYTE *argn2);

void *open_transfer_file(LONG argcnt, BYTE *filename);
void close_transfer_file();
LONG player_attrib(LONG argcnt, ULONG plrnum, ULONG offset, ULONG size);
LONG item_attrib(LONG argcnt, ULONG plrnum, ULONG invslot, ULONG attrib);
LONG arrow_count(LONG argcnt, ULONG plrnum);

//typedef void (*FARPROC)();
typedef void (*FARPROC)();
#define code_resources FARPROC code_resources[] =
#define PROCDEF (FARPROC)

} // namespace Aesop

#endif
