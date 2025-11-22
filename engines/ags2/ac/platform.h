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
#define CROOM_NOFUNCTIONS
#include "ags2/ac/room.h"
#include "ags2/ac/acruntime.h"
#include "ags2/ac/sound.h"
#include "ags2/common/cscomp.h"
#include "ags2/gfx/ali3d.h"

#include "ags2/common/agsplugin.h"

//if !defined(BSD_VERSION) && (defined(LINUX_VERSION) || defined(WINDOWS_VERSION))
//include "libcda.h"
//endif

namespace AGS2 {

#define MAX_PATH 256

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
