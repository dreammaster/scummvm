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

#ifndef DINK_DIRECTDRAW_DSOUND_H
#define DINK_DIRECTDRAW_DSOUND_H

#include "audio/audiostream.h"
#include "audio/mixer.h"
#include "dink/lib/wintypes.h"

namespace Dink {

// Play enum
#define DSBPLAY_LOOPING             0x00000001
#define DSBPLAY_LOCHARDWARE         0x00000002
#define DSBPLAY_LOCSOFTWARE         0x00000004
#define DSBPLAY_TERMINATEBY_TIME    0x00000008

// GetStatus enum
#define DS_OK                           0
#define DSBSTATUS_PLAYING           0x00000001

#define DSERR_INVALIDPARAM       1
#define DSERR_OUTOFMEMORY        2
#define DSERR_UNINITIALIZED      3
#define DSERR_ALLOCATED         10
#define DSERR_CONTROLUNAVAIL    30
#define DSERR_INVALIDCALL       50
#define DSERR_NODRIVER         120
#define DSERR_BUFFERLOST       150

// SetCooperativeLevel flags
#define DSSCL_NORMAL 1

// DSBUFFERDESC flags
#define DSBCAPS_PRIMARYBUFFER       0x00000001


struct WAVEFORMATEX {
	WORD        wFormatTag;         /* format type */
	WORD        nChannels;          /* number of channels (i.e. mono, stereo...) */
	DWORD       nSamplesPerSec;     /* sample rate */
	DWORD       nAvgBytesPerSec;    /* for buffer estimation */
	WORD        nBlockAlign;        /* block size of data */
	WORD        wBitsPerSample;     /* number of bits per sample of mono data */
	WORD        cbSize;             /* the count in bytes of the size of */
									/* extra information (after cbSize) */
};
typedef WAVEFORMATEX *LPWAVEFORMATEX;

struct DSBUFFERDESC {
	DWORD           dwSize;
	DWORD           dwFlags;
	DWORD           dwBufferBytes;
	DWORD           dwReserved;
	LPWAVEFORMATEX  lpwfxFormat;
};
typedef DSBUFFERDESC *LPDSBUFFERDESC;
typedef const DSBUFFERDESC *LPCDSBUFFERDESC;


struct IDirectSoundBuffer {
private:
	Common::Array<byte> _buffer;
	Audio::AudioStream *_audioStream = nullptr;
	Audio::SoundHandle _soundHandle;

	IDirectSoundBuffer() {}
public:
	static IDirectSoundBuffer *createWav(const Common::String &name);
public:
	IDirectSoundBuffer(const IDirectSoundBuffer &src);
	~IDirectSoundBuffer();

	HRESULT Play(uint32 dwReserved1, uint32 dwPriority, uint32 dwFlags);
	HRESULT SetVolume(int lVolume);
	HRESULT SetPan(int lPan);
	HRESULT SetFrequency(uint32 freq);
	HRESULT SetCurrentPosition(uint32 dwNewPosition);
	HRESULT Stop();
	HRESULT Release();
	HRESULT GetStatus(unsigned long *status);
};

typedef IDirectSoundBuffer *LPDIRECTSOUNDBUFFER;

struct IDirectSound {
	HRESULT CreateSoundBuffer(LPCDSBUFFERDESC pcDSBufferDesc,
		LPDIRECTSOUNDBUFFER *ppDSBuffer, LPUNKNOWN pUnkOuter);
	HRESULT SetCooperativeLevel(HWND hwnd, DWORD dwLevel);
	HRESULT DuplicateSoundBuffer(LPDIRECTSOUNDBUFFER pDSBufferOriginal,
		LPDIRECTSOUNDBUFFER *ppDSBufferDuplicate);
	HRESULT Release();
};

typedef IDirectSound *LPDIRECTSOUND;

extern HRESULT DirectSoundCreate(void *pcGuidDevice,
	LPDIRECTSOUND *ppDS, LPUNKNOWN pUnkOuter);

} // namespace Dink

#endif
