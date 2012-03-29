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

#include "engines/ags/scripting/scripting.h"
#include "engines/ags/audio.h"
#include "engines/ags/gamestate.h"

namespace AGS {

// import int CDAudio(eCDAudioFunction, int data)
// Performs various commands to start and stop an audio CD.
RuntimeValue Script_CDAudio(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint32 ecdaudiofunction = params[0]._value;
	UNUSED(ecdaudiofunction);
	int data = params[1]._signedValue;
	UNUSED(data);

	// FIXME
	error("CDAudio unimplemented");

	return RuntimeValue();
}

// import void PlayFlic(int flcNumber, int options)
// Plays a FLI/FLC animation.
RuntimeValue Script_PlayFlic(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int flcNumber = params[0]._signedValue;
	UNUSED(flcNumber);
	int options = params[1]._signedValue;
	UNUSED(options);

	// FIXME
	error("PlayFlic unimplemented");

	return RuntimeValue();
}

// import void PlayVideo(const string filename, VideoSkipStyle, int flags)
// Plays an AVI/MPG video.
RuntimeValue Script_PlayVideo(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *filename = (ScriptString *)params[0]._object;
	UNUSED(filename);
	uint32 videoskipstyle = params[1]._value;
	UNUSED(videoskipstyle);
	int flags = params[2]._signedValue;
	UNUSED(flags);

	// FIXME
	error("PlayVideo unimplemented");

	return RuntimeValue();
}

// import void PlayMusic(int musicNumber)
// Starts the specified music playing.
RuntimeValue Script_PlayMusic(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int musicNumber = params[0]._signedValue;
	UNUSED(musicNumber);

	// FIXME
	error("PlayMusic unimplemented");

	return RuntimeValue();
}

// import void PlayMusicQueued(int musicNumber)
// Queues up the specified music to play after the current one finishes.
RuntimeValue Script_PlayMusicQueued(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int musicNumber = params[0]._signedValue;
	UNUSED(musicNumber);

	// FIXME
	error("PlayMusicQueued unimplemented");

	return RuntimeValue();
}

// import void PlaySilentMIDI(int musicNumber)
// Plays a MIDI, but mutes the channel. This allows you to use it to time game events.
RuntimeValue Script_PlaySilentMIDI(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int musicNumber = params[0]._signedValue;
	UNUSED(musicNumber);

	// FIXME
	error("PlaySilentMIDI unimplemented");

	return RuntimeValue();
}

// import void PlayMP3File(const string filename)
// Plays a specified MP3 or OGG file, that is not part of the normal game package.
RuntimeValue Script_PlayMP3File(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	ScriptString *filename = (ScriptString *)params[0]._object;
	UNUSED(filename);

	// FIXME
	error("PlayMP3File unimplemented");

	return RuntimeValue();
}

// import int PlaySound(int soundNumber)
// Starts the specified sound number playing.
RuntimeValue Script_PlaySound(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint soundNumber = params[0]._value;

	return vm->_audio->playSound(soundNumber);
}

// import void PlaySoundEx(int soundNumber, int channel)
// Starts the specified sound number playing on the specified channel.
RuntimeValue Script_PlaySoundEx(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int soundNumber = params[0]._signedValue;
	UNUSED(soundNumber);
	int channel = params[1]._signedValue;
	UNUSED(channel);

	// FIXME
	error("PlaySoundEx unimplemented");

	return RuntimeValue();
}

// import void PlayAmbientSound (int channel, int sound, int volume, int x, int y)
// Starts an ambient looping sound playing.
RuntimeValue Script_PlayAmbientSound(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint channel = params[0]._value;
	uint sound = params[1]._value;
	uint volume = params[2]._value;
	int x = params[3]._signedValue;
	int y = params[4]._signedValue;

	vm->_audio->playAmbientSound(channel, sound, volume, Common::Point(x, y));

	return RuntimeValue();
}

// import void StopAmbientSound (int channel)
// Stops an ambient sound from playing.
RuntimeValue Script_StopAmbientSound(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint channel = params[0]._value;

	vm->_audio->stopAmbientSound(channel);

	return RuntimeValue();
}

// import int GetCurrentMusic()
// Returns the currently playing music number.
RuntimeValue Script_GetCurrentMusic(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GetCurrentMusic unimplemented");

	return RuntimeValue();
}

// import void SetMusicRepeat(int repeat)
// Sets whether music tracks should repeat once they reach the end.
RuntimeValue Script_SetMusicRepeat(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int repeat = params[0]._signedValue;
	UNUSED(repeat);

	// FIXME
	error("SetMusicRepeat unimplemented");

	return RuntimeValue();
}

// import void SetMusicVolume(int volume)
// Changes the current room's music volume modifier.
RuntimeValue Script_SetMusicVolume(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int volume = params[0]._signedValue;
	UNUSED(volume);

	// FIXME
	error("SetMusicVolume unimplemented");

	return RuntimeValue();
}

// import void SetSoundVolume(int volume)
// Changes the sound volume.
RuntimeValue Script_SetSoundVolume(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int volume = params[0]._signedValue;
	UNUSED(volume);

	// FIXME
	error("SetSoundVolume unimplemented");

	return RuntimeValue();
}

// import void SetMusicMasterVolume(int volume)
// Changes the music volume.
RuntimeValue Script_SetMusicMasterVolume(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint volume = params[0]._value;

	if (volume > 100)
		error("SetMusicMasterVolume: volume %d is too high (must be 0-100)", volume);

	vm->_state->_musicMasterVolume = volume + 60;
	vm->_audio->updateMusicVolume();

	return RuntimeValue();
}

// import void SetDigitalMasterVolume(int volume)
// Changes the volume of all digital sound and music.
RuntimeValue Script_SetDigitalMasterVolume(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int volume = params[0]._signedValue;
	UNUSED(volume);

	// FIXME
	error("SetDigitalMasterVolume unimplemented");

	return RuntimeValue();
}

// import void SeekMODPattern(int pattern)
// Seeks to a specified pattern in a MOD/XM file.
RuntimeValue Script_SeekMODPattern(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int pattern = params[0]._signedValue;
	UNUSED(pattern);

	// FIXME
	error("SeekMODPattern unimplemented");

	return RuntimeValue();
}

// import int IsChannelPlaying(int channel)
// Returns whether sound is playing on the specified sound channel.
RuntimeValue Script_IsChannelPlaying(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int channel = params[0]._signedValue;
	UNUSED(channel);

	// FIXME
	error("IsChannelPlaying unimplemented");

	return RuntimeValue();
}

// import int IsSoundPlaying()
// Returns whether a sound effect is currently playing.
RuntimeValue Script_IsSoundPlaying(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("IsSoundPlaying unimplemented");

	return RuntimeValue();
}

// import int IsMusicPlaying()
// Returns whether background music is currently playing.
RuntimeValue Script_IsMusicPlaying(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("IsMusicPlaying unimplemented");

	return RuntimeValue();
}

// import int GetMIDIPosition()
// Returns the current MIDI beat number.
RuntimeValue Script_GetMIDIPosition(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GetMIDIPosition unimplemented");

	return RuntimeValue();
}

// import void SeekMIDIPosition(int position)
// Seeks the MIDI player to the specified beat number.
RuntimeValue Script_SeekMIDIPosition(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int position = params[0]._signedValue;
	UNUSED(position);

	// FIXME
	error("SeekMIDIPosition unimplemented");

	return RuntimeValue();
}

// import int GetMP3PosMillis()
// Gets the offset into the currently playing MP3 or OGG music.
RuntimeValue Script_GetMP3PosMillis(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("GetMP3PosMillis unimplemented");

	return RuntimeValue();
}

// import void SeekMP3PosMillis(int offset)
// Seeks into the currently playing MP3 or OGG music.
RuntimeValue Script_SeekMP3PosMillis(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int offset = params[0]._signedValue;
	UNUSED(offset);

	// FIXME
	error("SeekMP3PosMillis unimplemented");

	return RuntimeValue();
}

// import void SetChannelVolume(int channel, int volume)
// Changes the volume of the specified sound channel.
RuntimeValue Script_SetChannelVolume(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int channel = params[0]._signedValue;
	UNUSED(channel);
	int volume = params[1]._signedValue;
	UNUSED(volume);

	// FIXME
	error("SetChannelVolume unimplemented");

	return RuntimeValue();
}

// import void StopChannel(int channel)
// Stops the sound currently playing on the specified sound channel.
RuntimeValue Script_StopChannel(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int channel = params[0]._signedValue;
	UNUSED(channel);

	// FIXME
	error("StopChannel unimplemented");

	return RuntimeValue();
}

// import void StopMusic()
// Stops the currently playing music.
RuntimeValue Script_StopMusic(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("StopMusic unimplemented");

	return RuntimeValue();
}

// import int IsVoxAvailable()
// Checks whether a SPEECH.VOX file was found.
RuntimeValue Script_IsVoxAvailable(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	return (vm->_state->_wantSpeech >= 0);
}

// import void SetSpeechVolume(int volume)
// Changes the voice speech volume.
RuntimeValue Script_SetSpeechVolume(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	int volume = params[0]._signedValue;
	UNUSED(volume);

	// FIXME
	error("SetSpeechVolume unimplemented");

	return RuntimeValue();
}

// import int IsMusicVoxAvailable()
// Checks whether a MUSIC.VOX file was found.
RuntimeValue Script_IsMusicVoxAvailable(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	return (vm->_state->_separateMusicLib != 0);
}

// AudioChannel: import void Seek(int position)
// Changes playback to continue from the specified position into the sound.
RuntimeValue Script_AudioChannel_Seek(AGSEngine *vm, AudioChannel *self, const Common::Array<RuntimeValue> &params) {
	int position = params[0]._signedValue;
	UNUSED(position);

	// FIXME
	error("AudioChannel::Seek unimplemented");

	return RuntimeValue();
}

// AudioChannel: import void SetRoomLocation(int x, int y)
// Sets the audio to have its location at (x,y); it will get quieter the further away the player is.
RuntimeValue Script_AudioChannel_SetRoomLocation(AGSEngine *vm, AudioChannel *self, const Common::Array<RuntimeValue> &params) {
	int x = params[0]._signedValue;
	UNUSED(x);
	int y = params[1]._signedValue;
	UNUSED(y);

	// FIXME
	error("AudioChannel::SetRoomLocation unimplemented");

	return RuntimeValue();
}

// AudioChannel: import void Stop()
// Stops the sound currently playing on this channel.
RuntimeValue Script_AudioChannel_Stop(AGSEngine *vm, AudioChannel *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("AudioChannel::Stop unimplemented");

	return RuntimeValue();
}

// AudioChannel: readonly import attribute int ID
// The channel ID of this channel (for use with legacy script).
RuntimeValue Script_AudioChannel_get_ID(AGSEngine *vm, AudioChannel *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("AudioChannel::get_ID unimplemented");

	return RuntimeValue();
}

// AudioChannel: readonly import attribute bool IsPlaying
// Whether this channel is currently playing something.
RuntimeValue Script_AudioChannel_get_IsPlaying(AGSEngine *vm, AudioChannel *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("AudioChannel::get_IsPlaying unimplemented");

	return RuntimeValue();
}

// AudioChannel: readonly import attribute int LengthMs
// The length of the currently playing audio clip, in milliseconds.
RuntimeValue Script_AudioChannel_get_LengthMs(AGSEngine *vm, AudioChannel *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("AudioChannel::get_LengthMs unimplemented");

	return RuntimeValue();
}

// AudioChannel: import attribute int Panning
// The stereo panning of the channel, from -100 to 100.
RuntimeValue Script_AudioChannel_get_Panning(AGSEngine *vm, AudioChannel *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("AudioChannel::get_Panning unimplemented");

	return RuntimeValue();
}

// AudioChannel: import attribute int Panning
// The stereo panning of the channel, from -100 to 100.
RuntimeValue Script_AudioChannel_set_Panning(AGSEngine *vm, AudioChannel *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("AudioChannel::set_Panning unimplemented");

	return RuntimeValue();
}

// AudioChannel: readonly import attribute AudioClip* PlayingClip
// The audio clip that is currently being played on this channel, or null if none.
RuntimeValue Script_AudioChannel_get_PlayingClip(AGSEngine *vm, AudioChannel *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("AudioChannel::get_PlayingClip unimplemented");

	return RuntimeValue();
}

// AudioChannel: readonly import attribute int Position
// The current offset into the sound. What this represents depends on the audio type.
RuntimeValue Script_AudioChannel_get_Position(AGSEngine *vm, AudioChannel *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("AudioChannel::get_Position unimplemented");

	return RuntimeValue();
}

// AudioChannel: readonly import attribute int PositionMs
// The current offset into the sound, in milliseconds.
RuntimeValue Script_AudioChannel_get_PositionMs(AGSEngine *vm, AudioChannel *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("AudioChannel::get_PositionMs unimplemented");

	return RuntimeValue();
}

// AudioChannel: import attribute int Volume
// The volume of this sound channel, from 0 to 100.
RuntimeValue Script_AudioChannel_get_Volume(AGSEngine *vm, AudioChannel *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("AudioChannel::get_Volume unimplemented");

	return RuntimeValue();
}

// AudioChannel: import attribute int Volume
// The volume of this sound channel, from 0 to 100.
RuntimeValue Script_AudioChannel_set_Volume(AGSEngine *vm, AudioChannel *self, const Common::Array<RuntimeValue> &params) {
	int value = params[0]._signedValue;
	UNUSED(value);

	// FIXME
	error("AudioChannel::set_Volume unimplemented");

	return RuntimeValue();
}

// AudioClip: import AudioChannel* Play(AudioPriority=SCR_NO_VALUE, RepeatStyle=SCR_NO_VALUE)
// Plays this audio clip.
RuntimeValue Script_AudioClip_Play(AGSEngine *vm, AudioClip *self, const Common::Array<RuntimeValue> &params) {
	uint32 audiopriority = params[0]._value;
	UNUSED(audiopriority);
	uint32 repeatstyle = params[1]._value;
	UNUSED(repeatstyle);

	// FIXME
	error("AudioClip::Play unimplemented");

	return RuntimeValue();
}

// AudioClip: import AudioChannel* PlayFrom(int position, AudioPriority=SCR_NO_VALUE, RepeatStyle=SCR_NO_VALUE)
// Plays this audio clip, starting from the specified offset.
RuntimeValue Script_AudioClip_PlayFrom(AGSEngine *vm, AudioClip *self, const Common::Array<RuntimeValue> &params) {
	int position = params[0]._signedValue;
	UNUSED(position);
	uint32 audiopriority = params[1]._value;
	UNUSED(audiopriority);
	uint32 repeatstyle = params[2]._value;
	UNUSED(repeatstyle);

	// FIXME
	error("AudioClip::PlayFrom unimplemented");

	return RuntimeValue();
}

// AudioClip: import AudioChannel* PlayQueued(AudioPriority=SCR_NO_VALUE, RepeatStyle=SCR_NO_VALUE)
// Plays this audio clip, or queues it if all channels are busy.
RuntimeValue Script_AudioClip_PlayQueued(AGSEngine *vm, AudioClip *self, const Common::Array<RuntimeValue> &params) {
	uint32 audiopriority = params[0]._value;
	UNUSED(audiopriority);
	uint32 repeatstyle = params[1]._value;
	UNUSED(repeatstyle);

	// FIXME
	error("AudioClip::PlayQueued unimplemented");

	return RuntimeValue();
}

// AudioClip: import void Stop()
// Stops all currently playing instances of this audio clip.
RuntimeValue Script_AudioClip_Stop(AGSEngine *vm, AudioClip *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("AudioClip::Stop unimplemented");

	return RuntimeValue();
}

// AudioClip: readonly import attribute AudioFileType FileType
// Gets the file type of the sound.
RuntimeValue Script_AudioClip_get_FileType(AGSEngine *vm, AudioClip *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("AudioClip::get_FileType unimplemented");

	return RuntimeValue();
}

// AudioClip: readonly import attribute bool IsAvailable
// Checks whether this audio file is available on the player's system.
RuntimeValue Script_AudioClip_get_IsAvailable(AGSEngine *vm, AudioClip *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("AudioClip::get_IsAvailable unimplemented");

	return RuntimeValue();
}

// AudioClip: readonly import attribute AudioType Type
// Gets the type of audio that this clip contains.
RuntimeValue Script_AudioClip_get_Type(AGSEngine *vm, AudioClip *self, const Common::Array<RuntimeValue> &params) {
	// FIXME
	error("AudioClip::get_Type unimplemented");

	return RuntimeValue();
}

// System: readonly import static attribute AudioChannel *AudioChannels[]
// Gets a specific audio channel instance.
RuntimeValue Script_System_geti_AudioChannels(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	uint index = params[0]._value;

	if (index >= vm->_audio->_channels.size())
		error("System::geti_AudioChannels: channel %d too high (only %d channels)", index, vm->_audio->_channels.size());

	return vm->_audio->_channels[index];
}

// System: readonly import static attribute int AudioChannelCount
// Gets the number of audio channels supported by AGS.
RuntimeValue Script_System_get_AudioChannelCount(AGSEngine *vm, ScriptObject *, const Common::Array<RuntimeValue> &params) {
	// FIXME
	return 8; // MAX_SOUND_CHANNELS
}

static const ScriptSystemFunctionInfo ourFunctionList[] = {
	{ "CDAudio", (ScriptAPIFunction *)&Script_CDAudio, "ii", sotNone },
	{ "PlayFlic", (ScriptAPIFunction *)&Script_PlayFlic, "ii", sotNone },
	{ "PlayVideo", (ScriptAPIFunction *)&Script_PlayVideo, "sii", sotNone },
	{ "PlayMusic", (ScriptAPIFunction *)&Script_PlayMusic, "i", sotNone },
	{ "PlayMusicQueued", (ScriptAPIFunction *)&Script_PlayMusicQueued, "i", sotNone },
	{ "PlaySilentMIDI", (ScriptAPIFunction *)&Script_PlaySilentMIDI, "i", sotNone },
	{ "PlayMP3File", (ScriptAPIFunction *)&Script_PlayMP3File, "s", sotNone },
	{ "PlaySound", (ScriptAPIFunction *)&Script_PlaySound, "i", sotNone },
	{ "PlaySoundEx", (ScriptAPIFunction *)&Script_PlaySoundEx, "ii", sotNone },
	{ "PlayAmbientSound", (ScriptAPIFunction *)&Script_PlayAmbientSound, "iiiii", sotNone },
	{ "StopAmbientSound", (ScriptAPIFunction *)&Script_StopAmbientSound, "i", sotNone },
	{ "GetCurrentMusic", (ScriptAPIFunction *)&Script_GetCurrentMusic, "", sotNone },
	{ "SetMusicRepeat", (ScriptAPIFunction *)&Script_SetMusicRepeat, "i", sotNone },
	{ "SetMusicVolume", (ScriptAPIFunction *)&Script_SetMusicVolume, "i", sotNone },
	{ "SetSoundVolume", (ScriptAPIFunction *)&Script_SetSoundVolume, "i", sotNone },
	{ "SetMusicMasterVolume", (ScriptAPIFunction *)&Script_SetMusicMasterVolume, "i", sotNone },
	{ "SetDigitalMasterVolume", (ScriptAPIFunction *)&Script_SetDigitalMasterVolume, "i", sotNone },
	{ "SeekMODPattern", (ScriptAPIFunction *)&Script_SeekMODPattern, "i", sotNone },
	{ "IsChannelPlaying", (ScriptAPIFunction *)&Script_IsChannelPlaying, "i", sotNone },
	{ "IsSoundPlaying", (ScriptAPIFunction *)&Script_IsSoundPlaying, "", sotNone },
	{ "IsMusicPlaying", (ScriptAPIFunction *)&Script_IsMusicPlaying, "", sotNone },
	{ "GetMIDIPosition", (ScriptAPIFunction *)&Script_GetMIDIPosition, "", sotNone },
	{ "SeekMIDIPosition", (ScriptAPIFunction *)&Script_SeekMIDIPosition, "i", sotNone },
	{ "GetMP3PosMillis", (ScriptAPIFunction *)&Script_GetMP3PosMillis, "", sotNone },
	{ "SeekMP3PosMillis", (ScriptAPIFunction *)&Script_SeekMP3PosMillis, "i", sotNone },
	{ "SetChannelVolume", (ScriptAPIFunction *)&Script_SetChannelVolume, "ii", sotNone },
	{ "StopChannel", (ScriptAPIFunction *)&Script_StopChannel, "i", sotNone },
	{ "StopMusic", (ScriptAPIFunction *)&Script_StopMusic, "", sotNone },
	{ "IsVoxAvailable", (ScriptAPIFunction *)&Script_IsVoxAvailable, "", sotNone },
	{ "SetSpeechVolume", (ScriptAPIFunction *)&Script_SetSpeechVolume, "i", sotNone },
	{ "IsMusicVoxAvailable", (ScriptAPIFunction *)&Script_IsMusicVoxAvailable, "", sotNone },
	{ "AudioChannel::Seek^1", (ScriptAPIFunction *)&Script_AudioChannel_Seek, "i", sotAudioChannel },
	{ "AudioChannel::SetRoomLocation^2", (ScriptAPIFunction *)&Script_AudioChannel_SetRoomLocation, "ii", sotAudioChannel },
	{ "AudioChannel::Stop^0", (ScriptAPIFunction *)&Script_AudioChannel_Stop, "", sotAudioChannel },
	{ "AudioChannel::get_ID", (ScriptAPIFunction *)&Script_AudioChannel_get_ID, "", sotAudioChannel },
	{ "AudioChannel::get_IsPlaying", (ScriptAPIFunction *)&Script_AudioChannel_get_IsPlaying, "", sotAudioChannel },
	{ "AudioChannel::get_LengthMs", (ScriptAPIFunction *)&Script_AudioChannel_get_LengthMs, "", sotAudioChannel },
	{ "AudioChannel::get_Panning", (ScriptAPIFunction *)&Script_AudioChannel_get_Panning, "", sotAudioChannel },
	{ "AudioChannel::set_Panning", (ScriptAPIFunction *)&Script_AudioChannel_set_Panning, "i", sotAudioChannel },
	{ "AudioChannel::get_PlayingClip", (ScriptAPIFunction *)&Script_AudioChannel_get_PlayingClip, "", sotAudioChannel },
	{ "AudioChannel::get_Position", (ScriptAPIFunction *)&Script_AudioChannel_get_Position, "", sotAudioChannel },
	{ "AudioChannel::get_PositionMs", (ScriptAPIFunction *)&Script_AudioChannel_get_PositionMs, "", sotAudioChannel },
	{ "AudioChannel::get_Volume", (ScriptAPIFunction *)&Script_AudioChannel_get_Volume, "", sotAudioChannel },
	{ "AudioChannel::set_Volume", (ScriptAPIFunction *)&Script_AudioChannel_set_Volume, "i", sotAudioChannel },
	{ "AudioClip::Play^2", (ScriptAPIFunction *)&Script_AudioClip_Play, "ii", sotAudioClip },
	{ "AudioClip::PlayFrom^3", (ScriptAPIFunction *)&Script_AudioClip_PlayFrom, "iii", sotAudioClip },
	{ "AudioClip::PlayQueued^2", (ScriptAPIFunction *)&Script_AudioClip_PlayQueued, "ii", sotAudioClip },
	{ "AudioClip::Stop^0", (ScriptAPIFunction *)&Script_AudioClip_Stop, "", sotAudioClip },
	{ "AudioClip::get_FileType", (ScriptAPIFunction *)&Script_AudioClip_get_FileType, "", sotAudioClip },
	{ "AudioClip::get_IsAvailable", (ScriptAPIFunction *)&Script_AudioClip_get_IsAvailable, "", sotAudioClip },
	{ "AudioClip::get_Type", (ScriptAPIFunction *)&Script_AudioClip_get_Type, "", sotAudioClip },
	{ "System::geti_AudioChannels", (ScriptAPIFunction *)&Script_System_geti_AudioChannels, "i", sotNone },
	{ "System::get_AudioChannelCount", (ScriptAPIFunction *)&Script_System_get_AudioChannelCount, "", sotNone },
};

void addAudioSystemScripting(AGSEngine *vm) {
	GlobalScriptState *state = vm->getScriptState();

	state->addSystemFunctionImportList(ourFunctionList, ARRAYSIZE(ourFunctionList));
}

} // End of namespace AGS
