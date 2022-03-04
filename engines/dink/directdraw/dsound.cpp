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

#include "common/textconsole.h"
#include "dink/directdraw/dsound.h"

namespace Dink {

HRESULT DirectSoundCreate(void *pcGuidDevice, LPDIRECTSOUND *ppDS, LPUNKNOWN pUnkOuter) {
	error("TODO");
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
	error("TODO");
}

HRESULT IDirectSound::DuplicateSoundBuffer(LPDIRECTSOUNDBUFFER pDSBufferOriginal, _Outptr_ LPDIRECTSOUNDBUFFER *ppDSBufferDuplicate) {
	error("TODO");
}

HRESULT IDirectSound::Release() {
	error("TODO");
}

} // namespace Dink
