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

#ifndef AGS2_VARS_H
#define AGS2_VARS_H

#include "ags2/lib/allegro/color.h"
#include "ags2/lib/allegro/surface.h"
#include "ags2/lib/allegro/system.h"
#include "ags2/ac/acruntime.h"
#include "ags2/ac/cc.h"
#include "ags2/ac/draw.h"
#include "ags2/ac/gui/gui.h"
#include "ags2/ac/platform.h"
#include "ags2/ac/room.h"
#include "ags2/ac/walkbehind.h"
#include "ags2/ac/gui/gui_object.h"
#include "ags2/common/dynamic_array.h"
#include "ags2/common/mouse32.h"
#include "ags2/data/animating_gui_button.h"
#include "ags2/data/game_state.h"
#include "ags2/data/game_setup_struct.h"
#include "ags2/data/screen_overlay.h"
#include "ags2/data/top_bar_settings.h"
#include "ags2/gfx/sprite_cache.h"
#include "ags2/console.h"

namespace AGS2 {
	
class Vars;

struct DebugConsoleText {
	char text[100];
	char script[12];
};

extern Vars *g_vars;
extern GameSetupStruct game;
extern GameState play;
extern GameSetup usetup;
extern RoomStatus troom;
extern roomstruct thisroom;
extern BITMAP *screen;
extern BITMAP *virtual_screen;
extern char *game_file_name;
extern IGraphicsDriver *gfxDriver;
extern MoveList *mls;
extern ViewStruct *views;
extern ccInstance *gameinst;
extern ccInstance *roominst;
extern RoomStatus *croom;
extern SOUNDCLIP *channels[MAX_SOUND_CHANNELS + 1];
extern SOUNDCLIP *cachedQueuedMusic;
extern int numSoundChannels;
extern CharacterCache *charcache;
extern CharacterExtras *charextra;
extern RoomObject *objs;
extern CharacterInfo *playerchar;
extern int displayed_room;
extern int use_cd_player;
extern int scrnwid, scrnhit;
extern int offsetx, offsety;
extern int inside_script;
extern int numPluginReaders;
extern PluginObjectReader pluginReaders[MAX_PLUGIN_OBJECT_READERS];
extern char lines[MAXLINE][200];
extern int numlines;
extern ObjectCache objcache[MAX_INIT_SPR];
extern int cur_mode, cur_cursor;
extern long _sc_PlayerCharPtr;

extern SpriteCache spriteset;
extern int spritewidth[MAX_SPRITES], spriteheight[MAX_SPRITES];
extern char saveGameDirectory[260];
extern char replayfile[MAX_PATH];
extern const char *sgnametemplate;
extern char saveGameSuffix[MAX_SG_EXT_LENGTH + 1];
extern int last_sound_played[MAX_SOUND_CHANNELS + 1];
extern int current_screen_resolution_multiplier_x, current_screen_resolution_multiplier_y;
extern int current_screen_resolution_multiplier;

extern int our_eip;
extern int eip_guinum;
extern int eip_guiobj;
extern int debug_flags;
extern int force_letterbox;
extern int game_paused;
extern int ifacepopped;
extern color palette[256];
extern int currentcolor;
extern int fps, display_fps;

extern int datafile_argv, change_to_game_dir, force_window;
extern int override_start_room, force_16bit;
extern bool justRegisterGame;
extern bool justUnRegisterGame;
extern const char *loadSaveGameOnStartup;

extern int editor_debugging_enabled;
extern int editor_debugging_initialized;
extern char editor_debugger_instance_token[100];
extern int break_on_next_script_step;
extern volatile int game_paused_in_debugger;

extern int in_enters_screen, done_es_error;
extern int in_leaves_screen;
extern int need_to_stop_cd;
extern bool debug_15bit_mode, debug_24bit_mode;
extern int said_text;
extern int convert_16bit_bgr;
extern int mouse_z, mouse_z_was;
extern int bg_just_changed;
extern int loaded_game_file_version;
extern bool check_dynamic_sprites_at_exit;
extern char return_to_roomedit[30];
extern char return_to_room[150];
extern int final_col_dep;
extern RoomStatus *roomstats;
extern block abuf;
extern AGSPlatformDriver *platform;

// Misc
extern int screenresIdx;
extern volatile uint32 globalTimerCounter;
extern volatile int mvolcounter, timerloop;
extern short *recordbuffer;
extern int recsize, recbuffersize;
extern unsigned long loopcounter, lastcounter;
extern unsigned long replay_last_second;
extern int replay_time;
extern int replay_start_this_time;
extern int first_debug_line, last_debug_line, display_console;
extern DebugConsoleText debug_line[DEBUG_CONSOLE_NUMLINES];
extern char pexbuf[STD_BUFFER_SIZE];
extern int current_music_type;
extern TreeMap *transtree;
extern char transFileName[MAX_PATH];
extern long lang_offs_start;
extern int source_text_length;
extern int update_music_at;
extern int time_between_timers;
extern bool current_background_is_dirty;
extern block debugConsoleBuffer;
extern block blank_mouse_cursor;
extern int longestline;
extern block _old_screen;
extern block _sub_screen;
extern IDriverDependantBitmap *mouseCursor;
extern IDriverDependantBitmap *blankImage;
extern IDriverDependantBitmap *blankSidebarImage;
extern IDriverDependantBitmap *debugConsole;
extern ScriptSystem scsystem;
extern int oldmouse, oldmousex, oldmousey;
extern EventHappened event[MAXEVENTS + 1];
extern int numevents;
extern volatile int switching_away_from_game;
extern int musicPollIterator;
extern char alpha_blend_cursor;
extern int engineNeedsAsInt;
extern char rbuffer[200];
extern uint32 lastTime;
extern int turnlooporder[8];
extern NonBlockingScriptFunction repExecAlways;
extern NonBlockingScriptFunction getDialogOptionsDimensionsFunc;
extern NonBlockingScriptFunction renderDialogOptionsFunc;
extern NonBlockingScriptFunction getDialogOptionUnderCursorFunc;
extern NonBlockingScriptFunction runDialogOptionMouseClickHandlerFunc;
extern int numOnStack;
extern block screenstack[10];
extern TopBarSettings topBar;
extern block screenop;
extern int wantFreeScreenop;
extern int texthit;

// ac/audio.cpp
extern ScriptAudioChannel scrAudioChannel[MAX_SOUND_CHANNELS + 1];
extern CCAudioChannel ccDynamicAudio;
extern CCAudioClip ccDynamicAudioClip;
extern char acaudio_buffer[256];
extern int reserved_channel_count;

// ac/dialog.cpp
extern int windowbackgroundcolor, pushbuttondarkcolor;
extern int pushbuttonlightcolor;
extern int topwindowhandle;
extern int cbuttfont;
extern int acdialog_font;

// ac/draw.cpp
extern SpriteListEntry thingsToDrawList[MAX_THINGS_TO_DRAW];
extern int thingsToDrawSize;
extern SpriteListEntry sprlist[MAX_SPRITES_ON_SCREEN];
extern int sprlistsize;
extern int trans_mode;
extern IDriverDependantBitmap **actspswbbmp;
extern CachedActSpsData *actspswbcache;
extern int actSpsCount;
extern block *actsps;

// ac/gfx.cpp
extern GFXFilter *gfxFilterList[10];
extern GFXFilter *gfxFilterListD3D[10];
extern GFXFilter *filter;

// ac/gui.cpp
extern DynamicArray<GUIButton> guibuts;
extern int numguibuts;
extern DynamicArray<GUILabel> guilabels;
extern int numguilabels;
extern DynamicArray<GUIInv> guiinv;
extern int numguiinv;
extern DynamicArray<GUISlider> guislider;
extern int numguislider;
extern DynamicArray<GUITextBox> guitext;
extern int numguitext;
extern DynamicArray<GUIListBox> guilist;
extern int numguilist;
extern AnimatingGUIButton animbuts[MAX_ANIMATING_BUTTONS];
extern int numAnimButs;
extern int guis_need_update;
extern int all_buttons_disabled, gui_inv_pic;
extern int gui_disabled_style;

// ac/overlay.cpp
extern int is_complete_overlay,is_text_overlay;
extern int crovr_id;		// Whether using SetTextOverlay or CreateTextOvelay

// ac/room.cpp
extern int in_new_room;
extern InteractionVariable globalvars[MAX_GLOBAL_VARIABLES];
extern int numGlobalVars;

// ac/savegame.cpp
extern unsigned int load_new_game;
extern int load_new_game_restore;
extern int gameHasBeenRestored;

// ac/scripts.cpp
extern int num_scripts, eventClaimed;
extern ExecutingScript scripts[MAX_SCRIPT_AT_ONCE];
extern ExecutingScript *curscript;
extern int numanother;
extern char scfunctionname[30];
extern bool eventWasClaimed;
extern int no_blocking_functions;
extern ccScript *scriptModules[MAX_SCRIPT_MODULES];
extern ccInstance *moduleInst[MAX_SCRIPT_MODULES];
extern ccInstance *moduleInstFork[MAX_SCRIPT_MODULES];
extern char *moduleRepExecAddr[MAX_SCRIPT_MODULES];
extern int numScriptModules;
extern ccScript *gamescript;
extern ccScript *dialogScriptsScript;
extern ccInstance *gameinst, *roominst;
extern ccInstance *dialogScriptsInst;
extern ccInstance *gameinstFork, *roominstFork;
extern int post_script_cleanup_stack;
extern ScriptMouse scmouse;
extern DialogTopic *dialog;
extern ViewStruct *views;
extern COLOR_MAP maincoltable;
extern MoveList *mls;
extern DialogTopic *dialog;
extern block walkareabackup, walkable_areas_temp;
extern ExecutingScript scripts[MAX_SCRIPT_AT_ONCE];
extern ExecutingScript *curscript;
extern AnimatingGUIButton animbuts[MAX_ANIMATING_BUTTONS];
extern int numAnimButs;
extern int getloctype_index, getloctype_throughgui;
extern int user_disabled_for, user_disabled_data, user_disabled_data2;
extern int user_disabled_data3;
extern int is_complete_overlay, is_text_overlay;
extern int face_talking, facetalkview, facetalkwait, facetalkframe;
extern int facetalkloop, facetalkrepeat, facetalkAllowBlink;
extern int facetalkBlinkLoop;
extern CharacterInfo *facetalkchar;
extern int loops_per_character, text_lips_offset, char_speaking;
extern char *text_lips_text;
extern SpeechLipSyncLine *splipsync;
extern int numLipLines, curLipLine, curLipLinePhenome;
extern int gameHasBeenRestored;
extern char **characterScriptObjNames;
extern char objectScriptObjNames[MAX_INIT_SPR][MAX_SCRIPT_NAME_LEN + 5];
extern char **guiScriptObjNames;
extern int working_gfx_mode_status;
extern int restrict_until;
extern int gs_to_newroom;
extern ScreenOverlay screenover[MAX_SCREEN_OVERLAYS];
extern int proper_exit, our_eip;
extern int numscreenover;
extern int scaddr;
extern int walk_behind_baselines_changed;
extern int displayed_room0, starting_room;
extern int mouse_on_iface;
extern int mouse_on_iface_button;
extern int mouse_pushed_iface;
extern int mouse_ifacebut_xoffs, mouse_ifacebut_yoffs;
extern int debug_flags;
extern IDriverDependantBitmap *roomBackgroundBmp;
extern int use_compiled_folder_as_current_dir;
extern int editor_debugging_enabled;
extern int editor_debugging_initialized;
extern char editor_debugger_instance_token[100];
extern IAGSEditorDebugger *editor_debugger;
extern int break_on_next_script_step;
extern volatile int game_paused_in_debugger;
extern void *editor_window_handle;
extern int in_enters_screen, done_es_error;
extern int in_leaves_screen;
extern int need_to_stop_cd;
extern int said_text;
extern int convert_16bit_bgr;
extern int mouse_z_was;
extern int bg_just_changed;
extern int loaded_game_file_version;
extern volatile bool want_exit, abort_engine;
extern const char *evblockbasename;
extern int evblocknum;
extern int frames_per_second;
extern int in_new_room, new_room_was;
extern int new_room_pos;
extern int new_room_x, new_room_y;
extern unsigned int load_new_game;
extern int load_new_game_restore;
extern int inside_script, in_graph_script;
extern int no_blocking_functions;
extern int in_inv_screen, inv_screen_newroom;
extern int mouse_frame, mouse_delay;
extern int lastmx, lastmy;
extern int new_room_flags;
extern SpriteListEntry sprlist[MAX_SPRITES_ON_SCREEN];
extern int sprlistsize;
extern SpriteListEntry thingsToDrawList[MAX_THINGS_TO_DRAW];
extern int thingsToDrawSize;
extern int use_cdplayer;
extern bool triedToUseCdAudioCommand;
extern int final_scrn_wid, final_scrn_hit, final_col_dep;
extern int post_script_cleanup_stack;
extern int actSpsCount;
extern block *actsps;
extern IDriverDependantBitmap **actspsbmp;
extern block *actspswb;
extern IDriverDependantBitmap **actspswbbmp;
extern CachedActSpsData *actspswbcache;
extern CharacterCache *charcache;
extern ObjectCache objcache[MAX_INIT_SPR];
extern ScriptObject scrObj[MAX_INIT_SPR];
extern ScriptGUI *scrGui;
extern ScriptHotspot scrHotspot[MAX_HOTSPOTS];
extern ScriptRegion scrRegion[MAX_REGIONS];
extern ScriptInvItem scrInv[MAX_INV];
extern ScriptDialog scrDialog[MAX_DIALOG];
extern RGB_MAP rgb_table;
extern int want_quit, screen_reset;
extern block raw_saved_screen;
extern block dotted_mouse_cursor;
extern block dynamicallyCreatedSurfaces[MAX_DYNAMIC_SURFACES];
extern int scrlockWasDown;
extern int screen_is_dirty;
extern int pluginsWantingDebugHooks;
extern Common::Stream *valid_handles[MAX_OPEN_SCRIPT_FILES + 1];
extern int num_open_script_files;

// ac/sound.cpp
extern int said_speech_line;
extern int said_speech_line;
extern int crossFading, crossFadeVolumePerStep, crossFadeStep;
extern int crossFadeVolumeAtStart;
extern AmbientSound ambient[MAX_SOUND_CHANNELS + 1];  // + 1 just for safety on array iterations

// ac/walkbehind.cpp
extern char *walkBehindExists;  // whether a WB area is in this column
extern int *walkBehindStartY, *walkBehindEndY;
extern char noWalkBehindsAtAll;
extern int walkBehindLeft[MAX_OBJ], walkBehindTop[MAX_OBJ];
extern int walkBehindRight[MAX_OBJ], walkBehindBottom[MAX_OBJ];
extern IDriverDependantBitmap *walkBehindBitmap[MAX_OBJ];
extern int walkBehindsCachedForBgNum;
extern WalkBehindMethodEnum walkBehindMethod;
extern block *actspswb;

// ac/gui/gui.cpp
extern CCGUIObject ccDynamicGUIObject;
extern CCCharacter ccDynamicCharacter;
extern CCHotspot   ccDynamicHotspot;
extern CCRegion    ccDynamicRegion;
extern CCInventory ccDynamicInv;
extern CCGUI       ccDynamicGUI;
extern CCObject    ccDynamicObject;
extern CCDialog    ccDynamicDialog;
extern ScriptString myScriptStringImpl;
extern ScriptDialogOptionsRendering ccDialogOptionsRendering;
extern ScriptDrawingSurface *dialogOptionsRenderingSurface;

// ac/gui/main.cpp
extern GUIMain *guis;
extern block *guibg;
extern IDriverDependantBitmap **guibgbmp;

// common/events.cpp
extern byte key[KEY_MAX];
extern int key_shifts;

// common/mouse32.cpp
extern int mousex, mousey, numcurso, hotx, hoty;
extern int boundx1, boundx2, boundy1, boundy2;
extern int disable_mgetgraphpos;
extern char ignore_bounds;
extern block savebk, mousecurs[MAXCURSORS];
extern int vesa_xres, vesa_yres;
extern int currentcursor;
extern int pluginSimulatedClick;

// gfx/sprite.cpp
extern block tmpdbl, curspr;
extern int newwid, newhit;

// routefnd.cpp
extern int *pathbackx, *pathbacky;
extern int waspossible;
extern int routex1, routey1;
extern int suggestx, suggesty;
extern fixed move_speed_x, move_speed_y;


class Vars {
public:
	BlenderMode _blender_mode = kRgbToRgbBlender;
	PALETTE _current_palette = {};
	PALETTE _prev_current_palette = {};
	int _color_depth = 0;
	int _trans_blend_alpha = 0;
	int _trans_blend_red = 0;
	int _trans_blend_green = 0;
	int _trans_blend_blue = 0;
	int *const _allegro_errno = &allegro_error;
	int _utype = 0;

public:
	// routefnd
	int _walk_area_zone5 = 0;
	int _routex1 = 0;

public:
	Vars();
	~Vars();
};

#define _G(X) (g_vars->_##X)

} // namespace AGS2

#endif
