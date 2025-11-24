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

#ifndef AGS2_AC_RUNTIME_H
#define AGS2_AC_RUNTIME_H

#include "ags2/gfx/ali3d.h"
#include "ags2/gfx/sprite_cache.h"
#include "ags2/ac/cc.h"
#include "ags2/ac/dialog.h"
#include "ags2/ac/dynamic.h"
#include "ags2/ac/events.h"
#include "ags2/ac/inventory.h"
#include "ags2/ac/overlay.h"
#include "ags2/ac/platform.h"
#include "ags2/ac/resolution.h"
#include "ags2/ac/room.h"
#include "ags2/ac/run.h"
#include "ags2/ac/savegame.h"
#include "ags2/ac/sound.h"
#include "ags2/ac/timer.h"
#include "ags2/ac/video.h"
#include "ags2/ac/gfx.h"
#include "ags2/ac/scripts.h"
#include "ags2/ac/gui/gui.h"
#include "ags2/common/agsplugin.h"
#include "ags2/common/cscomp.h"
#include "ags2/common/events.h"
#include "ags2/common/mouse32.h"
#include "ags2/common/tree_map.h"
#include "ags2/common/dynamic_array.h"
#include "ags2/data/sound.h"
#include "ags2/lib/winalleg.h"
#include "ags2/lib/aastr-0.1.1/aastr.h"
#include "ags2/lib/allegro/digi.h"
#include "ags2/lib/allegro/midi.h"
#include "ags2/lib/allegro/keyboard.h"

namespace AGS2 {

#define IS_ANTIALIAS_SPRITES usetup.enable_antialiasing && (play.disable_antialiasing == 0)

#define MAX_TOPIC_HISTORY 50
#define DLG_OPTION_PARSER 99

#define ACI_VERSION_TEXT "ScummVM AGS2"

enum {
	DBG_NOIFACE			=     1,
	DBG_NODRAWSPRITES	=     2,
	DBG_NOOBJECTS		=     4,
	DBG_NOUPDATE		=     8,
	DBG_NOSFX			=  0x10,
	DBG_NOMUSIC			=  0x20,
	DBG_NOSCRIPT		=  0x40,
	DBG_DBGSCRIPT		=  0x80,
	DBG_DEBUGMODE		= 0x100,
	DBG_REGONLY			= 0x200,
	DBG_NOVIDEO			= 0x400
};

#define REC_MOUSECLICK 1
#define REC_MOUSEMOVE  2
#define REC_MOUSEDOWN  3
#define REC_KBHIT      4
#define REC_GETCH      5
#define REC_KEYDOWN    6
#define REC_MOUSEWHEEL 7
#define REC_SPEECHFINISHED 8
#define REC_ENDOFFILE  0x6f

enum {
	LOCTYPE_HOTSPOT = 1,
	LOCTYPE_CHAR    = 2,
	LOCTYPE_OBJ     = 3
};

#define UPDATE_MP3 \
   while (switching_away_from_game) { }\
   for (musicPollIterator = 0; musicPollIterator <= MAX_SOUND_CHANNELS; musicPollIterator++) { \
     if ((channels[musicPollIterator] != NULL) && (channels[musicPollIterator]->done == 0)) \
       channels[musicPollIterator]->poll();\
   }

extern int spritewidth[MAX_SPRITES],spriteheight[MAX_SPRITES];


// The text script's "mouse" struct
struct ScriptMouse {
	int x, y;
};

// The text script's "system" struct
struct ScriptSystem {
	int width, height;
	int coldepth;
	int os;
	int windowed;
	int vsync;
	int viewport_width, viewport_height;
	char aci_version[10];
	int reserved[5];  // so that future scripts don't overwrite data
};

struct EventHappened {
	int type;
	int data1, data2, data3;
	int player;
};

struct SpriteListEntry {
	IDriverDependantBitmap *bmp;
	block pic;
	int baseline;
	int x, y;
	int transparent;
	bool takesPriorityIfEqual;
	bool hasAlphaChannel;
};

// game setup, read in from CFG file
// this struct is redefined in acdialog.cpp, any changes might
// need to be reflected there
struct GameSetup {
	bool usevox = false;
	int screenres = 0;

	int digicard = DIGI_AUTODETECT, midicard = MIDI_AUTODETECT;
	int mod_player = 1;
	int textheight = 0;
	int mp3_player = 1;
	int want_letterbox = 0;
	int windowed = 0;
	int base_width = 3200, base_height = 200;
	short refresh = 0;
	bool no_speech_pack = false;
	bool enable_antialiasing = false;
	bool force_hicolor_mode = false;
	bool disable_exception_handling;
	bool enable_side_borders = true;
	char *data_files_dir = nullptr;
	const char *main_data_filename = "ac2game.dat";
	Common::String translation;
	const char *gfxFilterID = nullptr;
	const char *gfxDriverID = nullptr;
};

#define BASEWIDTH usetup.base_width
#define BASEHEIGHT usetup.base_height
#define TRANS_ALPHA_CHANNEL 20000
#define TRANS_OPAQUE        20001
#define TRANS_RUN_PLUGIN    20002

struct ScriptObject {
	int id;
	RoomObject *obj;
};

struct ScriptInvItem {
	int id;
	int reserved;
};

struct ScriptDialog {
	int id;
	int reserved;
};


// object-based File routine -- struct definition
#define scFileRead   1
#define scFileWrite  2
#define scFileAppend 3
extern const char *fopenModes[];

struct sc_File : ICCDynamicObject {
	Common::Stream *handle;

	virtual int Dispose(const char *address, bool force) {
		Close();
		delete this;
		return 1;
	}

	virtual const char *GetType() {
		return "File";
	}

	virtual int Serialize(const char *address, char *buffer, int bufsize) {
		// we cannot serialize an open file, so it will get closed
		return 0;
	}

	int OpenFile(const char *filename, int mode);
	void Close();

	sc_File() {
		handle = NULL;
	}
};


// stores cached info about the character
struct CharacterCache {
	block image;
	int sppic;
	int scaling;
	int inUse;
	short tintredwas, tintgrnwas, tintbluwas, tintamntwas;
	short lightlevwas, tintlightwas;
	// no mirroredWas is required, since the code inverts the sprite number
};

// stores cached object info
struct ObjectCache {
	block image;
	int   sppic;
	short tintredwas, tintgrnwas, tintbluwas, tintamntwas, tintlightwas;
	short lightlevwas, mirroredWas, zoomWas;
	// The following are used to determine if the character has moved
	int   xwas, ywas;
};

extern int our_eip;
struct TempEip {
	int oldval;
	TempEip(int newval) {
		oldval = our_eip;
		our_eip = newval;
	}
	~TempEip() {
		our_eip = oldval;
	}
};

struct PluginObjectReader {
	IAGSManagedObjectReader *reader;
	const char *type;
};

#ifndef _AGS_PLUGIN_H
#undef IAGSManagedObjectReader

// If you change this, also need to update agsplugin.h
class IAGSFontRenderer {
public:
	virtual bool LoadFromDisk(int fontNumber, int fontSize) = 0;
	virtual void FreeMemory(int fontNumber) = 0;
	virtual bool SupportsExtendedCharacters(int fontNumber) = 0;
	virtual int GetTextWidth(const char *text, int fontNumber) = 0;
	virtual int GetTextHeight(const char *text, int fontNumber) = 0;
	virtual void RenderText(const char *text, int fontNumber, BITMAP *destination, int x, int y, int colour) = 0;
	virtual void AdjustYCoordinateForFont(int *ycoord, int fontNumber) = 0;
	virtual void EnsureTextValidForFont(char *text, int fontNumber) = 0;
};
#endif

extern GFXFilter *filter;

extern IAGSFontRenderer* fontRenderers[MAX_FONTS];

#define DEBUG_CONSOLE if (play.debug_mode) debug_write_console

#define NUM_DIGI_VOICES     16
#define NUM_MOD_DIGI_VOICES 12

#define DEBUG_CONSOLE_NUMLINES 6
#define TXT_SCOREBAR        29
#define MAXSCORE play.totalscore
#define CHANIM_REPEAT    2
#define CHANIM_BACKWARDS 4
#define ANIM_BACKWARDS 10
#define ANIM_ONCE      1
#define ANIM_REPEAT    2
#define ANIM_ONCERESET 3
#define FONT_STATUSBAR  0
#define FONT_NORMAL     play.normal_font
//#define FONT_SPEECHBACK 1
#define FONT_SPEECH     play.speech_font
#define MODE_WALK 0
#define MODE_LOOK 1
#define MODE_HAND 2
#define MODE_TALK 3
#define MODE_USE  4
#define MODE_PICKUP 5
#define CURS_ARROW  6
#define CURS_WAIT   7
#define MODE_CUSTOM1 8
#define MODE_CUSTOM2 9

#define OVER_TEXTMSG  1
#define OVER_COMPLETE 2
#define OVER_PICTURE  3
#define OVER_CUSTOM   100
#define OVR_AUTOPLACE 30000
#define FOR_ANIMATION 1
#define FOR_SCRIPT    2
#define FOR_EXITLOOP  3
#define opts usetup
#define CHMLSOFFS (MAX_INIT_SPR+1)    // reserve this many movelists for objects & stuff
#define MAX_SCREEN_OVERLAYS 20
#define abort_all_conditions restrict_until
#define MAX_SCRIPT_AT_ONCE 10
#define EVENT_NONE       0
#define EVENT_INPROGRESS 1
#define EVENT_CLAIMED    2

#define SKIP_AUTOTIMER  1
#define SKIP_KEYPRESS   2
#define SKIP_MOUSECLICK 4

#define UNTIL_ANIMEND   1
#define UNTIL_MOVEEND   2
#define UNTIL_CHARIS0   3
#define UNTIL_NOOVERLAY 4
#define UNTIL_NEGATIVE  5
#define UNTIL_INTIS0    6
#define UNTIL_SHORTIS0  7
#define UNTIL_INTISNEG  8
#define MANOBJNUM 99

#define STD_BUFFER_SIZE 3000

#define TURNING_AROUND     1000
#define TURNING_BACKWARDS 10000

#define MAXLINE 50
#define MAX_SOUND_CHANNELS 8
#define SPECIAL_CROSSFADE_CHANNEL 8
#define MAX_PLUGIN_OBJECT_READERS 50

#define NEXT_ITERATION() play.gamestep++

#if 0
extern GameSetupStruct game;
extern GameState play;
extern GameSetup usetup;
extern MoveList *mls;
extern ViewStruct *views;
extern int displayed_room;
extern int final_scrn_wid,final_scrn_hit,final_col_dep;
extern int in_enters_screen, done_es_error;
extern int new_room_pos, new_room_x, new_room_y;
extern int scrnwid, scrnhit;
extern int cur_mode,cur_cursor;
extern block wallscreen;
extern AmbientSound ambient[MAX_SOUND_CHANNELS + 1];
extern int lastcx,lastcy;
extern int guis_need_update;
extern int use_cdplayer, need_to_stop_cd;
extern ScreenOverlay screenover[MAX_SCREEN_OVERLAYS];
extern ScriptInvItem scrInv[MAX_INV];
extern color palette[256];
extern block virtual_screen; 
extern int pluginsWantingDebugHooks;
extern int pluginsWantingAudioHooks;
extern char lines[MAXLINE][200];
extern int  numlines,longestline;
extern int offsetx,offsety;     // for scumm-type scrolling rooms
extern int game_paused;
extern int inside_script;
extern int numPluginReaders;
extern int our_eip;
extern ccInstance *gameinst, *roominst;
extern RoomStatus *croom;
extern SpriteCache spriteset;
extern SOUNDCLIP *channels[MAX_SOUND_CHANNELS+1];
extern int last_sound_played[MAX_SOUND_CHANNELS+1];
extern CharacterCache *charcache;
extern ObjectCache objcache[MAX_INIT_SPR];
extern ExecutingScript *curscript;
extern PluginObjectReader pluginReaders[MAX_PLUGIN_OBJECT_READERS];
#endif

#define DOMOUSE_NOCURSOR 5
#define NONE -1
#define LEFT  0
#define RIGHT 1
extern int  mousex,mousey;
extern void domouse(int);
extern int  mgetbutton();

extern const char *get_engine_version();
extern void quitprintf(const char *texx, ...) ;
extern void RefreshMouse();
extern void PluginSimulateMouseClick(int pluginButtonID);
extern int  run_script_function_if_exist(ccInstance *sci, const char *tsname, int numParam, int iparam, int iparam2, int iparam3 = 0) ;
extern int  IsChannelPlaying(int chan) ;
extern void stop_and_destroy_channel (int chid) ;
extern int  rec_kbhit();
extern int  rec_getch();
extern void update_polled_stuff();
extern void invalidate_rect(int x1, int y1, int x2, int y2);
extern int  find_word_in_dictionary (char*);
extern void break_up_text_into_lines(int wii,int fonnt,char*todis) ;
extern int  wgetfontheight(int font);
extern void draw_and_invalidate_text(int x1, int y1, int font, const char *text);
extern void scriptDebugHook (ccInstance *ccinst, int linenum);
extern void invalidate_screen();
extern int  is_valid_character(int newchar);
extern int  is_valid_object(int obtest);
extern void debug_write_console (const char *msg, ...);
extern int  is_route_possible(int,int,int,int,block);
extern int  find_route(short,short,short,short,block,int,int=0,int=0);
extern void set_route_move_speed(int x, int y);
extern void init_pathfinder();
extern block prepare_walkable_areas (int sourceChar);
extern void do_main_cycle(int,int);
extern void add_inventory(int inum);
extern void lose_inventory(int inum);
extern void animate_character(CharacterInfo *,int,int,int,int = 0, int = 0);
extern void calculate_move_stage (MoveList *, int );
extern void EndSkippingUntilCharStops();
extern void MoveToWalkableArea(int charid);
extern void Display(const char *, ...);
extern int  do_movelist_move(short*,int*,int*);
extern int  is_char_on_another (int sourceChar, int ww, int*fromxptr, int*cwidptr);
extern int  find_looporder_index (int curloop);
extern int  doNextCharMoveStep (int aa, CharacterInfo *chi);
extern int  useDiagonal (CharacterInfo *char1);
extern void FaceCharacter(int cha,int toface);
extern void NewRoom(int);
extern void debug_log(char*texx, ...);
extern int  GetCharacterWidth(int ww);
extern block GetObjectImage(int obj, int *isFlipped);
extern block GetCharacterImage(int charid, int *isFlipped);
extern int  my_getpixel(BITMAP *blk, int x, int y);
extern void update_invorder();
extern int  GetRegionAt (int xxx, int yyy);
extern void SetNextCursor();
extern void SetActiveInventory(int iit);
extern void _DisplaySpeechCore(int chid, char *displbuf);
extern void _DisplayThoughtCore(int chid, const char *displbuf);
extern int  DisplaySpeechBackground(int charid,char*speel);
extern void DisplaySpeechAt (int xx, int yy, int wii, int aschar, char*spch);
extern char *get_translation(const char*);
extern int  can_see_from(int,int,int,int);
extern int  GetCursorMode();
extern void disable_cursor_mode(int);
extern void enable_cursor_mode(int);
extern void set_cursor_mode(int);
extern void update_inv_cursor(int invnum);
extern void setup_player_character(int charid);
extern void my_sprintf(char *buffer, const char *fmt, va_list ap);
extern int  find_overlay_of_type(int typ);
extern void run_on_event (int evtype, int wparam);
extern int  get_character_currently_talking();
extern void shutdown_sound();
extern void update_music_volume();
extern void newmusic(int);
extern void PlayAmbientSound (int channel, int sndnum, int vol, int x, int y);
extern void* ccGetSymbolAddress (char*);
extern int GetScalingAt (int x, int y) ;
extern int wgettextwidth_compensate(const char *tex, int font) ;
extern void add_dynamic_sprite (int gotSlot, block redin, bool hasAlpha = false);
extern void free_dynamic_sprite (int gotSlot);
const char* CreateNewScriptString(const char *fromText, bool reAllocate = true);
extern void convert_move_path_to_high_res(MoveList *ml);
extern void register_audio_script_objects();
extern void register_audio_script_functions();
extern bool unserialize_audio_script_object(int index, const char *objectType, const char *serializedData, int dataSize);
extern void audio_update_polled_stuff();
extern ScriptAudioChannel* play_audio_clip_on_channel(int channel, ScriptAudioClip *clip, int priority, int repeat, int fromOffset, SOUNDCLIP *cachedClip = NULL);
extern SOUNDCLIP *load_sound_clip_from_old_style_number(bool isMusic, int indexNumber, bool repeat);
extern void play_audio_clip_by_index(int audioClipIndex);
extern ScriptAudioClip* get_audio_clip_for_old_style_number(bool isMusic, int indexNumber);
extern void Game_StopAudio(int audioType);
extern void Game_SetAudioTypeVolume(int audioType, int volume, int changeType);
extern void update_directional_sound_vol();
extern void recache_queued_clips_after_loading_save_game();

#define AMBIENCE_FULL_DIST 25

// parameters to run_on_event
#define GE_LEAVE_ROOM 1
#define GE_ENTER_ROOM 2
#define GE_MAN_DIES   3
#define GE_GOT_SCORE  4
#define GE_GUI_MOUSEDOWN 5
#define GE_GUI_MOUSEUP   6
#define GE_ADD_INV       7
#define GE_LOSE_INV      8
#define GE_RESTORE_GAME  9

// These numbers were chosen arbitrarily -- the idea is
// to make sure that the user gets the parameters the right way round
#define ANYWHERE       304
#define WALKABLE_AREAS 305
#define BLOCKING       919
#define IN_BACKGROUND  920
#define FORWARDS       1062
#define BACKWARDS      1063
#define VOL_CHANGEEXISTING   1678
#define VOL_SETFUTUREDEFAULT 1679
#define VOL_BOTH             1680
#define SCR_NO_VALUE   31998
#define SCR_COLOR_TRANSPARENT -1

// Character methods
extern void Character_AddInventory(CharacterInfo *chaa, ScriptInvItem *, int addIndex);
extern void Character_AddWaypoint(CharacterInfo *chaa, int x, int y);
extern void Character_Animate(CharacterInfo *chaa, int loop, int delay, int repeat, int direction, int blocking);
extern void Character_ChangeRoom(CharacterInfo *chaa, int room, int x, int y);
extern void Character_ChangeRoomAutoPosition(CharacterInfo *chaa, int room, int newPos);
extern void Character_ChangeView(CharacterInfo *chap, int vii);
extern void Character_FaceCharacter(CharacterInfo *char1, CharacterInfo *otherChar, int blockingStyle);
extern void Character_FaceLocation(CharacterInfo *char1, int xx, int yy, int blockingStyle);
extern void Character_FaceObject(CharacterInfo *char1, ScriptObject* obj, int blockingStyle);
extern void Character_FollowCharacter(CharacterInfo *chaa, CharacterInfo *tofollow, int distaway, int eagerness);
extern int  Character_GetProperty(CharacterInfo *chaa, const char *property);
extern void Character_GetPropertyText(CharacterInfo *chaa, const char *property, char *bufer);
extern int  Character_HasInventory(CharacterInfo *chaa, ScriptInvItem *);
extern int  Character_IsCollidingWithChar(CharacterInfo *char1, CharacterInfo *cchar2) ;
extern int  Character_IsCollidingWithObject(CharacterInfo *chin, ScriptObject *objid);
extern void Character_LockView(CharacterInfo *chap, int vii);
extern void Character_LockViewAligned(CharacterInfo *chap, int vii, int loop, int align);
extern void Character_LockViewFrame(CharacterInfo *chaa, int view, int loop, int frame);
extern void Character_LockViewOffset(CharacterInfo *chap, int vii, int xoffs, int yoffs);
extern void Character_LoseInventory(CharacterInfo *chap, ScriptInvItem *);
extern void Character_Move(CharacterInfo *chaa, int x, int y, int direct, int blocking);
extern void Character_PlaceOnWalkableArea(CharacterInfo *chap);
extern void Character_RemoveTint(CharacterInfo *chaa);
extern void Character_RunInteraction(CharacterInfo *chaa, int mood);
extern void Character_Say(CharacterInfo *chaa, const char *texx, ...);
extern void Character_SayAt(CharacterInfo *chaa, int x, int y, int width, const char *texx);
extern ScriptOverlay* Character_SayBackground(CharacterInfo *chaa, const char *texx);
extern void Character_SetAsPlayer(CharacterInfo *chaa);
extern void Character_SetIdleView(CharacterInfo *chaa, int iview, int itime);
extern void Character_SetOption(CharacterInfo *chaa, int option, int yesorno);
extern void Character_SetSpeed(CharacterInfo *chaa, int xspeed, int yspeed);
extern void Character_StopMoving(CharacterInfo *charp);
extern void Character_Think(CharacterInfo *chaa, const char *texx, ...);
extern void Character_Tint(CharacterInfo *chaa, int red, int green, int blue, int opacity, int luminance);
extern void Character_UnlockView(CharacterInfo *chaa);
extern void Character_Walk(CharacterInfo *chaa, int x, int y, int direct, int blocking);
extern void Character_WalkStraight(CharacterInfo *chaa, int x, int y, int blocking);

// Character properties
extern ScriptInvItem* Character_GetActiveInventory(CharacterInfo *chaa);
extern void Character_SetActiveInventory(CharacterInfo *chaa, ScriptInvItem *iit);
extern int  Character_GetAnimating(CharacterInfo *chaa);
extern int  Character_GetAnimationSpeed(CharacterInfo *chaa) ;
extern void Character_SetAnimationSpeed(CharacterInfo *chaa, int newval);
extern int  Character_GetBaseline(CharacterInfo *chaa);
extern void Character_SetBaseline(CharacterInfo *chaa, int basel);
extern int  Character_GetBlinkInterval(CharacterInfo *chaa);
extern void Character_SetBlinkInterval(CharacterInfo *chaa, int interval);
extern int  Character_GetBlinkView(CharacterInfo *chaa);
extern void Character_SetBlinkView(CharacterInfo *chaa, int vii);
extern int  Character_GetBlinkWhileThinking(CharacterInfo *chaa);
extern void Character_SetBlinkWhileThinking(CharacterInfo *chaa, int yesOrNo);
extern int  Character_GetBlockingHeight(CharacterInfo *chaa);
extern void Character_SetBlockingHeight(CharacterInfo *chaa, int hit);
extern int  Character_GetBlockingWidth(CharacterInfo *chaa);
extern void Character_SetBlockingWidth(CharacterInfo *chaa, int wid);
extern int  Character_GetClickable(CharacterInfo *chaa);
extern void Character_SetClickable(CharacterInfo *chaa, int clik);
extern int  Character_GetDiagonalWalking(CharacterInfo *chaa);
extern void Character_SetDiagonalWalking(CharacterInfo *chaa, int yesorno);
extern int  Character_GetFrame(CharacterInfo *chaa);
extern void Character_SetFrame(CharacterInfo *chaa, int newval);
extern int  Character_GetHasExplicitTint(CharacterInfo *chaa);
extern int  Character_GetID(CharacterInfo *chaa);
extern int  Character_GetIdleView(CharacterInfo *chaa);
extern int  Character_GetIInventoryQuantity(CharacterInfo *chaa, int index);
extern void Character_SetIInventoryQuantity(CharacterInfo *chaa, int index, int quantity);
extern int  Character_GetIgnoreLighting(CharacterInfo *chaa);
extern void Character_SetIgnoreLighting(CharacterInfo *chaa, int yesorno);
extern int  Character_GetIgnoreScaling(CharacterInfo *chaa);
extern void Character_SetIgnoreScaling(CharacterInfo *chaa, int yesorno);
extern int  Character_GetIgnoreWalkbehinds(CharacterInfo *chaa);
extern void Character_SetIgnoreWalkbehinds(CharacterInfo *chaa, int yesorno);
extern int  Character_GetLoop(CharacterInfo *chaa);
extern void Character_SetLoop(CharacterInfo *chaa, int newval);
extern void Character_SetManualScaling(CharacterInfo *chaa, int yesorno);
extern int  Character_GetMovementLinkedToAnimation(CharacterInfo *chaa);
extern void Character_SetMovementLinkedToAnimation(CharacterInfo *chaa, int yesorno);
extern int  Character_GetMoving(CharacterInfo *chaa);
extern const char* Character_GetName(CharacterInfo *chaa);
extern void Character_SetName(CharacterInfo *chaa, const char *newName);
extern int  Character_GetNormalView(CharacterInfo *chaa);
extern int  Character_GetPreviousRoom(CharacterInfo *chaa);
extern int  Character_GetRoom(CharacterInfo *chaa);
extern int  Character_GetScaleMoveSpeed(CharacterInfo *chaa);
extern void Character_SetScaleMoveSpeed(CharacterInfo *chaa, int yesorno);
extern int  Character_GetScaleVolume(CharacterInfo *chaa);
extern void Character_SetScaleVolume(CharacterInfo *chaa, int yesorno);
extern int  Character_GetScaling(CharacterInfo *chaa);
extern void Character_SetScaling(CharacterInfo *chaa, int zoomlevel);
extern int  Character_GetSolid(CharacterInfo *chaa);
extern void Character_SetSolid(CharacterInfo *chaa, int yesorno);
extern int  Character_GetSpeaking(CharacterInfo *chaa);
extern int  GetCharacterSpeechAnimationDelay(CharacterInfo *cha);
extern void Character_SetSpeechAnimationDelay(CharacterInfo *chaa, int newDelay);
extern int  Character_GetSpeechColor(CharacterInfo *chaa);
extern void Character_SetSpeechColor(CharacterInfo *chaa, int ncol);
extern int  Character_GetSpeechView(CharacterInfo *chaa);
extern void Character_SetSpeechView(CharacterInfo *chaa, int vii);
extern int  Character_GetThinkView(CharacterInfo *chaa);
extern void Character_SetThinkView(CharacterInfo *chaa, int vii);
extern int  Character_GetTransparency(CharacterInfo *chaa);
extern void Character_SetTransparency(CharacterInfo *chaa, int trans);
extern int  Character_GetTurnBeforeWalking(CharacterInfo *chaa);
extern void Character_SetTurnBeforeWalking(CharacterInfo *chaa, int yesorno);
extern int  Character_GetView(CharacterInfo *chaa);
extern int  Character_GetWalkSpeedX(CharacterInfo *chaa);
extern int  Character_GetWalkSpeedY(CharacterInfo *chaa);
extern int  Character_GetX(CharacterInfo *chaa);
extern void Character_SetX(CharacterInfo *chaa, int newval);
extern int  Character_GetY(CharacterInfo *chaa);
extern void Character_SetY(CharacterInfo *chaa, int newval);
extern int  Character_GetZ(CharacterInfo *chaa);
extern void Character_SetZ(CharacterInfo *chaa, int newval);

extern void GetLocationName(int xxx, int yyy, char *tempo);

#ifdef WINDOWS_VERSION
#define AGS_INLINE inline
#else
// the linux compiler won't allow extern inline
#define AGS_INLINE
#endif

extern void invalidate_sprite(int x1, int y1, IDriverDependantBitmap *pic);
extern void draw_fps();
extern int _display_main(int xx, int yy, int wii, char *todis, int blocking, int usingfont, int asspch, int isThought, int allowShrink, bool overlayPositionFixed);
extern int GetTextDisplayTime(const char *text, int canberel = 0);
extern void mark_current_background_dirty();
extern void debug_log(const char *texx, ...);
extern void draw_sprite_support_alpha(int xpos, int ypos, block image, int slot);
extern void run_room_event(int id);
extern void load_new_room(int newnum, CharacterInfo *forchar);
extern void unload_old_room();
extern void construct_virtual_screen(bool fullRedraw);
extern void DisplayMessage(int msnum);
extern void setup_for_dialog();
extern int IsMusicPlaying();
extern void current_fade_out_effect();
extern void StopAmbientSound(int channel);

} // namespace AGS2

#endif
