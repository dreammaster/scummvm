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

#ifndef DINK_FREEDINK_H
#define DINK_FREEDINK_H

#include "dink/lib/wintypes.h"

namespace Dink {

extern int rnd();
extern int dir_num(const char *path);
extern void check_joystick();
extern int GetKeyboard(int key);
extern void Scrawl_OnMouseInput();
extern uint32 GetTickCount();
extern void Sleep(size_t seconds);
extern void process_show_bmp();
extern void flip_it();
extern void process_animated_tiles();
extern void process_item();
extern bool transition();
extern void up_cycle();
extern void process_warp_man();
extern void CyclePalette();
extern HRESULT restoreAll();
extern void process_talk();
extern void get_last_sprite();
extern void run_through_touch_damage_list(int h);

extern void bounce_brain(int h);
extern void no_brain(int h);
extern void duck_brain(int h);
extern void pig_brain(int h);
extern void one_time_brain(int h);
extern void repeat_brain(int h);
extern void one_time_brain_for_real(int h);
extern void text_brain(int h);
extern void pill_brain(int h);
extern void dragon_brain(int h);
extern void missile_brain(int h, bool repeat);
extern void scale_brain(int h);
extern void mouse_brain(int h);
extern void button_brain(int h);
extern void shadow_brain(int h);
extern void people_brain(int h);
extern void missile_brain_expire(int h);
extern void human_brain(int h);

extern int check_if_move_is_legal(int u);
extern int special_block(int block, int h);
extern bool in_this_base(int seq_, int base);
extern void run_through_tag_list(int h, int strength);
extern void did_player_cross_screen(bool real, int h);
extern void drawscreenlock();
extern bool initFail(HWND hWnd, const char *mess);
extern void move(int u, int amount, char kind, char kindy);
extern void text_draw(int h, HDC hdc);
extern int check_if_move_is_legal(int u);
extern void run_through_tag_list_push(int h);
extern int check_if_move_is_legal(int u);
extern void draw_box(RECT box, int color);

/**
 * Do work required for initializing graphics and data
 */
extern bool doInit();

/**
 * Finished with all objects we use; release them
 */
extern void finiObjects();

} // namespace Dink

#endif
