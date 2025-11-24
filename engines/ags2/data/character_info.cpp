
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

#include "common/str.h"
#include "ags2/data/character_info.h"

namespace AGS2 {

void CharacterInfo::synchronize(Common::Serializer &s) {
	s.syncAsSint32LE(defview);
	s.syncAsSint32LE(talkview);
	s.syncAsSint32LE(view);
	s.syncAsSint32LE(room);
	s.syncAsSint32LE(prevroom);
	s.syncAsSint32LE(x);
	s.syncAsSint32LE(y);
	s.syncAsSint32LE(wait);
	s.syncAsUint32LE(flags);
	s.syncAsSint16LE(following);
	s.syncAsSint16LE(followinfo);
	s.syncAsSint32LE(idleview);
	s.syncAsSint16LE(idletime);
	s.syncAsSint16LE(idleleft);
	s.syncAsSint16LE(transparency);
	s.syncAsSint16LE(baseline);
	s.syncAsSint32LE(activeinv);
	s.syncAsSint32LE(talkcolor);
	s.syncAsSint32LE(thinkview);

	s.syncAsSint16LE(blinkview);
	s.syncAsSint16LE(blinkinterval);
	s.syncAsSint16LE(blinktimer);
	s.syncAsSint16LE(blinkframe);
	s.syncAsSint16LE(walkspeed_y);
	s.syncAsSint16LE(pic_yoffs);

	s.syncAsSint32LE(z);
	s.syncAsSint32LE(walkwait);
	s.syncAsSint32LE(speech_anim_speed);
	s.syncAsSint32LE(reserved1);
	s.syncAsSint32LE(blocking_width);
	s.syncAsSint32LE(blocking_height);
	s.syncAsSint32LE(index_id);

	s.syncAsSint16LE(pic_xoffs);
	s.syncAsSint16LE(walkwaitcounter);
	s.syncAsSint16LE(loop);
	s.syncAsSint16LE(frame);
	s.syncAsSint16LE(walking);
	s.syncAsSint16LE(animating);
	s.syncAsSint16LE(walkspeed);
	s.syncAsSint16LE(animspeed);

	for (int i = 0; i < MAX_INV; ++i)
		s.syncAsSint16LE(inv[i]);
	s.syncAsSint16LE(actx);
	s.syncAsSint16LE(acty);
	s.syncBytes((byte *)name, 40);
	s.syncBytes((byte *)scrname, MAX_SCRIPT_NAME_LEN);
	s.syncAsByte(on);
}

/*------------------------------------------------------------------*/

void CharacterExtras::synchronize(Common::Serializer &s) {
	int i;

	for (i = 0; i < MAX_INVORDER; ++i)
		s.syncAsSint16LE(invorder[i]);

	s.syncAsSint16LE(invorder_count);
	s.syncAsSint16LE(width);
	s.syncAsSint16LE(height);
	s.syncAsSint16LE(zoom);
	s.syncAsSint16LE(xwas);
	s.syncAsSint16LE(ywas);
	s.syncAsSint16LE(tint_r);
	s.syncAsSint16LE(tint_g);
	s.syncAsSint16LE(tint_b);
	s.syncAsSint16LE(tint_level);
	s.syncAsSint16LE(tint_light);
	s.syncAsByte(process_idle_this_time);
	s.syncAsByte(slow_move_counter);
	s.syncAsSint16LE(animwait);
}

} // namespace AGS2
