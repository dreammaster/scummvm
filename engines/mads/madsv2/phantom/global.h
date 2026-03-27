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

#ifndef MADS_PHANTOM_GLOBAL_H
#define MADS_PHANTOM_GLOBAL_H

#include "mads/madsv2/core/vocabh.h"

namespace MADS {
namespace MADSV2 {
namespace Phantom {

enum {
	conv001_speech_talk = 0,
	conv001_continue_abc = 1,
	conv001_what_one = 4,
	conv001_yesno_yes = 8,
	conv001_everything_byebye = 10,
	conv001_everything_copycat = 12,
	conv001_speak_byebye = 18,
	conv001_saytwo_1 = 22,
	conv001_instructions_three = 24,
	conv001_point_two_abc = 27
};


// Global indices
#define walker_converse		 4
#define current_year		10
#define brie_talk_status	24
#define jacques_status		31

// brie_talk_status values
#define BEFORE_BRIE_MOTIONS			0
#define BEFORE_CHANDELIER_CONV		1
#define AFTER_CONVS_0_AND_1			2

// jacques_status values
#define JACQUES_IS_DEAD				1
#define JACQUES_IS_DEAD_RICH_GONE	2

// walker_converse values
#define CONVERSE_NONE           0
#define CONVERSE_LEAN           1
#define CONVERSE_HAND_WAVE      2
#define CONVERSE_HAND_WAVE_2    3
#define CONVERSE_HAND_CHIN      4

} // namespace Phantom
} // namespace MADSV2
} // namespace MADS

#endif
