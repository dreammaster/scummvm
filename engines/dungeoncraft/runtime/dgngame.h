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

#ifndef DUNGEONCRAFT_SHARED_DGNGAME
#define DUNGEONCRAFT_SHARED_DGNGAME

#include "dungeoncraft/shared/dungeon.h"
#include "dungeoncraft/shared/Thread.h"

namespace DungeonCraft {

BOOL serializeGame(BOOL save, int num);
BOOL readDatabase(void);
void scheduler( void );
BOOL InitGame(HWND hwnd);
void ShutdownGame(void);
void InitGameStructures(void);
void ResetAdventureData(void);
BOOL LoadOffscreenWalkingSurfaces(void);
BOOL LoadOffscreenCombatSurfaces(BOOL UseOutdoor);
BOOL DesignAvail(const char *path);
BOOL InitDesignData();
void StartBackgroundMusic();
void StartBackgroundZoneOnlyMusic();
void StopBackgroundMusic();

} // End of namespace DungeonCraft

#endif
