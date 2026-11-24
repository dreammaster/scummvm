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

#ifndef ULTIMA5_AUDIO_SOUND_H
#define ULTIMA5_AUDIO_SOUND_H

#include "audio/audiostream.h"
#include "audio/mixer.h"
#include "common/array.h"

namespace Ultima {
namespace Ultima5 {

/**
 * Recreates the sounds the game makes on the PC speaker. The original works them out as it goes, by
 * switching the speaker on and off in timing loops, and the game stops while they play. Here each is
 * rendered ahead and queued after whichever is playing, so they come one after the other
 */
class Sound {
private:
	typedef Common::Array<int16> Samples;

	Audio::SoundHandle _handle;
	Audio::QueuingAudioStream *_stream = nullptr;
	bool _on = true;
	uint16 _noiseState = 0;
	double _phase = 0;

	/**
	 * Adds a square wave
	 */
	void addTone(Samples &samples, double freq, double ms);
	void addSilence(Samples &samples, double ms);
	void addNoise(Samples &samples, int rate, int duration, int limit);
	void addSweep(Samples &samples, int from, int to, int step, int duration);
	void addPulse(Samples &samples, int freq, int delay, int duration, int width, int increment);

	/**
	 * Queues what's been rendered
	 */
	void play(const Samples &samples);

public:
	~Sound();

	/**
	 * Tells if the sounds are on, which the player switches
	 */
	bool isOn() const {
		return _on;
	}

	/**
	 * Turns the sounds on if they are off, and off if they are on
	 */
	void toggle();

	/**
	 * Stops what's playing and what is to follow
	 */
	void stop();

	// What the original asks for
	void pulse(int freq, int delay, int duration, int width, int increment);
	void noise(int rate, int duration, int limit);
	void tone(int freq, int ms);
	void sweep(int from, int to, int step, int duration);

	/**
	 * The two clicks of a step
	 */
	void footstep();

	/**
	 * Makes the sound of the party walking, if it does so as itself or on a horse
	 * @param partyTile		What the party is
	 * @param onHorse		Whether just that it's on a horse counts
	 */
	void walking(byte partyTile, bool onHorse = false);

	/**
	 * The sound of a way being blocked
	 */
	void blocked();

	/**
	 * The tune that is played when a fight is won
	 */
	void victory();

	/**
	 * The chirp that says that something can't be done
	 */
	void refuse();

	/**
	 * The rising chirp that says that something failed
	 */
	void alert();

	/**
	 * The sound of a spell of a circle being cast
	 */
	void castEffect(int circle);

	/**
	 * A swell and fade of a tone, as a gem or a shrine makes
	 */
	void flare(int freq, int duration);

	/**
	 * What is heard as the old man comes to the party in its sleep, and as each of them is made well
	 */
	void apparition();
	void apparitionHeals();

	/**
	 * The light of a healer
	 */
	void healerLight();

	/**
	 * A note of the harpsichord, from 0 to 9
	 */
	void harpsichord(int note);

	/**
	 * The sound of Absorbed! when magic is nullified
	 */
	void absorbed();
};

} // namespace Ultima5
} // namespace Ultima

#endif
