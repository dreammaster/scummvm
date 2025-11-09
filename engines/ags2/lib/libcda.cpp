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

#include "ags2/lib/libcda.h"

namespace AGS2 {

int cd_init() {
	return 0;
}

void cd_exit() {
}

int cd_play(int track) {
	return 0;
}

int cd_play_range(int start, int end) {
	return 0;
}

int cd_play_from(int track) {
	return 0;
}

int cd_current_track() {
	return 0;
}

void cd_pause() {
}

void cd_resume() {
}

int cd_is_paused() {
	return 0;
}

void cd_stop() {
}

int cd_get_tracks(int *first, int *last) {
	return 0;
}

int cd_is_audio(int track) {
	return 0;
}

void cd_get_volume(int *c0, int *c1) {
}

void cd_set_volume(int c0, int c1) {
}

void cd_eject() {
}

void cd_close() {
}

} // namespace AGS2
