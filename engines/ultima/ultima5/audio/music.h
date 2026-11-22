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

#ifndef ULTIMA5_AUDIO_MUSIC_H
#define ULTIMA5_AUDIO_MUSIC_H

#include "audio/midiplayer.h"
#include "ultima/ultima5/data/savegame.h"

namespace Ultima {
namespace Ultima5 {

/**
 * Plays the music of the Ultima V Upgrade, which is a set of XMIDI files that are kept in the game folder
 * by the names of the tunes. The game has none of its own, so if the files aren't there it is silent
 */
class Music : public Audio::MidiPlayer {
public:
	enum Track {
		TRACK_NONE = -1,
		TRACK_THEME,
		TRACK_BRITLAND,
		TRACK_HORNPIPE,
		TRACK_ENGGMNT,
		TRACK_STONES,
		TRACK_GREYSON,
		TRACK_FANFARE,
		TRACK_MONARCH,
		TRACK_TRNTLLA,
		TRACK_HALLS,
		TRACK_WRLDBLW,
		TRACK_BLCKTHRN,
		TRACK_LADYNAN,
		TRACK_REUNION,
		TRACK_RULEBRIT,
		TRACK_AMIGA,
		TRACK_COUNT
	};

private:
	bool _ready = false;
	int _current = TRACK_NONE;
	int _queued = TRACK_NONE;

	/**
	 * Starts a tune, if its file is there
	 */
	bool begin(int track);

protected:
	void onTimer() override;

public:
	Music();

	/**
	 * Tells if the option to play the Upgrade's music is on
	 */
	static bool enabled();

	/**
	 * Gives the tune that suits where the party is
	 */
	static int trackForGameState(const Data::Savegame &savegame);

	/**
	 * Gives the tune of a scene of the introduction
	 */
	static int trackForIntroScene(int scene);

	/**
	 * Gives the tune of a slide of the ending
	 */
	static int trackForEndingScene(int scene);

	/**
	 * Has a tune be the one that is playing. One that is playing already goes on, and one that has
	 * finished is played again. None is silence
	 */
	void cue(int track);

	/**
	 * Plays Reunion, once, and after it Rule Britannia
	 */
	void playReunion();

	void stop() override;
};

} // namespace Ultima5
} // namespace Ultima

#endif
