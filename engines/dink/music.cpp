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

#include "dink/music.h"
#include "dink/dink.h"
#include "dink/file.h"
#include "dink/globals.h"
#include "dink/text.h"
#include "dink/var.h"
#include "audio/midiparser.h"
#include "backends/audiocd/audiocd.h"

namespace Dink {

Music *g_music;

Music::Music() {
	g_music = this;
	Audio::MidiPlayer::createDriver();

	int ret = _driver->open();
	if (ret == 0) {
		if (_nativeMT32)
			_driver->sendMT32Reset();
		else
			_driver->sendGMReset();

		_driver->setTimerCallback(this, &timerCallback);
	}
}

Music::~Music() {
	stop();
	g_music = nullptr;
}

void Music::sendToChannel(byte channel, uint32 b) {
	if (!_channelsTable[channel]) {
		_channelsTable[channel] = (channel == 9) ? _driver->getPercussionChannel() : _driver->allocateChannel();
		// If a new channel is allocated during the playback, make sure
		// its volume is correctly initialized.
		if (_channelsTable[channel])
			_channelsTable[channel]->volume(_channelsVolume[channel] * _masterVolume / 255);
	}

	if (_channelsTable[channel])
		_channelsTable[channel]->send(b);
}

void Music::playMusic(Common::SeekableReadStream *midi, bool repeat) {
	stop();

	// Load MIDI resource data
	int midiMusicSize = midi->size();

	_midiData.resize(midi->size());
	midi->read(&_midiData[0], midi->size());

	MidiParser *parser = MidiParser::createParser_SMF();
	if (parser->loadMusic(&_midiData[0], midiMusicSize)) {
		parser->setTrack(0);
		parser->setMidiDriver(this);
		parser->setTimerRate(_driver->getBaseTempo());
		parser->property(MidiParser::mpCenterPitchWheelOnUnload, 1);
		parser->property(MidiParser::mpSendSustainOffOnNotesOff, true);

		_parser = parser;

		_isLooping = repeat;
		_isPlaying = true;
	} else {
		delete parser;
	}
}

bool Music::playMusic(const Common::String &filename, bool repeat) {
	Common::File f;
	if (!f.open(filename))
		return false;

	playMusic(&f, repeat);
	return true;
}


void init_midi() {
}

uint32 playMIDIFile(HWND hWndNotify, LPCSTR lpszMIDIFileName) {
	g_music->playMusic(lpszMIDIFileName);
	return 0L;
}

bool PlayMidi(const char *sFileName) {
	//no midi stuff right now
	if (sound_on == false)
		return true;

	char buf[256];
	char crap[256];

	if (compare(last_midi, sFileName)) {
		if (nothing_playing()) {
			Msg("I think %s is already playing, I should skip it...", sFileName);
			return false;
		}

	}

	sprintf(crap, "sound/%s", sFileName);
	if (!File::exists(crap)) {
		Msg("Error playing midi %s, doesn't exist in any dir.", sFileName);
		return false;
	}

	strcpy(last_midi, sFileName);
	Msg("Killing cd...");
	last_cd = 0;
	//mciSendCommand(CD_ID, MCI_STOP, 0, NULL);
	//killcd(g_hWnd, 1);
	killCD(1);

	playMIDIFile(g_hWnd, crap);
	return true;

	sprintf(buf, "open %s type sequencer alias MUSIC", crap);

	if (mciSendString("close MUSIC", NULL, 0, NULL) != 0) {
		Msg("Couldn't close current midi..");
		//  return false;
	}

	if (mciSendString(buf, NULL, 0, NULL) != 0) {
		Msg("Couldn't clear midi buffer");
		return false;
	}

	if (mciSendString("play MUSIC from 0", NULL, 0, g_hWnd) != 0) {
		Msg("Playmidi command failed");
		return false;
	}

	return true;
}

void check_midi() {
	char hold[20];

	if (!midi_active) return;
	if (map.music[*pmap] != 0) {
		if (map.music[*pmap] == -1) {
			//kill music
			mciSendCommand(CD_ID, MCI_CLOSE, 0, NULL);
			Msg("Stopped cd");
			killCD(1);
		}

		if (map.music[*pmap] > 1000) {
			if (cd_inserted) {
				if (map.music[*pmap] - 1000 == last_cd)
					if (cdplaying()) return;

				PlayCD(map.music[*pmap] - 1000);
			} else {

				sprintf(hold, "%d.mid", map.music[*pmap] - 1000);
				PlayMidi(hold);
			}
			return;
		}

		//there is music associated with this screen
		sprintf(hold, "%d.mid", map.music[*pmap]);
		PlayMidi(hold);
	}
}


//------------------------------------------------------------------
//
// Function     : PauseMidi()
//
// Purpose      : Pauses midi file
//
//------------------------------------------------------------------

bool PauseMidi() {
	// Pause if we're not already paused...
	if (mciSendString("stop MUSIC", NULL, 0, NULL) != 0) {
		return false;
	}


	// Yahoo!
	return true;
}

//------------------------------------------------------------------
//
// Function     : ResumeMidi()
//
// Purpose      : Resumes playing of a midi file
//
//------------------------------------------------------------------

bool ResumeMidi() {
	// Resume midi
	if (mciSendString("play MUSIC notify", NULL, 0, g_hWnd) != 0) {
		return false;
	}

	// Yahoo!
	return true;
}

//------------------------------------------------------------------
//
// Function     : StopMidi
//
// Purpose      : Stops a midi file playing
//
//------------------------------------------------------------------

bool StopMidi() {

	playMIDIFile(g_hWnd, "");

	// Yahoo!
	return true;
}

//------------------------------------------------------------------
//
// Function     : ReplayMidi()
//
// Purpose      : Replays a midi file
//
//------------------------------------------------------------------

bool ReplayMidi() {
	// Replay midi
	if (mciSendString("play MUSIC from 0 notify", NULL, 0, g_hWnd) != 0) {
		return false;
	}

	// Yahoo!
	return true;
}


bool nothing_playing() {
#ifdef TODO
	//int crap = (mciSendString("status MUSIC offset", NULL, 0, NULL));
	//int crap =  (mciSendString("play MUSIC from 0", NULL, 0, g_hWnd));
	uint32 dwReturn;
	MCI_OPEN_PARMS mciOpenParms;
	MCI_PLAY_PARMS mciPlayParms;
	MCI_STATUS_PARMS mciStatusParms;
	MCI_SEQ_SET_PARMS mciSeqSetParms;
	// Open the device by specifying the device and filename.
	// MCI will attempt to choose the MIDI mapper as the output port.

	//MCI_STATUS_LENGTH
	mciStatusParms.dwItem = MCI_STATUS_MODE;
	if (dwReturn = mciSendCommand(midi_id, MCI_STATUS,
		MCI_STATUS_ITEM, (uint32)(LPVOID)&mciStatusParms)) {

		Msg("Error reading status from midi mapper!");
		return false;
		//mciSendCommand(midi_id, MCI_CLOSE, 0, NULL);
	}
	//  return dwReturn;




	if (mciStatusParms.dwReturn == MCI_MODE_PLAY) return true;
	else
		return false;
#else
	error("TODO");
#endif
}

//thing to play the midi

void openCD() {
	cd_inserted = g_system->getAudioCDManager()->open();
}

void closeCD() {
	g_system->getAudioCDManager()->close();
}

uint32 PlayCD(byte bTrack) {
	g_system->getAudioCDManager()->play(bTrack, 1, 0, 0);
	return 0;
}

uint32 killCD(byte bTrack) {
	g_system->getAudioCDManager()->stop();
	return 0;
}

bool cdplaying() {
	return g_system->getAudioCDManager()->isPlaying();
}

} // End of namespace Dink
