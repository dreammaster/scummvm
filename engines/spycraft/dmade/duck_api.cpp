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
#include "spycraft/dmade/duck_api.h"

namespace Spycraft {

void DumpDelta() {
	warning("TODO: duck_api");
}

void BlackBox(unsigned char *addr, int x, int y, int w, int h, int pitch) {
	warning("TODO: duck_api");
}

void BlackDuck(DXL_VSCREEN_HANDLE vsp) {
	warning("TODO: duck_api");
}

void Announcement(const char *lpszString) {
	warning("TODO: duck_api");
}

void AnnDone() {
	warning("TODO: duck_api");
}

void longjmpOnAudioErrors(jmp_buf *pErrorContext) {
	warning("TODO: duck_api");
}
void ErrorBuffer(char *errorbuf) {
	warning("TODO: duck_api");
}
int RestoreDirty(DXL_XIMAGE_HANDLE pXImageint, DXL_VSCREEN_HANDLE dst) {
	warning("TODO: duck_api");
	return 0;
}
int doVGASetup(DXL_VSCREEN_HANDLE dst, char *bgstring) {
	warning("TODO: duck_api");
	return 0;
}
void resetDisplay(DXL_VSCREEN_HANDLE dst) {
	warning("TODO: duck_api");
}
void flip_screens(DXL_VSCREEN_HANDLE dst) {
	warning("TODO: duck_api");
}
void MarkSkips(DXL_VSCREEN_HANDLE dst, int skipnum) {
	warning("TODO: duck_api");
}
int GetAudioProfile() {
	warning("TODO: duck_api");
	return 0;
}
int Get_show_dots() {
	warning("TODO: duck_api");
	return 0;
}
void Set_show_dots(int fred) {
	warning("TODO: duck_api");
}
int DLLVersion() {
	warning("TODO: duck_api");
	return 0;
}
int Get_ZBuffer() {
	warning("TODO: duck_api");
	return 0;
}
void NewTrigger(int val) {
	warning("TODO: duck_api");
}
int FillAudio(HFB_BUFFER_HANDLE HFBuffer, MFP_STREAM_HANDLE AStream, int *pAudioChunkIndex,
	void **audaddr, long *audlen, int buffPreload, int MultiBlock) {
	warning("TODO: duck_api");
	return 0;
}
int SetupAudio(HFB_BUFFER_HANDLE HFBuffer, MFP_STREAM_HANDLE AStream, int *pAudioChunkIndex,
	void **audaddr, long *audlen, int buffPreload) {
	warning("TODO: duck_api");
	return 0;
}
void StartPlaying() {
	warning("TODO: duck_api");
}

void StopPlaying() {
	warning("TODO: duck_api");
}

void EndAudio() {
	warning("TODO: duck_api");
}

void TakeBack() {
	warning("TODO: duck_api");
}

void PauseSpeaker() {
	warning("TODO: duck_api");
}

void ResumeSpeaker() {
	warning("TODO: duck_api");
}

int TimeSpeaker() {
	warning("TODO: duck_api");
	return 0;
}

int IsPlaying() {
	warning("TODO: duck_api");
	return 0;
}

void SetStates(spkrst8 speaker, int S3style) {
	warning("TODO: duck_api SetStates");
}

void SetTraceNow() {
	warning("TODO: duck_api SetTraceNow");
}

void ClearTraceNow() {
	warning("TODO: duck_api ClearTraceNow");
}

} // namespace Spycraft
