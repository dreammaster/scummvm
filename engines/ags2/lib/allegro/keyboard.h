
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

#ifndef AGS2_LIB_ALLEGRO_KEYBOARD_H
#define AGS2_LIB_ALLEGRO_KEYBOARD_H

#include "common/keyboard.h"

namespace AGS2 {

enum {
	KB_SHIFT_FLAG         = Common::KBD_SHIFT,
	KB_CTRL_FLAG          = Common::KBD_CTRL,
	KB_ALT_FLAG           = Common::KBD_ALT
};

enum {
	KEY_A                 = Common::KEYCODE_a,
	KEY_B                 = Common::KEYCODE_b,
	KEY_C                 = Common::KEYCODE_c,
	KEY_D                 = Common::KEYCODE_d,
	KEY_E                 = Common::KEYCODE_e,
	KEY_F                 = Common::KEYCODE_f,
	KEY_G                 = Common::KEYCODE_g,
	KEY_H                 = Common::KEYCODE_h,
	KEY_I                 = Common::KEYCODE_i,
	KEY_J                 = Common::KEYCODE_j,
	KEY_K                 = Common::KEYCODE_k,
	KEY_L                 = Common::KEYCODE_l,
	KEY_M                 = Common::KEYCODE_m,
	KEY_N                 = Common::KEYCODE_n,
	KEY_O                 = Common::KEYCODE_o,
	KEY_P                 = Common::KEYCODE_p,
	KEY_Q                 = Common::KEYCODE_q,
	KEY_R                 = Common::KEYCODE_r,
	KEY_S                 = Common::KEYCODE_s,
	KEY_T                 = Common::KEYCODE_t,
	KEY_U                 = Common::KEYCODE_u,
	KEY_V                 = Common::KEYCODE_v,
	KEY_W                 = Common::KEYCODE_w,
	KEY_X                 = Common::KEYCODE_x,
	KEY_Y                 = Common::KEYCODE_y,
	KEY_Z                 = Common::KEYCODE_z,
	KEY_0                 = Common::KEYCODE_0,
	KEY_1                 = Common::KEYCODE_1,
	KEY_2                 = Common::KEYCODE_2,
	KEY_3                 = Common::KEYCODE_3,
	KEY_4                 = Common::KEYCODE_4,
	KEY_5                 = Common::KEYCODE_5,
	KEY_6                 = Common::KEYCODE_6,
	KEY_7                 = Common::KEYCODE_7,
	KEY_8                 = Common::KEYCODE_8,
	KEY_9                 = Common::KEYCODE_9,
	KEY_0_PAD             = Common::KEYCODE_KP0,
	KEY_1_PAD             = Common::KEYCODE_KP1,
	KEY_2_PAD             = Common::KEYCODE_KP2,
	KEY_3_PAD             = Common::KEYCODE_KP3,
	KEY_4_PAD             = Common::KEYCODE_KP4,
	KEY_5_PAD             = Common::KEYCODE_KP5,
	KEY_6_PAD             = Common::KEYCODE_KP6,
	KEY_7_PAD             = Common::KEYCODE_KP7,
	KEY_8_PAD             = Common::KEYCODE_KP8,
	KEY_9_PAD             = Common::KEYCODE_KP9,
	KEY_F1                = Common::KEYCODE_F1,
	KEY_F2                = Common::KEYCODE_F2,
	KEY_F3                = Common::KEYCODE_F3,
	KEY_F4                = Common::KEYCODE_F4,
	KEY_F5                = Common::KEYCODE_F5,
	KEY_F6                = Common::KEYCODE_F6,
	KEY_F7                = Common::KEYCODE_F7,
	KEY_F8                = Common::KEYCODE_F8,
	KEY_F9                = Common::KEYCODE_F9,
	KEY_F10               = Common::KEYCODE_F10,
	KEY_F11               = Common::KEYCODE_F11,
	KEY_F12               = Common::KEYCODE_F12,
	KEY_ESC               = Common::KEYCODE_ESCAPE,
	KEY_TILDE             = Common::KEYCODE_TILDE,
	KEY_MINUS             = Common::KEYCODE_MINUS,
	KEY_EQUALS            = Common::KEYCODE_EQUALS,
	KEY_BACKSPACE         = Common::KEYCODE_BACKSPACE,
	KEY_TAB               = Common::KEYCODE_TAB,
	KEY_OPENBRACE         = Common::KEYCODE_LEFTBRACKET,
	KEY_CLOSEBRACE        = Common::KEYCODE_RIGHTBRACKET,
	KEY_ENTER             = Common::KEYCODE_RETURN,
	KEY_COLON             = Common::KEYCODE_COLON,
	KEY_QUOTE             = Common::KEYCODE_QUOTE,
	KEY_BACKSLASH         = Common::KEYCODE_BACKSLASH,
	KEY_COMMA             = Common::KEYCODE_COMMA,
	KEY_STOP              = Common::KEYCODE_PERIOD,
	KEY_SLASH             = Common::KEYCODE_SLASH,
	KEY_SPACE             = Common::KEYCODE_SPACE,
	KEY_INSERT            = Common::KEYCODE_INSERT,
	KEY_DEL               = Common::KEYCODE_DELETE,
	KEY_HOME              = Common::KEYCODE_HOME,
	KEY_END               = Common::KEYCODE_END,
	KEY_PGUP              = Common::KEYCODE_PAGEUP,
	KEY_PGDN              = Common::KEYCODE_PAGEDOWN,
	KEY_LEFT              = Common::KEYCODE_LEFT,
	KEY_RIGHT             = Common::KEYCODE_RIGHT,
	KEY_UP                = Common::KEYCODE_UP,
	KEY_DOWN              = Common::KEYCODE_DOWN,
	KEY_SLASH_PAD         = Common::KEYCODE_KP_DIVIDE,
	KEY_ASTERISK          = Common::KEYCODE_ASTERISK,
	KEY_MINUS_PAD         = Common::KEYCODE_KP_MINUS,
	KEY_PLUS_PAD          = Common::KEYCODE_KP_PLUS,
	KEY_DEL_PAD           = Common::KEYCODE_KP_PERIOD,
	KEY_ENTER_PAD         = Common::KEYCODE_KP_ENTER,
	KEY_PRTSCR            = Common::KEYCODE_PRINT,
	KEY_PAUSE             = Common::KEYCODE_PAUSE,
	KEY_AT                = Common::KEYCODE_AT,
	KEY_BACKQUOTE         = Common::KEYCODE_BACKQUOTE,
	KEY_SEMICOLON         = Common::KEYCODE_SEMICOLON
};

enum {
	KEY_LSHIFT            = Common::KEYCODE_LSHIFT,
	KEY_RSHIFT            = Common::KEYCODE_RSHIFT,
	KEY_LCONTROL          = Common::KEYCODE_LCTRL,
	KEY_RCONTROL          = Common::KEYCODE_RCTRL,
	KEY_ALT               = Common::KEYCODE_LALT,
	KEY_MENU              = Common::KEYCODE_MENU,
	KEY_SCRLOCK           = Common::KEYCODE_SCROLLOCK,
	KEY_NUMLOCK           = Common::KEYCODE_NUMLOCK,
	KEY_CAPSLOCK          = Common::KEYCODE_CAPSLOCK
};

constexpr int KEY_MAX = Common::KEYCODE_LAST;

inline void install_keyboard() {
	// No implementation
}

inline int keyboard_needs_poll() {
	return 1;
}

int poll_keyboard() {
	error("TODO: poll_keyboard");
}

inline void set_leds(int leds) {
	// No implementation
}


} // namespace AGS2

#endif
