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
#include "spycraft/dmade/duck_hfb.h"

namespace Spycraft {

int MFP_Init(int MaxOpenFiles, int MaxOpenStreams, int MaxOpenBuffers) {
	return 0;
}

void MFP_Exit(void) {
}

MFP_FILE_HANDLE MFP_OpenFile(const char *FileName, HFB_BUFFER_HANDLE bfHnd) {
	error("TODO: MFP_OpenFile");
}

MFP_FILE_HANDLE MFP_FindFile(const char *FileName) {
	error("TODO: MFP_FindFile");
}

int MFP_ParseFile(MFP_FILE_HANDLE fHnd, HFB_BUFFER_HANDLE bfHnd) {
	error("TODO: MFP_ParseFile");
}

void MFP_LoadIndex(MFP_FILE_HANDLE fHnd, HFB_BUFFER_HANDLE bfHnd) {
	error("TODO: MFP_LoadIndex");
}

void MFP_CloseFile(MFP_FILE_HANDLE FileHandle) {
	error("TODO: MFP_CloseFile");
}

MFP_FILE_INFO *MFP_GetFileInfo(MFP_FILE_HANDLE FileHandle) {
	error("TODO: MFP_GetFileInfo");
}

MFP_STREAM_HANDLE MFP_GetStream(MFP_FILE_HANDLE FileHandle, const char *StreamName, int StreamNum, unsigned long SType) {
	error("TODO: MFP_GetStream");
}

void MFP_ReleaseStream(MFP_STREAM_HANDLE StreamHandle) {
	error("TODO: MFP_ReleaseStream");
}

MFP_STREAM_INFO *MFP_GetStreamInfo(MFP_STREAM_HANDLE StreamHandle) {
	error("TODO: MFP_GetStreamInfo");
}

int MFP_ReadData(MFP_STREAM_HANDLE StreamHandle, void *Data, long *MaxLength, enum dukDirect Direction, int Count) {
	error("TODO: MFP_ReadData");
}

DKWAVEFORM *MFP_GetAudioInfo(MFP_STREAM_HANDLE aStream, int *lNumChannels, int *lSamplesPerSec, int *lBytesPerSec, int *lwFormat) {
	error("TODO: MFP_GetAudioInfo");
}

int MFP_GetSamplesPerFrame(MFP_STREAM_HANDLE vStream, MFP_STREAM_HANDLE aStream) {
	error("TODO: MFP_GetSamplesPerFrame");
}

void MFP_GetFrameRates(MFP_STREAM_HANDLE vStream, MFP_STREAM_HANDLE aStream, int *lGlobalFrameRate, int *prebuffer) {
	error("TODO: MFP_GetFrameRates");
}

HFB_BUFFER_HANDLE HFB_CreateBuffer(long bSize, long minIndexFrames) {
	error("TODO: HFB_CreateBuffer");
}

int HFB_ConditionBuffer(HFB_BUFFER_HANDLE Buffer, long bSize, long minIndexFrames) {
	error("TODO: HFB_ConditionBuffer");
}

void HFB_InitBuffer(HFB_BUFFER_HANDLE Buffer, MFP_FILE_HANDLE File, int startFrame, int preload) {
	error("TODO: HFB_InitBuffer");
}

void HFB_DestroyBuffer(HFB_BUFFER_HANDLE Buffer) {
	error("TODO: HFB_DestroyBuffer");
}

int HFB_SetBufferMode(HFB_BUFFER_HANDLE Buffer, enum HFB_Modes newMode) {
	error("TODO: HFB_SetBufferMode");
}

MFP_FILE_HANDLE HFB_QueueOpenFile(const char *FileName, HFB_BUFFER_HANDLE bfHnd) {
	error("TODO: HFB_QueueOpenFile");
}

long HFB_getDataPosition(MFP_STREAM_HANDLE dckPtr, HFB_DATA_HANDLE chunkIndex) {
	error("TODO: HFB_getDataPosition");
}

long HFB_GetBufferPerCentFull(HFB_BUFFER_HANDLE dckPtr) {
	error("TODO: HFB_GetBufferPerCentFull");
}

int HFB_FillBuffer(HFB_BUFFER_HANDLE bfHnd, long AmountToFill, long ElapsedFrames) {
	error("TODO: HFB_FillBuffer");
}

int HFB_getBufferStatus(HFB_BUFFER_HANDLE dckPtr) {
	error("TODO: HFB_getBufferStatus");
}

HFB_DATA_HANDLE HFB_GetStreamingData(MFP_STREAM_HANDLE StreamHandle, void **DataPointer, long *Length, enum dukDirect Direction, int Count) {
	error("TODO: HFB_GetStreamingData");
}

void HFB_ReleaseStreamingData(HFB_BUFFER_HANDLE bfHnd, HFB_DATA_HANDLE DataHandle) {
	error("TODO: HFB_ReleaseStreamingData");
}

int HFB_GetIndexFlags(MFP_STREAM_HANDLE sPtr, enum FTYPE format, int frameNum) {
	error("TODO: HFB_GetIndexFlags");
}

int HFB_FramestoNextKeyFrame(MFP_STREAM_HANDLE sPtr, int ChunkIndex, int *pChunkINdex) {
	error("TODO: HFB_FramestoNextKeyFrame");
}

int HFB_FrameToChunk(MFP_STREAM_HANDLE sPtr, int FrameNum) {
	error("TODO: HFB_FrameToChunk");
}

int HFB_PreviousKeyFrame(MFP_STREAM_HANDLE sPtr, int FrameNum) {
	error("TODO: HFB_PreviousKeyFrame");
}

int HFB_PlayFile(const char *FileName, long bSize, int PreLoad) {
	error("TODO: HFB_PlayFile");
}

void HFB_WalkFlags(HFB_BUFFER_HANDLE bfHnd) {
	error("TODO: HFB_WalkFlags");
}

long HFB_GetBufferSpace(HFB_BUFFER_HANDLE dckPtr) {
	error("TODO: HFB_GetBufferSpace");
}

int MFP_GetInitialFrames(MFP_STREAM_HANDLE vStream, MFP_STREAM_HANDLE aStream) {
	error("TODO: MFP_GetInitialFrames");
}

} // namespace Spycraft
