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

#ifndef SPYCRAFT_GAME_ACTSOCK_H
#define SPYCRAFT_GAME_ACTSOCK_H

#include "spycraft/afxwin.h"
#include "spycraft/game/actor.h"
#include "spycraft/game/motion.h"

namespace Spycraft {

#define ACTSOCKERRORID                 int

// error code literals (if you add an error code, be sure to update ErrorCodeText[] and ErrorCodeToKeyName[] in WSSOCK.C)
#define ACTSOCKERRID_NOERROR           0
#define ACTSOCKERRID_SUCCESS           0
#define ACTSOCKERRID_CANTSIGNOFF       1
#define ACTSOCKERRID_CANTOPEN          2
#define ACTSOCKERRID_NOTINSTALLED      3
#define ACTSOCKERRID_MISCERROR         4
#define ACTSOCKERRID_TIMEOUT           5
#define ACTSOCKERRID_XFERCOMPLETE      6
#define ACTSOCKERRID_CANTCONNECT       7
#define ACTSOCKERRID_DOWNLOADERROR     8
#define ACTSOCKERRID_PACKETTOOSMALL    9
#define ACTSOCKERRID_NODOWNLOAD        10
#define ACTSOCKERRID_CANTDISCONNECT    11
#define ACTSOCKERRID_CANTSTARTBROWSER  12
#define ACTSOCKERRID_FILENOTFOUND      13


// ACTSOCK_OpenFTP() mode parm literals
#define ACTSOCK_FTPMODE_DOWNLOAD       1
#define ACTSOCK_FTPMODE_UPLOAD         2

// ACTSOCK_GetServiceType() return code literals
#define ACTSOCK_SERVICETYPE_WINSOCK    1
#define ACTSOCK_SERVICETYPE_AOL        2


typedef void *HFTP;

typedef struct tagFTPFileInfo {
	long  LibIndex;
	long  FileIndex;
} FTPFileInfo, *FTPFileInfoPtr;


typedef struct tagACTSOCKPACKET {
	struct tagACTSOCKPACKET *nextptr;

	long len;
	char data[1];
} ACTSOCKPACKET, *ACTSOCKPACKETPTR;


extern ACTSOCKERRORID   ACTSOCK_FTPSetTimeouts(DWORD datatimeoutms, DWORD generaltimeoutms);

extern ACTSOCKERRORID   ACTSOCK_FTPSendDataPacket(HFTP hftp, ACTSOCKPACKETPTR packet, bool lastpacket);
extern ACTSOCKERRORID   ACTSOCK_FTPReceiveDataPacket(HFTP hftp, ACTSOCKPACKETPTR packetptr, int packetsize);
extern ACTSOCKERRORID   ACTSOCK_FTPStartXfer(HFTP hftp);
extern HFTP             ACTSOCK_FTPOpenFile(const char *FTPhost, const char *hostfilepath, int mode);
extern ACTSOCKERRORID   ACTSOCK_FTPCloseFile(HFTP FTPhandle);

extern ACTSOCKERRORID   ACTSOCK_FTPDownloadFile(const char *FTPhost, const char *hostfilepath, const char *clientfilepath, bool statuswindow);
extern ACTSOCKERRORID   ACTSOCK_FTPUploadFile(const char *FTPhost, const char *hostfilepath, const char *clientfilepath, bool statuswindow);

extern ACTSOCKERRORID   ACTSOCK_FTPDownloadDecompressFile(const char *FTPhost, const char *hostfilepath, const char *clientfilepath, bool statuswindow, bool delfile);

extern ACTSOCKERRORID   ACTSOCK_FTPDailyDownload(const char *FTPhost, const char *hostpath, const char *clientpath, bool statuswindow, bool delfile, bool downloadall);

extern ACTSOCKERRORID   ACTSOCK_WWWOpenBrowser(const char *URLname, bool maximized);
extern ACTSOCKERRORID   ACTSOCK_WWWCloseBrowser(void);
extern ACTSOCKERRORID   ACTSOCK_WWWRunBrowser(const char *URLname, bool maximized);
extern ACTSOCKERRORID   ACTSOCK_WWWSetBrowserDir(const char *path);

extern ACTSOCKERRORID   ACTSOCK_StartService(void);
extern ACTSOCKERRORID   ACTSOCK_EndService(void);

extern ACTSOCKERRORID   ACTSOCK_SetParentHWND(HWND hwnd);
extern ACTSOCKERRORID   ACTSOCK_EnableErrorDialogs(bool enable);
extern ACTSOCKERRORID   ACTSOCK_SetUserInfo(const char *UserName, const char *Password, const char *AuthCode);
extern int              ACTSOCK_GetServiceType(void);

} // namespace Spycraft

#endif
