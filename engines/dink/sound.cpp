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
#include "backends/audiocd/audiocd.h"
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

bool DSEnable() {
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

	dsrval = lpDS->SetCooperativeLevel(0, DSSCL_NORMAL);

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

bool InitSound() {
	Msg("initting sound");
	DSEnable();

	if (lpDS == NULL) {
		Msg("lpds wasn't initted in dsenable!");
		return false;
	}

	/*
	 * Load all sounds -- any that can't load for some reason will have NULL
	 * pointers instead of valid SOUNDEFFECT data, and we will know not to
	 * play them later on.
	 */
	CreateBufferFromWaveFile("gold.wav", 1);

	SoundLoadBanks();

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
	HRESULT ddsound;

	lpDS->DuplicateSoundBuffer(ssound[1].sound, &bowsound);

	for (int i = 1; i <= num_soundbanks; i++) {
		if (lpDS && soundbank[i] == nullptr) {
			// Duplicate the sound
			ddsound = lpDS->DuplicateSoundBuffer(ssound[1].sound, &soundbank[i]);
			if (ddsound != DS_OK)
				Msg("Couldn't load soundbank %d.", i);
		}
	}
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
	Common::String fname = Common::String::format("sound/%s", filename);
	ssound[dwBuf].sound = IDirectSoundBuffer::createWav(fname);
	return ssound[dwBuf].sound != nullptr;
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


bool nothing_playing() {
#ifdef TODO
	//int crap = (mciSendString("status MUSIC offset", NULL, 0, NULL));
	//int crap =  (mciSendString("play MUSIC from 0", NULL, 0, g_hWnd));
	uint32 dwReturn;
	MCI_OPEN_PARMS mciOpenParms;
	MCI_PLAY_PARMS mciPlayParms;
	MCI_STATUS_PARMS mciStatusParms;
	MCI_SEQ_SET_PARMS mciSeqSetParms;
	// Open the device by specifying the device and filename.
	// MCI will attempt to choose the MIDI mapper as the output port.

	//MCI_STATUS_LENGTH
	mciStatusParms.dwItem = MCI_STATUS_MODE;
	if (dwReturn = mciSendCommand(midi_id, MCI_STATUS,
		MCI_STATUS_ITEM, (uint32)(LPVOID)&mciStatusParms)) {

		Msg("Error reading status from midi mapper!");
		return false;
		//mciSendCommand(midi_id, MCI_CLOSE, 0, NULL);
	}
	//  return dwReturn;




	if (mciStatusParms.dwReturn == MCI_MODE_PLAY) return true;
	else
		return false;
#else
	error("TODO");
#endif
}

//thing to play the midi

void init_midi() {


}


uint32 playMIDIFile(HWND hWndNotify, LPCSTR lpszMIDIFileName) {
#ifdef TODO

	mciSendCommand(midi_id, MCI_CLOSE, 0, NULL);

	//  StopMidi();

	Msg("Attempting to play midi %s.", lpszMIDIFileName);

	uint32 dwReturn;
	MCI_OPEN_PARMS mciOpenParms;
	MCI_PLAY_PARMS mciPlayParms;
	MCI_STATUS_PARMS mciStatusParms;
	MCI_SEQ_SET_PARMS mciSeqSetParms;
	// Open the device by specifying the device and filename.
	// MCI will attempt to choose the MIDI mapper as the output port.

	mciOpenParms.lpstrDeviceType = "sequencer";
	mciOpenParms.lpstrElementName = lpszMIDIFileName;

	if (dwReturn = mciSendCommand(0, MCI_OPEN,
		MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (uint32)(LPVOID)&mciOpenParms)) {
		// Failed to open device. Don't close it; just return error.
		char ermess[129];
		//let's get the error message
		mciGetErrorString(dwReturn, ermess, 128);
		Msg("Midi error: %s", ermess);


		mciSendCommand(midi_id, MCI_CLOSE, 0, NULL);

		return dwReturn;
	}
	// The device opened successfully; get the device ID.
	midi_id = mciOpenParms.wDeviceID;

	// Check if the output port is the MIDI mapper.
	mciStatusParms.dwItem = MCI_SEQ_STATUS_PORT;
	if (dwReturn = mciSendCommand(midi_id, MCI_STATUS,
		MCI_STATUS_ITEM, (uint32)(LPVOID)&mciStatusParms)) {
		Msg("Status says no?");
		mciSendCommand(midi_id, MCI_CLOSE, 0, NULL);
		return dwReturn;
	}    // The output port is not the MIDI mapper.
	// Ask if the user wants to continue.
	/*if (LOWORD(mciStatusParms.dwReturn) != MIDI_MAPPER)
	{
	/*  if (MessageBox(hMainWnd,
	"The MIDI mapper is not available. Continue?",
	"", MB_YESNO) == IDNO)        {
	// User does not want to continue. Not an error;
	// just close the device and return.


	Msg("Middi mapper not available.  Oh well.");
	mciSendCommand(midi_id, MCI_CLOSE, 0, NULL);


	return 0L;

	}
	*/
	// Begin playback. The window procedure function for the parent
	// window will be notified with an MM_MCINOTIFY message when
	// playback is complete. At this time, the window procedure closes
	// the device.    mciPlayParms.dwCallback = (uint32) hWndNotify;
	if (dwReturn = mciSendCommand(midi_id, MCI_PLAY, MCI_NOTIFY,
		(uint32)(LPVOID)&mciPlayParms)) {
		mciSendCommand(midi_id, MCI_CLOSE, 0, NULL);
		return dwReturn;
	}


	return 0L;

#else
	error("TODO");
#endif
}

bool PlayMidi(const char *sFileName) {
	//no midi stuff right now
	if (sound_on == false) return true;

	char buf[256];
	char crap[256];

	if (compare(last_midi, sFileName)) {
		if (nothing_playing()) {
			Msg("I think %s is already playing, I should skip it...", sFileName);
			return false;
		}

	}

	sprintf(crap, "sound\\%s", sFileName);
	if (Common::File::exists(crap)) {

	} else {
		sprintf(crap, "..\\dink\\sound\\%s", sFileName);
		if (!Common::File::exists(crap)) {
			Msg("Error playing midi %s, doesn't exist in any dir.", sFileName);
			return false;
		}

	}

	strcpy(last_midi, sFileName);
	Msg("Killing cd...");
	last_cd = 0;
	mciSendCommand(CD_ID, MCI_STOP, 0, NULL);
	//killcd(g_hWnd, 1);

	playMIDIFile(g_hWnd, crap);
	return true;

	sprintf(buf, "open %s type sequencer alias MUSIC", crap);

	if (mciSendString("close MUSIC", NULL, 0, NULL) != 0) {
		Msg("Couldn't close current midi..");
		//  return false;
	}

	if (mciSendString(buf, NULL, 0, NULL) != 0) {
		Msg("Couldn't clear midi buffer");
		return false;
	}


	if (mciSendString("play MUSIC from 0", NULL, 0, g_hWnd) != 0) {
		Msg("Playmidi command failed");
		return false;
	}

	return true;
}


void check_midi() {
	char hold[20];

	if (!midi_active) return;
	if (map.music[*pmap] != 0) {

		if (map.music[*pmap] == -1) {
			//kill music
			mciSendCommand(CD_ID, MCI_CLOSE, 0, NULL);
			Msg("Stopped cd");
			killCD(1);

		}

		if (map.music[*pmap] > 1000) {

			if (cd_inserted) {
				if (map.music[*pmap] - 1000 == last_cd)
					if (cdplaying()) return;

				PlayCD(map.music[*pmap] - 1000);
			} else {

				sprintf(hold, "%d.mid", map.music[*pmap] - 1000);
				PlayMidi(hold);
			}
			return;
		}

		//there is music associated with this screen
		sprintf(hold, "%d.mid", map.music[*pmap]);
		PlayMidi(hold);
	}


}

void openCD() {
	cd_inserted = g_system->getAudioCDManager()->open();
}

void closeCD() {
	g_system->getAudioCDManager()->close();
}

uint32 PlayCD(byte bTrack) {
	g_system->getAudioCDManager()->play(bTrack, 1, 0, 0);
	return 0;
}

uint32 killCD(byte bTrack) {
	g_system->getAudioCDManager()->stop();
	return 0;
}

bool cdplaying() {
	return g_system->getAudioCDManager()->isPlaying();
}

//------------------------------------------------------------------
//
// Function     : PauseMidi()
//
// Purpose      : Pauses midi file
//
//------------------------------------------------------------------

bool PauseMidi() {
	// Pause if we're not already paused...
	if (mciSendString("stop MUSIC", NULL, 0, NULL) != 0) {
		return false;
	}


	// Yahoo!
	return true;
}

//------------------------------------------------------------------
//
// Function     : ResumeMidi()
//
// Purpose      : Resumes playing of a midi file
//
//------------------------------------------------------------------

bool ResumeMidi() {
	// Resume midi
	if (mciSendString("play MUSIC notify", NULL, 0, g_hWnd) != 0) {
		return false;
	}

	// Yahoo!
	return true;
}

//------------------------------------------------------------------
//
// Function     : StopMidi
//
// Purpose      : Stops a midi file playing
//
//------------------------------------------------------------------

bool StopMidi() {

	playMIDIFile(g_hWnd, "");

	// Yahoo!
	return true;
}

//------------------------------------------------------------------
//
// Function     : ReplayMidi()
//
// Purpose      : Replays a midi file
//
//------------------------------------------------------------------

bool ReplayMidi() {
	// Replay midi
	if (mciSendString("play MUSIC from 0 notify", NULL, 0, g_hWnd) != 0) {
		return false;
	}

	// Yahoo!
	return true;
}

} // namespace Dink
