
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

#ifndef AGS2_DATA_AUDIO_CLIP_H
#define AGS2_DATA_AUDIO_CLIP_H

#include "common/serializer.h"
#include "ags2/data/defines.h"

namespace AGS2 {

#define AUCL_BUNDLE_EXE 1
#define AUCL_BUNDLE_VOX 2

enum AudioFileType {
	eAudioFileOGG = 1,
	eAudioFileMP3 = 2,
	eAudioFileWAV = 3,
	eAudioFileVOC = 4,
	eAudioFileMIDI = 5,
	eAudioFileMOD = 6
};

struct ScriptAudioClip {
	int id;  // not used by editor, set in engine only
	char scriptName[30];
	char fileName[15];
	int8 bundlingType;
	int8 type;
	int8 fileType;
	int8 defaultRepeat;
	int16 defaultPriority;
	int16 defaultVolume;
	int  reserved;

	void synchronize(Common::Serializer &s);
	void load(Common::SeekableReadStream *rs) {
		Common::Serializer s(rs, nullptr);
		synchronize(s);
	}
	void save(Common::WriteStream *ws) {
		Common::Serializer s(nullptr, ws);
		synchronize(s);
	}
};

#define AUDIO_CLIP_TYPE_SOUND 1

struct AudioClipType {
	int id;
	int reservedChannels;
	int volume_reduction_while_speech_playing;
	int crossfadeSpeed;
	int reservedForFuture;

	void synchronize(Common::Serializer &s);
	void load(Common::SeekableReadStream *rs) {
		Common::Serializer s(rs, nullptr);
		synchronize(s);
	}
	void save(Common::WriteStream *ws) {
		Common::Serializer s(nullptr, ws);
		synchronize(s);
	}
};

struct AmbientSound {
	int  channel;  // channel number, 1 upwards
	int  x, y;
	int  vol;
	int  num;  // sound number, eg. 3 = sound3.wav
	int  maxdist;

	bool IsPlaying();
	void synchronize(Common::Serializer &s);
	void load(Common::SeekableReadStream *rs) {
		Common::Serializer s(rs, nullptr);
		synchronize(s);
	}
	void save(Common::WriteStream *ws) {
		Common::Serializer s(nullptr, ws);
		synchronize(s);
	}
};

} // namespace AGS2

#endif
