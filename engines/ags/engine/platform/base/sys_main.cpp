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

//include <SDL.h>
//include <SDL_syswm.h>
#include "ags/engine/platform/base/sys_main.h"
#include "ags/shared/util/geometry.h"
#include "ags/shared/util/string.h"

namespace AGS3 {

namespace ags = AGS::Shared;

// ----------------------------------------------------------------------------
// INIT / SHUTDOWN
// ----------------------------------------------------------------------------

int sys_main_init(/*config*/) {
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE); // TODO: backend log verbosity from config

	// TODO: setup these subsystems in config rather than keep hardcoded?
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return -1;
	}
	return 0;
}

void sys_main_shutdown() {
	sys_window_destroy();
	SDL_Quit();
}

void sys_set_background_mode(bool on) {
	// TODO: consider if we want any implementation here, and what...
}


// ----------------------------------------------------------------------------
// DISPLAY UTILS
// ----------------------------------------------------------------------------
const int DEFAULT_DISPLAY_INDEX = 0; // TODO: is this always right?

int sys_get_desktop_resolution(int &width, int &height) {
	SDL_Rect r;
	if (SDL_GetDisplayBounds(DEFAULT_DISPLAY_INDEX, &r) != 0) {
		SDL_Log("SDL_GetDisplayBounds failed: %s", SDL_GetError());
		return -1;
	}
	width = r.w;
	height = r.h;
	return 0;
}

void sys_get_desktop_modes(std::vector<AGS::Engine::DisplayMode> &dms) {
	SDL_DisplayMode mode;
	const int display_id = DEFAULT_DISPLAY_INDEX;
	const int count = SDL_GetNumDisplayModes(display_id);
	dms.clear();
	for (int i = 0; i < count; ++i) {
		if (SDL_GetDisplayMode(display_id, i, &mode) != 0) {
			SDL_Log("SDL_GetDisplayMode failed: %s", SDL_GetError());
			continue;
		}
		AGS::Engine::DisplayMode dm;
		dm.Width = mode.w;
		dm.Height = mode.h;
		dm.ColorDepth = SDL_BITSPERPIXEL(mode.format);
		dm.RefreshRate = mode.refresh_rate;
		dms.push_back(dm);
	}
}


// ----------------------------------------------------------------------------
// WINDOW UTILS
// ----------------------------------------------------------------------------
// TODO: support multiple windows? in case we need some for diag purposes etc
static SDL_Window *window = nullptr;

SDL_Window *sys_window_create(const char *window_title, int w, int h, bool windowed, int ex_flags) {
	if (window) {
		sys_window_destroy();
	}
	// TODO: support display index selection (?)
	// TODO: support separate fullscreen and desktop (borderless fullscreen window) modes
	Uint32 flags = SDL_WINDOW_RESIZABLE;
	if (!windowed) {
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP/*SDL_WINDOW_FULLSCREEN*/;
	}
	flags |= ex_flags;
	window = SDL_CreateWindow(
		window_title,
		SDL_WINDOWPOS_CENTERED_DISPLAY(DEFAULT_DISPLAY_INDEX),
		SDL_WINDOWPOS_CENTERED_DISPLAY(DEFAULT_DISPLAY_INDEX),
		w,
		h,
		flags
	);
	return window;
}

SDL_Window *sys_get_window() {
	return window;
}

void sys_window_set_style(bool windowed) {
	if (!window) return;
	// TODO: support separate fullscreen and desktop (borderless fullscreen window) modes
	// TODO: support resizable window later, might need callback for engine and/or gfx renderer
	SDL_SetWindowFullscreen(window, windowed ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
}

void sys_window_show_cursor(bool on) {
	SDL_ShowCursor(on ? SDL_ENABLE : SDL_DISABLE);
}

bool sys_window_lock_mouse(bool on) {
	if (!window) return false;
	SDL_SetWindowGrab(window, static_cast<SDL_bool>(on));
	return on; // TODO: test if successful?
}

void sys_window_set_mouse(int x, int y) {
	if (!window) return;
	SDL_WarpMouseInWindow(window, x, y);
}

void sys_window_destroy() {
	if (window) {
		SDL_DestroyWindow(window);
		window = nullptr;
	}
}

void sys_window_set_title(const char *title) {
	if (window) {
		SDL_SetWindowTitle(window, title);
	}
}

void sys_window_set_icon() {
	if (window) {
		// TODO: actually support getting icon from resources and converting into SDL_Surface.
		//  - on Linux we had platform/linux/icon.xpm
		//  - on Windows we had standard embedded resource under ID = 101
		SDL_SetWindowIcon(window, nullptr);
	}
}

bool sys_window_set_size(int w, int h, bool center) {
	if (window) {
		SDL_SetWindowSize(window, w, h);
		if (center)
			SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		int new_w, new_h;
		SDL_GetWindowSize(window, &new_w, &new_h);
		return new_w == w && new_h == h;
	}
	return false;
}

#if AGS_PLATFORM_OS_WINDOWS
void *sys_win_get_window() {
	if (!window) return nullptr;
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWindowWMInfo(window, &wmInfo);
	HWND hwnd = wmInfo.info.win.window;
	return hwnd;
}
#endif

} // namespace AGS3
