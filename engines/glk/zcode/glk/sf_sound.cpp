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

#include "glk/zcode/glk/sf_frotz.h"
#include "glk/zcode/blorb/blorblow.h"
#include "common/textconsole.h"

namespace Glk {
namespace ZCode {

// Temporary stubs
struct Mix_Music {
	int v1;
};
struct Mix_Chunk {
	int v1;
};
void Mix_CloseAudio() {}
void Mix_HaltChannel(int v1) {}
void Mix_HaltMusic() {}

/*************
 * AUDIO!!!
 *
 */
enum { SFX_TYPE, MOD_TYPE };

typedef struct EFFECT {
	void (*destroy)(struct EFFECT *);
	int number;
	int type;
	int active;
	int voice;
	Mix_Music *mod;
	Mix_Chunk *sam;
	int repeats;
	int volume;
	int ended;
	zword eos;
	ulong endtime;
} EFFECT;

/* no effects cache */
static EFFECT *e_sfx = nullptr;
static EFFECT *e_mod = nullptr;

int SFaudiorunning = 0;
volatile int end_of_sound_flag = 0;
int m_no_sound = 0;
int m_frequency = 44100;

static int audio_rate, audio_channels;

/* set this to any of 512,1024,2048,4096
 * the higher it is, the more FPS shown and CPU needed
 */
static int audio_buffers = 512;
static uint16 audio_format;
static int bits;

static void finishaudio()
{
	if (!SFaudiorunning)
		return;
	os_stop_sample(0);
	SFaudiorunning = 0;

	/* flush cache */
	if (e_sfx)
		e_sfx->destroy(e_sfx);
	if (e_mod)
		e_mod->destroy(e_mod);

	e_sfx = e_mod = nullptr;

	Mix_CloseAudio();
}

static void music_finished(void);
static void channel_finished(int channel);

/*
 * This interface is broken.  The function gets called regardless of
 * whether sound is desired and has no means for indicating whether sound is
 * available.  See sf_initsound for the real init.
 */
void os_init_sound()
{
}

int sf_initsound() {
	if (SFaudiorunning)
		return 1;
	if (m_no_sound)
		return 0;
	SFaudiorunning = 1;
#ifdef TODO
	/* initialize sdl mixer, open up the audio device */
	if (Mix_OpenAudio(m_frequency, MIX_DEFAULT_FORMAT, 2, audio_buffers) <
	    0) {
		SFaudiorunning = 0;
		return 0;
	}

	Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
	bits = audio_format & 0xFF;

	/* hook for end-of-music */
	Mix_HookMusicFinished(music_finished);
	/* hook for end-of-sfx */
	Mix_ChannelFinished(channel_finished);

	Mix_AllocateChannels(1);
	CLEANREG(finishaudio);
#endif
	return 1;
}


static void baredestroy(EFFECT * r) {
#ifdef TODO
	if (r) {
		if (r->mod)
			Mix_FreeMusic(r->mod);
		if (r->sam)
			Mix_FreeChunk(r->sam);
		free(r);
	}
#endif
}


static EFFECT *new_effect(int type, int num)
{
	EFFECT *reader = (EFFECT *) calloc(1, sizeof(EFFECT));
	if (reader) {
		reader->type = type;
		reader->number = num;
		reader->destroy = baredestroy;
	}
	return (EFFECT *) reader;
}


/* According to specs, this is only called when music ends "naturally",
 * which I take for "not halted programmatically"
 */
static void music_finished(void)
{
	if (!e_mod)
		return;
	if (!e_mod->active)
		return;
	e_mod->active = 0;
	e_mod->ended = 1;
}


/* This may be called also via a Mix_Haltetc. */
static void channel_finished(int channel)
{
	if (channel != 0)
		return;
	if (!e_sfx)
		return;
	if (!e_sfx->active)
		return;
	e_sfx->active = 0;
	e_sfx->ended = 1;	/* stopsample will take care of this... */
}


static void stopsample()
{
	if (!e_sfx)
		return;
	e_sfx->active = 0;
	Mix_HaltChannel(0);
	e_sfx->ended = 0;
}


static void stopmodule()
{
	if (!e_mod)
		return;
	e_mod->active = 0;
	Mix_HaltMusic();
	e_mod->ended = 0;
}


static void startsample()
{
	if (!e_sfx)
		return;
#ifdef TODO
	Mix_PlayChannel(0, e_sfx->sam, e_sfx->repeats);
	Mix_Volume(0, e_sfx->volume);
#endif
	e_sfx->active = 1;
}


static void startmodule()
{
	if (!e_mod)
		return;
#ifdef TODO
	Mix_PlayMusic(e_mod->mod, e_mod->repeats);
	Mix_VolumeMusic(e_mod->volume);
#endif
	e_mod->active = 1;
}


static EFFECT *getaiff(FILE * f, size_t pos, int len, int num) {
#ifdef TODO
	EFFECT *res = nullptr;

	res = new_effect(SFX_TYPE, num);
	if (!res)
		return nullptr;
	if (fseek(f, pos, SEEK_SET)
	    || !(res->sam = Mix_LoadWAV_RW(SDL_RWFromFP(f, false), 1))) {
		os_warn("Read error on audio data: %s", strerror(errno));
		res->destroy(res);
		return nullptr;
	}
	return res;
#else
	return nullptr;
#endif
}


static EFFECT *getmodule(FILE * f, size_t pos, int len, int num)
{
#ifdef TODO
	EFFECT *res;
	byte h[2];

	res = new_effect(MOD_TYPE, num);
	if (!res)
		return nullptr;

	fseek(f, pos, SEEK_SET);
	fread(h, 1, 2, f);
	fseek(f, pos, SEEK_SET);
	if (h[0] == 'P' && h[1] == 'K')	/* zipped module */
	{
		int size;
		void *data;
		int st = sf_pkread(f, pos, &data, &size);
		if (st) {
			res->destroy(res);
			return nullptr;
		}
		res->mod = Mix_LoadMUS_RW(SDL_RWFromMem(data, size), true);
		free(data);
	} else {
		res->mod = Mix_LoadMUS_RW(SDL_RWFromFP(f, false), true);
	}
	if (!res->mod) {
		res->destroy(res);
		return nullptr;
	}
	return res;
#else
	error("TODO: getmodule");
#endif
}


static EFFECT *geteffect(int num)
{
	myresource res;
	EFFECT *result = nullptr;
	unsigned int id;

	if ((e_sfx) && (e_sfx->number == num))
		return e_sfx;
	if ((e_mod) && (e_mod->number == num))
		return e_mod;

	if (sf_getresource(num, 0, bb_method_FilePos, &res) != bb_err_None)
		return nullptr;

	/* Look for a recognized format */
	id = res.type;

	if (id == bb_ID_FORM) {
		result =
		    getaiff(res.file, res.bbres.data.startpos, res.bbres.length,
			    num);
	} else if (id == bb_ID_MOD || id == bb_ID_OGGV) {
		result =
		    getmodule(res.file, res.bbres.data.startpos,
			      res.bbres.length, num);
	}
	sf_freeresource(&res);

	return result;
}

/* sound handling */

/*
 * os_beep
 *
 * Play a beep sound. Ideally, the sound should be high- (number == 1)
 * or low-pitched (number == 2).
 *
 */
void os_beep(int number) {
#ifdef TODO
	if (m_no_sound)
		return;
	printf("\a");
	fflush(stdout);
	if (!SFaudiorunning)
		return;
/*	theWnd->FlushDisplay();
	::MessageBeep(MB_ICONEXCLAMATION);*/
#else
	error("TODO: os_beep");
#endif
}


/*
 * os_finish_with_sample
 *
 * Remove the current sample from memory (if any).
 *
 */
void os_finish_with_sample(int number)
{
	if (!SFaudiorunning)
		return;
	os_stop_sample(number);
}


/*
 * os_prepare_sample
 *
 * Load the given sample from the disk.
 *
 */
void os_prepare_sample(int number)
{
	if (!SFaudiorunning)
		return;
}


/*
 * os_start_sample
 *
 * Play the given sample at the given volume (ranging from 1 to 8 and
 * 255 meaning a default volume). The sound is played once or several
 * times in the background (255 meaning forever). The end_of_sound
 * function is called as soon as the sound finishes, passing in the
 * eos argument.
 *
 */
void os_start_sample(int number, int volume, int repeats, zword eos)
{
	EFFECT *e;

	if (!SFaudiorunning)
		return;


	/* NOTE: geteffect may return an already loaded effect */
	e = geteffect(number);
	if (!e)
		return;
	if (e->type == SFX_TYPE)
		stopsample();
	else
		stopmodule();
	if (repeats < 1)
		repeats = 1;
	if (repeats == 255)
		repeats = -1;
	if (volume < 0)
		volume = 0;
	if (volume > 8)
		volume = 8;
	if (e->type == SFX_TYPE && repeats > 0)
		repeats--;
	e->repeats = repeats;
	e->volume = 32 * volume;
	e->eos = eos;
	e->ended = 0;
	if (e->type == SFX_TYPE) {
		if ((e_sfx) && (e_sfx != e))
			e_sfx->destroy(e_sfx);
		e_sfx = e;
		startsample();
	} else {
		if ((e_mod) && (e_mod != e))
			e_mod->destroy(e_mod);
		e_mod = e;
		startmodule();
	}
}


/*
 * os_stop_sample
 *
 * Turn off the current sample.
 *
 */
void os_stop_sample(int number)
{
	if (!SFaudiorunning)
		return;
	if (number == 0) {
		stopsample();
		stopmodule();
		return;
	}
	if ((e_sfx) && (e_sfx->number == number))
		stopsample();
	if ((e_mod) && (e_mod->number == number))
		stopmodule();
}


void sf_checksound()
{
	if ((e_sfx) && (e_sfx->ended)) {
		e_sfx->ended = 0;
		if (e_sfx->eos) {
			end_of_sound_flag = 1;
			end_of_sound();
		}
	}
	if ((e_mod) && (e_mod->ended)) {
		e_mod->ended = 0;
		if (e_mod->eos) {
			end_of_sound_flag = 1;
			end_of_sound();
		}
	}
}

} // End of namespace ZCode
} // End of namespace Glk
