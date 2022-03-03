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

#ifndef DINK_GLOBALS_H
#define DINK_GLOBALS_H

#include "common/hashmap.h"
#include "common/rect.h"
#include "common/str.h"
#include "dink/types.h"
#include "dink/directdraw/ddraw.h"
#include "dink/directdraw/dinput.h"
#include "dink/directdraw/dsound.h"

namespace Dink {

#ifndef __DEMO
#define __REAL_THING
#endif

//set language
#ifndef __LANGUAGE
#define __ENGLISH
#endif

#define DINPUT_BUFFERSIZE           16
#define ODS OutputDebugString

//redink1 Updated for v1.08
constexpr int dversion = 108;
constexpr int num_soundbanks = 20;
constexpr int old_burn = 0;
constexpr int max_idata = 1000; //redink1
constexpr int max_sounds = 100;
constexpr int max_vars = 250;
constexpr int text_timer = 77;
constexpr int text_min = 2700;
constexpr int max_sprites = 4000;
constexpr int max_sprites_at_once = 300;
constexpr int max_callbacks = 100;
constexpr int max_scripts = 200;
constexpr int max_sequences = 1000; //Max # of sprite animations
constexpr int max_game = 20;
constexpr int tile_screens = 41 + 1;


enum ColorDepth {
	ColorDepth16Bit_565,
	ColorDepth16Bit_555,
	ColorDepth24Bit,
	ColorDepth32Bit,
	ColorDepthNotRecognized
};

//redink1 added for font colors
struct font_color {
	int red;
	int green;
	int blue;
};

struct soundstruct {
	bool repeat;
	int owner;
	int survive;
	int vol;
};

struct show_bmp {
	bool active;
	bool showdot;
	int reserved;
	int script;
	int stime;
	int picframe;
};

struct refinfo {
	char name[10];
	long location;
	long current;
	int level;
	long end;
	int sprite; //if more than 0, it was spawned and is owned by a sprite, if 1000 doesn't die
	bool skipnext;
	int onlevel;
	int proc_return;
	int arg1;
	int arg2;
	int arg3;
	int arg4;
	int arg5;
	int arg6;
	int arg7;
	int arg8;
	int arg9;
};

struct call_back {
	int owner;
	bool active;
	int type;
	char name[20];
	int offset;
	long min, max;
	int lifespan;
	uint32 timer;
};

struct sequence {
	int frame[51];
	int delay[51];
	unsigned char special[51];
	char data[80];
	bool active;
};

struct sp {
	int x, moveman;
	int y;
	int mx, my;
	int lpx[51], lpy[51];
	int speed;
	int brain;
	int seq_orig, dir;
	int seq;
	int frame;
	uint32 delay;
	int pseq;
	int pframe;

	bool active;
	int attrib;
	uint32 wait;
	int timer;
	int skip;
	int skiptimer;
	int size;
	int que;
	int base_walk;
	int base_idle;
	int base_attack;

	int base_hit;
	int last_sound;
	int hard;
	Common::Rect alt;
	int althard;
	int sp_index;
	bool nocontrol;
	int idle;
	int strength;
	int damage;
	int defense;
	int hitpoints;
	int exp;
	int gold;
	int base_die;
	int kill;
	int kill_timer;
	int script_num;
	char text[200];
	int owner;
	int script;
	int sound;
	int callback;
	int freeze;
	bool move_active;
	int move_script;
	int move_dir;
	int move_num;
	bool move_nohard;
	int follow;
	int nohit;
	bool notouch;
	uint32 notouch_timer;
	bool flying;
	int touch_damage;
	int brain_parm;
	int brain_parm2;
	bool noclip;
	bool reverse;
	bool disabled;
	int target;
	int attack_wait;
	int move_wait;
	int distance;
	int last_hit;
	bool live;
	int range;
	int attack_hit_sound;
	int attack_hit_sound_speed;
	int action;
	int nodraw;
	int frame_delay;
	int picfreeze;
	//redink1
	int bloodseq;
	int bloodnum;
	Common::HashMap<Common::String, int> *custom;
};

struct seth_joy {
	bool joybit[17]; //is button held down?
	bool letgo[17]; //copy of old above
	bool button[17]; //has button been pressed recently?
	bool key[256];
	bool kletgo[256];
	bool realkey[256];
	bool right, left, up, down;
	bool rightd, leftd, upd, downd;
	bool rightold, leftold, upold, downold;
};

struct mega_y {
	byte y[401] = { 0 };

	void clear() {
		Common::fill(y, y + 401, 0);
	}
};

//struct for hardness map
struct hit_map {
	mega_y x[601];

	hit_map() {
		clear();
	}
	void clear() {
		for (int i = 0; i < 601; ++i)
			x[i].clear();
	}
};

//sub struct for tile hardness
struct block_y {
	byte y[51];
};

struct ts_block {
	block_y x[51];
	bool used;
	int hold;
};

//struct for hardness info, INDEX controls which hardness each block has.
// 800 max types available.
struct hardness {
	ts_block tile[800];
	int index[8000];

};

struct map_info {
	char name[20];
	int loc[769];
	int music[769];
	int indoor[769];
	int v[40];
	char s[80];
	char buffer[2000];

};

struct tile {
	int num, property, althard, more2;
	byte  more3, more4;

	int buff[15];
};

struct sprite_placement {
	int x, y, seq, frame, type, size;
	bool active;
	int rotation, special, brain;


	char script[13];
	char hit[13];
	char die[13];
	char talk[13];
	int speed, base_walk, base_idle, base_attack, base_hit, timer, que;
	int hard;
	Common::Rect alt;
	int prop;
	int warp_map;
	int warp_x;
	int warp_y;
	int parm_seq;

	int base_die, gold, hitpoints, strength, defense, exp, sound, vision, nohit, touch_damage;
	int buff[5];
};

struct small_map {
	char name[20];
	tile t[97];
	int v[40];
	char s[80];
	sprite_placement sprite[101];

	char script[13];
	char random[13];
	char load[13];
	char buffer[1000];
};

struct sprite_index {
	int x, y;
	int s;
	int last;

};

struct pic_info {
	LPDIRECTDRAWSURFACE     k;       // Sprites
	Common::Rect                box, hardbox;

	int yoffset;
	int xoffset;
};

struct attackinfo_struct {
	int time;
	bool active;
	int script;
	bool hitme;
	int last_power;
	int wait;
	int pull_wait;
};

struct wait_for_button {
	int script;
	int button;
	bool active;
};

struct talk_struct {
	char line[21][101];
	int line_return[21];
	char buffer[3000];
	int cur;
	int last;
	bool active;
	int cur_view;
	int cur_view_end;
	int page;
	int script;
	int offset;
	int newy;
	int color;
	int curf;
	int timer;
};

struct idata {
	int type;
	int seq;
	int frame;
	int xoffset, yoffset;
	Common::Rect hardbox;
};

struct mydata {
	unsigned char type[100];
	unsigned short seq[100];
	unsigned char frame[100];
	int last_time;
};

struct varman {
	int var;
	char name[20];
	int scope;
	bool active;
};

struct seth_sound {
	LPDIRECTSOUNDBUFFER   sound;
};

struct player_short_info {
	int version;
	char gameinfo[196];
	int minutes;

};

struct item_struct {
	bool active;
	char name[10];
	int seq;
	int frame;

};

//redink1... for storing current tiles in save game
struct player_info_tile {
	char file[50];
};

//redink1 added for storing global functions
struct global_function {
	char file[10];
	char func[20];
};

struct player_info {
	int version;
	char gameinfo[196];
	int minutes;
	int x, y, die, size, defense, dir, pframe, pseq, seq, frame, strength, base_walk, base_idle,


		base_hit, que;

	item_struct mitem[9]; //added one to these, because I don't like referring to a 0 item
	item_struct item[17];

	int curitem, unused;
	int counter;
	bool idle;
	mydata spmap[769];
	int button[10];
	varman var[max_vars];


	bool push_active;
	int push_dir;
	uint32 push_timer;
	int last_talk;
	int mouse;
	bool item_magic;
	int last_map;
	int crap;
	int buff[95];
	uint32 dbuff[20];

	long lbuff[10];

	//redink1... use wasted space for storing file location of map.dat, dink.dat, palette, and tiles
	char mapdat[50];
	char dinkdat[50];
	char palette[50];
	player_info_tile tile[42];
	global_function func[100];

	char  cbuff[750];
};


extern seth_sound ssound[max_sounds];
extern LPDIRECTSOUND lpDS;
extern bool please_wait;
extern int  show_dot;
extern int  plane_process;
extern hit_map hm;
extern HWND g_hWnd;
extern sprite_index index[max_sequences];
extern int last_sprite_added;
extern uint32 timer;
extern const char *command_line;
extern bool dinkedit;
extern int base_timing;
extern int weapon_script;
extern int magic_script;

extern int sp_mode;
extern int fps, fps_final;
extern int last_sprite_created;
extern int move_screen;
extern bool move_gonna;
extern int move_counter;
extern int m1x, m1y;
extern int m2x, m2y;
extern int m3x, m3y;
extern int playx;
extern bool windowed;
extern int playl;
extern HINSTANCE MyhInstance;
extern bool mouse1;
extern int cur_sprite;
extern int playy; //redink1's fix for the 'no sprites on pixel line above status bar' bug
extern int cur_map, cur_tile;
extern seth_joy sjoy;
extern int32 thisTickCount, lastTickCount;
extern uint32 timecrap;
extern Common::Rect math, box_crap, box_real;
extern wait_for_button wait;
extern HRESULT ddrval;
extern int sz, sy, x_offset, y_offset;
extern DDBLTFX ddbltfx;

extern int mode;
extern sequence seq[max_sequences];
extern map_info map;
extern small_map pam;
extern LPDIRECTDRAW            lpDD;           // DirectDraw object
//extern LPDIRECTDRAWSURFACE   lpDDSOne;       // Offscreen surface 1
extern LPDIRECTDRAWSURFACE     lpDDSTwo;       // Offscreen surface 2
extern LPDIRECTDRAWSURFACE     lpDDSTrick;       // Offscreen surface 2
extern LPDIRECTDRAWSURFACE     lpDDSTrick2;       // Offscreen surface 2
extern LPDIRECTDRAWSURFACE     lpDDSFade;       //redink1 surface for true-color fade-downs

extern bool trig_man;
extern bool total_trigger;
extern bool debug_mode;

extern pic_info     k[max_sprites];       // Sprite data
extern player_info play;

extern LPDIRECTDRAWSURFACE     lpDDSPrimary;   // DirectDraw primary surface
extern LPDIRECTDRAWSURFACE     lpDDSBack;      // DirectDraw back surface

extern LPDIRECTDRAWSURFACE     tiles[tile_screens];       // Game pieces
extern LPDIRECTDRAWSURFACE     game[max_game];       // Game pieces

extern sp spr[max_sprites_at_once]; //max sprite control systems at once

extern LPDIRECTDRAWPALETTE     lpDDPal;        // The primary surface palette
extern PALETTEENTRY    pe[256];
extern PALETTEENTRY    real_pal[256];
extern bool                    bActive;        // is application active?

extern LPDIRECTINPUT          g_pdi;
extern LPDIRECTINPUTDEVICE    g_pMouse;
extern HANDLE                 g_hevtMouse;

extern byte torusColors[256];  // Marks the colors used in the torus

extern HWND hWndMain;
extern JOYINFOEX jinfo; //joystick info
extern bool joystick;
extern bool disablejoystick;
extern hardness hmap;
extern Common::Rect tilerect[tile_screens];

//redink1 moves this here so could ignore keystrokes
extern HWND hwnd;

extern uint32 midi_id;

//redink1 code so dink will not write to the ini when in DMOD mode
extern bool g_b_no_write_ini;
extern bool no_cheat;
extern int last_saved_game;
extern bool g_b_kill_app; //if true, will close app as soon as the message pump is
//empty
extern int dinkspeed;
extern int last_cd;
//redink1... uh... increased these to 255?  I'm honestly not quite sure.  It has been a while. They might have been hard-coded, and I created char strings for them?
extern const char *current_map;
extern const char *current_dat;
extern int regcode, mycode;
extern time_t time_start;
extern bool item_screen;
extern bool midi_active;
 
//redink1 code for version change
extern char dversion_string[6];
//redink1 lets make a new save info thing
extern char save_game_info[200];

//redink1 and invertigo tags for windowed/high color mode
extern bool truecolor;
extern bool dinkpal;
//redink1 int for truecolor transition
extern int iTrueColorFadeTime;
extern bool bFadedDown;
extern bool smooth_follow;
extern unsigned short(*m16BitFadeValues)[30];
extern unsigned char(*m8BitFadeValues)[30];

extern ColorDepth mColorDepth;
extern bool mSwapped;
//unsigned int mTotalTime;
//unsigned int mTotalCount;
/*typedef Common::Array<uint32> FadeValues;
extern Common::HashMap<uint32, FadeValues>* mPrecalculatedFadeValues;*/
//redink1 added for midi looping
extern bool mLoopMidi;
//redink1 added for base push
extern unsigned int mDinkBasePush;
//redink1 added for true-color fade, amoung other things
extern uint32 dwRMask;
extern uint32 dwGMask;
extern uint32 dwBMask;
extern uint16 wRBits;
extern uint16 wGBits;
extern uint16 wBBits;
extern uint16 wRPos;
extern uint16 wGPos;
extern uint16 wBPos;
//redink1 added for console
extern bool mConsoleActive;
extern Common::String mConsoleLine;
extern unsigned int mConsoleScript;
extern Common::Array<Common::String> mConsoleLineHistory;
extern unsigned short int mConsoleHistoryIndex;
extern unsigned int mConsoleReturnValue;
extern uint32 CD_ID;
extern char dinkpath[200];
extern int map_vision;
extern int flub_mode;
extern unsigned short decipher_savegame;
extern int draw_map_tiny;
extern LPDIRECTSOUNDBUFFER   soundbank[num_soundbanks + 1];
extern LPDIRECTSOUNDBUFFER bowsound;
extern char last_debug[200];
extern int walk_off_screen;
extern char cbuf[64000];
extern bool cd_inserted;
extern int burn_revision;
extern bool abort_this_flip;
extern font_color font_colors[16];
extern show_bmp showb;
extern int keep_mouse;
extern int mcc, sound_support;
extern attackinfo_struct bow; 
extern soundstruct soundinfo[num_soundbanks + 1];
extern int screenlock; 
extern talk_struct talk;
extern char last_midi[15];
extern char midi_cur[20];
extern char midi_last[20];

extern int *pvision, *plife, *presult, *pspeed, *ptiming,
	*plifemax, *pexper, *pmap, *pstrength, *pcur_weapon,
	*pcur_magic, *pdefense, *pgold, *pmagic, *plevel,
	*plast_text, *pmagic_level;
extern int *pupdate_status, *pmissile_target, *penemy_sprite,
	*pmagic_cost, *pmissle_source;
extern int flife, fexp, fstrength, fdefense, fgold, fmagic,
	fmagic_level, flifemax, fraise, last_magic_draw;
extern int fcur_weapon, fcur_magic;
extern int mbase_timing;
extern idata id[max_idata];
extern uint32 mold;
extern int item_timer;
extern int item_pic; 
extern int mbase_count;
extern bool no_transition;
extern seth_sound ssound[max_sounds];
extern LPDIRECTSOUND lpDS; 
extern player_short_info short_play;
extern int push_active;
extern LOGFONT lf;
extern bool turn_on_plane;
extern HFONT hfont;
extern HFONT hfont_small;
extern int stop_entire_game;
 
extern bool in_enabled;
extern char *in_string;
extern bool first_frame;
extern bool no_running_main;
extern call_back callback[max_callbacks];
extern char *rbuf[max_scripts]; //pointers to buffers we may need
extern refinfo *rinfo[max_scripts];

extern int process_warp;
extern bool process_upcycle;
extern bool process_downcycle;
extern int32 cycle_clock;
extern int cycle_script;
extern int *in_int;
extern int in_x, in_y;
extern int sp_brain;
extern int returnint;
extern bool bKeepReturnInt;
extern char returnstring[200];
extern int sp_speed;
extern char slist[10][200];
extern long nlist[10];
extern int process_count;
extern Common::Rect sp_alt;
extern int hard_tile;
extern bool sp_screenmatch;
extern char in_temp[200];
extern int in_command;
extern int in_finish;
extern int in_onflag;
extern int in_max;
extern int in_huh;
extern char in_default[200];
extern int in_master;

extern bool sound_on;
extern char dir[80];

} // namespace Dink

#endif
