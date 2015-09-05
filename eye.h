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

#ifndef AESOP_EYE_H
#define AESOP_EYE_H

namespace Aesop {

extern void load_string(LONG argcnt, BYTE *array, ULONG string);
extern void load_resource(LONG argcnt, BYTE *array, ULONG resource);
extern void copy_string(LONG argcnt, BYTE *src, BYTE *dest);
extern void string_force_lower(LONG argcnt, BYTE *dest);
extern void string_force_upper(LONG argcnt, BYTE *dest);
extern ULONG string_len(LONG argcnt, BYTE *string);
extern ULONG string_compare(LONG argcnt, BYTE *str1, BYTE *str2);
extern void beep(void);
extern LONG strval(LONG argcnt, BYTE *string);
extern LONG envval(LONG argcnt, BYTE *name);
extern void pokemem(LONG argcnt, LONG *addr, LONG data);
extern LONG peekmem(LONG argcnt, LONG *addr);
extern ULONG rnd(LONG argcnt, ULONG low, ULONG high);
extern ULONG dice(LONG argcnt, ULONG ndice, ULONG nsides, ULONG bonus);
extern ULONG absv(LONG argcnt, LONG val);
extern LONG minv(LONG argcnt, LONG val1, LONG val2);
extern LONG maxv(LONG argcnt, LONG val1, LONG val2);
extern void diagnose(LONG argcnt, ULONG dtype, ULONG parm);
extern ULONG heapfree(void);

extern void notify(LONG argcnt, ULONG index, ULONG message, LONG event,
   LONG parameter);
extern void cancel(LONG argcnt, ULONG index, ULONG message, LONG event, 
   LONG parameter);
extern void drain_event_queue(void);
extern void post_event(LONG argcnt, ULONG owner, LONG event, LONG parameter);
extern void send_event(LONG argcnt, ULONG owner, LONG event, LONG parameter);
extern ULONG peek_event(void);
extern void dispatch_event(void);
extern void flush_event_queue(LONG argcnt, LONG owner, LONG event, LONG parameter);
extern void flush_input_events(void);

extern void init_interface(void);
extern void shutdown_interface(void);
extern void set_mouse_pointer(LONG argcnt, ULONG table, ULONG number, LONG hot_X,
   LONG hot_Y, ULONG scale, ULONG fade_table, ULONG fade_level);
extern void set_wait_pointer(LONG argcnt, ULONG number, LONG hot_X, LONG hot_Y);
extern void standby_cursor(void);
extern void resume_cursor(void);
extern void show_mouse(void);
extern void hide_mouse(void);
extern ULONG mouse_XY(void);
extern ULONG mouse_in_window(LONG argcnt, ULONG wnd);
extern void lock_mouse(void);
extern void unlock_mouse(void);
extern void getkey(void);

extern void init_graphics(void);
extern void draw_dot(LONG argcnt, ULONG page, ULONG x, ULONG y, ULONG color);
extern void draw_line(LONG argcnt, ULONG page,
   ULONG x1, ULONG y1, ULONG x2, ULONG y2, ULONG color);
extern void draw_rectangle(LONG argcnt, ULONG wndnum, LONG x1, LONG y1, LONG x2,
   LONG y2, ULONG color);
extern void fill_rectangle(LONG argcnt, ULONG wndnum, LONG x1, LONG y1, LONG x2,
   LONG y2, ULONG color);
extern void hash_rectangle(LONG argcnt, ULONG wndnum, LONG x1, LONG y1, LONG x2,
   LONG y2, ULONG color);
extern ULONG get_bitmap_height(LONG argcnt, ULONG table, ULONG number);
extern void draw_bitmap(LONG argcnt, ULONG page, ULONG table, ULONG number,
   LONG x, LONG y, ULONG scale, ULONG flip, ULONG fade_table, ULONG
   fade_level);
extern ULONG visible_bitmap_rect(LONG argcnt, LONG x, LONG y,
   ULONG flip, ULONG table, ULONG number, WORD *array);
extern void set_palette(LONG argcnt, ULONG region, ULONG resource);
extern void refresh_window(LONG argcnt, ULONG src, ULONG target);
extern void wipe_window(LONG argcnt, ULONG window, ULONG color);
extern void shutdown_graphics(void);
extern void wait_vertical_retrace(void);
extern ULONG read_palette(LONG argcnt, ULONG regnum);
extern void write_palette(LONG argcnt, ULONG regnum, ULONG value);
extern void pixel_fade(LONG argcnt, ULONG src_wnd, ULONG dest_wnd, ULONG intervals);
extern void color_fade(LONG argcnt, ULONG src_wnd, ULONG dest_wnd);
extern void light_fade(LONG argcnt, ULONG src_wnd, ULONG color);

extern ULONG assign_window(LONG argcnt, ULONG owner, ULONG x1, ULONG y1,
   ULONG x2, ULONG y2);
extern ULONG assign_subwindow(LONG argcnt, ULONG owner, ULONG parent,
   ULONG x1, ULONG y1, ULONG x2, ULONG y2);
extern void release_window(LONG argcnt, ULONG window);
extern ULONG get_x1(LONG argcnt, ULONG window);
extern ULONG get_x2(LONG argcnt, ULONG window);
extern ULONG get_y1(LONG argcnt, ULONG window);
extern ULONG get_y2(LONG argcnt, ULONG window);
extern void set_x1(LONG argcnt, ULONG window, ULONG x1);
extern void set_x2(LONG argcnt, ULONG window, ULONG x2);
extern void set_y1(LONG argcnt, ULONG window, ULONG y1);
extern void set_y2(LONG argcnt, ULONG window, ULONG y2);

extern void text_window(LONG argcnt, ULONG wndnum, ULONG wnd);
extern void text_style(LONG argcnt, ULONG wndnum, ULONG font, ULONG
   justify);
extern void text_xy(LONG argcnt, ULONG wndnum, ULONG htab, ULONG vtab);
extern void text_color(LONG argcnt, ULONG wndnum, ULONG current, ULONG newColor);
extern void text_refresh_window(LONG argcnt, ULONG wndnum, LONG wnd);

extern LONG get_text_x(LONG argcnt, ULONG wndnum);
extern LONG get_text_y(LONG argcnt, ULONG wndnum);

extern void home(LONG argcnt, ULONG wndnum);

extern void crout(LONG argcnt, ULONG wndnum);
extern ULONG char_width(LONG argcnt, ULONG wndnum, ULONG ch);
extern ULONG font_height(LONG argcnt, ULONG wndnum);

extern void solid_bar_graph(LONG argcnt, LONG x0, LONG y0, LONG x1, LONG y1,
   ULONG lb_border, ULONG tr_border, ULONG bkgnd, ULONG grn, ULONG yel,
   ULONG red, LONG val, LONG min, LONG crit, LONG max);

extern void init_sound(LONG argcnt, ULONG errprompt);
extern void shutdown_sound(void);
extern void load_sound_block(LONG argcnt, ULONG first_block, ULONG last_block,
   ULONG *array);
extern void sound_effect(LONG argcnt, ULONG index);
extern void play_sequence(LONG argcnt, ULONG LA_version, ULONG AD_version, ULONG
   PC_version);
extern void load_music(void);
extern void unload_music(void);
extern void set_sound_status(LONG argcnt, ULONG status);

extern LONG create_object(LONG argcnt, ULONG name);
extern LONG create_program(LONG argcnt, LONG index, ULONG name);
extern LONG destroy_object(LONG argcnt, LONG index);
extern void thrash_cache(void);
extern ULONG flush_cache(LONG argcnt, ULONG goal);

extern LONG step_X(LONG argcnt, ULONG x, ULONG fdir, ULONG mtype, ULONG distance);
extern LONG step_Y(LONG argcnt, ULONG y, ULONG fdir, ULONG mtype, ULONG distance);
extern ULONG step_FDIR(LONG argcnt, ULONG fdir, ULONG mtype);

extern LONG step_square_X(LONG argcnt, ULONG x, ULONG r, ULONG dir);
extern LONG step_square_Y(LONG argcnt, ULONG y, ULONG r, ULONG dir);
extern LONG step_region(LONG argcnt, ULONG r, ULONG dir);

extern ULONG distance(LONG argcnt, ULONG x1, ULONG y1, ULONG x2, ULONG y2);
extern ULONG seek_direction(LONG argcnt, ULONG obj_x, ULONG obj_y, ULONG
   dest_x, ULONG dest_y);

extern ULONG spell_request(LONG argcnt, BYTE *stat, BYTE *cnt, ULONG typ, ULONG num);
extern ULONG spell_list(LONG argcnt, BYTE *cnt, ULONG typ, ULONG lvl, BYTE *list,
   ULONG max);
extern void magic_field(LONG argcnt, ULONG p, ULONG redfield, ULONG yelfield,
   LONG sparkle);
extern void do_dots(LONG argcnt, LONG view, LONG scrn, LONG exp_x, LONG exp_y,
   LONG scale, LONG power, LONG dots, LONG life, LONG upval, BYTE *colors);
extern void do_ice(LONG argcnt, LONG view, LONG scrn, LONG dots, LONG mag, 
   LONG grav, LONG life, BYTE *colors);

extern void read_save_directory(void);
extern char *savegame_title(LONG argcnt, ULONG num);
extern void write_save_directory(void);

extern ULONG save_game(LONG argcnt, ULONG slotnum, ULONG lvlnum);
extern void suspend_game(LONG argcnt, ULONG cur_lvl);
extern void resume_items(LONG argcnt, ULONG first, ULONG last, ULONG restoring);
extern void resume_level(LONG argcnt, ULONG cur_lvl);
extern void change_level(LONG argcnt, ULONG old_lvl, ULONG new_lvl);
extern void restore_items(LONG argcnt, ULONG slotnum);
extern void restore_level_objects(LONG argcnt, ULONG slotnum, ULONG lvlnum);
extern void read_initial_items(void);
extern void write_initial_tempfiles(void);
extern void create_initial_binary_files(void);
extern void launch(LONG argcnt, const char *dirname, const char *prgname, const char *argn1,
	const char *argn2);

extern LONG player_attrib(LONG argcnt, ULONG plrnum, ULONG offset, ULONG size);
extern LONG item_attrib(LONG argcnt, ULONG plrnum, ULONG invslot, ULONG attrib);
extern LONG arrow_count(LONG argcnt, ULONG plrnum);
extern void *open_transfer_file(LONG argcnt, const char *filename);
extern void close_transfer_file();

#ifdef __AESOP__

// AESOP resource attribute declarations

attrib sequence fixed,precious
attrib sample fixed,precious
attrib string moveable,discardable
attrib source moveable,discardable
attrib document temporary
attrib map temporary
attrib palette moveable,discardable
attrib file moveable,discardable

#endif

} // End of namespace Aesop

#endif

