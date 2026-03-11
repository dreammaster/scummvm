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

#ifndef MADS_CORE_KEYS_H
#define MADS_CORE_KEYS_H

#include "mads/madsv2/core/general.h"

namespace MADS {
namespace MADSV2 {

/* keystroke defines */

#define key_status_1            (byte *) 0x00400017 /* key status 1 */
#define key_status_2            (byte *) 0x00400018 /* key status 2 */

#define KS1_INSERT              0x80    /* Insert state      */
#define KS1_CAPSLOCK            0x40    /* Caps Lock state   */
#define KS1_NUMLOCK             0x20    /* Num Lock state    */
#define KS1_SCROLLLOCK          0x10    /* Scroll Lock state */
#define KS1_ALT                 0x08    /* Alt key state     */
#define KS1_CTRL                0x04    /* Ctrl key state    */
#define KS1_LEFTSHIFT           0x02    /* Left shift state  */
#define KS1_RIGHTSHIFT          0x01    /* Right shift state */

#define left_key                0x14b
#define right_key               0x14d
#define up_key                  0x148
#define down_key                0x150

#define ins_key                 0x152
#define del_key                 0x153
#define delete_key              0x153

#define home_key                0x147
#define end_key                 0x14f

#define pgup_key                0x149
#define pgdn_key                0x151

#define enter_key               0x0d
#define esc_key                 0x1b
#define bksp_key                0x08
#define ctrl_del_key            0x193
#define tab_key                 0x09
#define backtab_key             0x10f

#define space_key               0x20

#define A_key                   0x41
#define B_key                   0x42
#define C_key                   0x43
#define D_key                   0x44
#define E_key                   0x45
#define F_key                   0x46
#define G_key                   0x47
#define H_key                   0x48
#define I_key                   0x49
#define J_key                   0x4a
#define K_key                   0x4b
#define L_key                   0x4c
#define M_key                   0x4d
#define N_key                   0x4e
#define O_key                   0x4f
#define P_key                   0x50
#define Q_key                   0x51
#define R_key                   0x52
#define S_key                   0x53
#define T_key                   0x54
#define U_key                   0x55
#define V_key                   0x56
#define W_key                   0x57
#define X_key                   0x58
#define Y_key                   0x59
#define Z_key                   0x5a

#define a_key                   0x61
#define b_key                   0x62
#define g_key                   0x67
#define i_key                   0x69
#define j_key                   0x6a
#define m_key                   0x6d
#define z_key                   0x7a

#define alt_a_key               0x11e
#define alt_b_key               0x130
#define alt_c_key               0x12e
#define alt_d_key               0x120
#define alt_e_key               0x112
#define alt_f_key               0x121
#define alt_g_key               0x122
#define alt_h_key               0x123
#define alt_i_key               0x117
#define alt_j_key               0x124
#define alt_k_key               0x125
#define alt_l_key               0x126
#define alt_m_key               0x132
#define alt_n_key               0x131
#define alt_o_key               0x118
#define alt_p_key               0x119
#define alt_q_key               0x110
#define alt_r_key               0x113
#define alt_s_key               0x11f
#define alt_t_key               0x114
#define alt_u_key               0x116
#define alt_v_key               0x12f
#define alt_w_key               0x111
#define alt_x_key               0x12d
#define alt_y_key               0x115
#define alt_z_key               0x12c

#define f1_key                  0x13b
#define f2_key                  0x13c
#define f3_key                  0x13d
#define f4_key                  0x13e
#define f5_key                  0x13f
#define f6_key                  0x140
#define f7_key                  0x141
#define f8_key                  0x142
#define f9_key                  0x143
#define f10_key                 0x144

#define shift_f1_key            0x154
#define shift_f2_key            0x155
#define shift_f3_key            0x156
#define shift_f4_key            0x157
#define shift_f5_key            0x158
#define shift_f6_key            0x159
#define shift_f7_key            0x15a
#define shift_f8_key            0x15b
#define shift_f9_key            0x15c
#define shift_f10_key           0x15d

#define ctrl_f1_key             0x15e
#define ctrl_f2_key             0x15f
#define ctrl_f3_key             0x160
#define ctrl_f4_key             0x161
#define ctrl_f5_key             0x162
#define ctrl_f6_key             0x163
#define ctrl_f7_key             0x164
#define ctrl_f8_key             0x165
#define ctrl_f9_key             0x166
#define ctrl_f10_key            0x167

#define alt_f1_key              0x168
#define alt_f2_key              0x169
#define alt_f3_key              0x16a
#define alt_f4_key              0x16b
#define alt_f5_key              0x16c
#define alt_f6_key              0x16d
#define alt_f7_key              0x16e
#define alt_f8_key              0x16f
#define alt_f9_key              0x170
#define alt_f10_key             0x171

#define ctrl_a_key              0x01
#define ctrl_b_key              0x02
#define ctrl_c_key              0x03
#define ctrl_d_key              0x04
#define ctrl_e_key              0x05
#define ctrl_f_key              0x06
#define ctrl_g_key              0x07
#define ctrl_h_key              0x08
#define ctrl_i_key              0x09
#define ctrl_j_key              0x0a
#define ctrl_k_key              0x0b
#define ctrl_l_key              0x0c
#define ctrl_m_key              0x0d
#define ctrl_n_key              0x0e
#define ctrl_o_key              0x0f
#define ctrl_p_key              0x10
#define ctrl_q_key              0x11
#define ctrl_r_key              0x12
#define ctrl_s_key              0x13
#define ctrl_t_key              0x14
#define ctrl_u_key              0x15
#define ctrl_v_key              0x16
#define ctrl_w_key              0x17
#define ctrl_x_key              0x18
#define ctrl_y_key              0x19
#define ctrl_z_key              0x1a

#define KEYS_MAX_BUF_CHARS        32

struct KeyBuffer {
	int buf[KEYS_MAX_BUF_CHARS];
	int len;
};

typedef struct KeyBuffer *KeyPtr;

extern word keys_special_button;


extern int keys_any();
extern int keys_get();

/**
 * Flushes the typeahead buffer
 */
extern void keys_flush();

/**
 * Flushes the contents of the specified key buffer
 */
extern void keys_flush_buffer(KeyPtr key_buf);

/**
 * Adds any waiting typeahead input keys to the key buffer
 */
extern void keys_fill_buffer(KeyPtr key_buf);

/**
 * Reads next character from key buffer (return 0 if no key available)
 */
extern int keys_read_buffer(KeyPtr key_buf);

/**
 * Appends the specified character to the key buffer list.  Returns
 * 0 if key list overflows; returns new length of list otherwise.
 */
extern int keys_append_buffer(KeyPtr key_buf, int newchar);

/**
 * Inserts a character into the key buffer list *before* the specified
 * element (first element is 0).  Returns 0 if key list overflows (or
 * if "before" is out of range).  Otherwise, returns "before + 1" for
 * use in adding lists of characters to middle of queue.
 */
extern int keys_insert_buffer(KeyPtr key_buf, int newchar, int before);

/**
 * Wipes the old key buffer and inserts the specified string in its place.
 */
extern void keys_stuff_buffer(KeyPtr key_buf, int *newstuff);

/**
 * If "target" is an ALT-key combination, returns the ascii value
 * of the associated (upper case) letter.
 * If "target" is an ascii letter, returns the UPPER CASE version
 * of that letter.
 *
 * (Used by routines that trap ALT key and need to accept alt-key
 * and plain key as same key value)
 */
extern int keys_fix_alt(int target);

extern void keys_install();
extern void keys_remove();
extern int keys_check_install();
extern void keys_enable();
extern void keys_disable();

} // namespace MADSV2
} // namespace MADS

#endif
