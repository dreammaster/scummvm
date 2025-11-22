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

#include "ags2/gfx/sprite.h"
#include "ags2/vars.h"

namespace AGS2 {

void initialize_sprite(int ee) {
	if ((ee < 0) || (ee > spriteset.elements))
		error("initialize_sprite: invalid sprite number");

	if ((spriteset[ee] == NULL) && (ee > 0)) {
		// replace empty sprites with blue cups, to avoid crashes
		//spriteset[ee] = spriteset[0];
		spriteset.set(ee, spriteset[0]);
		spritewidth[ee] = spritewidth[0];
		spriteheight[ee] = spriteheight[0];
	} else if (spriteset[ee] == NULL) {
		spritewidth[ee] = 0;
		spriteheight[ee] = 0;
	} else {
		// stretch sprites to correct resolution
		int oldeip = our_eip;
		our_eip = 4300;

		if (game.spriteflags[ee] & SPF_HADALPHACHANNEL) {
			// we stripped the alpha channel out last time, put
			// it back so that we can remove it properly again
			game.spriteflags[ee] |= SPF_ALPHACHANNEL;
		}

		curspr = spriteset[ee];
		get_new_size_for_sprite(ee, curspr->w, curspr->h, newwid, newhit);

		eip_guinum = ee;
		eip_guiobj = newwid;

		if ((newwid != curspr->w) || (newhit != curspr->h)) {
			tmpdbl = create_bitmap_ex(bitmap_color_depth(curspr), newwid, newhit);
			if (tmpdbl == NULL)
				error("Not enough memory to load sprite graphics");
			acquire_bitmap(tmpdbl);
			acquire_bitmap(curspr);
			clear_to_color(tmpdbl, bitmap_mask_color(tmpdbl));
			/*#ifdef USE_CUSTOM_EXCEPTION_HANDLER
				  __try {
			#endif*/
			stretch_sprite(tmpdbl, curspr, 0, 0, tmpdbl->w, tmpdbl->h);
			/*#ifdef USE_CUSTOM_EXCEPTION_HANDLER
				  } __except (1) {
					// I can't trace this fault, but occasionally stretch_sprite
					// crashes, even with valid source and dest bitmaps. So,
					// for now, just ignore the exception, since the stretch
					// looks successful
				  //MessageBox (allegro_wnd, "ERROR", "FATAL ERROR", MB_OK);
				  }
			#endif*/
			release_bitmap(curspr);
			release_bitmap(tmpdbl);
			wfreeblock(curspr);
			spriteset.set(ee, tmpdbl);
		}

		spritewidth[ee] = wgetblockwidth(spriteset[ee]);
		spriteheight[ee] = wgetblockheight(spriteset[ee]);

		int spcoldep = bitmap_color_depth(spriteset[ee]);

		if (((spcoldep > 16) && (final_col_dep <= 16)) ||
			((spcoldep == 16) && (final_col_dep > 16))) {
			// 16-bit sprite in 32-bit game or vice versa - convert
			// so that scaling and draw_sprite calls work properly
			block oldSprite = spriteset[ee];
			block newSprite;

			if (game.spriteflags[ee] & SPF_ALPHACHANNEL)
				newSprite = remove_alpha_channel(oldSprite);
			else {
				newSprite = create_bitmap_ex(final_col_dep, spritewidth[ee], spriteheight[ee]);
				blit(oldSprite, newSprite, 0, 0, 0, 0, spritewidth[ee], spriteheight[ee]);
			}
			spriteset.set(ee, newSprite);
			destroy_bitmap(oldSprite);
			spcoldep = final_col_dep;
		} else if ((spcoldep == 32) && (final_col_dep == 32) &&
			((game.spriteflags[ee] & SPF_ALPHACHANNEL) != 0))
		{
			set_rgb_mask_using_alpha_channel(spriteset[ee]);
		}
#ifdef USE_15BIT_FIX
		else if ((final_col_dep != game.color_depth * 8) && (spcoldep == game.color_depth * 8)) {
			// running in 15-bit mode with a 16-bit game, convert sprites
			block oldsprite = spriteset[ee];

			if (game.spriteflags[ee] & SPF_ALPHACHANNEL)
				// 32-to-24 with alpha channel
				spriteset.set(ee, remove_alpha_channel(oldsprite));
			else
				spriteset.set(ee, convert_16_to_15(oldsprite));

			destroy_bitmap(oldsprite);
		}
		if ((convert_16bit_bgr == 1) && (bitmap_color_depth(spriteset[ee]) == 16))
			spriteset.set(ee, convert_16_to_16bgr(spriteset[ee]));
#endif

		if ((spcoldep == 8) && (final_col_dep > 8))
			select_palette(palette);

		spriteset.set(ee, gfxDriver->ConvertBitmapToSupportedColourDepth(spriteset[ee]));

		if ((spcoldep == 8) && (final_col_dep > 8))
			unselect_palette();

		if (final_col_dep < 32) {
			game.spriteflags[ee] &= ~SPF_ALPHACHANNEL;
			// save the fact that it had one for the next time this
			// is re-loaded from disk
			game.spriteflags[ee] |= SPF_HADALPHACHANNEL;
		}

		platform->RunPluginHooks(AGSE_SPRITELOAD, ee);
		update_polled_stuff();

		our_eip = oldeip;
	}
}

void get_new_size_for_sprite(int ee, int ww, int hh, int &newWidth, int &newHeight) {
	newWidth = ww * current_screen_resolution_multiplier_x;
	newHeight = hh * current_screen_resolution_multiplier_y;

	// TODO: Check original logic
}

block remove_alpha_channel(block from) {
	int depth = final_col_dep;

	block to = create_bitmap_ex(depth, from->w, from->h);
	int maskcol = bitmap_mask_color(to);
	int y, x;
	unsigned long c, b, g, r;

	if (depth == 24) {
		// 32-to-24
		for (y = 0; y < from->h; y++) {
			unsigned long *psrc = (unsigned long *)from->line[y];
			unsigned char *pdest = (unsigned char *)to->line[y];

			for (x = 0; x < from->w; x++) {
				c = psrc[x];
				// less than 50% opaque, remove the pixel
				if (((c >> 24) & 0x00ff) < 128)
					c = maskcol;

				// copy the RGB values across
				memcpy(&pdest[x * 3], &c, 3);
			}
		}
	} else {  // 32 to 15 or 16

		for (y = 0; y < from->h; y++) {
			unsigned long *psrc = (unsigned long *)from->line[y];
			unsigned short *pdest = (unsigned short *)to->line[y];

			for (x = 0; x < from->w; x++) {
				c = psrc[x];
				// less than 50% opaque, remove the pixel
				if (((c >> 24) & 0x00ff) < 128)
					pdest[x] = maskcol;
				else {
					// otherwise, copy it across
					r = (c >> 16) & 0x00ff;
					g = (c >> 8) & 0x00ff;
					b = c & 0x00ff;
					pdest[x] = makecol_depth(depth, r, g, b);
				}
			}
		}
	}

	return to;
}

void set_rgb_mask_using_alpha_channel(block image) {
	int x, y;

	for (y = 0; y < image->h; y++)
	{
		unsigned long *psrc = (unsigned long *)image->line[y];

		for (x = 0; x < image->w; x++)
		{
			if ((psrc[x] & 0xff000000) == 0x00000000)
				psrc[x] = MASK_COLOR_32;
		}
	}
}

void pre_save_sprite(int ee) {
	// not used, we don't save
}

} // namespace AGS2
