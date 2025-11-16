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
int current_screen_resolution_multiplier;

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

// Misc
block abuf;
int screenres, screenresIdx;
uint32 globalTimerCounter;
uint32 mvolcounter;
uint32 frames_per_second;
uint32 time_between_timers;

// acgui.cpp
DynamicArray<GUIButton> guibuts;
int numguibuts;
DynamicArray<GUILabel> guilabels;
int numguilabels;
DynamicArray<GUIInv> guiinv;
int numguiinv;
DynamicArray<GUISlider> guislider;
int numguislider;
DynamicArray<GUITextBox> guitext;
int numguitext;
DynamicArray<GUIListBox> guilist;
int numguilist;

// acdraw.cpp
SpriteListEntry thingsToDrawList[MAX_THINGS_TO_DRAW];
int thingsToDrawSize;
SpriteListEntry sprlist[MAX_SPRITES_ON_SCREEN];
int sprlistsize;
int trans_mode;
IDriverDependantBitmap **actspswbbmp;
CachedActSpsData *actspswbcache;
int actSpsCount;
block *actsps;

// acoverlay.cpp
int is_complete_overlay, is_text_overlay;
ScreenOverlay screenover[MAX_SCREEN_OVERLAYS];
int crovr_id;

// acroom.cpp
int in_new_room;

// acsavegame.cpp
unsigned int load_new_game;
int load_new_game_restore;
int gameHasBeenRestored;

// acscripts.cpp
int num_scripts, eventClaimed;
ExecutingScript scripts[MAX_SCRIPT_AT_ONCE];
ExecutingScript *curscript;
int numanother;
char scfunctionname[30];
bool eventWasClaimed;
int no_blocking_functions;
ccScript *scriptModules[MAX_SCRIPT_MODULES];
ccInstance *moduleInst[MAX_SCRIPT_MODULES];
ccInstance *moduleInstFork[MAX_SCRIPT_MODULES];
char *moduleRepExecAddr[MAX_SCRIPT_MODULES];
int numScriptModules;
ccScript *gamescript;
ccScript *dialogScriptsScript;
ccInstance *gameinst, *roominst;
ccInstance *dialogScriptsInst;
ccInstance *gameinstFork, *roominstFork;
int post_script_cleanup_stack;
ScriptMouse scmouse;
DialogTopic *dialog;

// acsound.cpp
int said_speech_line;

// acwalkbehind.cpp
char *walkBehindExists;
int *walkBehindStartY, *walkBehindEndY;
char noWalkBehindsAtAll;
int walkBehindLeft[MAX_OBJ], walkBehindTop[MAX_OBJ];
int walkBehindRight[MAX_OBJ], walkBehindBottom[MAX_OBJ];
IDriverDependantBitmap *walkBehindBitmap[MAX_OBJ];
int walkBehindsCachedForBgNum;
WalkBehindMethodEnum walkBehindMethod;
block *actspswb;

// routefnd.cpp
int *pathbackx, *pathbacky;
int waspossible = 1;
int routex1, routey1;
int suggestx, suggesty;
fixed move_speed_x, move_speed_y;


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

	Common::fill((byte *)&thisroom, (byte *)&thisroom + sizeof(roomstruct), 0);
	Common::fill((byte *)palette, (byte *)palette + 256 * sizeof(color), 0);
	Common::fill(spritewidth, spritewidth + MAX_SPRITES, 0);
	Common::fill(spriteheight, spriteheight + MAX_SPRITES, 0);
	currentcolor = 0;

	current_screen_resolution_multiplier_x = current_screen_resolution_multiplier_y = 0;
	current_screen_resolution_multiplier = 1;
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

	// Misc
	screenres = screenresIdx = 0;
	globalTimerCounter = mvolcounter = 0;
	frames_per_second = time_between_timers = 0;

	// acgui.cpp
	numguibuts = 0;
	numguilabels = 0;
	numguiinv = 0;
	numguislider = 0;
	numguitext = 0;
	numguilist = 0;

	// acdraw.cpp
	thingsToDrawSize = 0;
	sprlistsize = 0;
	trans_mode = 0;
	actspswbbmp = nullptr;
	actspswbcache = nullptr;
	actSpsCount = 0;
	actsps = nullptr;

	// acoverlay.cpp
	is_complete_overlay = is_text_overlay = 0;
	crovr_id = 2;

	// acroom.cpp
	in_new_room = 0;

	// acsavegame.cpp
	load_new_game = 0;
	load_new_game_restore = -1;
	gameHasBeenRestored = 0;

	// acscripts.cpp
	num_scripts = 0;
	eventClaimed = EVENT_NONE;
	curscript = nullptr;
	numanother = 0;
	eventWasClaimed = false;
	no_blocking_functions = 0;
	numScriptModules = 0;
	gamescript = NULL;
	dialogScriptsScript = NULL;
	gameinst = roominst = NULL;
	dialogScriptsInst = NULL;
	gameinstFork = roominstFork = NULL;
	post_script_cleanup_stack = 0;
	scmouse.x = scmouse.y = 0;
	dialog = nullptr;

	// acsound.cpp
	said_speech_line = 0;

	// acwalkbehind.cpp
	walkBehindExists = NULL;
	walkBehindStartY = walkBehindEndY = NULL;
	noWalkBehindsAtAll = 0;
	walkBehindBitmap[MAX_OBJ];
	walkBehindsCachedForBgNum = 0;
	walkBehindMethod = DrawOverCharSprite;
	actspswb = nullptr;

	// routefnd.cpp
	pathbackx = pathbacky = nullptr;
	waspossible = 1;
	routex1 = routey1 = 0;
	suggestx = suggesty = 0;
	move_speed_x = move_speed_y = 0;
}

Vars::~Vars() {
	g_vars = nullptr;

	free(roomstats);
	free(pathbackx);
	free(pathbacky);
}

} // namespace AGS2
