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

#ifndef DUNGEONCRAFT_SHARED_CHARSTATSFORM
#define DUNGEONCRAFT_SHARED_CHARSTATSFORM

#include "dungeoncraft/shared/char.h"
#include "dungeoncraft/shared/TextForm.h"

namespace DungeonCraft {

enum CHAR_STATS_INPUT_MESSAGE_TYPE
{
  CHOOSESTATS_initial,    // (character)
  CHOOSESTATS_refresh,    // (character) redraw the screen
  CHOOSESTATS_mouseClick, // (character, x, y)
  CHOOSESTATS_key,        // (character, key)
  CHOOSESTATS_exit        // (void)
};

void displayCharStats(CHARACTER &dude);
int  handleChooseStatsInput(CHAR_STATS_INPUT_MESSAGE_TYPE msg,
                                   CHARACTER *dude=NULL, 
                                   int P1=0, 
                                   int P2=0);
extern char *CharStatusTypeText[];
extern char *CharGenderTypeText[];
extern char *CharAlignmentTypeText[];

} // End of namespace DungeonCraft

#endif
