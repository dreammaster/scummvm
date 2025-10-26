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

#include "common/str.h"
#include "ags2/ac.h"
#include "ags2/vars.h"
#include "ags2/routefnd.h"

namespace AGS2 {

static void parse_command_line(int argc, const char *argv[]) {
	for (int ee = 1; ee < argc; ee++) {
		if (scumm_stricmp(argv[ee], "-shelllaunch") == 0)
			change_to_game_dir = 1;
		else if (scumm_stricmp(argv[ee], "-updatereg") == 0)
			debug_flags |= DBG_REGONLY;
		else if (scumm_stricmp(argv[ee], "-windowed") == 0)
			force_window = 1;
		else if (scumm_stricmp(argv[ee], "-fullscreen") == 0)
			force_window = 2;
		else if (scumm_stricmp(argv[ee], "-hicolor") == 0)
			force_16bit = 1;
		else if (scumm_stricmp(argv[ee], "-letterbox") == 0)
			force_letterbox = 1;
		else if (scumm_stricmp(argv[ee], "-record") == 0)
			play.recording = 1;
		else if (scumm_stricmp(argv[ee], "-playback") == 0)
			play.playback = 1;
#ifdef _DEBUG
		else if ((scumm_stricmp(argv[ee], "--startr") == 0) && (ee < argc - 1)) {
			override_start_room = atoi(argv[ee + 1]);
			ee++;
		}
#endif
		else if ((scumm_stricmp(argv[ee], "--testre") == 0) && (ee < argc - 2)) {
			Common::strcpy_s(return_to_roomedit, argv[ee + 1]);
			Common::strcpy_s(return_to_room, argv[ee + 2]);
			ee += 2;
		} else if (scumm_stricmp(argv[ee], "--15bit") == 0) debug_15bit_mode = 1;
		else if (scumm_stricmp(argv[ee], "--24bit") == 0) debug_24bit_mode = 1;
		else if (scumm_stricmp(argv[ee], "--fps") == 0) display_fps = 2;
		else if (scumm_stricmp(argv[ee], "--test") == 0) debug_flags |= DBG_DEBUGMODE;
		else if (scumm_stricmp(argv[ee], "-noiface") == 0) debug_flags |= DBG_NOIFACE;
		else if (scumm_stricmp(argv[ee], "-nosprdisp") == 0) debug_flags |= DBG_NODRAWSPRITES;
		else if (scumm_stricmp(argv[ee], "-nospr") == 0) debug_flags |= DBG_NOOBJECTS;
		else if (scumm_stricmp(argv[ee], "-noupdate") == 0) debug_flags |= DBG_NOUPDATE;
		else if (scumm_stricmp(argv[ee], "-nosound") == 0) debug_flags |= DBG_NOSFX;
		else if (scumm_stricmp(argv[ee], "-nomusic") == 0) debug_flags |= DBG_NOMUSIC;
		else if (scumm_stricmp(argv[ee], "-noscript") == 0) debug_flags |= DBG_NOSCRIPT;
		else if (scumm_stricmp(argv[ee], "-novideo") == 0) debug_flags |= DBG_NOVIDEO;
		else if (scumm_stricmp(argv[ee], "-noexceptionhandler") == 0) usetup.disable_exception_handling = 1;
		else if (scumm_stricmp(argv[ee], "-dbgscript") == 0) debug_flags |= DBG_DBGSCRIPT;
		else if (scumm_stricmp(argv[ee], "-registergame") == 0)
		{
			justRegisterGame = true;
		} else if (scumm_stricmp(argv[ee], "-unregistergame") == 0)
		{
			justUnRegisterGame = true;
		} else if ((scumm_stricmp(argv[ee], "-loadsavedgame") == 0) && (argc > ee + 1))
		{
			loadSaveGameOnStartup = argv[ee + 1];
			ee++;
		} else if ((scumm_stricmp(argv[ee], "--enabledebugger") == 0) && (argc > ee + 1))
		{
			Common::strcpy_s(editor_debugger_instance_token, argv[ee + 1]);
			editor_debugging_enabled = 1;
			force_window = 1;
			ee++;
		} else if (scumm_stricmp(argv[ee], "--takeover") == 0) {
			if (argc < ee + 2)
				break;
			play.takeover_data = atoi(argv[ee + 1]);
			strncpy(play.takeover_from, argv[ee + 2], 49);
			play.takeover_from[49] = 0;
			ee += 2;
		} else if (argv[ee][0] != '-') datafile_argv = ee;
	}
}

void ags_main(int argc, const char *argv[]) {
	parse_command_line(argc, argv);

}

} // namespace AGS2
