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

#ifndef SPYCRAFT_DMADE_ADVBACK_H
#define SPYCRAFT_DMADE_ADVBACK_H

#include "common/array.h"
#include "common/ptr.h"
#include "spycraft/dmade/advlib.h"
#include "spycraft/dmade/advlist.h"
#include "spycraft/dmade/advport.h"
#include "spycraft/dmade/advrect.h"
#include "spycraft/dmade/advres.h"

namespace Spycraft {

/* MADE External */

	// special effects
#define VE_FLIP					((visualEffect)0)
#define VE_WIPE_RIGHT			((visualEffect)1)
#define VE_WIPE_LEFT			((visualEffect)2)
#define VE_WIPE_DOWN			((visualEffect)3)
#define VE_WIPE_UP				((visualEffect)4)
#define VE_WIPE_CEN_VERT		((visualEffect)5)
#define VE_WIPE_CEN_HORZ		((visualEffect)6)
#define VE_WIPE_CEN_OUT			((visualEffect)7)
#define VE_FULL_DISSOLVE		((visualEffect)8)
#define VE_DISSOLVE_RIGHT		((visualEffect)9)
#define VE_DISSOLVE_LEFT		((visualEffect)10)
#define VE_DISSOLVE_DOWN		((visualEffect)11)
#define VE_DISSOLVE_UP			((visualEffect)12)
#define VE_DISSOLVE_CEN_OUT		((visualEffect)13)
#define VE_DISSOLVE_CEN_HORZ	((visualEffect)14)
#define VE_DISSOLVE_CEN_VERT	((visualEffect)15)
#define VE_FADE					((visualEffect)16)
#define VE_SCROLL_LEFT			((visualEffect)17)
#define VE_SCROLL_RIGHT			((visualEffect)18)
#define VE_SCROLL_DOWN			((visualEffect)19)
#define VE_SCROLL_UP			((visualEffect)20)
#define	VE_WHIP_PAN_LEFT		((visualEffect)21)
#define	VE_WHIP_PAN_RIGHT		((visualEffect)22)

typedef int16 bufferNum;		// background buffer magic number
typedef uint16 visualEffect;	// predefined type for a visual effect
typedef uint16 channelNum;		// layering of items on a background

class SpriteArray;

//	Function:	sfxMakeDisplayBuffer
//
//	Purpose:	draws a FLEX image into a new offscreen buffer.
//				This allocates as much memory as the offscreen
//				buffer requires.
//
//	Parameters: pic_num - number of the FLEX in the project file
//				display_rect - the coordinates to display onscreen
//				flip_x - if true image is to be horizontally reversed
//				flip_y - if true image is to be vertically reversed
//				
//	Returns: 	the buffer number (to use in sfxVisualEffect, etc) if > 0,
//				if < 0, an error.

extern bufferNum sfxMakeDisplayBuffer(rsrcID pic_num, MADERect *display_rect, bool flip_x, bool flip_y);

//	Function:	sfxKillDisplayBuffer
//
//	Purpose:	releases the memory associated with an offscreen buffer
//
//	Parameters: buffer_num - the buffer number, as returned by sfxMakeDisplayBuffer
//					if buffer_num is 0 or the current buffer, sfxKillDisplayBuffer
//					will do nothing
//				
//	Returns: 	void

extern void sfxKillDisplayBuffer(bufferNum buffer_num);

//	Function:	sfxGetDisplayRect
//
//	Purpose:	gets the current display rect associated with a particular buffer
//
//	Parameters: buffer_num - the buffer number, as returned by sfxMakeDisplayBuffer;
//							if 0, the current on-screen buffer
//				
//	Returns: 	the current display rectangle in display_rect

void sfxGetDisplayRect(bufferNum buffer_num, MADERect *display_rect);


//	Function:	sfxGetDisplayBufferRect
//
//	Purpose:	gets the _COMPLETE_ rectangle covered by the display buffer
//
//	Parameters: buffer_num - the buffer number, as returned by sfxMakeDisplayBuffer;
//							if 0, the current on-screen buffer
//				
//	Returns: 	the buffer rectangle in buffer_rect

extern void sfxGetDisplayBufferRect(bufferNum buffer_num, MADERect *buffer_rect);


//	Function:	sfxSetDisplayRect
//
//	Purpose:	sets the current display rect associated with a particular buffer
//
//	Parameters: buffer_num - the buffer number, as returned by sfxMakeDisplayBuffer;
//							if 0, the current on-screen buffer
//				display_rect - pointer to the rectangle to be displayed onscreen
//				
//	Returns: 	void

extern void sfxSetDisplayRect(bufferNum buffer_num, MADERect *display_rect);


/****************** Visual Effects Functions *******************/

//	Function:	sfxFlash
//
//	Purpose:	inverts the palette briefly.
//
//	Parameters: none
//
//	Returns: 	none

extern void sfxFlash(void);


//	Function:	sfxShakeScreen
//
//	Purpose:	jiggles the screen up, down, left, and right to simulate
//				shaking.
//
//	Parameters: none 
//
//	Returns: 	none

extern void sfxShakeScreen(void);


//	Function:	sfxVisualEffect
//
//	Purpose:	plays a transitional visual effect and then moves
//				the given buffer to the screen. After this, it
//				is still necessary to update the sprites.
//
//	Parameters: effect - legal effects are as follows:
//					VE_FLIP 
//					VE_WIPE_RIGHT 
//					VE_WIPE_LEFT 
//					VE_WIPE_DOWN
//					VE_WIPE_UP
//					VE_WIPE_CEN_VERT
//					VE_WIPE_CEN_HORZ
//					VE_WIPE_CEN_OUT 
//					VE_FULL_DISSOLVE 
//					VE_DISSOLVE_RIGHT
//					VE_DISSOLVE_LEFT
//					VE_DISSOLVE_DOWN
//					VE_DISSOLVE_UP
//					VE_DISSOLVE_CEN_OUT
//					VE_DISSOLVE_CEN_HORZ
//					VE_DISSOLVE_CEN_VERT
//					VE_FADE 
//					VE_SCROLL_LEFT 
//					VE_SCROLL_RIGHT 
//					VE_SCROLL_DOWN 
//					VE_SCROLL_UP 
//					VE_WHIP_PAN_LEFT
//					VE_WHIP_PAN_RIGHT
//
//	            bufferNum new_buffer - backPort to display
//	            uint16    steps      - fraction of display to show at a time
//              uint32    stepTime   - time interval in ms for each step
//
//	Returns: 	none

extern void sfxVisualEffect(visualEffect effect, bufferNum new_buffer,
	uint16 steps, uint32 stepTime);

extern void sfxShowFrame(void);

/* MADE Internal */

typedef struct {
	uint8 red;
	uint8 green;
	uint8 blue;
} GamePaletteEntry, GamePalette[256];

struct Background {
	Viewport *animPort;
	Viewport *backPort;
	SRect displayRect;
	SpriteArray *spriteList;
	ArrayList *updateRgn;
	ArrayList *updateRects;
	ArrayList *underbits;
	SpriteArray *deleteList;
	SpriteArray *hiddenList;
	GamePalette palette;
	int picNum;
	bool released;
};

extern int curBack;
extern Background *backgrounds[];
extern Viewport *SparePort;
extern Viewport *animport;
extern int spritelist_size;

void InitBackgrounds(void);
void CleanBackgrounds(void);
Background *GetBackground(short back);
bufferNum GetBackgroundNum(Background *background);

} // namespace Spycraft

#endif
