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

#include "audio/mixer.h"
#include "audio/decoders/wave.h"
#include "common/memstream.h"
#include "krondor/sound.h"
#include "krondor/krondor.h"

namespace Krondor {


/* Standard MIDI File tags & constants */
static const uint32_t SMF_HEADER = 0x6468544d;
static const uint32_t SMF_TRACK = 0x6b72544d;
static const uint16_t SMF_FORMAT = 0;
static const uint32_t SMF_HEADER_SIZE = 6;
static const uint16_t SMF_PPQN = 32;

/* MIDI event codes */
static const uint8_t MIDI_NOTE_OFF = 0x80;
static const uint8_t MIDI_NOTE_ON = 0x90;
static const uint8_t MIDI_KEY = 0xa0;
static const uint8_t MIDI_CONTROL = 0xb0;
static const uint8_t MIDI_PATCH = 0xc0;
static const uint8_t MIDI_CHANNEL = 0xd0;
static const uint8_t MIDI_PITCH = 0xe0;
static const uint8_t MIDI_SYSEX = 0xf0;
static const uint8_t MIDI_TIMING = 0xf8;
static const uint8_t MIDI_SEQ_START = 0xfa;
static const uint8_t MIDI_SEQ_CONT = 0xfb;
static const uint8_t MIDI_SEQ_END = 0xfc;
static const uint8_t MIDI_META = 0xff;

/* MIDI Meta events */
static const uint8_t META_SEQNUM = 0x00;
static const uint8_t META_TEXT = 0x01;
static const uint8_t META_COPYRIGHT = 0x02;
static const uint8_t META_TRACK = 0x03;
static const uint8_t META_INSTRUMENT = 0x04;
static const uint8_t META_LYRIC = 0x05;
static const uint8_t META_MARKER = 0x06;
static const uint8_t META_CUE = 0x07;
static const uint8_t META_CHANNEL = 0x20;
static const uint8_t META_PORT = 0x21;
static const uint8_t META_EOT = 0x2f;
static const uint8_t META_TEMPO = 0x51;
static const uint8_t META_SMPTE = 0x54;
static const uint8_t META_TIME = 0x58;
static const uint8_t META_KEY = 0x59;
static const uint8_t META_SEQDATA = 0x7f;


Sound::~Sound() {
	free(_data);
}

void Sound::load(Common::SeekableReadStream *src) {
	if (_data)
		// TODO: xbak seemed to overwrite samples
		return;

	byte code = src->readByte();
	_channel = code & 0x0f;

	if (code == 0xfe) {
		_format = SF_WAVE;

		_size = src->size();
		byte *data = (byte *)malloc(_size);
		src->read(data, _size);

	} else {
		_format = SF_MIDI;
		parseMidi(src);
	}
}

void Sound::play() {
	if (_format == SF_WAVE) {
		Common::MemoryReadStream *stream = new Common::MemoryReadStream(_data, _size);
		Audio::SeekableAudioStream *audioStream = Audio::makeWAVStream(
			stream, DisposeAfterUse::YES);
		g_engine->_mixer->playStream(Audio::Mixer::kSFXSoundType,
			&_soundHandle, audioStream);

	} else if (_format == SF_MIDI) {
		g_engine->getMidi()->play(this);

	} else {
		error("Unknown format");
	}
}

void Sound::parseMidi(Common::SeekableReadStream *src) {
	uint delta;
	uint code;
	uint mode = 0;
	uint tick = 0;
	Common::MultiMap<uint, MidiEvent> midiEvents;

	src->skip(1);
	while ((mode != MIDI_SEQ_END) && !src->eos()) {
		delta = 0;
		code = src->readByte();

		while (code == MIDI_TIMING) {
			delta += 240;
			code = src->readByte();
		}

		delta += code;
		code = src->readByte();

		if (((code & 0xf0) == MIDI_NOTE_ON) ||
				((code & 0xf0) == MIDI_CONTROL) ||
				((code & 0xf0) == MIDI_PATCH) ||
				((code & 0xf0) == MIDI_PITCH)) {
			mode = code;
			if ((code & 0x0f) != _channel)
				error("Invalid MIDI channel");

		} else if (code == MIDI_SEQ_END) {
			mode = code;

		} else {
			src->seek(-1, SEEK_CUR);
		}

		if (mode != MIDI_SEQ_END) {
			MidiEvent me;
			memset(&me, 0, sizeof(MidiEvent));
			me.data[0] = mode;
			switch (mode & 0xf0)
			{
			case MIDI_NOTE_ON:
				me.data[1] = src->readByte();
				me.data[2] = src->readByte();
				if (me.data[2] == 0)
				{
					me.data[0] = MIDI_NOTE_OFF | _channel;
				}
				me.size = 3;
				break;
			case MIDI_CONTROL:
			case MIDI_PITCH:
				me.data[1] = src->readByte();
				me.data[2] = src->readByte();
				me.size = 3;
				break;
			case MIDI_PATCH:
				me.data[1] = src->readByte();
				me.size = 2;
				break;
			default:
				if (mode == MIDI_SEQ_END) {
					me.size = 1;
				} else {
					error("Unknown MIDI mode");
				}
				break;
			}

			tick += delta;
			midiEvents.insert(Common::Pair<uint, MidiEvent>(tick, me));
		}
	}

	// Now the formatted MIDI data
	generateMidi(midiEvents);
}

void Sound::generateMidi(Common::MultiMap<uint, MidiEvent> &midiEvents) {
	Common::MemoryWriteStreamDynamic buf(DisposeAfterUse::NO);
	uint size = 0;
	uint tick = 0;

	for (Common::MultiMap<uint, MidiEvent>::iterator it = midiEvents.begin();
			it != midiEvents.end(); ++it) {
		MidiEvent &me = (*it).second;
		me.delta = (*it).first - tick;
		size += 1;

		if (me.delta >= (1 << 7)) {
			size += 1;
		}
		if (me.delta >= (1 << 14)) {
			size += 1;
		}
		if (me.delta >= (1 << 21)) {
			size += 1;
		}
		size += me.size;
		tick = (*it).first;
	}

	buf.writeUint32LE(SMF_HEADER);
	buf.writeUint32BE(SMF_HEADER_SIZE);
	buf.writeUint16BE(SMF_FORMAT);
	buf.writeUint16BE(1);
	buf.writeUint16BE(SMF_PPQN);
	buf.writeUint32LE(SMF_TRACK);
	buf.writeUint32BE(size);

	for (Common::MultiMap<uint, MidiEvent>::iterator it = midiEvents.begin();
			it != midiEvents.end(); ++it) {
		MidiEvent &me = (*it).second;
		putVariableLength(&buf, me.delta);

		for (uint i = 0; i < me.size; i++) {
			buf.writeByte(me.data[i]);
		}
	}

	buf.writeByte(0);
	buf.writeByte(MIDI_META);
	buf.writeByte(META_EOT);
	buf.writeByte(0);

	// Store the data
	_data = buf.getData();
	_size = buf.size();
}

void Sound::putVariableLength(Common::WriteStream *dest, uint n) {
	unsigned int tmp = (n & 0x7f);
	unsigned int k = 1;
	while (n >>= 7) {
		tmp <<= 8;
		tmp |= ((n & 0x7f) | 0x80);
		k++;
	}

	while (k--) {
		dest->writeByte(tmp & 0xff);
		tmp >>= 8;
	}
}

} // namespace Krondor
