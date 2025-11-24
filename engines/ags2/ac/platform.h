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

#ifndef AGS2_AC_PLATFORM_H
#define AGS2_AC_PLATFORM_H

#define WGT2ALLEGRO_NOFUNCTIONS
#include "ags2/common/wgt2allg.h"
#include "ags2/ac/cc.h"
#define CROOM_NOFUNCTIONS
#include "ags2/ac/room.h"
#include "ags2/ac/sound.h"
#include "ags2/common/cscomp.h"
#include "ags2/gfx/ali3d.h"
#include "ags2/common/agsplugin.h"
#include "ags2/data/defines.h"
#include "ags2/data/character_info.h"
#include "ags2/data/words_dictionary.h"

//if !defined(BSD_VERSION) && (defined(LINUX_VERSION) || defined(WINDOWS_VERSION))
//include "libcda.h"
//endif

namespace AGS2 {

enum {
	GLED_INTERACTION = 1,
	GLED_EFFECTS = 2,
};

enum {
	SCALIGN_LEFT     = 1,
	SCALIGN_CENTRE   = 2,
	SCALIGN_RIGHT    = 3,
	DIALOG_NONE      = 0,
	DIALOG_RUNNING   = 1,
	DIALOG_STOP      = 2,
	DIALOG_NEWROOM   = 100,
	DIALOG_NEWTOPIC  = 12000
};

enum eScriptSystemOSID {
	eOS_DOS = 1,
	eOS_Win = 2,
	eOS_Linux = 3,
	eOS_Mac = 4
};

struct AGSPlatformDriver {
	virtual void AboutToQuitGame();
	virtual void Delay(int millis) = 0;
	virtual void DisplayAlert(const char *, ...) = 0;
	virtual const char *GetAllUsersDataDirectory() {
		return NULL;
	}
	virtual unsigned long GetDiskFreeSpaceMB() = 0;
	virtual const char *GetNoMouseErrorString() = 0;
	virtual eScriptSystemOSID GetSystemOSID() = 0;
	virtual void GetSystemTime(ScriptDateTime *) = 0;
	virtual void PlayVideo(const char *name, int skip, int flags) = 0;
	virtual void InitialiseAbufAtStartup();
	virtual void PostAllegroInit(bool windowed);
	virtual void PostAllegroExit() = 0;
	virtual void FinishedUsingGraphicsMode();
	virtual void ReplaceSpecialPaths(const char *sourcePath, char *destPath);
	virtual int  RunSetup() = 0;
	virtual void SetGameWindowIcon();
	virtual void WriteConsole(const char *, ...) = 0;
	virtual void WriteDebugString(const char *, ...);
	virtual void YieldCPU() = 0;
	virtual void DisplaySwitchOut();
	virtual void DisplaySwitchIn();
	virtual void RegisterGameWithGameExplorer();
	virtual void UnRegisterGameWithGameExplorer();
	virtual int  ConvertKeycodeToScanCode(int keyCode);

	virtual int  InitializeCDPlayer() = 0;  // return 0 on success
	virtual int  CDPlayerCommand(int cmdd, int datt) = 0;
	virtual void ShutdownCDPlayer() = 0;

	virtual void ReadPluginsFromDisk(Common::SeekableReadStream *);
	virtual void StartPlugins();
	virtual int  RunPluginHooks(int event, int data);
	virtual void RunPluginInitGfxHooks(const char *driverName, void *data);
	virtual int  RunPluginDebugHooks(const char *scriptfile, int linenum);
	virtual void ShutdownPlugins();

	static AGSPlatformDriver *GetDriver();

private:
	static AGSPlatformDriver *instance;
};

extern AGSPlatformDriver *platform;

struct AGS32BitOSDriver : AGSPlatformDriver {
	virtual void GetSystemTime(ScriptDateTime *);
	virtual void YieldCPU();
};

void pl_stop_plugins();
void pl_startup_plugins();
int pl_run_plugin_hooks(int event, int data);
void pl_run_plugin_init_gfx_hooks(const char *driverName, void *data);
int pl_run_plugin_debug_hooks(const char *scriptfile, int linenum);
void pl_read_plugins_from_disk(Common::SeekableReadStream *iii);
int cd_player_init();
int cd_player_control(int cmdd, int datt);

extern IGraphicsDriver *gfxDriver;
extern int editor_debugging_enabled;
extern int break_on_next_script_step;
extern void get_current_dir_path(char *buffer, const char *fileName);

} // namespace AGS2

#endif
