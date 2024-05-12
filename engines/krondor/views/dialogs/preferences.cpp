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

#include "krondor/views/dialogs/preferences.h"

namespace Krondor {
namespace Views {
namespace Dialogs {

enum {
	PREF_CANCEL = 46,
	PREF_DEFAULTS = 32,
	PREF_OK = 24,

	PREF_STEP_SMALL = 192,
	PREF_STEP_MEDIUM = 193,
	PREF_STEP_LARGE = 194,

	PREF_TURN_SMALL = 195,
	PREF_TURN_MEDIUM = 196,
	PREF_TURN_LARGE = 197,

	PREF_DETAIL_MIN = 198,
	PREF_DETAIL_LOW = 199,
	PREF_DETAIL_HIGH = 200,
	PREF_DETAIL_MAX = 201,

	PREF_TEXT_WAIT = 202,
	PREF_TEXT_MEDIUM = 203,
	PREF_TEXT_FAST = 204,

	PREF_SOUND = 205,
	PREF_MUSIC = 206,
	PREF_COMBAT_MUSIC = 207,
	PREF_INTRODUCTION = 208,
	PREF_CD_MUSIC = 209
};

Preferences::Preferences() : Dialog("Preferences",
	"req_pref.dat", "options.pal", "options2.scx",
	"game.fnt", "lbl_pref.dat") {
}

} // namespace Dialogs
} // namespace Views
} // namespace Krondor
