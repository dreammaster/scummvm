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

extern bool please_wait;
extern int  show_dot;
extern int  plane_process;
extern hit_map hm;

extern HWND     g_hWnd;
extern sprite_index index[max_sequences];
extern int last_sprite_added;
extern uint32 timer;
extern const char *command_line;
extern bool dinkedit;
extern int base_timing;
extern int weapon_script;
extern int magic_script;

extern int sp_mode;
extern int fps,fps_final;
extern int last_sprite_created;
extern int move_screen;
extern bool move_gonna;
extern int move_counter;
extern int m1x,m1y;
extern int m2x,m2y;
extern int m3x,m3y;
extern int playx;
extern bool windowed;
extern int playl;
extern HINSTANCE MyhInstance;
extern bool mouse1;
extern int cur_sprite;
extern int playy; //redink1's fix for the 'no sprites on pixel line above status bar' bug
extern int cur_map,cur_tile;
extern seth_joy sjoy;
extern int32               thisTickCount,lastTickCount; 
extern uint32 timecrap;
extern Common::Rect math,box_crap,box_real;

extern HRESULT             ddrval;
extern int sz,sy,x_offset,y_offset;
extern DDBLTFX     ddbltfx;

extern int mode;

extern sequence seq[max_sequences]; 
extern map_info map;
extern small_map pam;
extern LPDIRECTDRAW            lpDD;           // DirectDraw object
//LPDIRECTDRAWSURFACE     lpDDSOne;       // Offscreen surface 1
extern LPDIRECTDRAWSURFACE     lpDDSTwo;       // Offscreen surface 2
extern LPDIRECTDRAWSURFACE     lpDDSTrick;       // Offscreen surface 2
extern LPDIRECTDRAWSURFACE     lpDDSTrick2;       // Offscreen surface 2
extern LPDIRECTDRAWSURFACE     lpDDSFade;       //redink1 surface for true-color fade-downs

extern bool trig_man;
extern bool total_trigger;
extern bool debug_mode;

extern pic_info     picInfo[max_sprites];       // Sprite data
extern player_info play;

extern LPDIRECTDRAWSURFACE     lpDDSPrimary;   // DirectDraw primary surface
extern LPDIRECTDRAWSURFACE     lpDDSBack;      // DirectDraw back surface

extern LPDIRECTDRAWSURFACE     tiles[tile_screens];       // Game pieces
extern LPDIRECTDRAWSURFACE     game[max_game];       // Game pieces

extern sp spr[max_sprites_at_once]; //max sprite control systems at once

extern LPDIRECTDRAWPALETTE lpDDPal;        // The primary surface palette
extern PALETTEENTRY pe[256];
extern PALETTEENTRY real_pal[256];

extern bool bActive;        // is application active?
//LPDIRECTINPUT lpDI;


//direct input stuff for mouse reading

extern LPDIRECTINPUT       g_pdi;
extern LPDIRECTINPUTDEVICE g_pMouse;
extern HANDLE              g_hevtMouse;


//LPCDIDATAFORMAT lpc;

extern byte torusColors[256];  // Marks the colors used in the torus

extern HWND hWndMain;
extern JOYINFOEX jinfo;
extern bool joystick;
extern bool disablejoystick;
extern hardness hmap;
extern Common::Rect tilerect[tile_screens];

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
