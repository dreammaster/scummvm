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

#ifndef SPYCRAFT_DMADE_DUCK_HFB_H
#define SPYCRAFT_DMADE_DUCK_HFB_H

#include "common/scummsys.h"

namespace Spycraft {

/* new and improved A/V Streaming API */

enum dukDirect {             /* direction to move within stream */
	DUK_BACKWARD = -1,         /* move backward from current position */
	DUK_ABSOLUTE = 0,          /* move to an absolute position */
	DUK_FORWARD = 1            /* move forward from current position */
};

enum MFP_FileType {          /* type of file opened */
	DUK_NULFILE = 0,         /* not a TrueMotion "S" A/V file */
	DUK_DUKFILE = 1,         /* TrueMotion "S" duk file */
	DUK_AVIFILE = 2,         /* TrueMotion "S" Video for Windows AVI compatible file */
	DUK_QTFILE = 3           /* TrueMotion "S" Quicktime MOV compatible file */
};

enum MFP_StreamType {        /* stream types */
	DUK_UNDEFINED = 0,       /* indeterminate or uninitialized stream */
	DUK_AUDSTREAM = 1,       /* audio stream */
	DUK_VIDSTREAM = 2,       /* video stream */
	DUK_TXTSTREAM = 3        /* text stream */
};

enum HFB_Modes {             /* HFB buffering modes */
	HFBMODE_NORMAL = 0,      /* normal play once stream mode */
	HFBMODE_FORWARDLOOP = 1, /* forward looping mode (loop back to start) */
	HFBMODE_REVERSE = 2,     /* reverse play once mode (not yet implemented)*/
	HFBMODE_REVERSELOOP = 3, /* reverse looping mode (not yet implemented)*/
	HFBMODE_QUEUE = 4,        /* file queue mode (not yet implemented)*/
	HFBMODE_RANDOM = 5		  /* random play (no Stream) */
};

enum mv_returns {
	DUK_END = 0, DUK_FULL, DUK_MAXREAD, DUK_INCOMPLETE
};

#define HFB_USE_DEFAULT		NULL
#define HFB_RESET_COUNT		-1L
#define HFB_IGNORE_COUNT	-2L

#define HFB_DATA_READ		0x01
#define HFB_DATA_RELEASED	0x02
#define HFB_DATA_KEYFRAME	0x08

enum FTYPE { 					/* 7/20 typedef??*/
	HFB_FRAMENUM = 0,
	HFB_INDEXNUM = 1
};

typedef struct tDKWAVEFORM
{
	unsigned short	wFormatTag;        /* format type */
	unsigned short	nChannels;         /* number of channels (i.e. mono, stereo...) */
	unsigned long   nSamplesPerSec;    /* sample rate */
	unsigned long	nAvgBytesPerSec;   /* for buffer estimation */
	unsigned short	nBlockAlign;       /* block size of data */
	unsigned short	wBitsPerSample;    /* Number of bits per sample of mono data */
	unsigned short	cbSize;            /* The count in bytes of the size of
									extra information (after cbSize) */
	unsigned short	wSamplesPerBlock;

} DKWAVEFORM;

typedef struct tDKBITMAP
{
	unsigned short	usFormatTag;
	unsigned short	usWidth;			/* width */
	unsigned short	usHeight;			/* height */
} DKBITMAP;

typedef struct tMFP_STREAM_INFO {
	enum MFP_StreamType streamType;
	int streamNum;
	long lSampleRate;
	long lStart;
	long lLength;
	char szName[24];
	union {
		DKWAVEFORM WaveformInfo;
		DKBITMAP BitmapInfo;
	} a;
} MFP_STREAM_INFO;

typedef struct tMFP_FILE_INFO {
	long lFileSize;
	long lNumStreams;
	char sName[96];
} MFP_FILE_INFO;

typedef struct tMFPFile *MFP_FILE_HANDLE;
typedef struct tMFPStream *MFP_STREAM_HANDLE;
typedef struct tHFBBuffer *HFB_BUFFER_HANDLE;

typedef int HFB_DATA_HANDLE;

#define DCK_DEFAULT_READSIZE 0
#define DCK_DEFAULT_BUFFSIZE 0

/************************************************************************
 *
 * Multi-Format Parser functions
 *
 ************************************************************************/

 /* initialize Multi-Format Parser */
int MFP_Init(int MaxOpenFiles, int MaxOpenStreams, int MaxOpenBuffers);

/* exit Multi-Format Parser */
void MFP_Exit(void);

/* open a TrueMotion/S A/V file, if bfHnd is NULL no buffering is used */
MFP_FILE_HANDLE MFP_OpenFile(const char *FileName, HFB_BUFFER_HANDLE bfHnd);

/* MFP encapsulates the three following calls
 * Spacing these calls out and using the AVIINDEX.EXE patch
 * avoids having to wait for gfs to complete the tasks */

 /* initiate find file and read header */
MFP_FILE_HANDLE MFP_FindFile(const char *FileName);
/* parse file header and start seek to and load of a file's index */
int MFP_ParseFile(MFP_FILE_HANDLE fHnd, HFB_BUFFER_HANDLE bfHnd);
/* load the index of a file */
void MFP_LoadIndex(MFP_FILE_HANDLE fHnd, HFB_BUFFER_HANDLE bfHnd);


/* close a TrueMotion/S A/V file */
void MFP_CloseFile(MFP_FILE_HANDLE FileHandle);

/* get information about a TrueMotion/S A/V file */
MFP_FILE_INFO *MFP_GetFileInfo(MFP_FILE_HANDLE FileHandle);

/* get a handle to a stream within a TrueMotion/S A/V file by name, number or type*/
MFP_STREAM_HANDLE MFP_GetStream(MFP_FILE_HANDLE FileHandle, const char *StreamName, int StreamNum, unsigned long SType);

/* release a handle to a stream */
void MFP_ReleaseStream(MFP_STREAM_HANDLE StreamHandle);

/* get information about a stream */
MFP_STREAM_INFO *MFP_GetStreamInfo(MFP_STREAM_HANDLE StreamHandle);

/* unbuffered read from a stream, *Data and *MaxLength must be provided */
int MFP_ReadData(MFP_STREAM_HANDLE StreamHandle, void *Data, long *MaxLength, enum dukDirect Direction, int Count);

DKWAVEFORM *MFP_GetAudioInfo(MFP_STREAM_HANDLE aStream, int *lNumChannels, int *lSamplesPerSec, int *lBytesPerSec, int *lwFormat);
int MFP_GetSamplesPerFrame(MFP_STREAM_HANDLE vStream, MFP_STREAM_HANDLE aStream);
void MFP_GetFrameRates(MFP_STREAM_HANDLE vStream, MFP_STREAM_HANDLE aStream, int *lGlobalFrameRate, int *prebuffer);

/************************************************************************
 *
 * High-speed File Buffering functions
 *
 ************************************************************************/

HFB_BUFFER_HANDLE HFB_CreateBuffer(long bSize, long minIndexFrames);
int HFB_ConditionBuffer(HFB_BUFFER_HANDLE Buffer, long bSize, long minIndexFrames);
void HFB_InitBuffer(HFB_BUFFER_HANDLE Buffer, MFP_FILE_HANDLE File, int startFrame, int preload);

void HFB_DestroyBuffer(HFB_BUFFER_HANDLE Buffer);
int HFB_SetBufferMode(HFB_BUFFER_HANDLE Buffer, enum HFB_Modes newMode);

/* open a new TrueMotion/S A/V file and prepare it for reading while playing another file.
 * the original file will close automatically when the last data is released.
 */
MFP_FILE_HANDLE HFB_QueueOpenFile(const char *FileName, HFB_BUFFER_HANDLE bfHnd);

long 	HFB_getDataPosition(MFP_STREAM_HANDLE dckPtr, HFB_DATA_HANDLE chunkIndex);

/* returns percent of CD buffer filled with data	*/
long HFB_GetBufferPerCentFull(HFB_BUFFER_HANDLE dckPtr);

/* incrementally re-use released space within a buffer */
int HFB_FillBuffer(HFB_BUFFER_HANDLE bfHnd, long AmountToFill, long ElapsedFrames);

/* get buffer status */
int HFB_getBufferStatus(HFB_BUFFER_HANDLE dckPtr);

/* get DataHandle, DataPointer and DataLength of a chunk within a buffered file/stream */
HFB_DATA_HANDLE HFB_GetStreamingData(MFP_STREAM_HANDLE StreamHandle, void **DataPointer, long *Length, enum dukDirect Direction, int Count);

/* release/invalidate a chunk of data within a buffered file/stream */
void HFB_ReleaseStreamingData(HFB_BUFFER_HANDLE bfHnd, HFB_DATA_HANDLE DataHandle);

/* get information about a data block */
int HFB_GetIndexFlags(MFP_STREAM_HANDLE sPtr, enum FTYPE format, int frameNum);

/* compute number of frames to next keyframe */
int HFB_FramestoNextKeyFrame(MFP_STREAM_HANDLE sPtr, int ChunkIndex, int *pChunkINdex);

// convert frame number to chunk number
int HFB_FrameToChunk(MFP_STREAM_HANDLE sPtr, int FrameNum);

/* Returns Frame number of KeyFrame before this frame */
int HFB_PreviousKeyFrame(MFP_STREAM_HANDLE sPtr, int FrameNum);

/* play a simple TrueMotion/S A/V file,
 *** TO BE IMPLEMENTED IN PLAYER, NOT IN LIBRARY ***
 */
int HFB_PlayFile(const char *FileName, long bSize, int PreLoad);

void HFB_WalkFlags(HFB_BUFFER_HANDLE bfHnd);
long HFB_GetBufferSpace(HFB_BUFFER_HANDLE dckPtr);
int MFP_GetInitialFrames(MFP_STREAM_HANDLE vStream, MFP_STREAM_HANDLE aStream);

} // namespace Spycraft

#endif
