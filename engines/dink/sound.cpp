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

#include "dink/sound.h"
#include "dink/dink.h"
#include "dink/globals.h"
#include "dink/text.h"
#include "dink/var.h"
#include "audio/decoders/wave.h"
#include "common/file.h"

namespace Dink {

static int get_pan(int h) {
	int pan = 0;
	int x1 = 320;

	//uncomment to allow math to be done from Dink's current location
	//x1 = spr[1].x;

	if (spr[h].active) {
		if (spr[h].x > x1) pan += (spr[h].x - x1) * 6;
		if (x1 > spr[h].x) pan -= (x1 - spr[h].x) * 6;
	}

	if (pan > 10000)
		pan = 10000;
	if (pan < -10000)
		pan = -10000;

	return pan;
}

static int get_vol(int h) {
	int pan = 0;
	int pan2 = 0;

	if (spr[h].active) {
		if (spr[h].x > spr[1].x)
			pan -= (spr[h].x - spr[1].x) * 4;

		if (spr[1].x > spr[h].x)
			pan -= (spr[1].x - spr[h].x) * 4;

		if (spr[h].y > spr[1].y)
			pan2 -= (spr[h].y - spr[1].y) * 4;

		if (spr[1].y > spr[h].y)
			pan2 -= (spr[1].y - spr[h].y) * 4;

		//Msg("pan %d, pan2 %d", pan, pan2);

		if (pan2 < pan)
			pan = pan2;
	}

	if (pan > -100)
		pan = 0;

	if (pan < -10000)
		pan = -10000;

	return pan;
}

bool DSEnable(HWND hWnd) {
	HRESULT             dsrval;
	bool                bUseDSound;

	bUseDSound = true;
	//GetProfileInt("FoxBear", "use_dsound", bWantSound);

	if (!bUseDSound) {
		lpDS = NULL;
		Msg("Aborting sound init");
		return true;
	}

	if (lpDS != NULL) {
		Msg("DSEnable, already enabled");
		return true;
	}

	dsrval = DirectSoundCreate(NULL, &lpDS, NULL);

	if (dsrval != DS_OK) {
		if (dsrval == DSERR_INVALIDPARAM) TRACE("DSERR_INVALIDPARAM");
		if (dsrval == DSERR_ALLOCATED) TRACE("DSERR_ALLOCATED");
		if (dsrval == DSERR_OUTOFMEMORY) TRACE("DSERR_OUTOFMEMORY");
		if (dsrval == DSERR_NODRIVER) TRACE("DSERR_NODRIVER");

		TRACE("DirectSoundCreate FAILED");
		return false;
	}

	dsrval = lpDS->SetCooperativeLevel(hWnd, DSSCL_NORMAL);

	if (dsrval != DS_OK) {
		DSDisable();
		Msg("SetCooperativeLevel FAILED");
		return false;
	}

	return true;
}

bool DSDisable() {
	if (lpDS == NULL) {
		return true;
	}

	lpDS->Release();
	lpDS = NULL;

	return true;
}

bool InitSound(HWND hwndOwner) {
	Msg("initting sound");
	DSBUFFERDESC dsBD;
	IDirectSoundBuffer *lpPrimary;

	DSEnable(hwndOwner);

	if (lpDS == NULL) {
		Msg("lpds wasn't initted in dsenable!");
		return false;
	}
	/*
	* Load all sounds -- any that can't load for some reason will have NULL
	* pointers instead of valid SOUNDEFFECT data, and we will know not to
	* play them later on.
	*/


	/*  for( idx = 0; idx < NUM_SOUND_EFFECTS; idx++ )
	{
	if (SoundLoadEffect(idx))
	{
	DSBCAPS  caps;

	caps.dwSize = sizeof(caps);
	lpSoundEffects[idx]->GetCaps(&caps);

	if (caps.dwFlags & DSBCAPS_LOCHARDWARE)
	Msg( "Sound effect %s in hardware", szSoundEffects[idx]);
	else
	Msg( "Sound effect %s in software", szSoundEffects[idx]);
	}
	else
	{
	Msg( "cant load sound effect %s", szSoundEffects[idx]);
	}
	}
	*/

	CreateBufferFromWaveFile("gold.wav", 1);

	SoundLoadBanks();

	/*
	* get the primary buffer and start it playing
	*
	* by playing the primary buffer, DirectSound knows to keep the
	* mixer active, even though we are not making any noise.
	*/

	ZeroMemory(&dsBD, sizeof(DSBUFFERDESC));
	dsBD.dwSize = sizeof(dsBD);
	dsBD.dwFlags = DSBCAPS_PRIMARYBUFFER;

	if (SUCCEEDED(lpDS->CreateSoundBuffer(&dsBD, &lpPrimary, NULL))) {
		if (!SUCCEEDED(lpPrimary->Play(0, 0, DSBPLAY_LOOPING))) {
			Msg("Unable to play Primary sound buffer");
			return false;
		}
		return true;
		//  IDirectSoundBuffer_Release(lpPrimary);
	} else {
		return false;
		// TRACE("Unable to create Primary sound buffer");
	}

	return true;
}

bool DestroySound() {
	DWORD       idxKill;

	for (idxKill = 0; idxKill < max_sounds; idxKill++) {
		SoundDestroyEffect(idxKill);
	}

	DSDisable();
	return true;
}

void SoundLoadBanks() {
#ifdef TODO
	HRESULT ddsound;

	lpDS->DuplicateSoundBuffer(ssound[1].sound, &bowsound);


	for (int i = 1; i <= num_soundbanks; i++) {
		if (lpDS && soundbank[i] == nullptr) {
			//
			//  use DSLoadSoundBuffer (in ..\misc\dsutil.c) to load
			//  a sound from a resource.
			//

			ddsound = lpDS->DuplicateSoundBuffer(ssound[1].sound, &soundbank[i]);
			if (ddsound != DS_OK) Msg("Couldn't load soundbank %d.", i);
		}
	}
#endif
}

bool SoundStopEffect(int sound) {
	HRESULT     dsrval;

	if (!lpDS || !ssound[sound].sound) {
		return false;
	}

	dsrval = ssound[sound].sound->Stop();

	return SUCCEEDED(dsrval);
}

bool CreateBufferFromWaveFile(const char *filename, uint32 dwBuf) {
	// Open the wave file
	Common::File f;

	// Open wave file
	if (!f.open(Common::String::format("sound/%s", filename))) {
		warning("Error, cannot load sound file %s", filename);
		return false;
	}

	// Decode it
	Audio::AudioStream *wavStream = Audio::makeWAVStream(
		f.readStream(f.size()), DisposeAfterUse::YES);
	f.close();

	if (!wavStream) {
		warning("Couldn't create sound buffer for sound %s.", filename);
		return false;
	}

	// Create the sound buffer for the wave file
	ssound[dwBuf].sound = new IDirectSoundBuffer(wavStream);
	return true;
}

void update_sound() {
	unsigned long crap;

	if (!sound_on)
		return;

	for (int i = 1; i <= num_soundbanks; i++) {
		if (soundinfo[i].repeat) if (soundinfo[i].owner != 0) {
			if ((spr[soundinfo[i].owner].sound == 0) ||
					(soundinfo[i].owner == 0) ||
					(spr[soundinfo[i].owner].active == false)) {
				soundbank[i]->Stop();
				soundinfo[i].owner = 0;
				soundinfo[i].repeat = 0;
			} else {
				soundbank[i]->SetPan(get_pan(soundinfo[i].owner));
				soundbank[i]->SetVolume(get_vol(soundinfo[i].owner));
			}
		}

		soundbank[i]->GetStatus(&crap);
		if (crap == DSBSTATUS_PLAYING) {
			if (soundinfo[i].owner != 0) {
				if (spr[soundinfo[i].owner].active == false) {
					soundbank[i]->Stop();

				} else {
					soundbank[i]->SetPan(get_pan(soundinfo[i].owner));
					soundbank[i]->SetVolume(get_vol(soundinfo[i].owner));
				}
			}
		}
	}
}

int playbank(int sound, int min, int plus, int sound3d, bool repeat) {
	HRESULT     dsrval;
	unsigned long crap;
	HRESULT ddsound;
	int i;

	for (i = 1; i <= num_soundbanks; i++) {
		if (!lpDS || !soundbank[i]) {
			Msg("soundbank %d not initted!", i);
			return false;
		}

		ddsound = soundbank[i]->GetStatus(&crap);
		if (ddsound != DS_OK)
			Msg("Couldn't get status of soundbank %d..", i);
		if (crap != DSBSTATUS_PLAYING)
			if (!soundinfo[i].repeat)
				goto madeit;
	}

	return false;

madeit:
	soundbank[i]->Release();

	ddsound = lpDS->DuplicateSoundBuffer(ssound[sound].sound, &soundbank[i]);
	if (ddsound != DS_OK) {
		Msg("Problem programming new sound %d in soundbank %d.", sound, i);

		if (ddsound == DSERR_ALLOCATED) Msg("DSOUND: DSERR_ALLOCATED");
		if (ddsound == DSERR_INVALIDCALL) Msg("DSOUND: DSERR_INVALIDCALL");
		if (ddsound == DSERR_INVALIDPARAM) Msg("DSOUND: DSERR_INVALIDPARAM");
		if (ddsound == DSERR_OUTOFMEMORY) Msg("DSOUND: DSERR_OUTOFMEMORY");
		if (ddsound == DSERR_UNINITIALIZED) Msg("DSOUND: DSERR_UNINITIALIZED");

		return false;
	}

	// Rewind the play cursor to the start of the effect, and play
	soundbank[i]->SetCurrentPosition(0);

	if (plus == 0)
		soundbank[i]->SetFrequency(min);
	else
		soundbank[i]->SetFrequency(RANDOM(min, plus));
	if (sound3d > 0) {
		soundbank[i]->SetPan(get_pan(sound3d));
		soundbank[i]->SetVolume(get_vol(sound3d));
	}

	if (repeat) {
		dsrval = soundbank[i]->Play(0, 0, DSBPLAY_LOOPING);
		Msg("Making looping with buffer %d..", i);
	} else {
		dsrval = soundbank[i]->Play(0, 0, 0);
	}
	if (dsrval == DSERR_BUFFERLOST) {
		Msg("** Soundbank Buffer needs restoring");
	}

	soundinfo[i].owner = sound3d;
	soundinfo[i].repeat = repeat;
	soundinfo[i].survive = 0;
	soundinfo[i].vol = 0;

	return i;
}

int SoundPlayEffect(int sound, int min, int plus, int sound3d, bool repeat) {
	int bank = playbank(sound, min, plus, sound3d, repeat);

	if (!bank) {
		Msg("Ran out of banks.");
		return 0;

	}

	return bank;
}

bool SoundDestroyEffect(int sound) {
	if (ssound[sound].sound) {
		ssound[sound].sound->Release();
		ssound[sound].sound = NULL;
	}

	return true;
}

bool playing(int sound) {
	unsigned long crap;
	IDirectSoundBuffer *pdsb = ssound[sound].sound;

	if (!lpDS || !pdsb) {
		return false;
	}

	pdsb->GetStatus(&crap);
	if (crap == DSBSTATUS_PLAYING)
		return true;
	else
		return false;
}

void kill_repeat_sounds() {
	if (!sound_on) return;

	for (int i = 1; i <= num_soundbanks; i++) {
		if (soundinfo[i].repeat) if (soundinfo[i].owner == 0) if (soundinfo[i].survive == 0) {
			soundbank[i]->Stop();
			soundinfo[i].owner = 0;
			soundinfo[i].repeat = 0;
		}
	}
}

void kill_repeat_sounds_all() {
	if (!sound_on)
		return;

	for (int i = 1; i <= num_soundbanks; i++) {
		if (soundinfo[i].repeat) if (soundinfo[i].owner == 0) {
			soundbank[i]->Stop();
			soundinfo[i].owner = 0;
			soundinfo[i].repeat = 0;
		}
	}
}

} // namespace Dink
