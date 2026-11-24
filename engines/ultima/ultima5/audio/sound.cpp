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

#include "ultima/ultima5/audio/sound.h"
#include "audio/decoders/raw.h"
#include "common/system.h"

namespace Ultima {
namespace Ultima5 {

constexpr int SAMPLE_RATE = 44100;
constexpr int AMPLITUDE = 5000;
#ifdef SCUMM_LITTLE_ENDIAN
constexpr byte SAMPLE_ENDIAN = Audio::FLAG_LITTLE_ENDIAN;
#else
constexpr byte SAMPLE_ENDIAN = 0;
#endif

// A pulse is worked out a step at a time, a step being about this many to the second
constexpr double PULSE_STEPS_PER_SECOND = 24000.0;

// A step of noise lasts this many sixteenths of a millisecond for each unit of its rate
constexpr double NOISE_STEP_MS_PER_RATE = 1.0 / 16.0;

constexpr int NOISE_LOWEST = 100;
constexpr byte TILE_RIDING_HORSE = 0x12;
constexpr byte TILE_AVATAR = 0x1C;
constexpr int MAX_SAMPLES = SAMPLE_RATE * 6;

Sound::~Sound() {
	stop();
}

void Sound::toggle() {
	_on = !_on;

	if (!_on)
		stop();
}

void Sound::stop() {
	if (g_system && g_system->getMixer())
		g_system->getMixer()->stopHandle(_handle);

	_stream = nullptr;
}

void Sound::addTone(Samples &samples, double freq, double ms) {
	const int count = (int)(SAMPLE_RATE * ms / 1000.0 + 0.5);
	const double step = freq / SAMPLE_RATE;

	for (int i = 0; i < count && (int)samples.size() < MAX_SAMPLES; ++i) {
		_phase += step;
		if (_phase >= 1.0)
			_phase -= (int)_phase;

		samples.push_back(_phase < 0.5 ? AMPLITUDE : -AMPLITUDE);
	}
}

void Sound::addSilence(Samples &samples, double ms) {
	const int count = (int)(SAMPLE_RATE * ms / 1000.0 + 0.5);

	for (int i = 0; i < count && (int)samples.size() < MAX_SAMPLES; ++i)
		samples.push_back(0);
}

void Sound::addNoise(Samples &samples, int rate, int duration, int limit) {
	const double stepMs = rate * NOISE_STEP_MS_PER_RATE;
	int total = 0;
	int span = limit - NOISE_LOWEST + 1;

	if (span < 1)
		span = 1;

	do {
		// The next tone is picked from a shuffled number
		uint16 value = _noiseState + 0x9248;
		value = (value >> 3) | (value << 13);
		value ^= 0x9248;
		value += 0x11;
		_noiseState = value;

		addTone(samples, NOISE_LOWEST + value % span, stepMs < 0.03 ? 0.03 : stepMs);
		total += rate;
	} while (total < duration && (int)samples.size() < MAX_SAMPLES);
}

void Sound::addSweep(Samples &samples, int from, int to, int step, int duration) {
	if (step < 1 || duration < 1)
		return;

	const int change = (to - from) * step / duration;
	int freq = from;

	for (int i = 0; i < duration; i += step) {
		addTone(samples, freq < 1 ? 1 : freq, step);
		freq += change;
	}
}

void Sound::addPulse(Samples &samples, int freq, int delay, int duration, int width, int increment) {
	const double rate = PULSE_STEPS_PER_SECOND / (delay < 1 ? 1 : delay);
	const double stepsPerSample = rate / SAMPLE_RATE;
	const uint32 count = duration == 0 ? 0x10000 : (uint16)duration;
	const uint32 total = (uint32)(count / stepsPerSample);
	uint16 accumulator = 0;
	uint16 threshold = (uint16)width;
	uint32 done = 0;
	bool on = false;

	// The speaker is on whenever the accumulator is above the threshold, which moves as it goes
	for (uint32 i = 0; i < total && (int)samples.size() < MAX_SAMPLES; ++i) {
		const uint32 target = (uint32)((i + 1) * stepsPerSample);

		for (; done < target && done < count; ++done) {
			accumulator += (uint16)freq;
			on = accumulator > threshold;
			threshold += (uint16)increment;
		}

		samples.push_back(on ? AMPLITUDE : -AMPLITUDE);
	}
}

void Sound::play(const Samples &samples) {
	if (!_on || samples.empty() || !g_system)
		return;

	Audio::Mixer *mixer = g_system->getMixer();
	if (!mixer)
		return;

	if (!_stream || !mixer->isSoundHandleActive(_handle)) {
		_stream = Audio::makeQueuingAudioStream(SAMPLE_RATE, false);
		mixer->playStream(Audio::Mixer::kSFXSoundType, &_handle, _stream);
	}

	const uint size = samples.size() * sizeof(int16);
	int16 *data = (int16 *)malloc(size);
	memcpy(data, &samples[0], size);

	_stream->queueBuffer((byte *)data, size, DisposeAfterUse::YES, Audio::FLAG_16BITS | SAMPLE_ENDIAN);
}

void Sound::pulse(int freq, int delay, int duration, int width, int increment) {
	if (!_on)
		return;

	Samples samples;
	addPulse(samples, freq, delay, duration, width, increment);
	play(samples);
}

void Sound::noise(int rate, int duration, int limit) {
	if (!_on)
		return;

	Samples samples;
	addNoise(samples, rate, duration, limit);
	play(samples);
}

void Sound::tone(int freq, int ms) {
	if (!_on)
		return;

	Samples samples;
	addTone(samples, freq, ms);
	play(samples);
}

void Sound::sweep(int from, int to, int step, int duration) {
	if (!_on)
		return;

	Samples samples;
	addSweep(samples, from, to, step, duration);
	play(samples);
}

void Sound::footstep() {
	if (!_on)
		return;

	Samples samples;
	addNoise(samples, 1, 0x19, 1000);
	addSilence(samples, 20);
	addNoise(samples, 1, 0x19, 1500);
	play(samples);
}

void Sound::walking(byte partyTile, bool onHorse) {
	if ((partyTile & 0xFE) == TILE_RIDING_HORSE || (!onHorse && partyTile == TILE_AVATAR))
		footstep();
}

void Sound::blocked() {
	tone(0xA5, 200);
}

void Sound::victory() {
	if (!_on)
		return;

	Samples samples;
	for (int i = 0; i < 3; ++i)
		addPulse(samples, 0x11F8, 1, 0x2A30, 300, 6);

	addPulse(samples, 0x17D4, 1, 0x5460, 300, 3);
	play(samples);
}

void Sound::refuse() {
	sweep(0x4B0, 2000, 1, 0x28);
}

void Sound::alert() {
	sweep(800, 2000, 1, 0x32);
}

void Sound::flare(int freq, int duration) {
	if (!_on)
		return;

	Samples samples;
	for (int width = 2000; width < 25000; width += 0x32)
		addPulse(samples, freq, 1, duration, width, 0);
	for (int width = 25000; width > 2000; width -= 0x32)
		addPulse(samples, freq, 1, duration, width, 0);

	play(samples);
}

void Sound::apparition() {
	if (!_on)
		return;

	static const uint16 NOTES[6] = { 0x0A3C, 0x0A3C, 0x0A3C, 0x0E74, 0x0F3C, 0x1040 };
	Samples samples;

	addPulse(samples, 0x0A3C, 1, 10000, 0x9C4, 6);
	for (int i = 0; i < 6; ++i)
		addPulse(samples, NOTES[i], 1, 5000, 200, 0xD);

	play(samples);
}

void Sound::apparitionHeals() {
	if (!_on)
		return;

	Samples samples;
	addPulse(samples, 5500, 1, 5000, 200, 13);
	addPulse(samples, 5500, 1, 60000, 2500, 1);
	play(samples);
}

void Sound::healerLight() {
	if (!_on)
		return;

	Samples samples;
	addPulse(samples, 0x100E, 1, 0x57E4, 5000, 1);
	addPulse(samples, 0x100E, 1, 0x57E4, 0x6B6C, -1);
	addPulse(samples, 0x11B2, 1, 40000, 1, 1);
	addPulse(samples, 0x11B2, 1, 40000, 40000, -1);
	addPulse(samples, 0x08FC, 1, 18000, 1, 2);
	addPulse(samples, 0x08FC, 1, 18000, 36000, -2);
	play(samples);
}

static const uint16 HARPSICHORD_NOTES[10] = { 0x1EAB, 0x0C2C, 0x0DA9, 0x0F56, 0x103F, 0x123C, 0x1478, 0x16FA, 0x1857, 0x1B53 };

void Sound::harpsichord(int note) {
	if (note >= 0 && note < 10)
		pulse(HARPSICHORD_NOTES[note], 1, 4000, 20000, -4);
}

void Sound::absorbed() {
	pulse(0x2648, 1, 28000, 1000, 2);
}

// What is played for each of the circles of spells
static const uint16 CAST_FREQUENCY[9] = { 0x226A, 0x1E96, 0x1B94, 0x1996, 0x173E, 0x15C2, 0x143C, 0x12D4, 0x1180 };
static const uint16 CAST_WIDTH_FIRST[9] = { 0x0A8C, 0x0BB8, 0x03E8, 0x0064, 0x1388, 0x0FA0, 0x09C4, 0x03E8, 0x0001 };
static const uint16 CAST_WIDTH_SECOND[9] = { 0x7FBC, 0x7918, 0x9088, 0xAFC8, 0x7918, 0x84D0, 0x8E94, 0x9858, 0xA410 };
static const uint16 CAST_INCREMENT[9] = { 3, 2, 2, 2, 1, 1, 1, 1, 1 };

void Sound::castEffect(int circle) {
	if (!_on)
		return;

	Samples samples;

	if (circle < 9) {
		const int length = circle * 4000 + 10000;

		addNoise(samples, 800, circle * 0x640 + 8000, 700);
		addPulse(samples, CAST_FREQUENCY[circle], 1, length, CAST_WIDTH_FIRST[circle], CAST_INCREMENT[circle]);
		addPulse(samples, CAST_FREQUENCY[circle], 1, length, CAST_WIDTH_SECOND[circle], -CAST_INCREMENT[circle]);
	} else {
		addPulse(samples, 0x1180, 1, 65000, 300, 1);
	}

	play(samples);
}

} // namespace Ultima5
} // namespace Ultima
