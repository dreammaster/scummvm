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

#ifndef AGS2_LIB_LIBCDA_H
#define AGS2_LIB_LIBCDA_H

#include "common/scummsys.h"

namespace AGS2 {

extern int cd_init(void);
extern void cd_exit(void);

extern int cd_play(int track);
extern int cd_play_range(int start, int end);
extern int cd_play_from(int track);
extern int cd_current_track(void);
extern void cd_pause(void);
extern void cd_resume(void);
extern int cd_is_paused(void);
extern void cd_stop(void);

extern int cd_get_tracks(int *first, int *last);
extern int cd_is_audio(int track);

extern void cd_get_volume(int *c0, int *c1);
extern void cd_set_volume(int c0, int c1);

extern void cd_eject(void);
extern void cd_close(void);


} // namespace AGS2

#endif
