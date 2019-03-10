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

#ifndef DUNGEONCRAFT_CDX_CDXLOG_H
#define DUNGEONCRAFT_CDX_CDXLOG_H

#include "cdxscreen.h"

namespace DungeonCraft {
namespace CDX {

// CDX Logging Types.  May be combined together to log to different
// areas simultaneously
#define CDXLOG_TYPE_FILE			0x00000001L
#define CDXLOG_TYPE_STDOUT			0x00000002L
#define CDXLOG_TYPE_STDERR			0x00000004L
#define CDXLOG_TYPE_COM1			0x00000008L
#define CDXLOG_TYPE_COM2			0x00000010L
#define CDXLOG_TYPE_LPT1			0x00000020L
#define CDXLOG_TYPE_LPT2			0x00000040L
#define CDXLOG_TYPE_GUIWIN			0x00000080L
#define CDXLOG_TYPE_WINSOCK			0x00000100L
#define CDXLOG_TYPE_DEBUGWIN		0x00000200L

// Logging levels.  More detail as you increase
#define CDXLOG_LEVEL_1				0x00000001L
#define CDXLOG_LEVEL_2				0x00000002L

class CDXLog
{
public:
	CDXLog();
	~CDXLog();

public:
	// write output to the current log for level 0
	void Write(const char *, ...);
	// write output to the current log for level <= 1
	void Write2(const char *, ...);
	// clears the current log
	void Clear();

	void Screen(CDXScreen *);
	CDXScreen *Screen();

	void LogType(const int);
	int LogType();

	void Level(const int);
	int Level();

protected:
	void WriteInternal(const char *);
	void ClearFile();
	void FileWrite(const char *buff);
	void COMWrite(const int, const char *);
	void LPTWrite(const int, const char *);
	void GUIWrite(const char *);

private:
	CDXScreen *m_Screen;
	int m_LogType;
	int m_Level;
	DWORD m_dwStartTime;
	char m_FileName[MAX_PATH];
};

#ifdef CDX_LOG

extern CDXLog g_cdxlog;

// debug logging functions
#define CDXLOG g_cdxlog.Write
#define CDXLOG2 g_cdxlog.Write2
#define CDXLOGSCREEN g_cdxlog.Screen
#define CDXLOGLEVEL g_cdxlog.Level
#define CDXLOGTYPE g_cdxlog.LogType
#define CDXLOGCLEAR g_cdxlog.Clear

#else

// logging is NOT enabled

// just define it and let it go for a non-logging build
//#ifdef _MSC_VER
//#define CDXLOG ((void)0)
//#define CDXLOG2 ((void)0)
//#define CDXLOGSCREEN ((void)0)
//#define CDXLOGLEVEL ((void)0)
//#define CDXLOGTYPE ((void)0)
//#define CDXLOGCLEAR ((void)0)
//#else
#define CDXLOG
#define CDXLOG2
#define CDXLOGSCREEN
#define CDXLOGLEVEL
#define CDXLOGTYPE
#define CDXLOGCLEAR
//#endif          // _MSC_VER

#endif		// #ifdef CDX_LOG

} // End of namespace CDX
} // End of namespace DungeonCraft

#endif		// #ifndef CDXLOG_H
