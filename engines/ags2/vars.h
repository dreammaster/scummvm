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
#include "ags2/ac/draw.h"
#include "ags2/ac/walkbehind.h"
#include "ags2/ac/gui/gui.h"
#include "ags2/ac/room.h"
#include "ags2/gfx/sprite_cache.h"
#include "ags2/ac/acruntime.h"

namespace AGS2 {
	
class Vars;

extern Vars *g_vars;
extern GameSetupStruct game;
extern GameState play;
extern GameSetup usetup;
extern BITMAP *screen;
extern BITMAP *virtual_screen;
extern const char *game_file_name;
extern IGraphicsDriver *gfxDriver;
extern GFXFilter *filter;
extern MoveList *mls;
extern ViewStruct *views;
extern ccInstance *gameinst;
extern ccInstance *roominst;
extern RoomStatus *croom;
extern SOUNDCLIP *channels[MAX_SOUND_CHANNELS + 1];
extern CharacterCache *charcache;
extern int displayed_room;
extern int use_cd_player;
extern int scrnwid, scrnhit;
extern int mousex, mousey;
extern int offsetx, offsety;
extern int numlines;
extern int inside_script;
extern int numPluginReaders;
extern PluginObjectReader pluginReaders[MAX_PLUGIN_OBJECT_READERS];
extern char lines[MAXLINE][200];
extern ObjectCache objcache[MAX_INIT_SPR];

extern SpriteCache spriteset;
extern int spritewidth[MAX_SPRITES], spriteheight[MAX_SPRITES];
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

extern int use_compiled_folder_as_current_dir;
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
extern int mouse_z_was;
extern int bg_just_changed;
extern int loaded_game_file_version;
extern volatile bool want_exit, abort_engine;
extern bool check_dynamic_sprites_at_exit;
extern char return_to_roomedit[30];
extern char return_to_room[150];
extern int final_col_dep;
extern RoomStatus *roomstats;
extern block abuf;
extern AGSPlatformDriver *platform;

// misc
extern int screenresIdx;
extern uint32 globalTimerCounter;
extern uint32 mvolcounter;
extern uint32 frames_per_second;
extern uint32 time_between_timers;

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

// ac/overlay.cpp
extern int is_complete_overlay,is_text_overlay;
extern int crovr_id;

// ac/room.cpp
extern int in_new_room;

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

// ac/sound.cpp
extern int said_speech_line;

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

// ac/gui/main.cpp
extern GUIMain *guis;

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
	RGB_MAP *_rgb_map = nullptr;
	int _color_depth = 0;
	int _trans_blend_alpha = 0;
	int _trans_blend_red = 0;
	int _trans_blend_green = 0;
	int _trans_blend_blue = 0;
	int _rgb_r_shift_15 = 0, _rgb_g_shift_15 = 0, _rgb_b_shift_15 = 0;
	int _rgb_r_shift_16 = 0, _rgb_g_shift_16 = 0, _rgb_b_shift_16 = 0;
	int _rgb_r_shift_24 = 0, _rgb_g_shift_24 = 0, _rgb_b_shift_24 = 0;
	int _rgb_r_shift_32 = 0, _rgb_g_shift_32 = 0, _rgb_b_shift_32 = 0,
		_rgb_a_shift_32 = 0;
	int _errnum = 0;
	int *const _allegro_errno = &_errnum;
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
