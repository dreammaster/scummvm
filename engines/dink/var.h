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
extern int GetKeyboard(int key);
extern void changedir(int dir1, int k, int base);
extern void kill_sprite_all(int sprite);
extern bool playing(int sound);
extern int getpic(int h);
extern byte get_hard(int h, int x1, int y1);
extern void update_screen_time();
extern void load_map(int num);
extern void kill_returning_stuff(int script);
extern void kill_text_owned_by(int sprite);
extern int say_text(const char *text, int h, int script);
extern bool PauseMidi();
extern bool ResumeMidi();
extern byte get_hard_play(int h, int x1, int y1);
extern void draw_sprite_game(LPDIRECTDRAWSURFACE lpdest, int h);
extern int say_text_xy(const char *text, int mx, int my, int script);
extern bool add_time_to_saved_game(int num);
extern void log_path(bool playing);
extern void kill_all_scripts_for_real();
extern bool seperate_string(const char *str, int num, char liney, char *return1);
extern void finiObjects();
extern void check_midi();

extern void init_scripts();
extern int load_script(const char *filename, int sprite, bool set_sprite);
extern void strchar(char *string, char ch);
extern uint32 PlayCD(HWND hWndNotify, byte bTrack);
extern uint32 killcd(HWND hWndNotify, byte bTrack);
extern void update_status_all();
extern int add_sprite(int x1, int y, int brain, int pseq, int pframe);

extern void load_info(); //redink1
extern void add_exp(int num, int h, bool addEvenIfNotLastSpriteHit = false);
extern bool locate(int script, const char *proc);
extern bool SoundStopEffect(int sound);
extern void draw_status_all();
extern bool SoundDestroyEffect(int sound);
extern int SoundPlayEffect(int sound, int min, int plus, int sound3d, bool repeat);
extern void SoundLoadBanks();
extern bool StopMidi();
extern void check_seq_status(int h);
extern bool PlayMidi(char *sFileName);
extern int process_line(int script, char *s, bool doelse);

extern void get_word(char line[300], int word, char *crap);
extern void Msg(const char *fmt, ...);
extern void run_script(int script);
extern void add_text(const char *tex, const char *filename);
extern void program_idata();
extern void draw_map_game();
extern void draw_map_game_background();
extern int realhard(int tile);
extern void kill_repeat_sounds_all();
//redink1 added this prototype to fix this save info declaration error thing
extern void decipher_string(char line[200], int script);
//redink1 added for recursive scope checking
extern int get_var(int script, char *name);
extern void kill_fonts();

} // namespace Dink

#endif
