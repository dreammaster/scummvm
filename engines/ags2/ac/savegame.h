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

#ifndef AGS2_AC_SAVEGAME_H
#define AGS2_AC_SAVEGAME_H

#include "common/scummsys.h"

namespace AGS2 {

extern void restore_after_dialog();
extern void RestoreGameSlot(int slnum);
extern void get_save_game_path(int slotNum, char *buffer);
extern void DeleteSaveSlot(int slnum);
extern int Game_SetSaveGameDirectory(const char *newFolder);
extern int GetSaveSlotDescription(int slnum, char *desbuf);
extern const char *Game_GetSaveSlotDescription(int slnum);
extern int LoadSaveSlotScreenshot(int slnum, int width, int height);
extern int load_game_and_print_error(int toload);
extern void restore_game_dialog();
extern void save_game_dialog();
extern void restart_game();
extern void SetRestartPoint();
extern void save_game(int slotn, const char *descript);

} // namespace AGS2

#endif
