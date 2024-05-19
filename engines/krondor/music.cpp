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

#include "audio/midiparser_smf.h"
#include "krondor/music.h"

namespace Krondor {

MusicPlayer::MusicPlayer() {
	MidiPlayer::createDriver();

	int ret = _driver->open();
	if (ret == 0) {
		if (_nativeMT32)
			_driver->sendMT32Reset();
		else
			_driver->sendGMReset();

		_driver->setTimerCallback(this, &timerCallback);
	}
}

void MusicPlayer::play(Sound *sound) {
	Common::StackLock lock(_mutex);

	if (_isPlaying && sound == _sound) {
		// Already playing
		return;
	}

	MidiParser *parser = nullptr;
	bool loaded = false;
	parser = MidiParser::createParser_XMIDI();
	loaded = parser->loadMusic(sound->getData(), sound->getSize());

	if (loaded) {
		stop();
		parser->setTrack(0);
		parser->setMidiDriver(this);
		parser->setTimerRate(_driver->getBaseTempo());
		parser->property(MidiParser::mpCenterPitchWheelOnUnload, 1);

		_parser = parser;

		// TODO: Set channel volume
		syncVolume();

		_isLooping = false;
		_isPlaying = true;
		_sound = sound;

	} else {
		warning("Failed to play music");
		delete parser;
	}
}

void MusicPlayer::stop() {
	Audio::MidiPlayer::stop();
	_sound = nullptr;
}

} // namespace Krondor
