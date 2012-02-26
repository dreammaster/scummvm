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
 */

#include "engines/ags/ags.h"
#include "engines/ags/audio.h"
#include "engines/ags/gamestate.h"
#include "engines/ags/resourceman.h"

#include "common/debug.h"

namespace AGS {

AGSAudio::AGSAudio(AGSEngine *vm) : _vm(vm), _musicResources(NULL), _audioResources(NULL), _speechResources(NULL) {
	openResources();
}

AGSAudio::~AGSAudio() {
	delete _musicResources;
	delete _audioResources;
	delete _speechResources;
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

} // End of namespace AGS
