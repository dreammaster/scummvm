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

#include "ags2/vars.h"
#include "ags2/gfx/sprite_cache.h"

namespace AGS2 {

Vars *g_vars;
GameSetupStruct game;
GameSetupStruct thisgame;
GameState play;
GameSetup usetup;

SpriteCache spritset(1);
BITMAP *screen;
BITMAP *virtual_screen;
IGraphicsDriver *gfxDriver;
const char *game_file_name;
GFXFilter *filter;
MoveList *mls;
ViewStruct *views;
ccInstance *gameinst;
ccInstance *roominst;
RoomStatus *croom;
SOUNDCLIP *channels[MAX_SOUND_CHANNELS + 1];
CharacterCache *charcache;
int displayed_room;
int use_cd_player;
int scrnwid, scrnhit;
int mousex, mousey;
int offsetx, offsety;
int numlines;
int inside_script;
int numPluginReaders;
PluginObjectReader pluginReaders[MAX_PLUGIN_OBJECT_READERS];
char lines[MAXLINE][200];
int pluginsWantingDebugHooks;
int use_cdplayer;
ExecutingScript *curscript;
ObjectCache objcache[MAX_INIT_SPR];

int force_letterbox;
int game_paused;
int ifacepopped;
roomstruct thisroom;
color palette[256];
int currentcolor;

// initially size 1, this will be increased by the initFile function
SpriteCache spriteset(1);
int spritewidth[MAX_SPRITES], spriteheight[MAX_SPRITES];
int current_screen_resolution_multiplier_x, current_screen_resolution_multiplier_y;

int our_eip;
int eip_guinum, eip_guiobj;
int fps, display_fps;
int debug_flags;

// Startup flags, set from parameters to engine
int datafile_argv, change_to_game_dir, force_window;
int override_start_room, force_16bit;
bool justRegisterGame;
bool justUnRegisterGame;
const char *loadSaveGameOnStartup;

int use_compiled_folder_as_current_dir = 0;
int editor_debugging_enabled = 0;
int editor_debugging_initialized = 0;
char editor_debugger_instance_token[100];
int break_on_next_script_step = 0;
volatile int game_paused_in_debugger = 0;

int in_enters_screen, done_es_error;
int in_leaves_screen;
int need_to_stop_cd;
bool debug_15bit_mode, debug_24bit_mode;
int said_text;
int convert_16bit_bgr;
int mouse_z_was;
int bg_just_changed;
int loaded_game_file_version;
volatile bool want_exit, abort_engine;
bool check_dynamic_sprites_at_exit;
char return_to_roomedit[30];
char return_to_room[150];

int final_col_dep;
RoomStatus *roomstats;
AGSPlatformDriver *platform;

// routefnd.cpp
int *pathbackx, *pathbacky;
int waspossible = 1;
int routex1, routey1;
int suggestx, suggesty;
fixed move_speed_x, move_speed_y;

// acgui.cpp
DynamicArray<GUIButton> guibuts;
int numguibuts = 0;
DynamicArray<GUILabel> guilabels;
int numguilabels = 0;
DynamicArray<GUIInv> guiinv;
int numguiinv = 0;
DynamicArray<GUISlider> guislider;
int numguislider = 0;
DynamicArray<GUITextBox> guitext;
int numguitext = 0;
DynamicArray<GUIListBox> guilist;
int numguilist = 0;

block abuf;
int screenres, screenresIdx;
uint32 globalTimerCounter;
uint32 mvolcounter;
uint32 frames_per_second;
uint32 time_between_timers;

Vars::Vars() {
	g_vars = this;

	force_letterbox = 0;
	game_paused = 0;
	ifacepopped = -1;
	screen = nullptr;
	virtual_screen = nullptr;
	gfxDriver = nullptr;
	game_file_name = nullptr;
	mls = nullptr;
	views = nullptr;
	gameinst = nullptr;
	roominst = nullptr;
	croom = nullptr;
	Common::fill(channels, channels + MAX_SOUND_CHANNELS + 1, nullptr);
	charcache = nullptr;
	displayed_room = 0;
	use_cd_player = 0;
	scrnwid = scrnhit = 0;
	mousex = mousey = 0;
	offsetx = offsety = 0;
	numlines = 0;
	inside_script = 0;
	numPluginReaders = 0;
	pluginsWantingDebugHooks = 0;
	use_cdplayer = 0;
	curscript = nullptr;

	Common::fill((byte *)&thisroom, (byte *)&thisroom + sizeof(roomstruct), 0);
	Common::fill((byte *)palette, (byte *)palette + 256 * sizeof(color), 0);
	Common::fill(spritewidth, spritewidth + MAX_SPRITES, 0);
	Common::fill(spriteheight, spriteheight + MAX_SPRITES, 0);
	currentcolor = 0;

	current_screen_resolution_multiplier_x = current_screen_resolution_multiplier_y = 0;
	our_eip = 0;
	eip_guinum = eip_guiobj = 0;
	fps = display_fps = 0;

	debug_flags = 0;
	change_to_game_dir = 0;
	datafile_argv = 0;
	force_window = 0;
	override_start_room = force_16bit = 0;
	justRegisterGame = justUnRegisterGame = false;
	loadSaveGameOnStartup = nullptr;

	use_compiled_folder_as_current_dir = 0;
	editor_debugging_enabled = 0;
	editor_debugging_initialized = 0;
	Common::fill(editor_debugger_instance_token, editor_debugger_instance_token + 100, 0);
	break_on_next_script_step = 0;
	game_paused_in_debugger = 0;

	in_enters_screen = done_es_error = 0;
	in_leaves_screen = -1;
	need_to_stop_cd = false;
	debug_15bit_mode = debug_24bit_mode = false;
	said_text = 0;
	convert_16bit_bgr = 0;
	mouse_z_was = 0;
	bg_just_changed = 0;
	loaded_game_file_version = 0;
	want_exit = abort_engine = false;
	check_dynamic_sprites_at_exit = true;
	*return_to_roomedit = '\0';
	*return_to_room = '\0';

	final_col_dep = 0;
	roomstats = nullptr;
	abuf = nullptr;
	platform = nullptr;

	pathbackx = pathbacky = nullptr;
	waspossible = 1;
	routex1 = routey1 = 0;
	suggestx = suggesty = 0;
	move_speed_x = move_speed_y = 0;

	numguibuts = 0;
	numguilabels = 0;
	numguiinv = 0;
	numguislider = 0;
	numguitext = 0;
	numguilist = 0;

	screenres = screenresIdx = 0;
	globalTimerCounter = mvolcounter = 0;
	frames_per_second = time_between_timers = 0;
}

Vars::~Vars() {
	g_vars = nullptr;

	free(roomstats);
	free(pathbackx);
	free(pathbacky);
}

} // namespace AGS2
