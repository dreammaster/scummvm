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
#include "dink/file.h"
#include "dink/graphics.h"
#include "dink/sound.h"
#include "dink/text.h"
#include "dink/var.h"
#include "dink/lib/wintypes.h"
#include "dink/directdraw/ddutil.h"

namespace Dink {

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


void flip_it_second() {
	DDBLTFX ddBltFx;
	RECT rcRectSrc;
	RECT rcRectDest;
	POINT p;

	if (!windowed) {

		while (1) {
			ddrval = lpDDSPrimary->Flip(NULL, DDFLIP_WAIT);
			if (ddrval == DD_OK) {
				break;
			}
			if (ddrval == DDERR_SURFACELOST) {
				if (ddrval != DD_OK) {
					break;
				}
			}
			if (ddrval != DDERR_WASSTILLDRAWING) {


			}
		}

	} else {
		//windowed mode, no flipping
		p.x = 0;
		p.y = 0;
		ClientToScreen(hWndMain, &p);
		GetClientRect(hWndMain, &rcRectDest);

		//rcRectDest.top += winoffset;
		rcRectDest.bottom = 480;
		rcRectDest.right = 640;

		OffsetRect(&rcRectDest, p.x, p.y);
		SetRect(&rcRectSrc, 0, 0, 640, 480);

		ddBltFx.dwSize = sizeof(ddBltFx);

		ddBltFx.dwDDFX = DDBLTFX_NOTEARING;
		ddrval = lpDDSPrimary->Blt(&rcRectDest, lpDDSBack,
		                           &rcRectSrc, DDBLT_DDFX | DDBLT_WAIT, &ddBltFx);
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





IDirectDrawSurface *DDTileLoad(IDirectDraw *pdd, LPCSTR szBitmap, int dx, int dy, int sprite) {
	Image::BitmapDecoder decoder;
	File f;

	if (!f.open(szBitmap) || !decoder.loadStream(f))
		return nullptr;

	IDirectDrawSurface *surf = new IDirectDrawSurface();
	surf->copyFrom(decoder.getSurface());

	if (decoder.hasPalette())
		surf->setPalette(decoder.getPalette(), 0, decoder.getPaletteColorCount());

	return surf;
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


void load_map(int num) {
#ifdef TODO
	FILE           *fp;
	long holdme, lsize;
	//RECT box;
	// play.map = num;
	//Msg("Loading map %d...",num);

	fp = fopen(current_map, "rb");
	if (!fp) {
		Msg("Cannot find %s file!!!", current_map);
		return;
	}

	//redink1 set correctly so Dink appears on mini-map for warps and such
	//doesn't work, because 'num' is actually the offset in map.dat, not the map screen number
	//if (map.indoor[num] == 0)
	//   play.last_map = num;

	lsize = sizeof(struct small_map);
	holdme = (lsize * (num - 1));
	fseek(fp, holdme, SEEK_SET);
	//Msg("Trying to read %d bytes with offset of %d",lsize,holdme);
	int shit = fread(&pam, lsize, 1, fp);        /* current player */
	//       Msg("Read %d bytes.",shit);
	if (shit == 0) Msg("ERROR:  Couldn't read map %d?!?", num);
	fclose(fp);

	spr[1].move_active = false;
	spr[1].move_nohard = false;
	spr[1].freeze = false;
	screenlock = 0;
	fill_whole_hard();
	fix_dead_sprites();


	if (!dinkedit)
		check_midi();

	//   draw_map_game();
#else
	error("TODO");
#endif
}

void save_map(const int num) {
#ifdef TODO
	FILE           *fp;
	long holdme, lsize;
	char crap[80];


	Msg("Saving map data..");
	strcpy(crap, current_map);
	if (num > 0) {
		fp = fopen(crap, "r+b");
		lsize = sizeof(struct small_map);
		holdme = (lsize * (num - 1));
		fseek(fp, holdme, SEEK_SET);
		fwrite(&pam, lsize, 1, fp);        /* current player */
		fclose(fp);

	}


	Msg("Done saving map data..");
#else
	error("TODO");
#endif
}




void save_info() {
#ifdef TODO
	FILE           *fp;
	char crap[80];
	//redink1
	sprintf(crap, current_dat);
	fp = fopen(crap, "wb");
	fwrite(&map, sizeof(struct map_info), 1, fp);
	fclose(fp);
#else
	error("TODO");
#endif
}

void save_game(int num) {
#ifdef TODO
	FILE           *fp;
	char crap[80];
	//redink1 created this
	char info_temp[200];

	//lets set some vars first

	play.x = spr[1].x;
	play.y = spr[1].y;
	play.version = dversion;
	play.pseq =  spr[1].pseq;
	play.pframe =    spr[1].pframe;
	play.seq    =    spr[1].seq;
	play.frame  =    spr[1].frame;
	play.size   =    spr[1].size;
	play.dir    =    spr[1].dir;
	play.strength = spr[1].strength;
	play.defense  =  spr[1].defense;
	play.que  =  spr[1].que;
	time_t ct;

	time(&ct);
	play.minutes += (difftime(ct, time_start) / 60);
	//reset timer
	time(&time_start);

	play.base_idle = spr[1].base_idle;
	play.base_walk = spr[1].base_walk;
	play.base_hit = spr[1].base_hit;

	//redink1 - save game things for storing new map, palette, and tile information
	strncpy(play.mapdat, current_map, 50);
	strncpy(play.dinkdat, current_dat, 50);

	//redink1 code for custom save game names
	strcpy(info_temp, save_game_info);
	decipher_string(info_temp, 0);
	strncpy(play.gameinfo, info_temp, 77);
	//sprintf(play.gameinfo, "Level %d",*plevel);


	last_saved_game = num;
	sprintf(crap, "SAVE%d.DAT", num);
	fp = fopen(crap, "wb");
	fwrite(&play, sizeof(play), 1, fp);
	fclose(fp);
#else
	error("TODO");
#endif
}


void kill_all_vars() {
	FillMemory(&play, sizeof(play), 0);
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

bool add_time_to_saved_game(int num) {
#ifdef TODO
	FILE           *fp;
	char crap[80];

	sprintf(crap, "SAVE%d.DAT", num);

	fp = fopen(crap, "rb");
	if (!fp) {
		Msg("Couldn't load save game %d", num);
		return false;
	} else {

		fread(&play, sizeof(play), 1, fp);
		fclose(fp);


	}


	//great, now let's resave it with added time
	Msg("Ok, adding time.");
	time_t ct;

	time(&ct);
	play.minutes += (difftime(ct, time_start) / 60);



	sprintf(crap, "SAVE%d.DAT", num);
	fp = fopen(crap, "wb");
	if (fp) {
		fwrite(&play, sizeof(play), 1, fp);
		fclose(fp);
	}
	Msg("Wrote it.(%d of time)", play.minutes);

	return true;
#else
	error("TODO");
#endif
}




bool load_game(int num) {
#ifdef TODO
	FILE           *fp;
	char crap[80];

	//lets get rid of our magic and weapon scripts
	if (weapon_script != 0)

	{

		if (locate(weapon_script, "DISARM")) {
			run_script(weapon_script);
		} else {
		}

	}
	if (magic_script != 0) if (locate(magic_script, "DISARM")) run_script(magic_script);





	bow.active = false;
	weapon_script = 0;
	magic_script = 0;
	midi_active = true;


	if (last_saved_game > 0) {
		Msg("Modifying saved game.");

		if (!add_time_to_saved_game(last_saved_game))
			Msg("Error modifying saved game.");
	}
	StopMidi();

	sprintf(crap, "SAVE%d.DAT", num);



	fp = fopen(crap, "rb");
	if (!fp) {
		Msg("Couldn't load save game %d", num);
		return false;
	} else {

		fread(&play, sizeof(play), 1, fp);
		fclose(fp);

		//redink1 - new map, if exist
		if (strlen(play.mapdat) > 0 && strlen(play.dinkdat) > 0) {
			strcpy(current_map, play.mapdat);
			strcpy(current_dat, play.dinkdat);
			load_info();
		}
		//redink1 - load palette
		if (strlen(play.palette) > 0) {
			LPDIRECTDRAWSURFACE oldTrick = DDLoadBitmap(lpDD, play.palette, 0, 0);
			lpDDPal = DDLoadPalette(lpDD, play.palette);
			if (lpDDPal) {
				lpDDSPrimary->SetPalette(lpDDPal);
				lpDDPal->GetEntries(0, 0, 256, pe);
				lpDDPal->GetEntries(0, 0, 256, real_pal);
			}
			oldTrick->Release();
		}
		//redink1 - reload tiles
		char tile[50];
		for (int i = 1; i <= 41; i++) {
			//Need to unload old tiles...
			tiles[i]->Release();

			//Which tiles are we loading, new or default?
			if (strlen(play.tile[i].file) > 0) {
				//Check the original directory
				if (!File::exists(play.tile[i].file))
					sprintf(tile, "..\\DINK\\%s", play.tile[i].file);
				else
					strcpy(tile, play.tile[i].file);
			} else {
				sprintf(tile, "tiles\\TS%02d.bmp", i);
				if (!File::exists(tile))
					sprintf(tile, "..\\dink\\tiles\\TS%02d.BMP", i);
			}

			//Load in the tiles...
			tiles[i] = DDTileLoad(lpDD, tile, 0, 0, i);
			DDSetColorKey(tiles[i], RGB(0, 0, 0));
		}

		spr[1].damage = 0;
		spr[1].x = play.x;
		spr[1].y = play.y;
		walk_off_screen = 0;
		spr[1].nodraw = 0;
		push_active = 1;
		spr[1].pseq = play.pseq;
		spr[1].pframe = play.pframe;
		spr[1].size = play.size;
		spr[1].seq = play.seq;
		spr[1].frame = play.frame;
		spr[1].dir = play.dir;
		spr[1].strength = play.strength;
		spr[1].defense = play.defense;
		spr[1].que = play.que;

		time(&time_start);

		spr[1].base_idle = play.base_idle;
		spr[1].base_walk = play.base_walk;
		spr[1].base_hit = play.base_hit;

		int script = load_script("main", 0, true);
		locate(script, "main");
		run_script(script);
		//lets attach our vars to the scripts

		attach();
		Msg("Attached vars.");

		//redink1 fixes
		dinkspeed = 3;

		if (*pcur_weapon != 0) {
			if (play.item[*pcur_weapon].active == false) {
				*pcur_weapon = 1;
				weapon_script = 0;
				Msg("Loadgame error: Player doesn't have armed weapon - changed to 1.");
			} else {

				weapon_script = load_script(play.item[*pcur_weapon].name, 1000, false);


				if (locate(weapon_script, "DISARM")) run_script(weapon_script);

				weapon_script = load_script(play.item[*pcur_weapon].name, 1000, false);


				if (locate(weapon_script, "ARM")) run_script(weapon_script);
			}
		}
		if (*pcur_magic != 0) {
			if (play.item[*pcur_magic].active == false) {
				*pcur_magic = 0;
				magic_script = 0;
				Msg("Loadgame error: Player doesn't have armed magic - changed to 0.");
			} else {

				magic_script = load_script(play.mitem[*pcur_magic].name, 1000, false);
				if (locate(magic_script, "DISARM")) run_script(magic_script);
				magic_script = load_script(play.mitem[*pcur_magic].name, 1000, false);
				if (locate(magic_script, "ARM")) run_script(magic_script);
			}
		}
		kill_repeat_sounds_all();
		load_map(map.loc[*pmap]);
		Msg("Loaded map.");
		draw_map_game();
		Msg("Map drawn.");

		//redink1 fixes
		fexp = *pexper;
		draw_status_all();
		Msg("Status drawn.");

		last_saved_game = num;

		return true;
	}
#else
	error("TODO");
#endif
}

void kill_cur_item() {

	if (*pcur_weapon != 0) {
		if (play.item[*pcur_weapon].active == true) {

			if (weapon_script != 0) if (locate(weapon_script, "DISARM")) run_script(weapon_script);
			weapon_script = load_script(play.item[*pcur_weapon].name, 0, false);
			play.item[*pcur_weapon].active = false;
			*pcur_weapon = 0;
			if (weapon_script != 0) if (locate(weapon_script, "HOLDINGDROP")) run_script(weapon_script);

			if (weapon_script != 0) if (locate(weapon_script, "DROP")) run_script(weapon_script);
			weapon_script = 0;
		} else {
			Msg("Error:  Can't kill cur item, none armed.");
		}
	}
}



void kill_cur_item_script(char name[20]) {
	int select = 0;
	for (int i = 1; i < 17; i++) {
		if (play.item[i].active)
			if (compare(play.item[i].name, name)) {
				select = i;
				goto found;

			}

	}


	return;

found:

	if (*pcur_weapon == select)

	{
		//holding it right now
		if (locate(weapon_script, "HOLDINGDROP")) run_script(weapon_script);
		if (locate(weapon_script, "DISARM")) run_script(weapon_script);


		*pcur_weapon = 0;
		weapon_script = 0;
	}

	int script = load_script(play.item[select].name, 0, false);
	play.item[select].active = false;



	if (locate(script, "DROP")) run_script(script);

	draw_status_all();

}


void kill_cur_magic_script(char name[20]) {
	int select = 0;
	for (int i = 1; i < 9; i++) {
		if (play.mitem[i].active)
			if (compare(play.mitem[i].name, name)) {
				select = i;
				goto found;

			}
	}


	return;

found:

	if (*pcur_magic == select)

	{
		//holding it right now
		if (locate(magic_script, "HOLDINGDROP")) run_script(magic_script);
		if (locate(magic_script, "DISARM")) run_script(magic_script);


		*pcur_weapon = 0;
		magic_script = 0;
	}

	int script = load_script(play.mitem[select].name, 0, false);
	play.mitem[select].active = false;


	if (locate(script, "DROP")) run_script(script);

	draw_status_all();

}




void kill_cur_magic() {

	if (*pcur_magic != 0) {
		if (play.mitem[*pcur_magic].active == true) {

			if (magic_script != 0) if (locate(magic_script, "DISARM")) run_script(magic_script);
			magic_script = load_script(play.mitem[*pcur_magic].name, 0, false);
			play.mitem[*pcur_magic].active = false;
			*pcur_magic = 0;

			if (magic_script != 0) if (locate(magic_script, "HOLDINGDROP")) run_script(magic_script);
			if (magic_script != 0) if (locate(magic_script, "DROP")) run_script(magic_script);
			magic_script = 0;
		} else {
			Msg("Error:  Can't kill cur magic, none armed.");
		}
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
	sprintf(crap, "SAVE%d.DAT", num);


	fp = fopen(crap, "rb");
	if (!fp) {
		Msg("Couldn't quickload save game %d", num);
		return false;
	} else {

		fread(&short_play, sizeof(player_short_info), 1, fp);
		fclose(fp);
		*mytime = short_play.minutes;
		strcpy(line, short_play.gameinfo);
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
#ifdef TODO
	int work;

	HFASTFILE                  pfile;
	BITMAPFILEHEADER UNALIGNED *pbf;
	BITMAPINFOHEADER UNALIGNED *pbi;
	DDSURFACEDESC       ddsd;
	HBITMAP             hbm;
	BITMAP              bm;

	DDCOLORKEY          ddck;


	int x, y, dib_pitch;
	BYTE *src, *dst;
	uint32 ddst;
	char fname[20];
	LPTSTR dump;

	//IDirectDrawSurface *pdds;

	int sprite = 71;
	bool                       trans = false;
	bool reload = false;

	PALETTEENTRY    holdpal[256];
	char crap[200], hold[5];

	int save_cur = cur_sprite;

	if (index[nummy].last != 0) {
		//  Msg("Saving sprite %d", save_cur);
		cur_sprite = index[nummy].s + 1;
		//Msg("Temp cur_sprite is %d", cur_sprite);
		reload = true;
	}


	index[nummy].s = cur_sprite - 1;

	if (no_running_main) draw_wait();

	char crap2[200];
	strcpy(crap2, org);
	while (crap2[strlen(crap2) - 1] != '\\') {
		crap2[strlen(crap2) - 1] = 0;
	}
	crap2[strlen(crap2) - 1] = 0;

	int num = strlen(org) - strlen(crap2) - 1;
	strcpy(fname, &org[strlen(org) - num]);
	if (samedir)
		sprintf(crap, "%s\\dir.ff", crap2);
	else
		sprintf(crap, "..\\dink\\%s\\dir.ff", crap2);


	if (!FastFileInit((LPSTR)crap, 5)) {
		Msg("Could not load dir.ff art file %s err=%08lX", crap, GetLastError());

		cur_sprite = save_cur;
		return;

	}



	/*      if (!windowed)
	{
	lpDDPal->GetEntries(0,0,256,holdpal);
	lpDDPal->SetEntries(0,0,256,real_pal);
	}
	*/


	for (int oo = 1; oo <= 51; oo++) {


		//load sprite
		char dumb[100];
		sprite = cur_sprite;
		//if (reload) Msg("Ok, programming sprite %d", sprite);
		if (oo < 10) strcpy(crap2, "0");
		else strcpy(crap2, "");
		wsprintf(crap, "%s%s%d.bmp", fname, crap2, oo);

		pfile = FastFileOpen((LPSTR) crap);


		if (pfile == NULL) {
			FastFileClose(pfile);
			//  FastFileFini();
			if (oo == 1)
				Msg("Sprite_load_pak error:  Couldn't load %s.", crap);

			index[nummy].last = (oo - 1);
			//     initFail(hWndMain, crap);
			setup_anim(nummy, nummy, speed);
			//              if (!windowed)  lpDDPal->SetEntries(0,0,256,holdpal);

			//if (reload) Msg("Ok, tacking %d back on.", save_cur);
			cur_sprite = save_cur;
			return;


		} else {

			//got file

			pbf = (BITMAPFILEHEADER *)FastFileLock(pfile, 0, 0);
			pbi = (BITMAPINFOHEADER *)(pbf + 1);

			if (pbf->bfType != 0x4d42 ||
			        pbi->biSize != sizeof(BITMAPINFOHEADER)) {
				Msg("Failed to load");
				Msg(crap);
				cur_sprite = save_cur;
				FastFileClose(pfile);
				//  FastFileFini();


				return;

			}

			//redink1 modified this to work with every color depth
			//Ok, what we do here is take each color, shift it right to get rid of any
			//lower-order bits that we don't use, then shift it left to go to the correct
			//position.  Should work fine with 16-24-32 bit color depth.
#define _RGBXBIT(r, g, b) ( (r >> (8 - wRBits) << wRPos) | (g >> (8 - wGBits) << wGPos) | (b >> (8 - wBBits) << wBPos) )
			PALETTEENTRY ape[256];
			uint32 dwBlack;
			uint32 dwNearBlack;
			uint32 dwWhite;
			uint32 dwNearWhite;

			if (truecolor) {
				if (dinkpal) {
					memcpy(ape, real_pal, sizeof(PALETTEENTRY) * 256);
				} else {
					byte *palzor = (byte *)pbf + 54;
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

			byte *pic;

			pic = (byte *)pbf + 1078;

			//Msg("Pic's size is now %d.",sizeof(pic));

			//redink1 THIS DOESN'T DO ANYTHING!!!! ARGHAGHARHA!
			/*bm.bmWidth = pbi->biWidth;
			bm.bmHeight = pbi->biHeight;
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
			ddsd.dwWidth = pbi->biWidth;
			ddsd.dwHeight = pbi->biHeight; // + 1; //redink1 fixed crashing in 16-bit mode... would overflow by a byte or two by writing dwords, really weird bug. Eh, actually moved solution to everywhere we update a uint32, include any trailing bits that were there before.  Saves memory.

			if (picInfo[sprite].k != NULL) picInfo[sprite].k->Release();

			if (lpDD->CreateSurface(&ddsd, &picInfo[sprite].k, NULL) != DD_OK) {
				Msg("Failed to create pdd surface description");


			} else {


				ddsd.dwSize = sizeof(ddsd);
				ddrval = IDirectDrawSurface_Lock(
				             picInfo[sprite].k, NULL, &ddsd, DDLOCK_WAIT, NULL);

				if (ddrval == DD_OK) {
					dib_pitch = (pbi->biWidth + 3) & ~3;
					src = (BYTE *)pic + dib_pitch * (pbi->biHeight - 1);
					dst = (BYTE *)ddsd.lpSurface;
					ddst = (uint32)ddsd.lpSurface;
					int bytesPerPixel = ddsd.ddpfPixelFormat.dwRGBBitCount >> 3; //Divide by 8, basically
					uint32 dwPixel;
					if (leftalign) {
						//Msg("left aligning..");

						for (y = 0; y < (int)pbi->biHeight; y++) {
							for (x = 0; x < (int)pbi->biWidth; x++) {
								if (truecolor) {
									dwPixel = _RGBXBIT(ape[src[x]].peRed, ape[src[x]].peGreen, ape[src[x]].peBlue);
									if (dwPixel == dwBlack) {
										// Msg("Found a 255...");
										dwPixel = dwNearBlack;
									} else if (dwPixel == dwWhite) {
										dwPixel = dwNearWhite;
									}
									//Make sure to or it with the rest of the uint32 present, just in case it overflows the buffer
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
							((uint32)ddst) += ddsd.lPitch - ((int)pbi->biWidth) * bytesPerPixel; //ddst += ddsd.lPitch / 4;
							dst += ddsd.lPitch;
							src -= dib_pitch;
						}


					} else

						if (black) {

							for (y = 0; y < (int)pbi->biHeight; y++) {
								for (x = 0; x < (int)pbi->biWidth; x++) {
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
								((uint32)ddst) += ddsd.lPitch - ((int)pbi->biWidth) * bytesPerPixel; //
								dst += ddsd.lPitch;
								src -= dib_pitch;
							}

						} else {

							//doing white
							for (y = 0; y < (int)pbi->biHeight; y++) {
								for (x = 0; x < (int)pbi->biWidth; x++) {
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
								((uint32)ddst) += ddsd.lPitch - ((int)pbi->biWidth) * bytesPerPixel; //ddst += ddsd.lPitch / 4;
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

					if (black)

					{
						ddck.dwColorSpaceLowValue  = DDColorMatch(picInfo[cur_sprite].k, RGB(255, 255, 255));

						ddck.dwColorSpaceHighValue = ddck.dwColorSpaceLowValue;
						picInfo[cur_sprite].k->SetColorKey(DDCKEY_SRCBLT, &ddck);

					}

					else

					{
						ddck.dwColorSpaceLowValue  = DDColorMatch(picInfo[cur_sprite].k, RGB(0, 0, 0));
						ddck.dwColorSpaceHighValue = ddck.dwColorSpaceLowValue;
						picInfo[cur_sprite].k->SetColorKey(DDCKEY_SRCBLT, &ddck);

					}
					cur_sprite++;
					if (!reload)
						save_cur++;
					FastFileClose(pfile);
				}

			}


		}

	}


	//  FastFileFini();


	return;

#else
	error("TODO");
#endif
}



void load_sprites(char org[100], int nummy, int speed, int xoffset, int yoffset,
                  RECT hardbox, bool notanim, bool black, bool leftalign) {
	int work;
	PALETTEENTRY    holdpal[256];
	char crap[200];

	if (no_running_main)
		draw_wait();

	char crap2[200];
	strcpy(crap2, org);
	while (crap2[strlen(crap2) - 1] != '\\') {
		crap2[strlen(crap2) - 1] = 0;
	}
	crap2[strlen(crap2) - 1] = 0;

	sprintf(crap, "%s\\dir.ff", crap2);
	//Msg("Checking for %s..", crap);
	if (File::exists(crap)) {
		load_sprite_pak(org, nummy, speed, xoffset, yoffset, hardbox, notanim, black, leftalign, true);
		return;
	}

	sprintf(crap, "%s01.bmp", org);
	if (!File::exists(crap)) {
		sprintf(crap, "..\\dink\\%s\\dir.ff", crap2);
		//Msg("Checking for %s..", crap);
		if (File::exists(crap)) {
			load_sprite_pak(org, nummy, speed, xoffset, yoffset, hardbox, notanim, black, leftalign, false);
			return;
		}

		//  Msg("Dir bad for sprite, changing");
		sprintf(crap, "..\\dink\\%s", org);
		strcpy(org, crap);
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
		sprintf(crap, "%s%.2d.BMP", org, oo);
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
		strcpy(seq[atol(ev[3])].data, line);
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
		strcpy(seq[atol(ev[3])].data, line);
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
		strcpy(seq[atol(ev[3])].data, line);
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
	strcpy(final, "");
	strcpy(nums, ltoa(fexp, buffer, 10));
	if (strlen(nums) < 5)
		for (int i = 1; i < (6 - (int)strlen(nums)); i++)
			strcat(final, "0");
	strcat(final, nums);
	strcat(final, "/");

	strcpy(nums, ltoa(fraise, buffer, 10));
	if (strlen(nums) < 5)
		for (int i = 1; i < (6 - (int)strlen(nums)); i++)
			strcat(final, "0");
	strcat(final, nums);
	draw_num(181, final, 404, 459);

}


void draw_strength() {
	char final[30];
	char buffer[30];
	char nums[30];
	//Msg("Drawing exp.. which is %d and %d",fexp, *pexp);
	strcpy(final, "");

	strcpy(nums, ltoa(fstrength, buffer, 10));
	if (strlen(nums) < 3)
		for (int i = 1; i < (4 - (int)strlen(nums)); i++)
			strcat(final, "0");
	strcat(final, nums);
	//Msg("Drawing %s..",final);
	draw_num(182, final, 81, 415);
}


void draw_defense() {
	char final[30];
	char buffer[30];
	char nums[30];
	//Msg("Drawing exp.. which is %d and %d",fexp, *pexp);
	strcpy(final, "");
	strcpy(nums, ltoa(fdefense, buffer, 10));
	if (strlen(nums) < 3)
		for (int i = 1; i < (4 - (int)strlen(nums)); i++)
			strcat(final, "0");
	strcat(final, nums);
	draw_num(183, final, 81, 437);
}


void draw_magic() {
	char final[30];
	char buffer[30];
	char nums[30];
	//Msg("Drawing exp.. which is %d and %d",fexp, *pexp);
	strcpy(final, "");
	strcpy(nums, ltoa(fmagic, buffer, 10));
	if (strlen(nums) < 3)
		for (int i = 1; i < (4 - (int)strlen(nums)); i++)
			strcat(final, "0");
	strcat(final, nums);
	draw_num(184, final, 81, 459);
}


void draw_level() {
	char final[30];
	char buffer[30];
	//char nums[30];
	//*plevel = 15;
	//Msg("Drawing level.. which is %d ",*plevel);
	strcpy(final, ltoa(*plevel, buffer, 10));

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
	strcpy(final, "");
	strcpy(nums, ltoa(fgold, buffer, 10));
	if (strlen(nums) < 5)
		for (int i = 1; i < (6 - (int)strlen(nums)); i++)
			strcat(final, "0");
	strcat(final, nums);
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


			if (n < 10) strcpy(hold, "0");
			else strcpy(hold, "");
			sprintf(crap, "%s%s%d.BMP", name, hold, n);

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

void strchar(char *string, char ch)
/* This acts in the same way as strcat except it combines a string and
a single character, updating the null at the end. */
{
	int last;
	last = strlen(string);
	string[last] = ch;
	string[last + 1] = 0;

}


void kill_callbacks_owned_by_script(int script) {
	for (int i = 1; i < max_callbacks; i++) {
		if (callback[i].owner == script) {
			if (debug_mode) Msg("Kill_all_callbacks just killed %d for script %d", i, script);
			//killed callback
			callback[i].active = false;
		}
	}


}

void kill_script(int num) {
	if (rinfo[num] != NULL) {

		kill_callbacks_owned_by_script(num);

		//now lets kill all local vars assocated with this script

		for (int i = 1; i < max_vars; i++) {
			if (play.var[i].active) if (play.var[i].scope == num) {
					play.var[i].active = false;
				}

		}
		if (debug_mode) Msg("Killed script %s. (num %d)", rinfo[num]->name, num);


		free(rinfo[num]);
		rinfo[num] = NULL;
		free(rbuf[num]);
		rbuf[num] = NULL;
	}
}




void kill_all_scripts() {
	int ii;

	for (ii = 1; ii < max_scripts; ii++) {

		if (rinfo[ii] != NULL) if (rinfo[ii]->sprite != 1000)
				kill_script(ii);
	}

	for (ii = 1; ii <= max_callbacks; ii++) {
		if (callback[ii].active) {
			if ((rinfo[callback[ii].owner] != NULL) && (rinfo[callback[ii].owner]->sprite == 1000)) {

			} else {
				if (debug_mode) Msg("Killed callback %d.  (was attached to script %d.)", ii, callback[ii].owner);
				callback[ii].active = 0;
			}
		}
	}
}

void kill_all_scripts_for_real() {
	int ii;

	for (ii = 1; ii < max_scripts; ii++) {

		if (rinfo[ii] != NULL)
			kill_script(ii);
	}

	for (ii = 1; ii <= max_callbacks; ii++) {

		callback[ii].active = 0;
	}
}


bool read_next_line(int script, char *line) {
	if ((rinfo[script] == NULL) || (rbuf == NULL)) {

		Msg("  ERROR:  Tried to read script %d, it doesn't exist.", script);
		return false;
	}

	if (rinfo[script]->current >= rinfo[script]->end) {
		//at end of buffer
		return false;
	}

	/*      if (rinfo[script]->current < -1);
	{
	//something errored out, why did it go negetive?
	return false;
	}
	*/

	strcpy(line, "");

	for (int ii = rinfo[script]->current; (ii < rinfo[script]->end); ii++) {


		//      Msg("..%d",ii);
		strchar(line, rbuf[script][ii]);
		rinfo[script]->current++;

		if ((rbuf[script][ii] == '\n') || (rbuf[script][ii] == '\r')) {
			return true;
		}

		if (rinfo[script]->current >= rinfo[script]->end) return false;

	}

	return false;
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
	sprintf(tab, "%c", 9);

	sprintf(temp, "story\\%s.d", filename);

	if (!File::exists(temp)) {
		sprintf(temp, "story\\%s.c", filename);
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

	strcpy(rinfo[script]->name, filename);
	rinfo[script]->sprite = sprite;

	if (set_sprite) {
		if (sprite != 0) if (sprite != 1000)
			spr[sprite].script = script;
	}

	return script;
}


void strip_beginning_spaces(char *s) {


	char *h;
	h = s;


	if (s[0] != 32) {
		return;
	}

	while (h[0] == 32) {
		h = &h[1];
	}


	strcpy(s, h);

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

bool locate_goto(char proc[50], int script) {
	rinfo[script]->current = 0;
	char line[200];
	char ev[3][100];
//    char temp[100];
	replace(";", "", proc);
	strchar(proc, ':');
	//  Msg("locate is looking for %s", proc);

	while (read_next_line(script, line)) {
		strip_beginning_spaces(line);

		get_word(line, 1, ev[1]);
		replace("\n", "", ev[1]);

		if (compare(ev[1], proc)) {
			if (debug_mode) Msg("Found goto : Line is %s, word is %s.", line, ev[1]);

			rinfo[script]->skipnext = false;
			rinfo[script]->onlevel = 0;
			rinfo[script]->level = 0;


			return true;
			//this is desired proc
		}

	}
	Msg("ERROR:  Cannot goto %s in %s.", proc, rinfo[script]->name);
	return false;

}



/*void check_for_real_vars(char crap[20], int i)
{
if (compare(play.var[i].name, "&vision")) map_vision = play.var[i].var;
if (compare(play.var[i].name, "&life")) map_vision = play.var[i].var;
if (compare(play.var[i].name, "&exp")) map_vision = play.var[i].var;

}
*/

void decipher(char *crap, int script) {

	if (compare(crap, "&current_sprite")) {
		sprintf(crap, "%d", rinfo[script]->sprite);
		//Msg("cur sprite returning %s, ",crap);
		return;
	}

	if (compare(crap, "&current_script")) {
		sprintf(crap, "%d", script);
		return;
	}

	//v1.08 special variables.
	if (compare(crap, "&return")) {
		sprintf(crap, "%d", returnint);
		return;
	}

	if (compare(crap, "&arg1")) {
		sprintf(crap, "%d", rinfo[script]->arg1);
		return;
	}

	if (compare(crap, "&arg2")) {
		sprintf(crap, "%d", rinfo[script]->arg2);
		return;
	}

	if (compare(crap, "&arg3")) {
		sprintf(crap, "%d", rinfo[script]->arg3);
		return;
	}

	if (compare(crap, "&arg4")) {
		sprintf(crap, "%d", rinfo[script]->arg4);
		return;
	}

	if (compare(crap, "&arg5")) {
		sprintf(crap, "%d", rinfo[script]->arg5);
		return;
	}

	if (compare(crap, "&arg6")) {
		sprintf(crap, "%d", rinfo[script]->arg6);
		return;
	}

	if (compare(crap, "&arg7")) {
		sprintf(crap, "%d", rinfo[script]->arg7);
		return;
	}

	if (compare(crap, "&arg8")) {
		sprintf(crap, "%d", rinfo[script]->arg8);
		return;
	}

	if (compare(crap, "&arg9")) {
		sprintf(crap, "%d", rinfo[script]->arg9);
		return;
	}

	for (int i = 1; i < max_vars; i ++) {


		if (play.var[i].active == true) if (i == get_var(script, play.var[i].name))   //redink1 changed for recursive scoping
				if (compare(play.var[i].name, crap)) {


					sprintf(crap, "%d", play.var[i].var);
					//        check_for_real_vars(crap, i);

					return;
				}

	}


}

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
				sprintf(crap, "%d", play.var[i].var);
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
	sprintf(crap, "%d", play.var[i].var);
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
			sprintf(crap, "save%d.dat", decipher_savegame);
			if (File::exists(crap)) {
				load_game_small(decipher_savegame, crab, &mytime);
				//redink1 fix for savegame time bug
				sprintf(line, "Slot %d - %d:%02d - %s", decipher_savegame, (mytime / 60), mytime - ((mytime / 60) * 60), crab);
				//sprintf(line, "In Use");
			} else {

#ifdef __GERMAN
				sprintf(line, "Slot %d - Ungebraucht", decipher_savegame);
#endif

#ifdef __ENGLISH
				sprintf(line, "Slot %d - Empty", decipher_savegame);
#endif


			}

		}


}



bool get_parms(char proc_name[20], int script, char *h, const int p[10]) {
	memset(nlist, 0, 10 * sizeof(int));

	char crap[100];

	strip_beginning_spaces(h);
	if (h[0] == '(') {
		//Msg("Found first (.");
		h = &h[1];

	} else {
		Msg("Missing ( in %s, offset %d.", rinfo[script]->name, rinfo[script]->current);


		return false;
	}



	for (int i = 0; i < 10; i++) {

		strip_beginning_spaces(h);


		if (p[i] == 1) {
			// Msg("Checking for number..");


			if (strchr(h, ',') != NULL)
				seperate_string(h, 1, ',', crap);
			else if (strchr(h, ')') != NULL)
				seperate_string(h, 1, ')', crap);


			h = &h[strlen(crap)];


			if (crap[0] == '&') {
				replace(" ", "", crap);
				//  Msg("Found %s, 1st is %c",crap, crap[0]);
				decipher(crap, script);


			}

			nlist[i] = atol(crap);

		} else

			if (p[i] == 2) {
				// Msg("Checking for string..");
				seperate_string(h, 2, '"', crap);
				h = &h[strlen(crap) + 2];

				//Msg("Found %s",crap);
				strcpy(slist[i], crap);

			}


		if (p[i + 1] == 0) {
			//finish
			strip_beginning_spaces(h);

			if (h[0] == ')') {
				h = &h[1];
			} else {

				Msg("Missing ) in %s, offset %d.", rinfo[script]->name, rinfo[script]->current);
				h = &h[1];

				return false;
			}

			strip_beginning_spaces(h);

			if (h[0] == ';') {
				//  Msg("Found ending ;");
				h = &h[1];

			} else {
				//Msg("Missing ; in %s, offset %d.", rinfo[script]->name, rinfo[script]->current);
				//  h = &h[1];

				return true;
			}





			return true;
		}


		//got a parm, but there is more to get, lets make sure there is a comma there
		strip_beginning_spaces(h);

		if (h[0] == ',') {
			//     Msg("Found expected ,");
			h = &h[1];

		} else {
			Msg("Procedure %s does not take %d parms in %s, offset %d. (%s?)", proc_name, i + 1, rinfo[script]->name, rinfo[script]->current, h);

			return false;
		}

	}


	return true;
}


int add_callback(const char *name, int n1, int n2, int script) {


	for (int ii = 1; ii < max_callbacks; ii++) {
		if (callback[ii].active == false) {
			memset(&callback[ii], 0, sizeof(callback[ii]));

			callback[ii].active = true;
			callback[ii].min = n1;
			callback[ii].max = n2;
			callback[ii].owner = script;
			strcpy(callback[ii].name, name);

			if (debug_mode) Msg("Callback added to %d.", ii);
			return ii;
		}

	}

	Msg("Couldn't add callback, all out of space");

	return 0;

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
	strcpy(spr[crap2].text, text);
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
	strcpy(spr[crap2].text, text);
	spr[crap2].kill = strlen(text) * text_timer;
	if (spr[crap2].kill < text_min) spr[crap2].kill = text_min;
	spr[crap2].damage = -1;
	spr[crap2].nohit = 1;
	spr[crap2].owner = 1000;
	spr[crap2].hard = 1;
	spr[crap2].script = script;
	return crap2;
}



int does_sprite_have_text(int sprite) {
	//Msg("getting callback # with %d..", sprite);
	for (int k = 1; k <= max_sprites_at_once; k++) {
		if (spr[k].active) if (spr[k].owner == sprite) if (spr[k].brain == 8) {
					//Msg("Found it!  returning %d.", k);

					return k;
				}

	}

	return 0;

}

int var_exists(char name[20], int scope) {

	for (int i = 1; i < max_vars; i++) {
		if (play.var[i].active) {
			if (compare(play.var[i].name, name)) {

				if (play.var[i].scope == scope) { //redink1 changed to check recursively... then changed back.  Hrm.
					//Msg("Found match for %s.", name);
					return i;
				}
			}



		}
	}

	return 0;
}

//redink1 added this to make new global functions
void make_function(char file[10], char func[20]) {
	//See if it already exists
	bool exists = false;
	int i;
	for (i = 0; strlen(play.func[i].func) > 0 && i < 100; i++) {
		if (compare(func, play.func[i].func)) {
			exists = true;
			break;
		}
	}

	if (exists) {
		strncpy(play.func[i].file, file, 10);
	} else {
		strncpy(play.func[0].file, file, 10);
		strncpy(play.func[0].func, func, 20);
	}
}

void make_int(char name[80], int value, int scope, int script) {
	int dupe;
	if (strlen(name) > 19) {

		Msg("ERROR:  Varname %s is too long in script %s.", name, rinfo[script]->name);
		return;
	}
	dupe = var_exists(name, scope);

	if (dupe > 0) {
		if (scope != 0) {
			Msg("Local var %s already used in this procedure in script %s.", name, rinfo[script]->name);

			play.var[dupe].var = value;

		} else
			Msg("Var %s is already a global, not changing value.", name);

		return;
	}


	//make new var

	for (int i = 1; i < max_vars; i++) {
		if (play.var[i].active == false) {

			play.var[i].active = true;
			play.var[i].scope = scope;
			strcpy(play.var[i].name, name);
			//g("var %s created, used slot %d ", name,i);
			play.var[i].var = value;
			return;
		}
	}


	Msg("ERROR: Out of var space, all %d used.", max_vars);
}

int var_equals(char name[20], char newname[20], char math_, int script, char rest[200]) {
	int k;
	//redink1 set newret to NULL so debug errors did not appear.
	int newret = NULL; // = NULL;

	if (name[0] != '&') {
		Msg("ERROR (var equals): Unknown var %s in %s offset %d.", name, rinfo[script]->name, rinfo[script]->current);
		return 0;
	}

	int i = get_var(script, name);
	if (i > 0) {
		goto next;
	}

	/*for (int i = 1; i < max_vars; i++)
	{
	if  (play.var[i].active == true)
	{
	if (i == 71)
	{
	i = 71;
	}
	if ( (play.var[i].scope == 0  || recurse_scope(i, script)) && compare(name, play.var[i].name) ) //redink1 changed to check recursive scope
	//found var
	goto next;



	}
	}*/





	Msg("ERROR: (var equals2) Unknown var %s in %s offset %d.", name, rinfo[script]->name, rinfo[script]->current);
	return 0;

next:


	int newval = 0;



	if (strchr(rest, '(') != NULL)

	{
		newret = process_line(script, rest, false);
		newval = returnint;
		goto next2;
	}


	if (strchr(newname, ';') != NULL) replace(";", "", newname);

	//redink1 fixed for scope and such
	k = get_var(script, newname);
	if (k > 0) {
		newval = play.var[k].var;
		goto next2;
	}

	/*for (k = 1; k < max_vars; k++)
	{
	if (play.var[k].active == true)
	{
	if ( (play.var[k].scope == 0) || recurse_scope(k, script) ) //redink1 changed i to k, made it so can set the parent proc's local variables.
	if (compare(newname, play.var[k].name))
	{
	newval = play.var[k].var;
	//found var
	goto next2;
	}
	}
	}*/

	if (compare(newname, "&current_sprite")) {
		newval = rinfo[script]->sprite;
		goto next2;
	}

	if (compare(newname, "&current_script")) {
		newval = script;
		goto next2;

	}

	//v1.08 special variables.
	if (compare(newname, "&return")) {
		newval = returnint;
		goto next2;
	}

	if (compare(newname, "&arg1")) {
		newval = rinfo[script]->arg1;
		goto next2;
	}

	if (compare(newname, "&arg2")) {
		newval = rinfo[script]->arg2;
		goto next2;
	}

	if (compare(newname, "&arg3")) {
		newval = rinfo[script]->arg3;
		goto next2;
	}

	if (compare(newname, "&arg4")) {
		newval = rinfo[script]->arg4;
		goto next2;
	}

	if (compare(newname, "&arg5")) {
		newval = rinfo[script]->arg5;
		goto next2;
	}

	if (compare(newname, "&arg6")) {
		newval = rinfo[script]->arg6;
		goto next2;
	}

	if (compare(newname, "&arg7")) {
		newval = rinfo[script]->arg7;
		goto next2;
	}

	if (compare(newname, "&arg8")) {
		newval = rinfo[script]->arg8;
		goto next2;
	}

	if (compare(newname, "&arg9")) {
		newval = rinfo[script]->arg9;
		goto next2;
	}

	newval = atol(newname);

next2:

	if (math_ == '=')
		play.var[i].var = newval;

	if (math_ == '+')
		play.var[i].var += newval;

	if (math_ == '-')
		play.var[i].var -= newval;

	if (math_ == '/')
		play.var[i].var = play.var[i].var / newval;

	if (math_ == '*')
		play.var[i].var = play.var[i].var * newval;

	return newret;
}

void kill_scripts_owned_by(int sprite) {
	for (int i = 1; i < max_scripts; i++) {
		if (rinfo[i] != NULL) {
			if (rinfo[i]->sprite == sprite) {
				kill_script(i);

			}

		}
	}

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

				strcpy(check, line);
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
				strcat(talk.buffer, line);
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

			strcpy(check, p);


			strcpy(line, check);

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
		strcpy(talk.line[cur], check);
		talk.line_return[cur] = retnum;
		cur++;
	}

}

void get_right(char line[200], char thing[100], char *ret) {
	char *dumb;
	int pos = strcspn(line, thing);


	if (pos == NULL) {
		strcpy(ret, "");
		return;
	}


	dumb = &ret[pos + 1];
	strcpy(ret, dumb);
}




void int_prepare(char line[100], int script) {
	int def = 0;
	char hold[100];
	strcpy(hold, line);
	char name[100];
	char crap[100];
	replace("=", " ", line);
	strcpy(crap, line);
	seperate_string(crap, 1, ';', line);
	get_word(line, 2, name);

	if (name[0] != '&') {
		Msg("ERROR:  Can't create var %s, should be &%s.", name, name);
		return;
	}


	make_int(name, def, script, script);

	strcpy(line, hold);

}

int change_sprite_inner(int h,  int val, int *change) {
	//Msg("Searching sprite %s with val %d.  Cur is %d", h, val, *change);

	if (h < 1) {
		Msg("Error with an SP command - Sprite %d is invalid.", h);
		return -1;
	}
	if (spr[h].active == false) return -1;
	if (val != -1) {
		*change = val;
	}

	return *change;

}

//redink1 added for long values
template<typename TYPE>
long change_sprite(int h,  int val, TYPE *change) {
	int temp = *change;
	long result = (long)change_sprite_inner(h, val, &temp);
	*change = temp;

	return result;
}

int change_edit(int h,  int val, unsigned short *change) {
	//Msg("Searching sprite %s with val %d.  Cur is %d", h, val, *change);

	if (h > 99) return -1;
	if (h < 1) return -1;
	if (val != -1) {
		*change = val;
	}

	return *change;

}
int change_edit_char(int h,  int val, unsigned char *change) {
	//Msg("Searching sprite %s with val %d.  Cur is %d", h, val, *change);
	//  Msg("h is %d..",val);
	if (h > 99) return -1;
	if (h < 1) return -1;
	if (val != -1) {
		*change = val;
	}

	return *change;
}


int change_sprite_noreturn(int h,  int val, int *change) {
	//Msg("Searching sprite %s with val %d.  Cur is %d", h, val, *change);
	if (spr[h].active == false) return -1;

	{
		*change = val;
	}

	return *change;

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
				strcpy(pam.sprite[j].script, "");


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


void show_bmp(char name[80], int showdot, int reserved, int script) {


	if (!File::exists(name)) {
		Msg("Error: Can't find bitmap at %s.", name);
		return;
	}

	lpDDSTrick = DDLoadBitmap(lpDD, name, 0, 0);

	lpDDPal = DDLoadPalette(lpDD, name);
	if (lpDDPal)
		lpDDSPrimary->SetPalette(lpDDPal);

	lpDDSTrick = DDLoadBitmap(lpDD, name, 0, 0);
	showb.active = true;
	showb.showdot = showdot;
	showb.script = script;

	abort_this_flip = true;

	RECT rcRect;
	SetRect(&rcRect, 0, 0, 640, 480);
	//HDC hdc;
	//char msg[200];

again:
	ddrval = lpDDSBack->BltFast(0, 0, lpDDSTrick,
	                            &rcRect, DDBLTFAST_NOCOLORKEY);

	if (ddrval == DDERR_WASSTILLDRAWING) goto again;


	flip_it_second();

}



void copy_bmp(char name[80]) {
	if (!File::exists(name)) {
		Msg("Error: Can't find bitmap at %s.", name);
		return;
	}

	lpDDSTrick = DDLoadBitmap(lpDD, name, 0, 0);

	lpDDPal = DDLoadPalette(lpDD, name);
	if (lpDDPal)
		lpDDSPrimary->SetPalette(lpDDPal);

	lpDDSTrick = DDLoadBitmap(lpDD, name, 0, 0);
	abort_this_flip = true;

	RECT rcRect;
	SetRect(&rcRect, 0, 0, 640, 480);
	//HDC hdc;
	//char msg[200];

again:
	ddrval = lpDDSBack->BltFast(0, 0, lpDDSTrick,
	                            &rcRect, DDBLTFAST_NOCOLORKEY);

	if (ddrval == DDERR_WASSTILLDRAWING) goto again;


again1:
	ddrval = lpDDSTwo->BltFast(0, 0, lpDDSTrick,
	                           &rcRect, DDBLTFAST_NOCOLORKEY);

	if (ddrval == DDERR_WASSTILLDRAWING) goto again1;



	flip_it_second();

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



void add_item(char name[10], int mseq, int mframe, bool magic) {
	if (magic == false) {
		//add reg item

		for (int i = 1; i < 17; i ++) {
			if (play.item[i].active == false) {
				if (debug_mode)
					Msg("Weapon/item %s added to inventory.", name);
				play.item[i].seq = mseq;
				play.item[i].frame = mframe;
				strcpy(play.item[i].name, name);
				play.item[i].active = true;

				int crap1 = load_script(play.item[i].name, 1000, false);
				if (locate(crap1, "PICKUP")) run_script(crap1);

				return;
			}
		}

	} else {
		//add magic item
		for (int i = 1; i < 9; i ++) {
			if (play.mitem[i].active == false) {
				if (debug_mode)
					Msg("Magic %s added to inventory.", name);
				play.mitem[i].seq = mseq;
				play.mitem[i].frame = mframe;
				strcpy(play.mitem[i].name, name);

				play.mitem[i].active = true;

				int crap = load_script(play.mitem[i].name, 1000, false);
				if (locate(crap, "PICKUP")) run_script(crap);

				return;
			}
		}


	}
}

void fill_screen(int num) {
	int crap;
	DDBLTFX     ddBltFx;
	ZeroMemory(&ddBltFx, sizeof(ddBltFx));
	ddBltFx.dwSize = sizeof(ddBltFx);
	//redink1 fix for correct fill_screen colors in truecolor mode
	if (truecolor) {
		lpDDPal->GetEntries(0, 0, 256, pe);
		ddBltFx.dwFillColor = pe[num].peBlue << wBPos | pe[num].peGreen << wGPos | pe[num].peRed << wRPos;
	} else
		ddBltFx.dwFillColor = num;
	crap = lpDDSTwo->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddBltFx);

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

int process_line(int script, char *s, bool doelse) {
	char *h, *p;
	int i;
	char line[200];
	char ev[15][100];
	char temp[100];
	char first[2];
	//char rstring[200];
	//int rint;
	int sprite = 0;

	if (rinfo[script]->level < 1) rinfo[script]->level = 1;


	for (int kk = 1; kk < 15; kk++) ev[kk][0] = 0;
	//refinfo save_rinfo;
	h = s;
	if (h[0] == 0) return 0;
	if ((h[0] == '/') && (h[1] == '/'))

	{
		//Msg("It was a comment!");
		goto bad;
	}



	for (i = 1; i <= 14; i++) {
		if (seperate_string(h, i, ' ', ev[i]) == false) goto pass;
	}

pass:
	//Msg("first line is %s (second is %s)", ev[1], ev[2]);
	if (compare(ev[1], "VOID")) {

		if (rinfo[script]->proc_return != 0) {
			run_script(rinfo[script]->proc_return);
			kill_script(script);
		}

		//Msg("returning..");
		return 2;
	}
	//replace("\n","",ev[1]);
	if (ev[1][strlen(ev[1]) - 1] == ':') if (strlen(ev[2]) < 2) if (strncmp(ev[1], "say", 3) != 0)

			{
				//  Msg("Found label %s..",ev[1]);
				return 0; //its a label
			}
	if (ev[1][0] == '(') {
		//this procedure has been passed a conditional statement finder
		//what kind of conditional statement is it?
		p = h;
		seperate_string(h, 2, ')', temp);
		//Msg("We found %s, woah!", temp);
		seperate_string(h, 1, ')', ev[1]);

		// Msg("Ok, turned h %s to  ev1 %s.",h,ev[1]);
		p = &p[strlen(ev[1]) + 1];

		strip_beginning_spaces(p);
		//  Msg("does %s have a ( in front?", p);
		//Msg("We found %s, woah!", temp);

		//These are used for conditionals??
		if (strchr(temp, '=') != NULL) {
			h = &h[1];
			strip_beginning_spaces(h);
			process_line(script, h, false);
			replace("==", "", temp);
			sprintf(line, "%d == %s", returnint, temp);
			returnint = var_figure(line, script);
			strcpy(h, "\n");
			return 0;
		}

		if (strchr(temp, '>') != NULL) {
			h = &h[1];
			strip_beginning_spaces(h);
			process_line(script, h, false);
			replace("==", "", temp);
			sprintf(line, "%d > %s", returnint, temp);
			returnint = var_figure(line, script);
			strcpy(h, "\n");
			return 0;
		}

		if (strchr(temp, '<') != NULL) {
			h = &h[1];
			strip_beginning_spaces(h);
			process_line(script, h, false);
			replace("==", "", temp);
			sprintf(line, "%d < %s", returnint, temp);
			returnint = var_figure(line, script);
			strcpy(h, "\n");
			return 0;
		}
		if (strchr(temp, '<=') != NULL) {
			h = &h[1];
			strip_beginning_spaces(h);
			process_line(script, h, false);
			replace("==", "", temp);
			sprintf(line, "%d <= %s", returnint, temp);
			returnint = var_figure(line, script);
			strcpy(h, "\n");
			return 0;
		}
		if (strchr(temp, '>=') != NULL) {
			h = &h[1];
			strip_beginning_spaces(h);
			process_line(script, h, false);
			replace("==", "", temp);
			sprintf(line, "%d >= %s", returnint, temp);
			returnint = var_figure(line, script);
			strcpy(h, "\n");
			return 0;
		}

		if (strchr(temp, '!=') != NULL) {
			h = &h[1];
			strip_beginning_spaces(h);
			process_line(script, h, false);
			replace("==", "", temp);
			sprintf(line, "%d != %s", returnint, temp);
			returnint = var_figure(line, script);
			strcpy(h, "\n");
			return 0;
		}


		if (p[0] == ')') {
			//its a procedure in the if statement!!!
			h = &h[1];
			p = &p[1];
			strcpy(line, p);
			process_line(script, h, false);

			//8
			Msg("Returned %d for the returnint", returnint);
			h = s;
			strcpy(s, line);

			//  Msg("Returing %s..", s);
			return 0;
		} else {
			h = &h[1];

			seperate_string(h, 1, ')', line);
			h = &h[strlen(line) + 1];
			returnint = var_figure(line, script);

			strcpy(s, h);

			return 0;
		}

		strip_beginning_spaces(h);
		strip_beginning_spaces(ev[1]);

		s = h;


	} else {



	}


	if (strchr(ev[1], '(') != NULL) {
		//Msg("Has a (, lets change it");
		seperate_string(h, 1, '(', ev[1]);
		//Msg("Ok, first is now %s",ev[1]);


	}

	sprintf(first, "%c", ev[1][0]);



	if (compare(first, "{")) {


		rinfo[script]->level++;
		//Msg("Went up level, now at %d.", rinfo[script]->level);
		h = &h[1];
		if (rinfo[script]->skipnext) {
			rinfo[script]->skipnext = false;
			rinfo[script]->onlevel = (rinfo[script]->level - 1);
			//Msg("Skipping until level %d is met..", rinfo[script]->onlevel);

		}
		goto good;
	}

	if (compare(first, "}")) {
		rinfo[script]->level--;
		//Msg("Went down a level, now at %d.", rinfo[script]->level);
		h = &h[1];

		if (rinfo[script]->onlevel > 0) if (rinfo[script]->level == rinfo[script]->onlevel) {
				strip_beginning_spaces(h);

				strcpy(s, h);
				return 4;
			}
		goto good;
	}




	if (rinfo[script]->level < 0) {
		rinfo[script]->level = 0;
	}

	if (compare(ev[1], "void")) {
		//     Msg("Next procedure starting, lets quit");
		strcpy(s, h);
		if (rinfo[script]->proc_return != 0) {
			run_script(rinfo[script]->proc_return);
			kill_script(script);
		}

		return 2;
	}


	{
		//used to be an if..


		if (rinfo[script]->onlevel > 0) {
			if (rinfo[script]->level > rinfo[script]->onlevel) return 0;

		}
		rinfo[script]->onlevel = 0;

		if (rinfo[script]->skipnext) {
			//sorry, can't do it, you were told to skip the next thing
			rinfo[script]->skipnext = false;
			strcpy(s, h);
			return 3;
		}

		//if (debug_mode) Msg("%s",s);


		if (compare(ev[1], "void")) {
			Msg("ERROR: Missing } in %s, offset %d.", rinfo[script]->name, rinfo[script]->current);
			strcpy(s, h);
			return 2;
		}

		if (compare(ev[1], "else")) {
			//Msg("Found else!");
			h = &h[strlen(ev[1])];


			if (doelse) {
				//Msg("Yes to else...");



			} else {
				//they shouldn't run the next thing
				rinfo[script]->skipnext = true;
				//Msg("No to else...");

			}
			strcpy(s, h);
			return 1;

		}

		if (compare(ev[1], "show_console")) {
			h = &h[strlen(ev[1])];
			mConsoleActive = true;
			/*int ARR[20] = {1,0,0,0,0,0,0,0,0,0};
			if (get_parms(ev[1], script, h, ARR))
			{
			    if ( nlist[0] >= 1 )
			    {
			        mConsoleActive = true;
			    }
			    else
			    {
			        mConsoleActive = false;
			    }
			}
			returnint = 0;
			if ( mConsoleActive )
			{
			    returnint = 1;
			}*/
		}

		//redink1 - sets font color
		if (compare(ev[1], "set_font_color")) {
			h = &h[strlen(ev[1])];
			const int ARR[20] = {1, 1, 1, 1, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				if (nlist[0] >= 1 && nlist[0] <= 15 &&
				        nlist[1] >= 0 && nlist[1] <= 255 &&
				        nlist[2] >= 0 && nlist[2] <= 255 &&
				        nlist[3] >= 0 && nlist[3] <= 255) {
					font_colors[nlist[0]].red   = nlist[1];
					font_colors[nlist[0]].green = nlist[2];
					font_colors[nlist[0]].blue  = nlist[3];
				}
			}

			strcpy(s, h);
			return 0;
		}

		//redink1 - clears the editor information, useful for save games and such
		if (compare(ev[1], "clear_editor_info")) {
			h = &h[strlen(ev[1])];
			for (int ii = 0; ii < 769; ii++) {
				for (int j = 0; j < 100; j++) {
					play.spmap[ii].seq[j] = 0;
					play.spmap[ii].frame[j] = 0;
					play.spmap[ii].type[j] = 0;
					play.spmap[ii].last_time = 0;
				}
			}
			returnint = 1;
			return 0;
		}

		//redink1 - returns the number of variables used
		if (compare(ev[1], "var_used")) {
			h = &h[strlen(ev[1])];
			int m = 0;
			for (int ii = 1; ii < max_vars; ii++)
				if (play.var[ii].active == true)
					m++;
			returnint = m;
			return 0;
		}

		//redink1 added this function to load a new map/dink.dat
		if (compare(ev[1], "load_map")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {2, 2, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				strcpy(current_map, slist[0]);
				strcpy(current_dat, slist[1]);
				map_info::load();
			}

			strcpy(s, h);
			return 0;
		}


		//redink1 added this function to load a pallete from any bmp
		if (compare(ev[1], "load_palette")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				if (File::exists(slist[0])) {
					LPDIRECTDRAWSURFACE oldTrick = DDLoadBitmap(lpDD, slist[0], 0, 0);
					lpDDPal = DDLoadPalette(lpDD, slist[0]);
					if (lpDDPal) {
						lpDDSPrimary->SetPalette(lpDDPal);
						lpDDPal->GetEntries(0, 0, 256, pe);
						lpDDPal->GetEntries(0, 0, 256, real_pal);
						//Store in save game
						strncpy(play.palette, slist[0], 50);
					}
					oldTrick->Release();
				}
			}

			strcpy(s, h);
			return 0;
		}

		//redink1 added this function to load new tiles, because he is a l33t guy
		if (compare(ev[1], "load_tile")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {2, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				if (!File::exists(slist[0])) {
					sprintf(slist[0], "..\\DINK\\%s", slist[0]);
				}

				if (File::exists(slist[0]) && nlist[1] > 0 && nlist[1] < tile_screens) {
					//Need to unload old tiles...
					tiles[nlist[1]]->Release();
					//Load in the new tiles...
					tiles[nlist[1]] = DDTileLoad(lpDD, slist[0], 0, 0, nlist[1]);
					DDSetColorKey(tiles[nlist[1]], RGB(0, 0, 0));
					//Store in save game
					strncpy(play.tile[nlist[1]].file, slist[0], 50);
				}
			}

			strcpy(s, h);
			return 0;
		}

		//redink1 added this function to change the save game 'info'
		if (compare(ev[1], "set_save_game_info")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				strcpy(save_game_info, slist[0]);
			}
			strcpy(s, h);
			return 0;
		}

		//redink1 added this function to show the item screen
		if (compare(ev[1], "show_inventory")) {
			h = &h[strlen(ev[1])];
			item_screen = true;
			strcpy(s, h);
			return 0;
		}

		//redink1 added this function,, and took it away.
		/*if (compare(ev[1], "get_compatibility"))
		{
		returnint = 0;

		h = &h[strlen(ev[1])];
		int ARR[20] = {2,1,0,0,0,0,0,0,0,0};
		if (get_parms(ev[1], script, h, ARR))
		{
		if (compare(slist[0],"get_compatibility"))
		{
		if (nlist[1] <= 1)
		{
		returnint = 1;
		}
		}
		}
		strcpy(s, h);
		return 0;
		}*/

		//redink1 added this function
		if (compare(ev[1], "get_time_game")) {
			h = &h[strlen(ev[1])];
			returnint = play.minutes +
			            (g_events->getTotalSeconds() / 60);
			strcpy(s, h);
			return 0;
		}

		//redink1 added this function
		if (compare(ev[1], "get_time_real")) {
			h = &h[strlen(ev[1])];
			TimeDate td;
			g_system->getTimeAndDate(td);

			returnint = td.tm_hour * 60 + td.tm_min;
			strcpy(s, h);
			return 0;
		}

		//redink1 added this function
		if (compare(ev[1], "get_date_year")) {
			h = &h[strlen(ev[1])];
			TimeDate td;
			g_system->getTimeAndDate(td);
			returnint = td.tm_year;
			strcpy(s, h);
			return 0;
		}

		//redink1 added this function
		if (compare(ev[1], "get_date_month")) {
			h = &h[strlen(ev[1])];
			TimeDate td;
			g_system->getTimeAndDate(td);
			returnint = td.tm_mon;
			strcpy(s, h);
			return 0;
		}

		//redink1 added this function
		if (compare(ev[1], "get_date_day")) {
			h = &h[strlen(ev[1])];
			TimeDate td;
			g_system->getTimeAndDate(td);
			returnint = td.tm_mday;
			strcpy(s, h);
			return 0;
		}

		//redink1 added this function
		if (compare(ev[1], "math_abs")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = abs(nlist[0]);
			}
			strcpy(s, h);
			return 0;
		}

		//redink1 added this function
		/*if (compare(ev[1], "math_sin"))
		{
		    h = &h[strlen(ev[1])];
		    int ARR[20] = {1,0,0,0,0,0,0,0,0,0};
		    if (get_parms(ev[1], script, h, ARR))
		    {
		        returnint = sin((double)nlist[0]);
		    }
		    strcpy(s, h);
		    return 0;
		}

		//redink1 added this function
		if (compare(ev[1], "math_cos"))
		{
		    h = &h[strlen(ev[1])];
		    int ARR[20] = {1,0,0,0,0,0,0,0,0,0};
		    if (get_parms(ev[1], script, h, ARR))
		    {
		        returnint = cos((double)nlist[0]);
		    }
		    strcpy(s, h);
		    return 0;
		}

		//redink1 added this function
		if (compare(ev[1], "math_tan"))
		{
		    h = &h[strlen(ev[1])];
		    int ARR[20] = {1,0,0,0,0,0,0,0,0,0};
		    if (get_parms(ev[1], script, h, ARR))
		    {
		        returnint = tan((double)nlist[0]);
		    }
		    strcpy(s, h);
		    return 0;
		}*/

		//redink1 added this function
		if (compare(ev[1], "math_sqrt")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = sqrt((double)abs(nlist[0]));
			}
			strcpy(s, h);
			return 0;
		}

		//redink1 added this function
		if (compare(ev[1], "math_mod")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = (nlist[0] % nlist[1]);
			}
			strcpy(s, h);
			return 0;
		}

		//redink1
		if (compare(ev[1], "sp_custom")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {2, 1, 1, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR) && spr[nlist[1]].active == true) {
				if (nlist[1] < 1 || spr[nlist[1]].active == false) {
					returnint = -1;
				} else {
					// If key doesn't exist, create it.
					if (spr[nlist[1]].custom->find(slist[0]) == spr[nlist[1]].custom->end()) {
						(*spr[nlist[1]].custom)[slist[0]] = 0;
					}

					// Set the value
					if (nlist[2] != -1) {
						spr[nlist[1]].custom->erase(slist[0]);
						(*spr[nlist[1]].custom)[slist[0]] = nlist[2];
					}

					returnint = spr[nlist[1]].custom->find(slist[0])->_value;
				}
				return 0;
			}
			returnint = -1;
			return 0;
		}

		//redink1
		if (compare(ev[1], "sp_blood_seq")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				change_sprite(nlist[0], nlist[1], &spr[nlist[0]].bloodseq);

				returnint = spr[nlist[0]].bloodseq;

				return 0;
			}
			returnint =  -1;
			return 0;
		}

		//redink1
		if (compare(ev[1], "sp_blood_num")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				change_sprite(nlist[0], nlist[1], &spr[nlist[0]].bloodnum);

				returnint = spr[nlist[0]].bloodseq;

				return 0;
			}
			returnint =  -1;
			return 0;
		}

		//redink1 added to get index of specified item
		if (compare(ev[1], "get_item")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = 0;
				for (i = 1; i < 17; i++) {
					if (play.item[i].active) {
						if (compare(play.item[i].name, slist[0])) {
							returnint = i;
							break;
						}
					}
				}
				return 0;
			}
			returnint =  -1;
			return 0;
		}

		//redink1 added to get index of specified magic spell
		if (compare(ev[1], "get_magic")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = 0;
				for (i = 1; i < 9; i++) {
					if (play.mitem[i].active) {
						if (compare(play.mitem[i].name, slist[0])) {
							returnint = i;
							break;
						}
					}
				}
				return 0;
			}
			returnint =  -1;
			return 0;
		}

		//redink1 clip stuff
		if (compare(ev[1], "sp_clip_left")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				change_sprite(nlist[0], nlist[1], &spr[nlist[0]].alt.left);

				returnint = spr[nlist[0]].alt.left;

				return 0;
			}
			returnint =  -1;
			return 0;
		}

		//redink1 clip stuff
		if (compare(ev[1], "sp_clip_top")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				change_sprite(nlist[0], nlist[1], &spr[nlist[0]].alt.top);

				returnint = spr[nlist[0]].alt.top;

				return 0;
			}
			returnint =  -1;
			return 0;
		}

		//redink1 clip stuff
		if (compare(ev[1], "sp_clip_right")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				change_sprite(nlist[0], nlist[1], &spr[nlist[0]].alt.right);

				returnint = spr[nlist[0]].alt.right;

				return 0;
			}
			returnint =  -1;
			return 0;
		}

		//redink1 clip stuff
		if (compare(ev[1], "sp_clip_bottom")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				change_sprite(nlist[0], nlist[1], &spr[nlist[0]].alt.bottom);

				returnint = spr[nlist[0]].alt.bottom;

				return 0;
			}
			returnint =  -1;
			return 0;
		}

		//redink1 added so users can check truecolor or not
		if (compare(ev[1], "get_truecolor")) {
			h = &h[strlen(ev[1])];
			returnint = truecolor;
			strcpy(s, h);
			return 0;
		}

		//redink1 added so developers can change or see what tile is at any given position
		if (compare(ev[1], "map_tile")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				//Yeah... they can only modify valid tiles
				if (nlist[0] > 0 && nlist[0] <= 96) {
					//Only change the value if it is greater than 0...
					if (nlist[1] > 0) {
						pam.t[nlist[0] - 1].num = nlist[1];
					}
					returnint = pam.t[nlist[0] - 1].num;
					return 0;
				}
			}
			returnint =  -1;
			return 0;
		}

		//redink1 added so a developer can retrieve/modify a hard tile
		if (compare(ev[1], "map_hard_tile")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				//Yeah... they can only modify valid tiles
				if (nlist[0] > 0 && nlist[0] <= 96) {
					//Only change the value if it is greater than 0...
					if (nlist[1] > 0) {
						pam.t[nlist[0] - 1].althard = nlist[1];
					}
					returnint = pam.t[nlist[0] - 1].althard;
					return 0;
				}
			}
			returnint =  -1;
			return 0;
		}

		if (compare(ev[1], "unfreeze")) {

			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				//Msg("UnFreeze called for %d.", nlist[0]);
				if (spr[nlist[0]].active) spr[nlist[0]].freeze = 0;
				else
					Msg("Couldn't unfreeze sprite %d in script %d, it doesn't exist.", nlist[0], script);

			}

			strcpy(s, h);
			return 0;
		}

		if (compare(ev[1], "freeze")) {
			//Msg("Freeze called (%s)", h);
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {

				if (spr[nlist[0]].active) spr[nlist[0]].freeze = script;
				else
					Msg("Couldn't freeze sprite %d in script %d, it doesn't exist.", nlist[0], script);

			}

			strcpy(s, h);
			return 0;
		}


		if (compare(ev[1], "set_callback_random")) {
			Msg("setting callback random");
			h = &h[strlen(ev[1])];
			int ARR[20] = {2, 1, 1, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				int cb = add_callback(slist[0], nlist[1], nlist[2], script);
				//got all parms, let do it
				returnint = cb;
			}

			strcpy(s, h);
			return 0;
		}

		// redink1 added
		if (compare(ev[1], "callback_kill")) {
			Msg("setting callback random");
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				if (nlist[0] >= 0 && nlist[0] <= 99) {
					callback[nlist[0]].active = false;
				}
			}
			strcpy(s, h);
			return 0;
		}

		if (compare(ev[1], "set_dink_speed")) {
			Msg("setting callback random");
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR) && nlist[0] != 0) {
				dinkspeed = nlist[0];
			}

			strcpy(s, h);
			return 0;
		}

		//redink1
		if (compare(ev[1], "set_dink_base_push")) {
			Msg("setting callback random");
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				mDinkBasePush = nlist[0];
			}
			strcpy(s, h);
			return 0;
		}

		if (compare(ev[1], "reset_timer")) {
#ifdef TODO
			h = &h[strlen(ev[1])];
			time(&time_start);
			play.minutes = 0;
			strcpy(s, h);
			return 0;
#else
			error("TODO: reset_timer");
#endif
		}


		if (compare(ev[1], "set_keep_mouse")) {
			Msg("setting callback random");
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				keep_mouse = nlist[0];

			}

			strcpy(s, h);
			return 0;
		}




		if (compare(ev[1], "add_item")) {

			h = &h[strlen(ev[1])];
			int ARR[20] = {2, 1, 1, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				add_item(slist[0], nlist[1], nlist[2], false);
			}

			strcpy(s, h);
			return 0;
		}

		if (compare(ev[1], "add_exp")) {

			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				add_exp(nlist[0], nlist[1], true);
			}

			strcpy(s, h);
			return 0;
		}


		if (compare(ev[1], "add_magic")) {

			h = &h[strlen(ev[1])];
			int ARR[20] = {2, 1, 1, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				add_item(slist[0], nlist[1], nlist[2], true);
			}

			strcpy(s, h);
			return 0;
		}


		if (compare(ev[1], "kill_this_item")) {

			h = &h[strlen(ev[1])];
			int ARR[20] = {2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				kill_cur_item_script(slist[0]);
			}

			strcpy(s, h);
			return 0;
		}

		if (compare(ev[1], "kill_this_magic")) {

			h = &h[strlen(ev[1])];
			int ARR[20] = {2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				kill_cur_magic_script(slist[0]);
			}

			strcpy(s, h);
			return 0;
		}



		if (compare(ev[1], "show_bmp")) {
			Msg("showing BMP");
			h = &h[strlen(ev[1])];
			int ARR[20] = {2, 1, 1, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				wait.active = false;
				show_bmp(slist[0], nlist[1], nlist[2], script);
			}

			strcpy(s, h);
			return 2;
		}


		if (compare(ev[1], "wait_for_button")) {
			Msg("waiting for button with script %d", script);
			h = &h[strlen(ev[1])];
			strcpy(s, h);
			wait.script = script;
			wait.active = true;
			wait.button = 0;
			return 2;
		}

		if (compare(ev[1], "stop_wait_for_button")) {
			wait.active = false;

			return 0;
		}


		if (compare(ev[1], "copy_bmp_to_screen")) {
			Msg("copying BMP");
			h = &h[strlen(ev[1])];
			int ARR[20] = {2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				copy_bmp(slist[0]);
			}

			strcpy(s, h);
			return 0;
		}




		if (compare(ev[1], "say")) {

			h = &h[strlen(ev[1])];
			int ARR[20] = {2, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {

				if (nlist[1] == 0) {
					Msg("Say_stop error:  Sprite 0 can talk? Yeah, didn't think so.");
					return 0;
				}

				if (nlist[1] != 1000)
					kill_text_owned_by(nlist[1]);
				decipher_string(slist[0], script);
				returnint = say_text(slist[0], nlist[1], script);
				//Msg("Just said %s.", slist[0]);
			}

			strcpy(s, h);
			return 0;
		}

		if (compare(ev[1], "draw_screen")) {

			if (rinfo[script]->sprite == 1000) {
				draw_map_game();
				return 0;
			}
			draw_map_game();
			return 2;
		}


		if (compare(ev[1], "free_items")) {
			returnint = 0;
			for (i = 1; i < 17; i ++) {
				if (play.item[i].active == false) {
					returnint += 1;
				}
			}
			return 0;
		}


		if (compare(ev[1], "kill_cur_item")) {
			returnint = 0;
			kill_cur_item();
			return 2;
		}



		if (compare(ev[1], "kill_cur_magic")) {
			returnint = 0;
			kill_cur_magic();
			return 2;
		}



		if (compare(ev[1], "free_magic")) {
			returnint = 0;

			for (i = 1; i < 9; i ++) {
				if (play.mitem[i].active == false) {
					returnint += 1;
				}
			}
			return 0;
		}




		if (compare(ev[1], "draw_status")) {
			draw_status_all();
			return 0;
		}


		if (compare(ev[1], "arm_weapon")) {

			if (weapon_script != 0) if (locate(weapon_script, "DISARM")) run_script(weapon_script);
			weapon_script = load_script(play.item[*pcur_weapon].name, 1000, false);
			if (locate(weapon_script, "ARM")) run_script(weapon_script);


			return 0;
		}

		if (compare(ev[1], "arm_magic")) {


			if (magic_script != 0) if (locate(magic_script, "DISARM")) run_script(magic_script);
			magic_script = load_script(play.mitem[*pcur_magic].name, 1000, false);
			if (locate(magic_script, "ARM")) run_script(magic_script);

			return 0;
		}


		if (compare(ev[1], "load_screen")) {
			//Msg("Loading map %d..",*pmap);
			update_screen_time();
			load_map(map.loc[*pmap]);

			//redink1 fix for correct indicator on mini-map
			if (map.indoor[*pmap] == 0)
				play.last_map = *pmap;
			return 0;
		}


		if (compare(ev[1], "choice_start")) {

			kill_text_owned_by(1);
			if (talk_get(script)) {

				//      Msg("Question gathered successfully.");
				return 2;
			}

			return 0;
		}


		if (compare(ev[1], "say_stop")) {

			h = &h[strlen(ev[1])];
			int ARR[20] = {2, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				if (nlist[1] == 0) {
					Msg("Say_stop error:  Sprite 0 can talk? Yeah, didn't think so.");
					return 0;
				}

				kill_text_owned_by(nlist[1]);
				kill_text_owned_by(1);
				kill_returning_stuff(script);

				decipher_string(slist[0], script);
				sprite = say_text(slist[0], nlist[1], script);
				returnint = sprite;
				spr[sprite].callback = script;
				play.last_talk = script;
				//Msg("Sprite %d marked callback true.", sprite);

				strcpy(s, h);

				return 2;

			}

			strcpy(s, h);
			return 0;
		}




		if (compare(ev[1], "say_stop_npc")) {

			h = &h[strlen(ev[1])];
			int ARR[20] = {2, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {

				if (text_owned_by(nlist[1])) {
					returnint = 0;
					return 0;
				}

				kill_returning_stuff(script);
				decipher_string(slist[0], script);
				sprite = say_text(slist[0], nlist[1], script);
				returnint = sprite;
				spr[sprite].callback = script;
				strcpy(s, h);

				return 2;

			}

			strcpy(s, h);
			return 0;
		}


		if (compare(ev[1], "say_stop_xy")) {

			h = &h[strlen(ev[1])];
			int ARR[20] = {2, 1, 1, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {

				Msg("Say_stop_xy: Adding %s", slist[0]);
				kill_returning_stuff(script);
				decipher_string(slist[0], script);
				sprite = say_text_xy(slist[0], nlist[1], nlist[2], script);
				spr[sprite].callback = script;
				spr[sprite].live = true;
				play.last_talk = script;
				strcpy(s, h);

				return 2;

			}

			strcpy(s, h);
			return 0;
		}


		if (compare(ev[1], "say_xy")) {

			h = &h[strlen(ev[1])];
			int ARR[20] = {2, 1, 1, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				kill_returning_stuff(script);
				decipher_string(slist[0], script);
				sprite = say_text_xy(slist[0], nlist[1], nlist[2], script);
				returnint = sprite;
				strcpy(s, h);
				return 0;

			}

			strcpy(s, h);
			return 0;
		}

		if (compare(ev[1], "restart_game")) {

			while (kill_last_sprite());
			kill_repeat_sounds_all();
			kill_all_scripts_for_real();
			mode = 0;
			screenlock = 0;
			kill_all_vars();
			FillMemory(&hm, sizeof(hm), 0);
			for (int u = 1; u <= 10; u++)
				play.button[u] = u;
			int scriptMain = load_script("main", 0, true);

			locate(scriptMain, "main");
			run_script(scriptMain);
			//lets attach our vars to the scripts
			attach();
			return 2;
		}

		if (compare(ev[1], "wait")) {

			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				//           Msg("Wait called for %d.", nlist[0]);
				strcpy(s, h);
				kill_returning_stuff(script);
				add_callback("", nlist[0], 0, script);

				return 2;
			}

			strcpy(s, h);
			return 0;
		}

		if (compare(ev[1], "preload_seq")) {

			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				check_seq_status(nlist[0]);
			}

			strcpy(s, h);
			return 0;
		}




		if (compare(ev[1], "script_attach")) {

			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {

				rinfo[script]->sprite = nlist[0];
			}
			strcpy(s, h);
			return 0;
		}

		if (compare(ev[1], "draw_hard_sprite")) {

			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				update_play_changes();
				int l = nlist[0];
				RECT mhard;
				CopyRect(&mhard, &picInfo[seq[spr[l].pseq].frame[spr[l].pframe]].hardbox);
				OffsetRect(&mhard, (spr[l].x - 20), spr[l].y);

				fill_hardxy(mhard);
				fill_back_sprites();
				fill_hard_sprites();


			}
			strcpy(s, h);
			return 0;
		}


		if (compare(ev[1], "move")) {
			// (sprite, direction, until, nohard);
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 1, 1, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {

				spr[nlist[0]].move_active = true;
				spr[nlist[0]].move_dir = nlist[1];
				spr[nlist[0]].move_num = nlist[2];
				spr[nlist[0]].move_nohard = nlist[3];
				spr[nlist[0]].move_script = 0;
				if (debug_mode) Msg("Moving: Sprite %d, dir %d, num %d", nlist[0], nlist[1], nlist[2]);


			}

			strcpy(s, h);
			return 0;
		}


		if (compare(ev[1], "sp_script")) {
			// (sprite, direction, until, nohard);
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 2, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				if (nlist[0] == 0) {
					Msg("Error: sp_script cannot process sprite 0??");
					return 0;
				}
				kill_scripts_owned_by(nlist[0]);
				if (load_script(slist[1], nlist[0], true) == 0) {
					returnint = 0;
					return 0;
				}
				if (no_running_main == true) Msg("Not running %s until later..", rinfo[spr[nlist[0]].script]->name);

				if (no_running_main == false)
					locate(spr[nlist[0]].script, "MAIN");


				int tempreturn = spr[nlist[0]].script;

				if (no_running_main == false)
					run_script(spr[nlist[0]].script);


				returnint = tempreturn;
			}

			strcpy(s, h);
			return 0;
		}


		if (compare(ev[1], "spawn")) {
			// (sprite, direction, until, nohard);
			h = &h[strlen(ev[1])];
			int ARR[20] = {2, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				int mysc = load_script(slist[0], 1000, true);
				if (mysc == 0) {
					returnint = 0;
					return 0;
				}
				locate(mysc, "MAIN");
				int tempreturn = mysc;
				run_script(mysc);
				returnint = tempreturn;
			}

			strcpy(s, h);
			return 0;
		}

		if (compare(ev[1], "run_script_by_number")) {
			// (sprite, direction, until, nohard);
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 2, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				if (locate(nlist[0], slist[1])) {
					run_script(nlist[0]);
				}

			}

			strcpy(s, h);
			return 0;
		}



		if (compare(ev[1], "draw_hard_map")) {
			// (sprite, direction, until, nohard);
			Msg("Drawing hard map..");
			update_play_changes();
			fill_whole_hard();
			fill_hard_sprites();
			fill_back_sprites();
			strcpy(s, h);
			return 0;
		}




		if (compare(ev[1], "draw_background")) {
			// (sprite, direction, until, nohard);
			draw_map_game_background();
			strcpy(s, h);
			return 0;
		}

		if (compare(ev[1], "fade_down")) {
			// (sprite, direction, until, nohard);
			process_downcycle = true;
			cycle_clock = thisTickCount + 1000;
			cycle_script = script;

			strcpy(s, h);

			return 2;
		}

		if (compare(ev[1], "fade_up")) {
			// (sprite, direction, until, nohard);
			h = &h[strlen(ev[1])];
			process_upcycle = true;
			cycle_script = script;

			strcpy(s, h);
			return 2;
		}

		if (compare(ev[1], "kill_this_task")) {
			// (sprite, direction, until, nohard);
			if (rinfo[script]->proc_return != 0) {
				run_script(rinfo[script]->proc_return);
			}
			kill_script(script);
			return 2;
		}

		if (compare(ev[1], "kill_game")) {
			// (sprite, direction, until, nohard);

			Msg("Was told to kill game, so doing it like a good boy.");
			//  PostMessage(hWndMain, WM_CLOSE, 0, 0);
			finiObjects();
			return 2;
		}


		/*if (compare(ev[1], "playavi"))
		{
		    // (sprite, direction, until, nohard);
		    h = &h[strlen(ev[1])];
		    int ARR[20] = {2,0,0,0,0,0,0,0,0,0};
		    if (get_parms(ev[1], script, h, ARR))
		    {
		        //DSDisable();


		        //ShowWindow(hWndMain, SW_MINIMIZE);

		        int myreturn =  _spawnl(_P_WAIT, "..\\dplay.exe","..\\dplay.exe",slist[0], NULL );

		        Msg("Return is %d", myreturn);
		        //ShowWindow(hWndMain, SW_MAXIMIZE);

		        //SetFocus(hWndMain);
		    }
		    //InitSound(hWndMain);
		    strcpy(s, h);
		    return 2;

		}*/

		//redink1 added
		if (compare(ev[1], "loopmidi")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				if (nlist[0] > 0) {
					mLoopMidi = true;
				} else {
					mLoopMidi = false;
				}
			}
		}

		if (compare(ev[1], "playmidi")) {
			// (sprite, direction, until, nohard);
			h = &h[strlen(ev[1])];
			int ARR[20] = {2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				//StopMidi();
				int regm = atol(slist[0]);
				Msg("Processing playmidi command.");
				if (regm > 1000) {
					Msg("playmidi - cd play command detected.");

					//cd directive

					if (cd_inserted) {
						if (regm == last_cd)
							if (cdplaying())
								return 2;
						Msg("Playing CD track %d.", regm - 1000);

						PlayCD(regm - 1000);
						strcpy(s, h);
						return 0;
					}   else {
						char hold[255];
						//cd isn't instered, can't play CD song!!!
						sprintf(hold, "%d.mid", regm - 1000);
						PlayMidi(hold);
					}
				}

				PlayMidi(slist[0]);
			}

			strcpy(s, h);
			return 0;
		}
		if (compare(ev[1], "stopmidi")) {
			// (sprite, direction, until, nohard);
			h = &h[strlen(ev[1])];
			StopMidi();
			strcpy(s, h);
			return 0;
		}

		if (compare(ev[1], "kill_all_sounds")) {
			kill_repeat_sounds_all();
			strcpy(s, h);
			return 0;

		}

		if (compare(ev[1], "turn_midi_off")) {
			midi_active = false;
			strcpy(s, h);
			return 0;

		}
		if (compare(ev[1], "turn_midi_on")) {
			midi_active = true;
			strcpy(s, h);
			return 0;

		}


		if (compare(ev[1], "Playsound")) {
			// (sprite, direction, until, nohard);
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 1, 1, 1, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				if (sound_on)
					returnint = SoundPlayEffect(nlist[0], nlist[1], nlist[2], nlist[3], nlist[4]);
				else returnint = 0;

			} else
				returnint = 0;

			strcpy(s, h);
			return 0;
		}


		if (compare(ev[1], "sound_set_survive")) {
			// (sprite, direction, until, nohard);
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				if (sound_on) {
					//let's set one sound to survive
					if (nlist[0] > 0)
						soundinfo[nlist[0]].survive = nlist[1];
				}
			}

			strcpy(s, h);
			return 0;
		}


		if (compare(ev[1], "sound_set_vol")) {
			// (sprite, direction, until, nohard);
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				if (sound_on) {
					//let's set one sound to survive
					if (nlist[0] > 0) {
						soundinfo[nlist[0]].vol = nlist[1];

						soundbank[nlist[0]]->SetVolume(nlist[1]);
					}
				}
			}

			strcpy(s, h);
			return 0;
		}


		if (compare(ev[1], "sound_set_kill")) {
			// (sprite, direction, until, nohard);
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				if (sound_on) {
					//let's set one sound to survive
					if (nlist[0] > 0)

						soundbank[nlist[0]]->Stop();
				}
			}

			strcpy(s, h);
			return 0;
		}



		if (compare(ev[1], "save_game")) {
			// (sprite, direction, until, nohard);
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {

				save_game(nlist[0]);
			}

			strcpy(s, h);
			return 0;
		}


		if (compare(ev[1], "force_vision")) {
			// (sprite, direction, until, nohard);
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {

				*pvision = nlist[0];
				rinfo[script]->sprite = 1000;
				fill_whole_hard();

				draw_map_game();

			}

			strcpy(s, h);
			return 0;
		}

		if (compare(ev[1], "fill_screen")) {
			// (sprite, direction, until, nohard);
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				fill_screen(nlist[0]);

			}

			strcpy(s, h);
			return 0;
		}


		if (compare(ev[1], "load_game")) {
			// (sprite, direction, until, nohard);
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				kill_all_scripts_for_real();
				returnint = load_game(nlist[0]);
				Msg("load completed. ");
				if (rinfo[script] == NULL) Msg("Script %d is suddenly null!", script);


				*pupdate_status = 1;
				draw_status_all();
				return 2;
			}

			strcpy(s, h);
			return 0;
		}

		if (compare(ev[1], "game_exist")) {
			// (sprite, direction, until, nohard);
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				sprintf(temp, "save%d.dat", nlist[0]);
				if (File::exists(temp)) returnint = 1;
				else returnint = 0;

			}

			strcpy(s, h);
			return 0;
		}


		if (compare(ev[1], "move_stop")) {
			// (sprite, direction, until, nohard);

			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 1, 1, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				//Msg("Move stop running %d to %d..", nlist[0], nlist[0]);
				spr[nlist[0]].move_active = true;
				spr[nlist[0]].move_dir = nlist[1];
				spr[nlist[0]].move_num = nlist[2];
				spr[nlist[0]].move_nohard = nlist[3];
				spr[nlist[0]].move_script = script;
				strcpy(s, h);
				if (debug_mode) Msg("Move_stop: Sprite %d, dir %d, num %d", nlist[0], nlist[1], nlist[2]);
				return 2;

			}

			strcpy(s, h);
			return 0;
		}




		if (compare(ev[1], "load_sound")) {

			h = &h[strlen(ev[1])];
			int ARR[20] = {2, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				if (sound_on) {
					Msg("getting %s..", slist[0]);
					CreateBufferFromWaveFile(slist[0], nlist[1]);
				}
			}

			strcpy(s, h);
			return 0;
		}




		if (compare(ev[1], "debug")) {

			h = &h[strlen(ev[1])];
			int ARR[20] = {2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {

				decipher_string(slist[0], script);
				Msg(slist[0]);
			}

			strcpy(s, h);
			return 0;
		}


		if (compare(ev[1], "goto")) {

			locate_goto(ev[2], script);
			return 0;
		}

		//redink1 added for global functions
		if (compare(ev[1], "make_global_function")) {

			h = &h[strlen(ev[1])];
			int ARR[20] = {2, 2, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {

				make_function(slist[0], slist[1]);
				//Msg(slist[0]);
			}
			strcpy(s, h);
			return 0;
		}

		if (compare(ev[1], "make_global_int")) {

			h = &h[strlen(ev[1])];
			int ARR[20] = {2, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {

				make_int(slist[0], nlist[1], 0, script);
				//Msg(slist[0]);
			}

			strcpy(s, h);
			return 0;
		}


		if (compare(ev[1], "int")) {

			int_prepare(h, script);

			//Msg(slist[0]);

			h = &h[strlen(ev[1])];

			//Msg("Int is studying %s..", h);
			if (strchr(h, '=') != NULL) {
				strip_beginning_spaces(h);
				//Msg("Found =...continuing equation");
				strcpy(s, h);
				return 4;
			}

			return 0;

		}



		if (compare(ev[1], "busy")) {

			h = &h[strlen(ev[1])];
			// Msg("Running busy, h is %s", h);
			int ARR[20] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				if (nlist[0] == 0) Msg("ERROR:  Busy cannot get info on sprite 0 in %s.", rinfo[script]->name);
				else {

					returnint = does_sprite_have_text(nlist[0]);

					Msg("Busy: Return int is %d and %d.  Nlist got %d.", returnint, does_sprite_have_text(nlist[0]), nlist[0]);

				}

			}  else Msg("Failed getting parms for Busy()");

			strcpy(s, h);
			return 0;
		}

		//redink1 added
		if (compare(ev[1], "sp_freeze")) {
			h = &h[strlen(ev[1])];
			// Msg("Running busy, h is %s", h);
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				// Set the value
				if (nlist[1] == 0) {
					spr[nlist[0]].freeze = 0;
				} else if (nlist[1] == 1) {
					spr[nlist[0]].freeze = script;
				}

				// Return the value
				if (spr[nlist[0]].freeze > 0) {
					returnint = 1;
				} else {
					returnint = 0;
				}
			}

			strcpy(s, h);
			return 0;
		}

		if (compare(ev[1], "inside_box")) {

			h = &h[strlen(ev[1])];
			Msg("Running pigs with h", h);
			int ARR[20] = {1, 1, 1, 1, 1, 1, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {

				RECT myrect;
				SetRect(&myrect, nlist[2], nlist[3], nlist[4], nlist[5]);
				returnint = inside_box(nlist[0], nlist[1], myrect);

				if (debug_mode)
					Msg("Inbox is int is %d and %d.  Nlist got %d.", returnint, nlist[0], nlist[1]);



			}  else Msg("Failed getting parms for inside_box");

			strcpy(s, h);
			return 0;
		}


		if (compare(ev[1], "random")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = RANDOM(nlist[1], nlist[0]);
			}  else Msg("Failed getting parms for Random()");

			strcpy(s, h);
			return 0;
		}

		if (compare(ev[1], "initfont")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {

				initfonts(slist[0]);
				Msg("Initted font %s", slist[0]);
			}  else Msg("Failed getting parms for Initfont()");

			strcpy(s, h);
			return 0;
		}


		if (compare(ev[1], "get_version")) {
			h = &h[strlen(ev[1])];
			returnint = dversion;
			strcpy(s, h);
			return 0;
		}

		if (compare(ev[1], "get_burn")) {
			h = &h[strlen(ev[1])];
			returnint = burn_revision;
			strcpy(s, h);
			return 0;
		}

		if (compare(ev[1], "set_mode")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				mode = nlist[0];
				returnint = mode;
			}  else Msg("Failed to set mode");

			strcpy(s, h);
			return 0;
		}

		if (compare(ev[1], "kill_shadow")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {

				for (int jj = 1; jj <= last_sprite_created; jj++) {
					if (spr[jj].brain == 15) if (spr[jj].brain_parm == nlist[0]) {

							spr[jj].active = 0;
						}


				}
			}

			strcpy(s, h);
			return 0;
		}



		if (compare(ev[1], "create_sprite")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 1, 1, 1, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {

				returnint = add_sprite_dumb(nlist[0], nlist[1], nlist[2],
				                            nlist[3], nlist[4],
				                            100);

				return 0;
			}
			returnint =  0;
			return 0;
		}



		if (compare(ev[1], "sp")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				for (int ii = 1; ii <= last_sprite_created; ii++) {

					if (spr[ii].sp_index == nlist[0]) {

						if (debug_mode) Msg("Sp returned %d.", ii);
						returnint = ii;
						return 0;
					}

				}
				if (last_sprite_created == 1) {
					Msg("warning - you can't call SP() from a screen-ref, no sprites have been created yet.");
				}

			}
			returnint =  0;
			return 0;
		}


		if (compare(ev[1], "is_script_attached")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {


				returnint =  spr[nlist[0]].script;

			}
			return 0;
		}



		if (compare(ev[1], "sp_speed")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].speed);

				if (nlist[1] != -1) changedir(spr[nlist[0]].dir, nlist[0], spr[nlist[0]].base_walk);

				return 0;
			}
			returnint =  -1;
			return 0;
		}


		if (compare(ev[1], "sp_range")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].range);

				return 0;
			}
			returnint =  -1;
			return 0;
		}


		if (compare(ev[1], "sp_nocontrol")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].nocontrol);
				return 0;
			}
			returnint =  -1;
			return 0;
		}

		if (compare(ev[1], "sp_nodraw")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].nodraw);
				return 0;
			}
			returnint =  -1;
			return 0;
		}


		if (compare(ev[1], "sp_picfreeze")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].picfreeze);
				return 0;
			}
			returnint =  -1;
			return 0;
		}



		if (compare(ev[1], "get_sprite_with_this_brain")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				for (i = 1; i <= last_sprite_created; i++) {
					if ((spr[i].brain == nlist[0]) && (i != nlist[1])) if
						(spr[i].active == 1) {
							Msg("Ok, sprite with brain %d is %d", nlist[0], i);
							returnint = i;
							return 0;
						}

				}
			}
			Msg("Ok, sprite with brain %d is 0", nlist[0], i);

			returnint =  0;
			return 0;
		}

		//redink1 added this to make Paul Pliska's life more fulfilling
		if (compare(ev[1], "get_next_sprite_with_this_brain")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 1, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				for (i = nlist[2]; i <= last_sprite_created; i++) {
					if ((spr[i].brain == nlist[0]) && (i != nlist[1])) if
						(spr[i].active == 1) {
							Msg("Ok, sprite with brain %d is %d", nlist[0], i);
							returnint = i;
							return 0;
						}

				}
			}
			Msg("Ok, sprite with brain %d is 0", nlist[0], i);

			returnint =  0;
			return 0;
		}


		if (compare(ev[1], "get_rand_sprite_with_this_brain")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {

				int cter = 0;
				for (i = 1; i <= last_sprite_created; i++) {
					if ((spr[i].brain == nlist[0]) && (i != nlist[1])) if
						(spr[i].active == 1) {
							cter++;

						}

				}

				if (cter == 0) {
					Msg("Get rand brain can't find any brains with %d.", nlist[0]);
					returnint =  0;
					return 0;
				}

				int mypick = RANDOM(1, cter);
				cter = 0;
				for (int ii = 1; ii <= last_sprite_created; ii++) {
					if ((spr[ii].brain == nlist[0]) && (ii != nlist[1])) if
						(spr[ii].active == 1) {
							cter++;
							if (cter == mypick) {
								returnint = ii;
								return 0;
							}
						}

				}


			}


			returnint =  0;
			return 0;
		}



		if (compare(ev[1], "sp_sound")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].sound);

				if (nlist[1] > 0) {
					SoundPlayEffect(spr[nlist[0]].sound, 22050, 0, nlist[0], 1);

				}
				return 0;
			}
			returnint =  -1;
			return 0;
		}

		if (compare(ev[1], "sp_attack_wait")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {

				returnint = change_sprite(nlist[0], nlist[1] + thisTickCount, &spr[nlist[0]].attack_wait);
				return 0;
			}
			returnint =  -1;
			return 0;
		}


		if (compare(ev[1], "sp_active")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {

				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].active);


				return 0;
			}
			returnint =  -1;
			return 0;
		}

		if (compare(ev[1], "sp_disabled")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {

				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].disabled);


				return 0;
			}
			returnint =  -1;
			return 0;
		}

		if (compare(ev[1], "sp_size")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].size);
				return 0;
			}
			returnint =  -1;
			return 0;
		}
		if (compare(ev[1], "activate_bow")) {

			spr[1].seq = 0;
			spr[1].pseq = 100 + spr[1].dir;
			spr[1].pframe = 1;
			bow.active = true;
			bow.script = script;
			bow.hitme = false;
			bow.time = 0;

			/*  bowsound->Release();

			lpDS->DuplicateSoundBuffer(ssound[42].sound,&bowsound);
			bowsound->Play(0, 0, DSBPLAY_LOOPING);
			*/

			return 2;
		}

		if (compare(ev[1], "get_last_bow_power")) {


			returnint = bow.last_power;
			return 0;
		}


		if (compare(ev[1], "sp_que")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].que);
				return 0;
			}
			returnint =  -1;
			return 0;
		}

		if (compare(ev[1], "sp_gold")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].gold);
				return 0;
			}
			returnint =  -1;
			return 0;
		}

		if (compare(ev[1], "sp_base_walk")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite_noreturn(nlist[0], nlist[1], &spr[nlist[0]].base_walk);
				return 0;
			}
			returnint =  -1;
			return 0;
		}


		if (compare(ev[1], "sp_target")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].target);
				return 0;
			}
			returnint =  -1;
			return 0;
		}

		if (compare(ev[1], "stopcd")) {
			mciSendCommand(CD_ID, MCI_CLOSE, 0, NULL);
			Msg("Stopped cd");
			killCD(1);
			return 0;
		}


		if (compare(ev[1], "sp_base_hit")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite_noreturn(nlist[0], nlist[1], &spr[nlist[0]].base_hit);
				return 0;
			}
			returnint =  -1;
			return 0;
		}

		if (compare(ev[1], "sp_base_attack")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite_noreturn(nlist[0], nlist[1], &spr[nlist[0]].base_attack);
				return 0;
			}
			returnint =  -1;
			return 0;
		}


		if (compare(ev[1], "sp_base_idle")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite_noreturn(nlist[0], nlist[1], &spr[nlist[0]].base_idle);
				return 0;
			}
			returnint =  -1;
			return 0;
		}


		if ((compare(ev[1], "sp_base_die")) || (compare(ev[1], "sp_base_death"))) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite_noreturn(nlist[0], nlist[1], &spr[nlist[0]].base_die);
				return 0;
			}
			returnint =  -1;
			return 0;
		}


		if (compare(ev[1], "disable_all_sprites")) {
			for (int jj = 1; jj < last_sprite_created; jj++)
				if (spr[jj].active) spr[jj].disabled = true;
			return 0;
		}
		if (compare(ev[1], "enable_all_sprites")) {
			for (int jj = 1; jj < last_sprite_created; jj++)
				if (spr[jj].active) spr[jj].disabled = false;
			return 0;
		}


		if (compare(ev[1], "sp_pseq")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].pseq);
				return 0;
			}
			returnint =  -1;
			return 0;
		}


		if (compare(ev[1], "sp_pframe")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].pframe);
				return 0;
			}
			returnint =  -1;
			return 0;
		}


		if (compare(ev[1], "sp_seq")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].seq);
				return 0;
			}
			returnint =  -1;
			return 0;
		}

		if (compare(ev[1], "editor_type")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				//Msg("Setting editor_type..");
				returnint = change_edit_char(nlist[0], nlist[1], &play.spmap[*pmap].type[nlist[0]]);
				return 0;
			}
			returnint =  -1;
			return 0;
		}
		if (compare(ev[1], "editor_seq")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_edit(nlist[0], nlist[1], &play.spmap[*pmap].seq[nlist[0]]);
				return 0;
			}
			returnint =  -1;
			return 0;
		}

		if (compare(ev[1], "editor_frame")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_edit_char(nlist[0], nlist[1], &play.spmap[*pmap].frame[nlist[0]]);
				return 0;
			}
			returnint =  -1;
			return 0;
		}



		if (compare(ev[1], "sp_editor_num")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = spr[nlist[0]].sp_index;
				return 0;
			}
			returnint =  -1;
			return 0;
		}

		if (compare(ev[1], "sp_brain")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].brain);
				return 0;
			}
			returnint =  -1;
			return 0;
		}

		if (compare(ev[1], "sp_exp")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].exp);
				return 0;
			}
			returnint =  -1;
			return 0;
		}
		if (compare(ev[1], "set_button")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {

				play.button[nlist[0]] = nlist[1];

				return 0;
			}
			returnint =  -1;
			return 0;
		}


		if (compare(ev[1], "sp_reverse")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].reverse);
				return 0;
			}
			returnint =  -1;
			return 0;
		}


		if (compare(ev[1], "sp_noclip")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].noclip);
				return 0;
			}
			returnint =  -1;
			return 0;
		}


		if (compare(ev[1], "sp_touch_damage")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite_noreturn(nlist[0], nlist[1], &spr[nlist[0]].touch_damage);
				return 0;
			}
			returnint =  -1;
			return 0;
		}



		if (compare(ev[1], "sp_brain_parm")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].brain_parm);
				return 0;
			}
			returnint =  -1;
			return 0;
		}
		if (compare(ev[1], "sp_brain_parm2")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].brain_parm2);
				return 0;
			}
			returnint =  -1;
			return 0;
		}

		if (compare(ev[1], "sp_follow")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].follow);
				return 0;
			}
			returnint =  -1;
			return 0;
		}

		if (compare(ev[1], "set_smooth_follow")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				if (nlist[0] == 0) {
					smooth_follow = false;
				} else if (nlist[0] == 1) {
					smooth_follow = true;
				}
				return 0;
			}
			returnint =  -1;
			return 0;
		}

		if (compare(ev[1], "sp_frame")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].frame);
				return 0;
			}
			returnint =  -1;
			return 0;
		}

		if (compare(ev[1], "sp_frame_delay")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].frame_delay);
				return 0;
			}
			returnint =  -1;
			return 0;
		}


		if (compare(ev[1], "hurt")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				//redink1 fix for freeze if hurt value is less than 0
				if (nlist[1] < 0)
					return 0;
				if (hurt_thing(nlist[0], nlist[1], 0) > 0)
					random_blood(spr[nlist[0]].x, spr[nlist[0]].y - 40, nlist[0]);
				if (spr[nlist[0]].nohit != 1)
					if (spr[nlist[0]].script != 0)

						if (locate(spr[nlist[0]].script, "HIT")) {

							if (rinfo[script]->sprite != 1000) {
								*penemy_sprite = rinfo[script]->sprite;
								//redink1 addition of missle_source stuff
								*pmissle_source = rinfo[script]->sprite;
							}

							kill_returning_stuff(spr[nlist[0]].script);
							run_script(spr[nlist[0]].script);
						}

				return 0;
			}
			returnint =  -1;
			return 0;
		}







		if (compare(ev[1], "sp_hard")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].hard);
				if (spr[nlist[0]].sp_index != 0) if (nlist[1] != -1) {

						pam.sprite[spr[nlist[0]].sp_index].hard = returnint;
					}
				return 0;
			}
			returnint =  -1;
			return 0;
		}

		if (compare(ev[1], "sp_move_nohard")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].move_nohard);
				return 0;
			}
			returnint =  -1;
			return 0;
		}
		if (compare(ev[1], "sp_flying")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].flying);
				return 0;
			}
			returnint =  -1;
			return 0;
		}




		if (compare(ev[1], "sp_kill_wait")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				spr[nlist[0]].wait = 0;
				return 0;
			}
			returnint =  -1;
			return 0;
		}
		if (compare(ev[1], "sp_kill")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				spr[nlist[0]].kill = nlist[1];
				return 0;
			}
			returnint =  -1;
			return 0;
		}

		if (compare(ev[1], "screenlock")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				if (nlist[0] == 0 || nlist[0] == 1) {
					screenlock = nlist[0];
				}
			}
			//redink1 - set screenlock() to return the screenlock value
			returnint = screenlock;
			return 0;
		}

		if (compare(ev[1], "stop_entire_game")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				stop_entire_game = nlist[0];




				while (1) {

					RECT rcRect;

					SetRect(&rcRect, 0, 0, 640, 480);
					ddrval = lpDDSTwo->BltFast(0, 0, lpDDSBack,
					                           &rcRect, DDBLTFAST_NOCOLORKEY);

					if (ddrval == DD_OK) {
						break;
					}
					if (ddrval != DDERR_WASSTILLDRAWING) {
						dderror(ddrval);
						return 0;
					}
				}










				return 0;
			}
			returnint =  -1;
			return 0;
		}


		if (compare(ev[1], "dink_can_walk_off_screen")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				walk_off_screen = nlist[0];
				return 0;
			}
			returnint =  -1;
			return 0;
		}

		if (compare(ev[1], "push_active")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				push_active = nlist[0];
				return 0;
			}
			returnint =  -1;
			return 0;
		}


		if (compare(ev[1], "sp_x")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].x);
				return 0;
			}
			returnint =  -1;
			return 0;
		}


		if (compare(ev[1], "count_item")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = 0;
				for (i = 1; i < 17; i++) {
					if (play.item[i].active) {
						if (compare(play.item[i].name, slist[0])) returnint++;
					}

				}

				return 0;
			}
			returnint =  -1;
			return 0;
		}


		if (compare(ev[1], "count_magic")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = 0;
				for (i = 1; i < 9; i++) {
					if (play.mitem[i].active) {
						if (compare(play.mitem[i].name, slist[0])) returnint++;
					}

				}

				return 0;
			}
			returnint =  -1;
			return 0;
		}


		if (compare(ev[1], "sp_mx")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].mx);
				return 0;
			}
			returnint =  -1;
			return 0;
		}

		if (compare(ev[1], "sp_move_x")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				change_sprite_noreturn(nlist[0], nlist[1], &spr[nlist[0]].mx);
				return 0;
			}
			return 0;
		}

		if (compare(ev[1], "sp_my")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].my);
				return 0;
			}
			returnint =  -1;
			return 0;
		}

		if (compare(ev[1], "sp_move_y")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				change_sprite_noreturn(nlist[0], nlist[1], &spr[nlist[0]].my);
				return 0;
			}
			return 0;
		}

		if (compare(ev[1], "scripts_used")) {
			h = &h[strlen(ev[1])];
			int m = 0;
			for (i = 1; i < max_scripts; i++)
				if (rinfo[i] != NULL) m++;
			returnint = m;
			return 0;
		}




		if (compare(ev[1], "sp_dir")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].dir);

				if (nlist[1] != -1) changedir(spr[nlist[0]].dir, nlist[0], spr[nlist[0]].base_walk);
				return 0;
			}
			returnint =  -1;
			return 0;
		}

		if (compare(ev[1], "sp_hitpoints")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].hitpoints);

				return 0;
			}
			returnint =  -1;
			return 0;
		}


		if (compare(ev[1], "sp_attack_hit_sound")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].attack_hit_sound);

				return 0;
			}
			returnint =  -1;
			return 0;
		}


		if (compare(ev[1], "sp_attack_hit_sound_speed")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].attack_hit_sound_speed);

				return 0;
			}
			returnint =  -1;
			return 0;
		}


		if (compare(ev[1], "sp_strength")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].strength);

				return 0;
			}
			returnint =  -1;
			return 0;
		}

		if (compare(ev[1], "sp_defense")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].defense);

				return 0;
			}
			returnint =  -1;
			return 0;
		}

		if (compare(ev[1], "init")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {

				figure_out(slist[0], 0);
				return 0;
			}
			returnint =  -1;
			return 0;
		}


		if (compare(ev[1], "sp_distance")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].distance);

				return 0;
			}
			returnint =  -1;
			return 0;
		}


		if (compare(ev[1], "sp_nohit")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].nohit);
				return 0;
			}
			returnint =  -1;
			return 0;
		}


		if (compare(ev[1], "sp_notouch")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].notouch);
				return 0;
			}
			returnint =  -1;
			return 0;
		}



		if (compare(ev[1], "compare_weapon")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = 0;
				if (*pcur_weapon == 0) {
					return 0;
				}

				if (compare(play.item[*pcur_weapon].name, slist[0])) {
					returnint = 1;

				}
				return 0;
			}


			return 0;
		}


		if (compare(ev[1], "compare_magic")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = 0;
				if (*pcur_magic == 0) {
					return 0;
				}

				//redink1 fix so compare_magic works!
				if (compare(play.mitem[*pcur_magic].name, slist[0])) {
					returnint = 1;

				}
				return 0;
			}


			return 0;
		}


		if (compare(ev[1], "compare_sprite_script")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 2, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = 0;

				if (nlist[0] == 0) {
					Msg("Error: Can't compare sprite script for sprite 0!??!?!");
					return 0;
				}
				if (spr[nlist[0]].active) {

					if (spr[nlist[0]].script == 0) {
						Msg("Compare sprite script says: Sprite %d has no script.", nlist[0]);
						return 0;
					}
					if (compare(slist[1], rinfo[spr[nlist[0]].script]->name)) {
						returnint = 1;
						return 0;
					}

				} else {
					Msg("Can't compare sprite script, sprite not active.");
				}



				return 0;
			}


			return 0;
		}


		if (compare(ev[1], "sp_y")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].y);
				return 0;
			}
			returnint =  -1;
			return 0;
		}


		if (compare(ev[1], "sp_timing")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
			if (get_parms(ev[1], script, h, ARR)) {
				returnint = change_sprite(nlist[0], nlist[1], &spr[nlist[0]].timer);
				return 0;
			}
			returnint =  -1;
			return 0;
		}


		if (compare(ev[1], "return;")) {

			if (debug_mode) Msg("Found return; statement");

			if (rinfo[script]->proc_return != 0) {
				bKeepReturnInt = true;
				run_script(rinfo[script]->proc_return);
				kill_script(script);
			}

			return 2;
		}

		//redink1 added so we can have return values and crap.
		if (compare(ev[1], "return")) {

			if (debug_mode) Msg("Found return; statement");

			h = &h[strlen(ev[1])];
			strip_beginning_spaces(h);
			process_line(script, h, false);

			if (rinfo[script]->proc_return != 0) {
				bKeepReturnInt = true;
				run_script(rinfo[script]->proc_return);
				kill_script(script);
			}

			return 2;
		}


		if (compare(ev[1], "if")) {

			h = &h[strlen(ev[1])];
			strip_beginning_spaces(h);
			//Msg("running if with string of %s", h);

			process_line(script, h, false);

			//Msg("Result is %d", returnint);

			if (returnint != 0) {
				if (debug_mode) Msg("If returned true");


			} else {
				//don't do it!
				rinfo[script]->skipnext = true;
				if (debug_mode) Msg("If returned false, skipping next thing");
			}

			//DO STUFF HERE!
			strcpy(s, h);
			//g("continuing to run line %s..", h);

			return 5;
		}



		if (compare(ev[2], "=")) {
			h = &h[strlen(ev[1])];
			strip_beginning_spaces(h);
			h = &h[1];
			strip_beginning_spaces(h);
			var_equals(ev[1], ev[3], '=', script, h);
			strcpy(s, h);
			return 0;
		}

		if (compare(ev[2], "+=")) {
			h = &h[strlen(ev[1])];
			strip_beginning_spaces(h);
			h = &h[2];
			strip_beginning_spaces(h);
			var_equals(ev[1], ev[3], '+', script, h);
			strcpy(s, h);
			return 0;
		}

		if (compare(ev[2], "*=")) {
			h = &h[strlen(ev[1])];
			strip_beginning_spaces(h);
			h = &h[2];
			strip_beginning_spaces(h);
			var_equals(ev[1], ev[3], '*', script, h);
			strcpy(s, h);
			return 0;
		}



		if (compare(ev[2], "-=")) {
			h = &h[strlen(ev[1])];
			strip_beginning_spaces(h);
			h = &h[2];
			strip_beginning_spaces(h);

			var_equals(ev[1], ev[3], '-', script, h);

			strcpy(s, h);
			return 0;
		}


		if (compare(ev[2], "/") || compare(ev[2], "/=")) {
			h = &h[strlen(ev[1])];
			strip_beginning_spaces(h);
			h = &h[1];
			strip_beginning_spaces(h);

			var_equals(ev[1], ev[3], '/', script, h);

			strcpy(s, h);
			return 0;
		}

		if (compare(ev[2], "*") || compare(ev[2], "*=")) {
			h = &h[strlen(ev[1])];
			strip_beginning_spaces(h);
			h = &h[1];
			strip_beginning_spaces(h);

			var_equals(ev[1], ev[3], '*', script, h);

			strcpy(s, h);
			return 0;
		}
		if (compare(ev[1], "external")) {
			h = &h[strlen(ev[1])];
			int ARR[20] = {2, 2, 1, 1, 1, 1, 1, 1, 1, 1};
			memset(slist, 0, 10 * 200);
			get_parms(ev[1], script, h, ARR);
			if (strlen(slist[0]) > 0 && strlen(slist[1]) > 0) {
				int myscript1 = load_script(slist[0], rinfo[script]->sprite, false);
				if (myscript1 == 0) {
					Msg("Error:  Couldn't find %s.c (for procedure %s)", slist[0], slist[1]);
					return 0;
				}
				rinfo[myscript1]->arg1 = nlist[2];
				rinfo[myscript1]->arg2 = nlist[3];
				rinfo[myscript1]->arg3 = nlist[4];
				rinfo[myscript1]->arg4 = nlist[5];
				rinfo[myscript1]->arg5 = nlist[6];
				rinfo[myscript1]->arg6 = nlist[7];
				rinfo[myscript1]->arg7 = nlist[8];
				rinfo[myscript1]->arg8 = nlist[9];
				if (locate(myscript1, slist[1])) {
					rinfo[myscript1]->proc_return = script;
					run_script(myscript1);

					return 2;
				} else {
					Msg("Error:  Couldn't find procedure %s in %s.", slist[1], slist[0]);
					kill_script(myscript1);
				}
			}
			strcpy(s, h);
			return 0;
		}


		if (strchr(h, '(') != NULL) {

			//lets attempt to run a procedure

			int myscript = load_script(rinfo[script]->name, rinfo[script]->sprite, false);

			h = &h[strlen(ev[1])];

			int ARR[20] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
			get_parms(ev[1], script, h, ARR);

			if (locate(myscript, ev[1])) {
				rinfo[myscript]->arg1 = nlist[0];
				rinfo[myscript]->arg2 = nlist[1];
				rinfo[myscript]->arg3 = nlist[2];
				rinfo[myscript]->arg4 = nlist[3];
				rinfo[myscript]->arg5 = nlist[4];
				rinfo[myscript]->arg6 = nlist[5];
				rinfo[myscript]->arg7 = nlist[6];
				rinfo[myscript]->arg8 = nlist[7];
				rinfo[myscript]->arg9 = nlist[8];

				rinfo[myscript]->proc_return = script;
				run_script(myscript);
				return 2;
			} else {
				for (i = 0; strlen(play.func[i].func) > 0 && i < 100; i++) {
					if (compare(play.func[i].func, ev[1])) {
						myscript = load_script(play.func[i].file, rinfo[script]->sprite, false);
						rinfo[myscript]->arg1 = nlist[0];
						rinfo[myscript]->arg2 = nlist[1];
						rinfo[myscript]->arg3 = nlist[2];
						rinfo[myscript]->arg4 = nlist[3];
						rinfo[myscript]->arg5 = nlist[4];
						rinfo[myscript]->arg6 = nlist[5];
						rinfo[myscript]->arg7 = nlist[6];
						rinfo[myscript]->arg8 = nlist[7];
						rinfo[myscript]->arg9 = nlist[8];
						if (locate(myscript, ev[1])) {
							rinfo[myscript]->proc_return = script;
							run_script(myscript);
							return 2;
						}
						break;
					}
				}
				Msg("ERROR:  Procedure void %s( void ); not found in script %s. (word 2 was %s) ", line,
				    ev[2], rinfo[myscript]->name);
				kill_script(myscript);
			}

			/*seperate_string(h, 1,'(',line);


			int myscript = load_script(rinfo[script]->name, rinfo[script]->sprite, false);

			if (locate( myscript, line))
			{
			rinfo[myscript]->proc_return = script;
			run_script(myscript);
			return 2;
			} else
			{
			Msg("ERROR:  Procedure void %s( void ); not found in script %s. (word 2 was %s) ", line,
			ev[2], rinfo[myscript]->name);
			kill_script(myscript);
			}*/


			return 0;

		}






		Msg("MERROR: \"%s\" unknown in %s, offset %d.", ev[1], rinfo[script]->name, rinfo[script]->current);


		//in a thingie, ready to go

	}

bad:
	strcpy(s, h);
	return 0;

good:
	strcpy(s, h);
	//s = h
	//Msg("ok, continuing with running %s..",s);
	return 1;

}


void run_script(int script) {
	int result;
	char line[200];

	if (bKeepReturnInt) {
		bKeepReturnInt = false;
	} else {
		returnint = 0;
	}

	returnstring[0] = 0;
	if (rinfo[script] != NULL) {
		if (debug_mode)
			Msg("Script %s is entered at offset %d.", rinfo[script]->name, rinfo[script]->current);
	} else {
		Msg("Error:  Tried to run a script that doesn't exist in memory.  Nice work.");
	}

	while (read_next_line(script, line)) {
		while (1) {
			strip_beginning_spaces(line);
			if (compare(line, "\n"))
				break;

			result = process_line(script, line, false);
			if (result == 3) {
redo:
				read_next_line(script, line);
crappa:
				strip_beginning_spaces(line);
				if (compare(line, "\n")) goto redo;
				if (compare(line, "\\\\")) goto redo;
				//       Msg("processing %s knowing we are going to skip it...", line);
				result = process_line(script, line, true);
			}

			if (result == 5)
				goto crappa;
			if (result == 3)
				goto redo;

			if (result == 2) {
				if (debug_mode) Msg("giving script the boot");
				// Quit script
				return;
			}
			if (result == 0)
				break;

			if (result == 4) {
				if (strlen(line) < 2) {
redo2:
					read_next_line(script, line);
					strip_beginning_spaces(line);
					//Msg("Comparing to %s.", line);
					if (compare(line, "\n")) goto redo2;
					if (compare(line, "\\\\")) goto redo2;
				}
				result = process_line(script, line, true);
			}

			if (result == 2) {
				if (debug_mode) Msg("giving script the boot");
				// Quit script
				return;
			}
			if (result == 0)
				break;
		}
	}

	if (rinfo[script] != NULL) {
		if (rinfo[script]->proc_return != 0) {
			run_script(rinfo[script]->proc_return);
			kill_script(script);
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




void init_scripts() {
	for (int k = 1; k < max_scripts; k++) {
		if (rinfo[k] != NULL && rinfo[k]->sprite != 0 && rinfo[k]->sprite < 300 && spr[rinfo[k]->sprite].active) {
			if (locate(k, "main")) {
				if (debug_mode) Msg("Screendraw: running main of script %s..", rinfo[k]->name);
				run_script(k);

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
