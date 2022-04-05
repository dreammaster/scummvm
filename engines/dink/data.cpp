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

#include "dink/data.h"
#include "dink/file.h"
#include "dink/globals.h"

namespace Dink {

void hardness::load() {
	File f;

	if (f.open("hard.dat")) {
		hmap.load(f);
	} else {
		ZeroMemory(&hmap, sizeof(hardness));
	}
}

void hardness::load(Common::SeekableReadStream &src) {
	for (int i = 0; i < 800; ++i)
		tile[i].load(src);
	for (int i = 0; i < 8000; ++i)
		index[i] = src.readUint32LE();
}

void ts_block::load(Common::SeekableReadStream &src) {
	for (int i = 0; i < 51; ++i)
		x[i].load(src);
	used = src.readByte() != 0;
	hold = src.readSint32LE();
}

void block_y::load(Common::SeekableReadStream &src) {
	src.read(y, 51);
}

void map_info::load() {
	File f;

	if (f.open(current_dat)) {
		map.load(f);
	} else {
		ZeroMemory(&hmap, sizeof(hardness));
		strcpy(map.name, "Smallwood");
	}
}

void map_info::load(Common::SeekableReadStream &src) {
	src.read(name, 20);
	for (int i = 0; i < 769; ++i)
		loc[i] = src.readSint32LE();
	for (int i = 0; i < 769; ++i)
		music[i] = src.readSint32LE();
	for (int i = 0; i < 769; ++i)
		indoor[i] = src.readSint32LE();
	for (int i = 0; i < 40; ++i)
		v[i] = src.readSint32LE();
	src.read(s, 80);
	src.read(buffer, 2000);
}

void sp::clear() {
	x = y = 0;
	moveman = 0;
	mx = my = 0;
	Common::fill(lpx, lpx + 51, 0);
	Common::fill(lpy, lpy + 51, 0);
	speed = 0;
	brain = 0;
	seq_orig = dir = 0;
	seq = 0;
	frame = 0;
	delay = 0;
	pseq = 0;
	pframe = 0;
	active = false;
	attrib = 0;
	wait = 0;
	timer = 0;
	skip = 0;
	skiptimer = 0;
	size = 0;
	que = 0;
	base_walk = 0;
	base_idle = 0;
	base_attack = 0;

	base_hit = 0;
	last_sound = 0;
	hard = 0;
	alt.clear();
	althard = 0;
	sp_index = 0;
	nocontrol = false;
	idle = 0;
	strength = 0;
	damage = 0;
	defense = 0;
	hitpoints = 0;
	exp = 0;
	gold = 0;
	base_die = 0;
	kill = 0;
	kill_timer = 0;
	script_num = 0;
	Common::fill(text, text + 200, '\0');
	owner = 0;
	script = 0;
	sound = 0;
	callback = 0;
	freeze = 0;
	move_active = false;
	move_script = 0;
	move_dir = 0;
	move_num = 0;
	move_nohard = false;
	follow = 0;
	nohit = 0;
	notouch = false;
	notouch_timer = 0;
	flying = false;
	touch_damage = 0;
	brain_parm = 0;
	brain_parm2 = 0;
	noclip = false;
	reverse = false;
	disabled = false;
	target = 0;
	attack_wait = 0;
	move_wait = 0;
	distance = 0;
	last_hit = 0;
	live = false;
	range = 0;
	attack_hit_sound = 0;
	attack_hit_sound_speed = 0;
	action = 0;
	nodraw = 0;
	frame_delay = 0;
	picfreeze = 0;
	bloodseq = 0;
	bloodnum = 0;
	custom = nullptr;
}

} // namespace Dink
