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

#ifndef SPYCRAFT_DMADE_ADVMOVIE_H
#define SPYCRAFT_DMADE_ADVMOVIE_H

namespace Spycraft {

typedef void (*callbackFn)();

extern int InitMovie();
extern void CleanMovie();
extern int sfxIsMovieOpen();
extern int sfxIsMoviePlaying();
extern int sfxSetMovieRange(int from, int to);
extern int sfxOpenMovie(const char *filename, int dx, int dy, int size);
extern int sfxPlayMovie(int close_done, int erase_done, callbackFn theCallBack);
extern int sfxCloseMovie();
extern int sfxPauseMovie(int repaint, int doCallBack);
extern int sfxResumeMovie();
extern int sfxGetMoviePosn();
extern int sfxEnableMovieAudio(int tOrF);
extern int sfxEnableMovieVideo(int tOrF);
extern int sfxStepMovie(int location);

void UpdateMovie();

} // namespace Spycraft

#endif
