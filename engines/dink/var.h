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

#ifndef DINK_VAR_H
#define DINK_VAR_H

#include "dink/globals.h"

namespace Dink {

extern void fill_screen(int num);
extern void changedir(int dir1, int k, int base);
extern void kill_sprite_all(int sprite);
extern int getpic(int h);
extern byte get_hard(int h, int x1, int y1);
extern void update_screen_time();
extern void load_map(int num);
extern void kill_returning_stuff(int script);
extern bool PauseMidi();
extern bool ResumeMidi();
extern byte get_hard_play(int h, int x1, int y1);
extern void draw_sprite_game(LPDIRECTDRAWSURFACE lpdest, int h);
extern bool add_time_to_saved_game(int num);
extern void kill_all_scripts_for_real();
extern bool seperate_string(const char *str, int num, char liney, char *return1);
extern void finiObjects();
extern void check_midi();

extern void init_scripts();
extern int load_script(const char *filename, int sprite, bool set_sprite);
extern void update_status_all();
extern int add_sprite(int x1, int y, int brain, int pseq, int pframe);

extern void load_info(); //redink1
extern void add_exp(int num, int h, bool addEvenIfNotLastSpriteHit = false);
extern bool locate(int script, const char *proc);
extern void draw_status_all();
extern int process_line(int script, char *s, bool doelse);

extern void run_script(int script);
extern void program_idata();
extern void draw_map_game();
extern void draw_map_game_background();
extern int realhard(int tile);
//redink1 added for recursive scope checking
extern int get_var(int script, char *name);
extern void kill_fonts();
extern bool nothing_playing();
extern void drawallhard();
extern void process_callbacks();
extern int hurt_thing(int h, int damage, int special);
extern void random_blood(int mx, int my, int h);
extern void load_hard();
extern IDirectDrawSurface *DDTileLoad(IDirectDraw *pdd, LPCSTR szBitmap, int dx, int dy, int sprite);
extern void attach();
//redink1 added for font colors
extern void init_font_colors();
extern void initfonts(const char *fontname);

} // namespace Dink

#endif
