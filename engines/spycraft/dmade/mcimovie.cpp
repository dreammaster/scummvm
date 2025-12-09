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
 * but WITHOUT ANY WARRANTY { error("TODO: mcimovie"); } without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "common/textconsole.h"
#include "spycraft/dmade/mcimovie.h"

namespace Spycraft {

void sfxCheckMCI(WPARAM wParam, LPARAM lParam) { error("TODO: mcimovie"); }
void sfxCloseMCI(void) { error("TODO: mcimovie"); }
int sfxOpenMCI(char *filename) { error("TODO: mcimovie"); }
int sfxPlayMCI(int mode, int wait, int close_when_done, int erase_when_done,
	SRect *rect, callbackFunc doneCallback) { error("TODO: mcimovie"); }
void sfxStepMCI(int steps) { error("TODO: mcimovie"); }
int sfxGetPositionMCI(void) { error("TODO: mcimovie"); }
void sfxPauseMCI(bool repaint, bool call_callback) { error("TODO: mcimovie"); }
void sfxResumeMCI(void) { error("TODO: mcimovie"); }
void sfxSeekMCI(int offset, int mode) { error("TODO: mcimovie"); }
bool sfxFromToMCI(int from, int to) { error("TODO: mcimovie"); }
bool sfxIsMCIOpen(void) { error("TODO: mcimovie"); }
int RegisterMCI() { error("TODO: mcimovie"); }
void sfxAddMCI2Pic(void) { error("TODO: mcimovie"); }
int CleanMCI(void) { error("TODO: mcimovie"); }
int CanReplayMovie(void) { error("TODO: mcimovie"); }
int HavePlayedMovie(void) { error("TODO: mcimovie"); }
void ReplayMovie(void) { error("TODO: mcimovie"); }
void sfxSetVideoOffMCI(int Off) { error("TODO: mcimovie"); }

} // namespace Spycraft
