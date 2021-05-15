/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

//
// Game loop
//

#include "ags/lib/std/limits.h"
//include <chrono>
//include <SDL.h>
#include "ags/shared/ac/common.h"
#include "ags/shared/ac/characterextras.h"
#include "ags/shared/ac/character_info.h"
#include "ags/shared/ac/draw.h"
#include "ags/shared/ac/event.h"
#include "ags/engine/ac/game.h"
#include "ags/engine/ac/game_setup.h"
#include "ags/shared/ac/game_setup_struct.h"
#include "ags/engine/ac/game_state.h"
#include "ags/shared/ac/global_debug.h"
#include "ags/shared/ac/global_display.h"
#include "ags/shared/ac/global_game.h"
#include "ags/shared/ac/global_gui.h"
#include "ags/shared/ac/global_region.h"
#include "ags/shared/ac/gui.h"
#include "ags/shared/ac/hotspot.h"
#include "ags/shared/ac/keycode.h"
#include "ags/shared/ac/mouse.h"
#include "ags/engine/ac/overlay.h"
#include "ags/shared/ac/sys_events.h"
#include "ags/shared/ac/room.h"
#include "ags/engine/ac/room_object.h"
#include "ags/shared/ac/roomstatus.h"
#include "ags/shared/debugging/debugger.h"
#include "ags/engine/debugging/debug_log.h"
#include "ags/shared/gui/guiinv.h"
#include "ags/shared/gui/gui_main.h"
#include "ags/shared/gui/guitextbox.h"
#include "ags/shared/main/mainheader.h"
#include "ags/shared/main/engine.h"
#include "ags/shared/main/game_run.h"
#include "ags/shared/main/update.h"
#include "ags/shared/plugin/agsplugin.h"
#include "ags/shared/plugin/plugin_engine.h"
#include "ags/shared/script/script.h"
#include "ags/shared/ac/sprite_cache.h"
#include "ags/engine/media/audio/audio_system.h"
#include "ags/shared/platform/base/agsplatformdriver.h"
#include "ags/engine/ac/timer.h"
#include "ags/shared/ac/keycode.h"

namespace AGS3 {

using namespace AGS::Shared;



extern int mouse_on_iface;   // mouse cursor is over this interface
extern int ifacepopped;
extern int is_text_overlay;
extern volatile char want_exit, abort_engine;
extern int proper_exit, our_eip;
extern int displayed_room, starting_room, in_new_room, new_room_was;


extern int game_paused;
extern int getloctype_index;
extern int _G(in_enters_screen), _G(done_es_error);
extern int _G(in_leaves_screen);
extern int inside_script, in_graph_script;
extern int no_blocking_functions;

extern int mouse_ifacebut_xoffs, mouse_ifacebut_yoffs;
extern int cur_mode;
extern RoomObject *objs;
extern char noWalkBehindsAtAll;
extern RoomStatus *croom;

extern SpriteCache _GP(spriteset);
extern int cur_mode, cur_cursor;
extern char _G(check_dynamic_sprites_at_exit);

// Checks if user interface should remain disabled for now
static int ShouldStayInWaitMode();

static int numEventsAtStartOfFunction;
static auto t1 = AGS_Clock::now();  // timer for FPS // ... 't1'... how very appropriate.. :)

static int user_disabled_for = 0;
static const void *user_disabled_data = nullptr;

#define UNTIL_ANIMEND   1
#define UNTIL_MOVEEND   2
#define UNTIL_CHARIS0   3
#define UNTIL_NOOVERLAY 4
#define UNTIL_NEGATIVE  5
#define UNTIL_INTIS0    6
#define UNTIL_SHORTIS0  7
#define UNTIL_INTISNEG  8

static int restrict_until = 0;

unsigned int loopcounter = 0;
static unsigned int lastcounter = 0;

static void ProperExit() {
	want_exit = 0;
	proper_exit = 1;
	quit("||exit!");
}

static void game_loop_check_problems_at_start() {
	if ((_G(in_enters_screen) != 0) & (displayed_room == starting_room))
		quit("!A text script run in the Player Enters Screen event caused the\n"
			"screen to be updated. If you need to use Wait(), do so in After Fadein");
	if ((_G(in_enters_screen) != 0) && (_G(done_es_error) == 0)) {
		debug_script_warn("Wait() was used in Player Enters Screen - use Enters Screen After Fadein instead");
		_G(done_es_error) = 1;
	}
	if (no_blocking_functions)
		quit("!A blocking function was called from within a non-blocking event such as " REP_EXEC_ALWAYS_NAME);
}

static void game_loop_check_new_room() {
	if (in_new_room == 0) {
		// Run the room and game script repeatedly_execute
		run_function_on_non_blocking_thread(&repExecAlways);
		setevent(EV_TEXTSCRIPT, TS_REPEAT);
		setevent(EV_RUNEVBLOCK, EVB_ROOM, 0, 6);
	}
	// run this immediately to make sure it gets done before fade-in
	// (player enters screen)
	check_new_room();
}

static void game_loop_do_late_update() {
	if (in_new_room == 0) {
		// Run the room and game script late_repeatedly_execute
		run_function_on_non_blocking_thread(&lateRepExecAlways);
	}
}

static int game_loop_check_ground_level_interactions() {
	if ((_GP(play).ground_level_areas_disabled & GLED_INTERACTION) == 0) {
		// check if he's standing on a hotspot
		int hotspotThere = get_hotspot_at(_G(_G(playerchar))->x, _G(_G(playerchar))->y);
		// run Stands on Hotspot event
		setevent(EV_RUNEVBLOCK, EVB_HOTSPOT, hotspotThere, 0);

		// check current region
		int onRegion = GetRegionIDAtRoom(_G(_G(playerchar))->x, _G(_G(playerchar))->y);
		int inRoom = displayed_room;

		if (onRegion != _GP(play).player_on_region) {
			// we need to save this and set _GP(play).player_on_region
			// now, so it's correct going into RunRegionInteraction
			int oldRegion = _GP(play).player_on_region;

			_GP(play).player_on_region = onRegion;
			// Walks Off last region
			if (oldRegion > 0)
				RunRegionInteraction(oldRegion, 2);
			// Walks Onto new region
			if (onRegion > 0)
				RunRegionInteraction(onRegion, 1);
		}
		if (_GP(play).player_on_region > 0)   // player stands on region
			RunRegionInteraction(_GP(play).player_on_region, 0);

		// one of the region interactions sent us to another room
		if (inRoom != displayed_room) {
			check_new_room();
		}

		// if in a Wait loop which is no longer valid (probably
		// because the Region interaction did a NewRoom), abort
		// the rest of the loop
		if ((restrict_until) && (!ShouldStayInWaitMode())) {
			// cancel the Rep Exec and Stands on Hotspot events that
			// we just added -- otherwise the event queue gets huge
			_G(numevents) = numEventsAtStartOfFunction;
			return 0;
		}
	} // end if checking ground level interactions

	return RETURN_CONTINUE;
}

static void lock_mouse_on_click() {
	if (usetup.mouse_auto_lock && _GP(scsystem).windowed)
		Mouse::TryLockToWindow();
}

static void toggle_mouse_lock() {
	if (_GP(scsystem).windowed) {
		if (Mouse::IsLockedToWindow())
			Mouse::UnlockFromWindow();
		else
			Mouse::TryLockToWindow();
	}
}

// Runs default mouse button handling
static void check_mouse_controls() {
	int mongu = -1;

	mongu = gui_on_mouse_move();

	mouse_on_iface = mongu;
	if ((ifacepopped >= 0) && (mousey >= _GP(guis)[ifacepopped].Y + _GP(guis)[ifacepopped].Height))
		remove_popup_interface(ifacepopped);

	// check mouse clicks on GUIs
	static int wasbutdown = 0, wasongui = 0;

	if ((wasbutdown > 0) && (ags_misbuttondown(wasbutdown - 1))) {
		gui_on_mouse_hold(wasongui, wasbutdown);
	} else if ((wasbutdown > 0) && (!ags_misbuttondown(wasbutdown - 1))) {
		gui_on_mouse_up(wasongui, wasbutdown);
		wasbutdown = 0;
	}

	int mbut = MouseNone;
	int mwheelz = 0;
	if (run_service_mb_controls(mbut, mwheelz) && mbut >= 0) {

		check_skip_cutscene_mclick(mbut);

		if (_GP(play).fast_forward || _GP(play).IsIgnoringInput()) { /* do nothing if skipping cutscene or input disabled */
		} else if ((_GP(play).wait_counter > 0) && (_GP(play).key_skip_wait & SKIP_MOUSECLICK) != 0)
			_GP(play).wait_counter = -1;
		else if (is_text_overlay > 0) {
			if (_GP(play).cant_skip_speech & SKIP_MOUSECLICK)
				remove_screen_overlay(OVER_TEXTMSG);
		} else if (!IsInterfaceEnabled());  // blocking cutscene, ignore mouse
		else if (pl_run_plugin_hooks(AGSE_MOUSECLICK, mbut + 1)) {
			// plugin took the click
			debug_script_log("Plugin handled mouse button %d", mbut + 1);
		} else if (mongu >= 0) {
			if (wasbutdown == 0) {
				gui_on_mouse_down(mongu, mbut + 1);
			}
			wasongui = mongu;
			wasbutdown = mbut + 1;
		} else setevent(EV_TEXTSCRIPT, TS_MCLICK, mbut + 1);
		//    else RunTextScriptIParam(gameinst,"on_mouse_click",aa+1);
	}

	if (mwheelz < 0)
		setevent(EV_TEXTSCRIPT, TS_MCLICK, 9);
	else if (mwheelz > 0)
		setevent(EV_TEXTSCRIPT, TS_MCLICK, 8);
}



// Special flags to OR saved SDL_Keymod flags with:
// Mod key combination already fired (wait until full mod release)
#define KEY_MODS_FIRED      0x80000000

int cur_key_mods = 0;
int old_key_mod = 0; // for saving previous key mods

// Runs service key controls, returns false if service key combinations were handled
// and no more processing required, otherwise returns true and provides current keycode and key shifts.
bool run_service_key_controls(int &out_key) {
	bool handled = false;
	const bool key_valid = ags_keyevent_ready();
	const SDL_Event key_evt = key_valid ? ags_get_next_keyevent() : SDL_Event();
	const bool is_only_mod_key = key_evt.type == SDL_KEYDOWN ? is_mod_key(key_evt.key.keysym) : false;

	// Following section is for testing for pushed and released mod-keys.
	// A bit of explanation: some service actions may require combination of
	// mod-keys, for example [Ctrl + Alt] toggles mouse lock in window.
	// Here comes a problem: other actions may also use [Ctrl + Alt] mods in
	// combination with a third key: e.g. [Ctrl + Alt + V] displays engine info.
	// For this reason we cannot simply test for pressed Ctrl and Alt here,
	// but we must wait until player *releases at least one mod key* of this combo,
	// while no third key was pressed.
	// In other words, such action should only trigger if:
	// * if combination of held down mod-keys was gathered,
	// * if no other key was pressed meanwhile,
	// * if at least one of those gathered mod-keys was released.
	//
	// TODO: maybe split this mod handling into sep procedure and make it easier to use (not that it's used alot)?

	// First, check mods
	const int cur_mod = make_merged_mod(SDL_GetModState());

	// If shifts combination have already triggered an action, then do nothing
	// until new shifts are empty, in which case reset saved shifts
	if (old_key_mod & KEY_MODS_FIRED) {
		if (cur_mod == 0)
			old_key_mod = 0;
	} else {
		// If any non-mod key is pressed, add fired flag to indicate that
		// this is no longer a pure mod keys combination
		if (key_valid && !is_only_mod_key) {
			old_key_mod = cur_mod | KEY_MODS_FIRED;
		}
		// If all the previously registered mods are still pressed,
		// then simply resave new mods state.
		else if ((old_key_mod & cur_mod) == old_key_mod) {
			old_key_mod = cur_mod;
		}
		// Otherwise some of the mods were released, then run key combo action
		// and set KEY_MODS_FIRED flag to prevent multiple execution
		else if (old_key_mod) {
			// Toggle mouse lock on Ctrl + Alt
			if (old_key_mod == (KMOD_CTRL | KMOD_ALT)) {
				toggle_mouse_lock();
				handled = true;
			}
			old_key_mod |= KEY_MODS_FIRED;
		}
	}
	cur_key_mods = cur_mod;

	if (!key_valid)
		return false; // if there was no key press, finish after handling current mod state
	if (is_only_mod_key || handled)
		return false; // rest of engine currently does not use pressed mod keys
					  // change this when it's no longer true (but be mindful about key-skipping!)

	int agskey = ags_keycode_from_sdl(key_evt);
	if (agskey == eAGSKeyCodeNone)
		return false; // should skip this key event

	// LAlt or RAlt + Enter/Return
	if ((cur_mod == KMOD_ALT) && agskey == eAGSKeyCodeReturn) {
		engine_try_switch_windowed_gfxmode();
		return false;
	}

	// Alt+X, abort (but only once game is loaded)
	if ((displayed_room >= 0) && (_GP(play).abort_key > 0 && agskey == _GP(play).abort_key)) {
		_G(check_dynamic_sprites_at_exit) = 0;
		quit("!|");
	}

	// debug console
	if ((agskey == '`') && (_GP(play).debug_mode > 0)) {
		_G(display_console) = !_G(display_console);
		return false;
	}

	if ((agskey == eAGSKeyCodeCtrlE) && (_G(display_fps) == kFPS_Forced)) {
		// if --fps paramter is used, Ctrl+E will max out frame rate
		setTimerFps(isTimerFpsMaxed() ? frames_per_second : 1000);
		return false;
	}

	if ((agskey == eAGSKeyCodeCtrlD) && (_GP(play).debug_mode > 0)) {
		// ctrl+D - show info
		char infobuf[900];
		int ff;
		// MACPORT FIX 9/6/5: added last %s
		sprintf(infobuf, "In room %d %s[Player at %d, %d (view %d, loop %d, frame %d)%s%s%s",
			displayed_room, (noWalkBehindsAtAll ? "(has no walk-behinds)" : ""), _G(_G(playerchar))->x, _G(_G(playerchar))->y,
			_G(_G(playerchar))->view + 1, _G(_G(playerchar))->loop, _G(_G(playerchar))->frame,
			(IsGamePaused() == 0) ? "" : "[Game paused.",
			(_GP(play).ground_level_areas_disabled == 0) ? "" : "[Ground areas disabled.",
			(IsInterfaceEnabled() == 0) ? "[Game in Wait state" : "");
		for (ff = 0; ff < croom->numobj; ff++) {
			if (ff >= 8) break; // buffer not big enough for more than 7
			sprintf(&infobuf[strlen(infobuf)],
				"[Object %d: (%d,%d) size (%d x %d) on:%d moving:%s animating:%d slot:%d trnsp:%d clkble:%d",
				ff, objs[ff].x, objs[ff].y,
				(_GP(spriteset)[objs[ff].num] != nullptr) ? _GP(game).SpriteInfos[objs[ff].num].Width : 0,
				(_GP(spriteset)[objs[ff].num] != nullptr) ? _GP(game).SpriteInfos[objs[ff].num].Height : 0,
				objs[ff].on,
				(objs[ff].moving > 0) ? "yes" : "no", objs[ff].cycling,
				objs[ff].num, objs[ff].transparent,
				((objs[ff].flags & OBJF_NOINTERACT) != 0) ? 0 : 1);
		}
		Display(infobuf);
		int chd = _GP(game).playercharacter;
		char bigbuffer[STD_BUFFER_SIZE] = "CHARACTERS IN THIS ROOM:[";
		for (ff = 0; ff < _GP(game).numcharacters; ff++) {
			if (_GP(game).chars[ff].room != displayed_room) continue;
			if (strlen(bigbuffer) > 430) {
				strcat(bigbuffer, "and more...");
				Display(bigbuffer);
				strcpy(bigbuffer, "CHARACTERS IN THIS ROOM (cont'd):[");
			}
			chd = ff;
			sprintf(&bigbuffer[strlen(bigbuffer)],
				"%s (view/loop/frm:%d,%d,%d  x/y/z:%d,%d,%d  idleview:%d,time:%d,left:%d walk:%d anim:%d follow:%d flags:%X wait:%d zoom:%d)[",
				_GP(game).chars[chd].scrname, _GP(game).chars[chd].view + 1, _GP(game).chars[chd].loop, _GP(game).chars[chd].frame,
				_GP(game).chars[chd].x, _GP(game).chars[chd].y, _GP(game).chars[chd].z,
				_GP(game).chars[chd].idleview, _GP(game).chars[chd].idletime, _GP(game).chars[chd].idleleft,
				_GP(game).chars[chd].walking, _GP(game).chars[chd].animating, _GP(game).chars[chd].following,
				_GP(game).chars[chd].flags, _GP(game).chars[chd].wait, _G(charextra)[chd].zoom);
		}
		Display(bigbuffer);
		return false;
	}

	if (((agskey == eAGSKeyCodeCtrlV) && (cur_key_mods & KMOD_ALT) != 0)
		&& (_GP(play).wait_counter < 1) && (is_text_overlay == 0) && (restrict_until == 0)) {
		// make sure we can't interrupt a Wait()
		// and desync the music to cutscene
		_GP(play).debug_mode++;
		script_debug(1, 0);
		_GP(play).debug_mode--;
		return false;
	}

	// No service operation triggered? return active keypress and mods to caller
	out_key = agskey;
	return true;
}

bool run_service_mb_controls(int &mbut, int &mwheelz) {
	int mb = ags_mgetbutton();
	int mz = ags_check_mouse_wheel();
	if (mb == MouseNone && mz == 0)
		return false;
	lock_mouse_on_click(); // do not claim
	mbut = mb;
	mwheelz = mz;
	return true;
}

// Runs default keyboard handling
static void check_keyboard_controls() {
	// First check for service engine's combinations (mouse lock, display mode switch, and so forth)
	int kgn;
	if (!run_service_key_controls(kgn)) {
		return;
	}
	// Then, check cutscene skip
	check_skip_cutscene_keypress(kgn);
	if (_GP(play).fast_forward) {
		return;
	}
	if (_GP(play).IsIgnoringInput()) {
		return;
	}
	// Now check for in-game controls
	if (pl_run_plugin_hooks(AGSE_KEYPRESS, kgn)) {
		// plugin took the keypress
		debug_script_log("Keypress code %d taken by plugin", kgn);
		return;
	}

	// skip speech if desired by Speech.SkipStyle
	if ((is_text_overlay > 0) && (_GP(play).cant_skip_speech & SKIP_KEYPRESS)) {
		// only allow a key to remove the overlay if the icon bar isn't up
		if (IsGamePaused() == 0) {
			// check if it requires a specific keypress
			if ((_GP(play).skip_speech_specific_key > 0) &&
				(kgn != _GP(play).skip_speech_specific_key)) {
			} else
				remove_screen_overlay(OVER_TEXTMSG);
		}

		return;
	}

	if ((_GP(play).wait_counter > 0) && (_GP(play).key_skip_wait & SKIP_KEYPRESS) != 0) {
		_GP(play).wait_counter = -1;
		debug_script_log("Keypress code %d ignored - in Wait", kgn);
		return;
	}

	if (inside_script) {
		// Don't queue up another keypress if it can't be run instantly
		debug_script_log("Keypress %d ignored (game blocked)", kgn);
		return;
	}

	int keywasprocessed = 0;

	// determine if a GUI Text Box should steal the click
	// it should do if a displayable character (32-255) is
	// pressed, but exclude control characters (<32) and
	// extended keys (eg. up/down arrow; 256+)
	if ((((kgn >= 32) && (kgn <= 255) && (kgn != '[')) || (kgn == eAGSKeyCodeReturn) || (kgn == eAGSKeyCodeBackspace))
		&& !all_buttons_disabled) {
		for (int guiIndex = 0; guiIndex < _GP(game).numgui; guiIndex++) {
			auto &gui = _GP(guis)[guiIndex];

			if (!gui.IsDisplayed()) continue;

			for (int controlIndex = 0; controlIndex < gui.GetControlCount(); controlIndex++) {
				// not a text box, ignore it
				if (gui.GetControlType(controlIndex) != kGUITextBox) {
					continue;
				}

				auto *guitex = static_cast<GUITextBox *>(gui.GetControl(controlIndex));
				if (guitex == nullptr) {
					continue;
				}

				// if the text box is disabled, it cannot accept keypresses
				if (!guitex->IsEnabled()) {
					continue;
				}
				if (!guitex->IsVisible()) {
					continue;
				}

				keywasprocessed = 1;

				guitex->OnKeyPress(kgn);

				if (guitex->IsActivated) {
					guitex->IsActivated = false;
					setevent(EV_IFACECLICK, guiIndex, controlIndex, 1);
				}
			}
		}
	}

	if (!keywasprocessed) {
		kgn = AGSKeyToScriptKey(kgn);
		debug_script_log("Running on_key_press keycode %d", kgn);
		setevent(EV_TEXTSCRIPT, TS_KEYPRESS, kgn);
	}

	// RunTextScriptIParam(gameinst,"on_key_press",kgn);
}

// check_controls: checks mouse & keyboard interface
static void check_controls() {
	our_eip = 1007;

	sys_evt_process_pending();

	check_mouse_controls();
	check_keyboard_controls();
}

static void check_room_edges(int numevents_was) {
	if ((IsInterfaceEnabled()) && (IsGamePaused() == 0) &&
		(in_new_room == 0) && (new_room_was == 0)) {
		// Only allow walking off edges if not in wait mode, and
		// if not in Player Enters Screen (allow walking in from off-screen)
		int edgesActivated[4] = { 0, 0, 0, 0 };
		// Only do it if nothing else has happened (eg. mouseclick)
		if ((_G(numevents) == numevents_was) &&
			((_GP(play).ground_level_areas_disabled & GLED_INTERACTION) == 0)) {

			if (_G(_G(playerchar))->x <= _GP(_GP(thisroom)).Edges.Left)
				edgesActivated[0] = 1;
			else if (_G(_G(playerchar))->x >= _GP(_GP(thisroom)).Edges.Right)
				edgesActivated[1] = 1;
			if (_G(_G(playerchar))->y >= _GP(_GP(thisroom)).Edges.Bottom)
				edgesActivated[2] = 1;
			else if (_G(_G(playerchar))->y <= _GP(_GP(thisroom)).Edges.Top)
				edgesActivated[3] = 1;

			if ((_GP(play).entered_edge >= 0) && (_GP(play).entered_edge <= 3)) {
				// once the player is no longer outside the edge, forget the stored edge
				if (edgesActivated[_GP(play).entered_edge] == 0)
					_GP(play).entered_edge = -10;
				// if we are walking in from off-screen, don't activate edges
				else
					edgesActivated[_GP(play).entered_edge] = 0;
			}

			for (int ii = 0; ii < 4; ii++) {
				if (edgesActivated[ii])
					setevent(EV_RUNEVBLOCK, EVB_ROOM, 0, ii);
			}
		}
	}
	our_eip = 1008;

}

static void game_loop_check_controls(bool checkControls) {
	// don't let the player do anything before the screen fades in
	if ((in_new_room == 0) && (checkControls)) {
		int inRoom = displayed_room;
		int numevents_was = _G(numevents);
		check_controls();
		check_room_edges(numevents_was);
		// If an inventory interaction changed the room
		if (inRoom != displayed_room)
			check_new_room();
	}
}

static void game_loop_do_update() {
	if (_G(debug_flags) & DBG_NOUPDATE);
	else if (game_paused == 0) update_stuff();
}

static void game_loop_update_animated_buttons() {
	// update animating GUI buttons
	// this bit isn't in update_stuff because it always needs to
	// happen, even when the game is paused
	for (int aa = 0; aa < _G(numAnimButs); aa++) {
		if (UpdateAnimatingButton(aa)) {
			StopButtonAnimation(aa);
			aa--;
		}
	}
}

static void game_loop_do_render_and_check_mouse(IDriverDependantBitmap *extraBitmap, int extraX, int extraY) {
	if (!_GP(play).fast_forward) {
		int mwasatx = mousex, mwasaty = mousey;

		// Only do this if we are not skipping a cutscene
		render_graphics(extraBitmap, extraX, extraY);

		// Check Mouse Moves Over Hotspot event
		// TODO: move this out of render related function? find out why we remember mwasatx and mwasaty before render
		// TODO: do not use static variables!
		// TODO: if we support rotation then we also need to compare full transform!
		if (displayed_room < 0)
			return;
		auto view = _GP(play).GetRoomViewportAt(mousex, mousey);
		auto cam = view ? view->GetCamera() : nullptr;
		if (cam) {
			// NOTE: all cameras are in same room right now, so their positions are in same coordinate system;
			// therefore we may use this as an indication that mouse is over different camera too.
			static int offsetxWas = -1000, offsetyWas = -1000;
			int offsetx = cam->GetRect().Left;
			int offsety = cam->GetRect().Top;

			if (((mwasatx != mousex) || (mwasaty != mousey) ||
				(offsetxWas != offsetx) || (offsetyWas != offsety))) {
				// mouse moves over hotspot
				if (__GetLocationType(game_to_data_coord(mousex), game_to_data_coord(mousey), 1) == LOCTYPE_HOTSPOT) {
					int onhs = getloctype_index;

					setevent(EV_RUNEVBLOCK, EVB_HOTSPOT, onhs, 6);
				}
			}

			offsetxWas = offsetx;
			offsetyWas = offsety;
		} // camera found under mouse
	}
}

static void game_loop_update_events() {
	new_room_was = in_new_room;
	if (in_new_room > 0)
		setevent(EV_FADEIN, 0, 0, 0);
	in_new_room = 0;
	update_events();
	if ((new_room_was > 0) && (in_new_room == 0)) {
		// if in a new room, and the room wasn't just changed again in update_events,
		// then queue the Enters Screen scripts
		// run these next time round, when it's faded in
		if (new_room_was == 2)  // first time enters screen
			setevent(EV_RUNEVBLOCK, EVB_ROOM, 0, 4);
		if (new_room_was != 3)   // enters screen after fadein
			setevent(EV_RUNEVBLOCK, EVB_ROOM, 0, 7);
	}
}

static void game_loop_update_background_animation() {
	if (_GP(play).bg_anim_delay > 0) _GP(play).bg_anim_delay--;
	else if (_GP(play).bg_frame_locked);
	else {
		_GP(play).bg_anim_delay = _GP(play).anim_background_speed;
		_GP(play).bg_frame++;
		if ((size_t)_GP(play).bg_frame >= _GP(_GP(thisroom)).BgFrameCount)
			_GP(play).bg_frame = 0;
		if (_GP(_GP(thisroom)).BgFrameCount >= 2) {
			// get the new frame's palette
			on_background_frame_change();
		}
	}
}

static void game_loop_update_loop_counter() {
	loopcounter++;

	if (_GP(play).wait_counter > 0) _GP(play).wait_counter--;
	if (_GP(play).shakesc_length > 0) _GP(play).shakesc_length--;

	if (loopcounter % 5 == 0) {
		update_ambient_sound_vol();
		update_directional_sound_vol();
	}
}

static void game_loop_update_fps() {
	auto t2 = AGS_Clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
	auto frames = loopcounter - lastcounter;

	if (duration >= std::chrono::milliseconds(1000) && frames > 0) {
		fps = 1000.0f * frames / duration.count();
		t1 = t2;
		lastcounter = loopcounter;
	}
}

float get_current_fps() {
	// if we have maxed out framerate then return the frame rate we're seeing instead
	// fps must be greater that 0 or some timings will take forever.
	if (isTimerFpsMaxed() && fps > 0.0f) {
		return fps;
	}
	return frames_per_second;
}

void set_loop_counter(unsigned int new_counter) {
	loopcounter = new_counter;
	t1 = AGS_Clock::now();
	lastcounter = loopcounter;
	fps = std::numeric_limits<float>::quiet_NaN();
}

void UpdateGameOnce(bool checkControls, IDriverDependantBitmap *extraBitmap, int extraX, int extraY) {

	int res;

	sys_evt_process_pending();

	numEventsAtStartOfFunction = _G(numevents);

	if (want_exit) {
		ProperExit();
	}

	ccNotifyScriptStillAlive();
	our_eip = 1;

	game_loop_check_problems_at_start();

	// if we're not fading in, don't count the fadeouts
	if ((_GP(play).no_hicolor_fadein) && (_GP(game).options[OPT_FADETYPE] == FADE_NORMAL))
		_GP(play).screen_is_faded_out = 0;

	our_eip = 1014;

	update_gui_disabled_status();

	our_eip = 1004;

	game_loop_check_new_room();

	our_eip = 1005;

	res = game_loop_check_ground_level_interactions();
	if (res != RETURN_CONTINUE) {
		return;
	}

	mouse_on_iface = -1;

	check_debug_keys();

	game_loop_check_controls(checkControls);

	our_eip = 2;

	game_loop_do_update();

	game_loop_update_animated_buttons();

	game_loop_do_late_update();

	update_audio_system_on_game_loop();

	game_loop_do_render_and_check_mouse(extraBitmap, extraX, extraY);

	our_eip = 6;

	game_loop_update_events();

	our_eip = 7;

	update_polled_stuff_if_runtime();

	game_loop_update_background_animation();

	game_loop_update_loop_counter();

	// Immediately start the next frame if we are skipping a cutscene
	if (_GP(play).fast_forward)
		return;

	our_eip = 72;

	game_loop_update_fps();

	update_polled_stuff_if_runtime();

	WaitForNextFrame();
}

static void UpdateMouseOverLocation() {
	// Call GetLocationName - it will internally force a GUI refresh
	// if the result it returns has changed from last time
	char tempo[STD_BUFFER_SIZE];
	GetLocationName(game_to_data_coord(mousex), game_to_data_coord(mousey), tempo);

	if ((_GP(play).get_loc_name_save_cursor >= 0) &&
		(_GP(play).get_loc_name_save_cursor != _GP(play).get_loc_name_last_time) &&
		(mouse_on_iface < 0) && (ifacepopped < 0)) {
		// we have saved the cursor, but the mouse location has changed
		// and it's time to restore it
		_GP(play).get_loc_name_save_cursor = -1;
		set_cursor_mode(_GP(play).restore_cursor_mode_to);

		if (cur_mode == _GP(play).restore_cursor_mode_to) {
			// make sure it changed -- the new mode might have been disabled
			// in which case don't change the image
			set_mouse_cursor(_GP(play).restore_cursor_image_to);
		}
		debug_script_log("Restore mouse to mode %d cursor %d", _GP(play).restore_cursor_mode_to, _GP(play).restore_cursor_image_to);
	}
}

// Checks if user interface should remain disabled for now
static int ShouldStayInWaitMode() {
	if (restrict_until == 0)
		quit("end_wait_loop called but game not in loop_until state");
	int retval = restrict_until;

	if (restrict_until == UNTIL_MOVEEND) {
		short *wkptr = (short *)user_disabled_data;
		if (wkptr[0] < 1) retval = 0;
	} else if (restrict_until == UNTIL_CHARIS0) {
		char *chptr = (char *)user_disabled_data;
		if (chptr[0] == 0) retval = 0;
	} else if (restrict_until == UNTIL_NEGATIVE) {
		short *wkptr = (short *)user_disabled_data;
		if (wkptr[0] < 0) retval = 0;
	} else if (restrict_until == UNTIL_INTISNEG) {
		int *wkptr = (int *)user_disabled_data;
		if (wkptr[0] < 0) retval = 0;
	} else if (restrict_until == UNTIL_NOOVERLAY) {
		if (is_text_overlay < 1) retval = 0;
	} else if (restrict_until == UNTIL_INTIS0) {
		int *wkptr = (int *)user_disabled_data;
		if (wkptr[0] == 0) retval = 0;
	} else if (restrict_until == UNTIL_SHORTIS0) {
		short *wkptr = (short *)user_disabled_data;
		if (wkptr[0] == 0) retval = 0;
	} else quit("loop_until: unknown until event");

	return retval;
}

static int UpdateWaitMode() {
	if (restrict_until == 0) {
		return RETURN_CONTINUE;
	}

	restrict_until = ShouldStayInWaitMode();
	our_eip = 77;

	if (restrict_until != 0) {
		return RETURN_CONTINUE;
	}

	auto was_disabled_for = user_disabled_for;

	set_default_cursor();
	if (gui_disabled_style != GUIDIS_UNCHANGED) { // If GUI looks change when disabled, then update them all
		GUI::MarkAllGUIForUpdate();
	}
	_GP(play).disabled_user_interface--;
	user_disabled_for = 0;

	switch (was_disabled_for) {
		// case FOR_ANIMATION:
		//     run_animation((FullAnimation*)user_disabled_data2,user_disabled_data3);
		//     break;
	case FOR_EXITLOOP:
		return -1;
	case FOR_SCRIPT:
		quit("err: for_script obsolete (v2.1 and earlier only)");
		break;
	default:
		quit("Unknown user_disabled_for in end restrict_until");
	}

	// we shouldn't get here.
	return RETURN_CONTINUE;
}

// Run single game iteration; calls UpdateGameOnce() internally
static int GameTick() {
	if (displayed_room < 0)
		quit("!A blocking function was called before the first room has been loaded");

	UpdateGameOnce(true);
	UpdateMouseOverLocation();

	our_eip = 76;

	int res = UpdateWaitMode();
	if (res == RETURN_CONTINUE) {
		return 0;
	} // continue looping 
	return res;
}

static void SetupLoopParameters(int untilwhat, const void *udata) {
	_GP(play).disabled_user_interface++;
	if (gui_disabled_style != GUIDIS_UNCHANGED) { // If GUI looks change when disabled, then update them all
		GUI::MarkAllGUIForUpdate();
	}
	// Only change the mouse cursor if it hasn't been specifically changed first
	// (or if it's speech, always change it)
	if (((cur_cursor == cur_mode) || (untilwhat == UNTIL_NOOVERLAY)) &&
		(cur_mode != CURS_WAIT))
		set_mouse_cursor(CURS_WAIT);

	restrict_until = untilwhat;
	user_disabled_data = udata;
	user_disabled_for = FOR_EXITLOOP;
}

// This function is called from lot of various functions
// in the game core, character, room object etc
static void GameLoopUntilEvent(int untilwhat, const void *daaa) {
	// blocking cutscene - end skipping
	EndSkippingUntilCharStops();

	// this function can get called in a nested context, so
	// remember the state of these vars in case a higher level
	// call needs them
	auto cached_restrict_until = restrict_until;
	auto cached_user_disabled_data = user_disabled_data;
	auto cached_user_disabled_for = user_disabled_for;

	SetupLoopParameters(untilwhat, daaa);
	while (GameTick() == 0);

	our_eip = 78;

	restrict_until = cached_restrict_until;
	user_disabled_data = cached_user_disabled_data;
	user_disabled_for = cached_user_disabled_for;
}

void GameLoopUntilValueIsZero(const char *value) {
	GameLoopUntilEvent(UNTIL_CHARIS0, value);
}

void GameLoopUntilValueIsZero(const short *value) {
	GameLoopUntilEvent(UNTIL_SHORTIS0, value);
}

void GameLoopUntilValueIsZero(const int *value) {
	GameLoopUntilEvent(UNTIL_INTIS0, value);
}

void GameLoopUntilValueIsZeroOrLess(const short *value) {
	GameLoopUntilEvent(UNTIL_MOVEEND, value);
}

void GameLoopUntilValueIsNegative(const short *value) {
	GameLoopUntilEvent(UNTIL_NEGATIVE, value);
}

void GameLoopUntilValueIsNegative(const int *value) {
	GameLoopUntilEvent(UNTIL_INTISNEG, value);
}

void GameLoopUntilNotMoving(const short *move) {
	GameLoopUntilEvent(UNTIL_MOVEEND, move);
}

void GameLoopUntilNoOverlay() {
	GameLoopUntilEvent(UNTIL_NOOVERLAY, 0);
}


extern unsigned int load_new_game;
void RunGameUntilAborted() {
	// skip ticks to account for time spent starting _GP(game).
	skipMissedTicks();

	while (!abort_engine) {
		GameTick();

		if (load_new_game) {
			RunAGSGame(nullptr, load_new_game, 0);
			load_new_game = 0;
		}
	}
}

void update_polled_stuff_if_runtime() {
	SDL_PumpEvents();

	if (want_exit) {
		want_exit = 0;
		quit("||exit!");
	}

	if (_G(editor_debugging_initialized))
		check_for_messages_from_editor();
}

} // namespace AGS3
