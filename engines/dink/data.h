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

#ifndef DINK_DATA_H
#define DINK_DATA_H

#include "common/file.h"
#include "dink/types.h"
#include "dink/lib/rect.h"

namespace Dink {

/**
 * Sub-structure for tile hardness
 */
struct block_y {
	byte y[51] = { 0 };

	void load(Common::SeekableReadStream &s);
};

struct ts_block {
	block_y x[51];
	bool used = false;
	int hold = 0;

	void load(Common::SeekableReadStream &s);
};

/**
 * Structure for hardness info, INDEX controls which
 * hardness each block has. 800 max types available.
 */
struct hardness {
	ts_block tile[800];
	int index[8000];

	static void load();
	void load(Common::SeekableReadStream &s);
};


struct map_info {
	char name[20];
	int loc[769];
	int music[769];
	int indoor[769];
	int v[40];
	char s[80];
	char buffer[2000];

	static void load();
	void load(Common::SeekableReadStream &s);
};


struct sp {
	int x, moveman;
	int y;
	int mx, my;
	int lpx[51], lpy[51];
	int speed;
	int brain;
	int seq_orig, dir;
	int seq;
	int frame;
	uint32 delay;
	int pseq;
	int pframe;

	bool active;
	int attrib;
	uint32 wait;
	int timer;
	int skip;
	int skiptimer;
	int size;
	int que;
	int base_walk;
	int base_idle;
	int base_attack;

	int base_hit;
	int last_sound;
	int hard;
	RECT alt;
	int althard;
	int sp_index;
	bool nocontrol;
	int idle;
	int strength;
	int damage;
	int defense;
	int hitpoints;
	int exp;
	int gold;
	int base_die;
	int kill;
	int kill_timer;
	int script_num;
	char text[200];
	int owner;
	int script;
	int sound;
	int callback;
	int freeze;
	bool move_active;
	int move_script;
	int move_dir;
	int move_num;
	bool move_nohard;
	int follow;
	int nohit;
	bool notouch;
	uint32 notouch_timer;
	bool flying;
	int touch_damage;
	int brain_parm;
	int brain_parm2;
	bool noclip;
	bool reverse;
	bool disabled;
	int target;
	int attack_wait;
	int move_wait;
	int distance;
	int last_hit;
	bool live;
	int range;
	int attack_hit_sound;
	int attack_hit_sound_speed;
	int action;
	int nodraw;
	int frame_delay;
	int picfreeze;
	//redink1
	int bloodseq;
	int bloodnum;
	Common::HashMap<Common::String, int> *custom;

	void clear();
};

} // namespace Dink

#endif
