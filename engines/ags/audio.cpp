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
#include "engines/ags/constants.h"
#include "engines/ags/gamestate.h"
#include "engines/ags/resourceman.h"
#include "engines/ags/room.h"
#include "engines/ags/scripting/scripting.h"

#include "common/debug.h"

#include "audio/decoders/mp3.h"
#include "audio/decoders/raw.h"
#include "audio/decoders/voc.h"
#include "audio/decoders/vorbis.h"
#include "audio/decoders/wave.h"
#include "audio/audiostream.h"

#define SPECIAL_CROSSFADE_CHANNEL 8

#define AMBIENCE_FULL_DIST 25

#define AUDIOTYPE_LEGACY_AMBIENT_SOUND 1
#define AUDIOTYPE_LEGACY_MUSIC 2
#define AUDIOTYPE_LEGACY_SOUND 3

#define VOL_CHANGEEXISTING   1678
#define VOL_SETFUTUREDEFAULT 1679
#define VOL_BOTH             1680

namespace AGS {

AGSAudio::AGSAudio(AGSEngine *vm) : _vm(vm), _musicResources(NULL), _audioResources(NULL), _speechResources(NULL) {
	openResources();

	_channels.resize(MAX_SOUND_CHANNELS + 1);
	for (uint i = 0; i < _channels.size(); ++i)
		_channels[i] = new AudioChannel(_vm, i);
	_ambients.resize(MAX_SOUND_CHANNELS + 1);
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
	// 'get_audio_clip_for_old_style_number'

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
	stopAmbientSound(channelId);

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

bool AGSAudio::playSpeech(const Common::String &filename) {
	assert(_speechResources);

	Common::SeekableReadStream *stream = NULL;

	AudioFileType myType = kAudioFileWAV;
	stream = _speechResources->getFile(filename + ".wav");
	if (!stream) {
		myType = kAudioFileOGG;
		stream = _speechResources->getFile(filename + ".ogg");
		if (!stream) {
			myType = kAudioFileMP3;
			stream = _speechResources->getFile(filename + ".mp3");
			if (!stream)
				return false;
		}
	}

	_channels[SCHAN_SPEECH]->playSound(stream, myType);

	// FIXME: adjust volumes
	return true;
}

void AGSAudio::playAmbientSound(uint channelId, uint soundId, uint volume, const Common::Point &pos) {
	// The use of ambient channels is a bit inconsistent in the original code:
	// "the channel parameter is to allow multiple ambient sounds in future"
	// I've tried to make this identical(-ish) for now.

	if (channelId >= _channels.size() - 1)
		error("playAmbientSound: channel %d is too high (only %d channels)", channelId, _channels.size());
	if (channelId == SCHAN_SPEECH)
		error("playAmbientSound: attempt to play ambient sound on speech channel");
	if (volume < 1 || volume > 255)
		error("playAmbientSound: volume %d is invalid (must be 1-255)", volume);

	if (_ambients[channelId]._channel == 0 || !_channels[_ambients[channelId]._channel]->isPlaying() ||
		_ambients[channelId]._soundId != soundId) {
		// The ambient sound isn't already playing on this ambient channel.
		AudioChannel *channel = _channels[channelId];

		stopAmbientSound(channelId);
		channel->stop();

		AudioClip *clip = getClipByIndex(false, soundId);
		if (!clip) {
			warning("playAmbientSound: no such sound %d", soundId);
			return;
		}

		_ambients[channelId]._channel = channelId;
		channel->playSound(clip, true);
		channel->setPriority(15); // ambient sound higher priority than normal sfx
	}

	if (pos.x > _vm->getCurrentRoom()->_width / 2)
		_ambients[channelId]._maxDist = pos.x;
	else
		_ambients[channelId]._maxDist = _vm->getCurrentRoom()->_width - pos.x;
	_ambients[channelId]._maxDist -= AMBIENCE_FULL_DIST;
	_ambients[channelId]._soundId = soundId;
	_ambients[channelId]._pos = pos;
	_ambients[channelId]._volume = volume;

	updateAmbientSoundVolume();
}

void AGSAudio::stopAmbientSound(uint channelId) {
	if (channelId >= _channels.size() - 1)
		error("stopAmbientSound: channel %d is too high (only %d channels)", channelId, _channels.size());

	if (_ambients[channelId]._channel == 0)
		return;

	_channels[channelId]->stop();
	_ambients[channelId]._channel = 0;
}

void AGSAudio::updateAmbientSoundVolume() {
	// FIXME
}

void AGSAudio::updateDirectionalSoundVolume() {
	// FIXME
}

void AGSAudio::updateMusicVolume() {
	// FIXME
}

void AGSAudio::setAudioTypeVolume(uint type, uint volume, uint changeType) {
	// FIXME
}

void AGSAudio::setSoundVolume(uint volume) {
	assert(volume <= 255);

	_vm->_state->_soundVolume = volume;
	setAudioTypeVolume(AUDIOTYPE_LEGACY_AMBIENT_SOUND, (volume * 100) / 255, VOL_BOTH);
	setAudioTypeVolume(AUDIOTYPE_LEGACY_SOUND, (volume * 100) / 255, VOL_BOTH);
	updateAmbientSoundVolume();
}

void AGSAudio::setSpeechVolume(uint volume) {
	assert(volume <= 255);

	if (_channels[SCHAN_SPEECH]->isPlaying())
		_channels[SCHAN_SPEECH]->setVolume(volume);
	_vm->_state->_speechVolume = volume;
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

bool AudioChannel::playSound(AudioClip *clip, bool repeat) {
	bool ret = playSound(_vm->getFile(clip->_filename), clip->_fileType, repeat);
	_clip = clip;
	return ret;
}

bool AudioChannel::playSound(Common::SeekableReadStream *stream, AudioFileType fileType, bool repeat) {
	_clip = NULL;
	_stream = NULL;

	// FIXME: stupid hack due to threading issues
	Common::SeekableReadStream *newStream = stream->readStream(stream->size());
	delete stream;
	stream = newStream;

	switch (fileType) {
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
	case kAudioFileVOC:
		_stream = Audio::makeVOCStream(stream, Audio::FLAG_UNSIGNED, DisposeAfterUse::YES);
		break;
	case kAudioFileMIDI:
	case kAudioFileMOD:
	default:
		// FIXME
		warning("AudioChannel::playSound: invalid clip file type %d", fileType);
		return false;
	}

	// FIXME: argh
	if (repeat) {
		// FIXME: horrible
		Audio::AudioStream *streamToPlay = new Audio::LoopingAudioStream(_stream, 0, DisposeAfterUse::NO);
		_vm->_mixer->playStream(Audio::Mixer::kSFXSoundType, &_handle, streamToPlay, -1, Audio::Mixer::kMaxChannelVolume, 0, DisposeAfterUse::YES);
	} else
		_vm->_mixer->playStream(Audio::Mixer::kSFXSoundType, &_handle, _stream, -1, Audio::Mixer::kMaxChannelVolume, 0, DisposeAfterUse::NO);

	_valid = true;
	return true;
}

void AudioChannel::stop(bool resetLegacyMusicSettings) {
	if (_valid) {
		_vm->_mixer->stopHandle(_handle);
		// FIXME: zap stream?
		_valid = false;
	}

	if (_vm->_state->_crossfadingInChannel == _id)
		_vm->_state->_crossfadingInChannel = 0;
	else if (_vm->_state->_crossfadingOutChannel == _id)
		_vm->_state->_crossfadingOutChannel = 0;

	// FIXME: ambient
	// FIXME: resetLegacyMusicSettings
}

bool AudioChannel::isPlaying() {
	if (!_valid)
		return false;

	return _vm->_mixer->isSoundHandleActive(_handle);
}

void AudioChannel::setVolume(uint volume) {
	// FIXME
}

} // End of namespace AGS
