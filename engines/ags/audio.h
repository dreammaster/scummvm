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

#ifndef AGS_AUDIO_H
#define AGS_AUDIO_H

#include "engines/ags/scriptobj.h"
#include "audio/mixer.h"

namespace Common {
class SeekableReadStream;
}

namespace Audio {
class SeekableAudioStream;
}

namespace AGS {

#define MAX_SOUND_CHANNELS 8

#define SCHAN_SPEECH  0
#define SCHAN_AMBIENT 1
#define SCHAN_MUSIC   2
#define SCHAN_NORMAL  3

enum AudioFileType {
	kAudioFileOGG = 1,
	kAudioFileMP3 = 2,
	kAudioFileWAV = 3,
	kAudioFileVOC = 4,
	kAudioFileMIDI = 5,
	kAudioFileMOD = 6
};

class AudioClip : public ScriptObject {
public:
	bool isOfType(ScriptObjectType objectType) { return (objectType == sotAudioClip); }
	const char *getObjectTypeName() { return "AudioClip"; }

	Common::String _scriptName;
	Common::String _filename;
	AudioFileType _fileType;
	bool _bundledInExecutable;

	uint _id;
	byte _type;

	bool _defaultRepeat;
	uint16 _defaultPriority;
	uint16 _defaultVolume;
};

struct AudioClipType {
	uint _id;
	uint _reservedChannels;
	uint _volumeReductionWhileSpeechPlaying;
	uint _crossfadeSpeed;
};

class AudioChannel : public ScriptObject {
public:
	AudioChannel(AGSEngine *vm, uint id);
	bool isOfType(ScriptObjectType objectType) { return (objectType == sotAudioChannel); }
	const char *getObjectTypeName() { return "AudioChannel"; }

	bool isValid() { return _valid; }

	bool playSound(AudioClip *clip, bool repeat = false);
	bool playSound(Common::SeekableReadStream *stream, AudioFileType fileType, bool repeat = false);
	void stop(bool resetLegacyMusicSettings = true);
	bool isPlaying();

	void setVolume(uint volume);
	uint getVolume() { return _volume; }

	void setPanning(int panning);
	int getPanning() { return _panning; }

	int getPriority() { return _priority; }
	void setPriority(int priority) { _priority = priority; }

	uint getId() { return _id; }
	AudioClip *getClip() { return isPlaying() ? _clip : NULL; }

	uint32 getPositionMs();
	uint32 getPosition();

protected:
	// housekeeping
	AGSEngine *_vm;
	uint _id;
	bool _valid;

	// AGS audio
	int _priority;
	AudioClip *_clip;

	uint _volume;
	int _panning;

	// ScummVM audio
	Audio::SoundHandle _handle;
	Audio::SeekableAudioStream *_stream;
};

class ResourceManager;

struct AmbientSound {
	AmbientSound() { _channel = 0; }

	uint _maxDist;
	uint _channel;
	uint _soundId;
	uint _volume;
	Common::Point _pos;
};

struct QueuedClip {
	uint _clipId;
	int _priority;
	bool _repeat;
};

class AGSAudio {
public:
	AGSAudio(class AGSEngine *vm);
	~AGSAudio();

	void init();
	void initFrom(Common::SeekableReadStream *stream);
	void registerScriptObjects();
	void deregisterScriptObjects();

	void update();

	AudioClip *getClipByIndex(bool isMusic, uint index);

	uint findFreeAudioChannel(AudioClip &clip, int priority, bool interruptEqualPriority);

	void queueAudioClipToPlay(AudioClip &clip, int priority, bool repeat);
	void removeClipsOfTypeFromQueue(uint clipType);
	void playAudioClipByIndex(uint index);
	uint playAudioClip(AudioClip &clip, int priority, uint repeat, uint fromOffset = 0, bool queueIfNoChannel = false);
	uint playAudioClipOnChannel(uint channelId, AudioClip &clip, int priority, bool repeat, uint fromOffset = 0);
	void stopClip(AudioClip &clip);

	uint playSound(uint soundId, int priority = 10);
	bool playSoundOnChannel(uint soundId, uint channelId);

	void playNewMusic(uint musicId);
	void playNewMusic(uint musicId, bool repeat);
	void stopMusic();
	bool isMusicPlaying();

	bool playSpeech(const Common::String &filename);

	void playAmbientSound(uint channelId, uint soundId, uint volume, const Common::Point &pos);
	void stopAmbientSound(uint channelId);

	void updateAmbientSoundVolume();
	void updateDirectionalSoundVolume();
	void updateMusicVolume();

	void setAudioTypeVolume(uint type, uint volume, uint changeType);

	void setVolume(uint volume);
	void setSoundVolume(uint volume);
	void setSpeechVolume(uint volume);

	bool hasSpeechResources() { return (bool)_speechResources; }
	bool getAudioClipIsAvailable(AudioClip *clip);

	Common::Array<AudioClip> _audioClips;
	Common::Array<AudioClipType> _audioClipTypes;
	Common::Array<AudioChannel *> _channels;
	Common::Array<AmbientSound> _ambients;

	Common::SeekableReadStream *getAudioResource(const Common::String &filename);

	void stopOrFadeOutChannel(uint channelId, uint newChannelId, AudioClip *clip);

protected:
	AGSEngine *_vm;

	ResourceManager *_musicResources, *_audioResources, *_speechResources;

	Common::Array<QueuedClip> _newMusicQueue;

	void addAudioResourcesFrom(ResourceManager *manager, bool isExecutable);
	void openResources();

	void playNextQueued();

	void updateClipDefaultVolume(AudioClip &clip);

	uint getFirstChannelToUseFor(uint clipType = (uint)-1);
	uint getLastChannelToUseFor(uint clipType);

	void startFadingInNewTrackIfApplicable(uint channelId, AudioClip &clip);
	void moveTrackToCrossfadeChannel(uint channelId, uint speed, uint fadeInChannel, AudioClip *clip);
};

} // End of namespace AGS

#endif // AGS_AUDIO_H
