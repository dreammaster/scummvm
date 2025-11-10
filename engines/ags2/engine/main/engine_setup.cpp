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

#include "ags2/shared/core/platform.h"
#include "ags2/shared/ac/common.h"
#include "ags2/engine/ac/display.h"
#include "ags2/engine/ac/draw.h"
#include "ags2/engine/ac/game_setup.h"
#include "ags2/engine/ac/game_state.h"
#include "ags2/engine/ac/global_game.h"
#include "ags2/engine/ac/mouse.h"
#include "ags2/engine/ac/runtime_defines.h"
#include "ags2/engine/ac/walk_behind.h"
#include "ags2/engine/ac/dynobj/script_system.h"
#include "ags2/shared/debugging/out.h"
#include "ags2/engine/device/mouse_w32.h"
#include "ags2/shared/font/fonts.h"
#include "ags2/engine/gfx/graphics_driver.h"
#include "ags2/shared/gui/gui_main.h"
#include "ags2/shared/gui/gui_inv.h"
#include "ags2/engine/main/game_run.h"
#include "ags2/engine/main/graphics_mode.h"
#include "ags2/engine/main/engine_setup.h"
#include "ags2/engine/media/video/video.h"
#include "ags2/engine/platform/base/ags_platform_driver.h"
#include "ags2/engine/platform/base/sys_main.h"
#include "ags2/globals.h"

namespace AGS2 {

using namespace AGS::Shared;
using namespace AGS::Engine;


void engine_adjust_for_rotation_settings() {
#if 0
	switch (_GP(usetup).rotation) {
	case ScreenRotation::kScreenRotation_Portrait:
		SDL_SetHint(SDL_HINT_ORIENTATIONS, "Portrait PortraitUpsideDown");
		break;
	case ScreenRotation::kScreenRotation_Landscape:
		SDL_SetHint(SDL_HINT_ORIENTATIONS, "LandscapeLeft LandscapeRight");
		break;
	case kScreenRotation_Unlocked:
		// let the user rotate as wished. No adjustment needed.
	default:
		break;
	}
#endif
}

// Setup gfx driver callbacks and options
void engine_post_gfxmode_driver_setup() {
	_G(gfxDriver)->SetCallbackForPolling(update_polled_stuff);
	_G(gfxDriver)->SetCallbackToDrawScreen(draw_game_screen_callback, construct_engine_overlay);
	_G(gfxDriver)->SetCallbackOnSpriteEvt(GfxDriverSpriteEvtCallback);
}

// Reset gfx driver callbacks
void engine_pre_gfxmode_driver_cleanup() {
	_G(gfxDriver)->SetCallbackForPolling(nullptr);
	_G(gfxDriver)->SetCallbackToDrawScreen(nullptr, nullptr);
	_G(gfxDriver)->SetCallbackOnSpriteEvt(nullptr);
	_G(gfxDriver)->SetMemoryBackBuffer(nullptr);
}

// Setup color conversion parameters
void engine_setup_color_conversions(int coldepth) {
	// default shifts for how we store the sprite data
	_G(_rgb_r_shift_32) = 16;
	_G(_rgb_g_shift_32) = 8;
	_G(_rgb_b_shift_32) = 0;
	_G(_rgb_r_shift_16) = 11;
	_G(_rgb_g_shift_16) = 5;
	_G(_rgb_b_shift_16) = 0;
	_G(_rgb_r_shift_15) = 10;
	_G(_rgb_g_shift_15) = 5;
	_G(_rgb_b_shift_15) = 0;

	// TODO: investigate if this is still necessary, and under which circumstances?
	// the color conversion should likely be done when preparing textures or
	// rendering to final output instead, not in the main engine code.
	if (coldepth < 16) {
		// ensure that any 32-bit graphics displayed are converted
		// properly to the current depth
#if AGS_PLATFORM_OS_WINDOWS
		_G(_rgb_r_shift_32) = 16;
		_G(_rgb_g_shift_32) = 8;
		_G(_rgb_b_shift_32) = 0;
#else
		_G(_rgb_r_shift_32) = 0;
		_G(_rgb_g_shift_32) = 8;
		_G(_rgb_b_shift_32) = 16;

		_G(_rgb_b_shift_15) = 0;
		_G(_rgb_g_shift_15) = 5;
		_G(_rgb_r_shift_15) = 10;
#endif
	}

	set_color_conversion(COLORCONV_MOST | COLORCONV_EXPAND_256);
}

// Setup drawing modes and color conversions;
// they depend primarily on gfx driver capabilities and new color depth
void engine_post_gfxmode_draw_setup(const DisplayMode &dm) {
	engine_setup_color_conversions(dm.ColorDepth);
	init_draw_method();
}

// Cleanup auxiliary drawing objects
void engine_pre_gfxmode_draw_cleanup() {
	dispose_draw_method();
}

// Setup mouse control mode and graphic area
void engine_post_gfxmode_mouse_setup(const Size &init_desktop) {
	// Assign mouse control parameters.
	//
	// NOTE that we setup speed and other related properties regardless of
	// whether mouse control was requested because it may be enabled later.
	_GP(mouse).SetSpeedUnit(1.f);
	if (_GP(usetup).mouse_speed_def == kMouseSpeed_CurrentDisplay) {
		Size cur_desktop;
		if (sys_get_desktop_resolution(cur_desktop.Width, cur_desktop.Height) == 0)
			_GP(mouse).SetSpeedUnit(MAX((float)cur_desktop.Width / (float)init_desktop.Width,
			                                  (float)cur_desktop.Height / (float)init_desktop.Height));
	}

	Mouse_EnableControl(_GP(usetup).mouse_ctrl_enabled);
	Debug::Printf(kDbgMsg_Info, "Mouse speed control: %s, unit: %f, user value: %f",
	              _GP(usetup).mouse_ctrl_enabled ? "enabled" : "disabled", _GP(mouse).GetSpeedUnit(), _GP(mouse).GetSpeed());

	on_coordinates_scaling_changed();

	// If auto lock option is set, lock mouse to the game window
	if (_GP(usetup).mouse_auto_lock && _GP(scsystem).windowed != 0)
		_GP(mouse).TryLockToWindow();
}

// Reset mouse controls before changing gfx mode
void engine_pre_gfxmode_mouse_cleanup() {
	// Always disable mouse control and unlock mouse when releasing down gfx mode
	_GP(mouse).SetMovementControl(false);
	_GP(mouse).UnlockFromWindow();
}

// Fill in _GP(scsystem) struct with display mode parameters
void engine_setup_scsystem_screen(const DisplayMode &dm) {
	_GP(scsystem).windowed = dm.IsWindowed();
	_GP(scsystem).vsync = dm.Vsync;
}

void engine_post_gfxmode_setup(const Size &init_desktop, const DisplayMode &old_dm) {
	DisplayMode dm = _G(gfxDriver)->GetDisplayMode();
	// If color depth has changed (or graphics mode was inited for the
	// very first time), we also need to recreate bitmaps
	bool has_driver_changed = old_dm.ColorDepth != dm.ColorDepth;

	engine_setup_scsystem_screen(dm);
	engine_post_gfxmode_driver_setup();
	if (has_driver_changed) {
		engine_post_gfxmode_draw_setup(dm);
	}
	engine_post_gfxmode_mouse_setup(init_desktop);

	// reset multitasking (may be overridden by the current display mode)
	SetMultitasking(_GP(usetup).multitasking);

	invalidate_screen();
}

void engine_pre_gfxmode_release() {
	engine_pre_gfxmode_mouse_cleanup();
	engine_pre_gfxmode_driver_cleanup();
}

void engine_pre_gfxsystem_shutdown() {
	engine_pre_gfxmode_release();
	engine_pre_gfxmode_draw_cleanup();
}

void on_coordinates_scaling_changed() {
	// Reset mouse graphic area and bounds
	_GP(mouse).UpdateGraphicArea();
	// If mouse bounds do not have valid values yet, then limit cursor to viewport
	if (_GP(play).mboundx1 == 0 && _GP(play).mboundy1 == 0 && _GP(play).mboundx2 == 0 && _GP(play).mboundy2 == 0)
		_GP(mouse).SetMoveLimit(_GP(play).GetMainViewport());
	else
		_GP(mouse).SetMoveLimit(Rect(_GP(play).mboundx1, _GP(play).mboundy1, _GP(play).mboundx2, _GP(play).mboundy2));
}

} // namespace AGS2
