
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
#include "ags2/data/game_setup_struct_base.h"

namespace AGS2 {


void GameSetupStructBase::synchronize(Common::Serializer &s) {
	int i;

	s.syncBytes((byte *)gamename, 50);
	for (i = 0; i < 100; ++i)
		s.syncAsUint32LE(options[i]);
	s.syncBytes(paluses, 256);

	for (i = 0; i < 256; ++i)
		defpal[i].synchronize(s);

	s.syncAsSint32LE(numviews);
	s.syncAsSint32LE(numcharacters);
	s.syncAsSint32LE(playercharacter);
	s.syncAsSint32LE(totalscore);
	s.syncAsSint16LE(numinvitems);
	s.syncAsSint32LE(numdialog);
	s.syncAsSint32LE(numdlgmessage);
	s.syncAsSint32LE(numfonts);
	s.syncAsSint32LE(color_depth);
	s.syncAsSint32LE(target_win);
	s.syncAsSint32LE(dialog_bullet);
	s.syncAsSint16LE(hotdot);
	s.syncAsSint16LE(hotdotouter);
	s.syncAsSint32LE(uniqueid);
	s.syncAsSint32LE(numgui);
	s.syncAsSint32LE(numcursors);
	s.syncAsSint32LE(default_resolution);
	s.syncAsSint32LE(default_lipsync_frame);
	s.syncAsSint32LE(invhotdotsprite);

	for (i = 0; i < 17; ++i)
		s.syncAsSint32LE(reserved[i]);

	Common::fill(messages, messages + MAXGLOBALMES, nullptr);
	s.skip(4 * MAXGLOBALMES);

	if (s.isLoading()) {
		dict = nullptr;
		globalscript = nullptr;
		chars = nullptr;
		compiled_script = nullptr;
	}
	s.skip(4 * 4);
}

} // namespace AGS2
