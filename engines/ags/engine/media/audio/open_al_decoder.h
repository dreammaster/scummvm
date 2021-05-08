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
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

//=============================================================================
//
// OpenAL Decoder.
//
//=============================================================================

#ifndef AGS_ENGINE_MEDIA_AUDIO_OPEN_AL_DECODER_H
#define AGS_ENGINE_MEDIA_AUDIO_OPEN_AL_DECODER_H

//include <future>
#include "ags/engine/media/audio/openal.h"
//include <SDL_sound.h>
#include "ags/engine/media/audio/audio_core_defs.h"
#include "ags/shared/util/string.h"

namespace AGS3 {

struct SoundSampleDeleterFunctor {
	void operator()(Sound_Sample *p) {
		Sound_FreeSample(p);
#ifdef AUDIO_CORE_DEBUG
		agsdbg::Printf(ags::kDbgMsg_Init, "SoundSampleDeleterFunctor");
#endif
	}
};

using SoundSampleUniquePtr = std::unique_ptr<Sound_Sample, SoundSampleDeleterFunctor>;

class OpenALDecoder {
public:
	OpenALDecoder(ALuint source, std::future<std::vector<char>> sampleBufFuture, AGS::Shared::String sampleExt, bool repeat);
	void Poll();
	void Play();
	void Pause();
	void Stop();
	void Seek(float pos_ms);
	AudioCorePlayState GetPlayState();
	float GetPositionMs();

private:
	ALuint source_;

	bool repeat_;

	AudioCorePlayState playState_ = PlayStateInitial;

	std::future<std::vector<char>> sampleBufFuture_{};
	std::vector<char> sampleData_{};
	AGS::Shared::String sampleExt_ = "";
	ALenum sampleOpenAlFormat_ = 0;
	SoundSampleUniquePtr sample_ = nullptr;

	AudioCorePlayState onLoadPlayState_ = PlayStatePaused;
	float onLoadPositionMs = 0.0f;

	float processedBuffersDurationMs_ = 0.0f;

	bool EOS_ = false;

	static float buffer_duration_ms(ALuint bufferID);
	static ALenum openalFormatFromSample(const SoundSampleUniquePtr &sample);
	void DecoderUnqueueProcessedBuffers();
	void PollBuffers();
};


void dump_al_errors();

} // namespace AGS3

#endif
