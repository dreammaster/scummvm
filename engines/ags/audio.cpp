/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/* Based on the Adventure Game Studio source code, copyright 1999-2011 Chris Jones,
 * which is licensed under the Artistic License 2.0.
 * You may also modify/distribute the code in this file under that license.
 */

#include "engines/ags/ags.h"
#include "engines/ags/audio.h"
#include "engines/ags/gamestate.h"
#include "engines/ags/resourceman.h"
#include "engines/ags/scripting/scripting.h"

#include "common/debug.h"

#include "audio/decoders/mp3.h"
#include "audio/decoders/raw.h"
#include "audio/decoders/voc.h"
#include "audio/decoders/vorbis.h"
#include "audio/decoders/wave.h"
#include "audio/audiostream.h"

#define MAX_SOUND_CHANNELS 8
#define SPECIAL_CROSSFADE_CHANNEL 8

#define SCHAN_SPEECH  0
#define SCHAN_AMBIENT 1
#define SCHAN_MUSIC   2
#define SCHAN_NORMAL  3
#define AUDIOTYPE_LEGACY_AMBIENT_SOUND 1
#define AUDIOTYPE_LEGACY_MUSIC 2
#define AUDIOTYPE_LEGACY_SOUND 3

namespace AGS {

AGSAudio::AGSAudio(AGSEngine *vm) : _vm(vm), _musicResources(NULL), _audioResources(NULL), _speechResources(NULL) {
	openResources();

	_channels.resize(MAX_SOUND_CHANNELS + 1);
	for (uint i = 0; i < _channels.size(); ++i)
		_channels[i] = new AudioChannel(_vm, i);
}

AGSAudio::~AGSAudio() {
	delete _musicResources;
	delete _audioResources;
	delete _speechResources;

	for (uint i = 0; i < _channels.size(); ++i)
		delete _channels[i];
}

// older versions don't store any audio information
void AGSAudio::init() {
	// FIXME: init clip types

	if (_audioResources)
		addAudioResourcesFrom(_audioResources, false);
	addAudioResourcesFrom(_vm->getResourceManager(), true);
}

void AGSAudio::addAudioResourcesFrom(ResourceManager *manager, bool isExecutable) {
	Common::Array<Common::String> filenames = manager->getFilenames();

	for (uint i = 0; i < filenames.size(); ++i) {
		Common::String filename = filenames[i];
		filename.toLowercase();

		if (!filename.hasPrefix("music") && !filename.hasPrefix("sound"))
			continue;

		uint id = 0;
		Common::String extension;
		for (uint j = 0; j < filename.size() - 5; ++j) {
			if (filename[j + 5] != '.')
				continue;
			id = atoi(filename.c_str() + 5);
			extension = filename.c_str() + j + 5 + 1;
			break;
		}

		if (extension.empty())
			continue;

		debug(7, "adding audio clip file '%s' (id %d)", filename.c_str(), id);

		AudioClip clip;
		clip._defaultVolume = 100;
		clip._defaultPriority = 50;
		clip._id = _audioClips.size();
		clip._filename = filename;
		clip._bundledInExecutable = isExecutable;
		if (filename.hasPrefix("music")) {
			clip._scriptName = Common::String::format("aMusic%d", id);
			clip._type = 2;
			clip._defaultRepeat = true;
		} else {
			clip._scriptName = Common::String::format("aSound%d", id);
			clip._type = 3;
			clip._defaultRepeat = false;
		}
		if (extension == "mp3") {
			clip._fileType = kAudioFileMP3;
		} else if (extension == "wav") {
			clip._fileType = kAudioFileWAV;
		} else if (extension == "mid") {
			clip._fileType = kAudioFileMIDI;
		} else if (extension == "ogg") {
			clip._fileType = kAudioFileOGG;
		} else if (extension == "voc") {
			clip._fileType = kAudioFileVOC;
		} else if (extension == "mod" || extension == "xm" || extension == "s3m" || extension == "it") {
			clip._fileType = kAudioFileMOD;
		} else
			error("AudioClip file '%s' had unknown extension '%s'", filename.c_str(), extension.c_str());
		_audioClips.push_back(clip);
	}
}

// 3.1+ stores the audio information in the game data file
void AGSAudio::initFrom(Common::SeekableReadStream *stream) {
	uint32 audioClipTypeCount = stream->readUint32LE();
	debug(4, "%d audio clip types", audioClipTypeCount);
	_audioClipTypes.resize(audioClipTypeCount);
	for (uint i = 0; i < audioClipTypeCount; ++i) {
		_audioClipTypes[i]._id = stream->readUint32LE();
		_audioClipTypes[i]._reservedChannels = stream->readUint32LE();
		_audioClipTypes[i]._volumeReductionWhileSpeechPlaying = stream->readUint32LE();
		_audioClipTypes[i]._crossfadeSpeed = stream->readUint32LE();
		stream->skip(4); // reservedForFuture
	}

	uint32 audioClipCount = stream->readUint32LE();
	debug(4, "%d audio clips", audioClipCount);
	_audioClips.resize(audioClipCount);
	for (uint i = 0; i < audioClipCount; ++i) {
		_audioClips[i]._id = stream->readUint32LE();
		char buf[31];
		stream->read(buf, 30);
		buf[30] = '\0';
		_audioClips[i]._scriptName = buf;
		stream->read(buf, 15);
		buf[15] = '\0';
		_audioClips[i]._filename = buf;
		debug(9, "clip '%s' (%s)", _audioClips[i]._scriptName.c_str(), buf);
		_audioClips[i]._bundledInExecutable = (stream->readByte() == 1);
		_audioClips[i]._type = stream->readByte();
		// FIXME: check validity
		_audioClips[i]._fileType = (AudioFileType)stream->readByte();
		_audioClips[i]._defaultRepeat = (bool)stream->readByte();
		stream->skip(1); // padding
		_audioClips[i]._defaultPriority = stream->readUint16LE();
		_audioClips[i]._defaultVolume = stream->readUint16LE();
		stream->skip(6); // padding + reserved
	}
}

AudioClip *AGSAudio::getClipByIndex(bool isMusic, uint index) {
	// TODO: this is from PSP code, checks using script name..

	Common::String scriptName;
	if (isMusic)
		scriptName = Common::String::format("aMusic%d", index);
	else
		scriptName = Common::String::format("aSound%d", index);

	for (uint i = 0; i < _audioClips.size(); ++i)
		if (_audioClips[i]._scriptName == scriptName)
			return &_audioClips[i];

	return NULL;
}

uint AGSAudio::playSound(uint soundId, uint priority) {
	uint lowestPrioritySoFar = 9999, lowestPriorityId = 0;

	for (uint i = SCHAN_NORMAL; i < _channels.size() - 1; ++i) {
		if (soundId == (uint)-1) {
			// playing sound -1 means iterate through and stop all sound
			_channels[i]->stop();
		} else if (!_channels[i]->isPlaying()) {
			// we can use this one
			if (playSoundOnChannel(soundId, i))
				_channels[i]->setPriority(priority);
			// TODO: why return a channel on failure?
			return i;
		} else if (_channels[i]->getPriority() < lowestPrioritySoFar) {
			lowestPrioritySoFar = _channels[i]->getPriority();
			lowestPriorityId = i;
		}
	}

	if (soundId == (uint)-1)
		return (uint)-1;

	// no free channels, but perhaps we can override one?
	if (priority >= lowestPrioritySoFar) {
		if (playSoundOnChannel(soundId, lowestPriorityId)) {
			_channels[lowestPriorityId]->setPriority(priority);
			return lowestPriorityId;
		}
	}

	return (uint)-1;
}

bool AGSAudio::playSoundOnChannel(uint soundId, uint channelId) {
	// must be a normal channel, and not the last reserved channel
	if (channelId >= _channels.size() - 1)
		error("playSoundOnChannel: channel %d is too high (only %d channels)", channelId, _channels.size());
	if (channelId < SCHAN_NORMAL)
		error("playSoundOnChannel: channel %d is a reserved channel", channelId);

	AudioChannel *channel = _channels[channelId];

	// if an ambient sound is playing on this channel, abort it
	channel->stopAmbientSound();

	if (soundId == (uint)-1) {
		channel->stop();
		return false;
	}

	// not music
	AudioClip *clip = getClipByIndex(false, soundId);
	if (!clip) {
		warning("playSoundOnChannel: no such sound %d", soundId);
		return false;
	}

	channel->playSound(clip);
	channel->setPriority(10);
	// FIXME
	// channel->setVolume(_vm->_state->_soundVolume);

	return true;
}

void AGSAudio::updateMusicVolume() {
	// FIXME
}

void AGSAudio::openResources() {
	_musicResources = new ResourceManager();
	if (!_musicResources->init("music.vox")) {
		delete _musicResources;
		_musicResources = NULL;
	}
	_audioResources = new ResourceManager();
	if (!_audioResources->init("audio.vox")) {
		delete _audioResources;
		_audioResources = NULL;
	} else
		_vm->_state->_separateMusicLib = 1;
	_speechResources = new ResourceManager();
	if (!_speechResources->init("speech.vox")) {
		delete _speechResources;
		_speechResources = NULL;
	} else
		_vm->_state->_wantSpeech = 1;
}

void AGSAudio::registerScriptObjects() {
	for (uint i = 0; i < _audioClips.size(); ++i)
		_vm->getScriptState()->addSystemObjectImport(_audioClips[i]._scriptName, &_audioClips[i]);
}

void AGSAudio::deregisterScriptObjects() {
	for (uint i = 0; i < _audioClips.size(); ++i)
		_vm->getScriptState()->removeImport(_audioClips[i]._scriptName);
}

AudioChannel::AudioChannel(AGSEngine *vm, uint id) : _vm(vm), _id(id), _valid(false) {
}

bool AudioChannel::playSound(AudioClip *clip) {
	_clip = clip;

	_stream = NULL;
	Common::SeekableReadStream *stream = _vm->getFile(clip->_filename);

	switch (_clip->_fileType) {
	case kAudioFileVOC:
		_stream = Audio::makeVOCStream(stream, Audio::FLAG_UNSIGNED, DisposeAfterUse::YES);
		break;
	case kAudioFileWAV: {
		int size, rate;
		byte rawFlags;
		if (Audio::loadWAVFromStream(*stream, size, rate, rawFlags))
			_stream = Audio::makeRawStream(stream->readStream(size), rate, rawFlags);
		else
			error("AudioChannel::playSound: Couldn't load WAV from stream");
		}
		break;
#ifdef USE_MAD
	case kAudioFileMP3:
		_stream = Audio::makeMP3Stream(stream, DisposeAfterUse::YES);
		break;
#endif
#ifdef USE_VORBIS
	case kAudioFileOGG:
		_stream = Audio::makeVorbisStream(stream, DisposeAfterUse::YES);
		break;
#endif
	default:
		// FIXME
		error("AudioChannel::playSound: invalid clip file type %d", _clip->_fileType);
	}

	// FIXME
	_vm->_mixer->playStream(Audio::Mixer::kSFXSoundType, &_handle, _stream, -1, Audio::Mixer::kMaxChannelVolume, 0, DisposeAfterUse::NO);

	_valid = true;
	return true;
}

void AudioChannel::stop(bool resetLegacyMusicSettings) {
	if (_valid) {
		// FIXME: actually stop
		_valid = false;
	}

	if (_vm->_state->_crossfadingInChannel == _id)
		_vm->_state->_crossfadingInChannel = 0;
	else if (_vm->_state->_crossfadingOutChannel == _id)
		_vm->_state->_crossfadingOutChannel = 0;

	// FIXME: ambient
	// FIXME: resetLegacyMusicSettings
}

void AudioChannel::stopAmbientSound() {
	// FIXME
}

bool AudioChannel::isPlaying() {
	if (!_valid)
		return false;

	return _vm->_mixer->isSoundHandleActive(_handle);
}

} // End of namespace AGS
