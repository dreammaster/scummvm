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

#ifndef SPYCRAFT_GAME_MOVIE_H
#define SPYCRAFT_GAME_MOVIE_H

#include "spycraft/game/object.h"
#include "spycraft/game/made.h"

namespace Spycraft {

#define MOVIE_X	0  //64
#define MOVIE_Y	16
#define MOVIE_WIDTH 512
#define MOVIE_HEIGHT 384

#define LOOP_INFINITE -1

#ifdef DIRECTX
#define  sfxCloseMCI             sfxCloseMovie
#define  sfxSetVideoOffMCI(T)  sfxEnableMovieVideo(!(T))
#endif

class Movie : public Object {
public:
	int	movieNum;
	const char *movieName;
	Object *caller;
	int		x;
	int		y;
	int 	isOpen;
	int 	paused;
	int 	lastPosn;
	int		soundNum;
	int     closeWhenDone;
	int 	isPlaying;

	Movie();

	void cue(void);
	void play(const char *theMovie, int theX, int theY, int theWidth, int theHeight, Object *whoToCue);
	void play(const char *theMovie, Object *whoToCue);
	void play(const char *theMovie, Object *whoToCue, int wait, int close, int erase);
	void play(const char *theMovie, Object *whoToCue, int close); //Vlink play (both interactive and non)
	void play(const char *moviename, int theX, int theY, int width, int height, Object *whoToCue, int wait, int close, int erase);
	void stop(void);
	void seek(int offset, int MODE);
	void pause(bool, bool);
	void pause(bool);
	void resume(void);
	int getPosn(void);
	bool fromTo(int from, int to);
	void step(int numSteps);
	void open(const char *theMovie);
};

extern Movie *theMovie;

} // namespace Spycraft

#endif
