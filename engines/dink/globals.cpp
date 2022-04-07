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

#include "graphics/font.h"
#include "dink/globals.h"

namespace Dink {

seth_sound ssound[max_sounds];
LPDIRECTSOUND lpDS;
bool please_wait = false;
int  show_dot = false;
int  plane_process = true;
hit_map hm;

HWND     g_hWnd;
sprite_index index[max_sequences];
int last_sprite_added = 0;
uint32 timer = 0;
const char *command_line;
bool dinkedit = false;
int base_timing;
int weapon_script = 0;
int magic_script = 0;

int sp_mode = 0;
int fps, fps_final = 0;
int last_sprite_created;
int move_screen = 0;
bool move_gonna = false;
int move_counter = 0;
int m1x, m1y;
int m2x, m2y;
int m3x, m3y;
int playx = 620;
bool windowed = false;
int playl = 20;
HINSTANCE MyhInstance = nullptr;
bool mouse1 = false;
int cur_sprite = 1;
int playy = 400; //redink1's fix for the 'no sprites on pixel line above status bar' bug
int cur_map, cur_tile;
seth_joy sjoy;
int32               thisTickCount, lastTickCount;
uint32 timecrap;
RECT math, box_crap, box_real;
wait_for_button wait;
HRESULT             ddrval;
int sz, sy, x_offset, y_offset;
DDBLTFX     ddbltfx;

int mode;

sequence seq[max_sequences];
map_info map;
small_map pam;
LPDIRECTDRAW            lpDD = nullptr;           // DirectDraw object
//LPDIRECTDRAWSURFACE     lpDDSOne;       // Offscreen surface 1
LPDIRECTDRAWSURFACE     lpDDSTwo = nullptr;       // Offscreen surface 2
LPDIRECTDRAWSURFACE     lpDDSTrick = nullptr;       // Offscreen surface 2
LPDIRECTDRAWSURFACE     lpDDSTrick2 = nullptr;       // Offscreen surface 2
LPDIRECTDRAWSURFACE     lpDDSFade = nullptr;       //redink1 surface for true-color fade-downs

bool trig_man = false;
bool total_trigger = false;
bool debug_mode = false;

pic_info picInfo[max_sprites];       // Sprite data
player_info play;


LPDIRECTDRAWSURFACE     lpDDSPrimary = nullptr;   // DirectDraw primary surface
LPDIRECTDRAWSURFACE     lpDDSBack = nullptr;      // DirectDraw back surface

LPDIRECTDRAWSURFACE     tiles[tile_screens];       // Game pieces
LPDIRECTDRAWSURFACE     game[max_game];       // Game pieces

sp spr[max_sprites_at_once]; //max sprite control systems at once

LPDIRECTDRAWPALETTE     lpDDPal = nullptr;        // The primary surface palette
PALETTEENTRY    pe[256];
PALETTEENTRY    real_pal[256];

bool                    bActive = false;        // is application active?

LPDIRECTINPUT          g_pdi = nullptr;
LPDIRECTINPUTDEVICE    g_pMouse = nullptr;
HANDLE                 g_hevtMouse = nullptr;

byte torusColors[256];  // Marks the colors used in the torus

HWND hWndMain = nullptr;
JOYINFOEX jinfo; //joystick info
bool joystick = false;
bool disablejoystick = false;
hardness hmap;
RECT tilerect[tile_screens];


//redink1 moves this here so could ignore keystrokes
HWND hwnd;

uint32 midi_id;

bool no_cheat = true;
int last_saved_game = 0;
bool g_b_kill_app = false; //if true, will close app as soon as the message pump is
//empty
int dinkspeed = 3;
int last_cd;

//redink1... uh... increased these to 255?  I'm honestly not quite sure.  It has been a while. They might have been hard-coded, and I created char strings for them?
char current_map[50] = "MAP.DAT";
char current_dat[50] = "DINK.DAT";

int regcode, mycode;
time_t time_start;
bool item_screen = false;
bool midi_active = true;

//redink1 code for version change
char dversion_string[6] = "v1.08";
//redink1 lets make a new save info thing
char save_game_info[200] = "Level &level";
//redink1 added this prototype to fix this save info declaration error thing
void decipher_string(char line[200], int script);
//redink1 and invertigo tags for windowed/high color mode
bool truecolor = false;
bool dinkpal = false;
//redink1 int for truecolor transition
int iTrueColorFadeTime = 0;
bool bFadedDown = false;
bool smooth_follow = false;
unsigned short(*m16BitFadeValues)[30];
unsigned char(*m8BitFadeValues)[30];

ColorDepth mColorDepth = ColorDepthNotRecognized;
bool mSwapped = false;
//unsigned int mTotalTime = 0;
//unsigned int mTotalCount = 0;
/*typedef Common::Array<uint32> FadeValues;
Common::HashMap<uint32, FadeValues>* mPrecalculatedFadeValues;*/
//redink1 added for midi looping
bool mLoopMidi = false;
//redink1 added for base push
unsigned int mDinkBasePush = 310;
//redink1 added for true-color fade, amoung other things
uint32 dwRMask;
uint32 dwGMask;
uint32 dwBMask;
uint16 wRBits;
uint16 wGBits;
uint16 wBBits;
uint16 wRPos;
uint16 wGPos;
uint16 wBPos;
//redink1 added for recursive scope checking
int get_var(int script, char *name);
//redink1 added for console
bool mConsoleActive = false;
Common::String mConsoleLine;
unsigned int mConsoleScript = 0;
Common::Array<Common::String> mConsoleLineHistory;
unsigned short int mConsoleHistoryIndex = 0;
unsigned int mConsoleReturnValue = 0;
uint32 CD_ID;
char dinkpath[200];

int map_vision = 0;
int flub_mode = -500;
unsigned short decipher_savegame = 0;
int draw_map_tiny = -1;
LPDIRECTSOUNDBUFFER   soundbank[num_soundbanks + 1];
LPDIRECTSOUNDBUFFER bowsound;
char last_debug[200];
int walk_off_screen = false;
char cbuf[64000];
bool cd_inserted;
int burn_revision = 0;
bool abort_this_flip = false;
font_color font_colors[16];

ShowBmp showb;

int keep_mouse = 0;

int mcc, sound_support;

attackinfo_struct bow;

soundstruct soundinfo[num_soundbanks + 1];
int screenlock = 0;


talk_struct talk;
char last_midi[15] = "";

char midi_cur[20];
char midi_last[20];

int *pvision, *plife, *presult, *pspeed, *ptiming, *plifemax,
    *pexper, *pmap, *pstrength, *pcur_weapon, *pcur_magic,
    *pdefense, *pgold, *pmagic, *plevel, *plast_text, *pmagic_level;
int *pupdate_status, *pmissile_target, *penemy_sprite, *pmagic_cost,
    *pmissle_source;
int flife, fexp, fstrength, fdefense, fgold, fmagic, fmagic_level,
    flifemax, fraise, last_magic_draw;
int fcur_weapon, fcur_magic;
int mbase_timing;
idata id[max_idata];

uint32 mold;

int item_timer;
int item_pic;

int mbase_count;
bool no_transition = false;
player_short_info short_play;
int push_active = 1;

bool turn_on_plane = false;
Graphics::Font *hfont = nullptr;
Graphics::Font *hfont_small = nullptr;
int stop_entire_game;

bool in_enabled = false;
char *in_string;

bool first_frame = false;

bool no_running_main = false;

call_back callback[max_callbacks];

char *rbuf[max_scripts]; //pointers to buffers we may need

refinfo *rinfo[max_scripts];

int process_warp = 0;
bool process_upcycle = false;
bool process_downcycle = false;
int32 cycle_clock = 0;
int cycle_script = 0;
int *in_int = nullptr;
int in_x = 0, in_y = 0;
int sp_brain = 0;
int returnint = 0;
bool bKeepReturnInt = false;
char returnstring[200];
int sp_speed = 0;
char slist[10][200];
long nlist[10];
int process_count = 0;
RECT sp_alt;
int hard_tile = 0;
bool sp_screenmatch = 0;
char in_temp[200];
int in_command = 0;
int in_finish = 0;
int in_onflag = 0;
int in_max = 10;
int in_huh = 0;
char in_default[200];
int in_master = 0;

bool sound_on = true;
char dir[80];

int fps_average;
int but_timer = 0;
int water_timer;
bool fire_forward;
int fire_flip;
int fps_show = 0;

//idirectsound
LPDIRECTDRAWCLIPPER lpClipper;

int drawthistime = true;
int currX = 640;
int currY = 480;
RECT                rc;
int winoffset = 25;
int winoffsetx = 5;

int cx;
int cy;
int speed;

} // namespace Dink
