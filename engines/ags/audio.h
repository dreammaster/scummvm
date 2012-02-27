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

#ifndef AGS_AUDIO_H
#define AGS_AUDIO_H

#include "engines/ags/scriptobj.h"
#include "audio/mixer.h"

namespace Common {
class SeekableReadStream;
}

namespace AGS {

struct AudioChannel : public ScriptObject {
	bool isOfType(ScriptObjectType objectType) { return (objectType == sotAudioChannel); }

	uint _priority;
	Audio::SoundHandle _handle;
};

enum AudioFileType {
	kAudioFileOGG = 1,
	kAudioFileMP3 = 2,
	kAudioFileWAV = 3,
	kAudioFileVOC = 4,
	kAudioFileMIDI = 5,
	kAudioFileMOD = 6
};

struct AudioClip : public ScriptObject {
	bool isOfType(ScriptObjectType objectType) { return (objectType == sotAudioClip); }

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

class ResourceManager;

class AGSAudio {
public:
	AGSAudio(class AGSEngine *vm);
	~AGSAudio();

	void init();
	void initFrom(Common::SeekableReadStream *stream);
	void registerScriptObjects();

	AudioClip *getClipByIndex(bool isMusic, uint index);

	Common::Array<AudioClip> _audioClips;
	Common::Array<AudioClipType> _audioClipTypes;
	Common::Array<AudioChannel> _channels;

protected:
	AGSEngine *_vm;

	ResourceManager *_musicResources, *_audioResources, *_speechResources;

	void addAudioResourcesFrom(ResourceManager *manager, bool isExecutable);
	void openResources();
};

} // End of namespace AGS

#endif // AGS_AUDIO_H
