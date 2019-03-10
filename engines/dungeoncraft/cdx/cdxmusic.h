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

#ifndef CDXMUSIC_H
#define CDXMUSIC_H

//////////////////////////////////////////////////////////////////////////////////
// CDXMusic allows you to load and play MIDI files for the music in your game. 
// CDXMusic currently uses the Windows MCI functions and in terms of performance 
// is quite demanding.
//////////////////////////////////////////////////////////////////////////////////
class CDXAPI CDXMusic
{
public:
	CDXMusic(void *hWnd);
	virtual ~CDXMusic();

	BOOL Play(const char *Filename);
	BOOL Stop(void);
	BOOL Pause(void);
	BOOL Resume(void);
	BOOL Restart(void);

public:
	HWND m_hWnd;				// Window handle
};

//////////////////////////////////////////////////////////////////////////////////
// CDXMusicCd allows you to load and play audio CD files for the music in your game. 
// CDXMusicCd currently uses the Windows MCI functions and in terms of performance 
// is quite demanding.
//////////////////////////////////////////////////////////////////////////////////
class CDXAPI CDXMusicCd
{
public:
	CDXMusicCd(void);
	~CDXMusicCd(void);

	short	Read(void);
	short	GetNumberOfTracks(void)	{ return m_nNumberOfTracks; }
	void  SetNumberOfTracks(short nTracks) { m_nNumberOfTracks = nTracks; }
	short	GetTrackLength(short nTrack);
	void  SetTrackLength(short nTrack, short nNewLength);
	short	GetTotalLength(void);
	void  Play(short nTrack);
	void  Stop(void);

private:
	short	m_nNumberOfTracks;
	short	m_nTrackLength[100];
	MCI_STATUS_PARMS m_MCIStatus;
	MCI_OPEN_PARMS m_MCIOpen;
};

#endif
