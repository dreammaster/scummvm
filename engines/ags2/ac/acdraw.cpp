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

#include "ags2/ac/acdraw.h"
#include "ags2/ac/acroom.h"
#include "ags2/vars.h"

namespace AGS2 {

void clear_draw_list() {
	thingsToDrawSize = 0;
}

void add_thing_to_draw(IDriverDependantBitmap *bmp, int x, int y, int trans, bool alphaChannel) {
	thingsToDrawList[thingsToDrawSize].pic = NULL;
	thingsToDrawList[thingsToDrawSize].bmp = bmp;
	thingsToDrawList[thingsToDrawSize].x = x;
	thingsToDrawList[thingsToDrawSize].y = y;
	thingsToDrawList[thingsToDrawSize].transparent = trans;
	thingsToDrawList[thingsToDrawSize].hasAlphaChannel = alphaChannel;
	thingsToDrawSize++;
	if (thingsToDrawSize >= MAX_THINGS_TO_DRAW - 1)
		quit("add_thing_to_draw: too many things added");
}

void clear_sprite_list() {
	sprlistsize = 0;
}


void add_to_sprite_list(IDriverDependantBitmap *spp, int xx, int yy, int baseline, int trans, int sprNum, bool isWalkBehind) {

	// completely invisible, so don't draw it at all
	if (trans == 255)
		return;

	if ((sprNum >= 0) && ((game.spriteflags[sprNum] & SPF_ALPHACHANNEL) != 0))
		sprlist[sprlistsize].hasAlphaChannel = true;
	else
		sprlist[sprlistsize].hasAlphaChannel = false;

	sprlist[sprlistsize].bmp = spp;
	sprlist[sprlistsize].baseline = baseline;
	sprlist[sprlistsize].x = xx;
	sprlist[sprlistsize].y = yy;
	sprlist[sprlistsize].transparent = trans;

	if (walkBehindMethod == DrawAsSeparateSprite)
		sprlist[sprlistsize].takesPriorityIfEqual = !isWalkBehind;
	else
		sprlist[sprlistsize].takesPriorityIfEqual = isWalkBehind;

	sprlistsize++;

	if (sprlistsize >= MAX_SPRITES_ON_SCREEN)
		quit("Too many sprites have been added to the sprite list. There is a limit of 75 objects and characters being visible at the same time. You may want to reconsider your design since you have over 75 objects/characters visible at once.");

	if (spp == NULL)
		quit("add_to_sprite_list: attempted to draw NULL sprite");
}

void draw_sprite_compensate(int picc, int xx, int yy, int useAlpha) {
	if ((useAlpha) &&
		(game.options[OPT_NEWGUIALPHA] > 0) &&
		(bitmap_color_depth(abuf) == 32))
	{
		if (game.spriteflags[picc] & SPF_ALPHACHANNEL)
			set_additive_alpha_blender();
		else
			set_opaque_alpha_blender();

		draw_trans_sprite(abuf, spriteset[picc], xx, yy);
	} else
	{
		put_sprite_256(xx, yy, spriteset[picc]);
	}
}

void put_sprite_256(int xxx, int yyy, block piccy) {

	if (trans_mode >= 255) {
		// fully transparent, don't draw it at all
		trans_mode = 0;
		return;
	}

#ifdef USE_15BIT_FIX
	int screen_depth = bitmap_color_depth(abuf);

	if (bitmap_color_depth(piccy) < screen_depth) {

		if ((bitmap_color_depth(piccy) == 8) && (screen_depth >= 24)) {
			// 256-col sprite -> truecolor background
			// this is automatically supported by allegro, no twiddling needed
			draw_sprite(abuf, piccy, xxx, yyy);
			return;
		}
		// 256-col spirte -> hi-color background, or
		// 16-bit sprite -> 32-bit background
		block hctemp = create_bitmap_ex(screen_depth, piccy->w, piccy->h);
		blit(piccy, hctemp, 0, 0, 0, 0, hctemp->w, hctemp->h);
		int bb, cc, mask_col = bitmap_mask_color(abuf);
		if (bitmap_color_depth(piccy) == 8) {
			// only do this for 256-col, cos the blit call converts
			// transparency for 16->32 bit
			for (bb = 0; bb < hctemp->w; bb++) {
				for (cc = 0; cc < hctemp->h; cc++)
					if (_getpixel(piccy, bb, cc) == 0) putpixel(hctemp, bb, cc, mask_col);
			}
		}
		wputblock(xxx, yyy, hctemp, 1);
		wfreeblock(hctemp);
	} else
#endif
	{
		if ((trans_mode != 0) && (game.color_depth > 1) && (bmp_bpp(piccy) > 1) && (bmp_bpp(abuf) > 1)) {
			set_trans_blender(0, 0, 0, trans_mode);
			draw_trans_sprite(abuf, piccy, xxx, yyy);
		}
		/*    else if ((lit_mode < 0) && (game.color_depth == 1) && (bmp_bpp(piccy) == 1)) {
			  draw_lit_sprite(abuf,piccy,xxx,yyy,250 - ((-lit_mode) * 5)/2);
			  }*/
		else
			wputblock(xxx, yyy, piccy, 1);
	}
	trans_mode = 0;
}

void repair_alpha_channel(block dest, block bgpic) {
	// Repair the alpha channel, because sprites may have been drawn
	// over it by the buttons, etc
	int theWid = (dest->w < bgpic->w) ? dest->w : bgpic->w;
	int theHit = (dest->h < bgpic->h) ? dest->h : bgpic->h;
	for (int y = 0; y < theHit; y++)
	{
		unsigned long *destination = ((unsigned long *)dest->line[y]);
		unsigned long *source = ((unsigned long *)bgpic->line[y]);
		for (int x = 0; x < theWid; x++)
		{
			destination[x] |= (source[x] & 0xff000000);
		}
	}
}

} // namespace AGS2
