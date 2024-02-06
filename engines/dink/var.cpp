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

#include "common/debug.h"
#include "common/file.h"
#include "image/bmp.h"
#include "dink/dink.h"
#include "dink/events.h"
#include "dink/fast_file.h"
#include "dink/file.h"
#include "dink/graphics.h"
#include "dink/music.h"
#include "dink/scripts.h"
#include "dink/sound.h"
#include "dink/text.h"
#include "dink/var.h"
#include "dink/lib/wintypes.h"
#include "dink/directdraw/ddutil.h"

namespace Dink {

void dink_strcpy(char *dst, const char *s) {
	Common::strcpy_s(dst, 255, s);
}

void dink_strcat(char *dst, const char *s) {
	Common::strcat_s(dst, 255, s);
}

void dink_sprintf(char *dst, const char *fmt, ...) {
	va_list va;
	va_start(va, fmt);
	Common::String tmp = Common::String::vformat(fmt, va);
	va_end(va);

	dink_strcpy(dst, tmp.c_str());
}

void clear_talk() {
	memset(&talk, 0, sizeof(talk));
	play.mouse = 0;
}

IDirectDrawSurface *DDSethLoad(IDirectDraw *pdd, LPCSTR szBitmap, int dx, int dy, int sprite) {
	IDirectDrawSurface *surf = DDLoadBitmap(pdd, szBitmap);
	if (!surf)
		return nullptr;

	if (sprite > 0) {
		picInfo[sprite].box.top = 0;
		picInfo[sprite].box.left = 0;
		picInfo[sprite].box.right = surf->w;
		picInfo[sprite].box.bottom = surf->h;
	}

	return surf;
}

void SaySmall(char thing[500], int px, int py, int r, int g, int b) {
	RECT rcRect;
	HDC         hdc;
	int dum;
	char shit[200];
	if (lpDDSBack->GetDC(&hdc) == DD_OK) {
		SetBkMode(hdc, TRANSPARENT);
		SetRect(&rcRect, px, py, px + 40, py + 40);
		//SelectObject (hdc, hfont);
		//if (hfont == NULL) Msg("CreafontIndirect failed. haha.");
		SetTextColor(hdc, RGB(r, g, b));
		DrawText(hdc, thing, lstrlen(thing), &rcRect, DT_WORDBREAK);

		//      OffsetRect(&rcRect,-1,-1);
		//  SetTextColor(hdc,RGB(150,150,150));
		//     DrawText(hdc,thing,lstrlen(thing),&rcRect,DT_WORDBREAK);

		dum =  GetTextFace(hdc, 100, shit) ;
		lpDDSBack->ReleaseDC(hdc);
	}
}


void Say(char thing[500], int px, int py) {
	RECT rcRect;
	HDC         hdc;
	int dum;
	char shit[200];


	if (lpDDSBack->GetDC(&hdc) == DD_OK) {
		SetBkMode(hdc, TRANSPARENT);
		SetRect(&rcRect, px, py, 620, 480);
		SelectObject(hdc, hfont_small);
		if (hfont == NULL) Msg("CreafontIndirect failed. haha.");
		SetTextColor(hdc, RGB(8, 14, 21));
		DrawText(hdc, thing, lstrlen(thing), &rcRect, DT_WORDBREAK);
		OffsetRect(&rcRect, -2, -2);
		DrawText(hdc, thing, lstrlen(thing), &rcRect, DT_WORDBREAK);

		OffsetRect(&rcRect, 1, 1);



		SetTextColor(hdc, RGB(255, 255, 0));
		DrawText(hdc, thing, lstrlen(thing), &rcRect, DT_WORDBREAK);

		dum =  GetTextFace(hdc, 100, shit) ;
		lpDDSBack->ReleaseDC(hdc);
	}


}


void Saytiny(char thing[2000], int px, int py, int r, int g, int b) {
	RECT rcRect;
	HDC         hdc;

	char shit[200];
	int dum;
	if (lpDDSBack->GetDC(&hdc) == DD_OK) {
		SetBkMode(hdc, TRANSPARENT);
		SetRect(&rcRect, px, py, 640, 480);

		SetTextColor(hdc, RGB(r, g, b));
		DrawText(hdc, thing, lstrlen(thing), &rcRect, DT_WORDBREAK);

		dum =  GetTextFace(hdc, 100, shit) ;
		lpDDSBack->ReleaseDC(hdc);
	}


}

//add hardness from a sprite

int getpic(int h) {
	if (spr[h].pseq == 0)
		return 0;
	if (spr[h].pseq > max_sequences) {
		Msg("Sequence %d?  But max is %d!", spr[h].pseq, max_sequences);
		return 0;
	}

	return seq[spr[h].pseq].frame[spr[h].pframe];
}


void add_hardness(int sprite, int num) {

	for (int  xx = spr[sprite].x + picInfo[getpic(sprite)].hardbox.left; xx < spr[sprite].x + picInfo[getpic(sprite)].hardbox.right; xx++) {
		for (int yy = spr[sprite].y + picInfo[getpic(sprite)].hardbox.top; yy < spr[sprite].y + picInfo[getpic(sprite)].hardbox.bottom; yy++) {
			if ((xx - 20 > 600) | (xx - 20 < 0) | (yy > 400) | (yy < 0)) {
			} else

				hm.x[xx - 20].y[yy] = num;
		}
	}
}




void setup_anim(int fr, int start, int delay) {
	//** start is sprite sequence #, don't know why it is called start

	for (int o = 1; o <= index[start].last; o++)

	{
		seq[fr].frame[o] = index[start].s + o;
		seq[fr].delay[o] = delay;
	}

	seq[fr].frame[index[start].last + 1] = 0;

}

byte get_hard(int h, int x1, int y1) {
	int value;

	//redink1 fix for screenlock bug
	if (screenlock) {
		if (x1 < 0 && x1 > -5) x1 = 0;
		else if (x1 > 599 && x1 < 605) x1 = 599;

		if (y1 < 0 && y1 > -5) y1 = 0;
		else if (y1 > 399 && x1 < 405) y1 = 399;
	}
	if (x1 < 0 || y1 < 0 || x1 > 599 || y1 > 399) return 0;

	value =  hm.x[x1].y[y1];


	return value;
}

byte get_hard_play(int h, int x1, int y1) {
	int value;
	x1 -= 20;

	//redink1 fix for screenlock bug
	if (screenlock) {
		if (x1 < 0 && x1 > -5) x1 = 0;
		else if (x1 > 599 && x1 < 605) x1 = 599;

		if (y1 < 0 && y1 > -5) y1 = 0;
		else if (y1 > 399 && x1 < 405) y1 = 399;
	}
	if (x1 < 0 || y1 < 0 || x1 > 599 || y1 > 399) return 0;

	value =  hm.x[x1].y[y1];

	if (value > 100) {

		if (pam.sprite[value - 100].prop != 0)

		{
			flub_mode = value;
			value = 0;
		}
	}

	return value;
}


byte get_hard_map(int h, int x1, int y1) {


	if ((x1 < 0) || (y1 < 0)) return 0;
	if ((x1 > 599)) return 0;
	if (y1 > 399) return 0;


	int til = (x1 / 50) + (((y1 / 50)) * 12);
	//til++;

	int offx = x1 - ((x1 / 50) * 50);


	int offy = y1 - ((y1 / 50) * 50);

	//Msg("tile %d ",til);

	return hmap.tile[ realhard(til)  ].x[offx].y[offy];
}



void fill_hardxy(RECT box) {
	//Msg("filling hard of %d %d %d %d", box.top, box.left, box.right, box.bottom);

	if (box.right > 599) box.right = 600; //redink1 screenlock bug
	if (box.top < 0) box.top = 0;
	if (box.bottom > 399) box.bottom = 400; //redink1 screenlock bug
	if (box.left < 0) box.left = 0;


	for (int x1 = box.left; x1 < box.right; x1++) {
		for (int y1 = box.top; y1 < box.bottom; y1++) {
			hm.x[x1].y[y1] = get_hard_map(0, x1, y1);

		}

	}

}



void add_exp(int num, int h, bool addEvenIfNotLastSpriteHit) {
	//redink1 fix - made work with all sprites when using add_exp DinkC command
	if (addEvenIfNotLastSpriteHit == false) {
		if (spr[h].last_hit != 1) return;
	}

	if (num > 0) {
		//add experience


		*pexper += num;


		int crap2 = add_sprite(spr[h].x, spr[h].y, 8, 0, 0);

		spr[crap2].y -= picInfo[seq[spr[h].pseq].frame[spr[h].pframe]].yoffset;
		spr[crap2].x -= picInfo[seq[spr[h].pseq].frame[spr[h].pframe]].xoffset;
		spr[crap2].y -= picInfo[seq[spr[h].pseq].frame[spr[h].pframe]].box.bottom / 3;
		spr[crap2].x += picInfo[seq[spr[h].pseq].frame[spr[h].pframe]].box.right / 5;
		spr[crap2].y -= 30;
		spr[crap2].speed = 1;
		spr[crap2].hard = 1;
		spr[crap2].brain_parm = 5000;
		spr[crap2].my = -1;
		spr[crap2].kill = 1000;
		spr[crap2].dir = 8;
		spr[crap2].damage = num;


		if (*pexper > 99999) *pexper = 99999;


	}

}

int realhard(int tile) {

	//  if (pam.t[tile].num > 3000) Msg("Hard is %d", pam.t[tile].num );
	if (pam.t[tile].althard > 0)
		return pam.t[tile].althard;
	else
		return hmap.index[pam.t[tile].num];

}

void fill_whole_hard() {
	for (int til = 0; til < 96; til++) {
		int offx = (til * 50 - ((til / 12) * 600));
		int offy = (til / 12) * 50;

		for (int x = 0; x < 50; x++) {
			for (int y = 0; y < 50; y++) {


				hm.x[offx + x].y[offy + y] = hmap.tile[  realhard(til)  ].x[x].y[y];


			}
		}
	}
}

void drawallhard() {
	RECT boxCrap;
	int ddrVal;
	DDBLTFX     ddBltFx;


	for (int x1 = 0; x1 < 600; x1++)
		for (int y1 = 0; y1 < 400; y1++) {
			if (hm.x[x1].y[y1] == 1) {
				ddBltFx.dwFillColor = 1;
				ddBltFx.dwSize = sizeof(ddBltFx);
				boxCrap.top = y1;
				boxCrap.bottom = y1 + 1;
				boxCrap.left = x1 + playl; //20 is to compensate for the border
				boxCrap.right = x1 + 1 + playl;
				ddrVal = lpDDSBack->Blt(&boxCrap, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddBltFx);
				if (ddrVal != DD_OK) Msg("There was an error!");
			}

			if (hm.x[x1].y[y1] == 2) {
				ddBltFx.dwFillColor = 128;
				ddBltFx.dwSize = sizeof(ddBltFx);
				boxCrap.top = y1;
				boxCrap.bottom = y1 + 1;
				boxCrap.left = x1 + playl; //20 is to compensate for the border
				boxCrap.right = x1 + 1 + playl;
				ddrVal = lpDDSBack->Blt(&boxCrap, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddBltFx);
				if (ddrVal != DD_OK) Msg("There was an error!");
			}


			if (hm.x[x1].y[y1] == 3) {
				ddBltFx.dwFillColor = 45;
				ddBltFx.dwSize = sizeof(ddBltFx);
				boxCrap.top = y1;
				boxCrap.bottom = y1 + 1;
				boxCrap.left = x1 + playl; //20 is to compensate for the border
				boxCrap.right = x1 + 1 + playl;
				ddrVal = lpDDSBack->Blt(&boxCrap, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddBltFx);
				if (ddrVal != DD_OK) Msg("There was an error!");
			}



			if (hm.x[x1].y[y1] > 100) {

				if (pam.sprite[(hm.x[x1].y[y1]) - 100].prop == 1) {
					//draw a little pixel
					ddBltFx.dwFillColor = 20;
					ddBltFx.dwSize = sizeof(ddBltFx);
					boxCrap.top = y1;
					boxCrap.bottom = y1 + 1;
					boxCrap.left = x1 + playl; //20 is to compensate for the border
					boxCrap.right = x1 + 1 + playl;
					ddrVal = lpDDSBack->Blt(&boxCrap, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddBltFx);
					if (ddrVal != DD_OK) Msg("There was an error!");


				} else {
					//draw a little pixel
					ddBltFx.dwFillColor = 23;
					ddBltFx.dwSize = sizeof(ddBltFx);
					boxCrap.top = y1;
					boxCrap.bottom = y1 + 1;
					boxCrap.left = x1 + playl; //20 is to compensate for the border
					boxCrap.right = x1 + 1 + playl;
					ddrVal = lpDDSBack->Blt(&boxCrap, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddBltFx);
					if (ddrVal != DD_OK) Msg("There was an error!");
				}
			}
		}
}

void fix_dead_sprites() {

	if (dinkedit) return;

	for (int i = 1; i < 100; i++) {
		if (play.spmap[*pmap].type[i] == 6) {

			if ((thisTickCount > (play.spmap[*pmap].last_time +  300000)) ||
			        (thisTickCount  + 400000 < play.spmap[*pmap].last_time +  300000))


			{
				//this sprite can come back online now
				play.spmap[*pmap].type[i] = 0;
			}
		}

		if (play.spmap[*pmap].type[i] == 7) {

			if (thisTickCount > (play.spmap[*pmap].last_time +  180000)) {
				//this sprite can come back online now
				play.spmap[*pmap].type[i] = 0;
			}
		}

		if (play.spmap[*pmap].type[i] == 8) {

			if (thisTickCount > (play.spmap[*pmap].last_time +  60000)) {
				//this sprite can come back online now
				play.spmap[*pmap].type[i] = 0;
			}
		}


	}

}

void attach() {
	for (int i = 1; i < max_vars; i++) {
		if (compare("&life", play.var[i].name)) {
			plife = &play.var[i].var;
		}
		if (compare("&vision", play.var[i].name)) {
			pvision = &play.var[i].var;
		}
		if (compare("&result", play.var[i].name)) {
			presult = &play.var[i].var;
		}
		if (compare("&speed", play.var[i].name)) {
			pspeed = &play.var[i].var;
		}
		if (compare("&timing", play.var[i].name)) {
			ptiming = &play.var[i].var;
		}

		if (compare("&lifemax", play.var[i].name)) {
			plifemax = &play.var[i].var;
		}

		if (compare("&exp", play.var[i].name))  pexper = &play.var[i].var;
		if (compare("&strength", play.var[i].name))  pstrength = &play.var[i].var;
		if (compare("&defense", play.var[i].name))  pdefense = &play.var[i].var;
		if (compare("&gold", play.var[i].name))  pgold = &play.var[i].var;
		if (compare("&magic", play.var[i].name))  pmagic = &play.var[i].var;
		if (compare("&level", play.var[i].name))  plevel = &play.var[i].var;
		if (compare("&player_map", play.var[i].name))  pmap = &play.var[i].var;
		if (compare("&cur_weapon", play.var[i].name))  pcur_weapon = &play.var[i].var;
		if (compare("&cur_magic", play.var[i].name))  pcur_magic = &play.var[i].var;
		if (compare("&last_text", play.var[i].name))  plast_text = &play.var[i].var;
		if (compare("&magic_level", play.var[i].name))  pmagic_level = &play.var[i].var;
		if (compare("&update_status", play.var[i].name))  pupdate_status = &play.var[i].var;
		if (compare("&missile_target", play.var[i].name))  pmissile_target = &play.var[i].var;
		if (compare("&enemy_sprite", play.var[i].name))  penemy_sprite = &play.var[i].var;
		if (compare("&magic_cost", play.var[i].name))  pmagic_cost = &play.var[i].var;
		if (compare("&missle_source", play.var[i].name))  pmissle_source = &play.var[i].var;
	}
}

void update_screen_time() {
	//Msg("Cur time is %d", play.spmap[*pmap].last_time);
	//Msg("Map is %d..", *pmap);
	play.spmap[*pmap].last_time = thisTickCount;
	//Msg("Time was saved as %d", play.spmap[*pmap].last_time);
}

bool load_game_small(int num, char *line, int *mytime) {
#ifdef TODO
	FILE           *fp;
	char crap[80];
	dink_sprintf(crap, "SAVE%d.DAT", num);


	fp = fopen(crap, "rb");
	if (!fp) {
		Msg("Couldn't quickload save game %d", num);
		return false;
	} else {

		fread(&short_play, sizeof(player_short_info), 1, fp);
		fclose(fp);
		*mytime = short_play.minutes;
		dink_strcpy(line, short_play.gameinfo);
		return true;
	}
#else
	error("TODO");
#endif
}

void blit_background() {
	RECT rcRect;

	SetRect(&rcRect, 0, 0, 640, 480);

	lpDDSBack->BltFast(0, 0, lpDDSTwo,
	                   &rcRect, DDBLTFAST_NOCOLORKEY);


}

void draw_wait() {

	if (seq[423].frame[8] != 0) {

		if (please_wait) {
			ddrval = lpDDSPrimary->BltFast(232, 0, picInfo[seq[423].frame[7]].k,
			                               &picInfo[seq[423].frame[7]].box, DDBLTFAST_SRCCOLORKEY);
			please_wait = false;
		} else {
			ddrval = lpDDSPrimary->BltFast(232, 0, picInfo[seq[423].frame[8]].k,
			                               &picInfo[seq[423].frame[7]].box, DDBLTFAST_SRCCOLORKEY);
			please_wait = true;

		}
	}

}


LPDIRECTDRAWSURFACE DDCreateSurface(uint32 width, uint32 height,
                                    bool sysmem, bool trans) {

	DDSURFACEDESC   ddsd;
	HRESULT     ddrVal;
	LPDIRECTDRAWSURFACE psurf;


	/*
	* fill in surface desc
	*/
	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;

	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	/*    if( sysmem || bUseSysMem )
	{
	ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
	}
	*/
	ddsd.dwHeight = height;
	ddsd.dwWidth = width;

	ddrVal = IDirectDraw_CreateSurface(lpDD, &ddsd, &psurf, NULL);

	/*
	* set the color key for this bitmap
	*/
	if (ddrVal == DD_OK) {

		/*  if( trans && !bTransDest )
		{
		ddck.dwColorSpaceLowValue = dwColorKey;
		ddck.dwColorSpaceHighValue = dwColorKey;
		IDirectDrawSurface_SetColorKey( psurf, DDCKEY_SRCBLT, &ddck);
		}
		*/

	} else {
		Msg("CreateSurface FAILED, rc = %ld", (uint32)LOWORD(ddrVal));
		psurf = NULL;
	}

	return psurf;

} /* DDCreateSurface */

//redink1 and Invertigo fix for windowed/true color mode
void load_sprite_pak(char org[100], int nummy, int speed, int xoffset, int yoffset,
                     RECT hardbox, bool notanim, bool black, bool leftalign, bool samedir) {
	int work;
	Common::File f;
	DDSURFACEDESC ddsd;
	DDCOLORKEY ddck;

#if 0
	HFASTFILE pfile;
	BITMAPFILEHEADER UNALIGNED *pbf;
	BITMAPINFOHEADER UNALIGNED *pbi;
	HBITMAP             hbm;
	BITMAP              bm;
	bool trans = false;
#endif

	int x, y, dib_pitch;
	const byte *src;
	byte *dst;
	byte *ddst;
	char fname[20];

	int sprite = 71;
	bool reload = false;

	PALETTEENTRY holdpal[256];
	char crap[200];// , hold[5];
	int save_cur = cur_sprite;

	if (index[nummy].last != 0) {
		//  Msg("Saving sprite %d", save_cur);
		cur_sprite = index[nummy].s + 1;
		//Msg("Temp cur_sprite is %d", cur_sprite);
		reload = true;
	}


	index[nummy].s = cur_sprite - 1;

	if (no_running_main)
		draw_wait();

	char crap2[200];
	dink_strcpy(crap2, org);
	while (crap2[strlen(crap2) - 1] != '\\') {
		crap2[strlen(crap2) - 1] = 0;
	}
	crap2[strlen(crap2) - 1] = 0;

	int num = strlen(org) - strlen(crap2) - 1;
	dink_strcpy(fname, &org[strlen(org) - num]);
	if (samedir)
		dink_sprintf(crap, "%s\\dir.ff", crap2);
	else
		dink_sprintf(crap, "..\\dink\\%s\\dir.ff", crap2);


	if (!FastFileInit(crap, 5)) {
		Msg("Could not load dir.ff art file %s", crap);

		cur_sprite = save_cur;
		return;
	}

	/* if (!windowed) {
	lpDDPal->GetEntries(0,0,256,holdpal);
	lpDDPal->SetEntries(0,0,256,real_pal);
	}
	*/

	for (int oo = 1; oo <= 51; oo++) {
		Image::BitmapDecoder decoder;

		// Load sprite
//		char dumb[100];
		sprite = cur_sprite;
		//if (reload) Msg("Ok, programming sprite %d", sprite);
		if (oo < 10) dink_strcpy(crap2, "0");
		else dink_strcpy(crap2, "");
		dink_sprintf(crap, "%s%s%d.bmp", fname, crap2, oo);

		if (!f.open(crap) || !decoder.loadStream(f)) {
			if (oo == 1)
				Msg("Sprite_load_pak error:  Couldn't load %s.", crap);

			index[nummy].last = (oo - 1);
			//     initFail(hWndMain, crap);
			setup_anim(nummy, nummy, speed);
			//     if (!windowed)  lpDDPal->SetEntries(0,0,256,holdpal);

			//if (reload) Msg("Ok, tacking %d back on.", save_cur);
			cur_sprite = save_cur;
			return;

		} else {
			// Got image
			const Graphics::Surface *surf = decoder.getSurface();
			const byte *palzor = decoder.getPalette();
			const int paletteCount = decoder.getPaletteColorCount();

#define _RGBXBIT(r, g, b) ( (r >> (8 - wRBits) << wRPos) | (g >> (8 - wGBits) << wGPos) | (b >> (8 - wBBits) << wBPos) )
			PALETTEENTRY ape[256];
			uint32 dwBlack = 0;
			uint32 dwNearBlack = 0;
			uint32 dwWhite = 0;
			uint32 dwNearWhite = 0;

			if (truecolor) {
				if (dinkpal) {
					memcpy(ape, real_pal, sizeof(PALETTEENTRY) * 256);
				} else {
					for (int pcount = 0; pcount < 256; pcount++) {
						ape[pcount].peRed = palzor[pcount * 4 + 2];
						ape[pcount].peGreen = palzor[pcount * 4 + 1];
						ape[pcount].peBlue = palzor[pcount * 4];
					}
				}
				dwBlack     = _RGBXBIT(ape[255].peRed, ape[255].peGreen, ape[255].peBlue);
				dwNearBlack = _RGBXBIT(ape[249].peRed, ape[255].peGreen, ape[255].peBlue);
				dwWhite     = _RGBXBIT(ape[0].peRed,   ape[0].peGreen,   ape[0].peBlue);
				dwNearWhite = _RGBXBIT(ape[30].peRed,  ape[30].peGreen,  ape[30].peBlue);
			}

			//Msg("Pic's size is now %d.",sizeof(pic));

			//redink1 THIS DOESN'T DO ANYTHING!!!! ARGHAGHARHA!
			/*bm.bmWidth = surf->w;
			bm.bmHeight = surf->h;
			bm.bmWidthBytes = 32;
			bm.bmPlanes = pbi->biPlanes;
			bm.bmBitsPixel = pbi->biBitCount;
			bm.bmBits = pic;*/

			//
			// create a DirectDrawSurface for this bitmap
			//
			ZeroMemory(&ddsd, sizeof(ddsd));
			ddsd.dwSize = sizeof(ddsd);
			ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
			ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
			ddsd.dwWidth = surf->w;
			ddsd.dwHeight = surf->h;

			if (picInfo[sprite].k != NULL)
				picInfo[sprite].k->Release();

			if (lpDD->CreateSurface(&ddsd, &picInfo[sprite].k, NULL) != DD_OK) {
				Msg("Failed to create pdd surface description");
			} else {
				ddsd.dwSize = sizeof(ddsd);
				ddrval = IDirectDrawSurface_Lock(
					picInfo[sprite].k, NULL, &ddsd, DDLOCK_WAIT, NULL);

				if (ddrval == DD_OK) {
					dib_pitch = (surf->w + 3) & ~3;
					src = (const byte *)surf->getBasePtr(0, surf->h - 1);
					dst = (BYTE *)ddsd.lpSurface;
					ddst = (byte *)ddsd.lpSurface;
					int bytesPerPixel = surf->format.bytesPerPixel;

					uint32 dwPixel;
					if (leftalign) {
						//Msg("left aligning..");
						for (y = 0; y < (int)surf->h; y++) {
							for (x = 0; x < (int)surf->w; x++) {
								if (truecolor) {
									dwPixel = _RGBXBIT(ape[src[x]].peRed, ape[src[x]].peGreen, ape[src[x]].peBlue);
									if (dwPixel == dwBlack) {
										// Msg("Found a 255...");
										dwPixel = dwNearBlack;
									} else if (dwPixel == dwWhite) {
										dwPixel = dwNearWhite;
									}

									// Make sure to or it with the rest of the uint32 present, just in case it overflows the buffer
									*((uint32 *)ddst) = dwPixel | *((uint32 *)ddst) & ~dwWhite;

									ddst += bytesPerPixel;
								} else {
									dst[x] = src[x];
									if (dst[x] == 0) {
										// Msg("Found a 255...");
										dst[x] = 30;
									} else if (dst[x] == 255) {
										dst[x] = 249;
									}
								}

							}

							//redink1 switched to 'better' version
							ddst += ddsd.lPitch - surf->w * bytesPerPixel; //ddst += ddsd.lPitch / 4;
							dst += ddsd.lPitch;
							src -= dib_pitch;
						}
					} else
						if (black) {
							for (y = 0; y < (int)surf->h; y++) {
								for (x = 0; x < (int)surf->w; x++) {
									if (truecolor) {
										dwPixel = _RGBXBIT(ape[src[x]].peRed, ape[src[x]].peGreen, ape[src[x]].peBlue);
										if (dwPixel == dwWhite) {
											// Msg("Found a 255...");
											dwPixel = dwNearWhite;
										} else if (dwPixel == dwBlack) {
											dwPixel = dwWhite;
										}
										*((uint32 *)ddst) = dwPixel | *((uint32 *)ddst) & ~dwWhite;;

										ddst += bytesPerPixel;
										/*ddst[x] = _RGB24BIT(ape[src[x]].peRed, ape[src[x]].peGreen, ape[src[x]].peBlue);

										if (ddst[x] ==  _RGB24BIT(ape[0].peRed, ape[0].peGreen, ape[0].peBlue))
										{
										// Msg("Found a 255...");
										ddst[x] = _RGB24BIT(ape[30].peRed, ape[30].peGreen, ape[30].peBlue);
										}
										if (!dinkpal)
										{
										if (ddst[x] ==  _RGB24BIT(ape[255].peRed, ape[255].peGreen, ape[255].peBlue))
										{
										ddst[x] = _RGB24BIT(ape[0].peRed, ape[0].peGreen, ape[0].peBlue);
										}
										}*/
									} else {

										dst[x] = src[x];

										if (dst[x] == 0) {
											dst[x] = 30;
										}
									}

								}



								//redink1 switched to 'better' version
								ddst += ddsd.lPitch - surf->w * bytesPerPixel;
								dst += ddsd.lPitch;
								src -= dib_pitch;
							}

						} else {

							//doing white
							for (y = 0; y < (int)surf->h; y++) {
								for (x = 0; x < (int)surf->w; x++) {
									if (truecolor) {
										dwPixel = _RGBXBIT(ape[src[x]].peRed, ape[src[x]].peGreen, ape[src[x]].peBlue);
										if (dwPixel == dwBlack) {
											// Msg("Found a 255...");
											dwPixel = dwNearBlack;
										} else if (dwPixel == dwWhite) {
											dwPixel = dwBlack;
										}
										*((uint32 *)ddst) = dwPixel | *((uint32 *)ddst) & ~dwWhite;;

										ddst += bytesPerPixel;
										/*ddst[x] = _RGB24BIT(ape[src[x]].peRed, ape[src[x]].peGreen, ape[src[x]].peBlue);

										if (ddst[x] ==  _RGB24BIT(ape[255].peRed, ape[255].peGreen, ape[255].peBlue))
										{
										ddst[x] = _RGB24BIT(ape[249].peRed, ape[249].peGreen, ape[249].peBlue);
										}
										if (!dinkpal)
										{
										if (ddst[x] ==  _RGB24BIT(ape[0].peRed, ape[0].peGreen, ape[0].peBlue))
										{
										ddst[x] = _RGB24BIT(ape[255].peRed, ape[255].peGreen, ape[255].peBlue);
										}
										}*/
									} else {
										dst[x] = src[x];


										if (dst[x] == 255) {
											// Msg("Found a 255...");
											dst[x] = 249;
										}
									}
								}

								//redink1 switched to 'better' version
								ddst += ddsd.lPitch - surf->w * bytesPerPixel; //ddst += ddsd.lPitch / 4;
								dst += ddsd.lPitch;
								src -= dib_pitch;
							}

						}



					IDirectDrawSurface_Unlock(picInfo[sprite].k, NULL);
				} else {
					Msg("Lock failed err=%d", ddrval);
					//return;
				}

				if (sprite > 0) {
					picInfo[sprite].box.top = 0;
					picInfo[sprite].box.left = 0;
					picInfo[sprite].box.right = ddsd.dwWidth;
					picInfo[sprite].box.bottom = ddsd.dwHeight;



					if ((oo > 1) & (notanim)) {

						picInfo[cur_sprite].yoffset = picInfo[index[nummy].s + 1].yoffset;
					} else {
						if (yoffset > 0)
							picInfo[cur_sprite].yoffset = yoffset;
						else {


							picInfo[cur_sprite].yoffset = (picInfo[cur_sprite].box.bottom -
							                               (picInfo[cur_sprite].box.bottom / 4)) - (picInfo[cur_sprite].box.bottom / 30);

						}
					}

					if ((oo > 1) & (notanim)) {

						picInfo[cur_sprite].xoffset =  picInfo[index[nummy].s + 1].xoffset;
					} else {

						if (xoffset > 0)
							picInfo[cur_sprite].xoffset = xoffset;
						else

						{


							picInfo[cur_sprite].xoffset = (picInfo[cur_sprite].box.right -
							                               (picInfo[cur_sprite].box.right / 2)) + (picInfo[cur_sprite].box.right / 6);


						}
					}
					//ok, setup main offsets, lets build the hard block

					if (hardbox.right > 0) {
						//forced setting
						picInfo[cur_sprite].hardbox.left = hardbox.left;
						picInfo[cur_sprite].hardbox.right = hardbox.right;
					} else {
						//guess setting
						work = picInfo[cur_sprite].box.right / 4;
						picInfo[cur_sprite].hardbox.left -= work;
						picInfo[cur_sprite].hardbox.right += work;

					}



					if (hardbox.bottom > 0) {
						picInfo[cur_sprite].hardbox.top = hardbox.top;
						picInfo[cur_sprite].hardbox.bottom = hardbox.bottom;

					} else {

						work = picInfo[cur_sprite].box.bottom / 10;
						picInfo[cur_sprite].hardbox.top -= work;
						picInfo[cur_sprite].hardbox.bottom += work;

					}

					if (black) {
						ddck.dwColorSpaceLowValue  = DDColorMatch(picInfo[cur_sprite].k, RGB(255, 255, 255));

						ddck.dwColorSpaceHighValue = ddck.dwColorSpaceLowValue;
						picInfo[cur_sprite].k->SetColorKey(DDCKEY_SRCBLT, &ddck);

					} else {
						ddck.dwColorSpaceLowValue  = DDColorMatch(picInfo[cur_sprite].k, RGB(0, 0, 0));
						ddck.dwColorSpaceHighValue = ddck.dwColorSpaceLowValue;
						picInfo[cur_sprite].k->SetColorKey(DDCKEY_SRCBLT, &ddck);

					}

					cur_sprite++;
					if (!reload)
						save_cur++;

					//FastFileClose(pfile);
				}
			}
		}
	}

	//  FastFileFini();
}



void load_sprites(char org[100], int nummy, int speed, int xoffset, int yoffset,
                  RECT hardbox, bool notanim, bool black, bool leftalign) {
	int work;
	PALETTEENTRY    holdpal[256];
	char crap[200];

	if (no_running_main)
		draw_wait();

	char crap2[200];
	dink_strcpy(crap2, org);
	while (crap2[strlen(crap2) - 1] != '\\') {
		crap2[strlen(crap2) - 1] = 0;
	}
	crap2[strlen(crap2) - 1] = 0;

	dink_sprintf(crap, "%s\\dir.ff", crap2);
	//Msg("Checking for %s..", crap);
	if (File::exists(crap)) {
		load_sprite_pak(org, nummy, speed, xoffset, yoffset, hardbox, notanim, black, leftalign, true);
		return;
	}

	dink_sprintf(crap, "%s01.bmp", org);
	if (!File::exists(crap)) {
		dink_sprintf(crap, "..\\dink\\%s\\dir.ff", crap2);
		//Msg("Checking for %s..", crap);
		if (File::exists(crap)) {
			load_sprite_pak(org, nummy, speed, xoffset, yoffset, hardbox, notanim, black, leftalign, false);
			return;
		}

		//  Msg("Dir bad for sprite, changing");
		dink_sprintf(crap, "..\\dink\\%s", org);
		dink_strcpy(org, crap);
	}

	// redink1 added to fix bug where loading sequences over others wouldn't work quite right.
	int save_cur = cur_sprite;
	bool reload = false;
	if (index[nummy].last != 0) {
		//  Msg("Saving sprite %d", save_cur);
		cur_sprite = index[nummy].s + 1;
		//Msg("Temp cur_sprite is %d", cur_sprite);
		reload = true;
	}

	index[nummy].s = cur_sprite - 1;

	if (!windowed) {
		lpDDPal->GetEntries(0, 0, 256, holdpal);
		lpDDPal->SetEntries(0, 0, 256, real_pal);
	}

	for (int oo = 1; oo <= 1000; oo++) {
		dink_sprintf(crap, "%s%.2d.BMP", org, oo);
		picInfo[cur_sprite].k = DDSethLoad(lpDD, crap, 0, 0, cur_sprite);

		if (picInfo[cur_sprite].k != NULL) {
			if ((oo > 1) & (notanim)) {
				picInfo[cur_sprite].yoffset = picInfo[index[nummy].s + 1].yoffset;
			} else {
				if (yoffset > 0)
					picInfo[cur_sprite].yoffset = yoffset;
				else {
					picInfo[cur_sprite].yoffset = (picInfo[cur_sprite].box.bottom -
						(picInfo[cur_sprite].box.bottom / 4)) - (picInfo[cur_sprite].box.bottom / 30);
				}
			}

			if ((oo > 1) & (notanim)) {
				picInfo[cur_sprite].xoffset =  picInfo[index[nummy].s + 1].xoffset;
			} else if (xoffset > 0) {
				picInfo[cur_sprite].xoffset = xoffset;
			} else {
				picInfo[cur_sprite].xoffset = (picInfo[cur_sprite].box.right -
					(picInfo[cur_sprite].box.right / 2)) + (picInfo[cur_sprite].box.right / 6);
			}

			// Ok, setup main offsets, lets build the hard block
			if (hardbox.right > 0) {
				// Forced setting
				picInfo[cur_sprite].hardbox.left = hardbox.left;
				picInfo[cur_sprite].hardbox.right = hardbox.right;
			} else {
				// Guess setting
				work = picInfo[cur_sprite].box.right / 4;
				picInfo[cur_sprite].hardbox.left -= work;
				picInfo[cur_sprite].hardbox.right += work;
			}

			if (hardbox.bottom > 0) {
				picInfo[cur_sprite].hardbox.top = hardbox.top;
				picInfo[cur_sprite].hardbox.bottom = hardbox.bottom;

			} else {
				work = picInfo[cur_sprite].box.bottom / 10;
				picInfo[cur_sprite].hardbox.top -= work;
				picInfo[cur_sprite].hardbox.bottom += work;
			}
		}

		if (picInfo[cur_sprite].k == NULL) {
			if (oo < 2) {
				Msg("load_sprites:  Anim %s not found.", org);
			}

			index[nummy].last = (oo - 1);
			//     initFail(hWndMain, crap);
			setup_anim(nummy, nummy, speed);
			if (!windowed)  lpDDPal->SetEntries(0, 0, 256, holdpal);
			cur_sprite = save_cur;
			return;
		}

		if (black)
			DDSetColorKey(picInfo[cur_sprite].k, RGB(0, 0, 0));
		else
			DDSetColorKey(picInfo[cur_sprite].k, RGB(255, 255, 255));

		cur_sprite++;
		if (!reload)
			save_cur++;
	}

	cur_sprite = save_cur;
}

void figure_out(char line[255], int load_seq) {
	char ev[15][100];
	RECT hardbox;
	ZeroMemory(&ev, sizeof(ev));
	int myseq = 0, myframe = 0;
	int special = 0;
	int special2 = 0;
	for (int i = 1; i <= 14; i++) {
		seperate_string(line, i, ' ', ev[i]);
		//   Msg("Word %d is \"%s\"",i,ev[i]);
	}

	if ((compare(ev[1], "LOAD_SEQUENCE_NOW")) | (compare(ev[1], "LOAD_SEQUENCE")))
		//  if (     (load_seq == -1) | (load_seq == atol(ev[3]))  )
	{
		//           name   seq    speed       offsetx     offsety       hardx      hardy

		ZeroMemory(&hardbox, sizeof(RECT));
		seq[atol(ev[3])].active = true;
		dink_strcpy(seq[atol(ev[3])].data, line);
		if (compare(ev[4], "BLACK")) {
			load_sprites(ev[2], atol(ev[3]), atol(ev[4]), atol(ev[5]), atol(ev[6]), hardbox, true, true, false);
		} else if (compare(ev[4], "LEFTALIGN")) {
			load_sprites(ev[2], atol(ev[3]), atol(ev[4]), atol(ev[5]), atol(ev[6]), hardbox, false, false, true);
		} else

			if (compare(ev[4], "NOTANIM")) {

				//not an animation!
				load_sprites(ev[2], atol(ev[3]), atol(ev[4]), atol(ev[5]), atol(ev[6]), hardbox, false, false, false); //Crap
			} else {
				//yes, an animation!
				hardbox.left = atol(ev[7]);
				hardbox.top = atol(ev[8]);
				hardbox.right = atol(ev[9]);
				hardbox.bottom = atol(ev[10]);

				load_sprites(ev[2], atol(ev[3]), atol(ev[4]), atol(ev[5]), atol(ev[6]), hardbox, true, false, false); //Crap
			}


		program_idata();
		return;
	}
	if (compare(ev[1], "SET_SPRITE_INFO")) {
		//           name   seq    speed       offsetx     offsety       hardx      hardy


		//if (picInfo[seq[myseq].frame[myframe]].frame = 0) Msg("Changing sprite that doesn't exist...");

		myseq = atol(ev[2]);
		myframe = atol(ev[3]);
		picInfo[seq[myseq].frame[myframe]].xoffset = atol(ev[4]);
		picInfo[seq[myseq].frame[myframe]].yoffset = atol(ev[5]);
		picInfo[seq[myseq].frame[myframe]].hardbox.left = atol(ev[6]);
		picInfo[seq[myseq].frame[myframe]].hardbox.top = atol(ev[7]);
		picInfo[seq[myseq].frame[myframe]].hardbox.right = atol(ev[8]);
		picInfo[seq[myseq].frame[myframe]].hardbox.bottom = atol(ev[9]);
	}

	if (compare(ev[1], "SET_FRAME_SPECIAL")) {
		//           name   seq    speed       offsetx     offsety       hardx      hardy


		//if (picInfo[seq[myseq].frame[myframe]].frame = 0) Msg("Changing sprite that doesn't exist...");

		myseq = atol(ev[2]);
		myframe = atol(ev[3]);
		special = atol(ev[4]);

		seq[myseq].special[myframe] = special;
		Msg("Set special.  %d %d %d", myseq, myframe, special);
	}

	if (compare(ev[1], "SET_FRAME_DELAY")) {
		//           name   seq    speed       offsetx     offsety       hardx      hardy


		//if (picInfo[seq[myseq].frame[myframe]].frame = 0) Msg("Changing sprite that doesn't exist...");

		myseq = atol(ev[2]);
		myframe = atol(ev[3]);
		special = atol(ev[4]);

		seq[myseq].delay[myframe] = special;
		Msg("Set delay.  %d %d %d", myseq, myframe, special);
	}

	if (compare(ev[1], "STARTING_DINK_X")) {
		myseq = atol(ev[2]);
		play.x = myseq;
	}




	if (compare(ev[1], "STARTING_DINK_Y")) {
		myseq = atol(ev[2]);
		play.y = myseq;
	}



	if (compare(ev[1], "SET_FRAME_FRAME")) {
		//           name   seq    speed       offsetx     offsety       hardx      hardy


		//if (picInfo[seq[myseq].frame[myframe]].frame = 0) Msg("Changing sprite that doesn't exist...");

		myseq = atol(ev[2]);
		myframe = atol(ev[3]);
		special = atol(ev[4]);
		special2 = atol(ev[5]);

		if (special == -1)
			seq[myseq].frame[myframe] = special;
		else
			seq[myseq].frame[myframe] = seq[special].frame[special2];
		Msg("Set frame.  %d %d %d", myseq, myframe, special);
	}

}


void program_idata() {
	for (int i = 1; i < max_idata; i++) {
		if (id[i].type == 0)
			return;

		if (id[i].type == 1) {
			picInfo[seq[id[i].seq].frame[id[i].frame]].xoffset = id[i].xoffset;
			picInfo[seq[id[i].seq].frame[id[i].frame]].yoffset = id[i].yoffset;
			CopyRect(&picInfo[seq[id[i].seq].frame[id[i].frame]].hardbox, &id[i].hardbox);
		}

		if (id[i].type == 2) {
			// Set special
			seq[id[i].seq].special[id[i].frame] = id[i].xoffset;
		}
		if (id[i].type == 3) {
			// Set delay
			seq[id[i].seq].delay[id[i].frame] = id[i].xoffset;
		}

		if (id[i].type == 4) {
			if (id[i].xoffset == -1)
				seq[id[i].seq].frame[id[i].frame] = id[i].xoffset;
			else
				seq[id[i].seq].frame[id[i].frame] = seq[id[i].xoffset].frame[id[i].yoffset];
		}
	}
}

void make_idata(int type, int myseq, int myframe, int xoffset, int yoffset, RECT crect) {
	for (int i = 1; i < max_idata; i++) {
		if (id[i].type == 0) {
			// Found empty one
			id[i].type = type;
			id[i].seq = myseq;
			id[i].frame = myframe;
			id[i].xoffset = xoffset;
			id[i].yoffset = yoffset;
			CopyRect(&id[i].hardbox, &crect);
			return;
		}
	}

	Msg("Out of idata spots (max is %d), no more sprite corrections can be allowed.", max_idata);
}

void pre_figure_out(char line[255]) {
	char ev[15][100];
	RECT hardbox;
	ZeroMemory(&ev, sizeof(ev));
	int myseq = 0, myframe = 0;
	int special = 0;
	int special2 = 0;

	for (int i = 1; i <= 14; i++) {
		seperate_string(line, i, ' ', ev[i]);
	}

	if (compare(ev[1], "playmidi")) {
		if (!dinkedit)
			PlayMidi(ev[2]);
	}

	if (compare(ev[1], "LOAD_SEQUENCE_NOW")) {
		// name   seq    speed       offsetx     offsety       hardx      hardy
		ZeroMemory(&hardbox, sizeof(RECT));
		seq[atol(ev[3])].active = true;
		dink_strcpy(seq[atol(ev[3])].data, line);
		if (compare(ev[4], "BLACK")) {
			load_sprites(ev[2], atol(ev[3]), atol(ev[4]), atol(ev[5]), atol(ev[6]), hardbox, true, true, false);
		} else if (compare(ev[4], "LEFTALIGN")) {
			load_sprites(ev[2], atol(ev[3]), atol(ev[4]), atol(ev[5]), atol(ev[6]), hardbox, false, false, true);
		} else if (compare(ev[4], "NOTANIM")) {
			//not an animation!
			load_sprites(ev[2], atol(ev[3]), atol(ev[4]), atol(ev[5]), atol(ev[6]), hardbox, false, false, false); //Crap
		} else {
			//yes, an animation!
			hardbox.left = atol(ev[7]);
			hardbox.top = atol(ev[8]);
			hardbox.right = atol(ev[9]);
			hardbox.bottom = atol(ev[10]);

			load_sprites(ev[2], atol(ev[3]), atol(ev[4]), atol(ev[5]), atol(ev[6]), hardbox, true, false, false); //Crap
		}

		myseq = atol(ev[2]);
		myframe = atol(ev[3]);
		SetRect(&hardbox, atol(ev[6]), atol(ev[7]), atol(ev[8]), atol(ev[9]));
		make_idata(1, myseq, myframe, atol(ev[4]), atol(ev[5]), hardbox);
		return;
	}

	if (compare(ev[1], "LOAD_SEQUENCE")) {
		// name   seq    speed       offsetx     offsety       hardx      hardy
		dink_strcpy(seq[atol(ev[3])].data, line);
		seq[atol(ev[3])].active = true;
		return;
	}

	if (compare(ev[1], "SET_SPRITE_INFO")) {
		// name   seq    speed       offsetx     offsety       hardx      hardy
		myseq = atol(ev[2]);
		myframe = atol(ev[3]);
		SetRect(&hardbox, atol(ev[6]), atol(ev[7]), atol(ev[8]), atol(ev[9]));
		make_idata(1, myseq, myframe, atol(ev[4]), atol(ev[5]), hardbox);
		return;
	}

	if (compare(ev[1], "SET_FRAME_SPECIAL")) {
		myseq = atol(ev[2]);
		myframe = atol(ev[3]);
		special = atol(ev[4]);
		make_idata(2, myseq, myframe, atol(ev[4]), 0, hardbox);
		return;
	}

	if (compare(ev[1], "SET_FRAME_DELAY")) {
		myseq = atol(ev[2]);
		myframe = atol(ev[3]);
		special = atol(ev[4]);
		make_idata(3, myseq, myframe, atol(ev[4]), 0, hardbox);
		return;
	}

	if (compare(ev[1], "STARTING_DINK_X")) {
		myseq = atol(ev[2]);
		play.x = myseq;
	}

	if (compare(ev[1], "STARTING_DINK_Y")) {
		myseq = atol(ev[2]);
		play.y = myseq;
	}

	if (compare(ev[1], "SET_FRAME_FRAME")) {
		// name   seq    speed       offsetx     offsety       hardx      hardy
		myseq = atol(ev[2]);
		myframe = atol(ev[3]);
		special = atol(ev[4]);
		special2 = atol(ev[5]);

		make_idata(4, myseq, myframe, atol(ev[4]), atol(ev[5]), hardbox);
	}
}

int draw_num(int mseq, char nums[50], int mx, int my) {
	int length = 0;
	HRESULT ddrVal;
	int rnum = 0;

	for (int i = 0; i < (int)strlen(nums); i++) {

		if (nums[i] == '0') rnum = 10;
		else if (nums[i] == '1') rnum = 1;
		else if (nums[i] == '2') rnum = 2;
		else if (nums[i] == '3') rnum = 3;
		else if (nums[i] == '4') rnum = 4;
		else if (nums[i] == '5') rnum = 5;
		else if (nums[i] == '6') rnum = 6;
		else if (nums[i] == '7') rnum = 7;
		else if (nums[i] == '8') rnum = 8;
		else if (nums[i] == '9') rnum = 9;
		else if (nums[i] == '/') rnum = 11;
again:
		if ((rnum != 11) && (!(mseq == 442)))
			ddrVal = lpDDSTwo->BltFast(mx + length, my, picInfo[seq[mseq].frame[rnum]].k,
			                           &picInfo[seq[mseq].frame[rnum]].box, DDBLTFAST_NOCOLORKEY);

		else
			ddrVal = lpDDSTwo->BltFast(mx + length, my, picInfo[seq[mseq].frame[rnum]].k,
			                           &picInfo[seq[mseq].frame[rnum]].box, DDBLTFAST_SRCCOLORKEY);


		if (ddrVal != DD_OK) {

			if (ddrVal == DDERR_WASSTILLDRAWING) goto again;

			//dderror(ddrVal);

		} else {

			length += picInfo[seq[mseq].frame[rnum]].box.right;
		}
	}
	return length;
}

int next_raise() {
	int crap = *plevel;
	int num = ((100 * crap) * crap);

	if (num > 99999) num = 99999;
	return num;

}


void draw_exp() {
	char buffer[30];
	char nums[30];
	//char buf[30];
	char final[30];

	//Msg("Drawing exp.. which is %d and %d",fexp, *pexp);
	dink_strcpy(final, "");
	dink_strcpy(nums, ltoa(fexp, buffer, 10));
	if (strlen(nums) < 5)
		for (int i = 1; i < (6 - (int)strlen(nums)); i++)
			dink_strcat(final, "0");
	dink_strcat(final, nums);
	dink_strcat(final, "/");

	dink_strcpy(nums, ltoa(fraise, buffer, 10));
	if (strlen(nums) < 5)
		for (int i = 1; i < (6 - (int)strlen(nums)); i++)
			dink_strcat(final, "0");
	dink_strcat(final, nums);
	draw_num(181, final, 404, 459);

}


void draw_strength() {
	char final[30];
	char buffer[30];
	char nums[30];
	//Msg("Drawing exp.. which is %d and %d",fexp, *pexp);
	dink_strcpy(final, "");

	dink_strcpy(nums, ltoa(fstrength, buffer, 10));
	if (strlen(nums) < 3)
		for (int i = 1; i < (4 - (int)strlen(nums)); i++)
			dink_strcat(final, "0");
	dink_strcat(final, nums);
	//Msg("Drawing %s..",final);
	draw_num(182, final, 81, 415);
}


void draw_defense() {
	char final[30];
	char buffer[30];
	char nums[30];
	//Msg("Drawing exp.. which is %d and %d",fexp, *pexp);
	dink_strcpy(final, "");
	dink_strcpy(nums, ltoa(fdefense, buffer, 10));
	if (strlen(nums) < 3)
		for (int i = 1; i < (4 - (int)strlen(nums)); i++)
			dink_strcat(final, "0");
	dink_strcat(final, nums);
	draw_num(183, final, 81, 437);
}


void draw_magic() {
	char final[30];
	char buffer[30];
	char nums[30];
	//Msg("Drawing exp.. which is %d and %d",fexp, *pexp);
	dink_strcpy(final, "");
	dink_strcpy(nums, ltoa(fmagic, buffer, 10));
	if (strlen(nums) < 3)
		for (int i = 1; i < (4 - (int)strlen(nums)); i++)
			dink_strcat(final, "0");
	dink_strcat(final, nums);
	draw_num(184, final, 81, 459);
}


void draw_level() {
	char final[30];
	char buffer[30];
	//char nums[30];
	//*plevel = 15;
	//Msg("Drawing level.. which is %d ",*plevel);
	dink_strcpy(final, ltoa(*plevel, buffer, 10));

	if (strlen(final) == 1)

		draw_num(442, final, 528, 456);
	else
		draw_num(442, final, 523, 456);

}


void draw_gold() {
	char final[30];
	char buffer[30];
	char nums[30];
	//Msg("Drawing exp.. which is %d and %d",fexp, *pexp);
	dink_strcpy(final, "");
	dink_strcpy(nums, ltoa(fgold, buffer, 10));
	if (strlen(nums) < 5)
		for (int i = 1; i < (6 - (int)strlen(nums)); i++)
			dink_strcat(final, "0");
	dink_strcat(final, nums);
	draw_num(185, final, 298, 457);
}


void draw_bar(int life, int seqman) {

	int cur = 0;
	int curx = 284;
	int cury = 412;
	int rnum = 3;
	int curx_start = curx;

	RECT box;
	while (1) {


		cur++;
		if (cur > life) {
			cur--;
			int rem = (cur) - (cur / 10) * 10;
			if (rem != 0) {

				CopyRect(&box, &picInfo[seq[seqman].frame[rnum]].box);
				//Msg("Drawing part bar . cur is %d", rem);
				box.right = (box.right * ((rem) * 10) / 100);
				//woah, there is part of a bar remaining.  Lets do it.
again:
				ddrval = lpDDSTwo->BltFast(curx, cury, picInfo[seq[seqman].frame[rnum]].k,
				                           &box, DDBLTFAST_NOCOLORKEY);

				if (ddrval == DDERR_WASSTILLDRAWING) goto again;


			}

			//are we done?
			return;
		}

		rnum = 2;
		if (cur < 11) rnum = 1;
		if (cur == *plifemax) rnum = 3;

		if ((cur / 10) * 10 == cur) {

again2:
			ddrval = lpDDSTwo->BltFast(curx, cury, picInfo[seq[seqman].frame[rnum]].k,
			                           &picInfo[seq[seqman].frame[rnum]].box, DDBLTFAST_NOCOLORKEY);

			if (ddrval == DDERR_WASSTILLDRAWING) goto again2;

			//if (ddrval != DD_OK) dderror(ddrval);
			curx += picInfo[seq[seqman].frame[rnum]].box.right;
			if (cur == 110) {
				cury += picInfo[seq[seqman].frame[rnum]].box.bottom + 5;
				curx = curx_start;

			}

			if (cur == 220) return;
		}
	}
}


void draw_health() {
	flifemax = *plifemax;
	draw_bar(flifemax, 190);
	flife = *plife;
	draw_bar(flife, 451);
}

void draw_icons() {

	if (*pcur_weapon != 0) if (play.item[*pcur_weapon].active) {
			//disarm old weapon
			//play.item[*pcur_weapon].seq,
again:

			check_seq_status(play.item[*pcur_weapon].seq);


			ddrval = lpDDSTwo->BltFast(557, 413, picInfo[seq[play.item[*pcur_weapon].seq].frame[play.item[*pcur_weapon].frame]].k,
			                           &picInfo[seq[play.item[*pcur_weapon].seq].frame[play.item[*pcur_weapon].frame]].box, DDBLTFAST_SRCCOLORKEY);

			if (ddrval == DDERR_WASSTILLDRAWING) goto again;


		}

	if (*pcur_magic != 0) if (play.mitem[*pcur_magic].active) {
			//disarm old weapon
			//play.mitem[*pcur_magic].seq,
			check_seq_status(play.mitem[*pcur_magic].seq);


again2:
			ddrval = lpDDSTwo->BltFast(153, 413, picInfo[seq[play.mitem[*pcur_magic].seq].frame[play.mitem[*pcur_magic].frame]].k,
			                           &picInfo[seq[play.mitem[*pcur_magic].seq].frame[play.mitem[*pcur_magic].frame]].box, DDBLTFAST_SRCCOLORKEY);

			if (ddrval == DDERR_WASSTILLDRAWING) goto again2;

		}



}


void draw_virtical(int percent, int mx, int my, int mseq, int mframe) {
	int cut;
	if (percent > 25) percent = 25;
	percent = (percent * 4);
	RECT myrect;
	CopyRect(&myrect, &picInfo[seq[mseq].frame[mframe]].box);
	int full = myrect.bottom;
	cut = (full * percent) / 100;

	myrect.bottom = cut;
	my += (full - cut);

	ddrval = lpDDSTwo->BltFast(mx, my, picInfo[seq[mseq].frame[mframe]].k,
	                           &myrect, DDBLTFAST_NOCOLORKEY);


}

void draw_virt2(int percent, int mx, int my, int mseq, int mframe) {
	int cut;
	if (percent > 25) percent = 25;
	percent = (percent * 4);
	RECT myrect;
	CopyRect(&myrect, &picInfo[seq[mseq].frame[mframe]].box);
	int full = myrect.bottom;
	cut = (full * percent) / 100;
	myrect.bottom = cut;


again:
	ddrval = lpDDSTwo->BltFast(mx, my, picInfo[seq[mseq].frame[mframe]].k,
	                           &myrect, DDBLTFAST_NOCOLORKEY);

	if (ddrval == DDERR_WASSTILLDRAWING) goto again;

}


void draw_hor(int percent, int mx, int my, int mseq, int mframe) {
	int cut;
	if (percent > 25) percent = 25;
	percent = (percent * 4);
	RECT myrect;
	CopyRect(&myrect, &picInfo[seq[mseq].frame[mframe]].box);
	int full = myrect.right;
	cut = (full * percent) / 100;
	full = cut;
	myrect.right = full;
again:
	ddrval = lpDDSTwo->BltFast(mx, my, picInfo[seq[mseq].frame[mframe]].k,
	                           &myrect, DDBLTFAST_NOCOLORKEY);

	if (ddrval == DDERR_WASSTILLDRAWING) goto again;

}


void draw_hor2(int percent, int mx, int my, int mseq, int mframe) {
	int cut;
	if (percent > 25) percent = 25;
	percent = (percent * 4);
	RECT myrect;
	CopyRect(&myrect, &picInfo[seq[mseq].frame[mframe]].box);
	int full = myrect.right;
	cut = (full * percent) / 100;

	myrect.right = cut;
	mx += (full - cut);


again:
	ddrval = lpDDSTwo->BltFast(mx, my, picInfo[seq[mseq].frame[mframe]].k,
	                           &myrect, DDBLTFAST_NOCOLORKEY);

	if (ddrval == DDERR_WASSTILLDRAWING) goto again;

}


void draw_mlevel(int percent) {

	//if (*pmagic_level < 1) return;

	int mseq = 180;
	int bary = 6;
	int barx = 7;



	if (percent > 0) draw_virtical(percent, 149, 411, mseq, bary);
	percent -= 25;
	if (percent > 0) draw_hor(percent, 149, 409, mseq, barx);
	percent -= 25;
	if (percent > 0) draw_virt2(percent, 215, 411, mseq, bary);
	percent -= 25;
	if (percent > 0) draw_hor2(percent, 149, 466, mseq, barx);


}


void draw_status_all() {
	RECT rcRect;
	rcRect.left = 0;
	rcRect.top = 0;
	rcRect.right = 640;
	rcRect.bottom = 80;

again:
	ddrval = lpDDSTwo->BltFast(0, 400, picInfo[seq[180].frame[3]].k,
	                           &rcRect, DDBLTFAST_NOCOLORKEY);

	if (ddrval == DDERR_WASSTILLDRAWING) goto again;

	rcRect.left = 0;
	rcRect.top = 0;
	rcRect.right = 20;
	rcRect.bottom = 400;
again2:
	ddrval = lpDDSTwo->BltFast(0, 0, picInfo[seq[180].frame[1]].k,
	                           &rcRect, DDBLTFAST_NOCOLORKEY);

	if (ddrval == DDERR_WASSTILLDRAWING) goto again2;


again3:
	ddrval = lpDDSTwo->BltFast(620, 0, picInfo[seq[180].frame[2]].k,
	                           &rcRect, DDBLTFAST_NOCOLORKEY);

	if (ddrval == DDERR_WASSTILLDRAWING) goto again3;

	fraise = next_raise();
	if (*pexper < fraise) {
		fexp = *pexper;
	} else {
		fexp = fraise - 1;
	}
	fstrength = *pstrength;
	fmagic = *pmagic;
	fgold = *pgold;
	fdefense = *pdefense;
	last_magic_draw = 0;
	draw_exp();
	draw_health();
	draw_strength();
	draw_defense();
	draw_magic();
	draw_gold();
	draw_level();
	draw_icons();
	if (*pmagic_cost > 0) if (*pmagic_level > 0)
			draw_mlevel(*pmagic_level / static_cast<double>(*pmagic_cost) / 100.0);
}

int add_sprite_dumb(int x1, int y, int brain, int pseq, int pframe, int size) {
	for (int x = 1; x < max_sprites_at_once; x++) {
		if (spr[x].active == false) {
			memset(&spr[x], 0, sizeof(spr[x]));

			//Msg("Making sprite %d.",x);
			spr[x].active = true;
			spr[x].x = x1;
			spr[x].y = y;
			spr[x].my = 0;
			spr[x].mx = 0;
			spr[x].speed = 0;
			spr[x].brain = brain;
			spr[x].frame = 0;
			spr[x].pseq = pseq;
			spr[x].pframe = pframe;
			spr[x].size = size;
			spr[x].seq = 0;
			if (x > last_sprite_created)
				last_sprite_created = x;

			spr[x].timer = 0;
			spr[x].wait = 0;
			spr[x].lpx[0] = 0;
			spr[x].lpy[0] = 0;
			spr[x].moveman = 0;
			spr[x].seq_orig = 0;


			spr[x].base_hit = -1;
			spr[x].base_walk = -1;
			spr[x].base_die = -1;
			spr[x].base_idle = -1;
			spr[x].base_attack = -1;
			spr[x].last_sound = 0;
			spr[x].hard = 1;

			SetRect(&spr[x].alt, 0, 0, 0, 0);
			spr[x].althard = 0;
			spr[x].sp_index = 0;
			spr[x].nocontrol = 0;
			spr[x].idle = 0;
			spr[x].strength = 0;
			spr[x].damage = 0;
			spr[x].defense = 0;

			if (spr[x].custom == NULL) {
				spr[x].custom = new Common::HashMap<Common::String, int>();
			} else {
				spr[x].custom->clear();
			}

			return x;
		}
	}

	return 0;
}



bool get_box(int h, RECT *boxCrap, RECT *boxReal) {
	RECT mathRect;
	int sZ, sY, xOffset, yOffset;

	int mplayx = playx;
	int mplayl = playl;
	int mplayy = playy;


	if (spr[h].noclip) {
		mplayx = 640;
		mplayl = 0;
		mplayy = 480;

	}

	RECT krect;

	if (getpic(h) < 1) {

		if (dinkedit) Msg("Yo, sprite %d has a bad pic. (Map %d) Seq %d, Frame %d", h, cur_map, spr[h].pseq, spr[h].pframe);
		else
			Msg("Yo, sprite %d has a bad pic. (Map %d) Seq %d, Frame %d", h, *pmap, spr[h].pseq, spr[h].pframe);

		//redink1 added to fix frame-not-in-memory immediately
		if (spr[h].pseq != 0) check_seq_status(spr[h].pseq);
		//spr[h].pic = 44;
	}

	int txoffset = picInfo[getpic(h)].xoffset;
	int tyoffset = picInfo[getpic(h)].yoffset;




	*boxReal = picInfo[getpic(h)].box;
	CopyRect(&krect, &picInfo[getpic(h)].box);

	if (spr[h].size != 100) sZ = ((krect.right * spr[h].size) / 100);
	else sZ = 0;
	if (spr[h].size != 100) sY = ((krect.bottom * spr[h].size) / 100);
	else sY = 0;

	if (spr[h].size != 100) {
		sZ = ((sZ - krect.right) / 2);
		sY = ((sY - krect.bottom) / 2);
	}


	boxCrap->left = spr[h].x - txoffset - sZ;
	mathRect.left = spr[h].x - txoffset;

	boxCrap->top = spr[h].y - tyoffset - sY;
	mathRect.top = spr[h].y - tyoffset;

	boxCrap->right = (mathRect.left + (krect.right - krect.left)) + sZ;
	mathRect.right = mathRect.left + krect.right;

	boxCrap->bottom = (mathRect.top + (krect.bottom - krect.top)) + sY;
	mathRect.bottom = mathRect.top + krect.bottom;
	//if (OffsetRect(&spr[h].alt2,44,33) == 0) Msg("Error with set rect");


	if ((spr[h].alt.right != 0) | (spr[h].alt.left != 0) | (spr[h].alt.top != 0) | (spr[h].alt.right != 0)) {
		//redink1 checks for correct box stuff
		if (spr[h].alt.left < 0)
			spr[h].alt.left = 0;
		if (spr[h].alt.left > picInfo[getpic(h)].box.right)
			spr[h].alt.left =  picInfo[getpic(h)].box.right;
		if (spr[h].alt.top < 0)
			spr[h].alt.top = 0;
		if (spr[h].alt.top > picInfo[getpic(h)].box.bottom)
			spr[h].alt.top =  picInfo[getpic(h)].box.bottom;
		if (spr[h].alt.right < 0)
			spr[h].alt.right = 0;
		if (spr[h].alt.right > picInfo[getpic(h)].box.right)
			spr[h].alt.right =  picInfo[getpic(h)].box.right;
		if (spr[h].alt.bottom < 0)
			spr[h].alt.bottom = 0;
		if (spr[h].alt.bottom > picInfo[getpic(h)].box.bottom)
			spr[h].alt.bottom =  picInfo[getpic(h)].box.bottom;
		//spr[h].alt.bottom = 10;
		boxCrap->left = boxCrap->left +  spr[h].alt.left;
		boxCrap->top = boxCrap->top + spr[h].alt.top;
		boxCrap->right = boxCrap->right - (picInfo[getpic(h)].box.right - spr[h].alt.right);
		boxCrap->bottom = boxCrap->bottom - (picInfo[getpic(h)].box.bottom - spr[h].alt.bottom);
		CopyRect(boxReal, &spr[h].alt);
		//Msg("I should be changing box size... %d %d,%d,%d",spr[h].alt.right,spr[h].alt.left,spr[h].alt.top,spr[h].alt.bottom);

	}


	//********* Check to see if they need to be cut down and do clipping






	if (spr[h].size == 0) spr[h].size = 100;

	if (dinkedit) if ((mode == 1) | (mode == 5)) if (draw_map_tiny < 1) goto do_draw;

	if (boxCrap->left < mplayl) {
		xOffset = boxCrap->left * (-1) + mplayl;
		boxCrap->left = mplayl;
		//boxReal->left += (mathRect.left * (-1)) + mplayl;

		if (spr[h].size != 100)
			boxReal->left += (((xOffset * 100) / (spr[h].size)));
		else

			boxReal->left += xOffset;
		if (boxCrap->right - 1 < mplayl) goto nodraw;
	}

	if (boxCrap->top < 0) {
		yOffset = boxCrap->top * (-1);
		boxCrap->top = 0;

		//boxReal->top += mathRect.top * (-1) + 0;
		if (spr[h].size != 100)
			boxReal->top += (((yOffset * 100) / (spr[h].size)));

		else boxReal->top += yOffset;
		if (boxCrap->bottom - 1 < 0) goto nodraw;
	}


	if (boxCrap->right > mplayx) {
		xOffset = (boxCrap->right) - mplayx;
		boxCrap->right = mplayx;
		//x_real->right -= mathRect.right - mplayx;
		if (spr[h].size != 100)
			boxReal->right -= ((xOffset * 100) / (spr[h].size));
		else boxReal->right -= xOffset;
		if (boxCrap->left + 1 > mplayx) goto nodraw;

		//  Msg("ok, crap right is %d, real right is %d.",boxCrap->right - boxCrap->left,boxReal->right);
	}

	if (boxCrap->bottom > mplayy) {
		yOffset = (boxCrap->bottom) - mplayy;
		boxCrap->bottom = mplayy;
		if (spr[h].size != 100)
			boxReal->bottom -= ((yOffset * 100) / (spr[h].size));
		else boxReal->bottom -= yOffset;
		if (boxCrap->top + 1 > mplayy) goto nodraw;
	}




do_draw:




	/*if (  (boxCrap->right-boxCrap->left) != (boxReal->right-boxReal->left) )
	{
	Msg("Ok, sprite %d is being scaled.", h);
	}
	*/
	return true;

nodraw:

	return false;

}


void reload_sprites(char name[100], int nummy, int junk) {
	HRESULT     ddrVal;
	PALETTEENTRY    holdpal[256];

	char crap[100], hold[10];
	int n;
	n = 0;

	lpDDPal->GetEntries(0, 0, 256, holdpal);
	lpDDPal->SetEntries(0, 0, 256, real_pal);


	for (int oo = index[nummy].s + 1; oo <= index[nummy].s + index[nummy].last; oo++) {
		n++;

		//  Msg( "%s", crap);

		//  initFail(hWndMain, crap);
		ddrVal = picInfo[oo].k->Restore();
		if (ddrVal == DD_OK) {


			if (n < 10) dink_strcpy(hold, "0");
			else dink_strcpy(hold, "");
			dink_sprintf(crap, "%s%s%d.BMP", name, hold, n);

			DDReLoadBitmap(picInfo[oo].k, crap);
			//Msg("Sprite %s%d.bmp reloaded into area %d. ",name,n,oo);


		}
	}
	lpDDPal->SetEntries(0, 0, 256, holdpal);
}





void refigure_out(const char *line) {
	char ev[15][100];
	RECT hardbox;
	ZeroMemory(&ev, sizeof(ev));
	//int myseq = 0, myframe = 0;

	for (int i = 1; i <= 14; i++) {
		seperate_string(line, i, ' ', ev[i]);
		//   Msg("Word %d is \"%s\"",i,ev[i]);
	}

	if (compare(ev[1], "LOAD_SEQUENCE_NOW")) {
		//           name   seq    speed       offsetx     offsety       hardx      hardy
		//  reload_sprites(ev[2],atol(ev[3]),0);
	}


}



void reload_batch() {
	File f;
	Common::String line;

	Msg("reoading .ini");

	if (!f.open("dink.ini")) {
		Msg("File not found.");

	} else {
		while (f.eos()) {
			line = f.readLine();
			refigure_out(line.c_str());
		}

		f.close();
	}
}

void decompress(Common::SeekableReadStream *in) {
	unsigned char stack[16], pair[128][2];
	byte c, top = 0;

	/* Check for optional pair count and pair table */
	if ((c = in->readByte()) > 127) {
		in->read(pair, 2 * (c - 128));
	} else {
		if (c == '\r')
			c = '\n';
		if (c == 9)
			c = ' ';

		strchar(cbuf, c);
	}

	for (;;) {
		/* Pop byte from stack or read byte from file */
		if (top)
			c = stack[--top];
		else {
			c = in->readByte();
			if (in->eos())
				break;
		}

		/* Push pair on stack or output byte to file */
		if (c > 127) {
			stack[top++] = pair[c - 128][1];
			stack[top++] = pair[c - 128][0];
		} else {
			if (c == '\r') c = '\n';
			if (c == 9) c = ' ';

			strchar(cbuf, c); //     putc(c,out);
		}
	}
}


void decompress_nocomp(Common::SeekableReadStream *in) {
	unsigned char stack[16];
	byte c;
	int top = 0;

	c = in->readByte();
	if (c == '\r')
		c = '\n';
	if (c == 9)
		c = ' ';

	strchar(cbuf, c);

	for (;;) {
		// Pop byte from stack or read byte from file
		if (top) {
			c = stack[--top];
		} else {
			c = in->readByte();
			if (in->eos())
				break;
		}

		if (c == '\r')
			c = '\n';
		if (c == 9)
			c = ' ';

		strchar(cbuf, c);
	}
}


int load_script(const char *filename, int sprite, bool set_sprite) {
	char temp[100];
	int script;
	File f;
	bool comp = false;
	char tab[10];

	Msg("Loading %s", filename);
	dink_sprintf(tab, "%c", 9);

	dink_sprintf(temp, "story\\%s.d", filename);

	if (!File::exists(temp)) {
		dink_sprintf(temp, "story\\%s.c", filename);
		if (!File::exists(temp)) {
			Msg("Script %s not found. (checked for .C and .D) (requested by %d?)", temp, sprite);
			return 0;
		}
	}

	strupr(temp);
	Msg("Temp thingie is %c", temp[strlen(temp) - 1]);
	if (temp[strlen(temp) - 1] == 'D')
		comp = true;
	else
		comp = false;

	int ii;
	for (ii = 1; ii < max_scripts; ii++) {
		if (rbuf[ii] == NULL) {
			// Found one not being used
			break;
		}
	}

	if (ii == max_scripts) {
		Msg("Couldn't find unused buffer for script.");
		return 0;
	}

	script = ii;
	rinfo[script] = (refinfo *)malloc(sizeof(refinfo));

	memset(rinfo[script], 0, sizeof(refinfo));

	//load compiled script
	if (!f.open(temp)) {
		Msg("Script %s not found. (checked for .c and .d) (requested by %d?)", temp, sprite);
		return 0;
	}

	cbuf[0] = 0;

	if (comp)
		decompress(&f);
	else
		decompress_nocomp(&f);

	f.close();

	rinfo[script]->end = strlen(cbuf);
	rbuf[script] = (char *)malloc(rinfo[script]->end);

	if (rbuf[script] == nullptr) {
		Msg("Couldn't allocate rbuff %d.", script);
		return 0;
	}

	CopyMemory(rbuf[script], &cbuf, rinfo[script]->end);

	if (rinfo[script] == nullptr) {
		Msg("Couldn't allocate rscript %d.", script);
		return 0;
	}

	dink_strcpy(rinfo[script]->name, filename);
	rinfo[script]->sprite = sprite;

	if (set_sprite) {
		if (sprite != 0) if (sprite != 1000)
			spr[sprite].script = script;
	}

	return script;
}

bool locate(int script, const char *proc) {

	if (rinfo[script] == NULL) {
		return false;

	}
	int saveme = rinfo[script]->current;
	rinfo[script]->current = 0;
	char line[200];
	char ev[3][100];
	char temp[100];

	//Msg("locate is looking for %s in %s", proc, rinfo[script]->name);

	while (read_next_line(script, line)) {
		strip_beginning_spaces(line);
		ZeroMemory(&ev, sizeof(ev));

		get_word(line, 1, ev[1]);
		if (compare(ev[1], "VOID")) {
			get_word(line, 2, ev[2]);

			seperate_string(ev[2], 1, '(', temp);

			//      Msg("Found procedure %s.",temp);
			if (compare(temp, proc)) {
				//              Msg("Located %s",proc);
				//clean up vars so it is ready to run
				if (rinfo[script]->sprite != 1000) {
					spr[rinfo[script]->sprite].move_active = false;
					spr[rinfo[script]->sprite].move_nohard = false;

				}
				rinfo[script]->skipnext = false;
				rinfo[script]->onlevel = 0;
				rinfo[script]->level = 0;

				return true;
				//this is desired proc

			}
		}

	}

	//Msg("Locate ended on %d.", saveme);
	rinfo[script]->current = saveme;
	return false;

}

/*void check_for_real_vars(char crap[20], int i)
{
if (compare(play.var[i].name, "&vision")) map_vision = play.var[i].var;
if (compare(play.var[i].name, "&life")) map_vision = play.var[i].var;
if (compare(play.var[i].name, "&exp")) map_vision = play.var[i].var;

}
*/

//redink1 added to recursively check scope
/*bool recurse_scope(int var, int script)
{
//if the script matches, return true!
if (play.var[var].scope == script)
return true;

//If it doesn't match, and we have a proc return, go down the rabbit hole
if (rinfo[script] && rinfo[script]->proc_return != 0)
return recurse_scope(var, rinfo[script]->proc_return);

//Otherwise, return false
return false;
}*/

//redink1 added, grabs the var index matching 'name' with the shortest scope.  Basically iterates for each scope seperate until it finds a match.
int get_var(int script, char *name) {
	//Can optimize here, by searching through variable array for start and end limits

	//Loop forever...
	while (1) {
		//We'll start going through every var, starting at one
		int var = 1;
		while (var < max_vars) {
			//Okay... make sure the var is active,
			//The scope should match the script,
			//Then make sure the name is the same.
			if (play.var[var].active && play.var[var].scope == script && compare(play.var[var].name, name))
				return var;

			//Otherwise, go to the next var.
			var++;
		}

		//If we just went through the global list, let's return
		if (script <= 0)
			break;

		//Bugfix... if there is no rinfo[script] entry (like if kill this task was used), we go directly to the globals.
		//Thanks Tal!
		//if (!rinfo[script])
		//  script = 0;
		//Go into the next proc from the script.  If there are no parent procs, it should be 0, which is global.
		//else
		//    script = rinfo[script]->proc_return;

		//Changed to not reference the parent procedure's variable list at all... just go on to globals.
		script = 0;
	}

	return 0;
}

//redink1 changes for replacing var in string
bool recurse_var_replace(int i, int script, char *line, char *prevar) {
	while (i < max_vars) {
		//First, make sure the variable is active.
		//Then, make sure it is in scope,
		//Then, see if the variable name is in the line
		//Then, prevar is null, or if prevar isn't null, see if current variable starts with prevar
		if (play.var[i].active &&
		        i == get_var(script, play.var[i].name) &&
		        strstr(line, play.var[i].name) &&
		        (prevar == NULL || prevar != NULL && strstr(play.var[i].name, prevar))) {
			//Look for shorter variables
			if (!recurse_var_replace(i + 1, script, line, play.var[i].name)) {
				//we didn't find any, so we replace!
				char crap[20];
				dink_sprintf(crap, "%d", play.var[i].var);
				replace(play.var[i].name, crap, line);
				//return true;
			}
		}
		i++;
	}
	return false;
}

void decipher_string(char line[200], int script) {
	char crap[20];
	char buffer[20];
	char crab[100];
	int mytime;

	//redink1 replaced with recursive function for finding longest variable
	recurse_var_replace(1, script, line, NULL);

	/*for (int i = 1; i < max_vars; i ++)
	{
	if (play.var[i].active == true)
	if (  (play.var[i].scope == 0) || recurse_scope(play.var[i].scope, script) )
	{
	dink_sprintf(crap, "%d", play.var[i].var);
	replace(play.var[i].name, crap, line);

	//        check_for_real_vars(crap, i);
	//break;
	}
	}*/

	if ((strchr(line, '&') != NULL) && (script != 0)) {
		replace("&current_sprite", ltoa(rinfo[script]->sprite, buffer, 10), line);
		replace("&current_script", ltoa(script, buffer, 10), line);
		//v1.08 special variables.
		replace("&return", ltoa(returnint, buffer, 10), line);
		replace("&arg1", ltoa(rinfo[script]->arg1, buffer, 10), line);
		replace("&arg2", ltoa(rinfo[script]->arg2, buffer, 10), line);
		replace("&arg3", ltoa(rinfo[script]->arg3, buffer, 10), line);
		replace("&arg4", ltoa(rinfo[script]->arg4, buffer, 10), line);
		replace("&arg5", ltoa(rinfo[script]->arg5, buffer, 10), line);
		replace("&arg6", ltoa(rinfo[script]->arg6, buffer, 10), line);
		replace("&arg7", ltoa(rinfo[script]->arg7, buffer, 10), line);
		replace("&arg8", ltoa(rinfo[script]->arg8, buffer, 10), line);
		replace("&arg9", ltoa(rinfo[script]->arg9, buffer, 10), line);

		if (decipher_savegame != 0) {
			if (play.button[decipher_savegame] == 1)    replace("&buttoninfo", "Attack", line);
			else if (play.button[decipher_savegame] == 2)    replace("&buttoninfo", "Talk/Examine", line);
			if (play.button[decipher_savegame] == 3)    replace("&buttoninfo", "Magic", line);
			if (play.button[decipher_savegame] == 4)    replace("&buttoninfo", "Item Screen", line);
			if (play.button[decipher_savegame] == 5)    replace("&buttoninfo", "Main Menu", line);
			if (play.button[decipher_savegame] == 6)    replace("&buttoninfo", "Map", line);
			if (play.button[decipher_savegame] == 7)    replace("&buttoninfo", "Unused", line);
			if (play.button[decipher_savegame] == 8)    replace("&buttoninfo", "Unused", line);
			if (play.button[decipher_savegame] == 9)    replace("&buttoninfo", "Unused", line);
			if (play.button[decipher_savegame] == 10)   replace("&buttoninfo", "Unused", line);


		}

	}


	if (decipher_savegame != 0)
		if (compare(line, "&savegameinfo")) {
			dink_sprintf(crap, "save%d.dat", decipher_savegame);
			if (File::exists(crap)) {
				load_game_small(decipher_savegame, crab, &mytime);
				//redink1 fix for savegame time bug
				dink_sprintf(line, "Slot %d - %d:%02d - %s", decipher_savegame, (mytime / 60), mytime - ((mytime / 60) * 60), crab);
				//dink_sprintf(line, "In Use");
			} else {

#ifdef __GERMAN
				dink_sprintf(line, "Slot %d - Ungebraucht", decipher_savegame);
#endif

#ifdef __ENGLISH
				dink_sprintf(line, "Slot %d - Empty", decipher_savegame);
#endif


			}

		}
}

int add_sprite(int x1, int y, int brain, int pseq, int pframe) {

	for (int x = 1; x < max_sprites_at_once; x++) {
		if (spr[x].active == false) {
			memset(&spr[x], 0, sizeof(spr[x]));

			spr[x].active = true;
			spr[x].x = x1;
			spr[x].y = y;
			spr[x].my = 0;
			spr[x].mx = 0;
			spr[x].speed = 1;
			spr[x].brain = brain;
			spr[x].frame = 0;
			spr[x].pseq = pseq;
			spr[x].pframe = pframe;
			spr[x].seq = 0;
			if (x > last_sprite_created)
				last_sprite_created = x;
			spr[x].timer = 33;
			spr[x].wait = 0;
			spr[x].lpx[0] = 0;
			spr[x].lpy[0] = 0;
			spr[x].moveman = 0;
			spr[x].size = 100;
			spr[x].que = 0;
			spr[x].strength = 0;
			spr[x].damage = 0;
			spr[x].defense = 0;
			spr[x].hard = 1;

			if (spr[x].custom == NULL) {
				spr[x].custom = new Common::HashMap<Common::String, int>();
			} else {
				spr[x].custom->clear();
			}

			return x;
		}

	}

	return 0;
}

void check_sprite_status(int h) {
	HRESULT dderror;
	char word1[80];
	//is sprite in memory?
	if (spr[h].pseq > 0) {
		// Msg("Smartload: Loading seq %d..", spr[h].seq);
		if (seq[spr[h].pseq].frame[1] == 0) {
			figure_out(seq[spr[h].pseq].data, 0);
		} else {
			//it's been loaded before.. is it lost or still there?
			//Msg("Sprite %d's seq is %d",h,spr[h].seq);

			dderror = picInfo[seq[spr[h].pseq].frame[1]].k->IsLost();

			if (dderror == DDERR_SURFACELOST) {
				get_word(seq[spr[h].pseq].data, 2, word1);

				reload_sprites(word1, spr[h].pseq, 0);
				//Msg("Reloaded seq %d with path of %s should be %s", spr[h].seq, word1,seq[spr[h].seq].data );
			}


		}
	}




}


void check_frame_status(int h, int frame)

{
//    HRESULT dderror;
	char word1[80];

	if (seq[h].active == false) return;

	if (h > 0) {
		// Msg("Smartload: Loading seq %d..", spr[h].seq);
		if (seq[h].frame[1] == 0) {
			first_frame = true;
			figure_out(seq[h].data, 0);
			first_frame = false;
		} else {
			//it's been loaded before.. is it lost or still there?
			//Msg("Sprite %d's seq is %d",h,spr[h].seq);

			if (picInfo[seq[h].frame[1]].k == NULL) {
				figure_out(seq[h].data, 0);
			} else if (picInfo[seq[h].frame[1]].k->IsLost() == DDERR_SURFACELOST) {
				get_word(seq[h].data, 2, word1);

				reload_sprites(word1, h, 0);
				//Msg("Reloaded seq %d with path of %s should be %s", spr[h].seq, word1,seq[spr[h].seq].data );
			}
		}
	}


}

void check_seq_status(int h)

{
//    HRESULT dderror;
	char word1[80];

	if (seq[h].active == false) return;
	if (h > 0) if (h < max_sequences) {
			// Msg("Smartload: Loading seq %d..", spr[h].seq);

			if (seq[h].frame[1] == 0 || picInfo[seq[h].frame[1]].k == NULL) {
				figure_out(seq[h].data, 0);
			} else {
				//it's been loaded before.. is it lost or still there?
				//Msg("Sprite %d's seq is %d",h,spr[h].seq);

				if (picInfo[seq[h].frame[1]].k->IsLost() == DDERR_SURFACELOST) {
					get_word(seq[h].data, 2, word1);

					reload_sprites(word1, h, 0);
					//Msg("Reloaded seq %d with path of %s should be %s", spr[h].seq, word1,seq[spr[h].seq].data );
				}
			}
		}


}


void check_base(int base) {
	for (int i = 1; i < 10; i++) {
		if (seq[base + i].active == true) check_seq_status(base + i);

	}


}

void check_sprite_status_full(int h) {
//    HRESULT dderror;
//    char word1[80];
	//same as above but checks for all seq's used by the (base) commands

	//is sprite in memory?

	check_seq_status(spr[h].pseq);

	if (spr[h].base_walk > -1) check_base(spr[h].base_walk);


}


int say_text(const char *text, int h, int script) {
	int crap2;
	//Msg("Creating new sprite with %s connect to %d.",text, h);
	if (h == 1000) crap2 = add_sprite(100, 100, 8, 0, 0);
	else crap2 = add_sprite(spr[h].x, spr[h].y, 8, 0, 0);

	if (crap2 == 0) {
		Msg("Couldn't say something, out of sprites.");
		return 0;

	}
	*plast_text = crap2;
	dink_strcpy(spr[crap2].text, text);
	spr[crap2].kill = strlen(text) * text_timer;
	if (spr[crap2].kill < text_min) spr[crap2].kill = text_min;
	spr[crap2].damage = -1;
	spr[crap2].owner = h;
	spr[crap2].hard = 1;
	spr[crap2].script = script;
	//set X offset for text, using strength var since it's unused
	spr[crap2].strength = 75;
	//spr[h].x - spr[crap2;
	spr[crap2].nohit = 1;
	check_seq_status(spr[spr[crap2].owner].seq);
	spr[crap2].defense = (((picInfo[getpic(spr[crap2].owner)].box.bottom) - picInfo[getpic(spr[crap2].owner)].yoffset) + 100);

	spr[crap2].x = spr[spr[crap2].owner].x - spr[crap2].strength;
	spr[crap2].y = spr[spr[crap2].owner].y - spr[crap2].defense;

	return crap2;
}


int say_text_xy(const char *text, int mx, int my, int script) {
	int crap2;
	//Msg("Creating new sprite with %s connect to %d.",text, h);
	crap2 = add_sprite(mx, my, 8, 0, 0);

	if (crap2 == 0) {
		Msg("Couldn't say something, out of sprites.");
		return 0;

	}
	*plast_text = crap2;
	dink_strcpy(spr[crap2].text, text);
	spr[crap2].kill = strlen(text) * text_timer;
	if (spr[crap2].kill < text_min) spr[crap2].kill = text_min;
	spr[crap2].damage = -1;
	spr[crap2].nohit = 1;
	spr[crap2].owner = 1000;
	spr[crap2].hard = 1;
	spr[crap2].script = script;
	return crap2;
}

void kill_sprite_all(int sprite) {
	spr[sprite].active = false;

	kill_text_owned_by(sprite);
	kill_scripts_owned_by(sprite);

}


void kill_returning_stuff(int script) {
	int i;

	//Msg("Checking callbacks..");
	//check callbacks
	for (i = 1; i < max_callbacks; i++) {
		if (callback[i].active) if (callback[i].owner == script)
				//  if (compare(callback[i].name, ""))
			{
				Msg("killed a returning callback, ha!");
				callback[i].active = false;
			}

	}


	for (i = 1; i <= last_sprite_created; i++) {
		if (spr[i].active) if (spr[i].brain == 8) if (spr[i].callback == script) {
					Msg("Killed sprites callback command");
					spr[i].callback = 0;
				}

	}

}




bool talk_get(int script) {
	char line[200], check[200], checker[200];
	int cur = 1;
	char *p;
	//int num;
	int retnum = 0;
	clear_talk();
	talk.newy = -5000;
	while (1) {
redo:

		read_next_line(script, line);



		strip_beginning_spaces(line);
		//Msg("Comparing to %s.", line);

		get_word(line, 1, checker);

		if (compare(checker, "set_y")) {

			get_word(line, 2, checker);
			talk.newy = atol(checker);

			goto redo;
		}

		if (compare(checker, "set_title_color")) {

			get_word(line, 2, checker);
			talk.color = atol(checker);
			goto redo;
		}


		if (compare(line, "\n")) goto redo;
		if (compare(line, "\\\\")) goto redo;


		strip_beginning_spaces(line);
		//Msg("Comparing to %s.", line);
		if (compare(line, "\n")) goto redo;
		if (compare(line, "\\\\")) goto redo;

morestuff:

		seperate_string(line, 1, '(', check);
		strip_beginning_spaces(check);

		if (compare(check, "title_start")) {

			while (read_next_line(script, line)) {

				dink_strcpy(check, line);
				strip_beginning_spaces(line);
				get_word(line, 1, checker);
				seperate_string(line, 1, '(', check);
				strip_beginning_spaces(check);

				if (compare(check, "title_end")) {
					replace("\n\n\n\n", "\n \n", talk.buffer);

					replace("\n\n", "\n", talk.buffer);

					goto redo;
				}

				line[strlen(line)] = 0;
				//Msg("LINE IS: %s: Like it?",line);

				decipher_string(line, script);
				dink_strcat(talk.buffer, line);
				//talk.buffer[strlen(talk.buffer)-1] = 0;
			}

			goto redo;
		}

		if (compare(check, "choice_end")) {
			if (cur - 1 == 0) {
				Msg("Error: choice() has 0 options in script %s, offset %d.",
				    rinfo[script]->name, rinfo[script]->current);

				return false;
			}
			//all done, lets jam
			//Msg("found choice_end, leaving!");
			talk.last = cur - 1;
			talk.cur = 1;
			talk.active = true;
			talk.page = 1;
			talk.cur_view = 1;
			talk.script = script;
			return true;

		}



		seperate_string(line, 1, '\"', check);
		strip_beginning_spaces(check);

		//Msg("Check is %s.",check);

		if (strlen(check) > 2) {
			//found conditional statement
			if (strchr(check, '(') == NULL)

			{
				Msg("Error with choice() statement in script %s, offset %d. (%s?)",
				    rinfo[script]->name, rinfo[script]->current, check);
				return false;
			}

			seperate_string(check, 2, '(', checker);
			seperate_string(checker, 1, ')', check);

			//Msg("Running %s through var figure..", check);
			if (var_figure(check, script) == 0) {
				Msg("Answer is no.");
				retnum++;
				goto redo;
				//said NO to statement
			}
			//Msg("Answer is yes.");
			seperate_string(line, 1, ')', check);

			p = &line[strlen(check) + 1];

			dink_strcpy(check, p);


			dink_strcpy(line, check);

			//Msg("new line is %s, happy?", line);
			goto morestuff;
		}



		seperate_string(line, 2, '\"', check);
		strip_beginning_spaces(check);
		// Msg("Line %d is %s.",cur,check);
		retnum++;
		decipher_savegame = retnum;
		decipher_string(check, script);
		decipher_savegame = 0;
		dink_strcpy(talk.line[cur], check);
		talk.line_return[cur] = retnum;
		cur++;
	}

}

void get_right(char line[200], char thing[100], char *ret) {
	char *dumb;
	int pos = strcspn(line, thing);


	if (pos == NULL) {
		dink_strcpy(ret, "");
		return;
	}


	dumb = &ret[pos + 1];
	dink_strcpy(ret, dumb);
}

void draw_sprite_game(LPDIRECTDRAWSURFACE lpdest, int h) {
	if (g_b_kill_app) return; //don't try, we're quitting
	if (spr[h].brain == 8) return;

	if (spr[h].nodraw == 1) return;
	RECT boxCrap, boxReal;

	HRESULT             ddrVal;

	DDBLTFX     ddBltFx;
	ddBltFx.dwSize = sizeof(ddBltFx);
	ddBltFx.dwFillColor = 0;

	if (get_box(h, &boxCrap, &boxReal)) {
		//redink1 error checking for invalid rectangle
		if (boxCrap.left >= boxCrap.right || boxCrap.top >= boxCrap.bottom)
			return;


		//redink1 error checking for out-of-bounds clipping
		/*if (boxCrap.left < 0)
		boxCrap.left = 0;
		if (boxCrap.top < boxReal.top)
		boxCrap.top = boxCrap.top;
		if (boxCrap.right > boxReal.right)
		boxCrap.right = boxReal.right;
		if (boxCrap.bottom > boxReal.bottom)
		boxCrap.bottom = boxReal.bottom;*/
		while (1) {

			//  Msg("Box_crap: %d %d %d %d, Box_real: %d %d %d %d",boxCrap.left,boxCrap.top,
			//      boxCrap.right, boxCrap.bottom,boxReal.left,boxReal.top,
			//      boxReal.right, boxReal.bottom);

again:
			ddrVal = lpdest->Blt(&boxCrap, picInfo[getpic(h)].k,
			                     &boxReal, DDBLT_KEYSRC, &ddBltFx);


			if (ddrVal == DDERR_WASSTILLDRAWING) goto again;

			if (ddrVal != DD_OK) {
				dderror(ddrVal);

				Msg("MainSpriteDraw(): Could not draw sprite %d, pic %d.", h, getpic(h));
				Msg("Box_crap: %d %d %d %d, Box_real: %d %d %d %d", boxCrap.left, boxCrap.top,
				    boxCrap.right, boxCrap.bottom, boxReal.left, boxReal.top,
				    boxReal.right, boxReal.bottom);
				if (spr[h].pseq != 0) check_seq_status(spr[h].pseq);
				break;
			} else {
				break;
			}


		}
	}
}

void changedir(int dir1, int k, int base) {
	int hspeed;
	int speed_hold = spr[k].speed;
	if (k > 1) if (spr[k].brain != 9) if (spr[k].brain != 10) {

				//if (mbase_timing > 20) mbase_timing = 20;

				//   Msg(",base_timing is %d", base_timing);
				hspeed = spr[k].speed * (base_timing / 4);
				if (hspeed > 49) {
					Msg("Speed was %d", hspeed);
					spr[k].speed = 49;
				} else
					spr[k].speed = hspeed;
			}
	int old_seq = spr[k].seq;
	spr[k].dir = dir1;

	if (dir1 == 1) {
		spr[k].mx = (0 - spr[k].speed) + (spr[k].speed / 3);
		spr[k].my = spr[k].speed - (spr[k].speed / 3);

		if (base != -1) {


			spr[k].seq = base + 1;
			if (seq[spr[k].seq].active == false) {
				spr[k].seq = base + 9;

			}

		}

		if (old_seq != spr[k].seq) {
			spr[k].frame = 0;
			spr[k].delay = 0;
		}


	}

	if (dir1 == 2) {
		spr[k].mx = 0;
		spr[k].my = spr[k].speed;
		if (base != -1)
			spr[k].seq = base + 2;

		if (seq[spr[k].seq].active == false) if (seq[base + 3].active) spr[k].seq = base + 3;
		if (seq[spr[k].seq].active == false) if (seq[base + 1].active) spr[k].seq = base + 1;


		if (old_seq != spr[k].seq) {
			spr[k].frame = 0;
			spr[k].delay = 0;
		}


	}
	if (dir1 == 3) {
		spr[k].mx = spr[k].speed - (spr[k].speed / 3);
		spr[k].my = spr[k].speed - (spr[k].speed / 3);
		if (base != -1) {
			spr[k].seq = base + 3;
			if (seq[spr[k].seq].active == false)
				spr[k].seq = base + 7;

		}

		if (old_seq != spr[k].seq) {
			spr[k].frame = 0;
			spr[k].delay = 0;
		}


	}

	if (dir1 == 4) {

		//Msg("Changing %d to four..",k);
		spr[k].mx = (0 - spr[k].speed);
		spr[k].my = 0;
		if (base != -1)
			spr[k].seq = base + 4;
		if (seq[spr[k].seq].active == false) if (seq[base + 7].active) spr[k].seq = base + 7;
		if (seq[spr[k].seq].active == false) if (seq[base + 1].active) spr[k].seq = base + 1;
	}

	if (dir1 == 6) {
		spr[k].mx = spr[k].speed;
		spr[k].my = 0;
		if (base != -1)
			spr[k].seq = base + 6;

		if (seq[spr[k].seq].active == false) if (seq[base + 3].active) spr[k].seq = base + 3;
		if (seq[spr[k].seq].active == false) if (seq[base + 9].active) spr[k].seq = base + 9;

	}

	if (dir1 == 7) {
		spr[k].mx = (0 - spr[k].speed) + (spr[k].speed / 3);
		spr[k].my = (0 - spr[k].speed) + (spr[k].speed / 3);
		if (base != -1) {
			spr[k].seq = base + 7;


			if (seq[spr[k].seq].active == false) {

				spr[k].seq = base + 3;
			}
		}

	}
	if (dir1 == 8) {
		spr[k].mx = 0;
		spr[k].my = (0 - spr[k].speed);
		if (base != -1)
			spr[k].seq = base + 8;

		if (seq[spr[k].seq].active == false) if (seq[base + 7].active) spr[k].seq = base + 7;
		if (seq[spr[k].seq].active == false) if (seq[base + 9].active) spr[k].seq = base + 9;

	}


	if (dir1 == 9) {
		spr[k].mx = spr[k].speed - (spr[k].speed / 3);
		spr[k].my = (0 - spr[k].speed) + (spr[k].speed / 3);
		if (base != -1) {
			spr[k].seq = base + 9;
			if (seq[spr[k].seq].active == false) {
				spr[k].seq = base + 1;
			}
		}
	}



	if (old_seq != spr[k].seq) {
		spr[k].frame = 0;
		spr[k].delay = 0;
	}


	if (seq[spr[k].seq].active == false) {
		//spr[k].mx = 0;
		//spr[k].my = 0;
		spr[k].seq = old_seq;

	}

	//Msg("Leaving with %d..", spr[k].dir);

	//Msg("Changedir: Tried to switch sprite %d to dir %d",k,dir1);

	spr[k].speed = speed_hold;

}


void update_play_changes() {

	for (int j = 1; j < 100; j++) {
		if (pam.sprite[j].active)
			if (play.spmap[*pmap].type[j] != 0) {
				//lets make some changes, player has extra info
				if (play.spmap[*pmap].type[j] == 1) {
					pam.sprite[j].active = 0;

				}

				if (play.spmap[*pmap].type[j] == 2) {
					pam.sprite[j].type = 1;
					pam.sprite[j].hard = 1;
				}
				if (play.spmap[*pmap].type[j] == 3) {

					//      Msg("Changing sprite %d", j);
					pam.sprite[j].type = 0;
					pam.sprite[j].hard = 1;

				}

				if (play.spmap[*pmap].type[j] == 4) {
					pam.sprite[j].type = 1;
					pam.sprite[j].hard = 0;
				}

				if (play.spmap[*pmap].type[j] == 5) {
					pam.sprite[j].type = 0;
					pam.sprite[j].hard = 0;
				}

				if (play.spmap[*pmap].type[j] == 6) {
					pam.sprite[j].active = 0;

				}
				if (play.spmap[*pmap].type[j] == 7) {
					pam.sprite[j].active = 0;

				}
				if (play.spmap[*pmap].type[j] == 8) {
					pam.sprite[j].active = 0;

				}

				pam.sprite[j].seq = play.spmap[*pmap].seq[j];
				pam.sprite[j].frame = play.spmap[*pmap].frame[j];
				dink_strcpy(pam.sprite[j].script, "");


			}


	}
}

void update_status_all() {
	bool drawexp = false;
	int next = next_raise();
	int script;
	if (next != fraise) {
		fraise += next / 40;

		if (fraise > next) fraise = next;
		//make noise here
		drawexp = true;
		SoundPlayEffect(13, 15050, 0, 0, 0);


	}

	if (*pexper != fexp) {
		if ((talk.active == false && item_screen == false && spr[1].freeze == 0) || fexp + 10 < fraise) {
			//update screen experience
			fexp += 10;
			//make noise here

			if (fexp > *pexper) fexp = *pexper;
			drawexp = true;
			SoundPlayEffect(13, 29050, 0, 0, 0);

			if (fexp >= fraise) {

				*pexper -= next;
				fexp = 0;

				script = load_script("lraise", 1, false);
				if (locate(script, "raise")) run_script(script);
			}
		}
	}

	if (drawexp) {


		draw_exp();
	}


	if ((flifemax != *plifemax) || (flife != *plife)) {
		if (flifemax < *plifemax) flifemax++;
		if (flifemax > *plifemax) flifemax--;
		if (flife > *plife) flife--;
		if (flife < *plife) flife++;
		if (flife > *plife) flife--;
		if (flife < *plife) flife++;
		draw_bar(flifemax, 190);
		draw_bar(flife, 451);
	}

	if (fstrength != *pstrength) {
		if (fstrength < *pstrength) fstrength++;
		if (fstrength > *pstrength) fstrength--;
		SoundPlayEffect(22, 22050, 0, 0, 0);

		draw_strength();
	}

	if (fdefense != *pdefense) {
		if (fdefense < *pdefense) fdefense++;
		if (fdefense > *pdefense) fdefense--;
		SoundPlayEffect(22, 22050, 0, 0, 0);
		draw_defense();
	}
	if (fmagic != *pmagic) {
		if (fmagic < *pmagic) fmagic++;
		if (fmagic > *pmagic) fmagic--;
		SoundPlayEffect(22, 22050, 0, 0, 0);
		draw_magic();
	}

	if (fgold != *pgold) {
		if (fgold < *pgold) {
			fgold += 20;
			if (fgold > *pgold) fgold = *pgold;
		}

		if (fgold > *pgold) {
			fgold -= 20;
			if (fgold < *pgold) fgold = *pgold;
		}
		SoundPlayEffect(14, 22050, 0, 0, 0);
		draw_gold();
	}

	if (*pmagic_level < *pmagic_cost) {
		if (item_screen == false)
			*pmagic_level += *pmagic;
		if (*pmagic_level > *pmagic_cost) *pmagic_level = *pmagic_cost;
	}
	if (*pmagic_cost > 0) if (*pmagic_level > 0) {
			double mnumd = *pmagic_level;
			mnumd *= 100;
			mnumd /= *pmagic_cost;
			int mnum = static_cast<int>(mnumd);
			//int mnum = *pmagic_level / (*pmagic_cost / 100);
			if (mnum != last_magic_draw) {

				draw_mlevel(mnum);

				//draw_status_all();
				last_magic_draw = mnum;


			}
		}


	spr[1].strength = fstrength;
	spr[1].defense = fdefense;


	if (flife < 1) {
		script = load_script("dinfo", 1000, false);
		if (locate(script, "die")) run_script(script);
	}

}




void place_sprites_game() {
	int sprite;

	bool bs[max_sprites_at_once];
	int rank[max_sprites_at_once];
	int highest_sprite;

	update_play_changes();

	memset(&bs, 0, sizeof(bs));
	memset(&rank, 0, sizeof(rank));
	int hs;

	for (int r1 = 1; r1 < 100; r1++) {

		highest_sprite = 20000; //more than it could ever be

		rank[r1] = 0;

		for (int h1 = 1; h1 < 100;  h1++) {
			if (bs[h1] == false) {
				if (pam.sprite[h1].active) {
					if (pam.sprite[h1].que != 0) hs = pam.sprite[h1].que;
					else hs = pam.sprite[h1].y;
					if (hs < highest_sprite) {
						highest_sprite = hs;
						rank[r1] = h1;
					}
				}

			}
		}
		if (rank[r1] != 0)
			bs[rank[r1]] = true;
	}




	int j;

	for (int oo = 1; rank[oo] > 0; oo++) {
		//Msg("Ok, rank[%d] is %d.",oo,rank[oo]);
		j = rank[oo];



		if (pam.sprite[j].active == true) if ((pam.sprite[j].vision == 0) || (pam.sprite[j].vision == *pvision)) {



				check_seq_status(pam.sprite[j].seq);

				//we have instructions to make a sprite
				if ((pam.sprite[j].type == 0)  | (pam.sprite[j].type == 2))

				{
					//make it part of the background (much faster)

					sprite = add_sprite_dumb(pam.sprite[j].x, pam.sprite[j].y, 0,
					                         pam.sprite[j].seq, pam.sprite[j].frame,
					                         pam.sprite[j].size);
					//Msg("Background sprite %d has hard of %d..", j, pam.sprite[j].hard);
					spr[sprite].hard = pam.sprite[j].hard;

					spr[sprite].sp_index = j;

					CopyRect(&spr[sprite].alt, &pam.sprite[j].alt);


					check_sprite_status_full(sprite);
					if (pam.sprite[j].type == 0)
						draw_sprite_game(lpDDSTwo, sprite);


					if (spr[sprite].hard == 0) {
						/*if (pam.sprite[j].prop == 0)
						add_hardness(sprite, 1); else */ add_hardness(sprite, 100 + j);
					}
					spr[sprite].active = false;
				}

				if (pam.sprite[j].type == 1) {
					//make it a living sprite


					sprite = add_sprite_dumb(pam.sprite[j].x, pam.sprite[j].y, 0,
					                         pam.sprite[j].seq, pam.sprite[j].frame,
					                         pam.sprite[j].size);

					spr[sprite].hard = pam.sprite[j].hard;



					//assign addition parms to the new sprite
					spr[sprite].sp_index = j;

					spr[sprite].brain = pam.sprite[j].brain;
					spr[sprite].speed = pam.sprite[j].speed;
					spr[sprite].base_walk = pam.sprite[j].base_walk;
					spr[sprite].base_idle = pam.sprite[j].base_idle;
					spr[sprite].base_attack = pam.sprite[j].base_attack;
					spr[sprite].base_hit = pam.sprite[j].base_hit;
					spr[sprite].hard = pam.sprite[j].hard;
					spr[sprite].timer = pam.sprite[j].timer;
					spr[sprite].que = pam.sprite[j].que;


					spr[sprite].sp_index = j;


					CopyRect(&spr[sprite].alt, &pam.sprite[j].alt);

					spr[sprite].base_die = pam.sprite[j].base_die;
					spr[sprite].strength = pam.sprite[j].strength;
					spr[sprite].defense = pam.sprite[j].defense;
					spr[sprite].gold = pam.sprite[j].gold;
					spr[sprite].exp = pam.sprite[j].exp;
					spr[sprite].nohit = pam.sprite[j].nohit;
					spr[sprite].touch_damage = pam.sprite[j].touch_damage;
					spr[sprite].hitpoints = pam.sprite[j].hitpoints;
					spr[sprite].sound = pam.sprite[j].sound;
					check_sprite_status_full(sprite);
					if (pam.sprite[j].prop == 0) if (spr[sprite].sound != 0) {
							//make looping sound
							Msg("making sound with sprite %d..", sprite);
							SoundPlayEffect(spr[sprite].sound, 22050, 0, sprite, 1);


						}
					if (spr[sprite].brain == 3) {

						check_seq_status(21);
						check_seq_status(23);
						check_seq_status(24);
						check_seq_status(26);
						check_seq_status(27);
						check_seq_status(29);
						check_seq_status(111);
						check_seq_status(113);
						check_seq_status(117);
						check_seq_status(119);
						check_seq_status(121);
						check_seq_status(123);
						check_seq_status(127);
						check_seq_status(129);

					}


					if (spr[sprite].hard == 0) {
						/*      if (pam.sprite[j].prop == 0)
						add_hardness(sprite, 1); else */ add_hardness(sprite, 100 + j);


					}

					//does it need a script loaded?

					if (strlen(pam.sprite[j].script) > 1) {
						spr[sprite].script = load_script(pam.sprite[j].script, sprite, true);

					}

				}
				//Msg("I just made sprite %d because rank[%d] told me to..",sprite,j);


			}

	}


}





bool kill_last_sprite() {
	int found;
	found = 0;
	bool nosetlast = false;
	for (int k = 1; k < max_sprites_at_once; k++)

		if (spr[k].active)

		{

			if (spr[k].live) {
				nosetlast = true;
				goto crazy;

			}

			found = k;
crazy:
			;

		}
	if (found > 1) {
		spr[found].active = false;
		if (nosetlast == false)
			last_sprite_created = found - 1;
		return true;
	}

	//we didn't kill any sprites, only 1 remains
	return false;
}

int hurt_thing(int h, int damage, int special) {
	//lets hurt this sprite but good
	if (damage < 1) return 0;
	int num = damage - spr[h].defense;

	//  Msg("num is %d.. defense was %d.of sprite %d", num, spr[h].defense, h);
	if (num < 1) num = 0;

	if (num == 0) {
		if (RANDOM(1, 2) == 1)
			num = 1;
	}

	spr[h].damage += num;
	return num;
	//draw blood here
}

void random_blood(int mx, int my, int h) {
	//if (RANDOM(0, 2) == 1) myseq = 188; else myseq = 187;
	//redink1 - customizable blood depending on what sprite we hit!!
	int myseq;
	int randy;

	if (spr[h].bloodseq > 0 && spr[h].bloodnum > 0) {
		myseq = spr[h].bloodseq;
		randy = spr[h].bloodnum;
	} else {
		myseq = 187;
		randy = 3;
	}

	myseq += RANDOM(0, randy);

	int crap2 = add_sprite(mx, my, 5, myseq, 1);
	spr[crap2].speed = 0;
	spr[crap2].base_walk = -1;
	spr[crap2].nohit = 1;
	spr[crap2].seq = myseq;
	if (h > 0)
		spr[crap2].que = spr[h].y + 1;

}

void draw_map_game() {
	RECT                rcRect;
	int pa, cool;


	*pvision = 0;

	while (kill_last_sprite());
	kill_repeat_sounds();
	kill_all_scripts();

	for (int x = 0; x < 96; x++) {
		cool = pam.t[x].num / 128;
		pa = pam.t[x].num - (cool * 128);
		rcRect.left = (pa * 50 - (pa / 12) * 600);
		rcRect.top = (pa / 12) * 50;
		rcRect.right = rcRect.left + 50;
		rcRect.bottom = rcRect.top + 50;


		lpDDSTwo->BltFast((x * 50 - ((x / 12) * 600)) + playl, (x / 12) * 50, tiles[cool + 1],
		                  &rcRect, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT);


	}


	if (strlen(pam.script) > 1) {
		int ms = load_script(pam.script, 0, true);

		if (ms > 0) {
			locate(ms, "main");
			no_running_main = true;
			run_script(ms);
			no_running_main = false;

		}
	}


	place_sprites_game();


	//lets add the sprites hardness to the real hardness, adding it's own uniqueness to our collective.


	//if script for overall screen, run it

	//Msg("Done loading screen.");
	thisTickCount = g_events->getTickCount();

	init_scripts();
}


void place_sprites_game_background() {
	int sprite;

	bool bs[max_sprites_at_once];
	int rank[max_sprites_at_once];
	int highest_sprite;
	memset(&bs, 0, sizeof(bs));
	int hs;
	for (int r1 = 1; r1 < 100; r1++) {
		highest_sprite = 20000; //more than it could ever be

		rank[r1] = 0;

		for (int h1 = 1; h1 < 100;  h1++) {
			if (bs[h1] == false) {
				if (pam.sprite[h1].active) if (pam.sprite[h1].type == 0) {
						if (pam.sprite[h1].que != 0) hs = pam.sprite[h1].que;
						else hs = pam.sprite[h1].y;
						if (hs < highest_sprite) {
							highest_sprite = hs;
							rank[r1] = h1;
						}
					}

			}
		}
		if (rank[r1] != 0)
			bs[rank[r1]] = true;
	}




	int j;

	for (int oo = 1; rank[oo] > 0; oo++) {
		//Msg("Ok, rank[%d] is %d.",oo,rank[oo]);
		j = rank[oo];

		if (pam.sprite[j].active == true) if ((pam.sprite[j].vision == 0) || (pam.sprite[j].vision == *pvision)) {

				check_seq_status(pam.sprite[j].seq);

				//we have instructions to make a sprite
				if (pam.sprite[j].type == 0)

				{
					//make it part of the background (much faster)

					sprite = add_sprite_dumb(pam.sprite[j].x, pam.sprite[j].y, 0,
					                         pam.sprite[j].seq, pam.sprite[j].frame,
					                         pam.sprite[j].size);
					//Msg("Background sprite %d has hard of %d..", j, pam.sprite[j].hard);
					check_sprite_status_full(sprite);
					draw_sprite_game(lpDDSTwo, sprite);
					spr[sprite].active = false;
				}


			}

	}


}


void draw_map_game_background() {
	RECT                rcRect;



	int pa, cool;


	for (int x = 0; x < 96; x++) {
		cool = pam.t[x].num / 128;
		pa = pam.t[x].num - (cool * 128);
		rcRect.left = (pa * 50 - (pa / 12) * 600);
		rcRect.top = (pa / 12) * 50;
		rcRect.right = rcRect.left + 50;
		rcRect.bottom = rcRect.top + 50;


		lpDDSTwo->BltFast((x * 50 - ((x / 12) * 600)) + playl, (x / 12) * 50, tiles[cool + 1],
		                  &rcRect, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT);


	}

	place_sprites_game_background();

}

void fill_back_sprites() {
	int sprite;

	bool bs[max_sprites_at_once];
	int rank[max_sprites_at_once];
	int highest_sprite;


	memset(&bs, 0, sizeof(bs));
	int hs;

	for (int r1 = 1; r1 < 100; r1++) {

		highest_sprite = 20000; //more than it could ever be

		rank[r1] = 0;

		for (int h1 = 1; h1 < 100;  h1++) {
			if (bs[h1] == false) {
				if (pam.sprite[h1].active) if (pam.sprite[h1].type != 1) if (pam.sprite[h1].hard == 0) {
							if (pam.sprite[h1].que != 0) hs = pam.sprite[h1].que;
							else hs = pam.sprite[h1].y;
							if (hs < highest_sprite) {
								highest_sprite = hs;
								rank[r1] = h1;
							}
						}

			}
		}
		if (rank[r1] != 0)
			bs[rank[r1]] = true;
	}




	int j;

	for (int oo = 1; rank[oo] > 0; oo++) {
		//Msg("Ok, rank[%d] is %d.",oo,rank[oo]);
		j = rank[oo];

		if (pam.sprite[j].active == true) if ((pam.sprite[j].vision == 0) || (pam.sprite[j].vision == *pvision)) {
				{
					//make it part of the background (much faster)

					sprite = add_sprite_dumb(pam.sprite[j].x, pam.sprite[j].y, 0,
					                         pam.sprite[j].seq, pam.sprite[j].frame,
					                         pam.sprite[j].size);
					spr[sprite].hard = pam.sprite[j].hard;
					spr[sprite].sp_index = j;
					CopyRect(&spr[sprite].alt, &pam.sprite[j].alt);
					check_sprite_status_full(sprite);
					if (spr[sprite].hard == 0) {
						/*if (pam.sprite[j].prop == 0)
						add_hardness(sprite, 1); else */ add_hardness(sprite, 100 + j);

					}
					spr[sprite].active = false;
				}
			}
	}
}

void fill_screen(int num) {
	uint32 col;

	if (truecolor) {
		lpDDPal->GetEntries(0, 0, 256, pe);
		col = pe[num].peBlue << wBPos | pe[num].peGreen << wGPos | pe[num].peRed << wRPos;
	} else {
		col = num;
	}

	lpDDSTwo->clearTransparentColor();
	lpDDSTwo->fillRect(lpDDSTwo->getBounds(), col);
}


void fill_hard_sprites() {
//    int sprite;
	bool bs[max_sprites_at_once];
	int rank[max_sprites_at_once];
	int highest_sprite;
	int h;
	memset(&bs, 0, sizeof(bs));
//    int hs;

	//Msg("filling sprite hardness...");


	int max_s = last_sprite_created;

	int height;

	for (int r1 = 1; r1 <= max_s; r1++) {

		highest_sprite = 22024; //more than it could ever be
		rank[r1] = 0;
		for (int h1 = 1; h1 < max_s + 1; h1++) {
			if (spr[h1].active) {
				if (bs[h1] == false) {
					//Msg( "Ok,  %d is %d", h1,(spr[h1].y + picInfo[spr[h1].pic].yoffset) );
					if (spr[h1].que != 0) height = spr[h1].que;
					else height = spr[h1].y;
					if (height < highest_sprite) {
						highest_sprite = height;
						rank[r1] = h1;
					}
				}
			}
		}
		if (rank[r1] != 0)
			bs[rank[r1]] = true;
	}



	for (int j = 1; j <= max_s; j++) {
		h = rank[j];
		if (h > 0)
			if (spr[h].active) {
				//          Msg("proccesing sprite %d", h);
				if (spr[h].sp_index != 0) {
					//Msg("has spindex of %d prop is %d",spr[h].sp_index,pam.sprite[spr[h].sp_index].prop);
					if (pam.sprite[spr[h].sp_index].hard == 0) {

						add_hardness(h, 100 + spr[h].sp_index);
						//Msg("added warp hardness for %d", spr[h].sp_index);
					}



				} else {
					if (spr[h].hard == 0) {
						//Msg("adding a new sprite hardness %d (not from editor)", h);
						add_hardness(h, 1);
					}

				}

			}



	}



}

void process_callbacks() {
	int thist = g_events->getTickCount();


	for (int i = 1; i < max_scripts; i++) {
		if (rinfo[i] != NULL) {
			if (rinfo[i]->sprite > 0) if (rinfo[i]->sprite != 1000) if (spr[rinfo[i]->sprite].active == false) {
						//kill this script, owner is dead
						if (debug_mode)
							Msg("Killing script %s, owner sprite %d is dead.", rinfo[i]->name, rinfo[i]->sprite);
						kill_script(i);
						/*free(rinfo[i]);
						rinfo[i] = NULL;
						free(rbuf[i]);
						rbuf[i] = NULL;*/
					}

		}
	}


	for (int k = 1; k < max_callbacks; k++) {
		if (callback[k].active) {

			if (callback[k].owner > 0) if (rinfo[callback[k].owner] == NULL) {
					//kill this process, it's owner sprite is 'effin dead.
					if (debug_mode) Msg("Killed callback %s because script %d is dead.",
						                    k, callback[k].owner);
					callback[k].active = false;

				} else {

					if (callback[k].timer == 0) {
						//set timer

						if (callback[k].max > 0) {
							callback[k].timer = thist + RANDOM(callback[k].min, callback[k].max);
						} else {
							callback[k].timer = thist + callback[k].min;
						}
					} else {

						if ((int)callback[k].timer < thist) {
							callback[k].timer = 0;

							if (compare(callback[k].name, "")) {
								//callback defined no proc name, so lets assume they want to start the script where it
								//left off
								//kill this callback
								callback[k].active = false;
								run_script(callback[k].owner);
								if (debug_mode) Msg("Called script %d with callback %d.", callback[k].owner, k);

							} else {

								if (debug_mode) Msg("Called proc %s with callback %d.", callback[k].name, k);

								//callback defined a proc name
								if (locate(callback[k].owner, callback[k].name)) {
									//found proc, lets run it
									run_script(callback[k].owner);

								}

							}

						}

					}
				}
		}
	}
}

void init_font_colors() {
	//Light Magenta
	font_colors[1].red   = 255;
	font_colors[1].green = 198;
	font_colors[1].blue  = 255;

	//Dark Green
	font_colors[2].red   = 131;
	font_colors[2].green = 181;
	font_colors[2].blue  = 74;

	//Bold Cyan
	font_colors[3].red   = 99;
	font_colors[3].green = 242;
	font_colors[3].blue  = 247;

	//Orange
	font_colors[4].red   = 255;
	font_colors[4].green = 156;
	font_colors[4].blue  = 74;

	//Magenta
	font_colors[5].red   = 222;
	font_colors[5].green = 173;
	font_colors[5].blue  = 255;

	//Brown Orange
	font_colors[6].red   = 244;
	font_colors[6].green = 188;
	font_colors[6].blue  = 73;

	//Light Gray
	font_colors[7].red   = 173;
	font_colors[7].green = 173;
	font_colors[7].blue  = 173;

	//Dark Gray
	font_colors[8].red   = 85;
	font_colors[8].green = 85;
	font_colors[8].blue  = 85;

	//Sky Blue
	font_colors[9].red   = 148;
	font_colors[9].green = 198;
	font_colors[9].blue  = 255;

	//Bright Green
	font_colors[10].red   = 0;
	font_colors[10].green = 255;
	font_colors[10].blue  = 0;

	//Yellow
	font_colors[11].red   = 255;
	font_colors[11].green = 255;
	font_colors[11].blue  = 2;

	//Yellow
	font_colors[12].red   = 255;
	font_colors[12].green = 255;
	font_colors[12].blue  = 2;

	//Hot Pink
	font_colors[13].red   = 255;
	font_colors[13].green = 132;
	font_colors[13].blue  = 132;

	//Yellow
	font_colors[14].red   = 255;
	font_colors[14].green = 255;
	font_colors[14].blue  = 2;

	//White
	font_colors[15].red   = 255;
	font_colors[15].green = 255;
	font_colors[15].blue  = 255;
}

} // namespace Dink
