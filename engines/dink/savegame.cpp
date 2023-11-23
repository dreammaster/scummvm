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

#include "dink/file.h"

namespace Dink {

void load_map(int num) {
#ifdef TODO
	FILE *fp;
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

bool add_time_to_saved_game(int num) {
#ifdef TODO
	FILE *fp;
	char crap[80];

	dink_sprintf(crap, "SAVE%d.DAT", num);

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



	dink_sprintf(crap, "SAVE%d.DAT", num);
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
	FILE *fp;
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

	dink_sprintf(crap, "SAVE%d.DAT", num);



	fp = fopen(crap, "rb");
	if (!fp) {
		Msg("Couldn't load save game %d", num);
		return false;
	} else {

		fread(&play, sizeof(play), 1, fp);
		fclose(fp);

		//redink1 - new map, if exist
		if (strlen(play.mapdat) > 0 && strlen(play.dinkdat) > 0) {
			dink_strcpy(current_map, play.mapdat);
			dink_strcpy(current_dat, play.dinkdat);
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
					dink_sprintf(tile, "..\\DINK\\%s", play.tile[i].file);
				else
					dink_strcpy(tile, play.tile[i].file);
			} else {
				dink_sprintf(tile, "tiles\\TS%02d.bmp", i);
				if (!File::exists(tile))
					dink_sprintf(tile, "..\\dink\\tiles\\TS%02d.BMP", i);
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

void save_map(const int num) {
#ifdef TODO
	FILE *fp;
	long holdme, lsize;
	char crap[80];


	Msg("Saving map data..");
	dink_strcpy(crap, current_map);
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
	FILE *fp;
	char crap[80];
	//redink1
	dink_sprintf(crap, current_dat);
	fp = fopen(crap, "wb");
	fwrite(&map, sizeof(struct map_info), 1, fp);
	fclose(fp);
#else
	error("TODO");
#endif
}

void save_game(int num) {
#ifdef TODO
	FILE *fp;
	char crap[80];
	//redink1 created this
	char info_temp[200];

	//lets set some vars first

	play.x = spr[1].x;
	play.y = spr[1].y;
	play.version = dversion;
	play.pseq = spr[1].pseq;
	play.pframe = spr[1].pframe;
	play.seq = spr[1].seq;
	play.frame = spr[1].frame;
	play.size = spr[1].size;
	play.dir = spr[1].dir;
	play.strength = spr[1].strength;
	play.defense = spr[1].defense;
	play.que = spr[1].que;
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
	dink_strcpy(info_temp, save_game_info);
	decipher_string(info_temp, 0);
	strncpy(play.gameinfo, info_temp, 77);
	//dink_sprintf(play.gameinfo, "Level %d",*plevel);


	last_saved_game = num;
	dink_sprintf(crap, "SAVE%d.DAT", num);
	fp = fopen(crap, "wb");
	fwrite(&play, sizeof(play), 1, fp);
	fclose(fp);
#else
	error("TODO");
#endif
}

} // namespace Dink
