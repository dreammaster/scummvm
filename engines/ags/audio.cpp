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

// old-style init: older versions don't store any audio information, so this is used instead of initFrom
void AGSAudio::init() {
	// 4 hardcoded clip types
	_audioClipTypes.resize(4);
	for (uint i = 0; i < 4; ++i) {
		_audioClipTypes[i]._id = i;
		_audioClipTypes[i]._reservedChannels = (i == 3 ? 0 : 1);
		_audioClipTypes[i]._volumeReductionWhileSpeechPlaying = 10;
		_audioClipTypes[i]._crossfadeSpeed = 0;
	}

	if (_audioResources)
		addAudioResourcesFrom(_audioResources, false);
	if (_musicResources)
		addAudioResourcesFrom(_musicResources, false);
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
		Common::String newScriptName;
		if (filename.hasPrefix("music")) {
			newScriptName = Common::String::format("aMusic%d", id);
			clip._type = 2;
			clip._defaultRepeat = true;
		} else {
			newScriptName = Common::String::format("aSound%d", id);
			clip._type = 3;
			clip._defaultRepeat = false;
		}
		// WORKAROUND: Old-style games can have clips with identical IDs. See Apprentice 2 for example.
		// FIXME: Does this break things?
		bool resolved = false;
		while (!resolved) {
			resolved = true;
			for (uint n = 0; n < _audioClips.size(); ++n)
				if (_audioClips[n]._scriptName.equalsIgnoreCase(newScriptName)) {
					newScriptName = newScriptName + "_";
					resolved = false;
					break;
				}
		}
		clip._scriptName = newScriptName;
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

Common::SeekableReadStream *AGSAudio::getAudioResource(const Common::String &filename) {
	if (_vm->getGameFileVersion() < kAGSVer321)
		return _musicResources->getFile(filename);
	else
		return _audioResources->getFile(filename);
}

// new-style init: 3.1+ stores the audio information in the game data file
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

void AGSAudio::update() {
	// FIXME: crossfading

	if (_vm->_state->_fastForward)
		return;

	if (_vm->_state->_curMusicNumber == (uint)-1)
		return;

	if (!isMusicPlaying()) {
		// We were playing music, but it's finished.
		_vm->_state->_curMusicNumber = (uint)-1;
		playNextQueued();
		return;
	}

	// FIXME: crossfading
}

uint AGSAudio::getFirstChannelToUseFor(uint clipType) {
	uint channel = 0;
	for (uint i = 0; i < _audioClipTypes.size(); ++i) {
		if (i == clipType && _audioClipTypes[i]._reservedChannels)
			break;
		channel += _audioClipTypes[i]._reservedChannels;
	}

	return channel;
}

uint AGSAudio::getLastChannelToUseFor(uint clipType) {
	if (clipType == (uint)-1 || _audioClipTypes[clipType]._reservedChannels == 0)
		return _channels.size() - 1;

	uint channel = 0;
	for (uint i = 0; i < _audioClipTypes.size(); ++i) {
		channel += _audioClipTypes[i]._reservedChannels;
		if (i == clipType)
			break;
	}

	return channel;

}

uint AGSAudio::findFreeAudioChannel(AudioClip &clip, int priority, bool interruptEqualPriority) {
	int lowestPrioritySoFar = 9999999;
	uint channelToUse = (uint)-1;

	if (!interruptEqualPriority)
		priority--;

	uint startAtChannel = getFirstChannelToUseFor(clip._type);
	uint endAtChannel = getLastChannelToUseFor(clip._type);

	for (uint i = startAtChannel; i < endAtChannel; ++i) {
		AudioClip *otherClip = _channels[i]->getClip(); // avoid race: getClip() can return NULL if !isPlaying()

		if (!_channels[i]->isPlaying()) {
			// This channel is free.
			if (_vm->_state->_crossfadingInChannel < 1)
				startFadingInNewTrackIfApplicable(i, clip);
			return i;
		}

		// We override the channel which is playing a clip of the same type,
		// but has the lowest priority.
		if (otherClip->_type != clip._type)
			continue;
		if (_channels[i]->getPriority() >= lowestPrioritySoFar)
			continue;
		if (_channels[i]->getPriority() > priority)
			continue;
		lowestPrioritySoFar = _channels[i]->getPriority();
		channelToUse = i;
	}

	if (channelToUse != (uint)-1)
		stopOrFadeOutChannel(channelToUse, channelToUse, &clip);

	return channelToUse;
}

void AGSAudio::queueAudioClipToPlay(AudioClip &clip, int priority, bool repeat) {
	QueuedClip queueEntry;
	queueEntry._clipId = clip._id;
	queueEntry._priority = priority;
	queueEntry._repeat = repeat;
	_newMusicQueue.push_back(queueEntry);
}

void AGSAudio::playNextQueued() {
	if (_vm->_state->_musicQueue.empty())
		return;

	uint16 musicId = _vm->_state->_musicQueue.front();

	if (musicId >= QUEUED_MUSIC_REPEAT)
		playNewMusic(musicId - QUEUED_MUSIC_REPEAT, true);
	else
		playNewMusic(musicId, false);

	_vm->_state->_musicQueue.remove_at(0);
}

void AGSAudio::removeClipsOfTypeFromQueue(uint clipType) {
	for (int i = 0; (uint)i < _newMusicQueue.size(); ++i) {
		AudioClip &clip = _audioClips[_newMusicQueue[i]._clipId];
		if (clip._type != clipType)
			continue;
		_newMusicQueue.remove_at(i);
		--i;
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
		if (_audioClips[i]._scriptName.equalsIgnoreCase(scriptName))
			return &_audioClips[i];

	return NULL;
}

void AGSAudio::playAudioClipByIndex(uint index) {
	if (index < _audioClips.size())
		playAudioClip(_audioClips[index], SCR_NO_VALUE, SCR_NO_VALUE);
}

uint AGSAudio::playAudioClip(AudioClip &clip, int priority, uint repeat, uint fromOffset, bool queueIfNoChannel) {
	if (!queueIfNoChannel)
		removeClipsOfTypeFromQueue(clip._type);

	bool doRepeat = (bool)repeat;
	if (repeat == SCR_NO_VALUE)
		doRepeat = clip._defaultRepeat;

	if (priority == SCR_NO_VALUE)
		priority = clip._defaultPriority;

	uint channel = findFreeAudioChannel(clip, priority, !queueIfNoChannel);
	if (channel != (uint)-1)
		return playAudioClipOnChannel(channel, clip, priority, doRepeat, fromOffset);

	if (queueIfNoChannel)
		queueAudioClipToPlay(clip, priority, repeat);
	else
		debug(2, "playAudioClip: no channels available for clip of priority %d", priority);

	return channel;
}

uint AGSAudio::playAudioClipOnChannel(uint channelId, AudioClip &clip, int priority, bool repeat, uint fromOffset) {
	AudioChannel *channel = _channels[channelId];

	channel->playSound(&clip, repeat);
	channel->setPriority(priority);
	// FIXME
	channel->setVolume(_vm->_state->_soundVolume);

	// FIXME: everything else
	return channelId;
}

void AGSAudio::stopClip(AudioClip &clip) {
	for (uint i = 0; i < _channels.size() - 1; ++i) {
		if (_channels[i]->getClip() == &clip)
			_channels[i]->stop();
	}
}

uint AGSAudio::playSound(uint soundId, int priority) {
	int lowestPrioritySoFar = 9999;
	uint lowestPriorityId = 0;

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

	// if skipping a cutscene, don't try and play the sound
	if (_vm->_state->_fastForward)
		return false;

	// not music
	AudioClip *clip = getClipByIndex(false, soundId);
	if (!clip) {
		warning("playSoundOnChannel: no such sound %d", soundId);
		return false;
	}

	channel->playSound(clip);
	channel->setPriority(10);
	channel->setVolume(_vm->_state->_soundVolume);

	return true;
}

void AGSAudio::playNewMusic(uint musicId) {
	// TODO: is -1 also 'not repeat'?
	playNewMusic(musicId, (bool)_vm->_state->_musicRepeat);
}

void AGSAudio::playNewMusic(uint musicId, bool repeat) {
	// don't play the music if it's already playing
	if (_vm->_state->_curMusicNumber == musicId)
		return;

	debug(2, "playNewMusic: playing music %d", musicId);

	if (musicId == (uint)-1) {
		stopMusic();
		return;
	}

	if (_vm->_state->_fastForward) {
		// while skipping cutscene, don't change the music
		_vm->_state->_endCutsceneMusic = musicId;
		return;
	}

	uint useChannel = SCHAN_MUSIC;
	// FIXME: useChannel = prepareForNewMusic();
	stopMusic();

	_vm->_state->_curMusicNumber = musicId;
	// FIXME: current_music_type = 0;

	// FIXME: kill channel contents

	_vm->_state->_currentMusicRepeating = repeat;

	if (musicId >= QUEUED_MUSIC_REPEAT) {
		musicId -= QUEUED_MUSIC_REPEAT;
		repeat = true;
	}
	AudioClip *clip = getClipByIndex(true, musicId);

	if (clip) {
		_channels[useChannel]->playSound(clip);

		// FIXME: set current_music_type
	} else if (musicId != 0)
		warning("failed to load music #%d", musicId);

	// FIXME: post_new_music_check(useChannel);
	updateMusicVolume();
}

void AGSAudio::stopMusic() {
	// FIXME: crossfading
	_channels[SCHAN_MUSIC]->stop();

	_vm->_state->_curMusicNumber = (uint)-1;
	// FIXME: current_music_type = 0;
}

bool AGSAudio::isMusicPlaying() {
	if (_vm->_state->_fastForward && _vm->_state->_skipUntilCharStops == (uint)-1)
		return false;

	// FIXME: crossfading
	return _channels[SCHAN_MUSIC]->isPlaying();
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

void AGSAudio::setVolume(uint volume) {
	// FIXME: set global volume
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

void AGSAudio::updateClipDefaultVolume(AudioClip &clip) {
	if (clip._type >= _vm->_state->_defaultAudioTypeVolumes.size())
		return;
	uint32 volume = _vm->_state->_defaultAudioTypeVolumes[clip._type];
	if (volume == (uint)-1)
		return;
	clip._defaultVolume = volume;
}

void AGSAudio::startFadingInNewTrackIfApplicable(uint channelId, AudioClip &clip) {
	AudioClipType &type = _audioClipTypes[clip._type];
	if (type._crossfadeSpeed == (uint)-1)
		return;

	updateClipDefaultVolume(clip);

	_vm->_state->_crossfadeInVolumePerStep = type._crossfadeSpeed;
	_vm->_state->_crossfadeFinalVolumeIn = clip._defaultVolume;
	_vm->_state->_crossfadingInChannel = channelId;
}

void AGSAudio::moveTrackToCrossfadeChannel(uint channelId, uint speed, uint fadeInChannel, AudioClip *clip) {
}

void AGSAudio::stopOrFadeOutChannel(uint channelId, uint newChannelId, AudioClip *clip) {
	AudioClip *sourceClip = _channels[channelId]->getClip();
	if (!sourceClip) {
		_channels[channelId]->stop();
		return;
	}
	AudioClipType &clipType = _audioClipTypes[sourceClip->_type];
	if (clipType._crossfadeSpeed != (uint)-1 && clipType._crossfadeSpeed != 0) {
		moveTrackToCrossfadeChannel(channelId, clipType._crossfadeSpeed, newChannelId, clip);
	}
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

bool AGSAudio::getAudioClipIsAvailable(AudioClip *clip) {
	if (clip->_bundledInExecutable)
		return true;
	if (_musicResources && _musicResources->hasFile(clip->_filename))
		return true;
	if (_audioResources && _audioResources->hasFile(clip->_filename))
		return true;

	return false;
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
	Common::SeekableReadStream *stream;
	if (clip->_bundledInExecutable)
		stream = _vm->getFile(clip->_filename);
	else
		stream = _vm->_audio->getAudioResource(clip->_filename);
	if (!stream) {
		warning("AudioChannel::playSound: failed to open file '%s'", clip->_filename.c_str());
		return false;
	}

	bool ret = playSound(stream, clip->_fileType, repeat);
	_clip = clip;
	return ret;
}

bool AudioChannel::playSound(Common::SeekableReadStream *stream, AudioFileType fileType, bool repeat) {
	_clip = NULL;
	_stream = NULL;

	// FIXME: muh
	if (isPlaying())
		stop(false);

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

// stop_and_destroy_channel_ex in original
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

	if (resetLegacyMusicSettings && (_id == SCHAN_MUSIC)) {
		_vm->_state->_curMusicNumber = (uint)-1;
	}
}

bool AudioChannel::isPlaying() {
	if (!_valid)
		return false;

	return _vm->_mixer->isSoundHandleActive(_handle);
}

void AudioChannel::setVolume(uint volume) {
	// FIXME: set volume

	// TODO: The original engine seems rather inconsistent about setting this, take a look at it.
	_volume = volume;
}

uint32 AudioChannel::getPositionMs() {
	return _vm->_mixer->getSoundElapsedTime(_handle);
}

uint32 AudioChannel::getPosition() {
	AudioFileType fileType = _clip->_fileType;

	switch (fileType) {
		case kAudioFileVOC:
		case kAudioFileWAV:
			// CHECKME
			// - might not be accurate enough (the mixer doesn't expose sample info?)
			// - what happens if audio is disabled or samplerate comes back as 0?
			// samples per millisecond from the mixer * milliseconds played
			return (_vm->_mixer->getOutputRate() / 1000) * _vm->_mixer->getSoundElapsedTime(_handle);
			break;
		case kAudioFileOGG:
		case kAudioFileMP3:
			return _vm->_mixer->getSoundElapsedTime(_handle);
			break;
		case kAudioFileMOD:
		default:
			// FIXME
			warning("AudioChannnel::getPosition not implemented for clip file type %d", fileType);
		case kAudioFileMIDI:
			return 0;
			break;
	}
}

} // End of namespace AGS
