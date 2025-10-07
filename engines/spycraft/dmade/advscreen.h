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

#ifndef SPYCRAFT_DMADE_ADVSCREEN_H
#define SPYCRAFT_DMADE_ADVSCREEN_H

#include "spycraft/dmade/advback.h"
#include "spycraft/dmade/advlist.h"
#include "spycraft/dmade/advrect.h"

namespace Spycraft {

/* MADE Internal */

extern int screen_width;
extern int screen_height;
extern int screen_colors;

extern int scene_width;
extern int scene_height;

/*
	Reset the SpriteList size
*/
extern void sfxSetSpritelistSize(int listsize);


/*
	Initialize the game window size, and the SpriteList size
*/
extern void sfxSetGameDimensions(uint16 width, uint16 height, int listsize);

/*
	Initialize the screen module
*/
extern void InitScreen(void);

/*
	Cleanup after the screen module
*/
extern void CleanScreen(void);

/*
	Set port to copy dirty rectangles from
*/
extern void SetScreenPort(Viewport *port);

/*
	Add a rectangle to the area that needs to be updated
	on the screen
*/
extern void AddBltRect(ArrayList *list, SRect *rect);

/*
	Update the area that needs to be updated on the screen
*/
extern void UpdateScreen(Background *stage);

/*
	Set the screen palette to the given palatte.  This only
	applies if the screen is in 8 bit mode.
*/
extern void UpdatePalette(GamePalette *palette);

/*
	Update a rectangle on the screen
*/
extern void UpdateScreenRect(Viewport *port, SRect *rect);

} // namespace Spycraft

#endif
