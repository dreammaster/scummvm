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

#include "ags2/ac/draw.h"
#include "ags2/vars.h"

namespace AGS2 {


void update_walk_behind_images() {
	int ee, rr;
	int bpp = (bitmap_color_depth(thisroom.ebscene[play.bg_frame]) + 7) / 8;
	BITMAP *wbbmp;
	for (ee = 1; ee < MAX_OBJ; ee++)
	{
		update_polled_stuff();

		if (walkBehindRight[ee] > 0)
		{
			wbbmp = create_bitmap_ex(bitmap_color_depth(thisroom.ebscene[play.bg_frame]),
				(walkBehindRight[ee] - walkBehindLeft[ee]) + 1,
				(walkBehindBottom[ee] - walkBehindTop[ee]) + 1);
			clear_to_color(wbbmp, bitmap_mask_color(wbbmp));
			int yy, startX = walkBehindLeft[ee], startY = walkBehindTop[ee];
			for (rr = startX; rr <= walkBehindRight[ee]; rr++)
			{
				for (yy = startY; yy <= walkBehindBottom[ee]; yy++)
				{
					if (thisroom.object->line[yy][rr] == ee)
					{
						for (int ii = 0; ii < bpp; ii++)
							wbbmp->line[yy - startY][(rr - startX) * bpp + ii] = thisroom.ebscene[play.bg_frame]->line[yy][rr * bpp + ii];
					}
				}
			}

			update_polled_stuff();

			if (walkBehindBitmap[ee] != NULL)
			{
				gfxDriver->DestroyDDB(walkBehindBitmap[ee]);
			}
			walkBehindBitmap[ee] = gfxDriver->CreateDDBFromBitmap(wbbmp, false);
			destroy_bitmap(wbbmp);
		}
	}

	walkBehindsCachedForBgNum = play.bg_frame;
}

void recache_walk_behinds() {
	if (walkBehindExists) {
		free(walkBehindExists);
		free(walkBehindStartY);
		free(walkBehindEndY);
	}

	walkBehindExists = (char *)malloc(thisroom.object->w);
	walkBehindStartY = (int *)malloc(thisroom.object->w * sizeof(int));
	walkBehindEndY = (int *)malloc(thisroom.object->w * sizeof(int));
	noWalkBehindsAtAll = 1;

	int ee, rr, tmm;
	const int NO_WALK_BEHIND = 100000;
	for (ee = 0; ee < MAX_OBJ; ee++)
	{
		walkBehindLeft[ee] = NO_WALK_BEHIND;
		walkBehindTop[ee] = NO_WALK_BEHIND;
		walkBehindRight[ee] = 0;
		walkBehindBottom[ee] = 0;

		if (walkBehindBitmap[ee] != NULL)
		{
			gfxDriver->DestroyDDB(walkBehindBitmap[ee]);
			walkBehindBitmap[ee] = NULL;
		}
	}

	update_polled_stuff();

	// since this is an 8-bit memory bitmap, we can just use direct 
	// memory access
	if ((!is_linear_bitmap(thisroom.object)) || (bitmap_color_depth(thisroom.object) != 8))
		quit("Walk behinds bitmap not linear");

	for (ee = 0; ee < thisroom.object->w; ee++) {
		walkBehindExists[ee] = 0;
		for (rr = 0; rr < thisroom.object->h; rr++) {
			tmm = thisroom.object->line[rr][ee];
			//tmm = _getpixel(thisroom.object,ee,rr);
			if ((tmm >= 1) && (tmm < MAX_OBJ)) {
				if (!walkBehindExists[ee]) {
					walkBehindStartY[ee] = rr;
					walkBehindExists[ee] = tmm;
					noWalkBehindsAtAll = 0;
				}
				walkBehindEndY[ee] = rr + 1;  // +1 to allow bottom line of screen to work

				if (ee < walkBehindLeft[tmm]) walkBehindLeft[tmm] = ee;
				if (rr < walkBehindTop[tmm]) walkBehindTop[tmm] = rr;
				if (ee > walkBehindRight[tmm]) walkBehindRight[tmm] = ee;
				if (rr > walkBehindBottom[tmm]) walkBehindBottom[tmm] = rr;
			}
		}
	}

	if (walkBehindMethod == DrawAsSeparateSprite)
	{
		update_walk_behind_images();
	}
}

int get_walkable_area_pixel(int x, int y) {
	return getpixel(thisroom.walls, convert_to_low_res(x), convert_to_low_res(y));
}


// sort_out_walk_behinds: modifies the supplied sprite by overwriting parts
// of it with transparent pixels where there are walk-behind areas
// Returns whether any pixels were updated
int sort_out_walk_behinds(block sprit, int xx, int yy, int basel, block copyPixelsFrom,
		block checkPixelsFrom, int zoom) {
	if (noWalkBehindsAtAll)
		return 0;

	if ((!is_memory_bitmap(thisroom.object)) ||
		(!is_memory_bitmap(sprit)))
		quit("!sort_out_walk_behinds: wb bitmap not linear");

	int rr, tmm, toheight;//,tcol;
	// precalculate this to try and shave some time off
	int maskcol = bitmap_mask_color(sprit);
	int spcoldep = bitmap_color_depth(sprit);
	int screenhit = thisroom.object->h;
	short *shptr, *shptr2;
	long *loptr, *loptr2;
	int pixelsChanged = 0;
	int ee = 0;
	if (xx < 0)
		ee = 0 - xx;

	if ((checkPixelsFrom != NULL) && (bitmap_color_depth(checkPixelsFrom) != spcoldep))
		quit("sprite colour depth does not match background colour depth");

	for (; ee < sprit->w; ee++) {
		if (ee + xx >= thisroom.object->w)
			break;

		if ((!walkBehindExists[ee + xx]) ||
			(walkBehindEndY[ee + xx] <= yy) ||
			(walkBehindStartY[ee + xx] > yy + sprit->h))
			continue;

		toheight = sprit->h;

		if (walkBehindStartY[ee + xx] < yy)
			rr = 0;
		else
			rr = (walkBehindStartY[ee + xx] - yy);

		// Since we will use _getpixel, ensure we only check within the screen
		if (rr + yy < 0)
			rr = 0 - yy;
		if (toheight + yy > screenhit)
			toheight = screenhit - yy;
		if (toheight + yy > walkBehindEndY[ee + xx])
			toheight = walkBehindEndY[ee + xx] - yy;
		if (rr < 0)
			rr = 0;

		for (; rr < toheight; rr++) {

			// we're ok with _getpixel because we've checked the screen edges
			//tmm = _getpixel(thisroom.object,ee+xx,rr+yy);
			// actually, _getpixel is well inefficient, do it ourselves
			// since we know it's 8-bit bitmap
			tmm = thisroom.object->line[rr + yy][ee + xx];
			if (tmm < 1) continue;
			if (croom->walkbehind_base[tmm] <= basel) continue;

			if (copyPixelsFrom != NULL)
			{
				if (spcoldep <= 8)
				{
					if (checkPixelsFrom->line[(rr * 100) / zoom][(ee * 100) / zoom] != maskcol) {
						sprit->line[rr][ee] = copyPixelsFrom->line[rr + yy][ee + xx];
						pixelsChanged = 1;
					}
				} else if (spcoldep <= 16) {
					shptr = (short *)&sprit->line[rr][0];
					shptr2 = (short *)&checkPixelsFrom->line[(rr * 100) / zoom][0];
					if (shptr2[(ee * 100) / zoom] != maskcol) {
						shptr[ee] = ((short *)(&copyPixelsFrom->line[rr + yy][0]))[ee + xx];
						pixelsChanged = 1;
					}
				} else if (spcoldep == 24) {
					char *chptr = (char *)&sprit->line[rr][0];
					char *chptr2 = (char *)&checkPixelsFrom->line[(rr * 100) / zoom][0];
					if (memcmp(&chptr2[((ee * 100) / zoom) * 3], &maskcol, 3) != 0) {
						memcpy(&chptr[ee * 3], &copyPixelsFrom->line[rr + yy][(ee + xx) * 3], 3);
						pixelsChanged = 1;
					}
				} else if (spcoldep <= 32) {
					loptr = (long *)&sprit->line[rr][0];
					loptr2 = (long *)&checkPixelsFrom->line[(rr * 100) / zoom][0];
					if (loptr2[(ee * 100) / zoom] != maskcol) {
						loptr[ee] = ((long *)(&copyPixelsFrom->line[rr + yy][0]))[ee + xx];
						pixelsChanged = 1;
					}
				}
			} else
			{
				pixelsChanged = 1;
				if (spcoldep <= 8)
					sprit->line[rr][ee] = maskcol;
				else if (spcoldep <= 16) {
					shptr = (short *)&sprit->line[rr][0];
					shptr[ee] = maskcol;
				} else if (spcoldep == 24) {
					char *chptr = (char *)&sprit->line[rr][0];
					memcpy(&chptr[ee * 3], &maskcol, 3);
				} else if (spcoldep <= 32) {
					loptr = (long *)&sprit->line[rr][0];
					loptr[ee] = maskcol;
				} else
					quit("!Sprite colour depth >32 ??");
			}
		}
	}
	return pixelsChanged;
}

void invalidate_cached_walkbehinds() {
	memset(&actspswbcache[0], 0, sizeof(CachedActSpsData) * actSpsCount);
}

void sort_out_char_sprite_walk_behind(int actspsIndex, int xx, int yy, int basel, int zoom, int width, int height) {
	if (noWalkBehindsAtAll)
		return;

	if ((!actspswbcache[actspsIndex].valid) ||
		(actspswbcache[actspsIndex].xWas != xx) ||
		(actspswbcache[actspsIndex].yWas != yy) ||
		(actspswbcache[actspsIndex].baselineWas != basel))
	{
		actspswb[actspsIndex] = recycle_bitmap(actspswb[actspsIndex], bitmap_color_depth(thisroom.ebscene[play.bg_frame]), width, height);

		block wbSprite = actspswb[actspsIndex];
		clear_to_color(wbSprite, bitmap_mask_color(wbSprite));

		actspswbcache[actspsIndex].isWalkBehindHere = sort_out_walk_behinds(wbSprite, xx, yy, basel, thisroom.ebscene[play.bg_frame], actsps[actspsIndex], zoom);
		actspswbcache[actspsIndex].xWas = xx;
		actspswbcache[actspsIndex].yWas = yy;
		actspswbcache[actspsIndex].baselineWas = basel;
		actspswbcache[actspsIndex].valid = 1;

		if (actspswbcache[actspsIndex].isWalkBehindHere)
		{
			actspswbbmp[actspsIndex] = recycle_ddb_bitmap(actspswbbmp[actspsIndex], actspswb[actspsIndex], false);
		}
	}

	if (actspswbcache[actspsIndex].isWalkBehindHere)
	{
		add_to_sprite_list(actspswbbmp[actspsIndex], xx - offsetx, yy - offsety, basel, 0, -1, true);
	}
}

} // namespace AGS2
