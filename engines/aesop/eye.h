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

void load_string(int32 argcnt, BYTE *array, uint32 string);
void load_resource(int32 argcnt, BYTE *array, uint32 resource);
void copy_string(int32 argcnt, BYTE *src, BYTE *dest);
void string_force_lower(int32 argcnt, BYTE *dest);
void string_force_upper(int32 argcnt, BYTE *dest);
uint32 string_len(int32 argcnt, BYTE *string);
uint32 string_compare(int32 argcnt, BYTE *str1, BYTE *str2);
void beep(void);
int32 strval(int32 argcnt, BYTE *string);
int32 envval(int32 argcnt, BYTE *name);
void pokemem(int32 argcnt, int32 *addr, int32 data);
int32 peekmem(int32 argcnt, int32 *addr);
uint32 rnd(int32 argcnt, uint32 low, uint32 high);
uint32 dice(int32 argcnt, uint32 ndice, uint32 nsides, uint32 bonus);
uint32 absv(int32 argcnt, int32 val);
int32 minv(int32 argcnt, int32 val1, int32 val2);
int32 maxv(int32 argcnt, int32 val1, int32 val2);
void diagnose(int32 argcnt, uint32 dtype, uint32 parm);
uint32 heapfree(void);

void notify(int32 argcnt, uint32 index, uint32 message, int32 event,
	int32 parameter);
void cancel(int32 argcnt, uint32 index, uint32 message, int32 event,
	int32 parameter);
void drain_event_queue(void);
void post_event(int32 argcnt, uint32 owner, int32 event, int32 parameter);
void send_event(int32 argcnt, uint32 owner, int32 event, int32 parameter);
uint32 peek_event(void);
void dispatch_event(void);
void flush_event_queue(int32 argcnt, int32 owner, int32 event, int32 parameter);
void flush_input_events(void);

void init_interface(void);
void shutdown_interface(void);
void set_mouse_pointer(int32 argcnt, uint32 table, uint32 number, int32 hot_X,
	int32 hot_Y, uint32 scale, uint32 fade_table, uint32 fade_level);
void set_wait_pointer(int32 argcnt, uint32 number, int32 hot_X, int32 hot_Y);
void standby_cursor(void);
void resume_cursor(void);
void show_mouse(void);
void hide_mouse(void);
uint32 mouse_XY(void);
uint32 mouse_in_window(int32 argcnt, uint32 wnd);
void lock_mouse(void);
void unlock_mouse(void);
void getkey(void);

void init_graphics(void);
void draw_dot(int32 argcnt, uint32 page, uint32 x, uint32 y, uint32 color);
void draw_line(int32 argcnt, uint32 page,
	uint32 x1, uint32 y1, uint32 x2, uint32 y2, uint32 color);
void line_to(int32 argcnt, uint32 x, uint32 y, uint32 color, ...);
void draw_rectangle(int32 argcnt, uint32 wndnum, int32 x1, int32 y1, int32 x2,
	int32 y2, uint32 color);
void fill_rectangle(int32 argcnt, uint32 wndnum, int32 x1, int32 y1, int32 x2,
	int32 y2, uint32 color);
void hash_rectangle(int32 argcnt, uint32 wndnum, int32 x1, int32 y1, int32 x2,
	int32 y2, uint32 color);
uint32 get_bitmap_height(int32 argcnt, uint32 table, uint32 number);
void draw_bitmap(int32 argcnt, uint32 page, uint32 table, uint32 number,
	int32 x, int32 y, uint32 scale, uint32 flip, uint32 fade_table, uint32
	fade_level);
uint32 visible_bitmap_rect(int32 argcnt, int32 x, int32 y,
	uint32 flip, uint32 table, uint32 number, WORD *array);
void set_palette(int32 argcnt, uint32 region, uint32 resource);
void refresh_window(int32 argcnt, uint32 src, uint32 target);
void wipe_window(int32 argcnt, uint32 window, uint32 color);
void shutdown_graphics(void);
void wait_vertical_retrace(void);
uint32 read_palette(int32 argcnt, uint32 regnum);
void write_palette(int32 argcnt, uint32 regnum, uint32 value);
void pixel_fade(int32 argcnt, uint32 src_wnd, uint32 dest_wnd, uint32 intervals);
void color_fade(int32 argcnt, uint32 src_wnd, uint32 dest_wnd);
void light_fade(int32 argcnt, uint32 src_wnd, uint32 color);

uint32 assign_window(int32 argcnt, uint32 owner, uint32 x1, uint32 y1,
	uint32 x2, uint32 y2);
uint32 assign_subwindow(int32 argcnt, uint32 owner, uint32 parent,
	uint32 x1, uint32 y1, uint32 x2, uint32 y2);
void release_window(int32 argcnt, uint32 window);
uint32 get_x1(int32 argcnt, uint32 window);
uint32 get_x2(int32 argcnt, uint32 window);
uint32 get_y1(int32 argcnt, uint32 window);
uint32 get_y2(int32 argcnt, uint32 window);
void set_x1(int32 argcnt, uint32 window, uint32 x1);
void set_x2(int32 argcnt, uint32 window, uint32 x2);
void set_y1(int32 argcnt, uint32 window, uint32 y1);
void set_y2(int32 argcnt, uint32 window, uint32 y2);

void text_window(int32 argcnt, uint32 wndnum, uint32 wnd);
void text_style(int32 argcnt, uint32 wndnum, uint32 font, uint32
	justify);
void text_xy(int32 argcnt, uint32 wndnum, uint32 htab, uint32 vtab);
void text_color(int32 argcnt, uint32 wndnum, uint32 current, uint32 new);
void text_refresh_window(int32 argcnt, uint32 wndnum, int32 wnd);

int32 get_text_x(int32 argcnt, uint32 wndnum);
int32 get_text_y(int32 argcnt, uint32 wndnum);

void home(int32 argcnt, uint32 wndnum);

void print(int32 argcnt, uint32 wndnum, uint32 format, ...);
void sprint(int32 argcnt, uint32 wndnum, BYTE *format, ...);
void dprint(int32 argcnt, const char *format, ...);
void aprint(int32 argcnt, const char *format, ...);
void crout(int32 argcnt, uint32 wndnum);
uint32 char_width(int32 argcnt, uint32 wndnum, uint32 ch);
uint32 font_height(int32 argcnt, uint32 wndnum);

void solid_bar_graph(int32 argcnt, int32 x0, int32 y0, int32 x1, int32 y1,
	uint32 lb_border, uint32 tr_border, uint32 bkgnd, uint32 grn, uint32 yel,
	uint32 red, int32 val, int32 min, int32 crit, int32 max);

void init_sound(int32 argcnt, uint32 errprompt);
void shutdown_sound(void);
void load_sound_block(int32 argcnt, uint32 first_block, uint32 last_block,
	uint32 *array);
void sound_effect(int32 argcnt, uint32 index);
void play_sequence(int32 argcnt, uint32 LA_version, uint32 AD_version, uint32
	PC_version);
void load_music(void);
void unload_music(void);
void set_sound_status(int32 argcnt, uint32 status);

int32 create_object(int32 argcnt, uint32 name);
int32 create_program(int32 argcnt, int32 index, uint32 name);
int32 destroy_object(int32 argcnt, int32 index);
void thrash_cache(void);
uint32 flush_cache(int32 argcnt, uint32 goal);

int32 step_X(int32 argcnt, uint32 x, uint32 fdir, uint32 mtype, uint32 distance);
int32 step_Y(int32 argcnt, uint32 y, uint32 fdir, uint32 mtype, uint32 distance);
uint32 step_FDIR(int32 argcnt, uint32 fdir, uint32 mtype);

int32 step_square_X(int32 argcnt, uint32 x, uint32 r, uint32 dir);
int32 step_square_Y(int32 argcnt, uint32 y, uint32 r, uint32 dir);
int32 step_region(int32 argcnt, uint32 r, uint32 dir);

uint32 distance(int32 argcnt, uint32 x1, uint32 y1, uint32 x2, uint32 y2);
uint32 seek_direction(int32 argcnt, uint32 obj_x, uint32 obj_y, uint32
	dest_x, uint32 dest_y);

uint32 spell_request(int32 argcnt, BYTE *stat, BYTE *cnt, uint32 typ, uint32 num);
uint32 spell_list(int32 argcnt, BYTE *cnt, uint32 typ, uint32 lvl, BYTE *list,
	uint32 max);
void magic_field(int32 argcnt, uint32 p, uint32 redfield, uint32 yelfield,
	int32 sparkle);
void do_dots(int32 argcnt, int32 view, int32 scrn, int32 exp_x, int32 exp_y,
	int32 scale, int32 power, int32 dots, int32 life, int32 upval, BYTE *colors);
void do_ice(int32 argcnt, int32 view, int32 scrn, int32 dots, int32 mag,
	int32 grav, int32 life, int32 colors);

void read_save_directory(void);
BYTE *savegame_title(int32 argcnt, uint32 num);
void write_save_directory(void);

uint32 save_game(int32 argcnt, uint32 slotnum, uint32 lvlnum);
void suspend_game(int32 argcnt, uint32 cur_lvl);
void resume_items(int32 argcnt, uint32 first, uint32 last, uint32 restoring);
void resume_level(int32 argcnt, uint32 cur_lvl);
void change_level(int32 argcnt, uint32 old_lvl, uint32 new_lvl);
void restore_items(int32 argcnt, uint32 slotnum);
void restore_level_objects(int32 argcnt, uint32 slotnum, uint32 lvlnum);
void read_initial_items(void);
void write_initial_tempfiles(void);
void create_initial_binary_files(void);
void launch(int32 argcnt, BYTE *dirname, BYTE *prgname, BYTE *argn1,
	BYTE *argn2);

void *open_transfer_file(int32 argcnt, BYTE *filename);
void close_transfer_file(void);
int32 player_attrib(int32 argcnt, uint32 plrnum, uint32 offset, uint32 size);
int32 item_attrib(int32 argcnt, uint32 plrnum, uint32 invslot, uint32 attrib);
int32 arrow_count(int32 argcnt, uint32 plrnum);

//typedef void (*FARPROC)();
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
