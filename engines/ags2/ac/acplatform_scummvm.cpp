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

#include "common/system.h"
#include "ags2/ac/acplatform.h"

namespace AGS2 {

struct AGSScummVM : AGS32BitOSDriver {

	virtual int  CDPlayerCommand(int cmdd, int datt);
	virtual void Delay(int millis);
	virtual void DisplayAlert(const char *, ...);
	virtual unsigned long GetDiskFreeSpaceMB();
	virtual const char *GetNoMouseErrorString();
	virtual eScriptSystemOSID GetSystemOSID();
	virtual int  InitializeCDPlayer();
	virtual void PlayVideo(const char *name, int skip, int flags);
	virtual void PostAllegroExit();
	virtual int  RunSetup();
	virtual void SetGameWindowIcon();
	virtual void ShutdownCDPlayer();
	virtual void WriteConsole(const char *, ...);
};


int AGSScummVM::CDPlayerCommand(int cmdd, int datt) {
	return cd_player_control(cmdd, datt);
}

void AGSScummVM::DisplayAlert(const char *text, ...) {
	va_list ap;
	va_start(ap, text);
	Common::String msg = Common::String::vformat(text, ap);
	va_end(ap);

	debug("%s", msg.c_str());
}

void AGSScummVM::Delay(int millis) {
	g_system->delayMillis(millis);
}

unsigned long AGSScummVM::GetDiskFreeSpaceMB() {
	// placeholder
	return 100;
}

const char *AGSScummVM::GetNoMouseErrorString() {
	return "This game requires a mouse. You need to configure and setup your mouse to play this game.\n";
}

eScriptSystemOSID AGSScummVM::GetSystemOSID() {
	return eOS_Linux;
}

int AGSScummVM::InitializeCDPlayer() {
	return cd_player_init();
}

void AGSScummVM::PlayVideo(const char *name, int skip, int flags) {
	// do nothing
}

void AGSScummVM::PostAllegroExit() {
	// do nothing
}

int AGSScummVM::RunSetup() {
	return 0;
}

void AGSScummVM::SetGameWindowIcon() {
	// do nothing
}

void AGSScummVM::WriteConsole(const char *text, ...) {
	va_list ap;
	va_start(ap, text);
	Common::String msg = Common::String::vformat(text, ap);
	va_end(ap);

	debug("%s", msg.c_str());
}

void AGSScummVM::ShutdownCDPlayer() {
	// do nothing
}

AGSPlatformDriver *AGSPlatformDriver::GetDriver() {
	if (instance == NULL)
		instance = new AGSScummVM();
	return instance;
}

} // namespace AGS2
