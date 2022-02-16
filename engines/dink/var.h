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

#include "common/hashmap.h"
#include "common/rect.h"
#include "common/str.h"
#include "dink/types.h"

namespace Dink {

#ifndef __DEMO
#define __REAL_THING
#endif

//set language
#ifndef __LANGUAGE
#define __ENGLISH
#endif

extern void finiObjects();
extern void check_midi();


//redink1 moves this here so could ignore keystrokes
HWND hwnd;

uint32 midi_id;

//redink1 code so dink will not write to the ini when in DMOD mode
#ifdef DMOD
bool g_b_no_write_ini = true;
#else
bool g_b_no_write_ini = false;
#endif

bool no_cheat =  true;
//redink1 Updated for v1.08
constexpr int dversion = 108;
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
unsigned short (* m16BitFadeValues )[30];
unsigned char (* m8BitFadeValues )[30];
enum ColorDepth
{
    ColorDepth16Bit_565,
    ColorDepth16Bit_555,
    ColorDepth24Bit,
    ColorDepth32Bit,
    ColorDepthNotRecognized
};
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
int get_var(int script, char* name);
//redink1 added for console
bool mConsoleActive = false;
Common::String mConsoleLine;
unsigned int mConsoleScript = 0;
Common::Array<Common::String> mConsoleLineHistory;
unsigned short int mConsoleHistoryIndex = 0;
unsigned int mConsoleReturnValue = 0;

void init_scripts(void);
int load_script(char filename[15], int sprite, bool set_sprite);
void strchar(char *string, char ch);
char dinkpath[200];
uint32 CD_ID;
uint32 PlayCD(HWND hWndNotify, byte bTrack);
uint32 killcd(HWND hWndNotify, byte bTrack);
void update_status_all(void);
int add_sprite(int x1, int y, int brain,int pseq, int pframe );

void load_info(); //redink1
void add_exp(int num, int h, bool addEvenIfNotLastSpriteHit=false);
bool locate(int script, char proc[20]);
bool exist(char name[255]);
bool SoundStopEffect( int sound );
void draw_status_all(void);
bool SoundDestroyEffect(int sound);
int SoundPlayEffect( int sound,int min,int plus ,int sound3d, bool repeat);
void SoundLoadBanks( void);
bool StopMidi(void);
void check_seq_status(int h);
bool PlayMidi(char *sFileName);

void get_word(char line[300], int word, char *crap);
void Msg(const char *fmt, ... );
void run_script (int script);
void add_text(const char *tex, const char *filename);
void program_idata(void);
int map_vision = 0;  
void draw_map_game( void);
int realhard(int tile);
int flub_mode = -500;
unsigned short decipher_savegame = 0;
int draw_map_tiny = -1;
const int num_soundbanks = 20;
LPDIRECTSOUNDBUFFER   soundbank[num_soundbanks+1];
LPDIRECTSOUNDBUFFER bowsound;
char last_debug[200];
int walk_off_screen = false;
char cbuf[64000];
bool cd_inserted;
const int old_burn = 0;
int burn_revision = 0;
bool abort_this_flip = false;

//redink1 added for font colors
struct font_color {
    int red;
    int green;
    int blue;
};
font_color font_colors[16];

struct soundstruct
{
    bool repeat;
    int owner;
    int survive;
    int vol;
};

struct show_bmp
{
    bool active;
    bool showdot;
    int reserved;
    int script;
    int stime;
    int picframe;
};

show_bmp showb;

int keep_mouse = 0;

struct attackinfo_struct
{
    int time;
    bool active;
    int script;
    bool hitme;
    int last_power;
    int wait;
    int pull_wait;
};

struct wait_for_button
{
    int script;
    int button;
    bool active;
} wait;



int mcc,sound_support;

attackinfo_struct bow;

soundstruct soundinfo[num_soundbanks+1];
int screenlock = 0;

struct talk_struct
{
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

talk_struct talk;
char last_midi[15] = "";

struct idata
{
    int type;
    int seq;
    int frame;
    int xoffset, yoffset;
    Common::Rect hardbox;
};
char midi_cur[20];
char midi_last[20];

int * pvision, * plife, * presult, * pspeed, * ptiming, *plifemax, *pexper, *pmap,
*pstrength, * pcur_weapon,* pcur_magic, *pdefense, *pgold, *pmagic, *plevel, *plast_text, *pmagic_level;
int *pupdate_status, *pmissile_target, *penemy_sprite, *pmagic_cost, *pmissle_source;
int flife, fexp, fstrength, fdefense, fgold, fmagic, fmagic_level, flifemax, fraise, last_magic_draw;
int fcur_weapon, fcur_magic;
constexpr int max_idata = 1000; //redink1
int mbase_timing;
idata id[max_idata];

uint32 mold;

int item_timer;
int item_pic;

int mbase_count;
bool no_transition = false;
struct mydata
{
    unsigned char type[100];
    unsigned short seq[100];
    unsigned char frame[100];
    int last_time;
};

struct varman
{
    int var;
    char name[20];
    int scope;
    bool active;
};


constexpr int max_sounds = 100;

struct seth_sound
{
    LPDIRECTSOUNDBUFFER   sound;
};

seth_sound ssound[max_sounds];


LPDIRECTSOUND lpDS;


constexpr int max_vars = 250;

struct player_short_info
{
    int version;
    char gameinfo[196];
    int minutes;

};

player_short_info short_play;


struct item_struct
{
    bool active;
    char name[10];
    int seq;
    int frame;

};

//redink1... for storing current tiles in save game
struct player_info_tile
{
    char file[50];
};

//redink1 added for storing global functions
struct global_function
{
    char file[10];
    char func[20];
};

struct player_info
{
    int version;
    char gameinfo[196];
    int minutes;
    int x,y,die, size, defense, dir, pframe, pseq, seq, frame, strength, base_walk, base_idle,


        base_hit,que;

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

int push_active = 1;


LOGFONT lf = { 0,0,0,0,0,0,0,0,0,0,0,0,0,"" };

bool turn_on_plane = false;
HFONT hfont = nullptr;
HFONT hfont_small = nullptr;
constexpr int text_timer = 77;
constexpr int text_min = 2700;
constexpr int max_sprites = 4000;
int stop_entire_game;
constexpr int max_sprites_at_once = 300;
constexpr int max_callbacks = 100;
constexpr int max_scripts = 200;
constexpr int max_sequences = 1000; //Max # of sprite animations
constexpr int max_game = 20;
bool in_enabled = false;
char *in_string;


struct refinfo
{
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


bool first_frame = false;

bool no_running_main = false;

struct call_back
{

    int owner;
    bool active;
    int type;
    char name[20];
    int offset;
    long min, max;
    int lifespan;
    uint32 timer;
};


call_back callback[max_callbacks];

char *rbuf[max_scripts]; //pointers to buffers we may need

refinfo *rinfo[max_scripts];

int process_warp = 0;
bool process_upcycle = false;
bool process_downcycle = false;
uint32 cycle_clock = 0;
int cycle_script = 0;
int *in_int;
int in_x, in_y;
int sp_brain = 0;
int returnint = 0;
bool bKeepReturnInt = false;
char returnstring[200];
int sp_speed = 0;
char slist[10][200];
long nlist[10];
int process_count = 0;
Common::Rect sp_alt;
int hard_tile = 0;
bool sp_screenmatch = 0;
char in_temp[200];
int in_command;
int in_finish;
int in_onflag;
int in_max = 10;
int in_huh = 0;
char in_default[200];
int in_master = 0;

bool sound_on = true;
char dir[80];


//defaults





struct sequence
{

    int frame[51];
    int delay[51];
    unsigned char special[51];
    char data[80];
    bool active;
};


struct sp
{
    int x,moveman;
    int y; 
    int mx,my;
    int lpx[51],lpy[51];
    int speed;
    int brain;
    int seq_orig,dir;
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




struct seth_joy
{
    bool joybit[17]; //is button held down?
    bool letgo[17]; //copy of old above
    bool button[17]; //has button been pressed recently?
    bool key[256];
    bool kletgo[256];
    bool realkey[256];
    bool right,left,up,down;
    bool rightd,leftd,upd,downd;
    bool rightold,leftold,upold,downold;



};

void kill_repeat_sounds_all( void );

//sub struct for hardness map

struct mega_y
{
    byte y[401];
};

//struct for hardness map

struct hit_map
{
    mega_y x[601];
};

//sub struct for tile hardness

struct block_y
{
    byte y[51];
};

struct ts_block
{
    block_y x[51];
    bool used;
    int hold;
};

//struct for hardness info, INDEX controls which hardness each block has.  800 max
//types available.
struct hardness
{
    ts_block tile[800];
    int index[8000];

};

struct map_info
{
    char name[20];
    int loc[769];
    int music[769];
    int indoor[769];
    int v[40];
    char s[80];
    char buffer[2000];

};

struct tile
{
    int num, property, althard, more2;
    byte  more3,more4;

    int buff[15];
};

struct sprite_placement
{
    int x,y,seq,frame, type,size;
    bool active;
    int rotation, special,brain;


    char script[13];
    char hit[13];
    char die[13];
    char talk[13];
    int speed, base_walk,base_idle,base_attack,base_hit,timer,que;
    int hard;
    Common::Rect alt;
    int prop;
    int warp_map;
    int warp_x;
    int warp_y;
    int parm_seq;

    int base_die, gold, hitpoints, strength, defense,exp, sound, vision, nohit, touch_damage;
    int buff[5];
};

struct small_map
{
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



//bool PlayMidi(char *sFileName);

struct sprite_index
{
    int x,y;
    int s;
    int last;

};



int process_line (int script, char *s, bool doelse);



struct pic_info
{
    LPDIRECTDRAWSURFACE     k;       // Sprites
    Common::Rect                box,hardbox;

    int yoffset;
    int xoffset;
};

bool please_wait = false;
constexpr int tile_screens = 41+1;
int  show_dot = false;
int  plane_process = true;
hit_map hm;

HWND     g_hWnd;
#define ODS OutputDebugString
sprite_index index[max_sequences];
int last_sprite_added = 0;
uint32 timer = 0;
const char *command_line;
bool dinkedit = false;
int base_timing;
int weapon_script = 0;
int magic_script = 0;

int sp_mode = 0;
int fps,fps_final = 0;
int last_sprite_created;
int move_screen = 0;
bool move_gonna = false;
int move_counter = 0;
int m1x,m1y;
int m2x,m2y;
int m3x,m3y;
int playx = 620;
bool windowed = false;
int playl = 20;
HINSTANCE MyhInstance = nullptr;
bool mouse1 = false;
int cur_sprite = 1;
int playy = 400; //redink1's fix for the 'no sprites on pixel line above status bar' bug
int cur_map,cur_tile;
seth_joy sjoy;
uint32               thisTickCount,lastTickCount; 
uint32 timecrap;
Common::Rect math,box_crap,box_real;

HRESULT             ddrval;
int sz,sy,x_offset,y_offset;
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

pic_info     k[max_sprites];       // Sprite data
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
//LPDIRECTINPUT lpDI;


//direct input stuff for mouse reading

LPDIRECTINPUT          g_pdi = nullptr;
LPDIRECTINPUTDEVICE    g_pMouse = nullptr;
#define DINPUT_BUFFERSIZE           16

HANDLE                 g_hevtMouse = nullptr;


//LPCDIDATAFORMAT lpc;

byte torusColors[256];  // Marks the colors used in the torus


HWND hWndMain = nullptr;
JOYINFOEX jinfo; //joystick info
bool joystick = false;
bool disablejoystick = false;
hardness hmap;
Common::Rect tilerect[tile_screens];

} // namespace Dink

#endif
