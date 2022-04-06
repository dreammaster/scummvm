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

#ifndef AGS_MUSIC_H
#define AGS_MUSIC_H

#include "audio/audiostream.h"
#include "audio/midiplayer.h"
#include "audio/mixer.h"
#include "common/array.h"

namespace Dink {

class Music : public Audio::MidiPlayer {
private:
	Audio::SoundHandle _soundHandle;
	Common::Array<byte> _midiData;
protected:
	// Overload Audio::MidiPlayer method
	void sendToChannel(byte channel, uint32 b) override;
public:
	Music();
	~Music() override;

	/**
	 * Play music
	 */
	void playMusic(Common::SeekableReadStream *midi, bool repeat = false);
	bool playMusic(const Common::String &filename, bool repeat = false);
};

extern Music *g_music;

extern bool StopMidi();
extern bool PlayMidi(const char *sFileName);

extern void openCD();
extern void closeCD();
extern uint32 PlayCD(byte bTrack);
extern uint32 killCD(byte bTrack);
extern bool cdplaying();

} // End of namespace Dink

#endif
