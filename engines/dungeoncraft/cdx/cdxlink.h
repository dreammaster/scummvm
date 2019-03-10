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

#ifndef DUNGEONCRAFT_CDX_CDXLINK_H
#define DUNGEONCRAFT_CDX_CDXLINK_H

namespace DungeonCraft {
namespace CDX {

//////////////////////////////////////////////////////////////////////////////////
// CDXLinkSession Structure
//////////////////////////////////////////////////////////////////////////////////
struct CDXLinkSession
{
	char *m_Name;
	GUID  m_Guid;
	GUID  m_AppGuid;
	DWORD m_Flags;
	DWORD m_MaxPlayers;
	DWORD m_CurrentPlayers;
};

//////////////////////////////////////////////////////////////////////////////////
// CDXLinkPlayer Structure
//////////////////////////////////////////////////////////////////////////////////
struct CDXLinkPlayer
{
	DPID m_ID;
	char *m_Name;
	DWORD m_Flags;
	HANDLE m_Event;
};

//////////////////////////////////////////////////////////////////////////////////
// CDXLink Class
//////////////////////////////////////////////////////////////////////////////////
class CDXAPI CDXLink
{
public:
	CDXLink(void);
	virtual ~CDXLink(void);

	BOOL CreateIPX(void* hWnd);
	BOOL CreateTCPIP(void* hWnd, char* IPAddress);
	BOOL CreateModem(void* hWnd, char* PhoneNo);
	BOOL CreateSerial(void* hWnd);

	BOOL EnumSessions(CDXLinkSession* Sessions, GUID APP_GUID);
	BOOL HostSession(CDXLinkSession* Session);
	BOOL JoinSession(CDXLinkSession* Session);
	BOOL CloseSession(void);

	BOOL EnumPlayers(CDXLinkPlayer* Players, DWORD Flags = 0);
	BOOL CreatePlayer(CDXLinkPlayer* Player);
	BOOL DestroyPlayer(CDXLinkPlayer* Player);

	HRESULT Send(DPID idFrom, DPID idTo, DWORD dwFlags,
							 LPVOID lpData, DWORD dwDataSize);
	HRESULT Receive(LPDPID lpidFrom, LPDPID lpidTo,	DWORD dwFlags,
									LPVOID lpData, LPDWORD lpdwDataSize);

public:
	HRESULT rval;
	LPDIRECTPLAY3A m_DP;
	LPDIRECTPLAYLOBBY2A m_DPLobby;
};

} // End of namespace CDX
} // End of namespace DungeonCraft

#endif
