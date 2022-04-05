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

#include "dink/directdraw/dsound.h"
#include "dink/file.h"
#include "audio/decoders/wave.h"
#include "common/memstream.h"
#include "common/textconsole.h"

namespace Dink {

HRESULT DirectSoundCreate(void *pcGuidDevice, LPDIRECTSOUND *ppDS, LPUNKNOWN pUnkOuter) {
	*ppDS = new IDirectSound();
	return DS_OK;
}

IDirectSoundBuffer *IDirectSoundBuffer::createWav(const Common::String &name) {
	File f;
	if (!f.open(name))
		return false;

	// Create the buffer and read the file contents
	IDirectSoundBuffer *buf = new IDirectSoundBuffer();
	buf->_buffer.resize(f.size());
	f.read(&buf->_buffer[0], f.size());

	// Create the wave file decoder
	buf->_audioStream = Audio::makeWAVStream(
		new Common::MemoryReadStream(&buf->_buffer[0], f.size(), DisposeAfterUse::NO),
		DisposeAfterUse::YES);
	if (!buf->_audioStream) {
		delete buf;
		return nullptr;
	}

	return buf;
}

IDirectSoundBuffer::IDirectSoundBuffer(const IDirectSoundBuffer &src) {
	// Copy the buffer
	size_t size = src._buffer.size();
	_buffer.resize(size);
	Common::copy(&src._buffer[0], &src._buffer[0] + size, &_buffer[0]);

	// Make a new audio stream
	_audioStream = Audio::makeWAVStream(
		new Common::MemoryReadStream(&_buffer[0], size, DisposeAfterUse::NO),
		DisposeAfterUse::YES);
	assert(_audioStream);
}

IDirectSoundBuffer::~IDirectSoundBuffer() {
	delete _audioStream;
}

HRESULT IDirectSoundBuffer::Play(uint32 dwReserved1, uint32 dwPriority, uint32 dwFlags) {
	error("TODO");
}

HRESULT IDirectSoundBuffer::SetVolume(int lVolume) {
	error("TODO");
}

HRESULT IDirectSoundBuffer::SetPan(int lPan) {
	error("TODO");
}

HRESULT IDirectSoundBuffer::SetFrequency(uint32 freq) {
	error("TODO");
}

HRESULT IDirectSoundBuffer::SetCurrentPosition(uint32 dwNewPosition) {
	error("TODO");
}

HRESULT IDirectSoundBuffer::Stop() {
	error("TODO");
}

HRESULT IDirectSoundBuffer::Release() {
	error("TODO");
}

HRESULT IDirectSoundBuffer::GetStatus(unsigned long *status) {
	error("TODO");
}


HRESULT CreateSoundBuffer(LPCDSBUFFERDESC pcDSBufferDesc, LPDIRECTSOUNDBUFFER *ppDSBuffer, LPUNKNOWN pUnkOuter) {
	error("TODO");
}

HRESULT IDirectSound::CreateSoundBuffer(LPCDSBUFFERDESC pcDSBufferDesc, LPDIRECTSOUNDBUFFER *ppDSBuffer, LPUNKNOWN pUnkOuter) {
	error("TODO");
}

HRESULT IDirectSound::SetCooperativeLevel(HWND hwnd, DWORD dwLevel) {
	return DS_OK;
}

HRESULT IDirectSound::DuplicateSoundBuffer(LPDIRECTSOUNDBUFFER pDSBufferOriginal, LPDIRECTSOUNDBUFFER *ppDSBufferDuplicate) {
	*ppDSBufferDuplicate = new IDirectSoundBuffer(*pDSBufferOriginal);
	return DS_OK;
}

HRESULT IDirectSound::Release() {
	return DS_OK;
}

} // namespace Dink
