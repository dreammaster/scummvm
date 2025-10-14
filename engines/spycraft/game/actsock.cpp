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

#include "spycraft/game/actsock.h"

namespace Spycraft {

ACTSOCKERRORID   ACTSOCK_FTPSetTimeouts(DWORD datatimeoutms, DWORD generaltimeoutms) { return 0; }

ACTSOCKERRORID   ACTSOCK_FTPSendDataPacket(HFTP hftp, ACTSOCKPACKETPTR packet, bool lastpacket) { return 0; }
ACTSOCKERRORID   ACTSOCK_FTPReceiveDataPacket(HFTP hftp, ACTSOCKPACKETPTR packetptr, int packetsize) { return 0; }
ACTSOCKERRORID   ACTSOCK_FTPStartXfer(HFTP hftp) { return 0; }
HFTP             ACTSOCK_FTPOpenFile(const char *FTPhost, const char *hostfilepath, int mode) { return 0; }
ACTSOCKERRORID   ACTSOCK_FTPCloseFile(HFTP FTPhandle) { return 0; }

ACTSOCKERRORID   ACTSOCK_FTPDownloadFile(const char *FTPhost, const char *hostfilepath, const char *clientfilepath, bool statuswindow) { return 0; }
ACTSOCKERRORID   ACTSOCK_FTPUploadFile(const char *FTPhost, const char *hostfilepath, const char *clientfilepath, bool statuswindow) { return 0; }

ACTSOCKERRORID   ACTSOCK_FTPDownloadDecompressFile(const char *FTPhost, const char *hostfilepath, const char *clientfilepath, bool statuswindow, bool delfile) { return 0; }

ACTSOCKERRORID   ACTSOCK_FTPDailyDownload(const char *FTPhost, const char *hostpath, const char *clientpath, bool statuswindow, bool delfile, bool downloadall) { return 0; }

ACTSOCKERRORID   ACTSOCK_WWWOpenBrowser(const char *URLname, bool maximized) { return 0; }
ACTSOCKERRORID   ACTSOCK_WWWCloseBrowser(void) { return 0; }
ACTSOCKERRORID   ACTSOCK_WWWRunBrowser(const char *URLname, bool maximized) { return 0; }
ACTSOCKERRORID   ACTSOCK_WWWSetBrowserDir(const char *path) { return 0; }

ACTSOCKERRORID   ACTSOCK_StartService(void) { return 0; }
ACTSOCKERRORID   ACTSOCK_EndService(void) { return 0; }

ACTSOCKERRORID   ACTSOCK_SetParentHWND(HWND hwnd) { return 0; }
ACTSOCKERRORID   ACTSOCK_EnableErrorDialogs(bool enable) { return 0; }
ACTSOCKERRORID   ACTSOCK_SetUserInfo(const char *UserName, const char *Password, const char *AuthCode) { return 0; }
int              ACTSOCK_GetServiceType(void) { return 0; }

} // namespace Spycraft
