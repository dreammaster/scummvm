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

#ifndef CDXMIDI_H
#define CDXMIDI_H

#include "cdx.h"

#ifdef NTDX3

typedef struct CDX_MIDI_TYP
{
	char*	fileName;
	int		id;	              // ID of where the file is in the array
	int		state;            // current state of the midi file
} CDX_MIDI, *CDX_MIDI_PTR;

#else

#include <wchar.h>
#include <direct.h>

#include <dsound.h>
#include <dmksctrl.h>
#include <dmusici.h>
#include <dmusicc.h>
#include <dmusicf.h>

#define MULTI_TO_WIDE( x,y )  MultiByteToWideChar( CP_ACP,MB_PRECOMPOSED, y,-1,x,_MAX_PATH);

typedef struct CDX_MIDI_TYP
{
	IDirectMusicSegment			*m_segment;
	IDirectMusicSegmentState	*m_segstate;
	int							id;				// ID of where the file is in the array
	int							state;			// current state of the midi file
} CDX_MIDI, *CDX_MIDI_PTR;

#endif

#include "cdxsound.h"

#define CDX_NUM_SEGMENTS 64	  // maximum number of midi files that can be loaded
#define MIDI_NULL        0	  // midi has not been loaded
#define MIDI_LOADED      1	  // midi has been loaded, but is not active
#define MIDI_PLAYING     2	  // midi is currently playing
#define MIDI_STOPPED     3	  // midi is current midi, but not playing

class CDXMidi
{
public:
    CDXMidi();
	virtual ~CDXMidi();

	HRESULT Init(HWND hWnd, CDXSound* pCDXDS = NULL);
	HRESULT Shutdown();                                // called in ~CDXMidi

	int     LoadMidi(const char * name);
	HRESULT Play(int MidiID);
	HRESULT Stop(int MidiID);
	HRESULT Remove(int MidiID);
	HRESULT RemoveAll();
	BOOL    IsPlaying(int MidiID);
	HRESULT SetVolume(long Vol);
	long    GetVolume();

	CDX_MIDI                 m_Midi[CDX_NUM_SEGMENTS]; // array of midi's and data
	int                      m_ActiveMidi;             // current id of active midi
	HWND                     m_hWnd;
#ifndef NTDX3
    IDirectMusicPerformance* m_Performance;
	IDirectMusicLoader*      m_Loader;
#endif
};

#endif CDXMIDI_H