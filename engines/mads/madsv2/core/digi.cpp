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

#include "common/textconsole.h"
#include "mads/madsv2/core/digi.h"

namespace MADS {
namespace MADSV2 {

bool digi_trigger_dialog;
bool digi_trigger_ambiance;
bool digi_trigger_effect;


void digi_install() {
	// No implementation in ScummVM
}

void digi_uninstall() {
	// No implementation in ScummVM
}

void digi_play(char name[30], int slot) {
	warning("TODO: digi_play");
}

void digi_play_build(int room, char thing, int num, int slot) {
	warning("TODO: digi_play_build");
}

void digi_play_build_ii(char thing, int num, int slot) {
	warning("TODO: digi_play_build_ii");
}

void digi_stop(int which_one) {
	warning("TODO: digi_stop");
}

void digi_read_another_chunk() {
	warning("TODO: digi_read_another_chunk");
}

void digi_initial_volume(int vol) {
	warning("TODO: digi_initial_volume");
}

void digi_set_volume(int vol, int slot) {
	warning("TODO: digi_set_volume");
}

} // namespace MADSV2
} // namespace MADS
