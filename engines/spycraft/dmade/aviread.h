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

#ifndef SPYCRAFT_DMADE_ADV_AVIREAD_H
#define SPYCRAFT_DMADE_ADV_AVIREAD_H

namespace Spycraft {

enum {
	ADV_MOVIE_RATIO,
	ADV_MOVIE_FROM,
	ADV_MOVIE_TO,
	ADV_MOVIE_ERASE,
	ADV_MOVIE_CLOSE,
	ADV_MOVIE_SKIP,
	ADV_MOVIE_LOOPCOUNT
};

struct MovieInfo {
	int		ratio;
	int		from;
	int		to;
	int 		erase;
	int		close;
	int		skip;
	int		loopCount;
};

extern void sfxSetMovieInfo(int type, int value);
extern void sfxStopBufferMovie(void);
extern void sfxBufferMovie(char *filename, int sx, int sy, int channel, void (*movieCallBack)(void));
extern void UpdateBufferMovie(void);

} // namespace Spycraft

#endif
