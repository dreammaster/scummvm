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
RoomStatus *roomstats;
roomstruct thisroom;
RoomStatus troom;    // used for non-saveable rooms, eg. intro

SpriteCache spritset(1);
BITMAP *screen;
BITMAP *virtual_screen;
IGraphicsDriver *gfxDriver;
char *game_file_name;
MoveList *mls;
ViewStruct *views;
RoomStatus *croom;
SOUNDCLIP *channels[MAX_SOUND_CHANNELS + 1];
SOUNDCLIP *cachedQueuedMusic;
int numSoundChannels;

char saveGameDirectory[260] = "./";
char replayfile[MAX_PATH] = "record.dat";
const char *sgnametemplate = "agssave.%03d";

// initially size 1, this will be increased by the initFile function
SpriteCache spriteset(1);
int spritewidth[MAX_SPRITES], spriteheight[MAX_SPRITES];
int current_screen_resolution_multiplier_x, current_screen_resolution_multiplier_y;
int currentcolor;

int our_eip, eip_guinum, eip_guiobj;
int fps, display_fps;
int frames_per_second;
int debug_flags;

// Startup flags, set from parameters to engine
int datafile_argv, change_to_game_dir, force_window;
int override_start_room, force_16bit;
bool justRegisterGame;
bool justUnRegisterGame;
const char *loadSaveGameOnStartup;
AGSPlatformDriver *platform;

// Misc
block abuf;
int screenres, screenresIdx;
volatile uint32 globalTimerCounter;
volatile int mvolcounter, timerloop;
short *recordbuffer;
int recsize, recbuffersize;
unsigned long loopcounter, lastcounter;
unsigned long replay_last_second;
int replay_time;
int replay_start_this_time;
int first_debug_line, last_debug_line, display_console;
DebugConsoleText debug_line[DEBUG_CONSOLE_NUMLINES];
char pexbuf[STD_BUFFER_SIZE];
int current_music_type;
TreeMap *transtree;
char transFileName[MAX_PATH];
long lang_offs_start;
int source_text_length;
int update_music_at;
int time_between_timers;
bool current_background_is_dirty;
int longestline;
block _old_screen, _sub_screen;
IDriverDependantBitmap *mouseCursor;
IDriverDependantBitmap *blankImage;
IDriverDependantBitmap *blankSidebarImage;
IDriverDependantBitmap *debugConsole;
ScriptSystem scsystem;
int oldmouse, oldmousex, oldmousey;

// crossFading is >0 (channel number of new track), or -1 (old
// track fading out, no new track)
int last_sound_played[MAX_SOUND_CHANNELS + 1];
int current_screen_resolution_multiplier;
int force_letterbox;
int ifacepopped;  // currently displayed pop-up GUI (-1 if none)
color palette[256];
//block spriteset[MAX_SPRITES+1];
//SpriteCache spriteset (MAX_SPRITES+1);
// initially size 1, this will be increased by the initFile function
long t1;  // timer for FPS
int cur_mode, cur_cursor;
char saveGameSuffix[MAX_SG_EXT_LENGTH + 1];
//int abort_all_conditions=0;
long _sc_PlayerCharPtr;
int use_extra_sound_offset;
block debugConsoleBuffer;
block blank_mouse_cursor;

PluginObjectReader pluginReaders[MAX_PLUGIN_OBJECT_READERS];
int numPluginReaders;
COLOR_MAP maincoltable;
block walkareabackup, walkable_areas_temp;
int getloctype_index, getloctype_throughgui;
int user_disabled_for, user_disabled_data, user_disabled_data2;
int user_disabled_data3;
int face_talking = -1, facetalkview, facetalkwait, facetalkframe;
int facetalkloop, facetalkrepeat, facetalkAllowBlink;
int facetalkBlinkLoop;
CharacterInfo *facetalkchar;
int loops_per_character, text_lips_offset, char_speaking = -1;
char *text_lips_text;
SpeechLipSyncLine *splipsync;
int numLipLines, curLipLine = -1, curLipLinePhenome;
char **characterScriptObjNames;
char objectScriptObjNames[MAX_INIT_SPR][MAX_SCRIPT_NAME_LEN + 5];
char **guiScriptObjNames;
int working_gfx_mode_status = -1;
int restrict_until;
int gs_to_newroom = -1;
int proper_exit;
int numscreenover;
int scaddr;
int walk_behind_baselines_changed;
int displayed_room, starting_room;
int scrnwid, scrnhit;
int offsetx, offsety;
int mouse_on_iface;
int mouse_on_iface_button;
int mouse_pushed_iface;
int mouse_ifacebut_xoffs, mouse_ifacebut_yoffs;
IDriverDependantBitmap *roomBackgroundBmp;
int use_compiled_folder_as_current_dir;
int editor_debugging_enabled;
int editor_debugging_initialized;
char editor_debugger_instance_token[100];
IAGSEditorDebugger *editor_debugger;
int break_on_next_script_step;
volatile int game_paused_in_debugger;
void *editor_window_handle;
int in_enters_screen, done_es_error;
int in_leaves_screen = -1;
int need_to_stop_cd;
bool debug_15bit_mode, debug_24bit_mode;
int said_text;
int convert_16bit_bgr;
int mouse_z_was;
int bg_just_changed;
int loaded_game_file_version;
volatile bool want_exit, abort_engine;
bool check_dynamic_sprites_at_exit;
const char *evblockbasename;
int evblocknum;
int new_room_pos;
int new_room_x = SCR_NO_VALUE, new_room_y = SCR_NO_VALUE;
int inside_script, in_graph_script;
int in_inv_screen, inv_screen_newroom = -1;
int mouse_frame, mouse_delay;
int lastmx = -1, lastmy = -1;
int new_room_flags;
int use_cdplayer;
bool triedToUseCdAudioCommand;
int final_scrn_wid, final_scrn_hit, final_col_dep;
char lines[MAXLINE][200];
int numlines;
IDriverDependantBitmap **actspsbmp;
CharacterCache *charcache;
CharacterExtras *charextra;
RoomObject *objs;
CharacterInfo *playerchar;
ObjectCache objcache[MAX_INIT_SPR];
ScriptObject scrObj[MAX_INIT_SPR];
ScriptGUI *scrGui;
ScriptHotspot scrHotspot[MAX_HOTSPOTS];
ScriptRegion scrRegion[MAX_REGIONS];
ScriptInvItem scrInv[MAX_INV];
ScriptDialog scrDialog[MAX_DIALOG];
RGB_MAP rgb_table;
int want_quit, screen_reset;
block raw_saved_screen;
block dotted_mouse_cursor;
block dynamicallyCreatedSurfaces[MAX_DYNAMIC_SURFACES];
int scrlockWasDown;
int screen_is_dirty;
int pluginsWantingDebugHooks;
EventHappened event[MAXEVENTS + 1];
int numevents;
volatile int switching_away_from_game;
int musicPollIterator;
char alpha_blend_cursor;
int engineNeedsAsInt;
char rbuffer[200];
uint32 lastTime;
int turnlooporder[8];
NonBlockingScriptFunction repExecAlways(REP_EXEC_ALWAYS_NAME, 0);
NonBlockingScriptFunction getDialogOptionsDimensionsFunc("dialog_options_get_dimensions", 1);
NonBlockingScriptFunction renderDialogOptionsFunc("dialog_options_render", 1);
NonBlockingScriptFunction getDialogOptionUnderCursorFunc("dialog_options_get_active", 1);
NonBlockingScriptFunction runDialogOptionMouseClickHandlerFunc("dialog_options_mouse_click", 2);
int numOnStack;
block screenstack[10];
TopBarSettings topBar;
block screenop = NULL;
int wantFreeScreenop = 0;
int texthit;

// ac/audio.cpp
ScriptAudioChannel scrAudioChannel[MAX_SOUND_CHANNELS + 1];
CCAudioChannel ccDynamicAudio;
CCAudioClip ccDynamicAudioClip;
char acaudio_buffer[256];
int reserved_channel_count;

// ac/dialog.cpp
int windowbackgroundcolor, pushbuttondarkcolor;
int pushbuttonlightcolor;
int topwindowhandle;
int cbuttfont;
int acdialog_font;

// ac/draw.cpp
SpriteListEntry thingsToDrawList[MAX_THINGS_TO_DRAW];
int thingsToDrawSize;
SpriteListEntry sprlist[MAX_SPRITES_ON_SCREEN];
int sprlistsize;
int trans_mode;
IDriverDependantBitmap **actspswbbmp;
CachedActSpsData *actspswbcache;
int actSpsCount;
block *actsps;

// ac/gfx.cpp
GFXFilter *gfxFilterList[10];
GFXFilter *gfxFilterListD3D[10];
GFXFilter *filter;

// ac/gui.cpp
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
AnimatingGUIButton animbuts[MAX_ANIMATING_BUTTONS];
int numAnimButs;
int guis_need_update;
int all_buttons_disabled, gui_inv_pic;
int gui_disabled_style;

// ac/overlay.cpp
int is_complete_overlay, is_text_overlay;
ScreenOverlay screenover[MAX_SCREEN_OVERLAYS];
int crovr_id;

// ac/room.cpp
int in_new_room, new_room_was;
InteractionVariable globalvars[MAX_GLOBAL_VARIABLES];
int numGlobalVars;
long maxsize, outbytes, putbytes;
BITMAP *recalced;

// ac/savegame.cpp
unsigned int load_new_game;
int load_new_game_restore;
int gameHasBeenRestored;

// ac/scripts.cpp
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
Common::Stream *valid_handles[MAX_OPEN_SCRIPT_FILES + 1];
int num_open_script_files;

// ac/sound.cpp
int said_speech_line;
int crossFading, crossFadeVolumePerStep, crossFadeStep;
int crossFadeVolumeAtStart;
AmbientSound ambient[MAX_SOUND_CHANNELS + 1];  // + 1 just for safety on array iterations

// ac/walkbehind.cpp
char *walkBehindExists;
int *walkBehindStartY, *walkBehindEndY;
char noWalkBehindsAtAll;
int walkBehindLeft[MAX_OBJ], walkBehindTop[MAX_OBJ];
int walkBehindRight[MAX_OBJ], walkBehindBottom[MAX_OBJ];
IDriverDependantBitmap *walkBehindBitmap[MAX_OBJ];
int walkBehindsCachedForBgNum;
WalkBehindMethodEnum walkBehindMethod;
block *actspswb;

// ac/gui/gui.cpp
CCGUIObject ccDynamicGUIObject;
CCCharacter ccDynamicCharacter;
CCHotspot   ccDynamicHotspot;
CCRegion    ccDynamicRegion;
CCInventory ccDynamicInv;
CCGUI       ccDynamicGUI;
CCObject    ccDynamicObject;
CCDialog    ccDynamicDialog;
ScriptString myScriptStringImpl;
ScriptDialogOptionsRendering ccDialogOptionsRendering;
ScriptDrawingSurface *dialogOptionsRenderingSurface;

// ac/gui/main.cpp
GUIMain *guis;
block *guibg;
IDriverDependantBitmap **guibgbmp;

// common/events.cpp
byte key[KEY_MAX];
int key_shifts;

// common/mouse32.cpp
int mousex, mousey, numcurso, hotx, hoty;
int boundx1, boundx2, boundy1, boundy2;
int disable_mgetgraphpos;
char ignore_bounds;
block savebk, mousecurs[MAXCURSORS];
int vesa_xres, vesa_yres;
int currentcursor;
int pluginSimulatedClick;

// gfx/sprite.cpp
block tmpdbl, curspr;
int newwid, newhit;

// lib/allegro
// in allegro.h

// routefnd.cpp
int *pathbackx, *pathbacky;
int waspossible = 1;
int routex1, routey1;
int suggestx, suggesty;
fixed move_speed_x, move_speed_y;


Vars::Vars() {
	g_vars = this;

	Common::fill((byte *)&thisroom, (byte *)&thisroom + sizeof(roomstruct), 0);
	Common::fill((byte *)palette, (byte *)palette + 256 * sizeof(color), 0);
	Common::fill(spritewidth, spritewidth + MAX_SPRITES, 0);
	Common::fill(spriteheight, spriteheight + MAX_SPRITES, 0);
	currentcolor = 0;

	current_screen_resolution_multiplier_x = current_screen_resolution_multiplier_y = 0;
	current_screen_resolution_multiplier = 1;
	our_eip = eip_guinum = eip_guiobj = 0;
	fps = display_fps = 0;
	force_letterbox = 0;
	ifacepopped = 0;
	Common::fill((byte *)palette, (byte *)palette + 256 * sizeof(color), 0);

	change_to_game_dir = 0;
	datafile_argv = 0;
	force_window = 0;
	override_start_room = force_16bit = 0;
	justRegisterGame = justUnRegisterGame = false;
	loadSaveGameOnStartup = nullptr;

	Common::fill(editor_debugger_instance_token, editor_debugger_instance_token + 100, 0);
	break_on_next_script_step = 0;

	roomstats = nullptr;
	abuf = nullptr;
	platform = nullptr;

	// Misc
	screenres = screenresIdx = 0;
	globalTimerCounter = mvolcounter = timerloop = 0;
	recordbuffer = nullptr;
	recsize = recbuffersize = 0;
	loopcounter = lastcounter = 0;
	replay_last_second = 0;
	replay_time = 0;
	replay_start_this_time = 0;
	first_debug_line = last_debug_line = display_console = 0;
	current_music_type = 0;
	transtree = nullptr;
	transFileName[0] = '\0';
	lang_offs_start = 0;
	source_text_length = -1;
	update_music_at = 0;
	time_between_timers = 25;
	current_background_is_dirty = false;
	blank_mouse_cursor = nullptr;
	longestline = 0;
	_old_screen = _sub_screen = nullptr;
	mouseCursor = nullptr;
	blankImage = nullptr;
	blankSidebarImage = nullptr;
	debugConsole = nullptr;
	oldmouse = oldmousex = oldmousey = 0;

	views = nullptr;
	croom = nullptr;
	cachedQueuedMusic = NULL;
	numSoundChannels = 8;
	mls = nullptr;
	walkareabackup = nullptr;
	walkable_areas_temp = nullptr;
	num_scripts = 0;
	getloctype_index = 0;
	getloctype_throughgui = 0;
	user_disabled_for = 0;
	user_disabled_data = 0;
	user_disabled_data2 = 0;
	user_disabled_data3 = 0;
	face_talking = -1;
	facetalkview = 0;
	facetalkwait = 0;
	facetalkframe = 0;
	facetalkloop = facetalkrepeat = 0;
	facetalkAllowBlink = 1;
	facetalkBlinkLoop = 0;
	facetalkchar = nullptr;
	loops_per_character = 0;
	text_lips_offset = 0;
	char_speaking = -1;
	text_lips_text = nullptr;
	splipsync = nullptr;
	numLipLines = 0;
	curLipLine = -1;
	curLipLinePhenome = 0;
	characterScriptObjNames = nullptr;
	guiScriptObjNames = nullptr;
	working_gfx_mode_status = -1;
	restrict_until = 0;
	gs_to_newroom = -1;
	proper_exit = 0;
	numscreenover = 0;
	scaddr = 0;
	walk_behind_baselines_changed = 0;
	displayed_room = -10;
	starting_room = -1;
	scrnwid = scrnhit = 0;
	offsetx = offsety = 0;
	_sc_PlayerCharPtr = 0;
	mouse_on_iface = -1;
	mouse_on_iface_button = -1;
	mouse_pushed_iface = -1;
	mouse_ifacebut_xoffs = mouse_ifacebut_yoffs = -1;
	debug_flags = 0;
	roomBackgroundBmp = nullptr;
	use_compiled_folder_as_current_dir = 0;
	editor_debugging_enabled = 0;
	editor_debugging_initialized = 0;
	editor_debugger = nullptr;
	break_on_next_script_step = 0;
	game_paused_in_debugger = 0;
	editor_window_handle = nullptr;
	in_enters_screen = 0;
	done_es_error = 0;
	in_leaves_screen = -1;
	need_to_stop_cd = 0;
	debug_15bit_mode = debug_24bit_mode = false;
	said_text = 0;
	convert_16bit_bgr = 0;
	mouse_z_was = 0;
	bg_just_changed = 0;
	loaded_game_file_version = 0;
	want_exit = abort_engine = false;
	check_dynamic_sprites_at_exit = true;
	abort_engine = 0;
	evblockbasename  = nullptr;
	evblocknum = 0;
	frames_per_second = 40;
	debug_flags = 0;
	in_new_room = new_room_was = 0;
	new_room_pos = 0;
	new_room_x = new_room_y = SCR_NO_VALUE;
	inside_script = in_graph_script = 0;
	in_inv_screen = 0;
	inv_screen_newroom = -1;
	mouse_frame = mouse_delay = 0;
	lastmx = lastmy = -1;
	new_room_flags = 0;
	use_cdplayer = 0;
	triedToUseCdAudioCommand = false;
	final_scrn_wid = final_scrn_hit = final_col_dep = 0;
	numlines = 0;
	actspsbmp = nullptr;
	charcache = nullptr;
	charextra = nullptr;
	objs = nullptr;
	scrGui = nullptr;
	want_quit = screen_reset = 0;
	raw_saved_screen = nullptr;
	dotted_mouse_cursor = nullptr;
	scrlockWasDown = 0;
	screen_is_dirty = 0;
	pluginsWantingDebugHooks = 0;
	numevents = 0;
	switching_away_from_game = 0;
	musicPollIterator = 0;
	alpha_blend_cursor = 0;
	engineNeedsAsInt = 100;
	lastTime = 0;
	numOnStack = 0;
	Common::fill(screenstack, screenstack + 10, nullptr);
	screenop = NULL;
	wantFreeScreenop = 0;
	texthit = 0;

	// ac/audio.cpp
	Common::fill(acaudio_buffer, acaudio_buffer + 256, 0);
	reserved_channel_count = 0;

	// ac/dialog.cpp
	windowbackgroundcolor =  pushbuttondarkcolor = 0;
	pushbuttonlightcolor = 0;
	topwindowhandle = 0;
	cbuttfont = 0;
	acdialog_font = 0;

	// ac/draw.cpp
	thingsToDrawSize = 0;
	sprlistsize = 0;
	trans_mode = 0;
	actspswbbmp = nullptr;
	actspswbcache = nullptr;
	actSpsCount = 0;
	actsps = nullptr;

	// ac/gfx.cpp
	Common::fill(gfxFilterList, gfxFilterList + 10, nullptr);
	Common::fill(gfxFilterListD3D, gfxFilterListD3D + 10, nullptr);
	filter = nullptr;

	// ac/gui.cpp
	numguibuts = 0;
	numguilabels = 0;
	numguiinv = 0;
	numguislider = 0;
	numguitext = 0;
	numguilist = 0;
	numAnimButs = 0;
	guis_need_update = 1;
	all_buttons_disabled = 0;
	gui_inv_pic = -1;
	gui_disabled_style = 0;

	// ac/overlay.cpp
	is_complete_overlay = is_text_overlay = 0;
	crovr_id = 2;

	// ac/room.cpp
	in_new_room = new_room_was = 0;
	globalvars[0] = { "Global 1", 0, 0 };
	numGlobalVars = 1;
	maxsize = outbytes = putbytes = 0;
	recalced = nullptr;

	// ac/savegame.cpp
	load_new_game = 0;
	load_new_game_restore = -1;
	gameHasBeenRestored = 0;

	// ac/scripts.cpp
	num_scripts = 0;
	eventClaimed = EVENT_NONE;
	curscript = nullptr;
	numanother = 0;
	eventWasClaimed = false;
	no_blocking_functions = 0;
	numScriptModules = 0;
	gamescript = nullptr;
	dialogScriptsScript = nullptr;
	gameinst = roominst = nullptr;
	dialogScriptsInst = nullptr;
	gameinstFork = roominstFork = nullptr;
	post_script_cleanup_stack = 0;
	scmouse.x = scmouse.y = 0;
	dialog = nullptr;
	Common::fill(valid_handles, valid_handles + MAX_OPEN_SCRIPT_FILES + 1, nullptr);
	num_open_script_files = 0;

	// ac/sound.cpp
	said_speech_line = 0;
	crossFading = 0;

	// ac/walkbehind.cpp
	walkBehindExists = nullptr;
	walkBehindStartY = walkBehindEndY = nullptr;
	noWalkBehindsAtAll = 0;
	walkBehindBitmap[MAX_OBJ];
	walkBehindsCachedForBgNum = 0;
	walkBehindMethod = DrawOverCharSprite;
	actspswb = nullptr;

	// ac/gui/gui.cpp
	dialogOptionsRenderingSurface = nullptr;

	// ac/gui/main.cpp
	guis = nullptr;
	guibg = nullptr;
	guibgbmp = nullptr;

	// common/events.cpp
	Common::fill(key, key + KEY_MAX, 0);
	key_shifts = 0;

	// common/mouse32.cpp
	mousex = mousey = 0;
	hotx = 0, hoty = 0;
	numcurso = -1;
	boundx1 = 0;
	boundx2 = 99999;
	boundy1 = 0;
	boundy2 = 99999;
	disable_mgetgraphpos = 0;
	ignore_bounds = 0;
	savebk = nullptr;
	vesa_xres = 320;
	vesa_yres = 200;
	currentcursor = 0;
	pluginSimulatedClick = NONE;

	// gfx/sprite.cpp
	tmpdbl = curspr = nullptr;
	newwid = newhit = 0;

	// lib/allegro
	allegro_error = 0;
	_rgb_r_shift_15 = _rgb_g_shift_15 = _rgb_b_shift_15 = 0;
	_rgb_r_shift_16 = _rgb_g_shift_16 = _rgb_b_shift_16 = 0;
	_rgb_r_shift_24 = _rgb_g_shift_24 = _rgb_b_shift_24 = 0;
	_rgb_r_shift_32 = _rgb_g_shift_32 = _rgb_b_shift_32 = 0,
	_rgb_a_shift_32 = 0;
	_places_r = _places_g = 0;

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
