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

#include "spycraft/dmade/advlib.h"
#include "spycraft/dmade/advscreen.h"
#include "spycraft/dmade/advback.h"
#include "spycraft/dmade/adverror.h"
#include "spycraft/dmade/advport.h"
#include "spycraft/dmade/advsprite.h"
#include "spycraft/dmade/advbits.h"
#include "spycraft/dmade/advlist.h"
#include "spycraft/dmade/advsfx.h"
#include "spycraft/dmade/advcursor.h"
#include "spycraft/dmade/advtext.h"

namespace Spycraft {

#define TEXT_WIDTH		512
#define TEXT_HEIGHT		32

Viewport *SparePort = nullptr;
Viewport *animport = nullptr;     // game's compositing DIB 

int picType = RES_PIC;
int curBack = -1;      // index of background on screen

#define MAX_BACKGROUNDS 2

Background *backgrounds[MAX_BACKGROUNDS];

extern bool CompareSprite(void *obj1, void *obj2);

void InitBackgrounds() {
	int i;
	Background *background;
	ArrayList *spriteList, *updateRgn, *updateRects;
	ArrayList *underbits, *deleteList, *hiddenList;

	// allocate animport
	animport = (Viewport *)AllocMDPort(scene_width, scene_height, screen_colors);
	if (animport == nullptr)
		ADV_ASSERT(false, __ERR_PORT_ALLOC_FAIL);

	// allocate SparePort
	SparePort = (Viewport *)AllocMDPort(TEXT_WIDTH, TEXT_HEIGHT, screen_colors);
	if (SparePort == nullptr)
		ADV_ASSERT(false, __ERR_PORT_ALLOC_FAIL);

	for (i = 0; i < MAX_BACKGROUNDS; i++) {

		// allocate background
		background = (Background *)AllocPtr(sizeof(Background));
		if (background == nullptr)
			ADV_ASSERT(false, __ERR_MEM_ALLOC_FAIL);

		backgrounds[i] = background;

		// initialize background's palette to black
		memset(&background->palette, 0, sizeof(GamePalette));

		// allocate a sprite array list for background
		spriteList = ArrayList_Calloc(spritelist_size);
		if (spriteList == nullptr) {
			FreePtr(background);
			ADV_ASSERT(false, __ERR_MEM_ALLOC_FAIL);
		}

		// allocate an update region array list for background
		updateRgn = ArrayList_Calloc(4 * spritelist_size);
		if (updateRgn == nullptr) {
			FreePtr(background);
			ArrayList_Free(spriteList, nullptr);
			ADV_ASSERT(false, __ERR_MEM_ALLOC_FAIL);
		}

		// allocate an update rects array list for background
		updateRects = ArrayList_Calloc(2 * spritelist_size);
		if (updateRects == nullptr) {
			FreePtr(background);
			ArrayList_Free(spriteList, nullptr);
			ArrayList_Free(updateRgn, nullptr);
			ADV_ASSERT(false, __ERR_MEM_ALLOC_FAIL);
		}

		// allocate an underbits array list for background
		underbits = ArrayList_Calloc(2 * spritelist_size);
		if (underbits == nullptr) {
			FreePtr(background);
			ArrayList_Free(spriteList, nullptr);
			ArrayList_Free(updateRgn, nullptr);
			ArrayList_Free(updateRects, nullptr);
			ADV_ASSERT(false, __ERR_MEM_ALLOC_FAIL);
		}

		// allocate an delete array list for background
		deleteList = ArrayList_Calloc(spritelist_size);
		if (deleteList == nullptr) {
			FreePtr(background);
			ArrayList_Free(spriteList, nullptr);
			ArrayList_Free(updateRgn, nullptr);
			ArrayList_Free(updateRects, nullptr);
			ArrayList_Free(underbits, nullptr);
			ADV_ASSERT(false, __ERR_MEM_ALLOC_FAIL);
		}

		// allocate an hidden array list for background
		hiddenList = ArrayList_Alloc();
		if (hiddenList == nullptr) {
			FreePtr(background);
			ArrayList_Free(spriteList, nullptr);
			ArrayList_Free(updateRgn, nullptr);
			ArrayList_Free(updateRects, nullptr);
			ArrayList_Free(underbits, nullptr);
			ArrayList_Free(deleteList, nullptr);
			ADV_ASSERT(false, __ERR_MEM_ALLOC_FAIL);
		}

		// store pointer to  animPort
		background->animPort = animport;

		// set pointer to backPort to nullptr to start
		background->backPort = nullptr;

		// store Sprite Arraylist
		background->spriteList = spriteList;

		// store Update Region Arraylist
		background->updateRgn = updateRgn;

		// store UpdateRect Arraylist
		background->updateRects = updateRects;

		// store underbits Arraylist
		background->underbits = underbits;

		// store delete Arraylist
		background->deleteList = deleteList;

		// store hidden ArrayList
		background->hiddenList = hiddenList;

		// signify that background is free to be used
		background->released = true;
	}
}

Background *GetBackground(short back) {
	return(backgrounds[back]);
}

bufferNum GetBackgroundNum(Background *background) {
	bufferNum num;

	for (num = 0; num < MAX_BACKGROUNDS; num++)
		if (backgrounds[num] == background)
			return(num);

	return(-1);
}

void CleanBackgrounds(void) {
	int i;
	Background *background;

	if (curBack < 0)
		return;

	if (animport != nullptr) {
		FreeMDPort(animport);
		animport = nullptr;
	}
	if (SparePort != nullptr) {
		FreeMDPort(SparePort);
		SparePort = nullptr;
	}

	for (i = 0; i < MAX_BACKGROUNDS; i++) {

		background = (Background *)backgrounds[i];

		if (background != nullptr) {

			// unlock final resources
			if (background->backPort) {
				if (background->picNum != -1)
					sfxUnlockRes(background->picNum, picType);
			}

			// clear out any text sprites
			FreeTextSlots((bufferNum)i);

			// need to delete each sprite here
			if (background->spriteList) {
				ArrayList_Free(background->spriteList, FreeSpriteFn);
				background->spriteList = nullptr;
			}

			if (background->updateRgn) {
				ArrayList_Free(background->updateRgn, nullptr);
				background->updateRgn = nullptr;
			}

			if (background->underbits) {
				ArrayList_Free(background->underbits, nullptr);
				background->underbits = nullptr;
			}

			if (background->updateRects) {
				ArrayList_Free(background->updateRects, nullptr);
				background->underbits = nullptr;
			}

			if (background->hiddenList) {
				ArrayList_Free(background->hiddenList, FreeSpriteFn);
				background->hiddenList = nullptr;
			}

			if (background->deleteList) {
				ArrayList_Release(background->deleteList);
				ArrayList_Free(background->deleteList, nullptr);
				background->deleteList = nullptr;
			}

			FreePtr(background);
			backgrounds[i] = nullptr;

		}
	}

	curBack = -1;
}


short get_next_background_slot() {
	short i;

	for (i = 0; i < MAX_BACKGROUNDS; i++) {
		if (backgrounds[i]->released == true)
			return i;
	}

	return -1;
}

extern int screen_colors;
extern void SetBackMem(const char *, int);

bufferNum sfxMakeDisplayBuffer(rsrcID pic_num, MADERect *display_rect,
	bool flip_x, bool flip_y) {
	Background *background;
	Viewport *backport;
	bufferNum index;
	uint16 *bptr;
	int total;

	// check for valid display_rect 
	if (((display_rect->right - display_rect->left + 1) > scene_width) ||
		((display_rect->bottom - display_rect->top + 1) > scene_height))
		ADV_ASSERT(false, __ERR_CODING);

	// find background slot for this pic
	index = get_next_background_slot();
	if (index == -1)
		ADV_ASSERT(false, __ERR_CODING);

	background = backgrounds[index];

	/* LOADING BACKGROUND RESOURCE */
	if (pic_num == -1) {
		backport = AllocPort(scene_width, scene_height, screen_colors);
		if (backport == nullptr) {
			FreePtr(background);
			ADV_ASSERT(false, __ERR_PORT_ALLOC_FAIL);
		}
		memset(backport->ptr, 0, backport->rowBytes * backport->height);
	} else {
		backport = (Viewport *)sfxLoadRes(pic_num, picType);
		if (backport == nullptr) {
			FreePtr(background);
			ADV_ASSERT(false, __ERR_PORT_ALLOC_FAIL);
		}
	}

	// check again for valid display_rect vis-a-vis pic size
	if (((display_rect->right - display_rect->left + 1) > backport->width) ||
		((display_rect->bottom - display_rect->top + 1) > backport->height))
		ADV_ASSERT(false, __ERR_CODING);

	// okay to lock pic resource
	if (pic_num != -1)
		sfxLockRes(pic_num, picType);

	// store pic's info in background structure
	background->picNum = pic_num;
	background->backPort = backport;
	background->released = false;

	// store display rect of pic
	SRect_Init(&background->displayRect,
		display_rect->left, display_rect->top,
		display_rect->right, display_rect->bottom);


	SetScreenPort(animport);

	// set balance, if any, of backport and SparePort to black
	bptr = (uint16 *)backport->ptr + scene_width * (backport->height);

	total = (scene_height - backport->height) * scene_width;
	memset(bptr, 0, total);

	return (index);
}

void sfxKillDisplayBuffer(bufferNum buffer_num) {
	Background *background;

	if (buffer_num < 0 || buffer_num >= MAX_BACKGROUNDS)
		ADV_ASSERT(false, __ERR_CODING);

	background = backgrounds[buffer_num];

	// unlock pic resource, free text slots, and release array lists for this background
	if (background != nullptr) {

		if (background->picNum == -1)
			FreePort(background->backPort);
		else
			sfxUnlockRes(background->picNum, picType);

		FreeTextSlots(buffer_num);
		if (!ArrayList_Unlink(background->spriteList, cursor))
			if (!ArrayList_Unlink(background->hiddenList, cursor))
				ADV_ASSERT(0, 0);
		CleanSpriteList(background);
		ArrayList_Release(background->spriteList);
		ArrayList_Release(background->updateRgn);
		ArrayList_Release(background->underbits);
		ArrayList_Release(background->updateRects);
		ArrayList_Release(background->hiddenList);
		ArrayList_Release(background->deleteList);
		background->released = true;
	}
}

void sfxSetDisplayRect(bufferNum buffer_num, MADERect *display_rect) {
	Background *background_ptr;

	background_ptr = backgrounds[buffer_num];
	background_ptr->displayRect.left = display_rect->left;
	background_ptr->displayRect.right = display_rect->right;
	background_ptr->displayRect.top = display_rect->top;
	background_ptr->displayRect.bottom = display_rect->bottom;

	sfxVisualEffect(VE_FLIP, buffer_num, 0, 0);
}

void sfxGetDisplayRect(bufferNum buffer_num, MADERect *display_rect) {
	if (buffer_num >= MAX_BACKGROUNDS || buffer_num < 0)
		ADV_ASSERT(false, __ERR_CODING);

	if (backgrounds[buffer_num] == nullptr)
		ADV_ASSERT(false, __ERR_CODING);

	display_rect->left = backgrounds[buffer_num]->displayRect.left;
	display_rect->right = backgrounds[buffer_num]->displayRect.right;
	display_rect->top = backgrounds[buffer_num]->displayRect.top;
	display_rect->bottom = backgrounds[buffer_num]->displayRect.bottom;
}

void sfxGetDisplayBufferRect(bufferNum buffer_num, MADERect *buffer_rect) {
	if (buffer_num >= MAX_BACKGROUNDS || buffer_num < 0)
		ADV_ASSERT(false, __ERR_CODING);

	if (backgrounds[buffer_num] == nullptr)
		ADV_ASSERT(false, __ERR_CODING);

	buffer_rect->left = 0;
	buffer_rect->right = backgrounds[buffer_num]->backPort->width;
	buffer_rect->top = 0;
	buffer_rect->bottom = backgrounds[buffer_num]->backPort->height;
}

inline int List_Contains(ArrayList *list, void *obj) {
	int i;

	for (i = 0; i < list->size; i++) {
		if (list->elements[i] == obj)
			return true;
	}
	return false;
}

void sfxVisualEffect(visualEffect effect, bufferNum new_buffer,
	uint16 steps, uint32 stepTime) {
	Background *new_background, *cur_background;
	Viewport *from_port, *to_port;

	if (new_buffer < 0 || new_buffer > MAX_BACKGROUNDS)
		ADV_ASSERT(false, __ERR_CODING);

	if (curBack >= 0)
		cur_background = backgrounds[curBack];
	else
		cur_background = nullptr;
	new_background = backgrounds[new_buffer];

	if (new_background == nullptr)
		ADV_ASSERT(false, __ERR_CODING);

	if (cur_background != nullptr)
		from_port = cur_background->animPort;
	else
		from_port = nullptr;
	to_port = new_background->animPort;

	switch (effect)
	{
	case(VE_FULL_DISSOLVE):
	case(VE_FLIP):
		/* UPDATE ENTIRE SCREEN */
		DoFlip(cur_background, new_background);
		DirectRelease(new_background, 0, 0);
		break;

	default:
		ADV_ASSERT(false, __ERR_CODING);
		break;
	}

	// add current cursor to new background's arraylist
	if (cur_background != nullptr && cursor != nullptr) {
		cursor->back = new_background;
		if (cursor->state & SPRITE_HIDDEN) {
			if (!List_Contains(new_background->hiddenList, cursor))
				ArrayList_Add(new_background->hiddenList, cursor, nullptr);
		} else {
			if (!List_Contains(new_background->spriteList, cursor))
				ArrayList_Add(new_background->spriteList, cursor, CompareSprite);
		}
	}

	// save index of current front picture
	curBack = new_buffer;

	// start cursor at current mouse position
	InitCursorPos();
}

} // namespace Spycraft
