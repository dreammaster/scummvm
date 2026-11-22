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

#include "ultima/ultima5/audio/music.h"
#include "audio/midiparser.h"
#include "common/file.h"
#include "common/textconsole.h"
#include "ultima/shared/engine/graphics_options.h"

namespace Ultima {
namespace Ultima5 {

constexpr int SHIP_TILES = 0x20;

// The files of the tunes, in the order of the tracks
static const char *const TRACK_FILES[Music::TRACK_COUNT] = {
	"u5theme.xmi", "britland.xmi", "hornpipe.xmi", "enggmnt.xmi", "stones.xmi", "greyson.xmi", "fanfare.xmi",
	"monarch.xmi", "trntlla.xmi", "halls.xmi", "wrldblw.xmi", "blckthrn.xmi", "ladynan.xmi", "reunion.xmi",
	"rulebrit.xmi", "amiga.xmi"
};

bool Music::enabled() {
	return Shared::configuredOption("upgrade_music", true);
}

int Music::trackForGameState(const Data::Savegame &sg) {
	const int map = sg._mapId;

	if ((sg._partyTile & 0xF8) == SHIP_TILES)
		return TRACK_HORNPIPE;

	if (map == 0)
		return sg._mapLevel == 0 ? TRACK_BRITLAND : TRACK_WRLDBLW;
	if (map <= 0x08)
		return TRACK_TRNTLLA;
	if (map <= 0x0C)
		return TRACK_LADYNAN;
	if (map <= 0x10)
		return TRACK_GREYSON;
	if (map <= 0x11)
		return TRACK_MONARCH;
	if (map <= 0x12)
		return TRACK_BLCKTHRN;
	if (map <= 0x18)
		return TRACK_GREYSON;
	if (map <= 0x1D)
		return TRACK_LADYNAN;
	if (map <= 0x20)
		return TRACK_FANFARE;
	if (map <= 0x28)
		return TRACK_HALLS;

	return TRACK_NONE;
}

int Music::trackForIntroScene(int scene) {
	if (scene <= 0x07)
		return TRACK_STONES;
	if (scene <= 0x0E)
		return TRACK_HALLS;
	if (scene <= 0x15)
		return TRACK_GREYSON;

	return TRACK_NONE;
}

int Music::trackForEndingScene(int scene) {
	if (scene <= 0x03)
		return TRACK_STONES;
	if (scene <= 0x07)
		return TRACK_LADYNAN;

	return TRACK_NONE;
}

Music::Music() {
	createDriver();

	if (_driver && _driver->open() == 0) {
		if (_nativeMT32)
			_driver->sendMT32Reset();
		else
			_driver->sendGMReset();

		_driver->setTimerCallback(this, &timerCallback);
		_ready = true;
	}
}

bool Music::begin(int track) {
	Common::File f;

	if (!_ready || track < 0 || track >= TRACK_COUNT || !f.open(TRACK_FILES[track]) || f.size() == 0)
		return false;

	const uint size = f.size();
	byte *data = (byte *)malloc(size);
	if (f.read(data, size) != size) {
		free(data);
		return false;
	}

	MidiParser *parser = MidiParser::createParser_XMIDI();
	if (!parser->loadMusic(data, size)) {
		delete parser;
		free(data);
		return false;
	}

	Common::StackLock lock(_mutex);
	Audio::MidiPlayer::stop();

	// The tunes go round again, except for Reunion, which Rule Britannia follows
	const bool loop = track != TRACK_REUNION;

	_midiData = data;
	parser->setTrack(0);
	parser->setMidiDriver(this);
	parser->setTimerRate(_driver->getBaseTempo());
	parser->property(MidiParser::mpCenterPitchWheelOnUnload, 1);
	parser->property(MidiParser::mpSendSustainOffOnNotesOff, 1);
	parser->property(MidiParser::mpAutoLoop, loop);
	_parser = parser;
	_isLooping = loop;
	syncVolume();
	_isPlaying = true;
	return true;
}

void Music::cue(int track) {
	Common::StackLock lock(_mutex);
	_queued = TRACK_NONE;

	if (track == _current) {
		if (!_isPlaying && track != TRACK_NONE)
			begin(track);
		return;
	}

	Audio::MidiPlayer::stop();
	_current = track;

	if (track != TRACK_NONE)
		begin(track);
}

void Music::playReunion() {
	cue(TRACK_REUNION);

	// Once it is over, Rule Britannia is the tune that's asked for
	Common::StackLock lock(_mutex);
	_current = TRACK_RULEBRIT;
	_queued = TRACK_RULEBRIT;
}

void Music::stop() {
	Common::StackLock lock(_mutex);

	Audio::MidiPlayer::stop();
	_current = TRACK_NONE;
	_queued = TRACK_NONE;
}

void Music::onTimer() {
	const int queued = _queued;

	Audio::MidiPlayer::onTimer();

	// The tune that was playing has ended, so the one that comes after it is begun
	if (queued != TRACK_NONE && !_isPlaying) {
		Common::StackLock lock(_mutex);

		_current = queued;
		_queued = TRACK_NONE;
		begin(queued);
	}
}

} // namespace Ultima5
} // namespace Ultima
