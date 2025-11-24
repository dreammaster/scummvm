
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

#ifndef AGS2_DATA_CHARACTER_INFO_H
#define AGS2_DATA_CHARACTER_INFO_H

#include "common/serializer.h"
#include "ags2/data/defines.h"

namespace AGS2 {

#define MAX_PARSER_WORD_LENGTH 30

struct CharacterInfo {
	int   defview;
	int   talkview;
	int   view;
	int   room, prevroom;
	int   x, y, wait;
	int   flags;
	short following;
	short followinfo;
	int   idleview;           // the loop will be randomly picked
	short idletime, idleleft; // num seconds idle before playing anim
	short transparency;       // if character is transparent
	short baseline;
	int   activeinv;
	int   talkcolor;
	int   thinkview;
	short blinkview, blinkinterval; // design time
	short blinktimer, blinkframe;   // run time
	short walkspeed_y, pic_yoffs;
	int   z;    // z-location, for flying etc
	int   walkwait;
	short speech_anim_speed, reserved1;  // only 1 reserved left!!
	short blocking_width, blocking_height;
	int   index_id;  // used for object functions to know the id
	short pic_xoffs, walkwaitcounter;
	short loop, frame;
	short walking, animating;
	short walkspeed, animspeed;
	short inv[MAX_INV];
	short actx, acty;
	char  name[40];
	char  scrname[MAX_SCRIPT_NAME_LEN];
	char  on;

	int get_effective_y();   // return Y - Z
	int get_baseline();      // return baseline, or Y if not set
	int get_blocking_top();    // return Y - BlockingHeight/2
	int get_blocking_bottom(); // return Y + BlockingHeight/2

	void synchronize(Common::Serializer &s);
	void load(Common::SeekableReadStream *rs) {
		Common::Serializer s(rs, nullptr);
		synchronize(s);
	}
	void save(Common::WriteStream *ws) {
		Common::Serializer s(nullptr, ws);
		synchronize(s);
	}
};

struct CharacterExtras {
	// UGLY UGLY UGLY!! The CharacterInfo struct size is fixed because it's
	// used in the scripts, therefore overflowing stuff has to go here
	short invorder[MAX_INVORDER];
	short invorder_count;
	short width, height;
	short zoom;
	short xwas, ywas;
	short tint_r, tint_g;
	short tint_b, tint_level;
	short tint_light;
	char  process_idle_this_time;
	char  slow_move_counter;
	short animwait;

	void synchronize(Common::Serializer &s);
	void load(Common::SeekableReadStream *rs) {
		Common::Serializer s(rs, nullptr);
		synchronize(s);
	}
	void save(Common::WriteStream *ws) {
		Common::Serializer s(nullptr, ws);
		synchronize(s);
	}
};

} // namespace AGS2

#endif
