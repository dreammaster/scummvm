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

#include "common/file.h"
#include "common/formats/ini-file.h"
#include "gui/debugger.h"
#include "ags2/ac/main.h"
#include "ags2/ac/acruntime.h"
#include "ags2/ac/timer.h"
#include "ags2/common/clib32.h"
#include "ags2/common/routefnd.h"
#include "ags2/lib/allegro/sound.h"
#include "ags2/lib/allegro/timer.h"
#include "ags2/lib/allegro/platform/alwin.h"
#include "ags2/ags2.h"
#include "ags2/vars.h"

namespace AGS2 {

#define AC_SETUP_CFG "acsetup.cfg"

static void parse_command_line(int argc, const char *argv[]);
static void read_config_file();
static void initialize_engine();

void ags_main(int argc, const char *argv[]) {
	parse_command_line(argc, argv);
	if (debug_flags & DBG_REGONLY)
		return;

	platform = AGSPlatformDriver::GetDriver();

	read_config_file();
	initialize_engine();
}

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
		else if (scumm_stricmp(argv[ee], "-registergame") == 0) {
			justRegisterGame = true;
		} else if (scumm_stricmp(argv[ee], "-unregistergame") == 0) {
			justUnRegisterGame = true;
		} else if ((scumm_stricmp(argv[ee], "-loadsavedgame") == 0) && (argc > ee + 1)) {
			loadSaveGameOnStartup = argv[ee + 1];
			ee++;
		} else if ((scumm_stricmp(argv[ee], "--enabledebugger") == 0) && (argc > ee + 1)) {
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
		} else if (argv[ee][0] != '-') {
			datafile_argv = ee;
		}
	}
}

static void read_config_file() {
	Common::INIFile ini;
	if (!ini.loadFromFile(AC_SETUP_CFG))
		return;

	Common::String tmp;
	if (ini.getKey("digiwin", "sound", tmp)) {
		usetup.digicard = atoi(tmp.c_str());
		if (usetup.digicard < 0)
			usetup.digicard = -1;
	}
	if (ini.getKey("midiwin", "sound", tmp)) {
		usetup.midicard = atoi(tmp.c_str());
		if (usetup.midicard < 0)
			usetup.midicard = -1;
	}
	usetup.usevox = ini.getKey("usevox", "sound", tmp) && atoi(tmp.c_str()) != 0;
	usetup.no_speech_pack = ini.getKey("usespeech", "sound", tmp) && atoi(tmp.c_str()) == 0;

	if (ini.getKey("windowed", "misc", tmp))
		usetup.windowed = atoi(tmp.c_str()) != 0;
	if (ini.getKey("screen", "misc", tmp))
		usetup.screenres = atoi(tmp.c_str());

	if (ini.getKey("datadir", "misc", tmp) && !tmp.empty())
		error("Separate data folders not yet supported");
	ini.getKey("translation", "language", usetup.translation);

	if (ini.getKey("cachemax", "misc", tmp)) {
		int tmpInt = atoi(tmp.c_str());
		if (tmpInt > 0)
			spriteset.maxCacheSize = tmpInt * 1024;
	}
}

void initialize_engine() {
	set_uformat(U_ASCII);

	if (install_allegro())
		error("Internal error: unable to initialize stripped Allegro 4 library.");
	game_file_name = scumm_strdup(g_engine->getFilename());

	int errCode = csetlib(game_file_name);
	if (errCode != 0) {
		errCode = csetlib("ac2game.dat");
		if (errCode != 0)
			errCode = csetlib("ac2game.ags");
	}
	if (errCode != 0)
		error("Could not locate game data");

	roomstats = (RoomStatus *)calloc(sizeof(RoomStatus), MAX_ROOMS);
	for (int ee = 0; ee < MAX_ROOMS; ee++) {
		roomstats[ee].beenhere = 0;
		roomstats[ee].numobj = 0;
		roomstats[ee].tsdatasize = 0;
		roomstats[ee].tsdata = NULL;
	}

	play.want_speech = -2;
	if (!usetup.no_speech_pack && Common::File::exists("speech.vox")) {
		if (csetlib("speech.vox"))
			error("Unable to initialize speech sample file - check for corruption "
				"and that it belongs to this game.");

		csetlib(game_file_name);
	}

	play.want_music = 0;
	if (!usetup.usevox && Common::File::exists("music.vox")) {
		if (csetlib("music.vox"))
			error("Unable to initialize music library - check for corruption and that it belongs to this game.");
		csetlib(game_file_name);
		play.want_music = 1;
	}

	if (opts.mod_player)
		reserve_voices(14);

	install_timer();
	set_volume_per_voice(1);

	if (usetup.digicard == -1 || usetup.digicard == DIGI_DIRECTX(0))
		usetup.digicard = DIGI_DIRECTAMX(0);

	if (install_sound(usetup.digicard, usetup.midicard)) {
		reserve_voices(-1, -1);
		opts.mod_player = 0;
		opts.mp3_player = 0;
		if (install_sound(usetup.digicard, usetup.midicard) &&
			usetup.digicard != 0 && usetup.midicard != 0)
			error("Unable to initialize your audio hardware");

		reserve_voices(0, 0);
		install_sound(0, 0);
		usetup.digicard = 0;
		usetup.midicard = 0;
	}

	if ((debug_flags & (~DBG_DEBUGMODE)) > 0) {
		g_engine->getDebugger()->debugPrintf("Engine debugging enabled.\n"
			"\nNOTE: You have selected to enable one or more engine debugging options.\n"
			"These options cause many parts of the game to behave abnormally, and you\n"
			"may not see the game as you are used to it. The point is to test whether\n"
			"the engine passes a point where it is crashing on you normally.\n"
			"[Debug flags enabled: 0x%02X]\n");
		g_engine->getDebugger()->attach();
	}

	init_pathfinder();

	abuf = create_bitmap_ex(8, 10, 10);
	usetup.base_width = 320;
	usetup.base_height = 200;
	screenresIdx = usetup.screenres + 1;

	switch (usetup.screenres) {
	case 1:
		usetup.base_width = 640;
		usetup.base_height = 400;
		break;
	case 2:
		usetup.base_width = 960;
		usetup.base_height = 600;
		break;
	default:
		break;
	}

	scrnwid = usetup.base_width;
	scrnhit = usetup.base_height;
	current_screen_resolution_multiplier_x = scrnwid / 320;
	current_screen_resolution_multiplier_y = scrnhit / 200;

	int width = usetup.base_width;
	int height = usetup.base_height;

	if (usetup.base_width == 960) {
		width = 1024;
		height = 768;
	}

	set_game_speed(40);

	// TODO
}

} // namespace AGS2
